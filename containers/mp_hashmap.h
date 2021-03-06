#ifndef __MP_HASHMAP__
#define __MP_HASHMAP__

#include <stddef.h>
#include <stdint.h>

/*
PROBLEM:
    * The hashmap does not own the items added to it
    * When using put(...), the key and the value should be copied and pointers to that 
      should be stored in the linked list. When clearing, these pointers should be freed.
    * This allows types like 'char *' to be safely added and freed 
*/

typedef uint64_t hash_t;

// must be a power of 2
#define __MP_HASHMAP_DEFAULT_BUCKETS__ (16)
#define __MP_HASHMAP_DEFAULT_BUCKET_LOAD__ (0.90)

typedef char * mkey_t;
typedef int value_t;

typedef struct __MP_HASHMAP_NODE__ 
{
    struct __MP_HASHMAP_NODE__ * next;
    
    mkey_t key;
    value_t value;

} mp_hashmap_node;

typedef struct __MP_HASHMAP__
{   
    /*
    An array of pointers to the heads of singly linked lists.
    Each node of these singly linked lists contains a key, value pair.

    This also allows a bucket value to be NULL, so it is known when the 
    bucket is not in use.
    */
    mp_hashmap_node ** buckets;

    // Total buckets allocated
    size_t total_buckets;

    // Buckets that are being used
    size_t used_buckets;

    // If (used_buckets/total_buckets) >= max_bucket_load, 
    // allocate more buckets
    const float max_bucket_load;

    // Total number of elements
    size_t length;

    // Hash function
    hash_t (*to_hash)(const mkey_t *);

    // Key Equals
    int (*key_equals)(const mkey_t *, const mkey_t *);

    // Value Equals
    int (*value_equals)(const value_t *, const value_t *);

} mp_hashmap;

/* 
Create a hashmap given a hashing function for key type.
*/
mp_hashmap mphm_create(
    hash_t (*to_hash)(const mkey_t *),
    int (*key_equals)(const mkey_t *, const mkey_t *),
    int (*value_equals)(const value_t *, const value_t *)
);

// Returns if a hasmap is empty (has any elements)
int mphm_is_empty(mp_hashmap *);

// Returns if a hashmap is valid (buckets aren't null)
int mphm_is_valid(mp_hashmap *);

// Clears all the buckets present
void mphm_clear(mp_hashmap *);

// Frees (and invalidates) a map
void mphm_free(mp_hashmap *);

/*
Get a pointer to a value for a given key.
NULL if key not present
*/
value_t * mphm_get(mp_hashmap *, mkey_t);

/*
Check if a hashmap contains a key.
*/
int mphm_contains(mp_hashmap *, mkey_t);

/*
Check if a hashmap contains a value.
*/
int mphm_contains_value(mp_hashmap *, value_t);

/*
Puts a value in a given key. 

This is the only function that allows the addition
of key value pairs.

Returns 1 if the key had not been present.
*/
int mphm_put(mp_hashmap *, mkey_t, value_t);

/*
Removes a key, value pair based on its key.

Returns 1 if the key was present and was removed
*/
int mphm_remove(mp_hashmap *, mkey_t);

/*
Removes key value pairs based on their values.

Returns 0 if no pairs were removed. Returns the number
of pairs that were removed otherwise.
*/
int mphm_remove_value(mp_hashmap *, value_t);

/*
Given a map and a new bucket count, setup the buckets
based on the given map's current key, value pairs.

The given map's current data will be freed and replaced
with the new length, used & total buckets, and bucket array.
*/
void mphm_resize(mp_hashmap *, size_t);

#endif