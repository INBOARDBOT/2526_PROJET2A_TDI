#include "motionCommunication.h"

static UART_HandleTypeDef *p_huart = NULL;

// Shared value with extern c files (motionState.c)
CommState_t commState = COMM_STATE_IDLE;


// Global control variables

static uint8_t currentSlaveIdx = 0;
static uint8_t retryCount = 0;
static uint32_t lastRequestTick = 0;

MotionPacket_t txPacket;
MotionPacket_t rxPacket;
uint8_t rxBuffer[sizeof(MotionPacket_t)];

// Helper to map index to address
static const uint8_t slaveAddresses[] = {ADDRESSPISTONTL, ADDRESSPISTONTR, ADDRESSPISTONBL, ADDRESSPISTONBR};



static int cycleCount = 0;
static int targetCycles = 0;

void motionComm_Init(UART_HandleTypeDef *huart_instance) {
    p_huart = huart_instance;
}


void startCommunicationCycle(void) {
    currentSlaveIdx = 0;
    retryCount = 0;
    commState = COMM_STATE_WAITING_FOR_RX;
    sendRequestToSlave(slaveAddresses[currentSlaveIdx]);
}

void sendRequestToSlave(uint8_t address) {
    memset(&txPacket, 0, sizeof(MotionPacket_t));
    txPacket.address = address;
    txPacket.statusFlags = FLAGM_AWN_REQUEST;

    // Flush RX DMA memory bfeore transmission
    HAL_UART_AbortReceive(p_huart);
    HAL_UARTEx_ReceiveToIdle_DMA(p_huart, rxBuffer, sizeof(MotionPacket_t));

    // transmitPacket(huart, &txPacket);
    lastRequestTick = HAL_GetTick();
}

void processCommunicationManager(void) {
    switch (commState) {
        case COMM_STATE_WAITING_FOR_RX:
            // Check for Hardware Timeout
            if ((HAL_GetTick() - lastRequestTick) > TIMEOUT_MS) {
                handleCommError();
            }
            break;

        case COMM_STATE_PROCESSING:
            // 1. Process data from previous slave (already stored in rxPacket)
        	handlePacket(&rxPacket);

            // 2. Move to next slave
            currentSlaveIdx++;
            if (currentSlaveIdx >= 4) {
                currentSlaveIdx = 0; // Cycle complete
                commState = COMM_STATE_IDLE;
            } else {
                retryCount = 0;
                commState = COMM_STATE_WAITING_FOR_RX;
                sendRequestToSlave(slaveAddresses[currentSlaveIdx]);
            }
            break;

        case COMM_STATE_FAILURE:
            // System-level error handling (e.g., enter MASTER_STATE_ERROR) [TO DO]
            break;

        default:
            break;
    }
}


void handleCommError(void) {
    if (retryCount < MAX_RETRIES) {
        retryCount++;
        sendRequestToSlave(slaveAddresses[currentSlaveIdx]);
    } else {
        // LINK: If retries fail, trigger the Global Error FSM
        commState = COMM_STATE_FAILURE;
        triggerGlobalError(ERR_COMM_LOST, STATE_IDLE, NULL);
    }
}


void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
    if(huart == p_huart && p_huart != NULL) {
        MotionPacket_t *received = (MotionPacket_t*)rxBuffer;
        uint8_t calc = calculateChecksum(received);

        if (Size >= sizeof(MotionPacket_t) && calc == received->checksum) {
            // Update Health Table for this specific slave
            uint8_t idx = received->address; // Assuming address 0-3
            Health_Update(idx, received);
            // systemHealth[idx].errorCount = 0;

            memcpy(&rxPacket, received, sizeof(MotionPacket_t));
            commState = COMM_STATE_PROCESSING;
        } else {
            handleCommError();
        }
    }
}

void comm_RequestAction(motionState_t mode, int param) {
    currentSlaveIdx = 0;
    retryCount = 0;
    cycleCount = 0;
    targetCycles = param;

    // Map Motion States to Comm States
    if (mode == STATE_PINGING) {
        commState = COMM_STATE_WAITING_FOR_RX;
        txPacket = pingPacketStructure;
        sendRequestToSlave(slaveAddresses[currentSlaveIdx]);
    } else {
        commState = COMM_STATE_PROCESSING;
    }
}
