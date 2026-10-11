#include "App_State.h"
#include "SysTickDispatcher.h"


static void RUN_Init()
{

}

static void RUN()
{

}

static void RUN_Deinit()
{

}

static const AppState_t runState = {
    .id     = APP_STATE_RUN,
    .init   = RUN_Init,
    .run    = RUN,
    .deinit = RUN_Deinit,
};

const AppState_t * Get_Run_State()
{
    return &runState;
}