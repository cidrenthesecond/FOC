#include "LOG_Spy.h"



static char message[LOG_SPY_MAX_LEN];
static int callCount;

void LOG_Spy_Init()
{
    callCount = 0;
    message[0] = '\0';
}

int LOG(const char* text)
{
    callCount++;
    int index = 0;
    while(text[index] != '\0')
    {
        message[index] = text[index];
        index++;
    }

    message[index] = '\0';

    return 1;
}

char *LOG_Spy_GetMessage()
{
    return message;
}

int LOG_Spy_GetCallCount()
{
    return callCount;
}