#include "CommandExecute.h"
#include "string.h"
#include <stdlib.h>

#define COMMAND_BUFFER_SIZE 64
#define MAX_LIST_INDEX      MAX_COMMANDS+1

//Commands are structured this way...
//cmd xxx <arg1> <arg2> <argx> ...

//where:
//cmd - command execute
//xxx - command index
//<argX> - arguments
//example:
//cmd 16 1

typedef struct {
    CommandFunc func;
} Command;

static Command CommandList[MAX_COMMANDS+1] = {0};
static uint8_t isInitalised = 0;

static int isCommandIndexValid(uint32_t commandIndex);

void CommandExecute_Init()
{
    for(uint8_t index = 0; index < MAX_COMMANDS; index++)
        CommandList[index].func = NULL;

    isInitalised = 1;
}


int CommandExecute_Register(uint32_t commandIndex,CommandFunc command)
{
    if(!isInitalised)
        return CMD_FAIL;

    if(!isCommandIndexValid(commandIndex))
        return CMD_FAIL;

    if(command == NULL)
        return CMD_FAIL;

    if(CommandList[commandIndex].func != NULL)
        return CMD_FAIL;

    CommandList[commandIndex].func = command;
    return CMD_SUCCESS;
}

int CommandExecute(const char*input)
{
    if(input == NULL)
        return CMD_FAIL;

    char buffer[COMMAND_BUFFER_SIZE];
    strncpy(buffer,input,COMMAND_BUFFER_SIZE);
    buffer[COMMAND_BUFFER_SIZE - 1] = '\0';

    buffer[strcspn(buffer,"\r\n")] = 0;

    char *argv[MAX_ARGS + 2];
    int argc = 0;
    
    char *token = strtok(buffer, " ");
    while(token != NULL && argc < (MAX_ARGS + 2))
    {
        argv[argc++] = token;
        token = strtok(NULL, " ");
    }

    if(argc < 2)
        return CMD_FAIL;

    if(strcmp("cmd",argv[0]) != 0)
        return CMD_FAIL;

    char *end;
    long commandIndex = strtol(argv[1], &end, 10);

    if(*end != '\0')
        return CMD_FAIL;

    if(commandIndex < 1 || commandIndex > MAX_COMMANDS)
        return CMD_FAIL;

    if(CommandList[commandIndex].func == NULL)
        return CMD_FAIL;

    CommandList[commandIndex].func(argc - 2,&argv[2]);
    return CMD_SUCCESS;
}

void CommandExecute_Destroy()
{
    for(uint8_t index = 0; index <= MAX_COMMANDS; index++)
        CommandList[index].func = NULL;

    isInitalised = 0;
}

static int isCommandIndexValid(uint32_t commandIndex)
{
    return (commandIndex != 0 && commandIndex <= MAX_COMMANDS);
}