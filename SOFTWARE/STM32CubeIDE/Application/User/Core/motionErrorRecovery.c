#include "motionErrorRecovery.h"

static int retryCounter = 0;

void retryLastCommand(void){


	retryCounter++;
}

void emergencyCoolDown(void){

}
