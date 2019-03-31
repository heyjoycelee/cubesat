/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "freeRTOS/FreeRTOS.h"
#include "freeRTOS/task.h"
#include "main.h"
#include "freeRTOS/CMSIS_RTOS_ST_V1/cmsis_os.h"

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
uint32_t timeout = 100;
uint8_t buffer[8] = "testtest";
uint16_t size = 8;

/* Private function prototypes -----------------------------------------------*/

static void vInitTask(void const * pvParameters);
static void vCAN1RxTask(void const * pvParameters);
static void vCAN1TxTask(void const * pvParameters);

void freeRTOS_Init(void);

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void freeRTOS_Init(void) {
    xTaskCreate(vInitTask, "initial task",configMINIMAL_STACK_SIZE, NULL, 0, NULL);
    vTaskStartScheduler();
    trace_printf("Not enough space to create the idle task");
}

static void
vInitTask(void const * pvParameters) {
    for(;;) {
            BaseType_t xCAN1RX_returned;
            BaseType_t xCAN1TX_returned;
            TaskHandle_t xHandleRx = NULL;
            TaskHandle_t xHandleTx = NULL;
            xCAN1TX_returned = xTaskCreate(vCAN1TxTask, "CAN1 transmit task",configMINIMAL_STACK_SIZE, NULL, 0, &xHandleTx);
            taskYIELD();
            if( xCAN1TX_returned == pdPASS )
            {
                    vTaskDelete( xHandleTx );
            }
            xCAN1RX_returned = xTaskCreate(vCAN1RxTask, "CAN1 receive task",configMINIMAL_STACK_SIZE, NULL, 0, &xHandleRx);
            taskYIELD();
            if( xCAN1RX_returned == pdPASS )
            {
                    vTaskDelete( xHandleRx );
            }
    }
}

static void
vCAN1RxTask(void const * pvParameters) {
    trace_printf("CAN Received\n");
    taskYIELD();
}

static void
vCAN1TxTask(void const * pvParameters) {
    trace_printf("CAN Transmitted\n");
    taskYIELD();
}

