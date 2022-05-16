#include <stdlib.h>
#include <string.h>
#include "mem_track/memtrack.h"
#include "product.h"
#include "error_codes.h"


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

bool product_valid(const product_t* product)
{
    return product->sender != NULL &&
        date_valid(&product->comes) &&
        product->name != NULL &&
        product->worker != NULL;
}

static int cmp_by_field(bool *eql, const char* field, const product_t* product1, const product_t* product2)
{
    if (strncmp(field, "comes", 5) == 0)
        *eql = date_eq(&product1->comes, &product2->comes);
    else if (strncmp(field, "sender", 6) == 0)
        *eql = strcmp(product1->sender, product2->sender) == 0;
    else if (strncmp(field, "name", 4) == 0)
        *eql = strcmp(product1->name, product2->name) == 0;
    else if (strncmp(field, "weight", 6) == 0)
        *eql = product1->weight == product2->weight;
    else if (strncmp(field, "count", 5) == 0)
        *eql = product1->count == product2->count;
    else if (strncmp(field, "images", 6) == 0)
        *eql = image_set_eq(&product1->images, &product2->images);
    else if (strncmp(field, "worker", 6) == 0)
        *eql = strcmp(product1->worker, product2->worker) == 0;
    else
        return UNKNOWN_FIELD;

    return SUCCESS;
}

int cmp_by_fields(bool *eql, const char* fields, const product_t* product1, const product_t* product2)
{
    *eql = true;
    while (*eql)
    {
        bool tmp;
        int status = cmp_by_field(&tmp, fields, product1, product2);
        if (status != SUCCESS)
            return status;
        *eql = *eql && tmp;

        fields = strchr(fields, ',');
        if (fields == NULL)
            break;
        fields++;
    }

    return SUCCESS;
}
