#ifndef FUSE_VECTOR_H
#define FUSE_VECTOR_H

#include <stdlib.h>

//-----------------------------------------------------------------------------
// Vector, a dynamic array using size doubling to achieve amortized constant time insertion
// There are prerequisites mentions in each function description. The user has to make sure
// that these conditions are fulfilled before the function call. Nevertheless, the function
// should check these conditions as well and react accordingly (e.g., exit the program)
//-----------------------------------------------------------------------------

// actually define the structs
typedef struct vector_t {
    size_t size;     // The size denotes the amount of elements currently occupied in the array. size_t was used because vector_capacity returns a size_t.  
    size_t capacity; // The capacity denotes the maximum length of the array.
    void **vbuffer;  // the vector buffer, this is our array and here we store our elements. Decided to use void** because that makes the most sense.
} vector_t;

// Initialize a vector to be empty
// Pre: 'vector' != NULL
void vector_init(vector_t *vector);

// Deallocate internal structures of the vector
// Note: the user is responsible for deleting all values
// Note: the user is responsible for deleting the actual vector if it was dynamically allocated
void vector_delete(vector_t *vector);

// Insert a new element at the end of the vector
// Pre: 'vector' != NULL
void vector_push(vector_t *vector, void *value);

// Remove the last element in the vector and return the value
// Pre: the vector is non-empty, 'vector' != NULL
void *vector_pop(vector_t *vector);

// Return the number of elements in the vector
// Pre: 'vector' != NULL
size_t vector_size(const vector_t *vector);

// Return the current capacity of the vector
// Pre: 'vector' != NULL
size_t vector_capacity(const vector_t *vector);

// Return the value at the given index
// Pre: index < vector_size(vector)
void *vector_get_element(const vector_t *vector, size_t index);

// Return a pointer to the underlying array
void **vector_get_array(const vector_t *vector);

#endif