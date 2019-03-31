#include "gpio.h"

void
GPIO_Init(void)
{
  /* UART5 GPIO CLK ENABLE */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
}
