#include "Moving_Avarage_Filter.h"
#include <stdlib.h>
#include <string.h>

#define ERROR_CODE UINT16_MAX

typedef struct MovingAvarage_struct{
    uint16_t *dataPtr;
    uint32_t sum;
    uint8_t kernel;
    uint8_t putIndex;
    uint8_t getIndex;
}MovingAvarage_struct;

MovingAvarage MovingAvarage_Init(const uint8_t kernel)
{
    if(kernel == 0)
        return NULL;

    MovingAvarage result = malloc(sizeof(MovingAvarage_struct));
    
    result -> dataPtr = calloc(kernel,sizeof(uint16_t));
    result -> sum = 0;
    result -> kernel = kernel;
    result -> putIndex = 0;
    result -> getIndex = 0;
    return result;
}

uint16_t      MovingAvarage_Filter(MovingAvarage filter, uint16_t inputSample)
{
    if( filter == NULL || filter -> dataPtr == NULL ) 
        return ERROR_CODE;

    filter -> sum -= filter -> dataPtr[filter -> putIndex];
    filter -> dataPtr[filter->putIndex] = inputSample;
    filter -> sum += inputSample;

    filter -> putIndex = (filter -> putIndex + 1) % filter -> kernel;

    return (uint16_t)(filter->sum / filter->kernel);
}

void          MovingAvarage_Destroy(MovingAvarage filter)
{
    free(filter->dataPtr);
    free(filter);
}