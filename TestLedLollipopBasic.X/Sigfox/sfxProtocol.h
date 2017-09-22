/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdbool.h>
#include <time.h>

//#define UART2_SIMULATE              1
////////////////////////////////////////////////////////////////////////////////
// TASK ENABLES
////////////////////////////////////////////////////////////////////////////////
//#define GPRS_TASK_ENABLED
#define SIGFOX_TASK_ENABLED
//#define GPS_TASK_ENABLED
#define PWR_TASK_ENABLED
//#define _1wire_TASK_ENABLED
////////////////////////////////////////////////////////////////////////////////

#define SAMPLING_SESSION_INTERVAL           600
#define COMM_SESSION_INTERVAL               600
#define SYNCH_SESSION_INTERVAL              300
#define SECONDS_IN_A_DAY                    86400

#define POWER_TASK_SESSION_TO               50
#define GPS_TASK_MEAUSUREMENT_SESSION_TO    360
#define SFX_TASK_COMM_SESSION_TO            60

typedef enum _mainTaskStates_t {
    MAIN_TASK_PARKED, 
    POWERING_TASK, 
    MEASURING_TASK, 
    COMMUNICATING_TASK
}mainTaskStates_t;

typedef enum _taskStates_t {
    TASK_PARKED, 
    TASK_RESET_TASK,
    TASK_INIT,
    TASK_READY,
    TASK_WAITING_DATA
}taskStates_t;

typedef enum _taskResult_t {
    TASK_NO_SENSOR_FOUND,
    TASK_ON_COURSE,
    TASK_DATA_READY,
    TASK_TO_ERR,
    TASK_DATA_NOT_AVAILABLE
}taskResult_t;

typedef enum _dataQualifier_t {
    SENSOR_READ,
    BORROWED,
    CALCULATED,
    NOT_AVAILABLE,
    UNKNOWN_SENSOR
}dataQualifier_t;

typedef enum _TaskCommManagerState_t {
    TASK_SEND_COMMAND_SM = 0, 
    TASK_DETECT_TRANSACTION_END_SM
} TaskCommManagerState_t;

typedef enum _systemEvent_t {
        HW_WARNING_EV,
        TIMER1_EV,
        TIMER2_EV,
        INPUT_EV,
        MOTION_EV,
        VARIABLE_ALARM_EV,
        BATTERY_EV
}systemEvent_t;

//////////////////////////////////////////////////////////////////////////
/* SYSTEM VARS*/
//////////////////////////////////////////////////////////////////////////

// Configuration Data

// GPRS Parameters
eeprom char sysVarAPN[32] = "\"wlapn.com\"";
eeprom char sysVarDestPORT[8] = "6000";
eeprom char sysVarDestADDRESS[32] = "52.88.24.74";

// GPSTask Data
bool sysVarGpsValidFlag = false;
long sysVarGpsLat;
long sysVarGpsLong;
unsigned int sysVarGpsSpeed;
int sysVarGpsCourse;
unsigned int sysVarGpsAlt = 6000;
struct tm sysVarGPS;
long sysVarGpsDayTime;
time_t sysVarUNIXDayTime;

unsigned int sysVarGpsOdometer;

// Gprs Data
char sysVarCCID[20] = "";
char sysVarIMEI[20] = "";
// Power Data
unsigned char sysVarBattery = 100;

// Digitak Input
bool sysVarMainInput = false;

// TODO Insert declarations
void ProtocolTaskSleepTimeUpdate(unsigned int watchDogSleepingTime);
void ProtocolTaskTick(void);
taskResult_t ProtocolTask(void);
void ProtocolTaskPowerDown(void);

#endif	/* XC_HEADER_TEMPLATE_H */

