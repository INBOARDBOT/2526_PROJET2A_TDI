#include "motionPacketHandler.h"

// Define the registry and the polling priority flag
SlaveState_t slaveRegistry[4];
bool slaveNeedsExtraPoll[4] = {false, false, false, false};

void process_slave_data(const MotionPacket_t *pPacketReceive) {
    uint8_t addr = pPacketReceive->address;
    if (addr >= 4) return;

    // 1. Update Core Telemetry (Digital Twin)
    slaveRegistry[addr].currentPosition = (pPacketReceive->payload.position.posHigh << 8) |
                                           pPacketReceive->payload.position.posLow;
    slaveRegistry[addr].lastSeen = HAL_GetTick();
    slaveRegistry[addr].isConnected = 1;

    // Update internal state flags from the bitmask
    slaveRegistry[addr].isMotorMoving = (pPacketReceive->flags & FLAG_MOTOR_MOVING) ? true : false;

    // 2. Handle Events
    if (pPacketReceive->eventCode != EVENT_NONE) {
        log_event(addr, pPacketReceive->eventCode);
    }

    // 3. Handle Errors
    if (pPacketReceive->errorCode != ERR_NONE) {
        log_error(addr, pPacketReceive->errorCode);
    }

    // 4. Reactive Polling Logic
    // If slave has more data in its internal FIFO, flag it for an immediate follow-up poll
    if (pPacketReceive->flags & (FLAG_MORE_EVENT | FLAG_MORE_ERROR)) {
        slaveNeedsExtraPoll[addr] = true;
    } else {
        slaveNeedsExtraPoll[addr] = false;
    }
}

void log_event(uint8_t address, uint8_t eventCode) {
    switch(eventCode) {
        case EVENT_STALL_DETECTED:
            if(slaveRegistry[address].isMotorMoving) {
                // doStopDynamicSystem();
                // doCalibration();
            }
            break;
        // ... other cases ...
        default:
            break;
    }
    slaveRegistry[address].lastEvent = eventCode;
}

void log_error(uint8_t address, uint8_t errorCode) {
    switch(errorCode) {
        case ERR_OVERHEATING:
        case ERR_OVERCURRENT:
            // Emergency logic here
            break;
        default:
            // printf("Slave %d: Error 0x%02X\n", address, errorCode);
            break;
    }
    slaveRegistry[address].lastError = errorCode;
}
