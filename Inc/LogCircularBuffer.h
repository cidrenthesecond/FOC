#ifndef LOG_CIRCULAR_BUFFER_H
#define LOG_CIRCULAR_BUFFER_H

#include <stdint.h>

//typedef struct LogCircularBufferStruct * LogCircularBuffer;

typedef enum{
    BUFFER_NO_INIT = 0,
    BUFFER_SUCCES,
    BUFFER_EMPTY,
    BUFFER_FULL,
    LOG_TRUNCATED,
    LOG_FULL,
    LOG_SPLITTED
} LogBufferStatus_t;

void LogCircularBuffer_Init(char * buffer,uint32_t size);

LogBufferStatus_t LogCircularbuffer_Get(char **getText, uint32_t *getSize);
LogBufferStatus_t LogCircularBuffer_Put(char *data);
void LogCircularBuffer_AdvanceGetIndex();

void LogCircularBuffer_Destroy();




#endif