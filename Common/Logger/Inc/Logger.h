#include "FIFO.h"
#include <stdint.h>

#ifndef LOGGER_H
#define LOGGER_H

enum{
    LOGGER_NOINIT = 0,
    LOGGER_SUCCESS,
    LOGGER_FAIL,
    LOG_SCHEDULED,
};

int LOG_Init(void (*ExternPrintLog)(const char *pData,uint8_t length),
                    uint8_t FIFO_Size);

void *LOG_GetPrintingFunction();

int LOG(const char* text);
int LOG_WakeUp();
void LOG_Destroy();
void * LOG_GetPrintingFunction();

#endif
