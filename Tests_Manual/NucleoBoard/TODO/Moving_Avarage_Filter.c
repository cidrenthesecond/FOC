#include "Moving_Avarage_Filter.h"
#include <cstdint>
#include <stdlib.h>
#include <string.h>

#define ERROR_CODE UINT16_MAX

typedef struct MovingAvarage_struct{
    uint16_t *dataPtr;
    uint32_t sum;
    uint8_t kernel;
    uint8_t putIndex;
    uint8_t gotFirstSample;
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
    result -> gotFirstSample = 0;
    return result;
}

static uint16_t OnFirstSample(MovingAvarage f, uint16_t sample)
{
    for(uint8_t i = 0; i< f->kernel; i++)
        f->dataPtr[i] = sample;

    f->gotFirstSample = 1;
    f->sum = sample*(f->kernel);
    return sample;
}

uint16_t      MovingAvarage_Filter(MovingAvarage filter, uint16_t inputSample)
{
    if( filter == NULL || filter -> dataPtr == NULL ) 
        return ERROR_CODE;

    if(!(filter->gotFirstSample))
        return OnFirstSample(filter,inputSample);

    filter -> sum -= filter -> dataPtr[filter -> putIndex];
    filter -> dataPtr[filter->putIndex] = inputSample;
    filter -> sum += inputSample;

    filter -> putIndex = (filter -> putIndex + 1) % filter -> kernel;

    return (uint16_t)(filter->sum / filter->kernel);
}

void MovingAvarage_Reset(MovingAvarage filter)
{
    memset((filter->dataPtr), 0, sizeof(uint16_t)*(filter -> kernel));
    filter -> sum = 0;
    filter -> putIndex = 0;
    filter -> gotFirstSample = 0;
}

void          MovingAvarage_Destroy(MovingAvarage filter)
{
    free(filter->dataPtr);
    free(filter);
}