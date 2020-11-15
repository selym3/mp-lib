#include "../../containers/mp_hashmap.h"

#include <stdlib.h>

mp_hashmap mphmp_create(hash_t (*to_hash)(key_t))
{
    mp_hashmap out = { 
        NULL, // bucket list
        __MP_HASHMAP_DEFAULT_BUCKETS__, // number of buckets 
        0, // number of used buckets
        __MP_HASHMAP_DEFAULT_BUCKET_LOAD__, // bucket load (const)
        0, // number of elements
        to_hash // hashing function
    };

    mp_hashmap_node ** new_buckets = calloc(out.total_buckets, sizeof(mp_hashmap_node *));

    out.buckets = new_buckets;

    return out;


}

int mphm_is_empty(mp_hashmap * map) { return map->length <= 0; }
int mphm_is_valid(mp_hashmap * map) { return map->buckets != NULL; }

static inline int get_index(mp_hashmap * map, key_t key)
{
    return map->to_hash(key) & (map->total_buckets-1);
}

void mphm_clear(mp_hashmap * map)
{
    for (int i = 0; i < map->total_buckets; ++i)
    {
        while(map->buckets[i])
        {
            mp_hashmap_node * next = map->buckets[i]->next;
            free(map->buckets[i]);
            map->buckets[i] = next;
        }
    }

    map->length = 0;
    map->used_buckets = 0;
}

void mphm_free(mp_hashmap * map)
{
    mphm_clear(map);
    free(map->buckets);
    map->buckets = NULL;
    map->total_buckets = 0;
}

static mp_hashmap_node * create_node(mp_hashmap_node * next, key_t key, value_t value)
{
    mp_hashmap_node * node = malloc(sizeof(mp_hashmap_node));
    node->key = key;
    node->value = value;
    node->next = next;
    return node;
}  

int mphmp_put(mp_hashmap * map, key_t key, value_t value)
{
    const int index = get_index(map, key);
    ++map->length;
    mp_hashmap_node * top = map->buckets[index];
    map->used_buckets += (!top);
    while (top)
    {
        if (top->key == key)
        {
            top->value = value;
            return 0;
        }
        top = top->next;
    }

    // need to create new bucket node if we get here
    mp_hashmap_node * new_node = create_node(map->buckets[index], key, value);
    map->buckets[index] = new_node;

    return 1;
}

value_t * mphm_get(mp_hashmap * map, key_t key)
{
    const index = get_index(map, key);
    mp_hashmap_node * top = map->buckets[index];
    while (top)
    {
        if (top->key == key)
            return &top->value;

        top = top->next;
    }
    return NULL;
}

int mphm_contains(mp_hashmap * map, key_t key) { return mphm_get(map, key) != NULL; }

int mphm_contains_value(mp_hashmap * map, value_t value)
{
    for (int i = 0; i < map->total_buckets; ++i)
    {
        mp_hashmap_node * top = map->buckets[i];
        while (top)
        {
            if (top->value == value)
                return 1;

            top = top->next;
        }
    }
    return 0;
}

int mphm_remove(mp_hashmap * map, key_t key)
{
    const index = get_index(map, key);
    mp_hashmap_node * top = map->buckets[index];
    mp_hashmap_node * before = NULL;
    while (top)
    {
        mp_hashmap_node * next = top->next;

        if (top->key == key)
        {
            // if we're NOT removing the first node
            if (before)
                before->next = next;
            // if we're removing the first node
            else 
                map->buckets[index] = next;

            free(top);
            top = NULL;

            if (!(map->buckets[index]))
                --map->used_buckets;

            --map->length;

            return 1;
        }

        before = top;
        top = next;
    }

    return 0;
}

int mphm_remove_value(mp_hashmap * map, value_t value)
{
    int old_length = map->length;

    for (int i = 0; i < map->total_buckets; ++i)
    {
        mp_hashmap_node * top = map->buckets[i];
        mp_hashmap_node * before = NULL;
        while (top)
        {
            mp_hashmap_node * next = top->next;

            if (top->value == value)
            {
                if (before)
                    before->next = next;
                else    
                    map->buckets[i] = next;

                free(top);
                top = NULL;

                --map->length;
            }
 
            before = top;
            top = next;
        }

        if (!map->buckets[i])
            --map->used_buckets;
    }

    return old_length - map->length;
}