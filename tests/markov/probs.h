#ifndef __PROBS_H__
#define __PROBS_H__

#include <stddef.h>

const static size_t PAST_CHARS = 5;

typedef struct {
    int probs[31];
    size_t total;
} probs;

typedef struct {
    char word[PAST_CHARS + 1];
} sstring;

#endif