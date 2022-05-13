#ifndef __IMAGE_SET_H__
#define __IMAGE_SET_H__

#include <stdbool.h>

typedef struct image_set
{
    bool fragile;
    bool toxic;
    bool perishable;
    bool acrid;
    bool inflammable;
    bool frozen;
} image_set_t;

#endif
