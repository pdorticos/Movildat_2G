/* 
 * File:   newfile.h
 * Author: pedro
 *
 * Created on July 20, 2017, 11:58 AM
 */

#include "sfxProtocol.h"

#ifndef GPRS_H
#define	GPRS_H

typedef enum _GPRS_TASK_COMMANDS_T {
    GPRS_CMD_INITIALIZE,
    GPRS_CMD_SEND_MSG,
    GPRS_CMD_PWR_DOWN,
} GPRS_TASK_COMMANDS_T;

///////////////////////////////////////////////
void gprsTaskTick(void);
void EUSART2_GPRS_Transmit_ISR(void);
void EUSART2_GPRS_Receive_ISR(void);

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* NEWFILE_H */

