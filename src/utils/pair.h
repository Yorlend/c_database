#ifndef __PAIR_H__
#define __PAIR_H__


typedef struct pair
{
    char* field;
    char* value;
} pair_t;

/**
 * @brief Инициализирует пару нулевыми значениями
 * 
 * @return pair_t 
 */
pair_t init_pair();

/**
 * @brief Преобразует строку в формате field=value в пару
 * 
 * @param dst результат - пара
 * @param src источник - строка
 * @return int код ошибки
 */
int parse_pair(pair_t* dst, const char* src);

/**
 * @brief Преобразует пару в строку формата field=value
 * 
 * @param dst результат - строка
 * @param src источник - пара
 * @return int код ошибки
 */
int pair_to_str(char* dst, const pair_t* src);

#endif
