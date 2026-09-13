#include "CommandExecute.h"
#include "stdint.h"
#include "string.h"
#include <stdlib.h>

#define MAX_COMMANDS        20
#define MAX_ARGS            5 
#define COMMAND_BUFFER_SIZE 64

//Commands are structured this way...
//cmd xxx <arg1> <arg2> <argx> ...

//where:
//cmd - command execute
//xxx - command index
//<argX> - arguments
//example:
//cmd 16 1


typedef void (*CommandFunc)(int argc, char *argv[]);

typedef struct {
    CommandFunc func;
} Command;

static Command CommandList[MAX_COMMANDS] = {0};
static uint8_t isInitalised = 0;

static int isCommandIndexValid(uint32_t commandIndex);

void CommandExecute_Init()
{
    for(uint8_t index = 0; index < MAX_COMMANDS; index++)
        CommandList->func = NULL;

    isInitalised = 1;
}


int CommandExecute_Register(uint32_t commandIndex,CommandFunc command)
{
    if(!isInitalised)
        return;

    if(!isCommandIndexValid)
        return;

    if(CommandList[commandIndex].func != NULL)
        return;

    CommandList[commandIndex].func = command;
}

int CommandExecute(const char*input)
{
    char buffer[COMMAND_BUFFER_SIZE];
    strncpy(buffer,input,COMMAND_BUFFER_SIZE);
    buffer[COMMAND_BUFFER_SIZE - 1] = '\0';

    buffer[strcspn(buffer,"\r\n")] = 0;

    char *argv[MAX_ARGS + 2];
    int argc = 0;
    
    char *token = strtok(buffer, " ");
    while(token != NULL && argc < MAX_ARGS)
    {
        argv[argc++] = token;
        token = strtok(NULL, " ");
    }

    if(argc < 2)
        return;

    if(strcmp("cmd",argv[0]) != 0)
        return;

    char *end;
    long commandIndex = strtol(argv[1], &end, 10);

    if(*end != '\0')
        return;
    if(commandIndex < 1 || commandIndex >= MAX_COMMANDS)
        return;

    if(CommandList[commandIndex].func == NULL)
        return;

    CommandList[commandIndex].func(argc - 2,&argv[2]);
}

void CommandExecute_Destroy()
{
    for(uint8_t index = 0; index < MAX_COMMANDS; index++)
        CommandList[index].func = NULL;

    isInitalised = 0;
}

static int isCommandIndexValid(uint32_t commandIndex)
{
    return commandIndex == 0 || commandIndex >= MAX_COMMANDS;
}