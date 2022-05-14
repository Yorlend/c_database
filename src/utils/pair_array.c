#include "pair_array.h"
#include "mem_track/memtrack.h"
#include <stdlib.h>
#include <string.h>

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

int parse_pair_array(pair_array_t* arr, char* str)
{
    char* token = strtok(str, DELIM);

    while (token != NULL)
    {
        
    }
}
