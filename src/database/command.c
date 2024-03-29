#include <string.h>
#include <stdlib.h>
#include "mem_track/memtrack.h"
#include "command.h"
#include "database.h"
#include "error_codes.h"
#include "utils/pair_array.h"
#include "io/userio.h"


static int execute_insert(char* args_str)
{
    pair_array_t args = init_pair_array();

    int status = parse_pair_array(&args, args_str);
    if (status != SUCCESS)
        return status;

    status = db_insert(&args);
    free_pair_array(&args);

    return status;
}

static int execute_select(char* args_str)
{
    char* cond_str = strchr(args_str, ' ');
    cond_array_t conds = init_cond_array();

    int status = SUCCESS;

    if (cond_str != NULL)
    {
        *cond_str++ = '\0';
        int status = parse_cond_array(&conds, cond_str);
        if (status != SUCCESS)
            return status;
    }
    
    status = db_select(args_str, &conds);
    free_cond_array(&conds);

    return status;
}

static int execute_update(char* args_str)
{
    pair_array_t pairs = init_pair_array();
    cond_array_t conds = init_cond_array();

    char* cond_str = strchr(args_str, ' ');
    if (cond_str != NULL)
    {
        *cond_str++ = '\0';
        int status = parse_cond_array(&conds, cond_str);
        if (status != SUCCESS)
            return status;
    }

    int status = parse_pair_array(&pairs, args_str);
    if (status == SUCCESS)
    {
        status = db_update(&pairs, &conds);
        free_pair_array(&pairs);
    }

    free_cond_array(&conds);
    return status;
}

static int execute_delete(char* args_str)
{
    cond_array_t conds = init_cond_array();
    int status = parse_cond_array(&conds, args_str);
    if (status != SUCCESS)
        return status;
    
    status = db_delete(&conds);
    free_cond_array(&conds);

    return status;
}

static int execute_unique(const char* fields)
{
    return db_unique(fields);
}

int db_cmd(char* command_line)
{
    if (command_line == NULL)
        return INVALID_PARAMS;

    char* dup_cmd = dup_nstring(command_line, 20);
    if (dup_cmd == NULL)
        return MEM_ERR;
    
    char* args_str = strchr(command_line, ' ');
    if (args_str == NULL)
    {
        printf("incorrect: %s\n", dup_cmd);
        free(dup_cmd);
        track_free();
        return INVALID_COMMAND;
    }
    *args_str = '\0';
    args_str++;

    int status = SUCCESS;
    if (strcmp(command_line, "insert") == 0)
        status = execute_insert(args_str);
    else if (strcmp(command_line, "select") == 0)
        status = execute_select(args_str);
    else if (strcmp(command_line, "update") == 0)
        status = execute_update(args_str);
    else if (strcmp(command_line, "delete") == 0)
        status = execute_delete(args_str);
    else if (strcmp(command_line, "uniq") == 0)
        status = execute_unique(args_str);
    else
        status = INVALID_COMMAND;
    
    if (status != SUCCESS)
        printf("incorrect: %s\n", dup_cmd);

    free(dup_cmd);
    track_free();
    return status;
}
