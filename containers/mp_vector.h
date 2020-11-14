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

mp_vector mpv_create();

void mpv_resize(mp_vector *, size_t);
void mpv_clear(mp_vector *);
void mpv_free(mp_vector *);

void mpv_push_back(mp_vector *, value_t);
value_t mpv_pop_back(mp_vector *);

value_t *mpv_get(mp_vector *, size_t);
value_t *mpv_front(mp_vector *);
value_t *mpv_back(mp_vector *);

int mpv_set(mp_vector *, size_t, value_t);

#endif