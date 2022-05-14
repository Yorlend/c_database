#ifndef __COND_ARRAY_H__
#define __COND_ARRAY_H__

#include <stddef.h>
#include "utils/cond.h"

typedef struct
{
    cond_t* data;
    size_t size;
} cond_array_t;

cond_array_t init_cond_array(void);

void free_cond_array(cond_array_t* arr);

int parse_cond_array(char* str);

#endif
