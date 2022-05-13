#include <stdio.h>
#include <stdlib.h>
#include "mem_track/memtrack.h"
#include "database/database.h"
#include "database/command.h"
#include "io/userio.h"

int main(void)
{
    char* buffer = NULL;
    size_t size = 0;
    
    while (getline_win(&buffer, &size, stdin) > 0)
    {
        int status = db_cmd(buffer);
        if (status != 0)
            printf("status: %d\n", status);
    }

    free(buffer);
    track_free();

    db_clear();
    return 0;
}