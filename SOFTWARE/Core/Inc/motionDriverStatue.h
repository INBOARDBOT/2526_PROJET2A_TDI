#ifndef MOTIONDRIVERSTATUE_H
#define MOTIONDRIVERSTATUE_H

#include "stackProtocolDefs.h"
#include <stdbool.h>

typedef struct {
    bool          isOnline;      // Is the driver responding?
    uint8_t       lastFlags;     // Captured FLAGS_MOTOR_MOVING, etc.
    uint32_t      lastSeenTick;  // Timestamp of last valid packet
    MotionError_t lastError;     // Last error code reported by the driver
} PistonHealth_t;

typedef struct {
    bool iscalibrated;
    bool isMotorEnabled;
    bool isMotorMoving;
    int  mode;
} PistonState_t;

// Public interface
void Health_Update(uint8_t id, MotionPacket_t *packet);
void Health_MarkOffline(uint8_t id);
bool Health_IsSystemReady(void);
PistonHealth_t* Health_GetStatus(uint8_t id);

bool Health_IsOnline(uint8_t id);
uint32_t Health_GetLastSeen(uint8_t id);
MotionError_t Health_GetLastError(uint8_t id);

// System-wide checks
bool isSystemSynchronized(uint8_t requiredFlag);
bool isConnectionStable(void);

#endif
