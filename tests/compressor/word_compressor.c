/**
 * This file assumes that mp_hashmap's mkey_t type is set to
 * char *.
 * 
 * This program takes in a text file of words on a singular line and
 * prints the words and their occurences.
 * 
 * NOTE: This code will be improved when the memory ownership of mp_hashmap 
 * is updated because this code does memory ownership on its own for now.
 */

#define _GNU_SOURCE
#include "../../containers/mp_hashmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

hash_t
strhash(char * const * input)
{
    hash_t out = 0;
    hash_t n = 1;
    for (char * tmp = (char *)*input; *tmp; ++tmp) {
        out += (*tmp * n);
        n*=31;
    }
    return out;
}

int 
strequals(char * const *a, char *const *b)
{
    return (strcmp(*a, *b) == 0); 
}

int
valequals(const int * a, const int * b)
{
    return *a == *b;
}

mp_hashmap 
compressFile(FILE * file)
{
    mp_hashmap out = mphm_create(strhash, strequals, valequals);
    
    size_t len = 256;
    char * line = malloc(sizeof(char) * len);

    while (getline(&line, &len, file) != -1) {
        char * leak = malloc(sizeof(char) * len);
        strcpy(leak, line);
        int * tmp = mphm_get(&out, leak);
        int count = tmp ? *tmp : 0;
        mphm_put(&out, leak, count + 1);
    }

    free(line);
    return out;
}

void
printMap(mp_hashmap * map, FILE * file)
{
    for (int i = 0; i < map->total_buckets; ++i) {
        mp_hashmap_node * tmp = map->buckets[i];
        while (tmp) {
            char * t = tmp->key;
            while (*++t); 
            *--t = '\0';

            fprintf(file, "%s, %i\n", tmp->key, tmp->value);
            tmp = tmp->next;
        }
    }
}

void
printBuckets(mp_hashmap * map, FILE * file)
{
    fprintf(file, "{\n");
    for (int i = 0; i < map->total_buckets; ++i) {
        fprintf(file, "\tbucket-%i {\n", i);

        mp_hashmap_node * tmp = map->buckets[i];
        while (tmp) {
            char * t = tmp->key;
            while (*++t); 
            *--t = '\0';

            fprintf(file, "\t\t%s -> %i\n", tmp->key, tmp->value);
            tmp = tmp->next;
        }

        fprintf(file, "\t}\n");
    }
    fprintf(file, "}\n");
}

void 
freeMap(mp_hashmap * map)
{
    for (int i = 0; i < map->total_buckets; ++i) {
        mp_hashmap_node * top = map->buckets[i];
        
        while (top) {
            free(top->key);
         
            top->key = NULL;
            top = top->next;
        }
    }

    mphm_free(map);
}

int
main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "It is necessary provide the input file path!\n");
        return 1;
    }

    FILE * inputFile = fopen(argv[1], "r");
    if (!inputFile) {
        printf("Provided input file did not exist!\n");
        return 1;
    }

    mp_hashmap compressed = compressFile(inputFile);
    fclose(inputFile);

    printMap(&compressed, stdout);

    freeMap(&compressed);
    return 0;
}