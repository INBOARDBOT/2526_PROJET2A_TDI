#include "handlePacketQueue.h"

// Define the queue ID (satisfies the 'extern' in the header)
// Note: PACKET_QUEUE_SIZE must be defined in stackProtocolDefs.h or elsewhere.

osStatus_t initializePacketQueue(void) {
    packetQueue_id = osMessageQueueNew(
		PACKET_QUEUE_FULL,
        sizeof(PacketData_t),
        NULL
    );

    if (packetQueue_id == NULL) {
        return osErrorResource;
    }
    return osOK;
}

void collapsePacketQueue(void){
	osMessageQueueDelete(packetQueue_id);
}

osStatus_t pullPacketQueue(PacketData_t *packetData) {

    if (packetData == NULL) {
        return osErrorParameter;
    }

    // Critical Check: Ensure the queue handle is initialized.
	if (packetQueue_id == NULL) { // FIX: Used packetQueue_id
		// Return a status indicating the resource is unavailable/uninitialized
		return osErrorResource;
	}

    osStatus_t QueueStatus;

    QueueStatus = osMessageQueueGet(
            packetQueue_id,      // FIX: Used packetQueue_id
			packetData,          // Pointer to the buffer where the received message will be stored
			PACKET_QUEUE_PRIORITY,// Priority is ignored for GET but often used as a dummy param in some ports
			PACKET_QUEUE_TIMEOUT  // The maximum time the task will wait if the queue is empty
    );

    return QueueStatus;
}

osStatus_t pushPacketQueue(const PacketData_t* packetData){
	if (packetData == NULL) {
	        return osErrorParameter;
	}

	// Critical Check: Ensure the queue handle is initialized.
	if (packetQueue_id == NULL) { // FIX: Used packetQueue_id
		// Return a status indicating the resource is unavailable/uninitialized
		return osErrorResource;
	}

	osStatus_t QueueStatus;

	QueueStatus = osMessageQueuePut(
			packetQueue_id,        // FIX: Used packetQueue_id
			packetData,            // Pointer to the message data (SOURCE buffer)
			PACKET_QUEUE_PRIORITY,// Priority is ignored for GET but often used as a dummy param in some ports
			PACKET_QUEUE_TIMEOUT  // The maximum time the task will wait if the queue is empty
	);

	return QueueStatus;
}


/**
 * @brief Drains all available packets from the queue and stores them in a buffer. Non-blocking.
 * @param packetData Pointer to the destination buffer (array).
 * @return uint32_t The number of packets successfully drained and stored.
 */
uint32_t drainPacketQueue(PacketData_t* packetData) {
    osStatus_t status = osOK;
    uint32_t count = 0;
    PacketData_t tmpevt = {0}; // Use PacketData_t for the temporary structure

    // Assuming EVENT_QUEUE_FULL is the max size of the external buffer
    while (status == osOK && count < PACKET_QUEUE_FULL) {

        status = pullPacketQueue(&tmpevt); // FIX: Called pullPacketQueue

        if (status == osOK) {
            // Success: The packet was retrieved. Store it and increment count.
        	packetData[count] = tmpevt;
        	count++;
        }
        else if (status == osErrorResource) {
            // Queue is Empty. Exit loop naturally.
        }
        else if (status == osErrorParameter || status == osErrorISR) {
            // Critical Error: Immediate abandonment required.
            return 0; // Return 0 new packets processed.
        }
    }

    // Return the total number of packets successfully stored.
    return count;
}


uint32_t fillPacketQueue(const PacketData_t* packetData, int size) {
	osStatus_t status = osOK;
	uint32_t count = 0;

	if(size < 0 || (uint32_t)size > PACKET_QUEUE_FULL){
		return 0;
	}

	while (status == osOK && count < (uint32_t)size) {

		status = pushPacketQueue(packetData + count); // FIX: Called pushPacketQueue

		if (status == osOK) {
			count++;
		}
		else if (status == osErrorResource) {
			break; // Queue is full. Stop filling.
		}
		else if (status == osErrorParameter || status == osErrorISR) {
			break; // Critical error. Stop filling.
		}
	}

	// Return the total number of packets successfully stored.
	return count;
}
