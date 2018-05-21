#include <fuse.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "inode.h"

int lfs_getattr(const char *, struct stat *);

int lfs_readdir(const char *, void *, fuse_fill_dir_t, off_t, struct fuse_file_info *);

int lfs_mknod (const char *, mode_t, dev_t);

int lfs_mkdir(const char*, mode_t);

int lfs_rmdir(const char*, mode_t);

int lfs_open(const char *, struct fuse_file_info *);

int lfs_read(const char *, char *, size_t, off_t, struct fuse_file_info *);

int lfs_write(const char*, const char*, size_t, off_t, struct fuse_file_info *);

int lfs_release(const char *path, struct fuse_file_info *fi);

inode* root;

static struct fuse_operations lfs_oper = {
        .getattr    = lfs_getattr,
        .readdir    = lfs_readdir,
        .mknod = lfs_mknod,
        .mkdir = lfs_mkdir,
        .unlink = lfs_rmdir,
        .rmdir = lfs_rmdir,
        .truncate = NULL,
        .open    = lfs_open,
        .read    = lfs_read,
        .release = lfs_release,
        .write = lfs_write,
        .rename = NULL,
        .utime = NULL
};

inode* make_node(char* name, char* data, int directory)
{
    if(strlen(name) > 20)
    {
        printf("node name was too long\n");
        return NULL;
    }

    inode* node;
    node = malloc(sizeof(inode));
    vector_init(&node->items);
    node->name = name;
    node->index = 0;
    if(directory == 0)
    {
        node->data = malloc(strlen(data));
        sprintf(node->data, "%s", data);
        node->size = strlen(data) + 1;
    }
    else
    {
        node->size = 0;
        node->data = NULL;
    }
    node->directory = directory;
    node->changed = time(NULL);
    node->accessed = time(NULL);
    node->modified = time(NULL);

    return node;
}

void init()
{
    root = make_node("/", 0, 1);

    inode* n1 = make_node("test", NULL, 1);
    inode* n2 = make_node("test1", NULL, 1);
    inode* n3 = make_node("filetest.txt", "1223456778121gabe you are VERY gay", 0);

    vector_push(&root->items, n1);
    vector_push(&root->items, n2);
    vector_push(&root->items, n3);

    printf("initialized system\n");
}

int get_depth(const char *path)
{
    int i = 0;
    char *p;
    p = path;
    while(*p != '\0') {
        if(*p == '/') i++;
        p++;
    }
    return i;
}

char* get_filename_at(int depth, const char *path)
{
    int i = 0, j = 0;
    char *p, *r;
    p = path;
    while(i <= depth) {
        if(p[j] == '/' || p[j] == '\0') {
            i++;
        };
        j++;
    }
    i = j;
    while(p[j] != '/' && p[j] != '\0') j++;
    p[j] = '\0';
    r = &p[i];
    return r;
}

char* get_filename(const char *path)
{
    char *p;
    p = path;
    while(*p != '\0') p++;
    while(*p != '/') p--;
    p++;
    return p;
}

inode* find_inode_in_vector(inode *node, const char *name)
{
    size_t size = vector_size(&node->items);

    for(size_t i = 0; i < size; i++)
    {
        inode* n = (inode*)vector_get_element(&node->items, i);
        if(n)
            if (strcmp(n->name, name) == 0)
                return n;
    }
    return NULL;
}


inode* get_inode(char* path)
{
    char* p;
    inode* node = root;
    if (strcmp(path, "/") == 0)
        return node;

    int d = get_depth(path);

    for ( int i = 0; i < d; i++)
    {
        p = strdup(get_filename_at(i, path));
        node = find_inode_in_vector(node, p);
    }
    return node;
}

int lfs_getattr(const char *path, struct stat *stbuf)
{
    int res = 0;
    printf("getattr: (path=%s)\n", path);

    memset(stbuf, 0, sizeof(struct stat));
    inode* node = get_inode(path);

    if( strcmp( path, "/" ) == 0 )
    {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
        return 0;
    }
    else if( node != NULL && node->directory)
    {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
    }
    else if(node != NULL)
    {
        stbuf->st_mode = S_IFREG | 0777;
        stbuf->st_nlink = 1;
        stbuf->st_size = node->size;
        stbuf->st_ctim.tv_sec = node->changed;
        stbuf->st_mtim.tv_sec = node->modified;
        stbuf->st_atim.tv_sec = node->accessed;
    }
    else
        res = -ENOENT;

    return res;
}

int lfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
    (void) offset;
    (void) fi;
    printf("readdir: (path=%s)\n", path);

    inode* node;

    if ((node = get_inode(path)) == NULL) return -ENOENT;

    filler(buf, ".", NULL,0);
    filler(buf, "..", NULL,0);

    for( int i = 0; i < vector_size(&node); i++)
    {
        inode* n = (inode*)vector_get_element(&node->items, i);
        if(n == NULL)
            break;

        filler(buf, n->name, NULL, 0);
    }

    return 0;
}

int lfs_mknod(const char* path, mode_t mode, dev_t dev)
{
    char *r = strdup(path);
    char *p = path;
    int i = 0, j = 0, depth = get_depth(path);
    if ( depth > 1 ) {
        while(i < depth) {
            if(p[j] == '/') {
                i++;
            };
            j++;
        }
        p[j-1] = '\0';
    } else {
        p = "/";
    }

    printf("p = %s\n", p);

    inode* n;
    n = get_inode(p);
    printf("MKNOD getting inode = %s \n", n->name);
    printf("get_filename = %s\n", get_filename(r));
    inode* node = make_node(get_filename(r), "\0", 0);
    printf("made new inode\n");
    vector_push(&n->items, node);
    printf("pushed node\n");
    return 0;
}

int lfs_mkdir(const char* path, mode_t t)
{
    char *r = strdup(path);
    char *p = path;
    int i = 0, j = 0, depth = get_depth(path);
    if ( depth > 1 ) {
        while(i < depth) {
            if(p[j] == '/') {
                i++;
            };
            j++;
        }
        p[j-1] = '\0';
    } else {
        p = "/";
    }

    printf("p = %s\n", p);

    inode* n;
    n = get_inode(p);
    printf("MKDIR getting inode = %s \n", n->name);
    printf("get_filename = %s\n", get_filename(r));
    inode* node = make_node(get_filename(r), 0, 1);
    vector_push(&n->items, node);
    return 0;
}

int lfs_rmdir(const char* path, mode_t t)
{
    char *r = strdup(path);
    char *p = path;
    int i = 0, j = 0, depth = get_depth(path);
    if ( depth > 1 ) {
        while(i < depth) {
            if(p[j] == '/') {
                i++;
            };
            j++;
        }
        p[j-1] = '\0';
    } else {
        p = "/";
    }

    printf("p = %s\n", p);

    inode *n;
    char* name = get_filename(r);
    n = get_inode(p);
    printf("RMDIR getting inode = %s \n", n->name);
    printf("get_filename = %s\n", name);
    vector_t new;
    vector_init(&new);

    for(int i = 0; i < vector_size(&n->items); i++)
    {
        inode* node = (inode*)vector_get_element(&n->items, i);
        if(node)
        {
            if (strcmp(node->name, name) != 0)
                vector_push(&new, node);
        }
    }

    vector_delete(&n->items);
    n->items = new;
    return 0;
}

//Permission
int lfs_open(const char *path, struct fuse_file_info *fi)
{
    printf("open: (path=%s)\n", path);
    inode* node = get_inode(path);
    if(!node || node->directory)
        return -ENOENT;

    if(strcmp(node->name, get_filename(path)) == 0)
    {
        fi->fh = (uint64_t)node;
        return 0;
    }

    return -ENOENT;
}

int lfs_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    printf("[CALLING lfs_read]\n");
    inode* node = get_inode(path);//(inode*)fi->fh;
    sprintf(buf, node->data, size);
    return strlen(node->data);
}

int lfs_write(const char* path, const char* buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    inode* node = get_inode(path);
    free(node->data);
    node->data = malloc(size);
    node->size = size;
    memcpy(node->data, buf, node->size);

    printf("size: %d - node size: %d\n", size, node->size);
    return size;
}

int lfs_release(const char *path, struct fuse_file_info *fi)
{
    printf("release: (path=%s)\n", path);

    inode* node = (inode*)fi->fh;
    if(node)
    {
        fi->fh = NULL;
        printf("released node %s\n", node->name);
        return 0;
    }

    return -ENOENT;
}

int main(int argc, char *argv[])
{
    init();
    fuse_main(argc, argv, &lfs_oper);

    return 0;
}