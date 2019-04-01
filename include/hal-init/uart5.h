
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UART5_H
#define __UART5_H

#include "stm32f4xx_hal.h"
#include "diag/Trace.h"

UART_HandleTypeDef huart5;
void UART5_Init(void);
HAL_StatusTypeDef UART5_Transmit(uint8_t *buffer, uint16_t size, uint32_t timeout);

#endif /* __UART5_H */
