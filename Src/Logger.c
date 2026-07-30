#include "Logger.h"
#include "string.h"

static void (*PrintLog)(const char *pData,uint8_t length) = NULL;

static volatile uint8_t isInitialized = 0;
static uint8_t isBusy = 0;
FIFO queue;

int LOG_Init(void (*ExternPrintLog)(const char *pData,uint8_t length),
             FIFO fifo)       
{   
    if(ExternPrintLog == NULL)
        return LOGGER_FAIL;
    
    if(fifo == NULL)
        return LOGGER_FAIL;

    PrintLog = ExternPrintLog;
    queue = fifo;
    isInitialized = 1;
    isBusy = 0;

    return LOGGER_SUCCESS;
}

int LOG(const char* text)
{   
    if(!isInitialized)
        return LOGGER_NOINIT;

    FIFO_Put(queue,text);

    if(isBusy)
        return LOG_SCHEDULED;

    char* resultText;
    uint8_t resultLen;
    FIFO_Get(queue,&resultText,&resultLen);
    
    isBusy = 1;
    PrintLog(resultText,resultLen);
    
    return LOGGER_SUCCESS;
}

int LOG_WakeUp()
{
    if(!isInitialized)
        return LOGGER_NOINIT;

    char* text;
    uint8_t len;

    if(FIFO_Get(queue,&text,&len) == FIFO_EMPTY)
    {
        isBusy = 0;
        return LOGGER_SUCCESS;
    }

    isBusy = 1;
    PrintLog(text,len);

    return LOGGER_SUCCESS;
}

void * LOG_GetPrintingFunction()
{
    return PrintLog;
}

void LOG_Destroy()
{   
    isInitialized = 0;
    PrintLog = NULL;
}
