#include "SpyStates.h"

static void RUN_INIT()
{
    RunSpy.initCallCount++;
}
static void RUN()
{
    RunSpy.runCallCount++;
}
static void RUN_DEINIT()
{
    RunSpy.deinitCallCount++;
}

static void STOP_INIT()
{
    StopSpy.initCallCount++;
}
static void STOP()
{
    StopSpy.runCallCount++;
}
static void STOP_DEINIT()
{
    StopSpy.deinitCallCount++;
}

static void IDLE_INIT()
{
    IdleSpy.initCallCount++;
}
static void IDLE()
{
    IdleSpy.runCallCount++;
}
static void IDLE_DEINIT()
{
    IdleSpy.deinitCallCount++;
}

static void EMERGENCY_INIT()
{
    EmergencySpy.initCallCount++;
}
static void EMERGENCY()
{
    EmergencySpy.runCallCount++;
}
static void EMERGENCy_DEINIT()
{
    EmergencySpy.deinitCallCount++;
}

void SpyStates_Init()
{
    RunSpy.state.id = APP_STATE_RUN;
    RunSpy.state.init = RUN_INIT;
    RunSpy.state.run = RUN;
    RunSpy.state.deinit = RUN_DEINIT;

    StopSpy.state.id = APP_STATE_STOP;
    StopSpy.state.init = STOP_INIT;
    StopSpy.state.run = STOP;
    StopSpy.state.deinit = STOP_DEINIT;

    IdleSpy.state.id = APP_STATE_IDLE;
    IdleSpy.state.init = IDLE_INIT;
    IdleSpy.state.run = IDLE;
    IdleSpy.state.deinit = IDLE_DEINIT;

    EmergencySpy.state.id      = APP_STATE_EMERGENCY;
    EmergencySpy.state.init    = EMERGENCY_INIT;
    EmergencySpy.state.run    = EMERGENCY;
    EmergencySpy.state.deinit = EMERGENCy_DEINIT;
}
