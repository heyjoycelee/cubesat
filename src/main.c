
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
  /* Call init function for freertos objects (in freertos.c) */
  freeRTOS_Init();

  trace_printf("freeRTOS Scheduler Starting...\n");
  /* Start scheduler */
  for(;;) {
  }
  /* We should never get here as control is now taken by the scheduler */
}
