#include <stdlib.h>
#include <string.h>
#include "mem_track/memtrack.h"
#include "cond_array.h"
#include "error_codes.h"

cond_array_t init_cond_array(void)
{
    return (cond_array_t) { .data = NULL, .size = 0 };
}

void free_cond_array(cond_array_t* arr)
{
    if (arr->data != NULL)
    {
        free(arr->data);
        track_free();
    }

    *arr = init_cond_array();
}

static size_t count_conds(const char* str)
{
    size_t size = 1;
    for (const char* i = strchr(str, ' '); i != NULL; i = strchr(i + 1, ' '))
        size++;
    return size;
}

int parse_cond_array(cond_array_t* arr, char* str)
{
    arr->size = count_conds(str);
    arr->data = (cond_t*) malloc(arr->size * sizeof(cond_t));
    if (arr->data == NULL)
        return MEM_ERR;
    track_malloc();

    int status = SUCCESS;
    for (size_t i = 0; status == SUCCESS && i < arr->size; i++)
    {
        char* end_pos = strchr(str, ' ');
        if (end_pos != NULL)
            *end_pos = '\0';

        status = parse_cond(arr->data + i, str);
        str = end_pos + 1;
    }

    if (status != SUCCESS)
        free_cond_array(arr);
    return status;
}

int cond_array_match(bool* result, const cond_array_t* arr, const product_t* product)
{
    int status = SUCCESS;
    bool match_flag = true;

    for (size_t i = 0; status == SUCCESS && match_flag && i < arr->size; i++)
    {
        bool tmp;
        status = cond_match(&tmp, arr->data + i, product);
        if (status == SUCCESS)
            match_flag = match_flag && tmp;
    }

    if (status == SUCCESS)
        *result = match_flag;
    
    return status;
}
