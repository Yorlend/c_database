#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "error_codes.h"
#include "mem_track/memtrack.h"
#include "userio.h"

#define BASE_BUFFER_SIZE 128
#define DELTA_BUFFER_SIZE 128

long getline_win(char** lineptr, size_t* n, FILE* stream) 
{
    if (lineptr == NULL || stream == NULL || n == NULL)
        return -1;

    char* bufptr = *lineptr;
    long size = *n;

    int c = fgetc(stream);
    if (c == EOF)
        return -1;

    if (bufptr == NULL)
    {
        bufptr = malloc(BASE_BUFFER_SIZE * sizeof(char));
        if (bufptr == NULL)
            return -1;
        track_malloc();
        
        size = BASE_BUFFER_SIZE;
    }

    char* p = bufptr;
    while(c != EOF)
    {
        if (p - bufptr > size - 1)
        {
            size += DELTA_BUFFER_SIZE;
            bufptr = realloc(bufptr, size * sizeof(char));
            if (bufptr == NULL)
                return -1;
            track_realloc();
        }
        *p++ = c;
        if (c == '\n')
            break;

        c = fgetc(stream);
    }

    *p++ = '\0';
    *lineptr = bufptr;
    *n = size;

    return p - bufptr - 1;
}

char* dup_string(const char* str)
{
    char* res = malloc((strlen(str) + 1) * sizeof(char));
    if (res != NULL)
    {
        track_malloc();
        strcpy(res, str);
    }
    return res;
}

char* dup_nstring(const char* str, size_t n)
{
    char* res = malloc((n + 1) * sizeof(char));
    if (res != NULL)
    {
        track_malloc();
        strncpy(res, str, n);
        res[n] = '\0';
    }
    return res;
}

int parse_int(int* res, const char* src)
{
    if (res == NULL || src == NULL)
        return INVALID_PARAMS;

    for (*res = 0; isdigit(*src); src++)
        *res = *res * 10 + *src - '0';

    if (*src != '\0')
        return BAD_INPUT;
    return SUCCESS;
}

int parse_string(char** res, const char* str)
{
    *res = dup_string(str);
    if (*res == NULL)
        return MEM_ERR;
    return SUCCESS;
}

int update_string(char** res, const char* str)
{
    char* new_ptr = realloc(*res, (strlen(str) + 1) * sizeof(char));
    if (new_ptr == NULL)
        return MEM_ERR;
    track_realloc();
    *res = new_ptr;

    strcpy(*res, str);
    return SUCCESS;
}
