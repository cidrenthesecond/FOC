#ifndef MAIN_SM_H
#define MAIN_SM_H

typedef enum {
    INIT_STATE,
    IDLE_STATE,
    RUN_STATE,
    STOP_STATE,
    ERROR_STATE
} MainSM_Enum_State_t;

void Enter_MainStateMachine();
void ToggleState();
void Main_RequestErrorReset();

void Main_RequestRUN();
void Main_RequestIDLE();
void Main_RequestERROR();


#endif