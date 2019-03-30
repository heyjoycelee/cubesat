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
#include "diag/Trace.h"
#include "uart5.h"

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
uint32_t timeout = 100;
uint8_t buffer[8] = "testtest";
uint16_t size = 8;

osThreadId defaultTaskHandle;

/* Private function prototypes -----------------------------------------------*/

void StartDefaultTask(void const * argument);
void CommandTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* Init */

  /* add mutexes, ... */

  /* add semaphores, ... */


  /* start timers, add new ones, ... */

  /* add queues, ... */


  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* add threads, ... */

}

void
StartDefaultTask(void const *argument)
{
  for(;;)
  {
    trace_printf("Default task running\n");
    osDelay(1000);
  }
}

void
CommandTask(void const *argument)
{
    for(;;)
    {
      trace_printf("Command task running\n");
      UART5_Transmit(buffer, size, timeout);
      osDelay(100);
    }
}

