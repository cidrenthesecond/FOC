#include "Cmd_Transport.h"
#include "stm32h5xx_ll_dma.h"
#include "stm32h5xx_ll_usart.h"
#include "string.h"

#define DMA_USED         GPDMA1
#define DMA_CHANNEL_USED LL_DMA_CHANNEL_1
#define USART_USED       USART2
#define BUFFER_SIZE      64

char rx_buffer[BUFFER_SIZE] = {0};
static volatile uint8_t data_ready_flag = 0;

static void Start_USART_RX_DMA(void);

int IsDataReady(void)
{
    return data_ready_flag;
}

char* GetData(void)
{
    return rx_buffer;
}

void SetUpMonitoring(void)
{
    memset(rx_buffer,0,sizeof(rx_buffer));
    data_ready_flag = 0;
    Start_USART_RX_DMA();
}

void USART2_IRQHandler(void)
{
  if (LL_USART_IsActiveFlag_IDLE(USART2) && LL_USART_IsEnabledIT_IDLE(USART2))
    {
        LL_USART_ClearFlag_IDLE(USART2);
        LL_DMA_DisableChannel(GPDMA1, LL_DMA_CHANNEL_1);
        data_ready_flag = 1;
    }
}

static void Start_USART_RX_DMA(void)
{
    // 1. Wyłącz kanał DMA przed konfiguracją
    LL_DMA_DisableChannel(DMA_USED, DMA_CHANNEL_USED);

    // 2. Adres źródłowy (rejestr odbiorczy USART RDR)
    LL_DMA_SetSrcAddress(DMA_USED, DMA_CHANNEL_USED, LL_USART_DMA_GetRegAddr(USART_USED, LL_USART_DMA_REG_DATA_RECEIVE));

    // 3. Adres docelowy (bufor w RAM)
    LL_DMA_SetDestAddress(DMA_USED, DMA_CHANNEL_USED, (uint32_t)rx_buffer);

    // 4. Maksymalny rozmiar oczekiwanego bufora
    LL_DMA_SetBlkDataLength(DMA_USED, DMA_CHANNEL_USED, 64);

    // 5. Włącz kanał DMA i zgłoszenie DMA w USART
    LL_DMA_EnableChannel(DMA_USED, DMA_CHANNEL_USED);
    LL_USART_EnableDMAReq_RX(USART_USED);

    // 6. Wyczyść i włącz przerwanie IDLE Line w USART
    LL_USART_ClearFlag_IDLE(USART_USED);
    LL_USART_EnableIT_IDLE(USART_USED);
}