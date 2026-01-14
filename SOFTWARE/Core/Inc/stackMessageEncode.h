#ifndef STACKMESSAGEENCODE_H
#define STACKMESSAGEENCODE_H


#include "stackProtocolDefs.h"

// ==================== TRANSMIT METHODS ====================


uint8_t create_command_packet(uint8_t address, MotionCommand_t command, uint8_t* data, uint8_t datalength, uint8_t* packetBuffer);

// int create_state_packet(uint8_t address, MotionCommand_t stateCommand, uint8_t* packetBuffer, uint8_t BufferSize);

// int create_error_packet(uint8_t address, MotionCommand_t errorCommand, uint8_t* packetBuffer, uint8_t BufferSize);

int create_data_packet(uint8_t address, MotionCommand_t stateCommand, uint16_t dataValue, uint8_t* packetToSend);

void set_cycle_id(uint8_t* Buffersize, uint8_t cycleId);

#endif
