#ifndef __MEM_TRACK_H__
#define __MEM_TRACK_H__

#include <stdio.h>

/**
 * @brief Вызван malloc, инкрементируем количество вызовов
 * 
 */
void track_malloc(void);

/**
 * @brief Вызван realloc, инкрементируем количество вызовов
 * 
 */
void track_realloc(void);

/**
 * @brief Вызван calloc, инкрементируем количество вызовов
 * 
 */
void track_calloc(void);

/**
 * @brief Вызван free, инкрементируем количество вызовов
 * 
 */
void track_free(void);

/**
 * @brief Запись информации о вызовах malloc/calloc/realloc/free в файл по указателю
 * 
 * @param file файловый указатель
 * @return int код ошибки
 */
int dump_memtrackf(FILE* file);

/**
 * @brief Запись информации о вызовах malloc/calloc/realloc/free в файл по имени файла
 * 
 * @param filename имя файла, в который записывается результат
 * @return int код ошибки
 */
int dump_memtrack(const char* filename);

#endif
