/******************************************************************************
  File Name:
    main.c
 Company:
    MOVILDAT SL
  Summary:
******************************************************************************/

//  Section: Included Files

#include "mcc_generated_files/mcc.h"
#include <xc.h>
#include "sfxProtocol.h"

//  Section: Macro Declarations

unsigned int sessionTO;
static unsigned char statusIndTimer = TASK_NO_SENSOR_FOUND;
static unsigned char ledTimer = TASK_NO_SENSOR_FOUND;
static  taskResult_t lastKnownResult = TASK_DATA_NOT_AVAILABLE;

void systemMainTimer(void){
    bool secondMultiplier;
    ProtocolTaskTick();
    if(secondMultiplier){
        if(sessionTO){
            sessionTO--;
        }
    }
    secondMultiplier = !secondMultiplier;
    
    if(LED_GetValue() == 0){
        LED_SetHigh();        
        ledTimer = statusIndTimer;
    }
    else{
        ledTimer--;
        if(ledTimer == 0){
            LED_SetLow();
        }
    }
}

void setLedStatus(taskResult_t status){
    if(status != lastKnownResult){
        switch(status){
        case TASK_NO_SENSOR_FOUND:
            statusIndTimer = 15;
            break;
        case TASK_ON_COURSE:
            statusIndTimer = 1;
            break;
        case  TASK_DATA_READY:
            statusIndTimer = 2;
            break;
        case TASK_DATA_NOT_AVAILABLE:
            statusIndTimer = 4;
            break;
        default:
            statusIndTimer = 15;
            break;
        }
        LED_SetHigh();        
        ledTimer = statusIndTimer;
    }
    lastKnownResult = status;
}

void main(void)
{
    taskResult_t taskResult;
    
    SYSTEM_Initialize();
    TMR0_SetInterruptHandler(systemMainTimer);
    
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();  
    MODRST_TRIS = 1;

    ProtocolTaskPowerDown();
    
    while(1){
        setLedStatus(TASK_ON_COURSE);
        sessionTO = POWER_TASK_SESSION_TO + GPS_TASK_MEAUSUREMENT_SESSION_TO + SFX_TASK_COMM_SESSION_TO;  
        do{
            taskResult = ProtocolTask();
            if(!sessionTO){
                break;
            }
        }while(taskResult!=TASK_NO_SENSOR_FOUND);

        if(sysVarGpsValidFlag){
            setLedStatus(TASK_DATA_READY);
            sessionTO = 15;
            while(sessionTO);
        }
        sessionTO = 60;

        setLedStatus(TASK_NO_SENSOR_FOUND);
        while (sessionTO){
            CLRWDT();            
        }
    }
}
/**
 End of File
*/