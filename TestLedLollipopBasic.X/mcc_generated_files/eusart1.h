#ifndef _EUSART1_H
#define _EUSART1_H

#include <xc.h>
#include <stdbool.h>
#include <stdint.h>
#include "sfxProtocol.h"

#define GPS_MAX_TTFF                        180
#define IGNORE_INIT_ERROR_FLAG              1
#define GPS_DEFAULT_DATUM_SPAIN_PORT        1
#define GPS_GET_ALTITUDE                    1

typedef enum gpsResponses {
    RMC_FILTER,
    GGA_FILTER,
    GPS_MTK_CMD_ACK,
    GPS_RESET_ACK};
        
typedef enum _GPS_TASK_COMMANDS_T {
    GPS_CMD_INITIALIZE,
    GPS_CMD_ACQUIRE,
    GPS_CMD_PWR_DOWN
} GPS_TASK_COMMANDS_T;
// Compiler Switches

// COMMANDS
typedef enum _gpsCommands_t {   
//    GPS_HOTSTART_MSG,
    GPS_WARMSTART_MSG,
    GPS_COLDSTART_MSG,
//    GPS_IMPROVE_TTFF_MSG,
    GPS_SBAS_DGPS_MSG,
    GPS_SBAS_EN_MSG,
    GPS_SET_DATUM_MSG,
    GPS_REFRESH_MSG,
    GPS_RECEIVE_UNS_RMC_MSG,
    GPS_RECEIVE_UNS_GGA_MSG,
    GPS_POWER_DOWN_MSG
} gpsCommands_t;

// Command Content
#define GPS_HOTSTART_MSG_STR "101*32"
#define GPS_WARMSTART_MSG_STR "102*31"
#define GPS_COLDSTART_MSG_STR "103*30"
#define GPS_IMPROVE_TTFF_MSG_STR "257,1*2F"
#define GPS_SBAS_DGPS_MSG_STR "301,2*2E"
#define GPS_SBAS_EN_MSG_STR "313,1*2E"
#define GPS_SET_DATUM_MSG_STR "330"
#define GPS_REFRESH_MSG_STR "741"
#define GPS_RECEIVE_UNS_RMC_MSG_STR ""
#define GPS_RECEIVE_UNS_GGA_MSG_STR ""
#define GPS_POWER_DOWN_MSG_STR "161,0*28"


dataQualifier_t gpsTaskGetVarValidity(uint8_t variableID);
void gpsTaskPowerDown(void);
void gpsTaskNewTask(GPS_TASK_COMMANDS_T command);
taskResult_t gpsTask(void);
void gpsTaskTick(void);

void EUSART1_Initialize(void);
void EUSART1_Transmit_ISR(void);
void EUSART1_Receive_ISR(void);

#endif  // _EUSART1_H

/**
 End of File
*/
