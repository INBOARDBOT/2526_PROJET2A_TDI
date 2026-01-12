#include "motionCommunication.h"

static uint8_t gCycleId = 0;
static uint8_t currentSlaveTarget = 0;
static uint32_t lastRequestTick = 0;

MotionPacket_t txPacket;
uint8_t rxBuffer[sizeof(MotionPacket_t) * 2];

#define SLAVE_TIMEOUT_MS 5

// --- 1. TRANSMIT FUNCTION ---
HAL_StatusTypeDef transmitPacket(UART_HandleTypeDef *huart, MotionPacket_t *pPacket)
{
    if (huart->gState != HAL_UART_STATE_READY) return HAL_BUSY;

    pPacket->cycleId = gCycleId++;
    pPacket->checksum = calculate_checksum(pPacket);

    // Record time to detect if slave never responds
    lastRequestTick = HAL_GetTick();

    return HAL_UART_Transmit_DMA(huart, (uint8_t*)pPacket, sizeof(MotionPacket_t));
}

// --- 2. THE WATCHDOG (Call this in your main while loop) ---
void monitorCommunicationTimeout(UART_HandleTypeDef *huart)
{
    if ((HAL_GetTick() - lastRequestTick) > SLAVE_TIMEOUT_MS)
    {
        // Slave didn't respond in time. Force move to next slave.
        triggerNextSlave(huart);
    }
}

// --- 3. LOGIC TO SWITCH SLAVES ---
void triggerNextSlave(UART_HandleTypeDef *huart)
{
    // Increment: 0 -> 1 -> 2 -> 3 -> 0
    currentSlaveTarget = (currentSlaveTarget + 1) % 4;

    txPacket.address = currentSlaveTarget;
    txPacket.command = CMD_GET_STATUS; // Or your specific logic

    // Restart RX DMA to be ready before we send the request
    HAL_UART_AbortReceive(huart);
    HAL_UARTEx_ReceiveToIdle_DMA(huart, rxBuffer, sizeof(rxBuffer));

    transmitPacket(huart, &txPacket);
}

// --- 4. THE RECEPTION (IDLE EVENT) ---
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if(huart->Instance == UART7) {
        // Map buffer to struct
        MotionPacket_t *received = (MotionPacket_t*)rxBuffer;

        // Verify: Correct Slave? Correct Start/Stop? Correct Checksum?
        if (Size >= sizeof(MotionPacket_t) &&
            received->address == currentSlaveTarget &&
            received->startByte == 0xAA && // Example start byte
            received->stopByte == 0x55)    // Example stop byte
        {
            if(calculate_checksum(received) == received->checksum) {
                process_slave_data(received);
            }
        }

        // Move to next slave immediately because we got "something" (or noise)
        // that triggered the IDLE line.
        triggerNextSlave(huart);
    }
}
