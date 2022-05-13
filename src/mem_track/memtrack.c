#include "memtrack.h"
#include "error_codes.h"

/**
 * @brief Считает количество вызовов функций управления дин. памятью.
 * 
 */
struct
{
    size_t malloc_calls;
    size_t realloc_calls;
    size_t calloc_calls;
    size_t free_calls;
} memory_manager;

void track_malloc(void)
{
    memory_manager.malloc_calls++;
}

void track_realloc(void)
{
    memory_manager.realloc_calls++;
}

void track_calloc(void)
{
    memory_manager.calloc_calls++;
}

void track_free(void)
{
    memory_manager.free_calls++;
}

int dump_memtrackf(FILE* file)
{
    if (file == NULL)
        return FILE_ERROR;

    fprintf(file, "malloc: %zu\nrealloc: %zu\ncalloc: %zu\nfree: %zu\n",
        memory_manager.malloc_calls,
        memory_manager.realloc_calls,
        memory_manager.calloc_calls,
        memory_manager.free_calls);
}

int dump_memtrack(const char* filename)
{
    FILE* fp = fopen(filename, "w");

    if (fp == NULL)
        return FILE_ERROR;

    dump_memtrackf(fp);

    fclose(fp);
    return SUCCESS;
}
