#include "cmsis_os.h"
#include <stdbool.h>

//osMutexId_t g_ledMutex;
osMutexId_t g_axesMutex;

volatile uint16_t g_axes[4];
//volatile bool g_leds[3];

