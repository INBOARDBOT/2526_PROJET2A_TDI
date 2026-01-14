#ifndef HANDLE_PACKET_QUEUE_H
#define HANDLE_PACKET_QUEUE_H

// Dependencies
#include "stackProtocolDefs.h"
#include "cmsis_os.h"
// Note: If using FreeRTOS directly, this is usually #include "FreeRTOS.h" and #include "queue.h"
// Since you included both, we'll keep cmsis_os.h as the main API source for task/queue types.


// =========================================================
// 1. Encapsulation: Use 'extern' for the queue handle declaration
// =========================================================

// extern declaration: Tells the compiler this variable exists and is defined
// in exactly ONE .c file (usually handle_packet_queue.c).
// Other modules should NOT use this variable directly.
extern osMessageQueueId_t packetQueue_id; // Using CMSIS type and a clearer name (ID)


// =========================================================
// 2. Data Structure (Consistent and good)
// =========================================================

// Note: For FreeRTOS/CMSIS, it's best practice to use the standardized integer types.
typedef struct {
    uint8_t address;
    uint8_t commandCode;   // The command that triggered this response (e.g., CMD_GET_STATUS)
    uint8_t mainFlags;     // Status flags (from MotionPacket_t.flags)

    // Combined Status Flags for Queue Control:
    // Bit 0: HasMoreEvents (MotionPacket_t.moreEvents)
    // Bit 1: HasMoreErrors (MotionPacket_t.moreErrors)
    uint8_t queueStatus;

    // Event or Error Codes:
    uint8_t eventCode;     // Non-zero if a new event was dequeued
    uint8_t moreEvent;

    uint8_t errorCode;     // Non-zero if a new error was dequeued
    uint8_t moreError;

    // Data (Position or Generic 16-bit value):
    uint16_t dataPayload;  // Contains position (LSB first) or other 16-bit data

} PacketData_t;


// =========================================================
// 3. API Functions (Good High-Level Interface)
// =========================================================

/**
 * @brief Initializes the global packet queue.
 * @return osStatus_t indicating success or failure.
 */
osStatus_t initializePacketQueue(void);


/**
 * @brief Deletes/collapses the packet queue, freeing resources.
 */
void collapsePacketQueue(void);


/**
 * @brief Tries to pull a packet from the queue. Blocks if empty.
 * @param packetData A pointer to the structure where the received data will be stored.
 * @return osStatus_t indicating success, timeout, or failure.
 */
osStatus_t pullPacketQueue(PacketData_t* packetData);


/**
 * @brief Pushes a new packet onto the queue.
 * @param packetData The packet structure to send.
 * @return osStatus_t indicating success or failure (e.g., queue full).
 */
osStatus_t pushPacketQueue(const PacketData_t* packetData); // FIX: Changed type to pointer


/**
 * @brief Attempts to fill the queue from a source array until the array is exhausted or the queue is full.
 * @param packetData Pointer to the source array of packets.
 * @param size The number of elements in the source array.
 * @return uint32_t The total number of packets successfully pushed to the queue.
 */
uint32_t fillPacketQueue(const PacketData_t* packetData, int size);

/**
 * @brief Drains all available packets from the queue and stores them in a buffer. Non-blocking.
 * @param packetData Pointer to the destination buffer (array).
 * @return uint32_t The number of packets successfully drained and stored.
 */
uint32_t drainPacketQueue(PacketData_t* packetData);


#endif // HANDLE_PACKET_QUEUE_H
