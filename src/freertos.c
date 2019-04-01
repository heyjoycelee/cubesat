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
uint32_t timeout = HAL_MAX_DELAY;
uint8_t receive_buffer[100]; // you have to hard code/ know the expected size of the incoming message
uint8_t message[100][100];
uint8_t flag = 1;
int incomingPacketCounter = 0;

CAN_TxHeaderTypeDef TxHeader;
CAN_RxHeaderTypeDef RxHeader;
uint32_t *pTxMailbox;
uint32_t *pRxMailbox;
uint8_t txData[8] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08} ;
uint8_t rxData[8];

/* Private function prototypes -----------------------------------------------*/

static TaskFunction_t vInitTask(void const * pvParameters);
//static TaskFunction_t vCAN1RxTask(void const * pvParameters);
//static TaskFunction_t vCAN1TxTask(void const * pvParameters);
static TaskFunction_t vUART5RxTask(void const * pvParameters);

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

static TaskFunction_t
vInitTask(void const * pvParameters) {
//    TxHeader.IDE = 0;
//    TxHeader.StdId = 0;
//    TxHeader.ExtId = 0;
//    TxHeader.DLC = 8; // size is 8 bytes
//    TxHeader.RTR = 0; // data frame, not remote
    for(;;) {
//            BaseType_t xCAN1RX_returned;
//            BaseType_t xCAN1TX_returned;
              BaseType_t UART5RX_returned;
//            TaskHandle_t xHandleRx = NULL;
//            TaskHandle_t xHandleTx = NULL;
              TaskHandle_t xHandleUARTRx = NULL;

//            xCAN1TX_returned = xTaskCreate(vCAN1TxTask, "CAN1 transmit task",configMINIMAL_STACK_SIZE, NULL, 0, &xHandleTx);
//            taskYIELD();
//            if( xCAN1TX_returned == pdPASS )
//            {
//                    vTaskDelete( xHandleTx );
//            }
//
//            xCAN1RX_returned = xTaskCreate(vCAN1RxTask, "CAN1 receive task",configMINIMAL_STACK_SIZE, NULL, 0, &xHandleRx);
//            taskYIELD();
//            if( xCAN1RX_returned == pdPASS )
//            {
//                    vTaskDelete( xHandleRx );
//            }
            UART5RX_returned = xTaskCreate(vUART5RxTask, "UART5 receive task",configMINIMAL_STACK_SIZE, NULL, 0, &xHandleUARTRx);
            taskYIELD();
            if( UART5RX_returned == pdPASS )
            {
                    vTaskDelete( xHandleUARTRx );
            }

    }
}

static TaskFunction_t
vUART5RxTask(void const * pvParameters) {
    if(flag) {
            trace_printf("Waiting for transmission...\n");
    }
    while (flag)
    {
        HAL_StatusTypeDef status = HAL_UART_Receive(&huart5,receive_buffer, sizeof(receive_buffer), timeout);
        trace_printf("%s\n",receive_buffer);
             incomingPacketCounter++;
             if(incomingPacketCounter == 52)
             {
                flag = 0;
                trace_printf("Exiting...");
             }
    }
    taskYIELD();
}


//static TaskFunction_t
//vCAN1TxTask(void const * pvParameters) {
//    trace_printf("Attempting to Transmit\n");
//    if(HAL_CAN_GetTxMailboxesFreeLevel(&hcan2)) {
//            HAL_StatusTypeDef status = HAL_CAN_AddTxMessage(&hcan2, &TxHeader, txData, pTxMailbox);
//            if(status == HAL_OK) {
//                    trace_printf("Transmit Successful\n");
//            }
//    }
//    taskYIELD();
//}
//
//static TaskFunction_t
//vCAN1RxTask(void const * pvParameters) {
//    trace_printf("Attempting to Receive\n");
//    uint32_t fifoFill = HAL_CAN_GetRxFifoFillLevel(&hcan2, CAN_RX_FIFO0);
//    trace_printf("fifo fill is %u\n", fifoFill);
//    if(fifoFill) {
//            HAL_StatusTypeDef status = HAL_CAN_GetRxMessage(&hcan2, CAN_RX_FIFO0, &RxHeader, rxData);
//            if(status == HAL_OK) {
//                    trace_printf("Receive Successful\n");
//            }
//    }
//    taskYIELD();
//}

