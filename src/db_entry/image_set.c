#include <string.h>
#include "image_set.h"
#include "error_codes.h"

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
    ///TODO: implement
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
    if (src == NULL || target == NULL)
        return false;
    
    return (!src->fragile || target->fragile) &&
        (!src->toxic || target->toxic) &&
        (!src->perishable || target->perishable) &&
        (!src->acrid || target->acrid) &&
        (!src->inflammable || target->inflammable) &&
        (!src->frozen || target->frozen);
}
