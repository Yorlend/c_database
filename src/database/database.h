#ifndef __DATABASE_H__
#define __DATABASE_H__

#include <stddef.h>

/**
 * @brief Вставка новой записи
 * 
 * @param pairs массив строк формата "field=value"
 * @param size размер массива pairs
 * @return int код ошибки
 */
int db_insert(const char** pairs, size_t size);

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
