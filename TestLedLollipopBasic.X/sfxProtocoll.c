#include "mcc_generated_files/mcc.h"
#include "sfxProtocol.h"
#include "powerTask.h"
#include "sit.h"

//#define _prueba 1

static bool protVarSystemSynchronizedFlag = false;
unsigned long protVarMeasurementTimer;
unsigned long protVarCommunicationTimer;
static mainTaskStates_t PROT_STATE = MAIN_TASK_PARKED;
uint8_t protDataShifter[12];

const uint8_t SIT_FRAME_TRACKER_SF_ID_DESC[] = {TRACKER_SIGFOX_FRAME};
const uint8_t SIT_FRAME_POWERUP_SF_DESC[] =  {POWERUP_SIGFOX_FRAME};

bool ProtocolSetNextMeasTime(void){

    if(!protVarSystemSynchronizedFlag){
        if(sysVarGpsValidFlag){
            protVarSystemSynchronizedFlag = true;
            protVarMeasurementTimer = SAMPLING_SESSION_INTERVAL - (sysVarGpsDayTime % SAMPLING_SESSION_INTERVAL);
            protVarCommunicationTimer = COMM_SESSION_INTERVAL - (sysVarGpsDayTime % COMM_SESSION_INTERVAL);
            return(false);
        }
    }
    if(protVarMeasurementTimer == 0){       
        if(!protVarSystemSynchronizedFlag){
            protVarMeasurementTimer = SYNCH_SESSION_INTERVAL;
        }
        else{
            protVarMeasurementTimer = SAMPLING_SESSION_INTERVAL - (sysVarGpsDayTime % SAMPLING_SESSION_INTERVAL);            
        }
        return(true);
    }
    return(false);
}
bool ProtocolSetNextCommTime(void){
#ifndef GPS_TASK_ENABLED
    protVarSystemSynchronizedFlag = true;
#endif
    if(!protVarSystemSynchronizedFlag){
        return(false);
    }      

    if(protVarCommunicationTimer == 0){
        protVarCommunicationTimer = COMM_SESSION_INTERVAL - (sysVarGpsDayTime % COMM_SESSION_INTERVAL) ;
        return(true);
    }
    return(false);
}
dataQualifier_t ProtocolGetValidity(uint8_t variableID){
    dataQualifier_t taskResponse;
    taskResponse = gpsTaskGetVarValidity(variableID);
    if(taskResponse != UNKNOWN_SENSOR){
        return(taskResponse);
    }
    taskResponse = powerTaskGetVarValidity(variableID);
    if(taskResponse != UNKNOWN_SENSOR){
        return(taskResponse);
    }
    return(UNKNOWN_SENSOR);
}
void ProtocolAddFrameData(unsigned long Data, uint8_t DataRepSize){
    uint8_t toLeft, i, bytes;
    unsigned long longMask;
    
    toLeft = DataRepSize / 8;
    for(i = 0; i < toLeft; i++){
        for(bytes = 0; bytes < 11; bytes++){
            protDataShifter[bytes] = protDataShifter[bytes + 1];
        }
        protDataShifter[bytes] = 0;
    }   
    toLeft = DataRepSize % 8;
    for(i = 0; i < toLeft; i++){
        for(bytes = 0; bytes < 12; bytes++){
            protDataShifter[bytes] = (protDataShifter[bytes] * 2);
            if(protDataShifter[bytes + 1] & 0x80){
                 protDataShifter[bytes] |= 1;
            }
            else{
                protDataShifter[bytes] &= 0xFE;
            }
        }
        protDataShifter[bytes] &= 0xFE;
    }
    longMask = 1;
    longMask = ((longMask << DataRepSize) - 1) & Data;
    for(i = 11; i > 7; i--){
        protDataShifter[i] |= (longMask & 0xFF);
        longMask = (longMask>>8);
    }    
}
bool ProtocolTaskBuildFrame(systemEvent_t EVENT){
    
    int totalBits = 12 * 8;
    unsigned long dataField;
    uint8_t dataSize;   
    const uint8_t *protFrameDescPtr;
    
    switch(EVENT){
        case TIMER1_EV:
            protFrameDescPtr = SIT_FRAME_TRACKER_SF_ID_DESC;           
            break;
        case TIMER2_EV:
            protFrameDescPtr = SIT_FRAME_POWERUP_SF_DESC;
            break;
        case INPUT_EV:
        case MOTION_EV:
        case VARIABLE_ALARM_EV:
        case BATTERY_EV:
        default:
            return(false);
    }
    ProtocolAddFrameData(EVENT, EVENT_REP_SIZE);
    totalBits -= EVENT_REP_SIZE;   
    ProtocolAddFrameData(protFrameDescPtr[0], SIT_FRAME_ID_SIZE);
    totalBits -= SIT_FRAME_ID_SIZE;
    protFrameDescPtr++;
    do{
        switch(*protFrameDescPtr){           
            case SITVAR_7FH_VALIDITY:
                dataField = (unsigned long) ProtocolGetValidity(protFrameDescPtr[1]);
                dataSize = SITVAR_7FH_SIZE;
                break;                    
            case SITVAR_0BH_BATTERY:
                dataField = (unsigned long)sysVarBattery;
                dataSize = SITVAR_0BH_SIZE;
                break;
            case SITVAR_7EH_MAIN_INPUT:
                dataField = (unsigned long)sysVarMainInput;
                dataSize = SITVAR_7EH_SIZE;
                break;
            case SITVAR_16H_LATITUDE:
                dataField = (unsigned long)sysVarGpsLat;
                dataSize = SITVAR_16H_SIZE;
                break;
            case SITVAR_18H_LONGITUDE:
                dataField = (unsigned long)sysVarGpsLong;
                dataSize = SITVAR_18H_SIZE;
                break;
            case SITVAR_1AH_GPS_SPEED: 
                dataField = (unsigned long)sysVarGpsSpeed;
                dataSize = SITVAR_1AH_SIZE;
                break;
            case SITVAR_1BH_GPS_HEADING: 
                dataField = (unsigned long)sysVarGpsCourse;
                dataSize = SITVAR_1BH_SIZE;
                break;
            case SITVAR_20H_GPS_ODOMETER: 
                dataField = (unsigned long)sysVarGpsOdometer;
                dataSize = SITVAR_20H_SIZE;
                break;
        }
        ProtocolAddFrameData(dataField,dataSize);
        totalBits -= dataSize;
        if(totalBits < 0){
            return(false);
        }
        protFrameDescPtr++;
    }while(*protFrameDescPtr != 0);
    if(totalBits>0){
        ProtocolAddFrameData(0,totalBits);
    }
#ifdef SIGFOX_TASK_ENABLED
#ifdef _prueba
    protDataShifter[0] = 0xA5;
    protDataShifter[1] = 0x05;
    protDataShifter[2] = 0x01;
    protDataShifter[3] = 0x25;
    protDataShifter[4] = 0x48;
    protDataShifter[5] = 0x65;
    protDataShifter[6] = 0x6C;
    protDataShifter[7] = 0x6C;
    protDataShifter[8] = 0x6F;
    protDataShifter[9] = 0x1F;
    protDataShifter[10] = 0x02;
    protDataShifter[11] = 0x5A;
#endif
    SigfoxTaskLoadPayloadBuffer(protDataShifter);
#endif
    return(true);
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void ProtocolTaskTick(void){
    static bool _500msToSecond;
    
    _500msToSecond = !_500msToSecond;
    if(_500msToSecond){
        sysVarGpsDayTime++;
        if(sysVarGpsDayTime >= SECONDS_IN_A_DAY){
            sysVarGpsDayTime -= SECONDS_IN_A_DAY;
        }
        if(protVarMeasurementTimer){
            protVarMeasurementTimer--;
        }
        if(protVarCommunicationTimer){
            protVarCommunicationTimer--;
        }
        #ifdef PWR_TASK_ENABLED
            powerTaskTick();
        #endif
        #ifdef GPS_TASK_ENABLED
            gpsTaskTick();
        #endif
        #ifdef SIGFOX_TASK_ENABLED
            SigfoxTaskTick();
        #endif      
    }
}
void ProtocolTaskSleepTimeUpdate(unsigned int watchDogSleepingTime){
    if(protVarMeasurementTimer < watchDogSleepingTime){
        protVarMeasurementTimer = 0;
    }
    else{
        protVarMeasurementTimer -= watchDogSleepingTime;
    }
    if(protVarCommunicationTimer < watchDogSleepingTime){
        protVarCommunicationTimer = 0;
    }
    else{
        protVarCommunicationTimer -= watchDogSleepingTime;
    }
    sysVarGpsDayTime += watchDogSleepingTime;
    if(sysVarGpsDayTime > SECONDS_IN_A_DAY){
        sysVarGpsDayTime -= SECONDS_IN_A_DAY;
    }
}
taskResult_t ProtocolTaskExecute(taskResult_t (*taskPointer)(void),unsigned int taskTimeOut){
    taskResult_t taskResult;
    unsigned int timeOut;
    
    timeOut = taskTimeOut;
    while(timeOut){
        taskResult = taskPointer();
        if(taskResult != TASK_ON_COURSE){
            return(taskResult);
        }
    }
    return(TASK_TO_ERR);
}
void ProtocolTaskPowerDown(void){
    PROT_STATE = MAIN_TASK_PARKED;
#ifdef GPS_TASK_ENABLED
    gpsTaskPowerDown();
#endif 
#ifdef SIGFOX_TASK_ENABLED
    SigfoxTaskPowerDown();
#endif
    powerTaskPowerDown();
}
taskResult_t ProtocolTask(void){
    switch(PROT_STATE){
        case MAIN_TASK_PARKED:
            if(ProtocolSetNextMeasTime()){
                protVarMeasurementTimer = 0;
                PROT_STATE = POWERING_TASK;
                return(TASK_ON_COURSE);
            }
            if(ProtocolSetNextCommTime()){
                protVarCommunicationTimer = 0;
                PROT_STATE = POWERING_TASK;
                return(TASK_ON_COURSE);
            }
            return(TASK_NO_SENSOR_FOUND);
        case POWERING_TASK:       
            if(ProtocolSetNextMeasTime()){
                powerTaskNewTask(PWR_CMD_INITIALIZE);
                powerTaskNewTask(PWR_CMD_VDD_EN);
                ProtocolTaskExecute(powerTask,POWER_TASK_SESSION_TO);
                PROT_STATE = MEASURING_TASK;
                return(TASK_ON_COURSE);
            }
            if(ProtocolSetNextCommTime()){
                powerTaskNewTask(PWR_CMD_INITIALIZE);
#ifdef HI_CURRENT_COMM_TASK_ENABLED
                powerTaskNewTask(PWR_CMD_VDD_DIS);
                powerTaskNewTask(PWR_CMD_VCC_EN);
#else   
                powerTaskNewTask(PWR_CMD_VDD_EN);
#endif
                ProtocolTaskExecute(powerTask,POWER_TASK_SESSION_TO);
                ProtocolTaskBuildFrame(TIMER1_EV);
                PROT_STATE = COMMUNICATING_TASK;                 
                return(TASK_ON_COURSE);
            }
            ProtocolTaskPowerDown();
            return(TASK_NO_SENSOR_FOUND);     
            
        case MEASURING_TASK:         
#ifdef GPS_TASK_ENABLED
            gpsTaskNewTask(GPS_CMD_INITIALIZE);
            gpsTaskNewTask(GPS_CMD_ACQUIRE);
            gpsTaskNewTask(GPS_CMD_PWR_DOWN);               
            ProtocolTaskExecute(gpsTask,GPS_TASK_MEAUSUREMENT_SESSION_TO);
            ProtocolSetNextMeasTime();
#endif
            break;       
        case COMMUNICATING_TASK:
#ifdef SIGFOX_TASK_ENABLED
            SigfoxTaskNewTask(SFX_CMD_INITIALIZE);
            SigfoxTaskNewTask(SFX_CMD_SEND_MSG);
//            SigfoxTaskNewTask(SFX_CMD_SEND_STATUS);
            SigfoxTaskNewTask(SFX_CMD_PWR_DOWN);
            ProtocolTaskExecute(SigfoxTask,SFX_TASK_COMM_SESSION_TO);
#endif
            break;
        default:
            ProtocolTaskPowerDown();
            return(TASK_NO_SENSOR_FOUND);
    }
    PROT_STATE = POWERING_TASK;
    return(TASK_ON_COURSE);
}
// EOF




