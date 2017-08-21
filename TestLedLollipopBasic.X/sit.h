/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef SIT_H
#define	SIT_H

#include <xc.h> 


#define EVENT_REP_SIZE                      3
#define SIT_FRAME_ID_SIZE                   05 

#define SITVAR_7FH_VALIDITY                 0x7F
#define SITVAR_7FH_SIZE                     2

///////////////////////////////////////////////////////////////////////////////
//                         SIGFOX FRAME IDENTIFIERS                          //                    
///////////////////////////////////////////////////////////////////////////////
                    
#define SITVAR_APP_ID                       65
#define SITVAR_SFX_SERIALNBR                66
#define SITVAR_SFX_ID                       67
  
#define TRACKER                             01
#define GPS_WEATHER_SOUND                   02                      
#define WEATHER_5LOG_BAT                    03
#define GPS_WEATHER_WEIGHT                  04
#define WEIGHT_9LOG_BAT                     05
#define SIT_FRAME_TRACKER_SF_ID             06
#define SIT_FRAME_POWERUP_SF_ID             07

///////////////////////////////////////////////////////////////////////////////
//                         SIGFOX VARIABLE IDENTIFIERS                       //                    
///////////////////////////////////////////////////////////////////////////////

#define SITVAR_0BH_BATTERY                  0x0B
#define SITVAR_0BH_SIZE                     6

#define SITVAR_16H_LATITUDE                 0x16
#define SITVAR_16H_SIZE                     28

#define SITVAR_18H_LONGITUDE                0x18
#define SITVAR_18H_SIZE                     28

#define SITVAR_1AH_GPS_SPEED                0x1A
#define SITVAR_1AH_SIZE                     8

#define SITVAR_1BH_GPS_HEADING              0x1B
#define SITVAR_1BH_SIZE                      9

#define SITVAR_20H_GPS_ODOMETER             0x20
#define SITVAR_20H_SIZE                     16

#define SITVAR_7EH_MAIN_INPUT               0x7E
#define SITVAR_7EH_SIZE                     1

#define TRACKER_SIGFOX_FRAME SIT_FRAME_TRACKER_SF_ID,SITVAR_7FH_VALIDITY,SITVAR_16H_LATITUDE,SITVAR_18H_LONGITUDE,SITVAR_20H_GPS_ODOMETER,SITVAR_7FH_VALIDITY,SITVAR_0BH_BATTERY,0

                                            
#define POWERUP_SIGFOX_FRAME SIT_FRAME_POWERUP_SF_ID,SITVAR_APP_ID,SITVAR_SFX_SERIALNBR,SITVAR_SFX_ID,0
// TODO Insert declarations

// Comment a function and leverage automatic documentation with slash star star
/**
    <p><b>Function prototype:</b></p>
  
    <p><b>Summary:</b></p>

    <p><b>Description:</b></p>

    <p><b>Precondition:</b></p>

    <p><b>Parameters:</b></p>

    <p><b>Returns:</b></p>

    <p><b>Example:</b></p>
    <code>
 
    </code>

    <p><b>Remarks:</b></p>
 */
// TODO Insert declarations or function prototypes (right here) to leverage 
// live documentation

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

