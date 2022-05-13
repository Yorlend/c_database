#ifndef __USERIO_H__
#define __USERIO_H__

#include <stdio.h>

/**
 * @brief Записывает строку неопределенной или определенной длины
 * 
 * @param lineptr указатель на указатель на строку
 * @param n количество записываемых символов или 0
 * @param stream файловый указатель
 * @return size_t количество считанных символов
 */
size_t getline_win(char** lineptr, size_t* n, FILE* stream);

#endif