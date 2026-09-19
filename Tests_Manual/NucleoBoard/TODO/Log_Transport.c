#include "Log_Transport.h"
#include "Logger.h"
#include "stm32h5xx_ll_usart.h"
#include "stm32h5xx_ll_dma.h"

#define UART_USED        USART2
#define DMA_USED         GPDMA1
#define DMA_CHANNEL_USED LL_DMA_CHANNEL_0

void UART_PrintPolling(const char *pData,uint8_t length)
{
    for(uint8_t index = 0; index < length; index++)
	  {
		while(!LL_USART_IsActiveFlag_TXE(UART_USED))
			;

		LL_USART_TransmitData8(UART_USED, pData[index]);	  
	}
      
    LOG_WakeUp();
}

void Send_USART_DMA_LL(const char *pData, uint8_t Size)
{
    // 1. Wyłącz kanał DMA przed konfiguracją
    LL_DMA_DisableChannel(DMA_USED, DMA_CHANNEL_USED);

    // 2. Ustaw adres źródłowy (pamięć RAM)
    LL_DMA_SetSrcAddress(DMA_USED, DMA_CHANNEL_USED, (uint32_t)pData);

    // 3. Ustaw adres docelowy (rejestr nadawczy USART TDR)
    LL_DMA_SetDestAddress(DMA_USED, DMA_CHANNEL_USED, LL_USART_DMA_GetRegAddr(UART_USED, LL_USART_DMA_REG_DATA_TRANSMIT));

    // 4. Ustaw liczbę bajtów do przesłania
    LL_DMA_SetBlkDataLength(DMA_USED, DMA_CHANNEL_USED, Size);

    // 5. Wyczyść flagę zakończenia transferu (Transfer Complete) na kanale 0
    LL_DMA_ClearFlag_TC(DMA_USED, DMA_CHANNEL_USED);

    // 6. Włącz kanał GPDMA
    LL_DMA_EnableChannel(DMA_USED, DMA_CHANNEL_USED);

    // 7. Włącz żądanie transmisji DMA w peryferium USART
    LL_USART_EnableDMAReq_TX(UART_USED);
}

void GPDMA1_Channel0_IRQHandler(void)
{
  if (LL_DMA_IsActiveFlag_TC(GPDMA1, LL_DMA_CHANNEL_0))
    {
        LL_DMA_ClearFlag_TC(GPDMA1, LL_DMA_CHANNEL_0);
        LL_USART_DisableDMAReq_TX(USART1);
        LOG_WakeUp();
    }
}