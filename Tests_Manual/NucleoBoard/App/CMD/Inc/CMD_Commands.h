#ifndef CMD_MANAGER_H
#define CMD_MANAGER_H

void System_ChangeMainSM(int argc, char*argv[]);
void ADC_Measure(int argc, char *argv[]);
void NTC_Measure(int argc, char *argv[]);
void CMD_GetSwitchingFrequency(int argc, char *argv[]);
void CMD_ESTOP_Command(int argc, char *argv[]);
void System_RequestErrorReset(int argc, char*argv[]);

#endif