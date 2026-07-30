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

#define LEN_STUB 5

int LOG_Init(void (*ExternPrintLog)(const char *pData,uint8_t length),
             FIFO fifo);

void *LOG_GetPrintingFunction();

int LOG(const char* text);
int LOG_WakeUp();
void LOG_Destroy();
void * LOG_GetPrintingFunction();

#endif
