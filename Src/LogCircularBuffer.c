#include "LogCircularBuffer.h"
#include <string.h>

#define TERMINATION_SIGN_LEN 1

static char *bufferPtr = NULL;

static volatile uint32_t putIndex;
static volatile uint32_t getIndex;
static uint32_t bufferSize;
static volatile uint32_t used;
static volatile uint32_t logLen;

void LogCircularBuffer_Init(char * buffer,uint32_t size)
{
    memset(buffer,' ',size);

    bufferPtr = buffer;
    putIndex = 0;
    getIndex = 0;
    bufferSize = size;
    logLen = 0;
    used = 0;
}

LogBufferStatus_t LogCircularbuffer_Get(char **getText, uint32_t *getSize)
{
    if(bufferPtr == NULL)
        return BUFFER_NO_INIT;

    if(used == 0)
        return BUFFER_EMPTY;

    *getText = bufferPtr + getIndex;

    logLen = 0;

    for(uint32_t index = 0; getIndex + index < bufferSize; index++)
    {
        if(bufferPtr[getIndex + index] == '\0')
        {
            logLen = index + 1;
            *getSize = index + 1;
            return LOG_FULL;
        }

        logLen = index + 1;
    }
        
    *getSize = logLen;

    return LOG_SPLITTED;
}

LogBufferStatus_t LogCircularBuffer_Put(char *data)
{
    if(bufferPtr == NULL)
        return BUFFER_NO_INIT;

    uint32_t freeSpace = bufferSize - used;

    if(freeSpace == 0)
        return BUFFER_FULL;

    uint32_t dataLen = strlen(data);
    uint32_t writeLen = dataLen;
    LogBufferStatus_t status = BUFFER_SUCCES;

    if(dataLen + TERMINATION_SIGN_LEN > freeSpace)
    {
        writeLen = freeSpace - TERMINATION_SIGN_LEN;        
        status = LOG_TRUNCATED;
    }

    for(uint32_t i = 0; i < writeLen; i++)
    {
        bufferPtr[putIndex] = data[i];
        putIndex = (putIndex + 1) % bufferSize;
        used++;
    }

    bufferPtr[putIndex] = '\0';
    putIndex = (putIndex + 1) % bufferSize;
    used++;

    return status;
}

void LogCircularBuffer_AdvanceGetIndex()
{
    memset(bufferPtr + getIndex,' ',logLen);
    getIndex = (getIndex + logLen) % bufferSize;
    used -= logLen;
    logLen = 0;
}

void LogCircularBuffer_Destroy()
{
    bufferPtr = NULL;
}