#ifndef STACKMESSAGEDECODE_H
#define STACKMESSAGEDECODE_H
 
#include "motionErrorHandler.h"
#include "motionSystemDefs.h"
#include "motionSystem.h"
#include "stackProtocolDefs.h"
#include "stackMessageEncode.h"
#include <stdint.h>
#include <string.h>

typedef struct{
    uint8_t address;
    MotionCommand_t queryType;
} motionQueryResponse_t;

// not sure about this struct maybe not in this header but must be thrown to send_start command in future send start class

// ==================== RECEIVE METHODS ====================

int decoder_parse(MotionResponse_t* resp, const uint8_t* raw, uint8_t len);

int handle_response(const MotionResponse_t* response);


// ==================== RESPONSE HANDLERS ====================

int handle_receive_data(const MotionResponse_t* resp);

int handle_soft_error(const MotionResponse_t* resp);

int handle_hard_error(const MotionResponse_t* resp);

int handle_query(const MotionResponse_t* resp);

#endif
