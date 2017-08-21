#ifndef _EUSART2_H
#define _EUSART2_H

#include <xc.h>
#include <stdbool.h>
#include <stdint.h>
#include "sfxProtocol.h"

/**
  Section: Macro Declarations
*/
typedef enum _SFX_TASK_COMMANDS_T {
    SFX_CMD_INITIALIZE,
    SFX_CMD_SEND_MSG,
    SFX_CMD_SEND_MSG_WAIT_ACK,
    SFX_CMD_SEND_STATUS,
    SFX_CMD_SET_SFX_MODE,
    SFX_CMD_PWR_DOWN,
    SFX_CMD_ADJ_PWR
} SFX_TASK_COMMANDS_T;

// Compiler Switches
//#define _DEBUG_SERIAL

/**
  Section: EUSART2 APIs
*/
void SigfoxTaskPowerDown(void);
taskResult_t SigfoxTask(void);
void SigfoxTaskTick(void);
void SigfoxTaskNewTask(SFX_TASK_COMMANDS_T command);
void SigfoxTaskLoadPayloadBuffer(uint8_t *rawData);

void EUSART2_Initialize(void);
void EUSART2_Transmit_ISR(void);
void EUSART2_Receive_ISR(void);

#endif  // _EUSART2_H
/**
 End of File eusart2.h
*/
