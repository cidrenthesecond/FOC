#ifndef CMD_TRANSPORT_H
#define CMD_TRANSPORT_H

void  InitCmdTransport();
void  Set_OnDataReady_Callback(void (*callback)(void));
char* GetData(void);
void  SetUpMonitoring(void);

#endif