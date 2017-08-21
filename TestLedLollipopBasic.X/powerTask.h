/* 
 * File:   powerTask.h
 * Author: Pedro Dorticos
 * Comments:
 * Revision history: 
 */

#ifndef _POWER_TASK_ENABLED
#define	_POWER_TASK_ENABLED

#include <xc.h> // include processor files - each processor file is guarded. 
#include "sfxProtocol.h"
#include "sit.h"

#define _PWR_VCAP_CNTRL_BY_TIME             1

typedef enum _POWER_TASK_COMMANDS_T {
    PWR_CMD_INITIALIZE,
    PWR_CMD_VDD_EN,
    PWR_CMD_VDD_DIS,
    PWR_CMD_VCC_EN,
    PWR_CMD_VCC_DIS,
    PWR_CMD_PWR_DOWN
} POWER_TASK_COMMANDS_T;

void powerTaskPowerDown(void);
void powerTaskTick(void);
taskResult_t powerTask(void);
void powerTaskNewTask(POWER_TASK_COMMANDS_T command);
dataQualifier_t powerTaskGetVarValidity(unsigned char variableID);

#endif	/* _POWER_TASK_ENABLED */

