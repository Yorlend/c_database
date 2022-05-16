#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "error_codes.h"
#include "mem_track/memtrack.h"
#include "list/list.h"
#include "utils/pair_array.h"
#include "io/userio.h"
#include "utils/logger.h"
#include "database.h"

#define ENTRY_FIELDS_COUNT 7

static list_t entry_list;

static bool has_duplicate_fields(const pair_array_t* pairs)
{
    for (size_t i = 0; i < pairs->size; i++)
        for (size_t j = i + 1; j < pairs->size; j++)
            if (strcmp(pairs->data[i].field, pairs->data[j].field) == 0)
                return true;

    return false;
}

int construct_product(product_t* product, const pair_array_t* pairs)
{
    int status = SUCCESS;
    for (size_t i = 0; status == SUCCESS && i < pairs->size; i++)
    {
        pair_t pair = pairs->data[i];

        if (strcmp(pair.field, "comes") == 0)
            status = parse_date(&product->comes, pair.value);
        else if (strcmp(pair.field, "sender") == 0)
            status = parse_string(&product->sender, pair.value);
        else if (strcmp(pair.field, "name") == 0)
            status = parse_string(&product->name, pair.value);
        else if (strcmp(pair.field, "weight") == 0)
            status = parse_int(&product->weight, pair.value);
        else if (strcmp(pair.field, "count") == 0)
            status = parse_int(&product->count, pair.value);
        else if (strcmp(pair.field, "images") == 0)
            status = parse_image_set(&product->images, pair.value);
        else if (strcmp(pair.field, "worker") == 0)
            status = parse_string(&product->worker, pair.value);
        else
            status = UNKNOWN_FIELD;
    }

    if (status != SUCCESS)
        free_product(product);
    
    return status;
}

int db_insert(const pair_array_t* pairs)
{
    if (pairs == NULL)
        return INVALID_PARAMS;
    
    if (pairs->size != 7)
        return INVALID_FIELDS_COUNT;

    if (has_duplicate_fields(pairs))
        return DUPLICATE_FIELDS;

    product_t product = init_product();
    int status = construct_product(&product, pairs);
    if (status == SUCCESS)
        status = push_back(&entry_list, &product);

    if (status == SUCCESS)
        log_printf("insert: %zu\n", db_rows_count());

    return status;
}

static int output_product_field(const char* field, const product_t* product)
{
    if (strncmp(field, "comes", 5) == 0)
    {
        char datebuf[DATE_LEN];
        date_to_str(datebuf, &product->comes);
        log_printf("comes=%s", datebuf);
    }
    else if (strncmp(field, "sender", 6) == 0)
        log_printf("sender=%s", product->sender);
    else if (strncmp(field, "name", 4) == 0)
        log_printf("name=%s", product->name);
    else if (strncmp(field, "weight", 6) == 0)
        log_printf("weight=%d", product->weight);
    else if (strncmp(field, "count", 5) == 0)
        log_printf("count=%d", product->count);
    else if (strncmp(field, "images", 6) == 0)
    {
        log_printf("images=");
        print_image_set(&product->images);
    }
    else if (strncmp(field, "worker", 6) == 0)
        log_printf("worker=%s", product->worker);
    else
        return UNKNOWN_FIELD;

    return SUCCESS;
}

static int output_product_fields(const char* fields, const product_t* product)
{
    while (true)
    {
        output_product_field(fields, product);
        fields = strchr(fields, ',');
        if (fields == NULL)
            break;
        log_printf(" ");
        fields++;
    }
    log_printf("\n");
}

static int count_matching(size_t* count, const cond_array_t* conds)
{
    *count = 0;

    int status = SUCCESS;
    for (node_t* node = entry_list.head; status == SUCCESS && node != NULL; node = node->next)
    {
        bool match_flag;
        status = cond_array_match(&match_flag, conds, &node->data);

        if (status == SUCCESS && match_flag)
            (*count)++;
    }

    return status;
}

int db_select(const char* fields, const cond_array_t* conds)
{
    if (fields == NULL || conds == NULL)
        return INVALID_PARAMS;
    
    size_t count = 0;
    int status = count_matching(&count, conds);

    if (status != SUCCESS)
        return status;

    log_printf("select: %zu\n", count);

    for (node_t* node = entry_list.head; status == SUCCESS && node != NULL; node = node->next)
    {
        bool match_flag;
        status = cond_array_match(&match_flag, conds, &node->data);

        // вывести поля товара подходящего по условиям поиска
        if (status == SUCCESS && match_flag)
            output_product_fields(fields, &node->data);
    }

    return status;
}

static int update_product_field(product_t* product, const pair_t* pair)
{
    int status = SUCCESS;

    if (strcmp(pair->field, "comes") == 0)
        status = parse_date(&product->comes, pair->value);
    else if (strcmp(pair->field, "sender") == 0)
        status = update_string(&product->sender, pair->value);
    else if (strcmp(pair->field, "name") == 0)
        status = update_string(&product->name, pair->value);
    else if (strcmp(pair->field, "weight") == 0)
        status = parse_int(&product->weight, pair->value);
    else if (strcmp(pair->field, "count") == 0)
        status = parse_int(&product->count, pair->value);
    else if (strcmp(pair->field, "images") == 0)
        status = parse_image_set(&product->images, pair->value);
    else if (strcmp(pair->field, "worker") == 0)
        status = update_string(&product->worker, pair->value);
    else
        status = UNKNOWN_FIELD;

    return status;
}

static int update_product_fields(product_t* product, const pair_array_t* pairs)
{
    int status = SUCCESS;
    for (size_t i = 0; status == SUCCESS && i < pairs->size; i++)
        status = update_product_field(product, pairs->data + i);
    return status;
}

int db_update(const pair_array_t* pairs, const cond_array_t* conds)
{
    if (pairs == NULL || conds == NULL)
        return INVALID_PARAMS;

    int status = SUCCESS;

    size_t count = 0;
    for (node_t* node = entry_list.head; status == SUCCESS && node != NULL; node = node->next)
    {
        bool match_flag;
        status = cond_array_match(&match_flag, conds, &node->data);

        // обновить поля товара подходящего по условиям поиска
        if (status == SUCCESS && match_flag)
        {
            status = update_product_fields(&node->data, pairs);
            count++;
        }
    }

    if (status == SUCCESS)
        log_printf("update: %zu\n", count);

    return status;
}

int db_delete(const cond_array_t* conds)
{
    if (conds == NULL)
        return INVALID_PARAMS;
    
    size_t count = 0;
    int status = SUCCESS;
    for (node_t* node = entry_list.head; status == SUCCESS && node != NULL;)
    {
        bool match_flag;
        status = cond_array_match(&match_flag, conds, &node->data);

        // удалить из списка товар подходящий по условиям поиска
        if (status == SUCCESS && match_flag)
        {
            node_t* next = node->next;
            status = erase(&entry_list, node);
            node = next;
            count++;
        }
        else
            node = node->next;
    }

    if (status == SUCCESS)
        log_printf("delete: %zu\n", count);

    return SUCCESS;
}

int db_unique(const char* fields)
{
    size_t count = 0;

    int status = SUCCESS;
    for (node_t* node1 = entry_list.head; status == SUCCESS && node1 != NULL;)
    {
        bool was_remove = false;
        for (node_t* node2 = node1->next; status == SUCCESS && node2 != NULL; node2 = node2->next)
        {
            bool eql;
            status = cmp_by_fields(&eql, fields, &node1->data, &node2->data);
            if (status == SUCCESS && eql)
            {
                node_t* next = node1->next;
                erase(&entry_list, node1);
                node1 = next;
                was_remove = true;
                count++;
                break;
            }
        }

        if (!was_remove)
            node1 = node1->next;
    }

    if (status == SUCCESS)
        log_printf("uniq: %zu\n", count);
    return status;
}

size_t db_rows_count(void)
{
    return list_size(&entry_list);
}

void db_clear(void)
{
    clear(&entry_list);
}
