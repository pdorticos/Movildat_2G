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
    GPRS_TASK_INITIALIZE,
    GPRS_TASK_SEND_MSG,
    GPRS_TASK_PWR_DOWN,
    GPRS_TASK_GET_EPO,
    GPRS_TASK_FLUSH,
    GPRS_TASK_GET_FTP_CFG,
    GRPS_TASK_SET_FTP
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

