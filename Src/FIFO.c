#include "FIFO.h"
#include "string.h"
#include "stdlib.h"

#define END_LINE_PADDING    1
#define BUFFER_PADDING_SIZE 2 //"Sentinel" for full state and additional overflowLog

typedef struct {
    char data[FIFO_MAX_LOG_LENGTH];
    uint8_t len;
} LogEntry;

static const char    OverflowMessage[19]   = "***LOG OVERFLOW***";

static int isQueueFull(FIFO this);
static void StoreMessage(FIFO this,const char * log);

typedef struct FIFO_Struct{
    LogEntry *queue;
    uint8_t actualBufferSize;
    uint8_t putIndex;
    uint8_t getIndex;
    uint8_t isInitialized;
    uint8_t isOverflowPresent;
} FIFO_Struct;

FIFO FIFO_Create(uint8_t logSize)
{
    if(logSize == 0)
        return NULL;

    FIFO result = malloc(sizeof(FIFO_Struct));
    result -> queue = malloc(sizeof(LogEntry) * (logSize + BUFFER_PADDING_SIZE));
    result -> actualBufferSize = logSize + BUFFER_PADDING_SIZE;
    result -> putIndex = 0;
    result -> getIndex = 0;
    result -> isInitialized = 1;
    result -> isOverflowPresent = 0;

    for(uint8_t index = 0; index < logSize; index++)
        result->queue[index].len = 0;

    return result;
}

int FIFO_Put(FIFO this,const char * log)
{   
    if(this->isInitialized == 0)
        return FIFO_NO_INIT;

    if(log == NULL)
        return FIFO_NULL_PTR;

    uint8_t nextPutIndex = ((this->putIndex) + 1) % this->actualBufferSize;

    if(isQueueFull(this))
    {
        if(this -> isOverflowPresent)
            return FIFO_FULL;

        StoreMessage(this, OverflowMessage);
        this -> putIndex = nextPutIndex;
        this -> isOverflowPresent = 1;
        return FIFO_FULL;
    }

    StoreMessage(this, log);
    this -> putIndex = nextPutIndex;

    return FIFO_SUCCESS;
}

static inline int isQueueFull(FIFO this)
{
    //uint8_t fullConditionIndex = (this->putIndex + 2) % this->actualBufferSize; // +2: sentinel slot + reserved overflow slot
    //return fullConditionIndex == this->getIndex;
    uint8_t nextPutIndex =
        (this->putIndex + 1) % this->actualBufferSize;

    uint8_t distance =
        (this->getIndex - nextPutIndex + this->actualBufferSize)
        % this->actualBufferSize;

    return distance <= 1;
}

static inline void StoreMessage(FIFO this,const char * log)
{   
    int index = 0;
    uint8_t logLength = 0;

    for(; index < FIFO_MAX_LOG_LENGTH - END_LINE_PADDING; index++)
    {   
        if(log[index] == '\0')
            break;

        this->queue[this->putIndex].data[index] = log[index];
        logLength++;
    }

    this->queue[this->putIndex].data[index] = '\n';// Stop using strlen in tests
    logLength++;

    this->queue[this->putIndex].len = logLength;
}

int FIFO_Get(FIFO this,char ** log, uint8_t *length)
{   
    if(this->isInitialized == 0)
        return FIFO_NO_INIT;

    if(this->getIndex == this->putIndex)
        return FIFO_EMPTY;

    *log = this->queue[this->getIndex].data;
    *length = this->queue[this->getIndex].len;

    //this->queue[this->getIndex].len = 0; //NO TESTS FOR THAT!!!!

    this->getIndex = (this->getIndex + 1) % this->actualBufferSize;
    this->isOverflowPresent = 0;

    return FIFO_SUCCESS;
}

void FIFO_Destroy(FIFO this)
{
    if(this == NULL)
        return;

    free(this->queue);
    free(this);
}