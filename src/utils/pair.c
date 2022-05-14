#include "pair.h"

#include "io/userio.h"
#include "error_codes.h"
#include "mem_track/memtrack.h"

#include <stdlib.h>
#include <string.h>

pair_t init_pair()
{
    return (pair_t) { .field = NULL, .value = NULL };
}

int parse_pair(pair_t* dst, const char* src)
{
    if (src == NULL || dst == NULL)
        return INVALID_PARAMS;

    char* delim = strchr(src, '=');

    if (delim == NULL)
        return BAD_INPUT;

    dst->field = dup_nstring(src, delim - src);
    dst->value = dup_string(delim + 1);

    return SUCCESS;
}

int pair_to_str(char** dst, const pair_t* src)
{
    if (dst == NULL || src == NULL)
        return INVALID_PARAMS;

    size_t buf_size = snprintf(NULL, 0, "%s=%s",
        src->field, src->value);

    *dst = malloc(buf_size);

    if (*dst == NULL)
        return MEM_ERR;

    track_malloc();

    snprintf(*dst, buf_size + 1, "%s=%s",
        src->field, src->value);

    return SUCCESS;
}
