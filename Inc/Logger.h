#include <stdint.h>

#ifndef LOGGER_H
#define LOGGER_H

enum{
    LOGGER_NOINIT = 0,
    LOGGER_SUCCES,
    LOGGER_NULL_PTR,
    LOGGER_BUSY,
    LOG_SCHEDULED,
    LOG_TRUNCATED,
};

void LOG_Init(void (*ExternPrintLog)(const char *pData,uint8_t length),
              char*logBuffer,
              uint32_t size);
void LOG_Destroy();
void *LOG_GetPrintingFunction();

int LOG(const char* text);
int LOG_Schedule(const char* text);
               
void LOG_WakeUp();

#endif
