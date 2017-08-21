/******************************************************************************
   @File Name
    eusart2.c
  @ Designer
    Pedro Dorticos
  @Company
    MOVILDAT SL
  @Summary
******************************************************************************/

//  Section: Included Files
#include "eusart2.h"
#include <string.h>
#include "pin_manager.h"
#include "sfxProtocol.h"

//  Section: Macro Declarations
#if 1
#define SFX_COMMAND_LIST_END        0xFF
#define SFX_TX_BUFFER_SIZE          35
#define SFX_RX_BUFFER_SIZE          22
#define SFX_RX_PAYLOAD_BUFFER_SIZE  17
#define SFX_TX_PAYLOAD_BUFFER_SIZE  25

#define SIGFOX_LONG_TO_COMMAND      30
#define SIGFOX_TO_COMMAND           5
#define SFX_POWERUP_TIME            2
#define SFX_INTERMSG_TIME           2
#endif // Application Constants
//  Section: Local types definition
// Section: Global Variables
#ifdef SIGFOX_TASK_ENABLED
#if 1
// Sigfox task variables
static taskStates_t SFX_STATE = TASK_RESET_TASK;
static TaskCommManagerState_t SigfoxCommManagerState = TASK_SEND_COMMAND_SM;
static taskResult_t SigfoxTransactionResult = TASK_ON_COURSE;
bool SigfoxMsgReceivedFlag;

static bool SigfoxScheduledMsgTaskFlag = false;
static bool SigfoxScheduledMsgAckTaskFlag = false;
static bool SigfoxScheduledPwrDownTaskFlag = false;
static bool SigfoxScheduledInitTaskFlag = false;
static bool SigfoxScheduledMsgStaTaskFlag = false;

uint8_t SigfoxPayloadBuffer[SFX_TX_PAYLOAD_BUFFER_SIZE];

// Communication with the Modem buffers

uint8_t SigfoxTransactionAttemps;

uint8_t SigfoxTXBuffer[SFX_TX_BUFFER_SIZE];
uint8_t SigfoxRXBuffer[SFX_RX_BUFFER_SIZE];

uint8_t SigfoxTXBufferPtr;
uint8_t SigfoxRXBufferPtr;
uint8_t SigfoxTimeOutTimer;
uint8_t SigfoxTxDelay;
#endif //Global Variables

void SigfoxReceive(void){
#ifndef UART2_SIMULATE
    RCSTA2bits.CREN = 0;
    
    SigfoxRXBufferPtr = 0;
    SigfoxRXBuffer[0] = 0;
    SigfoxMsgReceivedFlag = false;
    // enable receive interrupt
    RCSTA2bits.CREN = 1;
    PIE3bits.RC2IE = 1;
#endif
}
#ifdef UART2_SIMULATE
void EUSART2_Initialize(void){
}
#else
void EUSART2_Initialize(void){
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
    SPBRG2 = 0x33;

    // Baud Rate = 19200; 
    SPBRGH2 = 0x00;   
}
void EUSART2_Transmit_ISR(void){
    if(SigfoxTXBuffer[SigfoxTXBufferPtr] == 0)
    {
        PIE3bits.TX2IE = 0;
        SigfoxReceive();
    }
    else
    {
        TXREG2 = SigfoxTXBuffer[SigfoxTXBufferPtr];
        SigfoxTXBufferPtr++;
    }
    PIR3bits.TX2IF = 0;
    
//    TXREG2 = SigfoxTXBuffer[SigfoxTXBufferPtr];
//    PIR3bits.TX2IF = 0;
//    // add your EUSART1 interrupt custom code
//    if(SigfoxTXBuffer[SigfoxTXBufferPtr] == 0)
//    {
//        PIE3bits.TX2IE = 0;
//        SigfoxReceive();       
//    }
}
void EUSART2_Receive_ISR(void){
    uint8_t auxVar;

    auxVar = RCREG2;
    PIR3bits.RC2IF = 0;
    if(1 == RCSTA2bits.OERR)
    {
        RCSTA2bits.CREN = 0;
        RCSTA2bits.CREN = 1;
        SigfoxRXBufferPtr = 0;
        auxVar = 0;
    }
    if(SigfoxRXBufferPtr >= (SFX_RX_BUFFER_SIZE-1))
    {
        SigfoxRXBufferPtr = 0;
    }

    if((auxVar == 0x0d)&&(SigfoxRXBufferPtr > 0)){
        SigfoxMsgReceivedFlag = true;
        PIE3bits.RC2IE = 0;
    }
    SigfoxRXBuffer[SigfoxRXBufferPtr++] = auxVar;
    SigfoxRXBuffer[SigfoxRXBufferPtr] = 0;
}
#endif  // UART2_SIMULATE
//////////////////////////////////////////////////////////////////
// Sigfox commands
//////////////////////////////////////////////////////////////////
// Command Content
#define SFX_COMMAND_MODE_STR "+++"
#define SFX_SEND_SFXMSG_STR "AT$SF="
#define SFX_SEND_SFXMSG_ACK_STR "AT$SF="
#define SFX_SEND_STATUS_STR "AT$SB=1"
#define SFX_SFX_MODE_STR "ATX"
#define SFX_PWR_ADJ_STR "ATS202=7"

const char SigfoxCommandsDefinitions[][10] = {
    SFX_COMMAND_MODE_STR,
    SFX_SEND_SFXMSG_STR,
    SFX_SEND_SFXMSG_ACK_STR,
    SFX_SEND_STATUS_STR,
    SFX_SFX_MODE_STR,
    SFX_PWR_ADJ_STR
};

// Command APIs
// TODO
bool SigfoxResponseParser(void){
    char * dumyPtr;
    dumyPtr = strstr(SigfoxRXBuffer,"OK");
//    return(!dumyPtr);
     return(true);
}
void SigfoxCommandSelector(SFX_TASK_COMMANDS_T SigfoxCommand){

    const char *SigfoxRomCommandStrPtr;
    const char SFX_COMMAND_TAIL_STR[] = {0x0d, 0};
    const char SFX_COMMAND_TAIL_ACK_STR[] = {',','1',0x0d, 0};
    
    SigfoxTimeOutTimer = SIGFOX_TO_COMMAND;
    SigfoxRomCommandStrPtr = SigfoxCommandsDefinitions[SigfoxCommand];
    switch(SigfoxCommand){
        case SFX_CMD_INITIALIZE:
            strcpy(SigfoxTXBuffer,SigfoxRomCommandStrPtr);
            return;
        case SFX_CMD_SET_SFX_MODE:
        case SFX_CMD_SEND_STATUS:
        case SFX_CMD_ADJ_PWR:
            strcpy(SigfoxTXBuffer,SigfoxRomCommandStrPtr);
            break;        
        case SFX_CMD_SEND_MSG:
        case SFX_CMD_SEND_MSG_WAIT_ACK:
            SigfoxTimeOutTimer = SIGFOX_LONG_TO_COMMAND;
            strcpy(SigfoxTXBuffer,SigfoxRomCommandStrPtr);
            strcat(SigfoxTXBuffer,SigfoxPayloadBuffer);         
            break;
    }
    if(SigfoxCommand == SFX_CMD_SEND_MSG_WAIT_ACK){
        strcat(SigfoxTXBuffer,SFX_COMMAND_TAIL_ACK_STR);
    }
    else
    {
        strcat(SigfoxTXBuffer,SFX_COMMAND_TAIL_STR);
    }
    
}

//////////////////////////////////////////////////////////////////
// Low level routines
//////////////////////////////////////////////////////////////////

void SigfoxTransmit(void){
#ifndef UART2_SIMULATE
    SigfoxTXBufferPtr = 0;
    PIE3bits.TX2IE = 1;
#endif
}
// TODO
bool SigfoxTXReady(void){
    if(!SigfoxTxDelay){
//        return((!PIE3bits.TX2IE) && (MOSI_RTS_GetValue() == 0));
        return(!PIE3bits.TX2IE);
    }
    return(false);
}

//////////////////////////////////////////////////////////////////
// Low level Communication Protocol functions
//////////////////////////////////////////////////////////////////
taskResult_t SigfoxVerifyTransactionStatus(void){
    
    SigfoxTxDelay = SFX_INTERMSG_TIME;
    SigfoxCommManagerState = TASK_SEND_COMMAND_SM;
    if(SigfoxMsgReceivedFlag){
        SigfoxTransactionResult = TASK_DATA_READY;
        SigfoxMsgReceivedFlag = false;        
        return(TASK_DATA_READY);
    }
    if(SigfoxTransactionAttemps){
        SigfoxTransactionAttemps--;
    }
    if(SigfoxTransactionAttemps == 0){
        SFX_STATE = TASK_RESET_TASK;
        SigfoxTransactionResult = TASK_NO_SENSOR_FOUND;
        return(TASK_NO_SENSOR_FOUND);
    }
    SigfoxTransactionResult = TASK_ON_COURSE;
    return(TASK_ON_COURSE);
}
taskResult_t SigfoxCommManager(SFX_TASK_COMMANDS_T SigfoxCommand){
    
    SigfoxTransactionResult = TASK_ON_COURSE;
    switch(SigfoxCommManagerState){
    case TASK_SEND_COMMAND_SM:
        if(SigfoxTXReady()){
            SigfoxCommandSelector(SigfoxCommand);
            SigfoxCommManagerState = TASK_DETECT_TRANSACTION_END_SM;
            SigfoxTransmit();
        }
        break;      
    case TASK_DETECT_TRANSACTION_END_SM:     
        if(SigfoxTimeOutTimer == 0){
            PIE3bits.TX2IE = 0;
            PIE3bits.RC2IE = 0;
            return(SigfoxVerifyTransactionStatus());
        }
        if(SigfoxMsgReceivedFlag){
            SigfoxTimeOutTimer = 0;
            return(SigfoxVerifyTransactionStatus());
        }
        break;
    default:
        SigfoxCommManagerState = TASK_SEND_COMMAND_SM;
        SigfoxTxDelay = SFX_INTERMSG_TIME;
        break;
    }
    return(TASK_ON_COURSE);
}

//////////////////////////////////////////////////////////////////
// Public Task Functions
//////////////////////////////////////////////////////////////////
void SigfoxTaskTick(void){
    if(SigfoxTimeOutTimer){
        SigfoxTimeOutTimer--;
    }
    if(SigfoxTxDelay){
        SigfoxTxDelay--;
    }
}
void SigfoxTaskReset(void){
    EUSART2_Initialize();
    SFX_STATE = TASK_RESET_TASK;
    SigfoxTxDelay = SFX_POWERUP_TIME;
    SigfoxCommManagerState = TASK_SEND_COMMAND_SM;
    SigfoxTransactionResult = TASK_ON_COURSE;
    SigfoxTransactionAttemps = 3;
    SigfoxTimeOutTimer = 0;
    
    SigfoxMsgReceivedFlag = false;
    MODPWREN_SetHigh();
    MODCS_SetLow();
    //MODRST_SetHigh();
    MISO_CTS_SetLow();
}
void SigfoxTaskPowerDown(void){
    SigfoxScheduledInitTaskFlag = false;
    SigfoxScheduledPwrDownTaskFlag = false;
    PMD0bits.UART2MD = 1;
    MISO_CTS_SetHigh();
    MODPWREN_SetLow();
    MODCS_SetHigh();
    SFX_STATE = TASK_PARKED;
}
taskResult_t SigfoxTask(void){
    const uint8_t sigfoxInitCommandsROMList[3] = {SFX_CMD_INITIALIZE,
                                                //SFX_CMD_SET_SFX_MODE,
                                                SFX_CMD_ADJ_PWR,
                                                SFX_COMMAND_LIST_END};
    const char *SigfoxInitCommandPtr;
    
    switch(SFX_STATE){
        case TASK_PARKED:
            if(SigfoxScheduledInitTaskFlag){
                SFX_STATE = TASK_RESET_TASK;
                return(TASK_ON_COURSE);
            }
            
            return(TASK_NO_SENSOR_FOUND);
            
        case TASK_RESET_TASK:
            SigfoxTaskReset();
            SFX_STATE = TASK_INIT;
            SigfoxInitCommandPtr = sigfoxInitCommandsROMList;
            return(TASK_ON_COURSE);
            
        case TASK_INIT:
            while(SigfoxInitCommandPtr[0] != SFX_COMMAND_LIST_END)
            {
                SigfoxTransactionResult = SigfoxCommManager(SigfoxInitCommandPtr[0]);
                if(SigfoxTransactionResult == TASK_DATA_READY){
                    if(SigfoxResponseParser()){
                        SigfoxInitCommandPtr++;
                        SigfoxTransactionAttemps = 3;
                    }
                    else if(SigfoxTransactionAttemps){
                        SigfoxTransactionAttemps--;
                    }
                    continue;
                }
                if(SigfoxTransactionResult == TASK_NO_SENSOR_FOUND){
                    SigfoxTaskPowerDown();
                }
                return(SigfoxTransactionResult);
            }
            SigfoxTransactionResult = TASK_DATA_READY;
            SFX_STATE = TASK_READY;
            SigfoxScheduledInitTaskFlag = false;
            if(SigfoxScheduledMsgTaskFlag || SigfoxScheduledMsgAckTaskFlag || SigfoxScheduledMsgStaTaskFlag){
                return(TASK_ON_COURSE);
            }
            return(TASK_DATA_READY);
            
        case TASK_READY:
    
//            if(SigfoxScheduledInitTaskFlag){
//                SFX_STATE = TASK_RESET_TASK;
//                return(TASK_ON_COURSE);
//            }
            if(SigfoxScheduledMsgTaskFlag || SigfoxScheduledMsgAckTaskFlag || SigfoxScheduledMsgStaTaskFlag){
                SigfoxTransactionAttemps = 3;
                SFX_STATE = TASK_WAITING_DATA;                
                SigfoxCommManagerState = TASK_SEND_COMMAND_SM;
                return(TASK_ON_COURSE);
            }
            if(SigfoxScheduledPwrDownTaskFlag){
                SigfoxTaskPowerDown();
                return(TASK_NO_SENSOR_FOUND);
            }
            return(TASK_DATA_READY);
            
        case TASK_WAITING_DATA:
            if(SigfoxScheduledMsgAckTaskFlag){
                SigfoxTransactionResult = SigfoxCommManager(SFX_CMD_SEND_MSG_WAIT_ACK);                
            }
            else if(SigfoxScheduledMsgStaTaskFlag){
                 SigfoxTransactionResult = SigfoxCommManager(SFX_CMD_SEND_STATUS);
            }
            else
            {
                SigfoxTransactionResult = SigfoxCommManager(SFX_CMD_SEND_MSG);
            }
            if(SigfoxTransactionResult == TASK_DATA_READY){
                if(SigfoxResponseParser()){                   
                    SigfoxScheduledMsgTaskFlag = false;
                    SigfoxScheduledMsgAckTaskFlag = false;
                    SigfoxScheduledMsgStaTaskFlag = false;
                    
                    SFX_STATE = TASK_READY;
                    return(TASK_ON_COURSE);
                }
                else if(SigfoxTransactionAttemps){
                    SigfoxTransactionAttemps--;        
                }    
            }
            if(SigfoxTransactionResult == TASK_NO_SENSOR_FOUND){
//                SigfoxTaskPowerDown();
//                return(TASK_NO_SENSOR_FOUND);
                SigfoxScheduledMsgStaTaskFlag = false;
                SigfoxScheduledMsgTaskFlag = false;
                SigfoxScheduledMsgAckTaskFlag = false;
                SFX_STATE = TASK_READY;
                return(TASK_DATA_NOT_AVAILABLE);
            }
            return(TASK_ON_COURSE);
        default:
            SFX_STATE = TASK_RESET_TASK;    
            return(TASK_ON_COURSE);
    }
}
void SigfoxTaskLoadPayloadBuffer(uint8_t *rawData){
    uint8_t payloadBufferPtr, buffer;
    
    for(payloadBufferPtr = 0; payloadBufferPtr < 12; payloadBufferPtr++){
        buffer = (rawData[payloadBufferPtr] >> 4) & 0x0F;
        SigfoxPayloadBuffer[2 * payloadBufferPtr] = (buffer < 10)? (buffer + '0'): (buffer - 10 + 'A');
        buffer = rawData[payloadBufferPtr] & 0x0F;
        SigfoxPayloadBuffer[2 * payloadBufferPtr + 1] = (buffer < 10)? (buffer + '0'): (buffer - 10 + 'A');    
    }
    SigfoxPayloadBuffer[24] = 0;
}
void SigfoxTaskNewTask(SFX_TASK_COMMANDS_T command){
    switch(command){
        case SFX_CMD_INITIALIZE:
            SigfoxScheduledInitTaskFlag = true;
            break;
            
        case SFX_CMD_SEND_MSG:       
            SigfoxScheduledMsgAckTaskFlag = false;
            SigfoxScheduledMsgTaskFlag = true;
            SigfoxScheduledMsgStaTaskFlag = false;
            break;
            
            
        case SFX_CMD_SEND_MSG_WAIT_ACK:
            SigfoxScheduledMsgTaskFlag = false;
            SigfoxScheduledMsgAckTaskFlag = true;
            SigfoxScheduledMsgStaTaskFlag = false;
            break;
            
        case SFX_CMD_SEND_STATUS:
            SigfoxScheduledMsgTaskFlag = false;
            SigfoxScheduledMsgAckTaskFlag = false;
            SigfoxScheduledMsgStaTaskFlag = true;
            break;
            
        case SFX_CMD_PWR_DOWN:
            SigfoxScheduledPwrDownTaskFlag = true;
            break;
    }
}
#endif // #ifdef SIGFOX_TASK_ENABLED
/**
  End of File
*/
