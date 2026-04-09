#include "mutexRTOSDOFs.h"

volatile uint8_t g_axes[8] = {0};
osMutexId_t g_axesMutex = NULL;

const osMutexAttr_t g_axesMutexAttr = {
    .name = "axesMutex",
    .attr_bits = 0U,
    .cb_mem = NULL,
    .cb_size = 0U
};

void InitAxesMutex(void)
{
    if (g_axesMutex == NULL)
    {
        g_axesMutex = osMutexNew(&g_axesMutexAttr);
        if (g_axesMutex == NULL)
        {
            Error_Handler();
        }
    }
}
