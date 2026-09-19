#include "System_Config.h"

#include"Logger.h"
#include"Log_Transport.h"

void System_Init()
{
    LOG_Init(Send_USART_DMA_LL, 10);
}