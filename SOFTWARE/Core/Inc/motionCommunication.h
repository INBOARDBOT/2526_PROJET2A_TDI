#ifndef MOTION_COMMUNICATION_H
#define MOTION_COMMUNICATION_H


#include "stm32f7xx.h"
#include "stackprotocolDefs.h"
#include "motionDriverStatue.h"
#include "motionPackethandler.h"
#include "motionState.h"

#define MAX_RETRIES          3  // Number of attempts per slave before declaring a cycle failure



typedef enum {
    COMM_STATE_IDLE,
    COMM_STATE_WAITING_FOR_RX,
    COMM_STATE_PROCESSING,
    COMM_STATE_FAILURE
} CommState_t;

extern motionState_t currentState;
extern CommState_t commState;

void motionComm_Init(UART_HandleTypeDef *huart_instance);

bool isSystemSynchronized(uint8_t requiredFlag);

bool isConnectionStable(void);

void startCommunicationCycle(void);
void processCommunicationManager(void);
void comm_RequestAction(motionState_t mode, int param);

void sendRequestToSlave(uint8_t address);
void handleCommError(void);


void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size);



#endif
