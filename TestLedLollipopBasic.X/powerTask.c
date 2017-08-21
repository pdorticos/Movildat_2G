#include "powerTask.h"
#include "mcc_generated_files/mcc.h"

#include "sit.h"

#define PWR_SETTLING_TIME  2

#ifdef _PWR_VCAP_CNTRL_BY_TIME
#ifdef HI_CURRENT_COMM_TASK_ENABLED
#define PWR_VCAP_MAX_RISE_TIME 30
#else
#define PWR_VCAP_MAX_RISE_TIME 4
#endif
#endif

#ifdef PWR_TASK_ENABLED
static taskStates_t PWR_STATE = TASK_PARKED;
unsigned int powerTaskPwrReadyTimer;
bool pwrScheduledInitTaskFlag = false;
bool pwrScheduledMeasPowerTaskFlag = false;
bool pwrScheduledCommPowerTaskFlag = false;

void powerTaskPowerDown(void){
    PMD2bits.ADCMD = 1;
    pwrScheduledInitTaskFlag = false;
    pwrScheduledMeasPowerTaskFlag = false;
    pwrScheduledCommPowerTaskFlag = false;
    VDDEN_SetLow();
    VCCEN_SetLow();
    VCAPEN_SetLow();
    PWR_STATE = TASK_PARKED;
}
void powerTaskReset(void){
    VDDEN_SetLow();
    VCCEN_SetLow();
    VCAPEN_SetLow();
}
void powerTaskTick(void){
    if(powerTaskPwrReadyTimer){
        powerTaskPwrReadyTimer--;
    }
}
taskResult_t powerTask(void){
    switch(PWR_STATE){
        case TASK_PARKED:
            if(pwrScheduledInitTaskFlag){
                PWR_STATE = TASK_RESET_TASK;
                return(TASK_ON_COURSE);
            }
            return(TASK_NO_SENSOR_FOUND);
        case TASK_RESET_TASK:
            powerTaskReset();
            VCAPEN_SetHigh();
#ifdef _PWR_VCAP_CNTRL_BY_TIME
            powerTaskPwrReadyTimer = PWR_VCAP_MAX_RISE_TIME;
#else
            // TODO activate comparator/ADC module
#endif
            PWR_STATE = TASK_INIT;
            return(TASK_ON_COURSE);
        case TASK_INIT:
#ifdef _PWR_VCAP_CNTRL_BY_TIME
            if(powerTaskPwrReadyTimer){
                return(TASK_ON_COURSE);
            }
#else
            // TODO consult comparator/adc module
#endif
            pwrScheduledInitTaskFlag = false;
            PWR_STATE = TASK_READY;
            if(pwrScheduledMeasPowerTaskFlag||pwrScheduledCommPowerTaskFlag){
                return(TASK_ON_COURSE);
            }
            return(TASK_DATA_READY);
        case TASK_READY:
            if(pwrScheduledMeasPowerTaskFlag){
                VDDEN_SetHigh();
            }
            if(pwrScheduledCommPowerTaskFlag){
                VCCEN_SetHigh();
            }
            if(pwrScheduledMeasPowerTaskFlag||pwrScheduledCommPowerTaskFlag){
                PWR_STATE = TASK_WAITING_DATA;
                powerTaskPwrReadyTimer = PWR_SETTLING_TIME;
                return(TASK_ON_COURSE);
            }
            return(TASK_DATA_READY);
        case TASK_WAITING_DATA:
            if(powerTaskPwrReadyTimer == 0){
                PWR_STATE = TASK_READY;
                if(pwrScheduledMeasPowerTaskFlag){
                    pwrScheduledMeasPowerTaskFlag = false;
                }
                else if(pwrScheduledCommPowerTaskFlag){
                    pwrScheduledCommPowerTaskFlag = false;
                }
            }
            return(TASK_ON_COURSE);
        default:
            powerTaskPowerDown();
            return(TASK_ON_COURSE);
    }
}
void powerTaskNewTask(POWER_TASK_COMMANDS_T command){
    switch(command){
    case PWR_CMD_INITIALIZE:
        pwrScheduledInitTaskFlag = true;
        break;
    case PWR_CMD_VDD_EN:
        pwrScheduledMeasPowerTaskFlag = true;
        break;
    case PWR_CMD_VDD_DIS:
        pwrScheduledMeasPowerTaskFlag = false;
        VDDEN_SetLow();
        break;
    case PWR_CMD_VCC_EN:
#ifdef HI_CURRENT_COMM_TASK_ENABLED
        pwrScheduledCommPowerTaskFlag = true;
#else
        pwrScheduledMeasPowerTaskFlag = true;
#endif        
        break;
    case PWR_CMD_VCC_DIS:
#ifdef HI_CURRENT_COMM_TASK_ENABLED
        pwrScheduledCommPowerTaskFlag = true;
        VCCEN_SetLow();
#else
        pwrScheduledMeasPowerTaskFlag = false;
        VDDEN_SetLow();
#endif
        break;
    case PWR_CMD_PWR_DOWN:
        powerTaskPowerDown();
        break;
    }
}
dataQualifier_t powerTaskGetVarValidity(unsigned char variableID){
    if(variableID == SITVAR_0BH_BATTERY){
        return(SENSOR_READ);
    }
    return(UNKNOWN_SENSOR);
}
#endif // #ifdef PWR_TASK_ENABLED
// EOF powerTask.c
