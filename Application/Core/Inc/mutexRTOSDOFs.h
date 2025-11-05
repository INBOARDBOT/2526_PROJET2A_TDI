// mutexRTOSDOFs.h
#ifndef MUTEX_RTOS_DOFs_H
#define MUTEX_RTOS_DOFs_H

#include "cmsis_os.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif



extern volatile uint16_t g_axes[4];
extern osMutexId_t g_axesMutex;

#ifdef __cplusplus
}
#endif


#endif // MUTEX_RTOS_DOFs_H
