#include <string.h>
#include <stdio.h>
#include "image_set.h"
#include "error_codes.h"

#define OPT_BUFF_SIZE 64

image_set_t init_image_set(void)
{
    return (image_set_t) {
        .fragile = false,
        .toxic = false,
        .perishable = false,
        .acrid = false,
        .inflammable = false,
        .frozen = false
    };
}

int parse_image_set(image_set_t* set, const char* str)
{
    if (*str++ != '[')
        return INVALID_SET_FORMAT;

    *set = init_image_set();

    while (*str != ']')
    {
        if (*str++ != '\'')
            return INVALID_SET_FORMAT;

        char buff[OPT_BUFF_SIZE];
        int i = 0;
        for (i = 0; *str != '\'' && i < OPT_BUFF_SIZE; i++)
        {
            buff[i] = *str;
            str++;
        }
        if (i == OPT_BUFF_SIZE)
            return INVALID_SET_FORMAT;

        buff[i] = '\0';
        str++;
        if (*str == ',')
            str++;

        image_set_add(set, buff);
    }

    str++;
    if (*str != '\0')
        return INVALID_SET_FORMAT;

    return SUCCESS;
}

int image_set_add(image_set_t* set, const char* option)
{
    if (set == NULL || option == NULL)
        return INVALID_PARAMS;
    
    if (strcmp(option, "fragile") == 0)
        set->fragile = true;
    else if (strcmp(option, "toxic") == 0)
        set->toxic = true;
    else if (strcmp(option, "perishable") == 0)
        set->perishable = true;
    else if (strcmp(option, "acrid") == 0)
        set->acrid = true;
    else if (strcmp(option, "inflammable") == 0)
        set->inflammable = true;
    else if (strcmp(option, "frozen") == 0)
        set->frozen = true;
    else
        return INVALID_IMAGE_OPTION;

    return SUCCESS;
}

bool image_set_in(const image_set_t* src, const image_set_t* target)
{
    return (!src->fragile || target->fragile) &&
        (!src->toxic || target->toxic) &&
        (!src->perishable || target->perishable) &&
        (!src->acrid || target->acrid) &&
        (!src->inflammable || target->inflammable) &&
        (!src->frozen || target->frozen);
}

bool image_set_eq(const image_set_t* set1, const image_set_t* set2)
{
    return !(set1->fragile ^ set2->fragile) &&
        !(set1->toxic ^ set2->toxic) &&
        !(set1->perishable ^ set2->perishable) &&
        !(set1->acrid ^ set2->acrid) &&
        !(set1->inflammable ^ set2->inflammable) &&
        !(set1->frozen ^ set2->frozen);
}

void print_image_set(const image_set_t* set)
{
    bool was_any = false;

    printf("[");

    if (set->fragile)
    {
        printf("'fragile'");
        was_any = true;
    }
    if (set->toxic)
    {
        if (was_any)
            printf(",");
        printf("'toxic'");
        was_any = true;
    }
    if (set->perishable)
    {
        if (was_any)
            printf(",");
        printf("'perishable'");
        was_any = true;
    }
    if (set->acrid)
    {
        if (was_any)
            printf(",");
        printf("'acrid'");
        was_any = true;
    }
    if (set->inflammable)
    {
        if (was_any)
            printf(",");
        printf("'inflammable'");
        was_any = true;
    }
    if (set->frozen)
    {
        if (was_any)
            printf(",");
        printf("'frozen'");
    }

    printf("]");
}
