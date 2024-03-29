#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "date.h"
#include "error_codes.h"

date_t init_date()
{
    return (date_t) {.day = 0, .month = 0, .year = 0};
}

static int parse_quote(const char** str)
{
    if (**str != '\'')
        return BAD_INPUT;
    
    (*str)++;
    return SUCCESS;
}

static int parse_day(date_t* dst, const char** src)
{
    int day = 0;
    while (isdigit(**src))
    {
        day = day * 10 + **src - '0';
        (*src)++;
    }

    if (**src != '.' || day < 1 || day > 31)
        return BAD_INPUT;

    (*src)++;
    
    dst->day = day;

    return SUCCESS;
}

static int parse_month(date_t* dst, const char** src)
{
    int month = 0;
    while (isdigit(**src))
    {
        month = month * 10 + **src - '0';
        (*src)++;
    }

    if (**src != '.' || month < 1 || month > 12)
        return BAD_INPUT;
    
    (*src)++;

    dst->month = month;

    return SUCCESS;
}

static int parse_year(date_t* dst, const char** src)
{
    int year = 0;
    while (isdigit(**src))
    {
        year = year * 10 + **src - '0';
        (*src)++;
    }

    if (year < 1)
        return BAD_INPUT;
    
    dst->year = year;

    return SUCCESS;
}

int parse_date(date_t* dst, const char* src)
{
    if (src == NULL || dst == NULL)
        return INVALID_PARAMS;

    if (parse_quote(&src) == SUCCESS &&
        parse_day(dst, &src) == SUCCESS && 
        parse_month(dst, &src) == SUCCESS && 
        parse_year(dst, &src) == SUCCESS &&
        parse_quote(&src) == SUCCESS)
        return SUCCESS;

    return BAD_INPUT;
}

int date_to_str(char* dst, const date_t* src)
{
    if (dst == NULL || src == NULL)
        return INVALID_PARAMS;

    snprintf(dst, DATE_LEN, "'%02d.%02d.%04d'",
        src->day, src->month, src->year);
    return SUCCESS;
}

static inline bool is_leap_year(int year)
{
    return year % 400 == 0 || (year % 100 != 0 && year % 4 == 0);
}

bool date_valid(const date_t* date)
{
    if (date->day < 1 || 31 < date->day)
        return false;

    if (date->month < 1 || 12 < date->month)
        return false;

    if (date->month == 2)
    {
        if (is_leap_year(date->year))
        {
            if (date->day > 29)
                return false;
        }
        else if (date->day > 28)
            return false;
    }

    if ((date->month - 1 + (date->month > 6)) % 2 == 1)
        if (date->day > 30)
            return false;

    return true; 
}

bool date_eq(const date_t* date1, const date_t* date2)
{
    return date1->day == date2->day &&
        date1->month == date2->month &&
        date1->year == date2->year;
}

bool date_lt(const date_t* date1, const date_t* date2)
{
    if (date1->year == date2->year)
    {
        if (date1->month == date2->month)
            return date1->day < date2->day;
        return date1->month < date2->month;
    }
    return date1->year < date2->year;
}
