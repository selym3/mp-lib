#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "probs.h"
#include "../../containers/mp_hashmap.h"

static const char LETTERS[] = "_ abcdefghijklmnopqrstuvwxyz,.\n";

size_t 
LETTER_IDX(char c)
{   
    c = tolower(c);
    for (const char * tmp = LETTERS; *tmp; ++tmp) {
        if (*tmp == c)
            return (tmp - LETTERS);
    }
    return 0;
}

char
IDX_LETTER(int idx)
{
    return LETTERS[idx];
}

char
getNext(probs * a)
{
    static hash_t pool = 0xdeadbeef;
    pool ^= 0xBEEFDEAD;
    pool ^= pool << 13;
    pool ^= pool >> 7;
    pool ^= pool << 17;

    int l = pool % a->total;

    for (int i = 0; i < 31;++i) {
        l -= a->probs[i];
        if (l < 0)
            return IDX_LETTER(i);
    }
    return IDX_LETTER(0);
}

hash_t
sstringHash(const sstring * a)
{
    hash_t pool = 0;

    for(int i = 0; i < PAST_CHARS; ++i) {
        pool ^= a->word[i];
        pool ^= pool << 13;
        pool ^= pool >> 7;
        pool ^= pool << 17;
    }
    return pool;
    
}

int
sstringEquals(const sstring * a, const sstring * b)
{
    for (int i = 0; i < PAST_CHARS; ++i)
        if (a->word[i] != b->word[i])
            return 0;
    return 1;
}

int
probsEquals(const probs * a, const probs * b)
{
    return 0;
}

mp_hashmap
getTable(FILE * input)
{
    mp_hashmap out = mphm_create(
        sstringHash,
        sstringEquals,
        probsEquals
    );
    
    sstring s;
    for (int l=0; l < PAST_CHARS; s.word[l++] = '_');
    s.word[PAST_CHARS] = '\0';

    int c; 
    while ((c = getc(input)) > 0) {
        // if (!(__isascii(c)))
            // continue;
        
        probs * a = mphm_get(&out, s);
        if (!a) {
            mphm_put(&out, s, (probs){ {0}, 0 });
            a = mphm_get(&out, s);
        }
        ++(a->probs[LETTER_IDX( (char)(c = IDX_LETTER(LETTER_IDX(c))) )]);
        ++(a->total);

        for (int i = 1; i < PAST_CHARS; ++i)
            s.word[i-1] = s.word[i];
        s.word[PAST_CHARS-1] = (char)c;
    }

    return out;
}

int
main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Program requires an input file!\n");
        return 1;
    }

    const unsigned long long OUTPUT = 1000000;

    FILE * inputFile = fopen(argv[1], "r");
    if (!inputFile) {
        fprintf(stderr, "Input file could not be opened!\n");
        return 1;
    }
    mp_hashmap chain = getTable(inputFile);
    
    sstring s;
    for (int l=0; l < PAST_CHARS; s.word[l++] = '_');
    s.word[PAST_CHARS] = '\0';

    for (int i = 0; i < OUTPUT; ++i) {
        probs * a = mphm_get(&chain, s);

        char n = a ? getNext(a) : '_';
        printf("%c", n);

        for (int j = 1; j < PAST_CHARS; ++j)
            s.word[j-1] = s.word[j];
        s.word[PAST_CHARS-1] = (char)n;
    } 

    fclose(inputFile);
    mphm_free(&chain);

    return 0;
}