#include "SpeedEstimation_Task.h"
#include "qkernel.h"
#include "qtasks.h"
#include <stdint.h>

static void SpeedEstimation_Task_Callback(qEvent_t e);
static bool AreRunConditionsMet();

typedef struct {
    uint8_t isInitialised : 1;
    uint8_t isActive      : 1;
    uint8_t isDataReady   : 1;
    uint8_t isStrategySet : 1;
    uint8_t isUsed        : 1;
    uint8_t reserved      : 3;
}Task_Status_t;

typedef struct {
    void    (*BeforeLaunch)(void);
    int16_t (*CalculateVelocity)(uint16_t num_pulses);
}SpeedEstimation_Strategy_t;

static SpeedEstimation_Strategy_t StrategyUsed;
static Task_Status_t              Task_Status;
static qTask_t                    SpeedEstimation_Task;

void SpeedEstimation_Init()
{
    qOS_Add_EventTask(&SpeedEstimation_Task, SpeedEstimation_Task_Callback, qHigh_Priority, NULL);
    Task_Status.isInitialised = 1;
    Task_Status.isActive      = 0;
    Task_Status.isDataReady   = 0;

    if(StrategyUsed.BeforeLaunch == NULL || StrategyUsed.CalculateVelocity == NULL) Task_Status.isStrategySet = 0;
    else Task_Status.isStrategySet = 1; 
}

void SpeedEstimation_BeforeLaunch()
{
    if(!AreRunConditionsMet())
        return;

    StrategyUsed.BeforeLaunch();
}

void SpeedEstimation_Task_Callback(qEvent_t e)
{
    (void)e;

    if(!AreRunConditionsMet())
        return;

}

void SpeedEstimation_SetStrategy(SpeedEstimation_Enum_t strategy)
{
    (void)strategy;
}

void SpeedEstimation_SetTaskActive(bool set)
{
    if(set) Task_Status.isActive = 1;
    else    Task_Status.isActive = 0;
}

static bool AreRunConditionsMet()
{
    return Task_Status.isActive && 
           Task_Status.isInitialised && 
           Task_Status.isStrategySet;
}