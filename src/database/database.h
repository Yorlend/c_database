#ifndef __DATABASE_H__
#define __DATABASE_H__

#include <stddef.h>
#include "utils/pair_array.h"
#include "utils/cond_array.h"

/**
 * @brief Вставка новой записи
 * 
 * @param pairs массив пар 'поле=значение'
 * @return int код ошибки
 */
int db_insert(const pair_array_t* pairs);

/**
 * @brief Выполнение запроса
 * 
 * Найденные записи выводятся в стандартный поток вывода
 * 
 * @param fields строка в фрмате 'field1,field2,...'
 * @param conds массив условий фильтрации
 * @param conds_size размер массива conds
 * @return int код ошибки
 */
int db_select(const char* fields, const cond_array_t* conds);

/**
 * @brief Возвращает число обьектов в базе данных
 * 
 * @return size_t 
 */
size_t db_rows_count(void);

/**
 * @brief Удаление всех записей
 */
void db_clear(void);

#endif
