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
 * @param fields строка в формате 'field1,field2,...'
 * @param conds массив условий фильтрации
 * @param conds_size размер массива conds
 * @return int код ошибки
 */
int db_select(const char* fields, const cond_array_t* conds);

/**
 * @brief Обновляет поля товаров в базе данных
 * 
 * @param pairs массив пар 'поле=значение'
 * @param conds массив условий фильтрации
 * @return int код ошибки
 */
int db_update(const pair_array_t* pairs, const cond_array_t* conds);

/**
 * @brief Удаляет товары удовлетворяющие всем условиям
 * 
 * @param conds массив условий фильтрации
 * @return int код ошибки
 */
int db_delete(const cond_array_t* conds);

/**
 * @brief Удаляет из БД дубликаты
 * 
 * Удаляет записи, значения всех указанных в команде полей
 * в которых совпадает со значением полей в других записях
 * 
 * @param conds строка в формате 'field1,field2,...'
 * @return int код ошибки
 */
int db_unique(const char* fields);

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
