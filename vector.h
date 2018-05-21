#ifndef FUSE_VECTOR_H
#define FUSE_VECTOR_H

#include <stdlib.h>

typedef struct vector_t {
    size_t size;
    size_t capacity;
    void **vbuffer;
} vector_t;

void vector_init(vector_t *vector);

void vector_delete(vector_t *vector);

void vector_push(vector_t *vector, void *value);

void *vector_pop(vector_t *vector);

size_t vector_size(const vector_t *vector);

size_t vector_capacity(const vector_t *vector);

void *vector_get_element(const vector_t *vector, size_t index);

void **vector_get_array(const vector_t *vector);

#endif