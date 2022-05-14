#include <stdlib.h>
#include "error_codes.h"
#include "mem_track/memtrack.h"
#include "list.h"

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

list_t init_list(void)
{
    return (list_t) { .head = NULL, .tail = NULL };
}

size_t list_size(const list_t* list)
{
    size_t res = 0;

    if (list != NULL)
        for (node_t* node = list->head; node != NULL; node = node->next)
            res++;

    return res;
}

int push_back(list_t* dst, const product_t* src)
{
    if (dst == NULL || src == NULL)
        return INVALID_PARAMS;

    node_t* node = make_node(src);
    if (!node)
        return MEM_ERR;

    if (dst->tail == NULL)
        dst->head = node;
    else
        dst->tail->next = node;
    dst->tail = node;

    return SUCCESS;
}

int erase(list_t* dst, node_t* node)
{
    if (dst->head == NULL)
        return EMPTY_LIST_ERR;
    
    if (dst->head == dst->tail && dst->head == node)
    {
        free_product(&node->data);
        free(node);
        track_free();

        dst->head = NULL;
        dst->tail = NULL;
        return SUCCESS;
    }
    else if (dst->head == node)
    {
        dst->head = dst->head->next;
        free_product(&node->data);
        free(node);
        track_free();
        return SUCCESS;
    }

    node_t* head = dst->head;

    while (head->next != node)
        head = head->next;

    head->next = node->next;

    free_product(&node->data);
    free(node);
    track_free();

    return SUCCESS;
}

int clear(list_t* dst)
{
    if (dst == NULL)
        return INVALID_PARAMS;
    
    while (dst->head != NULL)
    {
        node_t* next = dst->head->next;

        free_product(&dst->head->data);
        free(dst->head);
        track_free();

        dst->head = next;
    }

    dst->tail = NULL;

    return SUCCESS;
}
