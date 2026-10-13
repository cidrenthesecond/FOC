#include "App_State.h"
#include "App_Manger.h"
#include "stdint.h"
#include "stdbool.h"
#include "string.h"

static const AppState_t *CurrentState = NULL;
static const AppState_t *PendingState = NULL;

static const AppState_t *StateRegistry[APP_STATE_COUNT];

static const bool AllowedTransitions[APP_STATE_COUNT][APP_STATE_COUNT] =
{
                 //IDLE   RUN    STOP   Emergency
    /*IDLE*/      {false, true,  false, true},
    /*RUN*/       {false, false, true,  true},
    /*STOP*/      {true,  false, false, true},
    /*EMERGENCY*/ {true,  false, false, false}
};

bool App_Manager_Register_State(AppState_t * state)
{
    if(state -> id >= APP_STATE_COUNT ||
       state -> run == NULL           ||
       state -> init == NULL           ||
       state -> deinit == NULL)
       return false;

    StateRegistry[state->id] = state;
    return true;
}

void App_Manager_Init(AppStateId_t initialId)
{
    if(CurrentState == NULL && StateRegistry[initialId] == NULL)
        return;

    CurrentState->init();
}

void App_Manager_Request_Transition(AppStateId_t state)
{
    if(state >= APP_STATE_COUNT)
        return;

    if(!AllowedTransitions[CurrentState ->id][state])
        return;

    if(StateRegistry[state] == NULL)
        return;

    if(PendingState->id != APP_STATE_EMERGENCY)
        return;

    PendingState = StateRegistry[state];
}

void App_Manager_Run()
{
    if(PendingState != NULL)
    {
        CurrentState->deinit();
        PendingState->init();
        CurrentState = PendingState;
        PendingState = NULL;
    }

    CurrentState->run();
}

void App_Manager_ClearStates()
{
    for(uint8_t index = 0; index < APP_STATE_COUNT; index++)
    {
        StateRegistry[index]->init = NULL;
        StateRegistry
    }
}


