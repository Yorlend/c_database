#ifndef __PAIR_ARRAY_H__
#define __PAIR_ARRAY_H__

#include "pair.h"
#include <stddef.h>

#define DELIM ","

typedef struct
{
    pair_t* data;
    size_t size;
} pair_array_t;

pair_array_t init_pair_array(void);

void free_pair_array(pair_array_t* arr);

int parse_pair_array(pair_array_t* arr, char* str);

#endif
