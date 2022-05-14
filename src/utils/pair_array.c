#include <stdlib.h>
#include <string.h>
#include "pair_array.h"
#include "mem_track/memtrack.h"
#include "error_codes.h"

pair_array_t init_pair_array(void)
{
    return (pair_array_t) { .data = NULL, .size = 0 };
}

void free_pair_array(pair_array_t* arr)
{
    if (arr->data != NULL)
    {
        free(arr->data);
        track_free();
        arr->size = 0;
    }
}

static size_t count_pairs(const char* str)
{
    size_t size = 0;
    for (const char* i = strchr(str, '='); i != NULL; i = strchr(i + 1, '='))
        size++;
    return size;
}

int parse_pair_array(pair_array_t* arr, char* str)
{
    arr->size = count_pairs(str);
    arr->data = (pair_t*) malloc(arr->size * sizeof(pair_t));
    if (arr->data == NULL)
        return MEM_ERR;
    track_malloc();

    int status = SUCCESS;
    for (size_t i = 0; status == SUCCESS && i < arr->size; i++)
    {
        char* end_pos = strchr(str, '=');
        end_pos = strchr(end_pos + 1, '=');
        if (end_pos != NULL)
            while (*end_pos != ',')
                end_pos--;
        else
            end_pos = str + strlen(str);
        *end_pos = '\0';

        status = parse_pair(arr->data + i, str);
        str = end_pos + 1;
    }

    if (status != SUCCESS)
        free_pair_array(arr);

    return status;
}
