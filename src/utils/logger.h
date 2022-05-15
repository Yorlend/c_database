#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <stdio.h>

int log_free(void);

int log_add_file(FILE* fp);

int log_printf(const char* fmt, ...);

#endif
