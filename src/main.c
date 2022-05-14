#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mem_track/memtrack.h"
#include "database/database.h"
#include "database/command.h"
#include "io/userio.h"
#include "error_codes.h"

int read_cmd_file(FILE* file)
{
    int status = SUCCESS;

    char* buffer = NULL;
    size_t size = 0;
    
    while (status == SUCCESS && getline_win(&buffer, &size, file) > 0)
    {
        while (buffer[strlen(buffer) - 1] == '\n')
            buffer[strlen(buffer) - 1] = '\0';

        status = db_cmd(buffer);
        if (status != 0)
            printf("status: %d\n", status);
    }

    free(buffer);
    track_free();

    return status;
}

int main(void)
{
    FILE *fp = fopen("test/cmds.txt", "r");
    read_cmd_file(fp);
    fclose(fp);

    db_clear();
    dump_memtrackf(stdout);
    return 0;
}