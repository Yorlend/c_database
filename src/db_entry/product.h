#ifndef __PRODUCT_H__
#define __PRODUCT_H__

#include "date.h"
#include "image_set.h"

typedef struct
{
    date_t comes;
    char* sender;
    char* name;
    int weight;
    int count;
    image_set_t images;
    char* worker;
} product_t;


#endif
