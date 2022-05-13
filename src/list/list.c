#include "list.h"

#include <stdlib.h>
#include "error_codes.h"
#include "mem_track/memtrack.h"

node_t* make_node(const product_t* product)
{
    node_t* node = malloc(sizeof(node_t));

    if (!node)
        return NULL;

    track_malloc();

    node->data = *product;
    node->next = NULL;

    return node;
}

list_t init_list()
{
    return (list_t) { .head = NULL, .tail = NULL };
}

int push_back(list_t* dst, const product_t* src)
{
    node_t* node = make_node(src);

    if (!node)
        return MEM_ERR;

    if (dst->tail == NULL)
    {
        dst->head = node;
        dst->tail = node;
    }
    else
        dst->tail->next = node;

    return SUCCESS;
}

int erase(list_t* dst, node_t* node)
{
    if (dst->head == NULL)
        return EMPTY_LIST_ERR;

    node_t* head = dst->head;

    while (head->next != node)
        head++;

    head->next = node->next;
    free(node);

    track_free();

    return SUCCESS;
}
