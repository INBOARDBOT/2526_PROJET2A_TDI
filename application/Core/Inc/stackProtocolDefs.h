#ifndef STACKPROTOCOLDEFS_H
#define STACKPROTOCOLDEFS_H


#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

// ==================== SYSTEM CONSTANTS ====================

// Address of each driver
#define ADDRESSPISTONTL  0x00 // address of the upper left driver
#define ADDRESSPISTONTR  0x01 // address of the lower left driver
#define ADDRESSPISTONBL  0x02 // address of the upper right driver
#define ADDRESSPISTONBR  0x03 // address of the lower right driver
#define ADDRESSBROADCAST 0x0F // address of broadcast : send to every driver


// Protocol constants
// Packet framing
#define START_BYTE          0xAA    // Start of packet
#define STOP_BYTE           0x55    // End of packet
#define SEPARATOR_BYTE      0x7E    // Separator byte inside packet

// Command topic (optional, if you want to classify commands by high nibble)
#define CMD_TOPIC_MASK      0xF0    // Mask to extract command topic
#define CMD_TOPIC_SHIFT     4       // Shift to get topic index

// Packet sizes
#define MINIMUMSIZEPACKET   8       // Minimum packet size without data
#define PACKET_SIZE         64      // Max size of master packet (arbitrary safe limit)
#define DATA_SIZE           2       // Max size of slave payload (if needed for future expansion)

// Generic constants
#define GENERICNUMVALUES    4       // Generic number of values (position, temp, current)
#define MAX_ERRORS          6       // Max number of errors if using multi-error payload (optional)

// Timeouts
#define TIMEOUT_MS             100  // Default timeout for master waiting for slave response
#define SLAVE_RESPONSE_TIME_MS 50   // Default time for slave to respond

// Optional: event limits (for queue handling)
#define NO_EVENT          0x40    // Default "no event" value

#define PACKET_QUEUE_EMPTY    0   // event driver queue is empty
#define PACKET_QUEUE_FULL     4   // event driver queue is full
#define PACKET_QUEUE_TIMEOUT  0   // event timeout before insertion in event queue
#define PACKET_QUEUE_PRIORITY 0U  // basic priority
#define MAX_PAYLOAD_SIZE     16  // Max bytes in data[] of MotionPacket_t

// FLAGS bitmask definitions Slave to Master
#define FLAG_MOTOR_ENABLED       0x01    // Bit 0: Motor enabled
#define FLAG_MOTOR_MOVING        0x02    // Bit 1: Motor currently moving
#define FLAG_MORE_EVENT          0x04    // Bit 2: more event present
#define FLAG_INIT_DONE           0x08    // Bit 3: Initialization sequence done
#define FLAG_BUSY                0x10    // Bit 4: Driver is busy processing a command
#define FLAG_POSITION_REACHED    0x20    // Bit 5: Target position reached
#define FLAG_MORE_ERROR          0x40    // Bit 6: more error present
#define FLAG_ACK                 0x80    // Bit 7: Acknowledgment flag

// FLAGS bitmask definitions Master to Slave
#define FLAG_EMPTY_DATA          0x01    // Bit 0: Packet has no data
#define FLAG_CYCLE_ID_RESET      0x02    // Bit 1: Process cycle ID reset
#define FLAG_NO_EVENT            0x04    // Bit 2: Packet has no event
#define FLAG_NO_ERROR            0x08    // Bit 3: Packet has no error
#define FLAG_AWN_REQUEST         0x10    // Bit 4: Master requests Acknowledgment With Notification
#define FLAG_TO_DEFINE_1         0x20    // Bit 5: Target position reached
#define FLAG_TO_DEFINE_2         0x40    // Bit 6: Event present
#define FLAG_TO_DEFINE_3         0x80    // Bit 7: Homing procedure done



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
#define isERRHARD(cmd) (cmd >= 0x90 && cmd <= 0x97)
#define isERRCOMM(cmd) (cmd >= 0xA0 && cmd <= 0xA1)


// ==================== ENUMERATIONS AND STRUCTURES ====================


typedef enum {
    CMD_NOP                 = 0x00,   // no operation
    CMD_PING                = 0x01,
    CMD_PONG                = 0x02,
    CMD_GET_STATUS          = 0x03,
    CMD_GET_EVENTS          = 0x04,
    CMD_GET_ERRORS          = 0x05,

    CMD_ENABLE_MOTOR        = 0x10,
    CMD_DISABLE_MOTOR       = 0x11,
    CMD_SET_TARGET_POSITION = 0x12,
    CMD_SET_TARGET_SPEED    = 0x13,
    CMD_SET_ACCEL           = 0x14,
    CMD_SET_DECEL           = 0x15,
    CMD_START_MOVE          = 0x16,
    CMD_STOP_MOVE           = 0x17,

    // reserved for future motion commands
    CMD_RESERVED_1          = 0x20,
    CMD_RESERVED_2          = 0x21,

} MotionCommand_t;

typedef enum {
    EVENT_NONE                 = 0x40,
    EVENT_MOVEMENT_STARTED     = 0x41,
    EVENT_MOVEMENT_COMPLETED   = 0x42,
    EVENT_TARGET_REACHED       = 0x43,
    EVENT_LIMIT_SWITCH_MIN     = 0x44,
    EVENT_LIMIT_SWITCH_MAX     = 0x45,
    EVENT_STALL_DETECTED       = 0x46,
    EVENT_HOMING_STARTED       = 0x47,
    EVENT_HOMING_COMPLETED     = 0x48,

    // system-level notifications
    EVENT_DRIVER_ONLINE        = 0x50,
    EVENT_DRIVER_OFFLINE       = 0x51,
    EVENT_PARAMETER_CHANGED    = 0x52,

    // reserve future event codes
    EVENT_RESERVED_1           = 0x60,
    EVENT_RESERVED_2           = 0x61,

} MotionEvent_t;


typedef enum {
    ERR_NONE                   = 0x80,
    ERR_INVALID_COMMAND        = 0x81,
    ERR_INVALID_PARAM          = 0x82,
    ERR_BUSY                   = 0x83,
    ERR_TIMEOUT                = 0x84,
    ERR_CHECKSUM_FAIL          = 0x85,
    ERR_UNSUPPORTED            = 0x86,

    // motor-specific errors
    ERR_OVERHEATING        = 0x90, // Overheating in one or more drivers
    ERR_OVERCURRENT        = 0x91, // Overcurrent in one or more drivers
    ERR_UNDERVOLTAGE       = 0x92, // Alimentation undervoltage in one or more drivers
    ERR_OVERVOLTAGE        = 0x93, // Alimentation overvoltage in one or more drivers
    ERR_OVERVOLTAGEMCU     = 0x94, // Overvoltage in MCU Vref
    ERR_UNDERVOLTAGEMCU    = 0x95, // Undervoltage in MCU Vref
    ERR_FAILEDCALIBRATION  = 0x96, // Driver(s) cannot calibrate the motion system
    ERR_FAILEDSETPISTONPOS = 0x97, // Piston(s) cannot set to the given position

    // communication errors
    ERR_COMM_LOST              = 0xA0,
    ERR_FRAME_INVALID          = 0xA1,

    // reserve future error codes
    ERR_RESERVED_1             = 0xB0,
    ERR_RESERVED_2             = 0xB1,

} MotionError_t;


typedef struct
{
    uint8_t startByte;
    uint8_t address;
    uint8_t command;       // CMD_GET_STATUS
    uint8_t cycleId;

    // Always present: position (LSB first)
    union {
        struct {
            uint8_t posLow;   // LSB of position (if command is position-related)
            uint8_t posHigh;  // MSB of position
        } position;
        uint8_t data[DATA_SIZE];  // Generic data array (e.g., for raw bytes)
    } payload;

    // Status flags (bitmask)
    uint8_t flags;

    // FIFO dequeue fields
    uint8_t eventCode;     // EVENT_NONE if no new event
    uint8_t errorCode;     // ERR_NONE if no new error

    uint8_t checksum;
    uint8_t stopByte;

} MotionPacket_t;



// ==================== UTILITY METHODS ====================
static inline uint8_t checksum_bytes(const uint8_t *buf, size_t len)
{
    uint8_t sum = 0;
    for (size_t i = 0; i < len; ++i) sum = (uint8_t)(sum + buf[i]);
    return (uint8_t)(~sum + 1); // two's complement so sum+chk == 0
}

#endif
