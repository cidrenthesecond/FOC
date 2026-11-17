#ifndef RUN_LED_TASK_H
#define RUN_LED_TASK_H

#include "stdbool.h"
#include "stdint.h"
#include "qtasks.h"

typedef struct {
    void (*Led_ON)(void);
    void (*Led_OFF)(void);
} LED_Task_Interface_t;

typedef struct {
    uint8_t isInitialised : 1;
    uint8_t isActive      : 1;
    uint8_t LedImage      : 1;
    uint8_t iterations    : 5;
} LED_Task_Status_t;

typedef struct {
    LED_Task_Interface_t interface;
    LED_Task_Status_t    Status;
    qTask_t              Task;
} LED_Task_t;

#define ITERATIONS_INDEFINITE (31) //2^5 - 1 

void LED_Task_Init(LED_Task_t *self, const LED_Task_Interface_t *interface, qTime_t period);
void LED_Task_SetActive(LED_Task_t *self, bool set, uint8_t iterations);

#endif