#include "Logger.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>


static void (*PrintLog)(const char *pData,uint8_t length) = NULL;

static uint8_t isLoggerBusy;
static uint8_t scheduledLogsNum;

enum{
    TERMINATION_SIGN = 1
};

static char *scheduledText;
static char *putPtr;
static char *getPtr;
static uint32_t bufferSize;

void LOG_Init(void (*ExternPrintLog)(const char *pData,uint8_t length),
              char *logBuffer,
              uint32_t size)
{
    PrintLog = ExternPrintLog;
    isLoggerBusy = 0;
    scheduledLogsNum = 0;
    scheduledText = logBuffer;
    bufferSize = size;

    memset(scheduledText,0,bufferSize);

    putPtr = logBuffer;
    getPtr = logBuffer;   

}

int LOG(const char* text)
{   
    if(PrintLog == NULL)
        return LOGGER_NOINIT;

    if(text == NULL)
        return LOGGER_NULL_PTR;

    if(isLoggerBusy)
    {
        scheduledLogsNum++;
        //strcpy(putPtr,text);
        while(*text != '\0')
        {
            *putPtr = *text;
            putPtr++;
            text++;
        }
        *putPtr = '\0';
        putPtr++;
        //putPtr+= strlen(text) + TERMINATION_SIGN;
        return LOGGER_BUSY;
    }
        

    PrintLog(text,strlen(text));

    isLoggerBusy = 1;
    return LOGGER_SUCCES;
}

void LOG_WakeUp()
{
    isLoggerBusy = 0;

    if(scheduledLogsNum > 0)
    {
        LOG(getPtr);
        getPtr += strlen(getPtr) + TERMINATION_SIGN;
        scheduledLogsNum--;
    }
}

void * LOG_GetPrintingFunction()
{
    return PrintLog;
}

void LOG_Destroy()
{
    PrintLog = NULL;
    free(scheduledText);
}
