#include <stdio.h>
#include "vector.h"

void printVector(vector_t *v)
{
    if (v != NULL)
    {
        for (int i = 0; i < v->size; i++)
        {
            printf("vbuffer: [i: %u, element ptr: %p, element deref: %d], Size: %zu, Capacity: %zu\n",
                   i, &v->vbuffer[i], *(int *) v->vbuffer[i], v->size, v->capacity);
            printf("actual size in bytes %zu\n", (v->size) * sizeof(void *));
            printf("total size in bytes %zu\n", v->capacity * sizeof(void *));
        }
    }
    else
    {
        printf("Attempt to print NULL vector.\n");
    }
}

void vector_init(vector_t *vector)
{
    if (vector != NULL)
    {
        vector->size = (size_t) 0;
        vector->capacity = (size_t) 10;
        vector->vbuffer = malloc(sizeof(void*) * vector->capacity);
        if (vector->vbuffer == 0)
        {
            printf("Failed to allocate memory.\n");
        }
    }
}

void vector_delete(vector_t *vector)
{
    if (vector != NULL)
    {
        free(vector->vbuffer);
        vector->capacity = 0;
        vector->size = 0;
    }
}

void vector_push(vector_t *vector, void *value)
{
    if (vector != NULL && value != NULL)
    {
        if (vector->size >= vector->capacity)
        {
            size_t newCapacity = vector->capacity * 2;

            if (newCapacity <= vector->capacity)
            {
                printf("vector overflow.\n");
                exit(1);
            }

            vector->vbuffer = realloc(vector->vbuffer, sizeof(void *) * newCapacity);

            if (vector->vbuffer == NULL)
            {
                printf("failed to allocate memory for vector resize.\n");
            }

            vector->capacity = newCapacity;
        }
        vector->vbuffer[vector->size] = value;
        vector->size++;
    }
}

void *vector_pop(vector_t *vector)
{
    if (vector != NULL &&
        vector->size >= 1)
    {
        void *element = vector->vbuffer[vector->size -
                                        1];
        vector->size = vector->size - 1;
        return element;
    }
}

size_t vector_size(const vector_t *vector)
{
    if (vector != NULL)
        return vector->size;
    else
    {
        exit(1);
    }
}

size_t vector_capacity(const vector_t *vector)
{
    if (vector != NULL)
        return vector->capacity;
    else
    {
        printf("Vector is null or not initialized.\n");
        exit(1);
    }
}

void *vector_get_element(const vector_t *vector, size_t index)
{
    if (vector != NULL &&
        index < vector->size)
        return vector->vbuffer[index];
    else
    {
        return NULL;
    }
}

void **vector_get_array(const vector_t *vector)
{
    if (vector != NULL)
        return vector->vbuffer;
    }
}