#ifndef __PAIR_ARRAY_H__
#define __PAIR_ARRAY_H__

#include "pair.h"
#include <stddef.h>

typedef struct
{
    pair_t* data;
    size_t size;
} pair_array_t;

pair_array_t init_pair_array(void);

void free_pair_array(pair_array_t* arr);

/**
 * @brief Разбирает строку на пары
 * 
 * @param arr результат
 * @param str входная строка в формате 'field=value1,field=value2,...'
 * @return int код ошибки
 */
int parse_pair_array(pair_array_t* arr, char* str);

#endif
