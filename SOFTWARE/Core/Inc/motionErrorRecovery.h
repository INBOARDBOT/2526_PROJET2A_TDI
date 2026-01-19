#ifndef ERRORRECOVERY_H
#define ERRORRECOVERY_H

typedef void (*RecoveryCallback)(void);

void retryLastCommand(void);

void emergencyCoolDown(void);

#endif
