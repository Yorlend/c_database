#ifndef __IMAGE_SET_H__
#define __IMAGE_SET_H__

#include <stdbool.h>

typedef struct
{
    bool fragile;
    bool toxic;
    bool perishable;
    bool acrid;
    bool inflammable;
    bool frozen;
} image_set_t;

/**
 * @brief Создаёт пустую структуру
 * 
 * @return image_set_t 
 */
image_set_t init_image_set(void);

/**
 * @brief Создание множества по строке
 * 
 * @param set результат
 * @param str строка в формате ['value1','value2','...']
 * @return int код ошибки
 */
int parse_image_set(image_set_t* set, const char* str);

/**
 * @brief Добавление элемента по строке
 * 
 * @param set - множество значков
 * @param option - добавляемый элемент
 * @return int - код ошибки
 */
int image_set_add(image_set_t* set, const char* option);

/**
 * @brief Проверка покрытия множества src множеством target
 * 
 * @param src - покрываемое множество
 * @param target - покрывающее множество
 * @return true если элементы из src содержатся в target
 * @return false если элементы из src не содержатся в target
 */
bool image_set_in(const image_set_t* src, const image_set_t* target);

bool image_set_eq(const image_set_t* set1, const image_set_t* set2);

void print_image_set(const image_set_t* set);

#endif
