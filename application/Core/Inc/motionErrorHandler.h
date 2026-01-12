#ifndef MOTIONERRORHANDLER_H
#define MOTIONERRORHANDLER_H

#include <stdint.h>
#include <stdbool.h>
#include "StackProtocolDefs.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// --- ENUMS & STRUCTS ---

// 1. Component Source IDs
typedef enum {
    SOURCE_MAIN = 0,
	SOURCE_HANDLE_PACKET_QUEUE,
    SOURCE_MOTION_SYSTEM,
    SOURCE_MOTION_COMMUNICATION,
    SOURCE_DRIVER_4,
} ErrorSource;

// 2. Master/Driver Error Codes
typedef enum {
    ERR_OK = 0,
    ERR_TIMEOUT,
    ERR_CRC_FAILURE,
    ERR_INVALID_CONFIG,
	ERR_FILLING_PACKET_QUEUE,
	ERR_DRAINING_PACKET_QUEUE,
    ERR_COMM_BUSY,
} ErrorCode;

// 3. Severity Levels
typedef enum {
    SEVERITY_INFO,
    SEVERITY_WARNING,
    SEVERITY_CRITICAL,
    SEVERITY_FATAL,
} ErrorSeverity;

// 4. Central Error Structure
typedef struct {
    ErrorSource source;
    ErrorCode code;
    ErrorSeverity severity;
    unsigned long timestamp;
    const char* message;
} Error_t;

// --- CALLBACK DEFINITION ---
typedef void (*ErrorHandlerCallback_t)(const Error_t* error);



#endif
