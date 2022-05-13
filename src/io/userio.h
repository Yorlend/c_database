#ifndef __USERIO_H__
#define __USERIO_H__

#include <stdio.h>

/**
 * @brief Записывает строку неопределенной или определенной длины
 * 
 * @param lineptr указатель на указатель на строку
 * @param n количество записываемых символов или 0
 * @param stream файловый указатель
 * @return ssize_t количество считанных символов
 */
ssize_t getline_win(char** lineptr, size_t* n, FILE* stream);

/**
 * @brief Дублирует строку в новую область памяти
 * 
 * @param str строка для копирования
 * @return char* новый буфер с копией строки
 */
char* dup_string(const char* str);

/**
 * @brief Считывает число из строки
 * 
 * @param res результат
 * @param src строка с числом
 * @return int код ошибки
 */
int parse_int(int* res, const char* src);

#endif
