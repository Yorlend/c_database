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

static int parse_field_name(cond_t* cond, char** str)
{
    cond->field_name = *str;
    while (isalpha(**str))
        (*str)++;

    return SUCCESS;
}

static int parse_cond_type(cond_t* cond, char** str)
{
    char* old = *str;
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
    
    *old = '\0';
    return SUCCESS;
}

static int parse_field_value(cond_t* cond, char* str)
{
    cond->value = str;    
    return SUCCESS;
}

int parse_cond(cond_t* cond, char* str)
{
    int status = parse_field_name(cond, &str);

    if (status == SUCCESS)
        status = parse_cond_type(cond, &str);
    
    if (status == SUCCESS)
        status = parse_field_value(cond, str);

    // if (status != SUCCESS)
    //     free_cond(cond);

    return status;
}

// void free_cond(cond_t* cond)
// {
//     //WARNING: no free here
//     return;

//     if (cond->field_name != NULL)
//     {
//         free((void*)cond->field_name);
//         track_free();
//     }

//     if (cond->value != NULL)
//     {
//         free((void*)cond->value);
//         track_free();
//     }

//     *cond = init_cond();
// }

bool cond_valid(const cond_t* cond)
{
    return cond->field_name != NULL && cond->value != NULL;
}

static int apply_to_int(bool* result, const cond_t* cond, int value)
{
    int res;
    int status = parse_int(&res, cond->value);
    if (status != SUCCESS)
        return status;
    
    switch (cond->type)
    {
    case COND_EQUAL:
        *result = value == res;
        break;
    
    case COND_NOTEQ:
        *result = value != res;
        break;
    
    case COND_LESS:
        *result = value < res;
        break;

    case COND_GREATER:
        *result = value > res;
        break;
    
    default:
        return INVALID_COND_TYPE;
    }

    return SUCCESS;
}

static int apply_to_string(bool* result, const cond_t* cond, const char* str)
{
    switch (cond->type)
    {
    case COND_EQUAL:
        *result = strcmp(cond->value, str) == 0;
        break;
    
    case COND_NOTEQ:
        *result = strcmp(cond->value, str) != 0;
        break;
    
    default:
        return INVALID_COND_TYPE;
    }

    return SUCCESS;
}

static int apply_to_date(bool* result, const cond_t* cond, const date_t* date)
{
    date_t cond_date; 
    int status = parse_date(&cond_date, cond->value);
    if (status != SUCCESS)
        return status;

    switch (cond->type)
    {
    case COND_EQUAL:
        *result = date_eq(date, &cond_date);
        break;
    
    case COND_NOTEQ:
        *result = !date_eq(date, &cond_date);
        break;
    
    case COND_GREATER:
        *result = date_lt(&cond_date, date);
        break;
    
    case COND_LESS:
        *result = date_lt(date, &cond_date);
        break;
    
    default:
        break;
    }

    return SUCCESS;
}

static int apply_to_image_set(bool* result, const cond_t* cond, const image_set_t* set)
{
    image_set_t cond_set;
    int status = parse_image_set(&cond_set, cond->value);
    if (status != SUCCESS)
        return status;
    
    switch (cond->type)
    {
    case COND_EQUAL:
        *result = image_set_eq(set, &cond_set);
        break;
    
    case COND_NOTEQ:
        *result = !image_set_eq(set, &cond_set);
        break;
    
    case COND_IN:
        *result = image_set_in(set, &cond_set);
        break;
    
    case COND_INCLUDE:
        *result = image_set_in(&cond_set, set);
        break;

    default:
        return INVALID_COND_TYPE;
    }

    return status;
}

int cond_match(bool* result, const cond_t* cond, const product_t* product)
{
    if (result == NULL || !cond_valid(cond) || !product_valid(product))
        return INVALID_PARAMS;

    int status = SUCCESS;
    
    if (strcmp(cond->field_name, "comes") == 0)
        status = apply_to_date(result, cond, &product->comes);
    else if (strcmp(cond->field_name, "sender") == 0)
        status = apply_to_string(result, cond, product->sender);
    else if (strcmp(cond->field_name, "name") == 0)
        status = apply_to_string(result, cond, product->name);
    else if (strcmp(cond->field_name, "weight") == 0)
        status = apply_to_int(result, cond, product->weight);
    else if (strcmp(cond->field_name, "count") == 0)
        status = apply_to_int(result, cond, product->count);
    else if (strcmp(cond->field_name, "images") == 0)
        status = apply_to_image_set(result, cond, &product->images);
    else if (strcmp(cond->field_name, "worker") == 0)
        status = apply_to_string(result, cond, product->worker);
    else
        status = UNKNOWN_FIELD;

    return status;
}

