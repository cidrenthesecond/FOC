#ifndef  SPY_STATE_H
#define SPY_STATE_H

#include "App_State.h"
#include "stdint.h"

typedef struct {
    AppState_t state;
    size_t initCallCount;
    size_t runCallCount;
    size_t deinitCallCount;
} SpyState_t;

SpyState_t RunSpy;
SpyState_t StopSpy;
SpyState_t IdleSpy;
SpyState_t EmergencySpy;

void SpyStates_Init();

#endif
