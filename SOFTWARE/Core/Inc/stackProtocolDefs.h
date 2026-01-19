#ifndef STACKPROTOCOLDEFS_H
#define STACKPROTOCOLDEFS_H


#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>

// ==================== SYSTEM CONSTANTS ====================

// Address of each driver
#define ADDRESSPISTONTL  	  0x00 	     // address of the upper left driver
#define ADDRESSPISTONTR  	  0x01 	     // address of the lower left driver
#define ADDRESSPISTONBL  	  0x02 	     // address of the upper right driver
#define ADDRESSPISTONBR       0x03 	     // address of the lower right driver
#define ADDRESSBROADCAST      0x0F       // address of broadcast : send to every driver

#define DRIVER_NUMBER         4

// Protocol constants
// Packet framing
#define START_BYTE             0xAA      // Start of packet
#define STOP_BYTE              0x55      // End of packet

//
#define MAX_DATA                64        // Max data (1 data = 16bits)

// Timeouts
#define TIMEOUT_MS              100       // Default timeout for master waiting for slave response
#define SLAVE_RESPONSE_TIME_MS  50        // Default time for slave to respond


#define PACKET_QUEUE_EMPTY       0        // event driver queue is empty
#define PACKET_QUEUE_FULL        4        // event driver queue is full
#define PACKET_QUEUE_TIMEOUT     0        // event timeout before insertion in event queue
#define PACKET_QUEUE_PRIORITY    0U       // basic priority
#define MAX_PAYLOAD_SIZE         16       // Max bytes in data[] of MotionPacket_t

// FLAGS bitmask definitions Slave to Master
#define FLAGS_ACK                 0x01    // Bit 0: Acknowledgment flag
#define FLAGS_INIT_DONE           0x02    // Bit 1: Initialization sequence done
#define FLAGS_MOTOR_EN            0x04    // Bit 2: Motor enabled
#define FLAGS_MOTOR_MOVING        0x08    // Bit 3: Motor currently moving
#define FLAGS_BUSY                0x10    // Bit 4: Driver is busy processing a command
#define FLAGS_EMPTY_DATA          0x20    // Bit 5: Packet has no data
#define FLAGS_EMPTY_STATUS        0x40    // Bit 6: Packet has no event/error
#define FLAGS_TO_DEFINE_0         0x80    // Bit 7: Undefined

// FLAGS bitmask definitions Master to Slave
#define FLAGM_AWN_REQUEST         0x01    // Bit 0: Master requests Acknowledgment With Notification
#define FLAGM_CYCLE_ID_RST        0x02    // Bit 1: Process cycle ID reset
#define FLAGM_EMPTY_DATA          0x04    // Bit 2: Packet has no data
#define FLAGM_EMPTY_STATUS        0x08    // Bit 3: Packet has no event/error
#define FLAGM_TO_DEFINE_1         0x10    // Bit 4: Undefined
#define FLAGM_TO_DEFINE_2         0x20    // Bit 5: Undefined
#define FLAGM_TO_DEFINE_3         0x40    // Bit 6: Undefined
#define FLAGM_TO_DEFINE_4         0x80    // Bit 7: Undefined



// MASTER STATES
#define MASTER_STATE_IDLE            0x00
#define MASTER_STATE_INIT            0x01
#define MASTER_STATE_PLAY            0x02
#define MASTER_STATE_ERROR           0x03
#define MASTER_STATE_MENU            0x04
#define MASTER_STATE_SHUTDOWN        0x05
#define MASTER_STATE_RESET           0x06
#define MASTER_STATE_EMERGENCY_STOP  0x07

// Command type check macros
#define isCMDTRIG(cmd) (cmd >= 0x00 && cmd <= 0x05)
#define isCMDMOTN(cmd) (cmd >= 0x10 && cmd <= 0x17)
#define isEVTMOTN(cmd) (cmd >= 0x40 && cmd <= 0x48)
#define isEVTSYST(cmd) (cmd >= 0x50 && cmd <= 0x52)
#define isERRSOFT(cmd) (cmd >= 0x80 && cmd <= 0x86)
#define isERRHARD(cmd) (cmd & 0xA0)
#define isERRCOMM(cmd) (cmd >= 0xA0 && cmd <= 0xA1)


// ==================== ENUMERATIONS AND STRUCTURES ====================


typedef enum {
    CMD_NOP                 = 0x00,   // no operation
    CMD_PING                = 0x01,
    CMD_GET_STATUS          = 0x03,
    CMD_GET_EVENTS          = 0x04,
    CMD_GET_ERRORS          = 0x05,
	// CMD_START_INIT
    CMD_ENABLE_MOTOR        = 0x10,
    CMD_DISABLE_MOTOR       = 0x11,
    CMD_SET_TARGET_POSITION = 0x12,
    CMD_SET_TARGET_SPEED    = 0x13,
    CMD_SET_ACCEL           = 0x14,
    CMD_SET_DECEL           = 0x15,
    CMD_START_MOVE          = 0x16,
    CMD_STOP_MOVE           = 0x17,

    // reserved for future motion commands
    CMD_START_PLAY          = 0x20,
	CMD_STOP_PLAY           = 0x21,

} MotionCommand_t;

typedef enum {
    EVT_NONE                   = 0x00,
    EVT_MOVEMENT_STARTED       = 0x01,
    EVT_MOVEMENT_COMPLETED     = 0x02,
    EVT_TARGET_REACHED         = 0x03,
    EVT_LIMIT_SWITCH_MIN       = 0x04,
    EVT_LIMIT_SWITCH_MAX       = 0x05,
    EVT_HOMING_STARTED         = 0x06,
    EVT_HOMING_COMPLETED       = 0x07,
    EVT_DRIVER_ONLINE          = 0x08,
    EVT_DRIVER_OFFLINE         = 0x09,
    EVT_PARAMETER_CHANGED      = 0x0A,
	EVT_CALIBRATION_STARTED    = 0x0B,
	EVT_CALIBRATION_COMPLETED  = 0x0C,
	EVT_PLAYMODE_ON            = 0x0D,
	EVT_PLAYMODE_OFF           = 0x0E
} MotionEvent_t;


typedef enum {
    ERR_NONE                   = 0x00,
    ERR_INVALID_COMMAND        = 0x01,
    ERR_INVALID_PARAM          = 0x02,
    ERR_TIMEOUT                = 0x03, // Driver doesn't respond in the delay
    ERR_CHECKSUM_FAIL          = 0x04,
	ERR_STALL_DETECTED         = 0x06,
	ERR_UNDERVOLTAGE           = 0x07, // Alimentation undervoltage in one or more drivers
	ERR_UNDERVOLTAGEMCU        = 0x08, // Undervoltage in MCU Vref
    ERR_FAILED_CALIB           = 0x09, // Driver(s) cannot calibrate the motion system
    ERR_FAILED_SET_PIS_POS     = 0x0A, // Piston(s) cannot set to the given position
    ERR_COMM_LOST              = 0x0B,
    ERR_INVALID_FRAME          = 0x0C,

    ERR_OVERHEATING            = 0xA0, // Overheating in one or more drivers
    ERR_OVERCURRENT            = 0xA1, // Overcurrent in one or more drivers
    ERR_OVERVOLTAGE            = 0xA2, // Alimentation overvoltage in one or more drivers
    ERR_OVERVOLTAGEMCU         = 0xA3 // Overvoltage in MCU Vref
} MotionError_t;


typedef struct __attribute__((packed))
{
    uint8_t startByte;
    uint8_t address;
    uint8_t cycleId;
    uint8_t command;

    uint8_t statusFlags;
    uint8_t payloadCount;
    uint8_t FIFOlen;

    union {
        struct {
            uint8_t type;
            uint8_t code;
        } fifoEntries[MAX_DATA / 2];

        uint16_t infoArray[MAX_DATA / 2];

        uint8_t rawBytes[MAX_DATA];
    } payload;

    uint8_t checksum;
    uint8_t stopByte;
} MotionPacket_t;



// ==================== UTILITY METHODS ====================
/**
 * @brief Sums all bytes in the packet up to the checksum field.
 */
static inline uint8_t calculateChecksum(const MotionPacket_t *packet) {
    uint8_t crc = 0;
    uint8_t *ptr = (uint8_t*)packet;

    // Calculate length to sum: Total size minus checksum and stopByte
    // offsetof(MotionPacket_t, checksum) is the most robust way to do this
    uint16_t length = offsetof(MotionPacket_t, checksum);

    for (uint16_t i = 0; i < length; i++) {
        crc += ptr[i];
    }

    return crc;
}

/**
 * @brief Builds a packet packing BOTH FIFO and Info data into the union sequentially.
 * * @param fifoLen Number of type/code pairs to add.
 * @param dataLen Number of uint16_t values to add.
 */
static inline void buildMotionPacket(MotionPacket_t *packet, uint8_t addr, uint8_t flags,
                                     uint8_t* type, uint8_t* code, uint8_t fifoLen,
                                     uint16_t* data, uint8_t dataLen) {

    memset(packet, 0, sizeof(MotionPacket_t));

    // 1. Header Setup
    packet->startByte   = START_BYTE;
    packet->address     = addr;
    packet->statusFlags = flags;
    packet->stopByte    = STOP_BYTE;

    uint8_t offset = 0;

    // 2. Pack FIFO (2 bytes per entry: type + code)
    if (type != NULL && code != NULL) {
        for (uint8_t i = 0; i < fifoLen; i++) {
            if (offset + 2 <= MAX_DATA) {
                packet->payload.rawBytes[offset++] = type[i];
                packet->payload.rawBytes[offset++] = code[i];
            }
        }
        packet->FIFOlen = fifoLen;
    }
    else{
    	packet->FIFOlen = 0;
    }

    // 3. Pack InfoArray (2 bytes per entry: uint16_t)
    if (data != NULL) {
        for (uint8_t i = 0; i < dataLen; i++) {
            if (offset + 2 <= MAX_DATA) {
                // Packing Little-Endian (LSB first)
                packet->payload.rawBytes[offset++] = (uint8_t)(data[i] & 0xFF);
                packet->payload.rawBytes[offset++] = (uint8_t)((data[i] >> 8) & 0xFF);
            }
        }
    }

    // 4. Update count and Checksum
    packet->payloadCount = fifoLen + dataLen;
    packet->checksum = calculateChecksum(packet);
}




// ==================== PREMADE PACKET ====================
static const MotionPacket_t pingPacketStructure = {
    .startByte    = START_BYTE,
    .address      = 0,
    .cycleId      = 0,
    .command      = CMD_PING,
    .statusFlags  = FLAGM_AWN_REQUEST | FLAGM_EMPTY_STATUS | FLAGM_EMPTY_DATA,
    .payloadCount = 0,
    .FIFOlen      = 0,
    .payload      = { .rawBytes = {0} },
    .checksum     = 0,
    .stopByte     = STOP_BYTE
};

#endif
