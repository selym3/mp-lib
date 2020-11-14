#include "../../containers/mp_vector.h"
#include <stdlib.h>

mp_vector mpv_create()
{
    mp_vector out = {NULL, 0, __MP_VECTOR_DEFAULT_SIZE__};
    value_t *array = calloc(out.capacity, sizeof(value_t));
    out.array = array;
    return out;
}

int mpv_is_empty(mp_vector * v) {return v->length <= 0;}
int mpv_is_valid(mp_vector * v) {return v->array != NULL; }

void mpv_resize(mp_vector *v, size_t new_capacity)
{
    value_t *new_array = calloc(new_capacity, sizeof(value_t));

    int i = 0;
    for (; i < v->length && i < new_capacity; ++i)
        new_array[i] = v->array[i];

    free(v->array);
    v->array = new_array;
    v->capacity = new_capacity;
    v->length = i;
}

void mpv_clear(mp_vector *v)
{
    value_t r = {0}; // get zeroed value
    for (; v->length > 0; v->array[--v->length] = r);
}

void mpv_free(mp_vector *v)
{
    free(v->array);
    v->array = NULL;
    v->length = 0;
    v->capacity = 0;
}

void mpv_push_back(mp_vector *v, value_t value)
{
    if (v->length >= v->capacity)
        mpv_resize(v, (v->capacity * (__MP_VECTOR_GROWTH_RATE__)));
    v->array[v->length++] = value;
}

value_t mpv_pop_back(mp_vector *v)
{
    if (v->length > 0)
    {
        value_t * loc = v->array + (--v->length);
        value_t out = *loc;
        *loc = (value_t){ 0 }; // zero out value
        return out;
    }

    return (value_t){ 0 };
}

value_t * mpv_get(mp_vector * v, size_t index)
{   
    // make sure null is returned if out of bounds
    // technically cant be lower than zero
    if (index >= v->length || v->length == 0 || index < 0)
        return NULL;
    
    return (v->array + index);
}
value_t * mpv_front(mp_vector * v)
{
    return mpv_get(v, 0);
}
value_t * mpv_back(mp_vector * v)
{
    return mpv_get(v, v->length - 1);
}

int mpv_set(mp_vector * v, size_t index, value_t value)
{
    value_t * loc = mpv_get(v, index);

    if (!loc)
        return 0;
    
    *loc = value;
    return 1;
}