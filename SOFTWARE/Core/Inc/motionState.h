#ifndef MOTIONSTATE_H
#define MOTIONSTATE_H


#include "stackProtocolDefs.h"
#include "motionErrorRecovery.h"
#include "motionDriverStatue.h"

typedef enum {
	STATE_EMERGENCY_STOP,
	STATE_IDLE,
	STATE_PINGING,
	STATE_START_INIT,
	STATE_CALIBRATING,
	STATE_MENU,
	STATE_START_PLAY,
	STATE_PLAY,
	STATE_STOP,
	STATE_RESET,
	STATE_SHUTDOWN,
	STATE_ERROR_RECOVERY
}motionState_t;

/*
typedef struct {
    MotionError_t error;
    motionState_t nextState;      // Where the FSM should go
    motionState_t currentState;   // Present matters depending of error's context
    uint8_t 	  driverID;       // For ID purpose
    bool          mustAbort;      // True if we stop all 4 drivers immediately
    const char* description;      // Useful for debugging
} ErrorBehavior_t;

// This table maps your Enum to the FSM behavior
const ErrorBehavior_t errorTable[] = {
    // Critical condition
	{ ERR_OVERCURRENT,        STATE_EMERG_STOP,     true,  "[Physical][Critical]: Over Current in Piston" },
    { ERR_OVERHEATING,        STATE_EMERG_STOP,     true,  "[Physical][Critical]: Over Heating in Piston" },
	{ ERR_OVERVOLTAGE,        STATE_EMERG_STOP,     true,  "[Physical][Critical]: Over Voltage in Piston" },
	{ ERR_OVERVOLTAGEMCU,     STATE_EMERG_STOP,     true,  "[Physical][Critical]: Over Voltage in Driver MCU" },
	{ ERR_UNDERVOLTAGE,       STATE_EMERG_STOP,     true,  "[Physical][Critical]: Under Voltage in Piston" },
	{ ERR_UNDERVOLTAGEMCU,    STATE_EMERG_STOP,     true,  "[Physical][Critical]: Under Voltage in Driver MCU" },

	// Error communication master - slaves
	{ ERR_CHECKSUM_FAIL,      STATE_ERROR_RECOVERY, false,  "[Comms][Warning]: Checksum Error" },
	{ ERR_COMM_LOST,          STATE_PINGING,        true,   "[Comms][Fatal]: Comms Lost" },
	{ ERR_INVALID_COMMAND,    STATE_EMERG_STOP,     false,  "[Comms][Warning]: Invalid Command not found" },
	{ ERR_INVALID_PARAM,      STATE_EMERG_STOP,     false,  "[Comms][Warning]: Invalid Parameter(s) not found" },
	{ ERR_INVALID_FRAME,      STATE_EMERG_STOP,     false,  "[Comms][Warning]: Invalid Frame Packet" },

	// Timouts
	{ ERR_TIMEOUT,            STATE_EMERG_STOP,     false,  "[Timeout][Warning]: Driver did not perform its task in the requested delay" },

	// Error Physical issue
	{ ERR_STALL_DETECTED,     STATE_ERROR_RECOVERY, true,   "[Physical][Fatal]: Stall detected" },
    { ERR_FAILED_CALIB,       STATE_IDLE,           true,   "[Physical][Fatal]: Calibration failed" },
	{ ERR_FAILED_SET_PIS_POS, STATE_IDLE,           true,   "[Physical][Fatal]: Reaching requested position failed" },

};*/

/**
 * Creative and Destructive method for motionState FSM
 */
void generateMotionFSM(void);
void resetMotionFSM(void);

void triggerGlobalError(MotionError_t err, motionState_t safePoint, RecoveryCallback callback);

void checkSlaveHealth();

/**
 * FSM main loop
 */
void runMotionFSM(void);

void launchPingProcedure(void);

#endif
