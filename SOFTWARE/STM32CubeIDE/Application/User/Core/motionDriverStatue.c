#include "motionDriverStatue.h"
#include "stm32f7xx_hal.h"

static PistonHealth_t healthTable[DRIVER_NUMBER];
static PistonState_t stateTable[DRIVER_NUMBER];

void Health_Update(uint8_t id, MotionPacket_t *packet) {
    if (id >= 4) return;

    healthTable[id].isOnline = true;
    healthTable[id].lastSeenTick = HAL_GetTick();
    healthTable[id].lastFlags = packet->statusFlags;

    // If the packet has data, extract the error/event from payload
    if (packet->payloadCount > 0) {
        healthTable[id].lastError = (MotionError_t)packet->payload.fifoEntries[0].code;
    }
}

void Health_MarkOffline(uint8_t id) {
    if (id < 4) healthTable[id].isOnline = false;
}

bool Health_IsSystemReady(void) {
    for(int i=0; i<4; i++) {
        if(!healthTable[i].isOnline) return false;
    }
    return true;
}


bool isSystemSynchronized(uint8_t requiredFlag) {
    for (int i = 0; i < 4; i++) {
        if (!healthTable[i].isOnline) return false;
        if (!(healthTable[i].lastFlags & requiredFlag)) return false;
    }
    return true;
}

bool isConnectionStable(void) {
    int stableDrivers = 0;
    for (int i = 0; i < 4; i++) {
        // A driver is stable if seen in the last 200ms
        if (healthTable[i].isOnline &&
           (HAL_GetTick() - healthTable[i].lastSeenTick < 200)) {
            stableDrivers++;
        }
    }
    return (stableDrivers == 4);
}

bool Health_IsOnline(uint8_t id) {
    return (id < 4) ? healthTable[id].isOnline : false;
}

uint32_t Health_GetLastSeen(uint8_t id) {
    return (id < 4) ? healthTable[id].lastSeenTick : 0;
}

MotionError_t Health_GetLastError(uint8_t id) {
    return (id < 4) ? healthTable[id].lastError : ERR_NONE;
}

int getmode(int index){
	return stateTable[index].mode;
}

