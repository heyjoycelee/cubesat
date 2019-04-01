
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void freeRTOS_Init(void);

/* Private user code ---------------------------------------------------------*/

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
                                                                                                                                                                                                                                                                                                                                                                                                                                                                  HAL_Init();
  /* Configure the system clock */
  SystemClock_Init();
  /* SysInit */

  /* Peripheral initialization*/
  GPIO_Init();
  UART5_Init();
  CAN1_Init();
  CAN2_Init();
  HAL_CAN_Start(&hcan2);
  /* Call init function for freertos objects (in freertos.c) */
  trace_printf("freeRTOS Scheduler Starting...\n");
  freeRTOS_Init();
  /* We should never get here as control is now taken by the scheduler */
  trace_printf("Reached end of main...\n");
}
