#ifndef MOTION_PACKET_HANDLER_H
#define MOTION_PACKET_HANDLER_H

#include "stackProtocolDefs.h"

typedef struct {
    bool isCalibrated;
    bool isMotorMoving;
    uint16_t currentPosition;
    uint8_t  lastEvent;
    uint8_t  lastError;
    uint8_t  isConnected;
    uint32_t lastSeen;
} SlaveState_t;

extern SlaveState_t slaveRegistry[4];
extern bool slaveNeedsExtraPoll[4];

void process_slave_data(const MotionPacket_t *pPacketReceive);
void log_event(uint8_t address, uint8_t eventCode);
void log_error(uint8_t address, uint8_t errorCode);

#endif
