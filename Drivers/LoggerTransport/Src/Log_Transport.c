#include "bsp.h"
#include "Log_Transport.h"
#include "Logger.h"
#include "stm32h5xx_ll_usart.h"
#include "stm32h5xx_ll_dma.h"

void LogTransport_Init()
{
    LL_USART_EnableDirectionTx(LOG_UART);
    LL_DMA_EnableIT_TC(LOG_DMA, LOG_DMA_CHANNEL);
    LL_USART_Enable(LOG_UART);
}

void UART_PrintPolling(const char *pData,uint8_t length)
{
    for(uint8_t index = 0; index < length; index++)
	  {
		while(!LL_USART_IsActiveFlag_TXE(LOG_UART))
			;

		LL_USART_TransmitData8(LOG_UART, pData[index]);	  
	}
      
    LOG_WakeUp();
}



void Send_USART_DMA_LL(const char *pData, uint8_t Size)
{
    // 1. Wyłącz kanał DMA przed konfiguracją
    LL_DMA_DisableChannel(LOG_DMA, LOG_DMA_CHANNEL);

    // 2. Ustaw adres źródłowy (pamięć RAM)
    LL_DMA_SetSrcAddress(LOG_DMA, LOG_DMA_CHANNEL, (uint32_t)pData);

    // 3. Ustaw adres docelowy (rejestr nadawczy USART TDR)
    LL_DMA_SetDestAddress(LOG_DMA, LOG_DMA_CHANNEL, LL_USART_DMA_GetRegAddr(LOG_UART, LL_USART_DMA_REG_DATA_TRANSMIT));

    // 4. Ustaw liczbę bajtów do przesłania
    LL_DMA_SetBlkDataLength(LOG_DMA, LOG_DMA_CHANNEL, Size);

    // 5. Wyczyść flagę zakończenia transferu (Transfer Complete) na kanale 0
    LL_DMA_ClearFlag_TC(LOG_DMA, LOG_DMA_CHANNEL);

    // 6. Włącz kanał GPDMA
    LL_DMA_EnableChannel(LOG_DMA, LOG_DMA_CHANNEL);

    // 7. Włącz żądanie transmisji DMA w peryferium USART
    LL_USART_EnableDMAReq_TX(LOG_UART);
}

void GPDMA1_Channel0_IRQHandler(void)
{
  if (LL_DMA_IsActiveFlag_TC(LOG_DMA, LOG_DMA_CHANNEL))
    {
        LL_DMA_ClearFlag_TC(LOG_DMA, LOG_DMA_CHANNEL);
        LL_USART_DisableDMAReq_TX(LOG_UART);
        LOG_WakeUp();
    }
}