#ifndef __PRODUCT_H__
#define __PRODUCT_H__

#include "date.h"
#include "image_set.h"

typedef struct
{
    date_t comes;
    char* sender;
    char* name;
    int weight;
    int count;
    image_set_t images;
    char* worker;
} product_t;

/**
 * @brief Создаёт пустую структуру
 * 
 * @return product_t продукт
 */
product_t init_product(void);

/**
 * @brief Очищает память под структуру
 * 
 * @param product товар
 */
void free_product(product_t* product);

bool product_valid(const product_t* product);

#endif
