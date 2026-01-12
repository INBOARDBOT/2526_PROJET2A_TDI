#ifndef MUTEX_RTOS_DOFs_H
#define MUTEX_RTOS_DOFs_H

#include "cmsis_os.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

extern volatile uint8_t g_axes[8];
extern osMutexId_t g_axesMutex;
extern const osMutexAttr_t g_axesMutexAttr;

// Add initialization function
void InitAxesMutex(void);

#ifdef __cplusplus
}
#endif

#endif // MUTEX_RTOS_DOFs_H
