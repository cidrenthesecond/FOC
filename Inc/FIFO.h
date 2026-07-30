#ifndef FIFO_H
#define FIFO_H

#include "stdint.h"

enum {
    FIFO_NO_INIT = 0,
    FIFO_EMPTY,
    FIFO_NULL_PTR,
    FIFO_FULL,
    FIFO_SUCCESS
};

enum {
    FIFO_LOG_SIZE = 64,
};

typedef struct FIFO_Struct * FIFO;

FIFO FIFO_Create(uint8_t logSize);
int FIFO_Put(FIFO this, const char * log);
int FIFO_Get(FIFO this, char ** log, uint8_t *length);
int FIFO_IsInitialized(FIFO this);
void FIFO_Destroy(FIFO this);

#endif