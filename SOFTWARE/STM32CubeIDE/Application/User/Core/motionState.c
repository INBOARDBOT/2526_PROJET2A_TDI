#include "motionState.h"
#include "motionCommunication.h"

// Global System Variables
motionState_t currentState                 = STATE_IDLE;
static MotionError_t pendingError          = ERR_NONE;
static motionState_t returnStateAfterError = STATE_IDLE;

// Timeout handle variable
static uint32_t lastStateChangeTick = 0;
static motionState_t previousState = STATE_IDLE;

void resetMotionFSM(void){
	currentState          = STATE_IDLE;
	pendingError          = ERR_NONE;
	returnStateAfterError = STATE_IDLE;
}

void triggerGlobalError(MotionError_t err, motionState_t safePoint, RecoveryCallback callback) {
    currentState = STATE_ERROR_RECOVERY;
    pendingError = err;
    returnStateAfterError = safePoint;

    if (callback != NULL) {
        callback();
    }
}


void checkSlaveHealth() {
    for (int i = 0; i < 4; i++) {
        if (HAL_GetTick() - Health_GetLastSeen(i) > (TIMEOUT_MS * 2)) {
        	Health_MarkOffline(i);
            triggerGlobalError(ERR_COMM_LOST, STATE_IDLE, NULL);
            return;
        }
        if (Health_GetLastError(i) == ERR_OVERHEATING) {
            triggerGlobalError(ERR_OVERHEATING, STATE_SHUTDOWN, emergencyCoolDown);
            return;
        }
    }
}

void runStateMachine() {
	bool isNewState = (currentState != previousState);
	if (isNewState) {
		lastStateChangeTick = HAL_GetTick();
		previousState = currentState;
	}

	if (currentState > STATE_PINGING && currentState != STATE_ERROR_RECOVERY) {
		checkSlaveHealth();
	}


	checkSlaveHealth();

    switch (currentState) {

        case STATE_IDLE:
            if (/* Init button pressed */ true) currentState = STATE_PINGING;
            break;

		case STATE_PINGING:
			launchPingProcedure();
			break;

        case STATE_START_INIT:
        	// doCatchSpecificEventandCommandFromDriver(); // can take several cycles, master makes sure the driver is ready to operate

        	if (isSystemSynchronized(FLAGS_INIT_DONE)) {
				currentState = STATE_CALIBRATING;
			}
            break;

        case STATE_CALIBRATING:
        	// doCatchSpecificEventandCommandFromDriver(); // can take several cycles, master makes sure the driver is callibrated

        	if (/* Calibration completed */ true) currentState = STATE_MENU;
            break;

        case STATE_MENU:
        	// doWaitForHUmanInteraction();

/*            if ( Error A occurs  false) triggerError(ERR_A, STATE_CALIBRATING);
            else if ( Play button  true) currentState = STATE_START_PLAY;
            else if ( Exit system  false) currentState = STATE_RESET;*/
            break;

        case STATE_START_PLAY:
        	// doCatchSpecificEventandCommandFromDriver(); // can take several cycles, master makes sure the driver is ready to go in playmode

            if (/* Error A occurs */ false){}// triggerError(ERR_A, STATE_CALIBRATING);
            else if (/* Drivers ready */ true) currentState = STATE_PLAY;
            break;

        case STATE_PLAY:
        	// doSendCyclesofDatatoDriver(); // infinite number of cycles until user ends it or fatal/critical error happenned

            if (/* Error A occurs */ false) {}// triggerError(ERR_A, STATE_CALIBRATING);
            else if (/* Stop button */ false) currentState = STATE_STOP;
            break;

        case STATE_STOP:
            if (/* Drivers parked */ true) currentState = STATE_MENU;
            break;

        case STATE_RESET:
            if (/* Global reset success */ true) currentState = STATE_IDLE;
            break;

        case STATE_EMERGENCY_STOP:
            if (/* Hard reset button */ false) currentState = STATE_IDLE;
            break;

        case STATE_SHUTDOWN:
			if (/* Hard reset button */ false) currentState = STATE_IDLE;
			break;

        case STATE_ERROR_RECOVERY:
            // Handle the specific error logic
            switch (pendingError) {
/*                case ERR_A: recoveryErrorA(); break;
                case ERR_B: recoveryErrorB(); break;
                case ERR_C: recoveryErrorC(); break;*/
                default: break;
            }
            // Move to the Safe Point provided when the error was triggered
            currentState = returnStateAfterError;
            pendingError = ERR_NONE;
            break;
    }
}


void launchPingProcedure(void){
	if (commState == COMM_STATE_IDLE) {
		for(int i=0; i<4; i++){
			Health_MarkOffline(i);
		}
		comm_RequestAction(STATE_PINGING, 0);
	}

	int onlineCount = 0;
	for (int i = 0; i < 4; i++) {
		if (Health_IsOnline(i)) onlineCount++;
	}

	if (onlineCount == 4) {
		printf("All drivers reached. Moving to Init.\n");
		currentState = STATE_START_INIT;
	}
	else if ((HAL_GetTick() - lastStateChangeTick) > 5000) {
		// Global Timeout: If we can't find all 4 drivers in 5 seconds
		triggerGlobalError(ERR_TIMEOUT, STATE_IDLE, NULL);
	}
}
