/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : stm32f4xx_hal_msp.c
  * Description        : This file provides code for the MSP Initialization 
  *                      and de-Initialization codes.
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
#include "main.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* External functions --------------------------------------------------------*/

/**
  * Initializes the Global MSP.
  */
void
HAL_MspInit(void)
{
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();

  /* System interrupt init*/
  /* PendSV_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(PendSV_IRQn, 15, 0);
}

void
HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(huart->Instance==UART5)
  {
    __HAL_RCC_UART5_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    /**UART5 GPIO Configuration
        PC12     ------> UART5_TX (GPIO C
        PD2     ------> UART5_RX
    */

    GPIO_InitStruct.Pin = GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_UART5;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_UART5;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
  }
}

void
HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{
  if(huart->Instance==UART5)
  {
    /* Peripheral clock disable */
    __HAL_RCC_UART5_CLK_DISABLE();

    /**UART5 GPIO Configuration
        PC12     ------> UART5_TX
        PD2     ------> UART5_RX
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_12);

    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_2);
  }
}

void
HAL_CAN_MspInit(CAN_HandleTypeDef* hcan) {
    GPIO_InitTypeDef GPIO_InitStructTX = {0};
    GPIO_InitTypeDef GPIO_InitStructRX = {0};

    if(hcan->Instance==CAN1)
    {
        // CAN1 CLK
        __HAL_RCC_CAN1_CLK_ENABLE();
        // CAN1 GPIO
        __HAL_RCC_GPIOA_CLK_ENABLE();

        /**CAN1 GPIO Configuration
            PA11     ------> CAN1_RX
            PA12     ------> CAN1_TX
        */

        // CAN1 TX GPIO Pin Configuration
        GPIO_InitStructTX.Pin = GPIO_PIN_11|GPIO_PIN_12;
        GPIO_InitStructTX.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStructTX.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStructTX.Pull = GPIO_PULLUP;
        GPIO_InitStructTX.Alternate = GPIO_AF9_CAN1;

        HAL_GPIO_Init(GPIOA, &GPIO_InitStructTX);
    }
    if(hcan->Instance==CAN2)
    {
        // CAN2 CLK + GPIO
        __HAL_RCC_CAN2_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();

        /**CAN2 GPIO Configuration
            PB12     ------> CAN2_RX
            PB13     ------> CAN2_TX
        */

        //CAN2 RX GPIO Pin Configuration
        GPIO_InitStructRX.Pin = GPIO_PIN_12|GPIO_PIN_13;
        GPIO_InitStructRX.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStructRX.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStructRX.Pull = GPIO_PULLUP;
        GPIO_InitStructRX.Alternate = GPIO_AF9_CAN2;

        HAL_GPIO_Init(GPIOB, &GPIO_InitStructRX);
    }


}

void
HAL_CAL_MspDeInit(CAN_HandleTypeDef* hcan) {
    if(hcan->Instance==CAN1)
    {
            __HAL_RCC_CAN1_CLK_DISABLE();

            __HAL_RCC_CAN1_FORCE_RESET();
            __HAL_RCC_CAN1_RELEASE_RESET();
            HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11|GPIO_PIN_12);
    }
    if(hcan->Instance==CAN2)
    {
            __HAL_RCC_CAN2_CLK_DISABLE();

            __HAL_RCC_CAN2_FORCE_RESET();
            __HAL_RCC_CAN2_RELEASE_RESET();
            HAL_GPIO_DeInit(GPIOB, GPIO_PIN_12|GPIO_PIN_13);
    }
}


