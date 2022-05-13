#ifndef __COMMAND_H__
#define __COMMAND_H__

/**
 * @brief Выполняет команду в строке
 * 
 * @param command_line строка с командой
 * @return int код ошибки
 */
int db_cmd(const char* command_line);

#endif
