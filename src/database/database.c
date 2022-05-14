#include <stdbool.h>
#include <string.h>
#include "error_codes.h"
#include "mem_track/memtrack.h"
#include "list/list.h"
#include "utils/pair_array.h"
#include "io/userio.h"
#include "database.h"

#define ENTRY_FIELDS_COUNT 7

list_t entry_list;

// static bool fields_format_valid(const char** pairs, size_t size)
// {
//     for (size_t i = 0; i < size; i++)
//         if (strchr(pairs[i], '=') == NULL)
//             return false;

//     return true;
// }

// static bool has_duplicate_fields(const char** pairs, size_t size)
// {
//     for (size_t i = 0; i < size; i++)
//     {
//         int eq_pos_i = strchr(pairs[i], '=') - pairs[i];
//         for (size_t j = i + 1; j < size; j++)
//         {
//             int eq_pos_j = strchr(pairs[j], '=') - pairs[j];
//             if (eq_pos_i == eq_pos_j && strncmp(pairs[i], pairs[j], eq_pos_i) == 0)
//                 return true;
//         }
//     }

//     return false;
// }

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
    
    ///TODO: check duplicates
    // if (has_duplicate_fields(pairs))
    //     return DUPLICATE_FIELDS;

    product_t product = init_product();
    int status = construct_product(&product, pairs);
    if (status == SUCCESS)
        status = push_back(&entry_list, &product);

    return status;
}

static int output_product_field(const char* field, const product_t* product)
{
    if (strncmp(field, "comes", 5) == 0)
    {
        char datebuf[DATE_LEN];
        date_to_str(datebuf, &product->comes);
        printf("comes=%s", datebuf);
    }
    else if (strncmp(field, "sender", 6) == 0)
        printf("sender=%s", product->sender);
    else if (strncmp(field, "name", 4) == 0)
        printf("name=%s", product->name);
    else if (strncmp(field, "weight", 6) == 0)
        printf("weight=%d", product->weight);
    else if (strncmp(field, "count", 5) == 0)
        printf("count=%d", product->count);
    else if (strncmp(field, "images", 6) == 0)
    {
        printf("images=");
        print_image_set(&product->images);
    }
    else if (strncmp(field, "worker", 6) == 0)
        printf("worker=%s", product->worker);
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
        printf(" ");
        fields++;
    }
    printf("\n");
}

static int count_matching(size_t* count, const cond_array_t* conds)
{
    *count = 0;

    int status = SUCCESS;
    for (node_t* node = entry_list.head; status == SUCCESS && node != NULL; node = node->next)
    {
        bool match_flag = true;
        for (size_t i = 0; status == SUCCESS && match_flag && i < conds->size; i++)
        {
            bool tmp;
            status = cond_match(&tmp, conds->data + i, &node->data);
            if (status == SUCCESS)
                match_flag = match_flag && tmp;
        }

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

    printf("select: %zu\n", count);

    for (node_t* node = entry_list.head; status == SUCCESS && node != NULL; node = node->next)
    {
        bool match_flag = true;
        for (size_t i = 0; status == SUCCESS && match_flag && i < conds->size; i++)
        {
            bool tmp;
            status = cond_match(&tmp, conds->data + i, &node->data);
            if (status == SUCCESS)
                match_flag = match_flag && tmp;
        }

        // вывести поля товара подходящего по условиям поиска
        if (status == SUCCESS && match_flag)
            output_product_fields(fields, &node->data);
    }

    return SUCCESS;
}

int db_delete(const cond_array_t* conds)
{
    if (conds == NULL)
        return INVALID_PARAMS;
    
    size_t count = 0;
    int status = SUCCESS;
    for (node_t* node = entry_list.head; status == SUCCESS && node != NULL;)
    {
        bool match_flag = true;
        for (size_t i = 0; status == SUCCESS && match_flag && i < conds->size; i++)
        {
            bool tmp;
            status = cond_match(&tmp, conds->data + i, &node->data);
            if (status == SUCCESS)
                match_flag = match_flag && tmp;
        }

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
        printf("delete: %zu\n", count);

    return SUCCESS;
}

static int cmp_by_field(bool *eql, const char* field, const product_t* product1, const product_t* product2)
{
    if (strncmp(field, "comes", 5) == 0)
        *eql = date_eq(&product1->comes, &product2->comes);
    else if (strncmp(field, "sender", 6) == 0)
        *eql = strcmp(product1->sender, product2->sender) == 0;
    else if (strncmp(field, "name", 4) == 0)
        *eql = strcmp(product1->name, product2->name) == 0;
    else if (strncmp(field, "weight", 6) == 0)
        *eql = product1->weight == product2->weight;
    else if (strncmp(field, "count", 5) == 0)
        *eql = product1->count == product2->count;
    else if (strncmp(field, "images", 6) == 0)
        *eql = image_set_eq(&product1->images, &product2->images);
    else if (strncmp(field, "worker", 6) == 0)
        *eql = strcmp(product1->worker, product2->worker) == 0;
    else
        return UNKNOWN_FIELD;

    return SUCCESS;
}

/**
 * @brief Сравнивает два товара по переданным полям
 * 
 * @param eql true, если равны
 * @param fields строка со списком полей в формате 'field1,field2,...'
 * @param product1 
 * @param product2 
 * @return int код ошибки
 */
static int cmp_by_fields(bool *eql, const char* fields, const product_t* product1, const product_t* product2)
{
    *eql = true;
    while (*eql)
    {
        bool tmp;
        int status = cmp_by_field(&tmp, fields, product1, product2);
        if (status != SUCCESS)
            return status;
        *eql = *eql && tmp;

        fields = strchr(fields, ',');
        if (fields == NULL)
            break;
        fields++;
    }

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

    printf("uniq: %zu\n", count);
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
