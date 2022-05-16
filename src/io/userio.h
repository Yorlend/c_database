#ifndef __USERIO_H__
#define __USERIO_H__

#include <stdio.h>

/**
 * @brief Записывает строку неопределенной или определенной длины
 * 
 * @param lineptr указатель на указатель на строку
 * @param n количество записываемых символов или 0
 * @param stream файловый указатель
 * @return long количество считанных символов
 */
long getline_win(char** lineptr, size_t* n, FILE* stream);

/**
 * @brief Дублирует строку в новую область памяти
 * 
 * @param str строка для копирования
 * @return char* новый буфер с копией строки
 */
char* dup_string(const char* str);

/**
 * @brief Дублирует первые n символов строки в новую область памяти
 * 
 * В конец помещается символ конца строки
 * 
 * @param str строка для копирования
 * @param n количество символов
 * @return char* новый буфер с копией строки
 */
char* dup_nstring(const char* str, size_t n);

/**
 * @brief Считывает число из строки
 * 
 * @param res результат
 * @param src строка с числом
 * @return int код ошибки
 */
int parse_int(int* res, const char* src);

/**
 * @brief Дублирует строку с проверкой на ошибки
 * 
 * @param res результат
 * @param str исходная строка
 * @return int код ошибки
 */
int parse_string(char** res, const char* str);

/**
 * @brief Обновляет динамическую строку
 * 
 * @param res строка, которую необходимо обновить
 * @param str источник
 * @return int код ошибки
 */
int update_string(char** res, const char* str);

#endif
