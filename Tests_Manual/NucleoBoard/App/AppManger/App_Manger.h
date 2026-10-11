#ifndef APP_MANAGER_H
#define APP_MANAGER_H

#include "App_State.h"

void App_Manager_Register_State(AppState_t * state);
void App_Manager_Init(AppStateId_t initialId);
void App_Manager_Request_Transition(AppStateId_t state);
void App_Manager_Run();

#endif