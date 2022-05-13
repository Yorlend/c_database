#ifndef __LIST_H__
#define __LIST_H__

#include "db_entry/product.h"

typedef struct node
{
    product_t data;
    struct node* next;
} node_t;

typedef struct list
{
    node_t* head;
    node_t* tail;
} list_t;

/**
 * @brief Создает узел из конкретного продукта
 * 
 * @param product продукт, содержащийся в узле
 * @return node_t* указатель на узел
 */
node_t* make_node(const product_t* product);

/**
 * @brief Инициализация списка нулевыми значениями
 * 
 * @return list_t 
 */
list_t init_list(void);

/**
 * @brief Возвращает число элементов в списке
 * 
 * @param list список
 * @return size_t 
 */
size_t list_size(const list_t* list);

/**
 * @brief Вставка в хвост списка
 * 
 * @param dst спискок, в который вставляем
 * @param src продукт, который вставляем
 * @return int код ошибки
 */
int push_back(list_t* dst, const product_t* src);

/**
 * @brief Удаление элемента из списка по указателю на узел
 * 
 * @param dst список, из которого удаляем
 * @param node узел, который удаляем
 * @return int код ошибки
 */
int erase(list_t* dst, node_t* node);

/**
 * @brief Удаление всех элементов списка
 * 
 * @param dst список, подлежащий очистке
 * @return int код ошибки
 */
int clear(list_t* dst);

#endif
