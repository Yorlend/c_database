#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "mem_track/memtrack.h"
#include "error_codes.h"
#include "cond.h"
#include "io/userio.h"

cond_t init_cond(void)
{
    return (cond_t) {
        .field_name = NULL,
        .type = COND_EQUAL,
        .value = NULL
    };
}

static int parse_field_name(cond_t* cond, const char** str)
{
    size_t n = 0;
    while (isalpha((*str)[n]))
        n++;
    
    cond->field_name = dup_nstring(*str, n);
    if (cond->field_name == NULL)
        return MEM_ERR;

    *str += n;
    return SUCCESS;
}

static int parse_cond_type(cond_t* cond, const char** str)
{
    if (strncmp(*str, "<", 1) == 0)
    {
        cond->type = COND_LESS;
        (*str)++;
    }
    else if (strncmp(*str, ">", 1) == 0)
    {
        cond->type = COND_GREATER;
        (*str)++;
    }
    else if (strncmp(*str, "==", 2) == 0)
    {
        cond->type = COND_EQUAL;
        *str += 2;
    }
    else if (strncmp(*str, "!=", 2) == 0)
    {
        cond->type = COND_NOTEQ;
        *str += 2;
    }
    else if (strncmp(*str, "/in/", 4) == 0)
    {
        cond->type = COND_IN;
        *str += 4;
    }
    else if (strncmp(*str, "/include/", 9) == 0)
    {
        cond->type = COND_INCLUDE;
        *str += 9;
    }
    else
        return UNKNOWN_COND_TYPE;
    
    return SUCCESS;
}

static int parse_field_value(cond_t* cond, const char* str)
{
    cond->value = dup_string(str);
    if (cond->value == NULL)
        return MEM_ERR;
    
    return SUCCESS;
}

int parse_cond(cond_t* cond, const char* str)
{
    int status = parse_field_name(cond, &str);

    if (status == SUCCESS)
        status = parse_cond_type(cond, &str);
    
    if (status == SUCCESS)
        status = parse_field_value(cond, str);

    if (status != SUCCESS)
        free_cond(cond);

    return status;
}

void free_cond(cond_t* cond)
{
    //WARNING: no free here
    return;

    if (cond->field_name != NULL)
    {
        free(cond->field_name);
        track_free();
    }

    if (cond->value != NULL)
    {
        free(cond->value);
        track_free();
    }

    *cond = init_cond();
}

bool cond_valid(const cond_t* cond)
{
    return cond->field_name != NULL && cond->value != NULL;
}

int cond_match(bool* result, const cond_t* cond, const product_t* product)
{
    if (!cond_valid(cond) || !product_valid(product))
        return INVALID_PARAMS;
    
    if (strcmp(cond->field_name, "sender") == 0)
    {
        switch (cond->type)
        {
        case COND_EQUAL:
            *result = strcmp(cond->value, product->sender) == 0;
            break;
        
        case COND_NOTEQ:
            *result = strcmp(cond->value, product->sender) != 0;
            break;
        
        default:
            return INVALID_COND_TYPE;
        }
    }
    else if (strcmp(cond->field_name, "name") == 0)
    {
        switch (cond->type)
        {
        case COND_EQUAL:
            *result = strcmp(cond->value, product->name) == 0;
            break;
        
        case COND_NOTEQ:
            *result = strcmp(cond->value, product->name) != 0;
            break;
        
        default:
            return INVALID_COND_TYPE;
        }
    }
    else ///TODO: implement other fields
        return UNKNOWN_FIELD;

    return SUCCESS;
}

