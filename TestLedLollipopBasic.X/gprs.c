#include "mcc_generated_files/mcc.h"
#include <string.h>
#include "sit.h"

//  Section: Macro Declarations
#if 1
#define GPRS_COMMAND_LIST_END           0xFF
#define GPRS_TX_BUFFER_SIZE             100
#define GPRS_RX_BUFFER_SIZE             100

#define GPRS_TO_COMMAND                 8
#define GPRS_POWERUP_TIME               5
#define GPRS_INTERMSG_TIME              2
#endif // Application Constants

////////////////////////////////////////////////////////////////////////////
//  Variables
////////////////////////////////////////////////////////////////////////////

/*VERIFICAR*/
static taskResult_t gprsTransactionResult = TASK_ON_COURSE;
static TaskCommManagerState_t gprsCommManagerState = TASK_SEND_COMMAND_SM;

static taskStates_t gprsTaskState = TASK_RESET_TASK;

char gprsTxBuffer[GPRS_TX_BUFFER_SIZE];
uint8_t gprsTXBufferPtr;
char gprsRxBuffer[GPRS_RX_BUFFER_SIZE];
uint8_t gprsRXBufferPtr;

static TaskCommManagerState_t GPRS_CommManagerState = TASK_SEND_COMMAND_SM;
bool gprsMsgReceivedFlag;
static taskResult_t GPRS_TransactionResult = TASK_ON_COURSE;
uint8_t gprsTransactionAttemps=3;
        
bool gprsScheduledInitTaskFlag;
bool gprsScheduledPwrDownTaskFlag;
uint8_t gprsTimeOutTimer;
uint8_t gprsTxDelay;
////////////////////////////////////////////////////////////////////////////

/*
 VERIFICAR
 */

bool gprsScheduledMsgTaskFlag = false, 
     gprsScheduledMsgStaTaskFlag=false,
     gprsScheduledMsgAckTaskFlag;

void EUSART2_GPRS_Initialize(void){
    PMD0bits.UART2MD = 0;
    // disable interrupts before changing states
    PIE3bits.RC2IE = 0;
    PIE3bits.TX2IE = 0;

    // Set the EUSART2 module to the options selected in the user interface.

    // ABDOVF no_overflow; CKTXP async_noninverted_sync_fallingedge; BRG16 16bit_generator; WUE disabled; ABDEN disabled; DTRXP not_inverted; 
    BAUDCON2 = 0x08;
    // SPEN enabled; RX9 8-bit; CREN enabled; ADDEN disabled; SREN disabled; 
    RCSTA2 = 0x90;
    // TX9 8-bit; TX9D 0; SENDB sync_break_complete; TXEN enabled; SYNC asynchronous; BRGH hi_speed; CSRC slave_mode; 
    TXSTA2 = 0x24;

    // Baud Rate = 19200; 
    SPBRG2 = 0x67;

    // Baud Rate = 19200; 
    SPBRGH2 = 0x00;   
}
void EUSART2_GPRS_Transmit_ISR(void){
    
}
void EUSART2_GPRS_Receive_ISR(void){
 
}
void gprsTaskTick(void){
    if(gprsTimeOutTimer){
        gprsTimeOutTimer--;
    }
    if(gprsTxDelay){
        gprsTxDelay--;
    }
}
void gprsTransmit(void){
#ifndef UART2_SIMULATE
    gprsTXBufferPtr = 0;
    PIE3bits.TX2IE = 1;
#endif
}
//////////////////////////////////////////////////////////////////
// Low level Communication Protocol functions
//////////////////////////////////////////////////////////////////
taskResult_t GPRS_VerifyTransactionStatus(void){
    gprsTxDelay = GPRS_INTERMSG_TIME;
    GPRS_CommManagerState = TASK_SEND_COMMAND_SM;
    if(gprsMsgReceivedFlag){
        GPRS_TransactionResult = TASK_DATA_READY;
        gprsMsgReceivedFlag = false;        
        return(TASK_DATA_READY);
    }
    if(gprsTransactionAttemps){
        gprsTransactionAttemps--;
    }
    if(gprsTransactionAttemps == 0){
        gprsTaskState = TASK_RESET_TASK;
        GPRS_TransactionResult = TASK_NO_SENSOR_FOUND;
        return(TASK_NO_SENSOR_FOUND);
    }
    GPRS_TransactionResult = TASK_ON_COURSE;
    return(TASK_ON_COURSE);
}
//IMPLEMENTAR
void gprsCommandSelector(SFX_TASK_COMMANDS_T command){
    
}



// TODO
bool gprsTXReady(void){
    if(!gprsTxDelay){
//        return((!PIE3bits.TX2IE) && (MOSI_RTS_GetValue() == 0));
        return(!PIE3bits.TX2IE);
    }
    return(false);
}

bool gprsResponseParser(void){
    char * dumyPtr;
    dumyPtr = strstr(gprsRxBuffer,"OK");
//    return(!dumyPtr);
     return(true);
}

taskResult_t gprsVerifyTransactionStatus(void){
    
    gprsTxDelay = GPRS_INTERMSG_TIME;
    gprsCommManagerState = TASK_SEND_COMMAND_SM;
    if(gprsMsgReceivedFlag){
        gprsTransactionResult = TASK_DATA_READY;
        gprsMsgReceivedFlag = false;        
        return(TASK_DATA_READY);
    }
    if(gprsTransactionAttemps){
        gprsTransactionAttemps--;
    }
    if(gprsTransactionAttemps == 0){
        gprsTaskState = TASK_RESET_TASK;
        gprsTransactionResult = TASK_NO_SENSOR_FOUND;
        return(TASK_NO_SENSOR_FOUND);
    }
    gprsTransactionResult = TASK_ON_COURSE;
    return(TASK_ON_COURSE);
}

taskResult_t GPRS_CommManager(SFX_TASK_COMMANDS_T GPRS_Command){
    
    GPRS_TransactionResult = TASK_ON_COURSE;
    switch(GPRS_CommManagerState){
    case TASK_SEND_COMMAND_SM:
        if(gprsTXReady()){
            gprsCommandSelector(GPRS_Command);
            GPRS_CommManagerState = TASK_DETECT_TRANSACTION_END_SM;
            gprsTransmit();
        }
        break;      
    case TASK_DETECT_TRANSACTION_END_SM:     
        if(gprsTimeOutTimer == 0){
            PIE3bits.TX2IE = 0;
            PIE3bits.RC2IE = 0;
            return(GPRS_VerifyTransactionStatus());
        }
        if(gprsMsgReceivedFlag){
            gprsTimeOutTimer = 0;
            return(GPRS_VerifyTransactionStatus());
        }
        break;
    default:
        GPRS_CommManagerState = TASK_SEND_COMMAND_SM;
        gprsTxDelay = GPRS_INTERMSG_TIME;
        break;
    }
    return(TASK_ON_COURSE);
}
void gprsTaskReset(void){
    
}
taskResult_t gprsCommManager(SFX_TASK_COMMANDS_T gprsCommand){
    
    gprsTransactionResult = TASK_ON_COURSE;
    switch(gprsCommManagerState){
    case TASK_SEND_COMMAND_SM:
        if(gprsTXReady()){
            gprsCommandSelector(gprsCommand);
            gprsCommManagerState = TASK_DETECT_TRANSACTION_END_SM;
            gprsTransmit();
        }
        break;      
    case TASK_DETECT_TRANSACTION_END_SM:     
        if(gprsTimeOutTimer == 0){
            PIE3bits.TX2IE = 0;
            PIE3bits.RC2IE = 0;
            return(gprsVerifyTransactionStatus());
        }
        if(gprsMsgReceivedFlag){
            gprsTimeOutTimer = 0;
            return(gprsVerifyTransactionStatus());
        }
        break;
    default:
        gprsCommManagerState = TASK_SEND_COMMAND_SM;
        gprsTxDelay = GPRS_INTERMSG_TIME;
        break;
    }
    return(TASK_ON_COURSE);
}




taskResult_t gprsTask(void){
    const uint8_t gprsInitCommandsROMList[3] = {GPRS_COMMAND_LIST_END};
    const char *gprsInitCommandPtr;
    
    switch(gprsTaskState){
        case TASK_PARKED:
            if(gprsScheduledInitTaskFlag){
                gprsTaskState = TASK_RESET_TASK;
                return(TASK_ON_COURSE);
            }           
            return(TASK_NO_SENSOR_FOUND);
            
        case TASK_RESET_TASK:
            gprsTaskReset();
            gprsTaskState = TASK_INIT;
            gprsInitCommandPtr = gprsInitCommandsROMList;
            return(TASK_ON_COURSE);
            
        case TASK_INIT:
            while(gprsInitCommandPtr[0] != GPRS_COMMAND_LIST_END)
            {
                GPRS_TransactionResult = GPRS_CommManager(gprsInitCommandPtr[0]);
                if(GPRS_TransactionResult == TASK_DATA_READY){
                    if(gprsResponseParser()){
                        gprsInitCommandPtr++;
                        gprsTransactionAttemps=3;
                    }
                    else if(gprsTransactionAttemps){
                        gprsTransactionAttemps--;
                    }
                    continue;
                }
                if(GPRS_TransactionResult == TASK_NO_SENSOR_FOUND){
                    SigfoxTaskPowerDown();
                }
                return(GPRS_TransactionResult);
            }
            GPRS_TransactionResult = TASK_DATA_READY;
            gprsTaskState = TASK_READY;
            
            gprsScheduledInitTaskFlag = false;
            //FUNCION O CTE?
            //EXISTEN LAS FUNCIONES DE CLASE?
            if(gprsScheduledMsgTaskFlag || gprsScheduledMsgStaTaskFlag){
                return(TASK_ON_COURSE);
            }
            return(TASK_DATA_READY);
            
        case TASK_READY:
    
//            if(gprsScheduledInitTaskFlag){
//                GPRS_TASK_STATE = TASK_RESET_TASK;
//                return(TASK_ON_COURSE);
//            }
            if(gprsScheduledMsgTaskFlag || gprsScheduledMsgAckTaskFlag || gprsScheduledMsgStaTaskFlag){
                gprsTransactionAttemps = 3;
                gprsTaskState = TASK_WAITING_DATA;                
                GPRS_CommManagerState = TASK_SEND_COMMAND_SM;
                return(TASK_ON_COURSE);
            }
            /*if(){gprsScheduledPwrDownTaskFlag
                SigfoxTaskPowerDown();
                return(TASK_NO_SENSOR_FOUND);
            }*/
            //VERIFICAR
            if(gprsScheduledPwrDownTaskFlag){
                SigfoxTaskPowerDown();
                return(TASK_NO_SENSOR_FOUND);
            }
            return(TASK_DATA_READY);
            
        case TASK_WAITING_DATA:
            if(gprsScheduledMsgAckTaskFlag){
                gprsTransactionResult = gprsCommManager(SFX_CMD_SEND_MSG_WAIT_ACK);                
            }
            else if(gprsScheduledMsgStaTaskFlag){
                 gprsTransactionResult = gprsCommManager(SFX_CMD_SEND_STATUS);
            }
            else
            {
                gprsTransactionResult = gprsCommManager(SFX_CMD_SEND_MSG);
            }
            if(gprsTransactionResult == TASK_DATA_READY){
                if(gprsResponseParser()){                   
                    gprsScheduledMsgTaskFlag = false;
                    gprsScheduledMsgAckTaskFlag = false;
                    gprsScheduledMsgStaTaskFlag = false;
                    
                    gprsTaskState = TASK_READY;
                    return(TASK_ON_COURSE);
                }
                else if(gprsTransactionAttemps){
                    gprsTransactionAttemps--;        
                }    
            }
            if(GPRS_TransactionResult == TASK_NO_SENSOR_FOUND){
//                SigfoxTaskPowerDown();
//                return(TASK_NO_SENSOR_FOUND);
                gprsScheduledMsgStaTaskFlag = false;
                gprsScheduledMsgTaskFlag = false;
                gprsScheduledMsgAckTaskFlag = false;
                gprsTaskState = TASK_READY;
                return(TASK_DATA_NOT_AVAILABLE);
            }
            return(TASK_ON_COURSE);
        default:
            gprsTaskState = TASK_RESET_TASK;    
            return(TASK_ON_COURSE);
    }
}
