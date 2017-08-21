
#include "sfxProtocol.h"
#include "mcc_generated_files/mcc.h"

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

static taskStates_t GPRS_TASK_STATE = TASK_RESET_TASK;

char gprsTxBuffer[GPRS_TX_BUFFER_SIZE];
uint8_t gprsTXBufferPtr;
char gprsRxBuffer[GPRS_RX_BUFFER_SIZE];
uint8_t gprsRXBufferPtr;

static TaskCommManagerState_t GPRS_CommManagerState = TASK_SEND_COMMAND_SM;
bool GPRS_MsgReceivedFlag;
static taskResult_t GPRS_TransactionResult = TASK_ON_COURSE;
uint8_t GPRS_TransactionAttemps;
        
bool gprsScheduledInitTaskFlag;
bool gprsScheduledPwrDownTaskFlag;
uint8_t gprsTimeOutTimer;
uint8_t gprsTxDelay;
////////////////////////////////////////////////////////////////////////////

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
void GPRS_Transmit(void){
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
    if(GPRS_MsgReceivedFlag){
        GPRS_TransactionResult = TASK_DATA_READY;
        GPRS_MsgReceivedFlag = false;        
        return(TASK_DATA_READY);
    }
    if(GPRS_TransactionAttemps){
        GPRS_TransactionAttemps--;
    }
    if(GPRS_TransactionAttemps == 0){
        GPRS_TASK_STATE = TASK_RESET_TASK;
        GPRS_TransactionResult = TASK_NO_SENSOR_FOUND;
        return(TASK_NO_SENSOR_FOUND);
    }
    GPRS_TransactionResult = TASK_ON_COURSE;
    return(TASK_ON_COURSE);
}
taskResult_t GPRS_CommManager(SFX_TASK_COMMANDS_T GPRS_Command){
    
    GPRS_TransactionResult = TASK_ON_COURSE;
    switch(GPRS_CommManagerState){
    case TASK_SEND_COMMAND_SM:
        if(GPRS_TXReady()){
            GPRS_CommandSelector(GPRS_Command);
            GPRS_CommManagerState = TASK_DETECT_TRANSACTION_END_SM;
            GPRS_Transmit();
        }
        break;      
    case TASK_DETECT_TRANSACTION_END_SM:     
        if(gprsTimeOutTimer == 0){
            PIE3bits.TX2IE = 0;
            PIE3bits.RC2IE = 0;
            return(GPRS_VerifyTransactionStatus());
        }
        if(GPRS_MsgReceivedFlag){
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
taskResult_t gprsTask(void){
    const uint8_t gprsInitCommandsROMList[3] = {GPRS_COMMAND_LIST_END};
    const char *gprsInitCommandPtr;
    
    switch(GPRS_TASK_STATE){
        case TASK_PARKED:
            if(gprsScheduledInitTaskFlag){
                GPRS_TASK_STATE = TASK_RESET_TASK;
                return(TASK_ON_COURSE);
            }           
            return(TASK_NO_SENSOR_FOUND);
            
        case TASK_RESET_TASK:
            gprsTaskReset();
            GPRS_TASK_STATE = TASK_INIT;
            gprsInitCommandPtr = gprsInitCommandsROMList;
            return(TASK_ON_COURSE);
            
        case TASK_INIT:
            while(gprsInitCommandPtr[0] != GPRS_COMMAND_LIST_END)
            {
                GPRS_TransactionResult = GPRS_CommManager(gprsInitCommandPtr[0]);
                if(GPRS_TransactionResult == TASK_DATA_READY){
                    if(SigfoxResponseParser()){
                        gprsInitCommandPtr++;
                        SigfoxTransactionAttemps = 3;
                    }
                    else if(SigfoxTransactionAttemps){
                        SigfoxTransactionAttemps--;
                    }
                    continue;
                }
                if(GPRS_TransactionResult == TASK_NO_SENSOR_FOUND){
                    SigfoxTaskPowerDown();
                }
                return(GPRS_TransactionResult);
            }
            GPRS_TransactionResult = TASK_DATA_READY;
            GPRS_TASK_STATE = TASK_READY;
            
            gprsScheduledInitTaskFlag = false;
            if(SigfoxScheduledMsgTaskFlag || SigfoxScheduledMsgStaTaskFlag){
                return(TASK_ON_COURSE);
            }
            return(TASK_DATA_READY);
            
        case TASK_READY:
    
//            if(gprsScheduledInitTaskFlag){
//                GPRS_TASK_STATE = TASK_RESET_TASK;
//                return(TASK_ON_COURSE);
//            }
            if(SigfoxScheduledMsgTaskFlag || SigfoxScheduledMsgAckTaskFlag || SigfoxScheduledMsgStaTaskFlag){
                SigfoxTransactionAttemps = 3;
                GPRS_TASK_STATE = TASK_WAITING_DATA;                
                GPRS_CommManagerState = TASK_SEND_COMMAND_SM;
                return(TASK_ON_COURSE);
            }
            if(){gprsScheduledPwrDownTaskFlag
                SigfoxTaskPowerDown();
                return(TASK_NO_SENSOR_FOUND);
            }
            return(TASK_DATA_READY);
            
        case TASK_WAITING_DATA:
            if(SigfoxScheduledMsgAckTaskFlag){
                GPRS_TransactionResult = SigfoxCommManager(SFX_CMD_SEND_MSG_WAIT_ACK);                
            }
            else if(SigfoxScheduledMsgStaTaskFlag){
                 GPRS_TransactionResult = SigfoxCommManager(SFX_CMD_SEND_STATUS);
            }
            else
            {
                GPRS_TransactionResult = SigfoxCommManager(SFX_CMD_SEND_MSG);
            }
            if(GPRS_TransactionResult == TASK_DATA_READY){
                if(SigfoxResponseParser()){                   
                    SigfoxScheduledMsgTaskFlag = false;
                    SigfoxScheduledMsgAckTaskFlag = false;
                    SigfoxScheduledMsgStaTaskFlag = false;
                    
                    GPRS_TASK_STATE = TASK_READY;
                    return(TASK_ON_COURSE);
                }
                else if(SigfoxTransactionAttemps){
                    SigfoxTransactionAttemps--;        
                }    
            }
            if(GPRS_TransactionResult == TASK_NO_SENSOR_FOUND){
//                SigfoxTaskPowerDown();
//                return(TASK_NO_SENSOR_FOUND);
                SigfoxScheduledMsgStaTaskFlag = false;
                SigfoxScheduledMsgTaskFlag = false;
                SigfoxScheduledMsgAckTaskFlag = false;
                GPRS_TASK_STATE = TASK_READY;
                return(TASK_DATA_NOT_AVAILABLE);
            }
            return(TASK_ON_COURSE);
        default:
            GPRS_TASK_STATE = TASK_RESET_TASK;    
            return(TASK_ON_COURSE);
    }
}
