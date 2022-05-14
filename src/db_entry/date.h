#ifndef __DATE_H__
#define __DATE_H__

#include <stdbool.h>

// длина строки с датой
#define DATE_LEN 12

typedef struct
{
    int day;
    int month;
    int year;
} date_t;

/**
 * @brief инициализирует дату нулевыми значениями
 * 
 * @return date_t 
 */
date_t init_date();

/**
 * @brief Преобразует строку в дату
 * 
 * @param dst результат - дата
 * @param src строка, содержащая дату в формате ДД.ММ.ГГГГ
 * @return int код ошибки
 */
int parse_date(date_t* dst, const char* src);

/**
 * @brief Преобразует дату в строку
 * 
 * @param dst результат - строка в формате ДД.ММ.ГГГГ
 * @param src исходная дата
 * @return int код ошибки
 */
int date_to_str(char* dst, const date_t* src);

bool date_valid(const date_t* date);

bool date_eq(const date_t* date1, const date_t* date2);
bool date_lt(const date_t* date1, const date_t* date2);

#endif
