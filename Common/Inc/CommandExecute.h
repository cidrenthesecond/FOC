#ifndef COMMAND_EXECUTE_H
#define COMMAND_EXECUTE_H

#include "stdint.h"

enum {
    CMD_FAIL = 0,
    CMD_SUCCESS = 1
};

typedef void (*CommandFunc)(int argc, char *argv[]);

void CommandExecute_Init();
int CommandExecute_Register(uint32_t commandIndex,CommandFunc command);
int CommandExecute(const char*input);
void CommandExecute_Destroy();


#endif