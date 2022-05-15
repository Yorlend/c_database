#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mem_track/memtrack.h"
#include "database/database.h"
#include "database/command.h"
#include "utils/logger.h"
#include "io/userio.h"
#include "error_codes.h"

#define INPUT_FILE_NAME     "input.txt"
#define OUTPUT_FILE_NAME    "output.txt"

FILE* log_file;

void read_cmd_file(FILE* file)
{
    char* buffer = NULL;
    size_t size = 0;
    
    while (getline_win(&buffer, &size, file) > 0)
    {
        while (buffer[strlen(buffer) - 1] == '\n')
            buffer[strlen(buffer) - 1] = '\0';
        
        if (strlen(buffer) == 0)
            continue;
        
        // Команда завершения ввода
        if (strcmp(buffer, "exit") == 0)
            break;

        int status = db_cmd(buffer);
        if (status != 0)
            printf("command failed with error code %d\n", status);
    }

    free(buffer);
    track_free();
}

void init_logs(void)
{
    log_add_file(stdout);
    
    log_file = fopen(OUTPUT_FILE_NAME, "w");
    if (log_file == NULL)
        printf("no output.txt file. Output to stdout only...\n");
    else
        log_add_file(log_file);
}

void dump_meminfo(void)
{
    FILE* fp = fopen("memstat.txt", "w");
    if (fp == NULL)
        printf("unable to write to file memstat.txt\n");
    else
    {
        dump_memtrackf(fp);
        fclose(fp);
    }
}

int main(void)
{
    init_logs();

    FILE* fp = fopen(INPUT_FILE_NAME, "r");
    if (fp == NULL)
        printf("no input.txt file. Skipping...\n");
    else
    {
        read_cmd_file(fp);
        fclose(fp);
    }

    read_cmd_file(stdin);

    db_clear();
    log_free();

    if (log_file != NULL)
        fclose(log_file);
    
    dump_meminfo();
    return 0;
}