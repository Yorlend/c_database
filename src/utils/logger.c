#include <stdlib.h>
#include <stdarg.h>
#include "mem_track/memtrack.h"
#include "logger.h"
#include "error_codes.h"

static size_t files_count;
static FILE** log_files = NULL; // stdout + выходной файл

void log_free(void)
{
    if (log_files != NULL)
    {
        free(log_files);
        track_free();

        files_count = 0;
        log_files = NULL;
    }
}

int log_add_file(FILE* fp)
{
    FILE** new = realloc(log_files, (files_count + 1) * sizeof(FILE*));
    if (new == NULL)
        return MEM_ERR;
    
    if (log_files == NULL)
        track_malloc();
    else
        track_realloc();
    
    log_files = new;
    log_files[files_count] = fp;
    files_count++;

    return SUCCESS;
}

int log_printf(const char* fmt, ...)
{
    int status = SUCCESS;
    va_list va_lst;

    for (size_t i = 0; status == SUCCESS && i < files_count; i++)
    {
        va_start(va_lst, fmt);
        if (vfprintf(log_files[i], fmt, va_lst) < 0)
            status = OUTPUT_ERROR;
        va_end(va_lst);
    }

    return status;
}

