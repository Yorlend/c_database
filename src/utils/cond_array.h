#ifndef __COND_ARRAY_H__
#define __COND_ARRAY_H__

#include <stddef.h>
#include "utils/cond.h"

typedef struct
{
    cond_t* data;
    size_t size;
} cond_array_t;

cond_array_t init_cond_array(void);

void free_cond_array(cond_array_t* arr);

int parse_cond_array(cond_array_t* arr, char* str);

/**
 * @brief Проверка всех условий в массиве arr
 * 
 * @param result true, если товар подходит по всем условиям
 * @param arr массив условий
 * @param product товар
 * @return int код ошибки
 */
int cond_array_match(bool* result, const cond_array_t* arr, const product_t* product);

#endif
