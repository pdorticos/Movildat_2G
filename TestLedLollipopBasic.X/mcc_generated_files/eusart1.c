/******************************************************************************
  @File Name
    eusart1.c       // TelitGPS_Driver
  @ Designer
    Pedro Dorticos
  @Company
    MOVILDAT SL
  @Summary
******************************************************************************/

//  Section: Included Files
#include "eusart1.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sfxProtocol.h"
#include "../sit.h"

//  Section: Macro Declarations
#if 1
#define GPS_COMMAND_LIST_END        0xFF
#define EUSART1_TX_BUFFER_SIZE      80
#define EUSART1_RX_BUFFER_SIZE      90
#define GPS_BUFFER_FILTER_SIZE      10
#define GPS_FILTER_SIZE             8
#define MTK001_ACK_GLAG_OFFSET      13
#define GPS_DATA_VALID_AFTER_FF     5

#define GPS_MSG_MAX_ATTEMPS 3

#define GPS_COMMAND_RESPONSE_TIME 2
#define GPS_INTERMESSAGE_TIME 1
#define GPS_POWERUP_TIME 2;

#endif // Application Constants
//  Section: Local types definition
#if (1)
typedef struct {
    char DD[3];
    char MM[3];
    char YYYY[3];
}UTCTime_t;

#endif // Local Types Definitions
// Section: Global Variables
#if (1)
uint8_t gpsRxBufferPtr = 0;
char gpsRxBuffer[EUSART1_RX_BUFFER_SIZE];
char gpsRxBufferFilter[GPS_BUFFER_FILTER_SIZE];
bool (*gpsParsePtr)(void);
uint8_t gpsTxBufferPtr = 0;
char gpsTxBuffer[EUSART1_TX_BUFFER_SIZE];
uint8_t rmcParserConsecutiveOK = GPS_DATA_VALID_AFTER_FF;

uint8_t gpsTimeOutTimer;
uint8_t gpsTxDelay;

taskStates_t GPS_STATE = TASK_PARKED;
uint8_t gpsTransactionAttemps;
bool gpsMsgReceivedFlag;

bool gpsScheduledAcquireTaskFlag = false;
bool gpsScheduledPwrDownTaskFlag = false;
bool gpsScheduledInitTaskFlag = false;

#ifdef _MKT_CHECKSUM_CTRL
uint8_t inCheckSum = 0;
#endif
TaskCommManagerState_t gpsCommManagerState = TASK_SEND_COMMAND_SM;
taskResult_t gpsTransactionResult = TASK_ON_COURSE;

static UTCTime_t sysVarGpsDate;
#endif // Global Variables
void gpsReceive(void){
    
    RCSTA1bits.CREN = 0;
    RCSTA1bits.CREN = 1;
    gpsRxBufferPtr = 0;
    gpsRxBuffer[0] = 0;
    // enable receive interrupt
    gpsMsgReceivedFlag = false;
#ifdef _MKT_CHECKSUM_CTRL
    inCheckSum = 0;
#endif
    PIE1bits.RC1IE = 1;
}
#ifdef GPS_TASK_ENABLED
//////////////////////////////////////////////////////////////////
// gps commands 
//////////////////////////////////////////////////////////////////
// MTK101-Hot Restart Command 
// MTK102-Warm Restart Command (GPS_WARMSTART_MSG_STR)
// MTK103-Cold Restart Command (GPS_COLDSTART_MSG_STR)
// MTK225-Set Periodic Power Saving Mode (GPS_NORM_PWR_SAV_MODE_MSG_STR)
// MTK257-Set Solution Priority (Fast TTFF or High Position Accuracy)
// MTK301-Set DGPS Correction Data Source
// MTK313-Enable or Disable SBAS Search 
// MTK330-Set Default Datum    

const char gpsCommandsDefinitions[9][10] = {
//    GPS_HOTSTART_MSG_STR,
    GPS_WARMSTART_MSG_STR,
    GPS_COLDSTART_MSG_STR,
//    GPS_IMPROVE_TTFF_MSG_STR,
    GPS_SBAS_DGPS_MSG_STR,
    GPS_SBAS_EN_MSG_STR,
    GPS_SET_DATUM_MSG_STR, 
    GPS_REFRESH_MSG_STR,
    GPS_RECEIVE_UNS_RMC_MSG_STR,
    GPS_RECEIVE_UNS_GGA_MSG_STR,
    GPS_POWER_DOWN_MSG_STR
};

// Command APIs
void dcToolsStrncpy(char *dest, char *source, unsigned char n){
    unsigned char i;
    for(i = 0; i < n; i++){
        dest[i] = source[i];
    }
    dest[i] = 0;
}
bool gpsNMEA_RMCParser2(void){
    long auxLong1;
    char *auxStrPtr;
    char *auxNextStrPtr;
    char auxBuffArray[3];
 
    auxStrPtr = strchr(gpsRxBuffer,',');        // sysVarGpsDayTime;
    dcToolsStrncpy(auxBuffArray,auxStrPtr + 1,2); 
    auxLong1 = 3600 * strtol(auxBuffArray,&auxNextStrPtr,10);
    
    dcToolsStrncpy(auxBuffArray,auxStrPtr+3,2);
    auxLong1 += (60 * strtol(auxBuffArray,&auxNextStrPtr,10));
    
    dcToolsStrncpy(auxBuffArray,auxStrPtr+5,2);
    auxLong1 += strtol(auxBuffArray,&auxNextStrPtr,10);
    
    auxStrPtr++;
    strtol(auxStrPtr,&auxNextStrPtr,10);        // skip utc

    auxStrPtr = auxNextStrPtr;
    sysVarGpsValidFlag = (bool)(strtol(auxStrPtr,&auxNextStrPtr,10) == 1);
    if(sysVarGpsValidFlag == true){
        if(rmcParserConsecutiveOK){
            rmcParserConsecutiveOK--;
            gpsTransactionAttemps++;
        }
        sysVarGpsDayTime = auxLong1;
        
        auxStrPtr = auxNextStrPtr;
        sysVarGpsLat = strtol(auxStrPtr,&auxNextStrPtr,10);
        
        auxStrPtr = auxNextStrPtr;
        if(strtol(auxStrPtr,&auxNextStrPtr,10) == 0){
            sysVarGpsLat *= -1;
        }

        auxStrPtr = auxNextStrPtr;
        sysVarGpsLong = strtol(auxStrPtr,&auxNextStrPtr,10);
        auxStrPtr = auxNextStrPtr;
        if(strtol(auxStrPtr,&auxNextStrPtr,10) == 0){
            sysVarGpsLong *= -1;
        }
        
        auxStrPtr = auxNextStrPtr;
        sysVarGpsSpeed = (strtol(auxStrPtr,&auxNextStrPtr,10)* 185,2)/100;
        
        auxStrPtr = auxNextStrPtr;
        sysVarGpsCourse = strtol(auxStrPtr,&auxNextStrPtr,10)/100;
        
        dcToolsStrncpy(sysVarGpsDate.DD,auxNextStrPtr + 1,2);
        dcToolsStrncpy(sysVarGpsDate.MM,auxNextStrPtr+3,2);
        dcToolsStrncpy(sysVarGpsDate.YYYY,auxNextStrPtr+5,2);
    }
    else{
        rmcParserConsecutiveOK = GPS_DATA_VALID_AFTER_FF;
    }
    return(rmcParserConsecutiveOK == 0);
}
#ifdef GPS_GET_ALTITUDE
bool gpsNMEA_GGAParser(void){
    char *auxStrPtr;
    char *auxNextStrPtr;
    unsigned i;
    
    auxNextStrPtr = gpsRxBuffer;
    for(i=0; i<8; i++){
        auxStrPtr = strchr(auxNextStrPtr,' ');
        auxNextStrPtr = auxStrPtr + 1;
    }
    sysVarGpsAlt = strtol(auxStrPtr,&auxNextStrPtr,10);
    return(true);
}
#endif
bool gpsMTKParser(void){
    if(gpsRxBuffer[9] == 'M'){
        return(true);
    }
    return((gpsRxBuffer[MTK001_ACK_GLAG_OFFSET] == '3')||(gpsRxBuffer[MTK001_ACK_GLAG_OFFSET] == '2'));
}

void gpsSetMTKFilter(uint8_t filter){
    switch(filter){
        
        case GPS_MTK_CMD_ACK:
            strcpy(gpsRxBufferFilter,"$PMTK001");
            break;
        case GPS_RESET_ACK:
            strcpy(gpsRxBufferFilter,"$PMTK011");
            break;
#ifdef GPS_GET_ALTITUDE
        case GGA_FILTER:
            strcpy(gpsRxBufferFilter,"$G*GGA**");
            break;
#endif
        case RMC_FILTER:
        default:
            strcpy(gpsRxBufferFilter,"$G*RMC**");
            break;
    }
}
void gpsLong2AsciiFloat(long longValue, long divider, uint8_t decimalPoints){
    
    long aritmeticLong;
    uint8_t asciiFloatAuxChar;
    char asciiFloatAuxStr[12];
    
    aritmeticLong = (longValue/divider);
    ltoa(asciiFloatAuxStr,aritmeticLong,10);
    strcat(gpsTxBuffer,asciiFloatAuxStr);
    if(decimalPoints == 0){return;}
    strcat(gpsTxBuffer,".");
    aritmeticLong = (long)(longValue%divider);
    if(aritmeticLong < 0){
        aritmeticLong *= -1;
    }
    ltoa(asciiFloatAuxStr,aritmeticLong,10);
    asciiFloatAuxStr[decimalPoints] = 0;
    strcat(gpsTxBuffer,asciiFloatAuxStr);
    asciiFloatAuxChar = strlen(asciiFloatAuxStr);
    while(asciiFloatAuxChar < decimalPoints){
        strcat(gpsTxBuffer,"0");
        asciiFloatAuxChar++;
    }
}
void gpsMtkMsgCheckSum(void){
    uint8_t checkSumAuxChar;
    char *checkSumStrPtr;
    char checkSumAuxStr[3];
    
    checkSumAuxChar = 0;
    checkSumStrPtr = gpsTxBuffer + 1;
    while(*checkSumStrPtr != 0){
        checkSumAuxChar ^=  *checkSumStrPtr++;
    }
    strcat(gpsTxBuffer,"*");
    ltoa(checkSumAuxStr,checkSumAuxChar,16);
    strcat(gpsTxBuffer,checkSumAuxStr);
}
void gpsRefreshLocationMsgBuild(void){
    long anotherAuxLong;
    strcat(gpsTxBuffer,",");
    gpsLong2AsciiFloat(sysVarGpsLat, 1000000, 6);
    strcat(gpsTxBuffer,",");
    gpsLong2AsciiFloat(sysVarGpsLong, 1000000, 6);
    strcat(gpsTxBuffer,",");
    gpsLong2AsciiFloat(sysVarGpsAlt, 10, 0);
    strcat(gpsTxBuffer,",20");
    strcat(gpsTxBuffer,sysVarGpsDate.YYYY);
    strcat(gpsTxBuffer,",");
    strcat(gpsTxBuffer,sysVarGpsDate.MM);
    strcat(gpsTxBuffer,",");
    strcat(gpsTxBuffer,sysVarGpsDate.DD);
    strcat(gpsTxBuffer,",");
    anotherAuxLong = sysVarGpsDayTime + 1;
    gpsLong2AsciiFloat(anotherAuxLong, 3600,0);
    strcat(gpsTxBuffer,",");
    anotherAuxLong = anotherAuxLong % 3600;
    gpsLong2AsciiFloat(anotherAuxLong, 60,0);
    strcat(gpsTxBuffer,",");
    anotherAuxLong = anotherAuxLong % 60;
    gpsLong2AsciiFloat(anotherAuxLong, 1,0);
    
}
void gpsSetDatumMsgBuild(void){
#ifdef GPS_DEFAULT_DATUM_SPAIN_PORT
    strcat(gpsTxBuffer,",70");
    return;
#elif defined(GPS_DEFAULT_DATUM_EUROPEAN)
    strcat(gpsTxBuffer,",67");
    return;
#else
    strcat(gpsTxBuffer,",0");
#endif
}
void gpsCommandSelector(gpsCommands_t gpsCommand){

    const char GPS_COMMAND_TAIL_STR[] = {0x0d,0x0A,0};
 
    strcpy(gpsTxBuffer,"$PMTK");
    gpsParsePtr = gpsMTKParser;
    gpsSetMTKFilter(GPS_MTK_CMD_ACK); 
    strcat(gpsTxBuffer,gpsCommandsDefinitions[gpsCommand]);

    switch(gpsCommand){
        case GPS_REFRESH_MSG:
            gpsRefreshLocationMsgBuild();
            gpsMtkMsgCheckSum();                    
            break;
        case GPS_SET_DATUM_MSG:
            gpsSetDatumMsgBuild();
            gpsMtkMsgCheckSum();
            break;
//        case GPS_HOTSTART_MSG:
        case GPS_WARMSTART_MSG:
        case GPS_COLDSTART_MSG:
            gpsSetMTKFilter(GPS_RESET_ACK);
            break;
//        case GPS_IMPROVE_TTFF_MSG:
        case GPS_POWER_DOWN_MSG:
            break;
        case GPS_RECEIVE_UNS_RMC_MSG:
            gpsTxBuffer[0] = 0;
            gpsTimeOutTimer = GPS_COMMAND_RESPONSE_TIME;
            gpsSetMTKFilter(RMC_FILTER);
            gpsParsePtr = gpsNMEA_RMCParser2;
            return;
#ifdef GPS_GET_ALTITUDE
        case GPS_RECEIVE_UNS_GGA_MSG:
            gpsTxBuffer[0] = 0;
            gpsTimeOutTimer = GPS_COMMAND_RESPONSE_TIME;
            gpsSetMTKFilter(GGA_FILTER);
            gpsParsePtr = gpsNMEA_GGAParser;
            return;
#endif
        case GPS_SBAS_EN_MSG:
        case GPS_SBAS_DGPS_MSG:
            break;
        default:
            gpsTxBuffer[0] = 0;
            return;
    }
    strcat(gpsTxBuffer,GPS_COMMAND_TAIL_STR);
    gpsTimeOutTimer = GPS_COMMAND_RESPONSE_TIME;
}

//////////////////////////////////////////////////////////////////
// Low level routines
//////////////////////////////////////////////////////////////////
void EUSART1_Initialize(void){
    PMD0bits.UART1MD = 0;;
    PIE1bits.RC1IE = 0;
    PIE1bits.TX1IE = 0;
    BAUDCON1 = 0x08;
    RCSTA1 = 0x90;
    TXSTA1 = 0x24;
// Baud Rate = 9600; 
    SPBRG1 = 0x67;
    SPBRGH1 = 0x00;   
    // initializing the driver state
}
void gpsTransmit(void){
    gpsTxBufferPtr = 0;
    PIE1bits.TX1IE = 1;
}
bool gpsTxReady(void){
    if((!gpsTxDelay)){
        return(!PIE1bits.TX1IE);
    }
    return(false);
}
//////////////////////////////////////////////////////////////////
// Low level Communication Protocol functions
//////////////////////////////////////////////////////////////////
taskResult_t gpsVerifyTransactionStatus(void){
    
    gpsTxDelay = GPS_INTERMESSAGE_TIME;
    gpsCommManagerState = TASK_SEND_COMMAND_SM;
    if(gpsMsgReceivedFlag){
        gpsTransactionResult = TASK_DATA_READY;
        gpsMsgReceivedFlag = false;
        return(TASK_DATA_READY);
    }
    if(gpsTransactionAttemps){
        gpsTransactionAttemps--;
    }
    if(gpsTransactionAttemps == 0){
        GPS_STATE = TASK_RESET_TASK;
        gpsTransactionResult = TASK_NO_SENSOR_FOUND;
        return(TASK_NO_SENSOR_FOUND);
    }
    gpsTransactionResult = TASK_ON_COURSE;
    return(TASK_ON_COURSE);
}
taskResult_t gpsCommManager(gpsCommands_t gpsCommand){
    
    gpsTransactionResult = TASK_ON_COURSE;
    switch(gpsCommManagerState){
    case TASK_SEND_COMMAND_SM:
        if(gpsTxReady()){
            gpsCommandSelector(gpsCommand);
            gpsCommManagerState = TASK_DETECT_TRANSACTION_END_SM;
            if(strlen(gpsTxBuffer)){
                gpsTransmit();                
            }
            else{
                PIE1bits.TX1IE = 0;
                gpsReceive();
            }
        }       
        break;
        
    case TASK_DETECT_TRANSACTION_END_SM:      // Stop comm driver
        if(gpsTimeOutTimer == 0){           // due to timeout
            PIE1bits.TX1IE = 0;
            PIE1bits.RC1IE = 0;
            return(gpsVerifyTransactionStatus());
        }
        if(gpsMsgReceivedFlag){             // due to message arrived
            gpsTimeOutTimer = 0;
            return(gpsVerifyTransactionStatus());
        }
        break;
    default:
        gpsCommManagerState = TASK_SEND_COMMAND_SM;
        gpsTxDelay = GPS_INTERMESSAGE_TIME;
        break;
    }
    return(TASK_ON_COURSE);
}

//////////////////////////////////////////////////////////////////
// Public Task Functions
//////////////////////////////////////////////////////////////////
void gpsTaskTick(void){
    if(gpsTimeOutTimer){
        gpsTimeOutTimer--;
    }
    if(gpsTxDelay){
        gpsTxDelay--;
    }
}
void gpsTaskPowerDown(void){
    gpsScheduledInitTaskFlag = false;
    gpsScheduledAcquireTaskFlag = false;
    gpsScheduledPwrDownTaskFlag = false;
    PMD0bits.UART1MD = 1;
    GPS_STATE = TASK_PARKED;
}
void gpsTaskReset(void){
    EUSART1_Initialize();
    GPS_STATE = TASK_RESET_TASK;
    gpsTxDelay = GPS_POWERUP_TIME;
    gpsCommManagerState = TASK_SEND_COMMAND_SM;
    gpsTransactionResult = TASK_ON_COURSE;
    gpsTransactionAttemps = 3;
    gpsTimeOutTimer = 0;
    gpsMsgReceivedFlag = false;
}
    const char gpsInitWarmStartCommandsRomList[] = {
        GPS_REFRESH_MSG,
        //GPS_IMPROVE_TTFF_MSG,        
        GPS_SBAS_EN_MSG,
        GPS_SBAS_DGPS_MSG,
        GPS_WARMSTART_MSG,
        GPS_COMMAND_LIST_END
        };

    const char gpsInitColdStartCommandsRomList[] = {
        GPS_SET_DATUM_MSG,
        //GPS_IMPROVE_TTFF_MSG,
        GPS_COLDSTART_MSG,
        GPS_SET_DATUM_MSG,
        GPS_SBAS_EN_MSG,
        GPS_SBAS_DGPS_MSG,
        GPS_COMMAND_LIST_END
        };
    const char gpsAcquisitionMessagesRomList[] = {
        GPS_RECEIVE_UNS_RMC_MSG,
#ifdef GPS_GET_ALTITUDE
//        GPS_RECEIVE_UNS_GGA_MSG,
#endif
        GPS_COMMAND_LIST_END
    };
    const char gpsPowerDownMessagesRomList[] = {
        GPS_POWER_DOWN_MSG,
        GPS_COMMAND_LIST_END
    };
taskResult_t gpsTask(void){
// Hot Restart that uses all available data in the NonVolatile (NV) Store      
//    const char gpsInitHotStartCommandsRomList[] = {
//        GPS_HOTSTART_MSG,
//        GPS_COMMAND_LIST_END
//        };

    
    const char *gpsFlashCommandPtr;
    bool gpsParserResult;
    
    switch(GPS_STATE){
        case TASK_PARKED:
            if(gpsScheduledInitTaskFlag){
                GPS_STATE = TASK_RESET_TASK;
                return(TASK_ON_COURSE);
            }
            return(TASK_NO_SENSOR_FOUND);
            
        case TASK_RESET_TASK:
            gpsTaskReset();
            GPS_STATE = TASK_INIT;
            if(!sysVarGpsValidFlag){
                gpsFlashCommandPtr = gpsInitColdStartCommandsRomList;
            }
            else{
                gpsFlashCommandPtr = gpsInitWarmStartCommandsRomList;
            }
            return(TASK_ON_COURSE);
            
        case TASK_INIT:
            while(gpsFlashCommandPtr[0] != GPS_COMMAND_LIST_END)
            {
                gpsTransactionResult = gpsCommManager(gpsFlashCommandPtr[0]);
                if(gpsTransactionResult == TASK_DATA_READY){
                    if(gpsMTKParser()){
                        gpsTransactionAttemps = 3;
                        gpsFlashCommandPtr += 1;
                        
                    }
                    else if(gpsTransactionAttemps){
                        gpsTransactionAttemps--;
                    }
                    continue;
                }
                if(gpsTransactionResult == TASK_NO_SENSOR_FOUND){
#ifdef IGNORE_INIT_ERROR_FLAG
                    gpsFlashCommandPtr++;
                    gpsTransactionAttemps = 3;
                    continue;
#else
                    gpsTaskPowerDown();
#endif
                }
                return(gpsTransactionResult);
            }
            GPS_STATE = TASK_READY;
            gpsScheduledInitTaskFlag = false;
            if(gpsScheduledAcquireTaskFlag||gpsScheduledPwrDownTaskFlag){
                return(TASK_ON_COURSE);
            }
            return(TASK_DATA_READY);

        case TASK_READY:
            if(gpsScheduledAcquireTaskFlag){
                rmcParserConsecutiveOK = GPS_DATA_VALID_AFTER_FF;
                gpsTransactionAttemps = GPS_MAX_TTFF;
                GPS_STATE = TASK_WAITING_DATA;
                gpsFlashCommandPtr = gpsAcquisitionMessagesRomList;
                sysVarGpsValidFlag = false;
                return(TASK_ON_COURSE);
            }
            if(gpsScheduledPwrDownTaskFlag){
                gpsTransactionAttemps = 3;
                GPS_STATE = TASK_WAITING_DATA;
                gpsFlashCommandPtr = gpsPowerDownMessagesRomList;
                return(TASK_ON_COURSE);
            }
            return(TASK_DATA_READY);
            
        case TASK_WAITING_DATA:
            
            while(gpsFlashCommandPtr[0] != GPS_COMMAND_LIST_END){
                gpsTransactionResult = gpsCommManager(gpsFlashCommandPtr[0]);
                if(gpsTransactionResult == TASK_DATA_READY){
                    gpsParserResult = gpsParsePtr();
                    if(gpsParserResult){
                        gpsFlashCommandPtr++;
                        if(gpsScheduledAcquireTaskFlag){
                            continue;
                        }
                        
                        gpsTaskPowerDown();
                        return(TASK_NO_SENSOR_FOUND);
                    }
                    if(gpsTransactionAttemps){
                        gpsTransactionAttemps--;
                    }
                    else{
                        gpsTransactionResult = TASK_NO_SENSOR_FOUND;
                    }
                }
                if(gpsTransactionResult == TASK_NO_SENSOR_FOUND){                    
                    if(gpsScheduledAcquireTaskFlag){
                        gpsScheduledAcquireTaskFlag = false;
                        GPS_STATE = TASK_READY;
                        if(!gpsScheduledPwrDownTaskFlag){
                            return(TASK_DATA_NOT_AVAILABLE);
                        }
                    }
                    else if(gpsScheduledPwrDownTaskFlag){
                        gpsTaskPowerDown();
                        return(TASK_NO_SENSOR_FOUND);
                    }
                }
                return(TASK_ON_COURSE);
            }
            if(gpsScheduledAcquireTaskFlag){
                gpsScheduledAcquireTaskFlag = false;
            }
            else{
                gpsScheduledPwrDownTaskFlag = false;
            }
            GPS_STATE = TASK_READY;
            return(TASK_DATA_READY);
        default:
            gpsTransactionResult = TASK_ON_COURSE; 
            GPS_STATE = TASK_RESET_TASK;
            break;
    }
    return(gpsTransactionResult);
}
void gpsTaskNewTask(GPS_TASK_COMMANDS_T command){
    switch(command){
       
        case GPS_CMD_INITIALIZE:
            gpsScheduledInitTaskFlag = true;
            break;
            
        case GPS_CMD_ACQUIRE:       
            gpsScheduledAcquireTaskFlag = true;
            break;
            
        case GPS_CMD_PWR_DOWN:
            gpsScheduledPwrDownTaskFlag = true;
            break;
    }
}
#endif
void EUSART1_Transmit_ISR(void){
    TXREG1 = gpsTxBuffer[gpsTxBufferPtr++];
    PIR1bits.TX1IF = 0;
    // add your EUSART1 interrupt custom code
    if(gpsTxBuffer[gpsTxBufferPtr] == 0)
    {
        PIE1bits.TX1IE = 0;
        gpsReceive();       
    }
}
void EUSART1_Receive_ISR(void){
    char auxVar;
    
    auxVar = RCREG1;
    if(auxVar == '$'){
        gpsRxBufferPtr = 0;
#ifdef _MKT_CHECKSUM_CTRL
        inCheckSum = '$';
    }
    inCheckSum ^= auxVar;
#else
    }
#endif
    PIR1bits.RC1IF = 0;
    if(1 == RCSTA1bits.OERR){
        RCSTA1bits.CREN = 0;
        RCSTA1bits.CREN = 1;
        gpsRxBufferPtr = 0;
        auxVar = 0;
#ifdef _MKT_CHECKSUM_CTRL
        inCheckSum = 0;
#endif
        return;
    }
    if(gpsRxBufferPtr < GPS_FILTER_SIZE){        
        if((gpsRxBufferFilter[gpsRxBufferPtr] != '*')&&(auxVar != gpsRxBufferFilter[gpsRxBufferPtr])){
#ifdef _MKT_CHECKSUM_CTRL
            inCheckSum = 0;
#endif
            gpsRxBuffer[0] = 0;
            gpsRxBufferPtr = 0;
            return;
        }
    }
    else if(gpsRxBufferPtr < (EUSART1_RX_BUFFER_SIZE-3)){

        switch(auxVar){
        case '.':
        case ' ':
            return;
        case ',':
            if(gpsRxBuffer[gpsRxBufferPtr-1] == ' '){
                gpsRxBuffer[gpsRxBufferPtr++] = '0';
            }
            auxVar = ' ';
            break;
        case 'A':
        case 'N':
        case 'E':
            auxVar = '1';
            break;
        case 'V':
        case 'W':
        case 'S':
            auxVar = '0';
            break;
        case '*':
#ifdef _MKT_CHECKSUM_CTRL
            inCheckSum ^= '*';
#endif
            auxVar = ' ';
            break;
        case '\n':  
            PIE1bits.RC1IE = 0;
            gpsMsgReceivedFlag = true;
            break;
        }
    }
    else{
#ifdef _MKT_CHECKSUM_CTRL
        inCheckSum = 0;
#endif
        gpsRxBufferPtr = 0;
        gpsRxBuffer[0] = 0;
        gpsMsgReceivedFlag = false;
        return;
    }
    gpsRxBuffer[gpsRxBufferPtr++] = auxVar;
    gpsRxBuffer[gpsRxBufferPtr] = 0;
}
dataQualifier_t gpsTaskGetVarValidity(uint8_t variableID){
#ifndef GPS_TASK_ENABLED
    return(UNKNOWN_SENSOR);
#endif
    switch(variableID){
        case SITVAR_16H_LATITUDE:
        case SITVAR_18H_LONGITUDE:
        case SITVAR_20H_GPS_ODOMETER:
            if(sysVarGpsValidFlag){
                return(SENSOR_READ);
            }
            return(NOT_AVAILABLE);
    }
    return(UNKNOWN_SENSOR);
}
/**
  End of File
*/
