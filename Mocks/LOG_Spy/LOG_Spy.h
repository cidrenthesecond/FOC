#ifndef LOG_SPY_H
#define LOG_SPY_H

#include "Logger.h"

#define LOG_SPY_MAX_LEN 1024

void LOG_Spy_Init();
char * LOG_Spy_GetMessage();
int  LOG_Spy_GetCallCount();


#endif