#include "LogCircularBuffer.h"
#include <string.h>

// typedef struct LogCircularBufferStruct {
//     char *bufferPtr;

//     uint32_t putIndex;
//     uint32_t getIndex;
//     uint32_t bufferSize;
    
// } CircularBufferStruct;

#define TERMINATION_SIGN_LEN 1

static char *bufferPtr = NULL;

static volatile uint32_t putIndex;
static volatile uint32_t getIndex;
static uint32_t bufferSize;
static volatile uint32_t used;
static volatile uint32_t logLen;

static uint32_t LogCircularBuffer_GetFreeSpace(void);

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

    if(getIndex == putIndex)
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

    uint32_t freeSpace = LogCircularBuffer_GetFreeSpace();

    // if(freeSpace == 0)
    //     return BUFFER_FULL;

    uint32_t dataLen = strlen(data);
    uint32_t writeLen = dataLen;
    LogBufferStatus_t status = BUFFER_SUCCES;

    if(dataLen + 1 > freeSpace)          // data + terminator doesn't fit
    {
        writeLen = freeSpace - 1;        // leave exactly 1 slot for terminator
        status = LOG_TRUNCATED;
    }

    for(uint32_t i = 0; i < writeLen; i++)
    {
        bufferPtr[putIndex] = data[i];
        putIndex = (putIndex + 1) % bufferSize;
    }

    bufferPtr[putIndex] = '\0';
    putIndex = (putIndex + 1) % bufferSize;

    return status;

    // while(1)
    // {
    //     uint32_t nextPutIndex = (putIndex + 1) % bufferSize;

    //     if(nextPutIndex == getIndex)
    //     {
    //         bufferPtr[putIndex] = '\0';
    //         return LOG_TRUNCATED;
    //     }

    //     char c = *data;
    //     bufferPtr[putIndex] = c;
    //     putIndex = nextPutIndex;

    //     if(c == '\0')
    //         return BUFFER_SUCCES;

    //     data++;
    // }
}

static uint32_t LogCircularBuffer_GetFreeSpace(void)
{
    uint32_t used = (putIndex + bufferSize - getIndex) % bufferSize;
    return bufferSize - used;//(bufferSize - 1) - used;   // 1 slot always reserved as sentinel gap
}

void LogCircularBuffer_AdvanceGetIndex()
{
    memset(bufferPtr + getIndex,' ',logLen);
    getIndex = (getIndex + logLen) % bufferSize;
    logLen = 0;
}

void LogCircularBuffer_Destroy()
{
    bufferPtr = NULL;
}