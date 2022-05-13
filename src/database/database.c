#include <stdbool.h>
#include <string.h>
#include "error_codes.h"
#include "list/list.h"
#include "mem_track/memtrack.h"
#include "io/userio.h"
#include "database.h"

#define ENTRY_FIELDS_COUNT 7

list_t entry_list;

static bool fields_format_valid(const char** pairs, size_t size)
{
    for (size_t i = 0; i < size; i++)
        if (strchr(pairs[i], '=') == NULL)
            return false;

    return true;
}

static bool has_duplicate_fields(const char** pairs, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        int eq_pos_i = strchr(pairs[i], '=') - pairs[i];
        for (size_t j = i + 1; j < size; j++)
        {
            int eq_pos_j = strchr(pairs[j], '=') - pairs[j];
            if (eq_pos_i == eq_pos_j && strncmp(pairs[i], pairs[j], eq_pos_i) == 0)
                return true;
        }
    }

    return false;
}

int construct_product(product_t* product, const char** pairs, size_t size)
{
    int status = SUCCESS;
    for (size_t i = 0; i < size && status == SUCCESS; i++)
    {
        int eq_pos = strchr(pairs[i], '=') - pairs[i];
        const char* value = pairs[i] + eq_pos + 1;

        if (strncmp(pairs[i], "comes", eq_pos) == 0)
            status = parse_date(&product->comes, value);
        else if (strncmp(pairs[i], "sender", eq_pos) == 0)
        {
            product->sender = dup_string(value);
            if (product->sender == NULL)
                status = MEM_ERR;
        }
        else if (strncmp(pairs[i], "name", eq_pos) == 0)
        {
            product->name = dup_string(value);
            if (product->name == NULL)
                status = MEM_ERR;
        }
        else if (strncmp(pairs[i], "weight", eq_pos) == 0)
            status = parse_int(&product->weight, value);
        else if (strncmp(pairs[i], "count", eq_pos) == 0)
            status = parse_int(&product->count, value);
        else if (strncmp(pairs[i], "images", eq_pos) == 0)
            status = parse_image_set(&product->images, value);
        else if (strncmp(pairs[i], "worker", eq_pos) == 0)
        {
            product->worker = dup_string(value);
            if (product->worker == NULL)
                status = MEM_ERR;
        }
        else
            status = UNKNOWN_FIELD;
    }

    if (status != SUCCESS)
        free_product(product);
    
    return status;
}

int db_insert(const char** pairs, size_t size)
{
    if (pairs == NULL)
        return INVALID_PARAMS;

    if (size != ENTRY_FIELDS_COUNT)
        return INVALID_FIELDS_COUNT;
    
    if (!fields_format_valid(pairs, size))
        return INVALID_FIELDS_FORMAT; 
    
    if (has_duplicate_fields(pairs, size))
        return DUPLICATE_FIELDS;

    product_t product = init_product();
    int status = construct_product(&product, pairs, size);    
    if (status == SUCCESS)
        status = push_back(&entry_list, &product);

    return status;
}

size_t db_rows_count(void)
{
    return list_size(&entry_list);
}

void db_clear(void)
{
    clear(&entry_list);
}
