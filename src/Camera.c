/*
 * Camera.c
 *
 *  Created on: Mar. 12, 2019
 *      Author: joycelee
 */
// HAL = hardware abstraction layer
//
#include "Camera.h"

UART_HandleTypeDef uart5; // see Peripheral docs Section 7.3.5
uint32_t timeout = 10;
uint8_t buffer[13] = "Hello World!";
uint16_t size = 13;

void transmitCameraData()
{
	HAL_UART_Transmit(&uart5, buffer, size, timeout);
	HAL_UART_Receive();
}
