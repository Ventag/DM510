#ifndef FUSE_INODE_H
#define FUSE_INODE_H

#include "vector.h"

typedef struct inode
{
    time_t changed;
    time_t modified;
    time_t accessed;
    size_t size;
    char* name;
    char* data;

    vector_t items;
    int directory;
} inode;

#endif //FUSE_INODE_H
