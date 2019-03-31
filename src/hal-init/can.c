
#include "can.h"

CAN_HandleTypeDef hcan1;
CAN_HandleTypeDef hcan2;

void
CAN1_Init(void) {
    // CAN1 Master
    hcan1.Instance = CAN1;
    hcan1.Init.Prescaler = 16;
    hcan1.Init.Mode = CAN_MODE_LOOPBACK;
    hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
    hcan1.Init.TimeSeg1 = CAN_BS1_1TQ;
    hcan1.Init.TimeSeg2 = CAN_BS2_1TQ;
    hcan1.Init.TimeTriggeredMode = DISABLE;
    hcan1.Init.AutoBusOff = DISABLE;
    hcan1.Init.AutoWakeUp = DISABLE;
    hcan1.Init.AutoRetransmission = DISABLE;
    hcan1.Init.ReceiveFifoLocked = DISABLE;
    hcan1.Init.TransmitFifoPriority = DISABLE;
    if(HAL_CAN_Init(&hcan1 != HAL_OK)) {
            trace_printf("CAN1 Init not successful");
    }
}

void
CAN2_Init(void) {
    hcan2.Instance = CAN2;
    hcan2.Init.Prescaler = 16;
    hcan2.Init.Mode = CAN_MODE_LOOPBACK;
    hcan2.Init.SyncJumpWidth = CAN_SJW_1TQ;
    hcan2.Init.TimeSeg1 = CAN_BS1_1TQ;
    hcan2.Init.TimeSeg2 = CAN_BS2_1TQ;
    hcan2.Init.TimeTriggeredMode = DISABLE;
    hcan2.Init.AutoBusOff = DISABLE;
    hcan2.Init.AutoWakeUp = DISABLE;
    hcan2.Init.AutoRetransmission = DISABLE;
    hcan2.Init.ReceiveFifoLocked = DISABLE;
    hcan2.Init.TransmitFifoPriority = DISABLE;
    if(HAL_CAN_Init(&hcan2 != HAL_OK)) {
            trace_printf("CAN2 Init not successful");
    }
}
