#ifndef CMD_TRANSPORT_H
#define CMD_TRANSPORT_H

void  InitCmdTransport();
int   IsDataReady(void);
char* GetData(void);
void  SetUpMonitoring(void);

#endif