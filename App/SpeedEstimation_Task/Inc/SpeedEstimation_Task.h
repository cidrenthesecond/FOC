#ifndef SPEED_ESTIMATION_TASK
#define SPEED_ESTIMATION_TASK

#include "stdbool.h"
#include "stdint.h"

typedef enum{
    PCV
}SpeedEstimation_Enum_t;

void SpeedEstimation_Init();
void SpeedEstimation_BeforeLaunch();
void SpeedEstimation_SetStrategy(SpeedEstimation_Enum_t strategy);
void SpeedEstimation_SetTaskActive(bool set);





#endif