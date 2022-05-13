#include <stdio.h>
#include "mem_track/memtrack.h"
#include "db_entry/date.h"
#include "io/userio.h"

int main(void)
{
    char* date = NULL;
    size_t len = 0;

    getline_win(&date, &len, stdin);

    printf("ДАТА: %s\n", date);

    return 0;
}