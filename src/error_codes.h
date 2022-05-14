#ifndef __ERROR_CODES_H__
#define __ERROR_CODES_H__

#define SUCCESS                  0 // Успешное завершение
#define FILE_ERROR               1 // Ошибка записи/чтения из файла
#define INVALID_OP               2 // Некорректная операция
#define MEM_ERR                  3 // Ошибка выделения памяти
#define INVALID_PARAMS           4 // Некорректные аргументы функции
#define BAD_INPUT                5 // Некорректный ввод
#define EMPTY_LIST_ERR           6 // Удаление из пустого списка
#define INVALID_FIELDS_COUNT     7
#define INVALID_FIELDS_FORMAT    8
#define DUPLICATE_FIELDS         9
#define UNKNOWN_FIELD           10
#define INVALID_IMAGE_OPTION    11
#define INVALID_COMMAND         12
#define INVALID_CONDITION       13
#define UNKNOWN_COND_TYPE       14
#define INVALID_COND_TYPE       15

#endif
