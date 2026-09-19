#ifndef LOG_TRANSPORT_H
#define LOG_TRANSPORT_H

#include "stdint.h"

void UART_PrintPolling(const char *pData,uint8_t length);
void Send_USART_DMA_LL(const char *pData, uint8_t Size);


#endif