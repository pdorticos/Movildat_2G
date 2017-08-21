/**
  EUSART1 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    eusart1.c

  @Summary
    This is the generated driver implementation file for the EUSART1 driver using MPLAB(c) Code Configurator

  @Description
    This header file provides implementations for driver APIs for EUSART1.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - 3.16
        Device            :  PIC18LF23K22
        Driver Version    :  2.00
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.35
        MPLAB             :  MPLAB X 3.20
*/

/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/**
  Section: Included Files
*/
#include "eusart1.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>



/**
  Section: Macro Declarations
*/
#define EUSART1_TX_BUFFER_SIZE 30
#define EUSART1_RX_BUFFER_SIZE 50
#define GPS_MSG_FILTER_SIZE 10
#define GPS_MAX_RESPONSE_TIME 90
#define GPS_COMMAND_RESPONSE_TIME 3
#define GPS_INTERMESSAGE_TIME 2
#define GPS_MSG_MAX_ATTEMPS 3

#define _EXCLUDE_GPS_UTC_DATA 1
//#define _EXCLUDE_GPS_BLOCK 1

#ifdef _EXCLUDE_GPS_BLOCK


uint32_t utilityGetIntegerPart(double arg){
    return((uint32_t)arg);
}

uint32_t utilityGetDecimalParts(double arg, uint8_t precision){
    int32_t result;
    //result = exp(precision) * (arg - (uint32_t)arg);  
    return((uint32_t)(arg - ((uint32_t)arg));
}

#define _UGI(x) utilityGetDecimalParts(x)
#define _UGF(x) utilityGetDecimalParts(x)

_gpsResult_t gpsTask(void){
    double sprintfTestDouble = 3.1415951623;
    char sprintTestBuffer[50];
    
//    sprintf(sprintTestBuffer,"$PMTK741,%4d.%6d",(int)_UGI(sprintfTestDouble),_UGF(sprintfTestDouble));
    Nop();
    return(GPS_NO_SENSOR_FOUND);
}
void EUSART1_Initialize(void){
}
void gpsTaskTick(void){
}
void EUSART1_Receive_ISR(void)
{
    uint8_t auxVar;
    
    auxVar = RCREG1;
    PIR1bits.RC1IF = 0;
}
void EUSART1_Transmit_ISR(void)
{
    PIR1bits.TX1IF = 0;
}

#else

#ifndef _EXCLUDE_GPS_UTC_DATA
typedef struct {
    uint8_t UTCSeconds;
    uint8_t UTCMinutes;
    uint8_t UTCHours;
    uint8_t UTCDay;
    uint8_t UTCMonth;
    uint16_t UTCYear;
    uint8_t UTCValidity;
} UTCTime_t;

static UTCTime_t systemTimer;
#endif

static uint8_t gpsRxBufferPtr = 0;
static uint8_t gpsRxBuffer[EUSART1_RX_BUFFER_SIZE];
static uint8_t gpsRxBufferFilter[GPS_MSG_FILTER_SIZE];

static uint8_t gpsTxBufferPtr = 0;
static uint8_t gpsTxBuffer[EUSART1_TX_BUFFER_SIZE];
static uint8_t gpsTOTimer;
static uint8_t gpsTxDelay;
static uint8_t GPS_STATE;
static uint8_t GPS_INIT_STATE;
static uint8_t gpsAttempts;
static enum gpsResult_t gpsResult;


uint8_t gpsValidity[2];
float gpsLatitude;
float gpsLongitude;
float gpsSpeed;
float gpsCourse;

/**
  Section: EUSART1 APIs
*/

void EUSART1_Initialize(void)
{
    // disable interrupts before changing states
    PIE1bits.RC1IE = 0;
    PIE1bits.TX1IE = 0;

    // Set the EUSART1 module to the options selected in the user interface.

    // ABDOVF no_overflow; CKTXP async_noninverted_sync_fallingedge; BRG16 16bit_generator; WUE disabled; ABDEN disabled; DTRXP not_inverted; 
    BAUDCON1 = 0x08;

    // SPEN enabled; RX9 8-bit; CREN enabled; ADDEN disabled; SREN disabled; 
    RCSTA1 = 0x90;

    // TX9 8-bit; TX9D 0; SENDB sync_break_complete; TXEN enabled; SYNC asynchronous; BRGH hi_speed; CSRC slave_mode; 
    TXSTA1 = 0x24;

    // Baud Rate = 9600; 
    SPBRG1 = 0x19;

    // Baud Rate = 9600; 
    SPBRGH1 = 0x00;

    // initializing the driver state
    GPS_STATE = GPS_RESET_TASK;
    
    
}

bool gpsTxReady(void){
    return(!PIE1bits.TX1IE);
}

bool gpsMsgArrived(void){
    return(!PIE1bits.RC1IE);
}
#ifndef _EXCLUDE_GPS_UTC_DATA
void gpsUTC_Update(uint16_t secondsEllapsed){
    uint16_t auxUtcUpdate;
    
    auxUtcUpdate = systemTimer.UTCSeconds + secondsEllapsed;
    systemTimer.UTCSeconds = auxUtcUpdate % 60;
    auxUtcUpdate -= systemTimer.UTCSeconds;
    if(!auxUtcUpdate){
        return;
    }
    
    systemTimer.UTCMinutes = auxUtcUpdate % 60;
    auxUtcUpdate -= systemTimer.UTCMinutes;
    if(!auxUtcUpdate){
        return;
    }
    
    systemTimer.UTCHours = auxUtcUpdate % 24;
    auxUtcUpdate -= systemTimer.UTCHours;
    if(!auxUtcUpdate){
        return;
    }
    
    systemTimer.UTCDay++;
    switch(systemTimer.UTCMonth){
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            if(systemTimer.UTCDay <= 31){
                return;
            }
            break;
        case 2:
            // TODO: Fix año bisiesto
            if(systemTimer.UTCDay <= 28){
                return;
            }
            break;
        default:
            if(systemTimer.UTCDay <= 30){
                return;
            }
    }
    systemTimer.UTCDay = 1;
    systemTimer.UTCMonth++;
    if(systemTimer.UTCMonth>12){
        systemTimer.UTCMonth = 1;
        systemTimer.UTCYear++;
    }
 
}
#endif //_EXCLUDE_GPS_UTC_DATA
void gpsTaskTick(void){
    if(gpsTOTimer){
        gpsTOTimer--;
    }
    if(gpsTxDelay){
        gpsTxDelay--;
    }
#ifndef _EXCLUDE_GPS_UTC_DATA
    gpsUTC_Update(1);
#endif
}

bool gpsDataValid(void){

    return(gpsValidity[0] == 'A');
}

void gpsRMCParser(void){

    uint8_t parserBuffer[12];
//    uint8_t auxBuffArray[8];
    uint8_t *parserPtr;
    
    strcpy(gpsValidity,"V");
    parserPtr = strtok(gpsRxBuffer,",");
    if(parserPtr != NULL){
        parserPtr = strtok(NULL,",");
        if(parserPtr != NULL){
            strcpy(parserBuffer,parserPtr);                     //  utc time  
                        
            parserPtr = strtok(NULL,",");
            if(parserPtr != NULL){
                strcpy(gpsValidity,parserPtr);                  // validity
                
                if(gpsDataValid()){
#ifndef _EXCLUDE_GPS_UTC_DATA
                    strncpy(auxBuffArray,parserBuffer,2);
                    systemTimer.UTCHours = atoi(auxBuffArray);
                    strncpy(auxBuffArray,parserBuffer+2,2);
                    systemTimer.UTCMinutes = atoi(auxBuffArray);
                    strncpy(auxBuffArray,parserBuffer+4,2);
                    systemTimer.UTCSeconds = atoi(auxBuffArray);
#endif
                    parserPtr = strtok(NULL,",");               // Latitude
                    if(parserPtr != NULL){
                        gpsLatitude = atof(parserPtr)/100;          
                                                
                        parserPtr = strtok(NULL,",");           // N/S
                        if(parserPtr != NULL){
                            if(parserPtr[0] == 'S'){
                                gpsLatitude *=-1;
                            }
                            
                            parserPtr = strtok(NULL,",");
                            if(parserPtr != NULL){
                                gpsLongitude = atof(parserPtr)/100;   // Longitude
                                                                
                                parserPtr = strtok(NULL,",");   // E/W
                                if(parserPtr != NULL){
                                    if(parserPtr[0] == 'W'){
                                        gpsLongitude *=-1;
                                    } 
                                    
                                    parserPtr = strtok(NULL,","); // Speed
                                    if(parserPtr != NULL){
                                        gpsSpeed = atof(parserPtr); 
                                        
                                        parserPtr = strtok(NULL,","); // Course
                                        if(parserPtr != NULL){
                                            gpsCourse = atof(parserPtr); 
 #ifndef _EXCLUDE_GPS_UTC_DATA                                           
                                            parserPtr = strtok(NULL,",");
                                            if(parserPtr != NULL){

                                                
                                                strncpy(auxBuffArray,parserBuffer,2);
                                                systemTimer.UTCDay = atoi(auxBuffArray);
                                                strncpy(auxBuffArray,parserBuffer+2,2);
                                                systemTimer.UTCMonth = atoi(auxBuffArray);
                                                strncpy(auxBuffArray,parserBuffer+4,2);
                                                systemTimer.UTCYear = atoi(auxBuffArray) + 2000;

                                            }
#endif
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

bool gpsResponseParser(void){
    //TODO: Write the GPS RESULT MEMORY ARRAY with the read values in the NMEA response
    return(true);
}

void gpsCommandSend(uint8_t GpsCommandID)
{
    const unsigned char gpsCommandTail[4] = {0x0D, 0x0A, 0};
    char p1[8];
    double gpsAuxDouble;
    
    strcpy(gpsTxBuffer,"$PMTK");
    switch(GpsCommandID){        
        case GPS_FULLPWR_MSG:
            strcpy(gpsTxBuffer,"$PMTK225,0*2B");
            break;
        case GPS_REFRESH_MSG:
//            sprintf(gpsTxBuffer,"$PMTK741,%S,%S,,%S,%S,%S,%S,%S,%S*",ftoa(gpsLatitude),ftoa(gpsLongitude),itoa(systemTimer.UTCYear),itoa(systemTimer.UTCMonth),itoa(systemTimer.UTCDay),ftoa(systemTimer.UTCHours),ftoa(systemTimer.UTCMinutes),ftoa(systemTimer.UTCSeconds));
            
            
            /*itoa(p1, (int) systemTimer.UTCHours, 10);
            strcat(gpsTxBuffer,p1);
            strcat(gpsTxBuffer,",");*/
            
            gpsAuxDouble = (double)gpsLatitude;
//            sprintf((char*)gpsTxBuffer,"$PMTK741,%10.6f",gpsAuxDouble);
            break;
        case GPS_PPS_OFF_MSG:
            strcat(gpsTxBuffer,"285,0*21");
            break;
        case GPS_SET_DATUM_MSG:
            strcat(gpsTxBuffer,"330,70*19");
            break;
/*        case GPS_HOTSTART:
            strcpy(gpsTxBuffer,"$PMTK101*32");
            break;
        case GPS_WARMSTART:
            strcpy(gpsTxBuffer,"$PMTK102*31");
            break;
        case GPS_STATIC_SPEED:
            break;
        case GPS_COLDSTART_MSG:
            strcpy(gpsTxBuffer,"$PMTK103*30");
            break;

        case GPS_STANDBY:
            strcpy(gpsTxBuffer,"$PMTK103*30");
            break;
        case GPS_FAST_FIX:
            strcpy(gpsTxBuffer,"$PMTK257,0*1E");
            break;
        case GPS_HI_ACCURACY:
            strcpy(gpsTxBuffer,"$PMTK257,1*2F");
            break;
 */
        default:
            gpsTxBuffer[0] = 0;
    }
    strcat(gpsTxBuffer,gpsCommandTail);
    gpsTxBufferPtr = 0;
    PIE1bits.TX1IE = 1;
}


bool gpsReceiveStateManager(uint8_t OnTimeOutState, uint8_t OnMsgOkState){
    if(gpsMsgArrived()){
        if(gpsResponseParser()) {
            if(OnMsgOkState == GPS_INIT_FINISHED_SM){
                GPS_INIT_STATE = GPS_VARINIT_SM;
                return(true);
            }
            GPS_INIT_STATE = OnMsgOkState;
            gpsAttempts = GPS_MSG_MAX_ATTEMPS;
            gpsTxDelay = GPS_INTERMESSAGE_TIME;
        }
        else if((!gpsAttempts)||(!gpsAttempts--)){
            GPS_INIT_STATE = GPS_VARINIT_SM;
            gpsResult = GPS_TO_ERR;
            return(true);
        }
    }
    if(gpsTOTimer == 0){
        PIE1bits.RC1IE = 0;
        if((!gpsAttempts)||(!gpsAttempts--)){
            GPS_INIT_STATE = GPS_VARINIT_SM;
            gpsResult = GPS_TO_ERR;
            return(true);
        }
        GPS_INIT_STATE = OnTimeOutState;
    }
    return(false);
}

void gpsTrasmitStateManager(uint8_t nmeaCommand, uint8_t promotingState){
    
    if(gpsTxReady()){
        if(gpsTxDelay == 0){
            gpsCommandSend(nmeaCommand);
            gpsTOTimer = GPS_COMMAND_RESPONSE_TIME;
            gpsReceive();
            GPS_INIT_STATE = promotingState;
        }
    }
}

bool gpsInitDone(void){
    switch(GPS_INIT_STATE){
        
        case GPS_VARINIT_SM:
            gpsAttempts = GPS_MSG_MAX_ATTEMPS;
            gpsTxDelay = GPS_INTERMESSAGE_TIME;
            if(gpsDataValid()){
                GPS_INIT_STATE = GPS_REFRESH_SM;
                
                break;
            }
            else{
                GPS_INIT_STATE = GPS_FULLPWR_SM;
            }
            
            
        case GPS_FULLPWR_SM:
            gpsSetNMEAFilter(GPS_ACK);
            gpsTrasmitStateManager(GPS_FULLPWR_MSG, GPS_FULLPWR_RESPONSE_SM);
            return(false);
            
        case GPS_FULLPWR_RESPONSE_SM:           
            return(gpsReceiveStateManager(GPS_FULLPWR_SM, GPS_DATUM_SM));
            
        case GPS_DATUM_SM:
            gpsSetNMEAFilter(GPS_ACK);
            gpsTrasmitStateManager(GPS_SET_DATUM_MSG,GPS_DATUM_RESPONSE_SM);
            return(false);
            
        case GPS_DATUM_RESPONSE_SM:
            return(gpsReceiveStateManager(GPS_DATUM_SM, GPS_PPS_OFF_SM));
           
        
        case GPS_PPS_OFF_SM:
            gpsSetNMEAFilter(GPS_ACK);
            gpsTrasmitStateManager(GPS_PPS_OFF_MSG, GPS_PPS_RESPONSE_OFF_SM);
            return(false);
            
        case GPS_PPS_RESPONSE_OFF_SM:
            return(gpsReceiveStateManager(GPS_PPS_OFF_SM, GPS_INIT_FINISHED_SM));
        
        case GPS_REFRESH_SM:
            gpsSetNMEAFilter(GPS_ACK);
            gpsTrasmitStateManager(GPS_REFRESH_MSG, GPS_REFRESH_RESPONSE_SM);
            break;
        case GPS_REFRESH_RESPONSE_SM:
            return(gpsReceiveStateManager(GPS_REFRESH_SM, GPS_INIT_FINISHED_SM));
        default:
            GPS_INIT_STATE = GPS_VARINIT_SM;
                        
    }
    return(false);
}

_gpsResult_t gpsTask(void){
    switch(GPS_STATE){
        case GPS_RESET_TASK:
            gpsRxBufferPtr = 0;
            gpsRxBuffer[0] = 0;
            gpsTxBufferPtr = 0;
            gpsTxBuffer[0] = 0;
            
            gpsResult = GPS_TASK_ON_COURSE;
            GPS_INIT_STATE = GPS_VARINIT_SM;
            GPS_STATE = GPS_INIT;
            
        case GPS_INIT:
            // TODO: Gps initialization
            if(gpsInitDone()){
                if(gpsResult == GPS_TASK_ON_COURSE){
                    GPS_STATE = GPS_WAITING_DATA;
                    gpsSetNMEAFilter(RMC_FILTER);
                    gpsTOTimer = GPS_MAX_RESPONSE_TIME;
                    gpsReceive();
                }
                else{
                    GPS_STATE = GPS_RESET_TASK;
                }
            }          
            break;
               
        case GPS_WAITING_DATA:
            if(gpsMsgArrived()){
                gpsRMCParser();
                if(gpsDataValid()){
                    gpsTOTimer = GPS_MAX_RESPONSE_TIME;
                    gpsResult = GPS_DATA_READY;
                 }
                gpsReceive();
            }
            if(gpsTOTimer == 0){
                PIE1bits.RC1IE = 0;
                gpsResult = GPS_DATA_NOT_AVAILABLE;
                GPS_STATE = GPS_RESET_TASK;
                strcpy(gpsValidity,"V");
            }
            break;
            
        default:
            GPS_STATE = GPS_RESET_TASK;
    }
    return(gpsResult);
}

void gpsReceive(void){
    
    RCSTA1bits.CREN = 0;
    RCSTA1bits.CREN = 1;
    gpsRxBufferPtr = 0;
    gpsRxBuffer[0] = 0;
    // enable receive interrupt
    
    PIE1bits.RC1IE = 1;
}

void gpsSetNMEAFilter(uint8_t filter){
    switch(filter){
        
        case GPS_ACK:
            //strcpy(gpsRxBufferFilter,"$PMTK001");
            strcpy(gpsRxBufferFilter,"$PMTK0**");
            break;
        case GPS_RESET_ACK:
            strcpy(gpsRxBufferFilter,"$PMTK0**");
            break;
        case RMC_FILTER:
        default:
            strcpy(gpsRxBufferFilter,"$G*RMC");
            break;
    }
}

void EUSART1_Transmit_ISR(void)
{
    TXREG1 = gpsTxBuffer[gpsTxBufferPtr++];
    PIR1bits.TX1IF = 0;
    // add your EUSART1 interrupt custom code
    if(gpsTxBuffer[gpsTxBufferPtr] == 0)
    {
        PIE1bits.TX1IE = 0;
        
    }
}


void EUSART1_Receive_ISR(void)
{
    uint8_t auxVar;
    
    auxVar = RCREG1;
    PIR1bits.RC1IF = 0;
    if(1 == RCSTA1bits.OERR)
    {
        RCSTA1bits.CREN = 0;
        RCSTA1bits.CREN = 1;
        gpsRxBufferPtr = 0;
        auxVar = 0;
    }
    if(gpsRxBufferPtr < strlen(gpsRxBufferFilter)){
        if((gpsRxBufferFilter[gpsRxBufferPtr] == '*')||(auxVar == gpsRxBufferFilter[gpsRxBufferPtr])){
            gpsRxBuffer[gpsRxBufferPtr++] = auxVar;
        }
        else {
            gpsRxBufferPtr = 0;
        }
    }
    else {
        if(gpsRxBufferPtr < EUSART1_RX_BUFFER_SIZE){
            if((gpsRxBuffer[gpsRxBufferPtr-1]==',')&&(auxVar ==',')){
                gpsRxBuffer[gpsRxBufferPtr++]='0';
            }
            gpsRxBuffer[gpsRxBufferPtr++] = auxVar;
        }
        if((auxVar == 0x0a)||(auxVar == 0x0d)){
            PIE1bits.RC1IE = 0;
        }
    }
    gpsRxBuffer[gpsRxBufferPtr] = 0;
}
#endif // _EXCLUDE_GPS_BLOCK
/**
  End of File
*/
