
#include "uart5.h"

UART_HandleTypeDef huart5;

void
UART5_Init(void)
{
    huart5.Instance = UART5;
    huart5.Init.BaudRate = 115200;
    huart5.Init.WordLength = UART_WORDLENGTH_8B;
    huart5.Init.StopBits = UART_STOPBITS_1;
    huart5.Init.Parity = UART_PARITY_NONE;
    huart5.Init.Mode = UART_MODE_TX_RX;
    huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart5.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart5) != HAL_OK)
    {
            trace_printf("UART 5 Error");
    }
}

HAL_StatusTypeDef
UART5_Transmit(uint8_t *buffer, uint16_t size, uint32_t timeout)
{
    return HAL_UART_Transmit(&huart5, buffer, size, timeout);
}
