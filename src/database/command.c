#include <string.h>
#include <stdlib.h>
#include "mem_track/memtrack.h"
#include "command.h"
#include "database.h"
#include "error_codes.h"
#include "utils/pair_array.h"

// static size_t count_args(const char* str)
// {
//     size_t size = 1;
//     for (const char* i = strchr(str, ' '); i != NULL; i = strchr(i + 1, ' '))
//         size++;
//     return size;
// }

// static void free_args(char** args, size_t size)
// {
//     for (size_t i = 0; i < size; i++)
//     {
//         if (args[i] != NULL)
//         {
//             free(args[i]);
//             track_free();
//         }
//     }
//     free(args);
//     track_free();
// }

// static int malloc_args(char*** args, size_t size)
// {
//     *args = malloc(size * sizeof(char**));
//     if (*args == NULL)
//         return MEM_ERR;

//     track_malloc();
//     return SUCCESS;
// }

// static int parse_args(char*** args, size_t* size, const char* str)
// {
//     if (args == NULL || size == NULL || str == NULL)
//         return INVALID_PARAMS;
    
//     // подсчитать число параметров в строке
//     *size = count_args(str);
//     int status = malloc_args(args, *size);

//     for (size_t i = 0; status == SUCCESS && i < *size; i++)
//     {
//         const char* next = strchr(str, ' ');
//         int len = next == NULL ? strlen(str) : next - str;

//         (*args)[i] = malloc((len + 1) * sizeof(char));
//         if ((*args)[i] == NULL)
//         {
//             status = MEM_ERR;
//             break;
//         }
//         track_malloc();
//         strncpy((*args)[i], str, len);
//         (*args)[i][len] = '\0';
//         str = next + 1;
//     }

//     if (status != SUCCESS)
//     {
//         free_args(*args, *size);
//         *args = NULL;
//     }

//     return status;
// }

int db_cmd(char* command_line)
{
    printf("command: %s\n", command_line);

    if (command_line == NULL)
        return INVALID_PARAMS;
    
    char* args_str = strchr(command_line, ' ');
    if (args_str == NULL)
        return INVALID_COMMAND;
    *args_str = '\0';
    args_str++;

    int status = SUCCESS;
    if (strcmp(command_line, "insert") == 0)
    {
        pair_array_t args = init_pair_array();

        status = parse_pair_array(&args, args_str);
        if (status != SUCCESS)
            return status;

        status = db_insert(&args);

        free_pair_array(&args);
    }
    else if (strcmp(command_line, "select") == 0)
    {
        char* cond_str = strchr(args_str, ' ');
        *cond_str++ = '\0';

        cond_array_t conds = init_cond_array();
        status = parse_cond_array(&conds, cond_str);
        if (status != SUCCESS)
            return status;
        
        status = db_select(args_str, &conds);
        free_cond_array(&conds);
    }
    else
        status = INVALID_COMMAND;

    return status;
}
