#include <stdio.h>
#include "vector.h"

void printVector(vector_t *v)
{
    if (v != NULL)
    { //check if null before printing as printf with %i and others will result in undefined behaviour
        for (int i = 0; i < v->size; i++)
        {
            printf("vbuffer: [i: %u, element ptr: %p, element deref: %d], Size: %zu, Capacity: %zu\n",
                   i, &v->vbuffer[i], *(int *) v->vbuffer[i], v->size, v->capacity);
            printf("actual size in bytes %zu\n", (v->size) * sizeof(void *)); //This is the actual size in bytes, the other is the spare room in bytes.
            printf("total size in bytes %zu\n", v->capacity * sizeof(void *)); //just for extra debugging.
        }
    }
    else
    {
        printf("Attempt to print NULL vector.\n");
    }
}

/*
* Initialize a vector to be empty
* Pre: 'vector' != NULL
* Initialize the vector.
* allocate and assign memory for the array and assign it's elements.
*/
void vector_init(vector_t *vector)
{
    if (vector != NULL)
    {
        vector->size = (size_t) 0; //reset size to 0;
        vector->capacity = (size_t) 10; // set the initial capacity to 10;
        vector->vbuffer = malloc(sizeof(void*) * vector->capacity); //allocate memory on the heap.
        if (vector->vbuffer == 0)
        {
            printf("Failed to allocate memory.\n");
        } //failsafe for malloc, in case memory is full etc.
    }
}

// Deallocate internal structures of the vector
// Note: the user is responsible for deleting all values
// Note: the user is responsible for deleting the actual vector if it was dynamically allocated
void vector_delete(vector_t *vector)
{
    if (vector != NULL)
    {
        free(vector->vbuffer);   // free the allocated memory
        vector->capacity = 0;    // reset capacity
        vector->size = 0;        // reset size
    }
}

// Insert a new element at the end of the vector
// Pre: 'vector' != NULL
void vector_push(vector_t *vector, void *value)
{
    if (vector != NULL && value != NULL)
    { // make sure the passed in parameters are valid
        if (vector->size >= vector->capacity)
        {
            size_t newCapacity = vector->capacity * 2; //resize 2n

            if (newCapacity <= vector->capacity)
            {
                printf("vector overflow.\n");
                exit(1);
            } // handle overflowing, if vector is deleted and someone pushes into it, this will trigger.

            vector->vbuffer = realloc(vector->vbuffer, sizeof(void *) * newCapacity); // realloc is nice :)

            if (vector->vbuffer == NULL)
            {
                printf("failed to allocate memory for vector resize.\n");
            } // handle realloc failure.

            vector->capacity = newCapacity; //update to the new capacity
        }
        vector->vbuffer[vector->size] = value; // update the last element in the vector to the value given.
        vector->size++; // increment the size for future pushes.
    }
}

// Remove the last element in the vector and return the value
// Pre: the vector is non-empty, 'vector' != NULL
void *vector_pop(vector_t *vector)
{
    if (vector != NULL &&
        vector->size >= 1)
    { // first we check if the vector is not null and that the size of the vector is valid.
        void *element = vector->vbuffer[vector->size -
                                        1]; // put void * element on the stack and retrieve the last element from our buffer and assign it to element.
        vector->size = vector->size - 1; //decrement the size since we've popped it.
        return element; // return to user.
    }
}

// Return the number of elements in the vector
// Pre: 'vector' != NULL
size_t vector_size(const vector_t *vector)
{
    if (vector != NULL) //check for null
        return vector->size; // return to user.
    else
    {
        exit(1);
    }
}

// Return the current capacity of the vector
// Pre: 'vector' != NULL
size_t vector_capacity(const vector_t *vector)
{
    if (vector != NULL) //check for null
        return vector->capacity; //return to user
    else
    {
        printf("Vector is null or not initialized.\n");
        exit(1);
    }
}

// Return the value at the given index
// Pre: index < vector_size(vector)
void *vector_get_element(const vector_t *vector, size_t index)
{
    if (vector != NULL &&
        index < vector->size) // check if the vector is null, and the given index is less than the vector size.
        return vector->vbuffer[index]; //return the given index in the buffer.
    else
    {
        return NULL;
    }
}

// Return a pointer to the underlying array
void **vector_get_array(const vector_t *vector)
{
    if (vector != NULL)
    { // Not explicitly stated as a prerequisite. Should still check this though.
        return vector->vbuffer; // return the actual.
    }
}