#ifndef __ERROR_CODES_H__
#define __ERROR_CODES_H__

#define SUCCESS                  0 // Успешное завершение
#define FILE_ERROR               1 // Ошибка записи/чтения из файла
#define INVALID_OP               2 // Некорректная операция
#define MEM_ERR                  3 // Ошибка выделения памяти
#define INVALID_PARAMS           4 // Некорректные аргументы функции
#define BAD_INPUT                5 // Некорректный ввод
#define EMPTY_LIST_ERR           6 // Удаление из пустого списка
#define INVALID_FIELDS_COUNT     7 // Неверное количество полей
#define INVALID_FIELDS_FORMAT    8 // Неверный формат параметров команды
#define DUPLICATE_FIELDS         9 // Дублирование полей в параметрах команды
#define UNKNOWN_FIELD           10 // Неизвестное поле
#define INVALID_IMAGE_OPTION    11 // Неверный элемент множества
#define INVALID_COMMAND         12 // Неверная команда
#define INVALID_CONDITION       13 // Неверное условие фильтрации
#define UNKNOWN_COND_TYPE       14 // Неизвестный тип условия
#define INVALID_COND_TYPE       15 // Некорректный тип условия
#define INVALID_SET_FORMAT      16 // Неверный формат множества
#define OUTPUT_ERROR            17 // Ошибка вывода

#endif
