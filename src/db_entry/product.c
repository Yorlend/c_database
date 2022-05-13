#include <stddef.h>
#include <stdlib.h>
#include "mem_track/memtrack.h"
#include "product.h"

product_t init_product(void)
{
    return (product_t) {
        .comes = init_date(),
        .sender = NULL,
        .name = NULL,
        .weight = 0,
        .count = 0,
        .images = init_image_set(),
        .worker = NULL
    };
}

void free_product(product_t* product)
{
    if (product != NULL)
    {
        if (product->sender != NULL)
        {
            free(product->sender);
            track_free();
        }

        if (product->name != NULL)
        {
            free(product->name);
            track_free();
        }

        if (product->worker != NULL)
        {
            free(product->worker);
            track_free();
        }

        *product = init_product();
    }
}
