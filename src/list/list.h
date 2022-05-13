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

node_t make_node(const product_t* product);

list_t init_list();

int push_back(list_t* dst, const node_t* src);

#endif
