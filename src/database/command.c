#include <string.h>
#include <stdlib.h>
#include "mem_track/memtrack.h"
#include "command.h"
#include "database.h"
#include "error_codes.h"

static size_t count_args(const char* str)
{
    size_t size = 1;
    for (const char* i = strchr(str, ' '); i != NULL; i = strchr(i + 1, ' '))
        size++;
    return size;
}

static void free_args(char** args, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        if (args[i] != NULL)
        {
            free(args[i]);
            track_free();
        }
    }
    free(args);
    track_free();
}

static int malloc_args(char*** args, size_t size)
{
    *args = malloc(size * sizeof(char**));
    if (*args == NULL)
        return MEM_ERR;

    track_malloc();
    return SUCCESS;
}

static int parse_args(char*** args, size_t* size, const char* str)
{
    if (args == NULL || size == NULL || str == NULL)
        return INVALID_PARAMS;
    
    // подсчитать число параметров в строке
    *size = count_args(str);
    int status = malloc_args(args, *size);

    for (size_t i = 0; status == SUCCESS && i < *size; i++)
    {
        const char* next = strchr(str, ' ');
        int len = next == NULL ? strlen(str) : next - str;

        (*args)[i] = malloc((len + 1) * sizeof(char));
        if ((*args)[i] == NULL)
        {
            status = MEM_ERR;
            break;
        }
        track_malloc();
        strncpy((*args)[i], str, len);
        (*args)[i][len] = '\0';
        str = next + 1;
    }

    if (status != SUCCESS)
    {
        free_args(*args, *size);
        *args = NULL;
    }

    return status;
}

int db_cmd(const char* command_line)
{
    if (command_line == NULL)
        return INVALID_PARAMS;
    
    char* args_str = strchr(command_line, ' ');
    if (args_str == NULL)
        return INVALID_COMMAND;
    int cmd_len = args_str - command_line;
    args_str++;

    char** args = NULL;
    size_t args_size = 0;

    int status = parse_args(&args, &args_size, args_str);
    if (status != SUCCESS)
        return status;

    if (strncmp(command_line, "insert", cmd_len) == 0)
    {
        status = db_insert((const char**)args, args_size);
        if (status == SUCCESS)
            printf("select: %zu\n", db_rows_count());
    }
    else
        status = INVALID_COMMAND;

    free_args(args, args_size);
    return status;
}
