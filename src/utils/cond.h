#ifndef __COND_H__
#define __COND_H__

#include "db_entry/product.h"

typedef enum
{
    COND_EQUAL,
    COND_NOTEQ,
    COND_LESS,
    COND_GREATER,
    COND_IN,
    COND_INCLUDE,
} cond_type_t;

typedef struct
{
    const char* field_name;
    cond_type_t type;
    const char* value;
} cond_t;

/**
 * @brief Создает пустую структуру
 * 
 * @return cond_t результат
 */
cond_t init_cond(void);

/**
 * @brief Проверка корректности структуры
 * 
 * @param cond 
 * @return true 
 * @return false 
 */
bool cond_valid(const cond_t *cond);

/**
 * @brief Считывание условия из строки
 * 
 * @param cond результат
 * @param str строка с условием
 * @return int код ошибки
 */
int parse_cond(cond_t* cond, char* str);

/**
 * @brief Проверяет условие для товара
 * 
 * @param result true, если проверка успешна
 * @param cond условие проверки
 * @param product проверяемый товар
 * @return int код ошибки
 */
int cond_match(bool* result, const cond_t* cond, const product_t* product);

#endif
