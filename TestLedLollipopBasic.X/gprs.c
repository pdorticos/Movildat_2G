#include "mcc_generated_files/mcc.h"
#include <string.h>
#include "sit.h"
#include "mcc_generated_files/gprs.h"

//  Section: Macro Declarations
#if 1
#define GPRS_COMMAND_LIST_END           0xFF
#define GPRS_TX_BUFFER_SIZE             250
#define GPRS_RX_BUFFER_SIZE             180
#define GPRS_RX_PAYLOAD_BUFFER_SIZE     150
#define GPRS_TX_PAYLOAD_BUFFER_SIZE     240
#define GPRS_TO_COMMAND                   5
#define GPRS_POWERUP_TIME                 5
#define GPRS_INTERMSG_TIME                2
#endif // Application Constants

////////////////////////////////////////////////////////////////////////////
//  Variables
////////////////////////////////////////////////////////////////////////////

/*VERIFICAR*/
static taskResult_t gprsTransactionResult = TASK_ON_COURSE;
static TaskCommManagerState_t gprsCommManagerState = TASK_SEND_COMMAND_SM;
static taskStates_t gprsTaskState = TASK_RESET_TASK;
uint8_t gprsPayloadBuffer[GPRS_TX_PAYLOAD_BUFFER_SIZE];
char gprsTxBuffer[GPRS_TX_BUFFER_SIZE];
uint8_t gprsTXBufferPtr;
char gprsRxBuffer[GPRS_RX_BUFFER_SIZE];
uint8_t gprsRXBufferPtr;
bool gprsMsgReceivedFlag;

uint8_t gprsTransactionAttemps=3;
        
bool gprsScheduledInitTaskFlag=false;
bool gprsScheduledPwrDownTaskFlag=false;
uint8_t gprsTimeOutTimer;
uint8_t gprsTxDelay;
bool gprsScheduledMsgTaskFlag = false, 
     gprsScheduledMsgStaTaskFlag=false,
     gprsScheduledMsgAckTaskFlag;
////////////////////////////////////////////////////////////////////////////

void gprsReceive(void){
#ifndef UART2_SIMULATE
    RCSTA2bits.CREN = 0;
    gprsRXBufferPtr = 0;
    gprsRxBuffer[0] = 0;
    gprsMsgReceivedFlag = false;
    // enable receive interrupt
    RCSTA2bits.CREN = 1;
    PIE3bits.RC2IE = 1;
#endif
}

/*
 ¿IFDEF?
 * 
 * #ifdef UART2_SIMULATE
 */

/*
 DEFINIR AQUÍ COMANDOS
 */
#define GPRS_COMMAND_ATENTION_STR "AT"
#define GPRS_COMMAND_RESPONSE_NUM_STR "ATV0"
#define GPRS_COMMAND_ECHO_OFF_STR "ATE0"
#define GPRS_COMMAND_CTXT_CONFIG_STR "AT+CGDCONT=1,\"IP\","
#define GPRS_COMMAND_CTXT_ACTIVATION_STR "AT+CGACT=1,1"
#define GPRS_COMMAND_TURN_ON_STR "AT#GPRS=1"
#define GPRS_COMMAND_SAVE_STR "AT&W"
#define GPRS_COMMAND_GET_ICID_STR "AT#CCID"
#define GPRS_COMMAND_GET_IMEI_STR "AT#CGSN"
#define GPRS_COMMAND_SOCKET_DIAL_STR "AT#SD=1,1,"

///command content


const char  gprsCommandsDefinitions[][10] ={
    GPRS_COMMAND_ATENTION_STR,
    GPRS_COMMAND_RESPONSE_NUM_STR,
    GPRS_COMMAND_ECHO_OFF_STR,
    GPRS_COMMAND_CTXT_CONFIG_STR,
    GPRS_COMMAND_CTXT_ACTIVATION_STR,
    GPRS_COMMAND_TURN_ON_STR,
    GPRS_COMMAND_SAVE_STR,
    GPRS_COMMAND_GET_ICID_STR,
    GPRS_COMMAND_GET_IMEI_STR,
    GPRS_COMMAND_SOCKET_DIAL_STR
};

bool gprsResponseParser(void){
    char * dumyPtr;
    dumyPtr = strstr(gprsRxBuffer,"OK");
//    return(!dumyPtr);
     return(true);
}

void gprsCommandSelector(SFX_TASK_COMMANDS_T gprsCommand){
    const char *gprsRomCommandStrPtr;
    const char gprsCOMMAND_TAIL_STR[] = {0x0d, 0};
    const char gprsCOMMAND_TAIL_ACK_STR[] = {',','1',0x0d, 0};
    
    gprsTimeOutTimer = GPRS_TO_COMMAND;
    //gprsRomCommandStrPtr = gprsCommandsDefinitions[SigfoxCommand];
    switch(gprsCommand){
        case GPRS_CMD_INITIALIZE:
        break;
        
        case GPRS_CMD_SEND_MSG:
        break;
        
        
        default:
        break;
    }    
}
/*
 LOW LEVEL
 */

void gprsTransmit(void){
#ifndef UART2_SIMULATE
    gprsTXBufferPtr = 0;
    PIE3bits.TX2IE = 1;
#endif
}
bool gprsTXReady(void){
    if(!gprsTxDelay){
//        return((!PIE3bits.TX2IE) && (MOSI_RTS_GetValue() == 0));
        return(!PIE3bits.TX2IE);
    }
    return(false);
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
/*
 Public tasks
 */

void gprsTaskTick(void){
    if(gprsTimeOutTimer){
        gprsTimeOutTimer--;
    }
    if(gprsTxDelay){
        gprsTxDelay--;
    }
}
void gprsTaskReset(void){
    EUSART2_GPRS_Initialize();
    gprsTaskState=TASK_RESET_TASK;
    gprsTxDelay=GPRS_POWERUP_TIME;
    gprsCommManagerState=TASK_SEND_COMMAND_SM;
    gprsTransactionResult=TASK_ON_COURSE;
    gprsTransactionAttemps=3;
    gprsTimeOutTimer=0;
    
    gprsMsgReceivedFlag=false;
//    MODPWREN_SetHigh();
//    MODCS_SetLow();
    MISO_CTS_SetLow();
}
void gprsTaskPowerDown(void){
    gprsScheduledInitTaskFlag=false;
    gprsScheduledPwrDownTaskFlag=false;
    //PMD0bits.UART2MD = 1;    ????
//    MISO_CTS_SetHigh();
//    MODPWREN_SetLow();
//    MODCS_SetHigh();
    gprsTaskState=TASK_PARKED;
    setSemaphore(NO_OWNER);
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
                gprsTransactionResult = gprsCommManager(gprsInitCommandPtr[0]);
                if(gprsTransactionResult == TASK_DATA_READY){
                    if(gprsResponseParser()){
                        gprsInitCommandPtr++;
                        gprsTransactionAttemps=3;
                    }
                    else if(gprsTransactionAttemps){
                        gprsTransactionAttemps--;
                    }
                    continue;
                }
                if(gprsTransactionResult == TASK_NO_SENSOR_FOUND){
                    gprsTaskPowerDown();
                }
                return(gprsTransactionResult);
            }
            gprsTransactionResult = TASK_DATA_READY;
            gprsTaskState = TASK_READY;
            gprsScheduledInitTaskFlag = false;
                    if(gprsScheduledMsgTaskFlag || gprsScheduledMsgStaTaskFlag){
                return(TASK_ON_COURSE);
            }
            return(TASK_DATA_READY);
        case TASK_READY:
                if(gprsScheduledMsgTaskFlag || gprsScheduledMsgAckTaskFlag || gprsScheduledMsgStaTaskFlag){
                gprsTransactionAttemps = 3;
                gprsTaskState = TASK_WAITING_DATA;                
                gprsCommManagerState = TASK_SEND_COMMAND_SM;
                return(TASK_ON_COURSE);
            }
            //VERIFICAR
            if(gprsScheduledPwrDownTaskFlag){
                gprsTaskPowerDown();
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
            if(gprsTransactionResult == TASK_NO_SENSOR_FOUND){
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
void gprsTaskLoadPayloadBuffer(uint8_t *rawData){
    uint8_t payloadBufferPtr, buffer;
    
    for(payloadBufferPtr = 0; payloadBufferPtr < 12; payloadBufferPtr++){
        buffer = (rawData[payloadBufferPtr] >> 4) & 0x0F;
        gprsPayloadBuffer[2 * payloadBufferPtr] = (buffer < 10)? (buffer + '0'): (buffer - 10 + 'A');
        buffer = rawData[payloadBufferPtr] & 0x0F;
        gprsPayloadBuffer[2 * payloadBufferPtr + 1] = (buffer < 10)? (buffer + '0'): (buffer - 10 + 'A');    
    }
    gprsPayloadBuffer[24] = 0;
}
void gprsTaskNewTask(GPRS_TASK_COMMANDS_T command){
    switch(command){
        case GPRS_CMD_INITIALIZE:
            gprsScheduledInitTaskFlag = true;
            break;
            
        case GPRS_CMD_SEND_MSG:       
            gprsScheduledMsgAckTaskFlag = false;
            gprsScheduledMsgTaskFlag = true;
            gprsScheduledMsgStaTaskFlag = false;
            break;
        /*    
            
        case GPRS_CMD_SEND_MSG_WAIT_ACK:
            gprsScheduledMsgTaskFlag = false;
            gprsScheduledMsgAckTaskFlag = true;
            gprsScheduledMsgStaTaskFlag = false;
            break;
            
        case GPRS_CMD_SEND_STATUS:
            gprsScheduledMsgTaskFlag = false;
            gprsScheduledMsgAckTaskFlag = false;
            gprsScheduledMsgStaTaskFlag = true;
            break;
        */    
        case GPRS_CMD_PWR_DOWN:
            gprsScheduledPwrDownTaskFlag = true;
            break;
    }
}
/*
 EUSART2_GPRS_FUNCTIONS
 */
void EUSART2_GPRS_Initialize(void){
    PMD0bits.UART2MD = 0;
    // disable interrupts before changing states
    PIE3bits.RC2IE = 0;
    PIE3bits.TX2IE = 0;
    /*pedir ownership*/
    setSemaphore(GPRS_OWNER);

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
/*VERIFICAR*/
void EUSART2_GPRS_Transmit_ISR(void){
    if(gprsTxBuffer[gprsTXBufferPtr] == 0)
    {
        PIE3bits.TX2IE = 0;
        gprsReceive();
    }
    else
    {
        TXREG2 = gprsTxBuffer[gprsTXBufferPtr];
        gprsTXBufferPtr++;
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
//VERIFICAR
void EUSART2_GPRS_Receive_ISR(void){
    uint8_t auxVar;
    static uint8_t lastReceivedB;
    auxVar = RCREG2;
    PIR3bits.RC2IF = 0;
    if(1 == RCSTA2bits.OERR)
    {
        RCSTA2bits.CREN = 0;
        RCSTA2bits.CREN = 1;
        gprsRXBufferPtr = 0;
        auxVar = 0;
        lastReceivedB=0;
    }
    if(gprsRXBufferPtr >= (GPRS_RX_BUFFER_SIZE-1))
    {
        lastReceivedB=0;
        gprsRXBufferPtr = 0;
    }
    if((auxVar == '4'||auxVar=='0')&&(gprsRXBufferPtr > 0)&&(lastReceivedB==0x0d)){
        gprsMsgReceivedFlag = true;
        lastReceivedB=0;
        PIE3bits.RC2IE = 0;
    }
    lastReceivedB=auxVar;
    gprsRxBuffer[gprsRXBufferPtr++] = auxVar;
    gprsRxBuffer[gprsRXBufferPtr] = 0;
}