
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CAN_H
#define __CAN_H

#include "stm32f4xx_hal.h"
#include "diag/Trace.h"

CAN_HandleTypeDef hcan1;
CAN_HandleTypeDef hcan2;

void CAN1_Init(void);
void CAN2_Init(void);

#endif /* __CAN_H */
