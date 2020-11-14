#ifndef __MP_VECTOR__
#define __MP_VECTOR__

#include <stddef.h>

#define __MP_VECTOR_GROWTH_RATE__ (2)
#define __MP_VECTOR_DEFAULT_SIZE__ (16)

typedef int value_t;

typedef struct __MP_VECTOR__
{
    value_t *array;
    size_t length;
    size_t capacity;
} mp_vector;

/*
Initializes a vector. Any vector created with this function
must be freed.
*/
mp_vector mpv_create();

/*
Creates a new block of memory given a new size. Copies as many
values as possible from the given vector. Then, frees the
vector's old values and makes the vector point to the new 
block of memory.
*/
void mpv_resize(mp_vector *, size_t);

/*
Loops through all set values and zeroes them out. Capacity
remains the same and length is set to zero.
*/
void mpv_clear(mp_vector *);

/*
Frees a vector's block of memory and invalidates it.
*/
void mpv_free(mp_vector *);

/*
Checks if a vector's length is above zero.
*/
int mpv_is_empty(mp_vector *);

/*
Checks if a vector is still valid (array is not null)
*/
int mpv_is_valid(mp_vector *);

/*
If the length is equal to the capacity, resize before adding
a new element. Length is incremented appropiately.
*/
void mpv_push_back(mp_vector *, value_t);

/*
If the list isn't empty, find it's last element, store the value
locally, zero the element in memory, and return the last element.
Length is decremented appropiately.

If the list is empty, return the zero value for the value type.
*/
value_t mpv_pop_back(mp_vector *);

/*
Return a pointer to an element given the index. 

NULL pointer if index out of bounds.
*/
value_t *mpv_get(mp_vector *, size_t);

/*
Return a pointer to the front of the list.

NULL if empty.
*/ 
value_t *mpv_front(mp_vector *);

/*
Return a pointer to the back of the list.

NULL if empty.
*/
value_t *mpv_back(mp_vector *);

/*
Sets a given index to a given value.

Returns 1 if value was changed, 0 if the value
could not be changed because the index did not exist.
*/
int mpv_set(mp_vector *, size_t, value_t);

#endif