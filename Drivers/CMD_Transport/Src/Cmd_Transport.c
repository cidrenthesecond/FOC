#include "bsp.h"
#include "Cmd_Transport.h"
#include "stm32h5xx_ll_dma.h"
#include "stm32h5xx_ll_usart.h"
#include "string.h"


#define BUFFER_SIZE      64

char rx_buffer[BUFFER_SIZE] = {0};
void (* OnDataReady_Callback)(void);

static void Start_USART_RX_DMA(void);

void  InitCmdTransport()
{
    LL_USART_EnableDirectionRx(CMD_UART);
    LL_USART_Enable(CMD_UART);
    OnDataReady_Callback = NULL;
}

void  Set_OnDataReady_Callback(void (*callback)(void))
{
    OnDataReady_Callback = callback;
}

char* GetData(void)
{
    return rx_buffer;
}

void SetUpMonitoring(void)
{
    memset(rx_buffer,0,sizeof(rx_buffer));
    Start_USART_RX_DMA();
}

void USART2_IRQHandler(void)
{
  if (LL_USART_IsActiveFlag_IDLE(CMD_UART) && LL_USART_IsEnabledIT_IDLE(CMD_UART))
    {
        LL_DMA_DisableChannel(CMD_DMA, CMD_DMA_CHANNEL);

        if(OnDataReady_Callback != NULL)
            OnDataReady_Callback();

        LL_USART_ClearFlag_IDLE(CMD_UART);
    }
}

static void Start_USART_RX_DMA(void)
{
    // 1. Wyłącz kanał DMA przed konfiguracją
    LL_DMA_DisableChannel(CMD_DMA, CMD_DMA_CHANNEL);

    // 2. Adres źródłowy (rejestr odbiorczy USART RDR)
    LL_DMA_SetSrcAddress(CMD_DMA, CMD_DMA_CHANNEL, LL_USART_DMA_GetRegAddr(CMD_UART, LL_USART_DMA_REG_DATA_RECEIVE));

    // 3. Adres docelowy (bufor w RAM)
    LL_DMA_SetDestAddress(CMD_DMA, CMD_DMA_CHANNEL, (uint32_t)rx_buffer);

    // 4. Maksymalny rozmiar oczekiwanego bufora
    LL_DMA_SetBlkDataLength(CMD_DMA, CMD_DMA_CHANNEL, 64);

    // 5. Włącz kanał DMA i zgłoszenie DMA w USART
    LL_DMA_EnableChannel(CMD_DMA, CMD_DMA_CHANNEL);
    LL_USART_EnableDMAReq_RX(CMD_UART);

    // 6. Wyczyść i włącz przerwanie IDLE Line w USART
    LL_USART_ClearFlag_IDLE(CMD_UART);
    LL_USART_EnableIT_IDLE(CMD_UART);
}