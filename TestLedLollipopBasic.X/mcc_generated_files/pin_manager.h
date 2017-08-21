/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using MPLAB(c) Code Configurator

  @Description:
    This header file provides implementations for pin APIs for all pins selected in the GUI.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - 4.15.1
        Device            :  PIC18LF23K22
        Version           :  1.01
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.35
        MPLAB             :  MPLAB X 3.40

    Copyright (c) 2013 - 2015 released Microchip Technology Inc.  All rights reserved.

    Microchip licenses to you the right to use, modify, copy and distribute
    Software only when embedded on a Microchip microcontroller or digital signal
    controller that is integrated into your product or third party product
    (pursuant to the sublicense terms in the accompanying license agreement).

    You should refer to the license agreement accompanying this Software for
    additional information regarding your rights and obligations.

    SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
    EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
    MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
    IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
    CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
    OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
    INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
    CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
    SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
    (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

*/


#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set ACCX aliases
#define ACCX_TRIS               TRISAbits.TRISA0
#define ACCX_LAT                LATAbits.LATA0
#define ACCX_PORT               PORTAbits.RA0
#define ACCX_ANS                ANSELAbits.ANSA0
#define ACCX_SetHigh()            do { LATAbits.LATA0 = 1; } while(0)
#define ACCX_SetLow()             do { LATAbits.LATA0 = 0; } while(0)
#define ACCX_Toggle()             do { LATAbits.LATA0 = ~LATAbits.LATA0; } while(0)
#define ACCX_GetValue()           PORTAbits.RA0
#define ACCX_SetDigitalInput()    do { TRISAbits.TRISA0 = 1; } while(0)
#define ACCX_SetDigitalOutput()   do { TRISAbits.TRISA0 = 0; } while(0)
#define ACCX_SetAnalogMode()  do { ANSELAbits.ANSA0 = 1; } while(0)
#define ACCX_SetDigitalMode() do { ANSELAbits.ANSA0 = 0; } while(0)

// get/set VCAP aliases
#define VCAP_TRIS               TRISAbits.TRISA1
#define VCAP_LAT                LATAbits.LATA1
#define VCAP_PORT               PORTAbits.RA1
#define VCAP_ANS                ANSELAbits.ANSA1
#define VCAP_SetHigh()            do { LATAbits.LATA1 = 1; } while(0)
#define VCAP_SetLow()             do { LATAbits.LATA1 = 0; } while(0)
#define VCAP_Toggle()             do { LATAbits.LATA1 = ~LATAbits.LATA1; } while(0)
#define VCAP_GetValue()           PORTAbits.RA1
#define VCAP_SetDigitalInput()    do { TRISAbits.TRISA1 = 1; } while(0)
#define VCAP_SetDigitalOutput()   do { TRISAbits.TRISA1 = 0; } while(0)
#define VCAP_SetAnalogMode()  do { ANSELAbits.ANSA1 = 1; } while(0)
#define VCAP_SetDigitalMode() do { ANSELAbits.ANSA1 = 0; } while(0)

// get/set LED aliases
#define LED_TRIS               TRISAbits.TRISA2
#define LED_LAT                LATAbits.LATA2
#define LED_PORT               PORTAbits.RA2
#define LED_ANS                ANSELAbits.ANSA2
#define LED_SetHigh()            do { LATAbits.LATA2 = 1; } while(0)
#define LED_SetLow()             do { LATAbits.LATA2 = 0; } while(0)
#define LED_Toggle()             do { LATAbits.LATA2 = ~LATAbits.LATA2; } while(0)
#define LED_GetValue()           PORTAbits.RA2
#define LED_SetDigitalInput()    do { TRISAbits.TRISA2 = 1; } while(0)
#define LED_SetDigitalOutput()   do { TRISAbits.TRISA2 = 0; } while(0)
#define LED_SetAnalogMode()  do { ANSELAbits.ANSA2 = 1; } while(0)
#define LED_SetDigitalMode() do { ANSELAbits.ANSA2 = 0; } while(0)

// get/set IO_RA3 aliases
#define IO_RA3_TRIS               TRISAbits.TRISA3
#define IO_RA3_LAT                LATAbits.LATA3
#define IO_RA3_PORT               PORTAbits.RA3
#define IO_RA3_ANS                ANSELAbits.ANSA3
#define IO_RA3_SetHigh()            do { LATAbits.LATA3 = 1; } while(0)
#define IO_RA3_SetLow()             do { LATAbits.LATA3 = 0; } while(0)
#define IO_RA3_Toggle()             do { LATAbits.LATA3 = ~LATAbits.LATA3; } while(0)
#define IO_RA3_GetValue()           PORTAbits.RA3
#define IO_RA3_SetDigitalInput()    do { TRISAbits.TRISA3 = 1; } while(0)
#define IO_RA3_SetDigitalOutput()   do { TRISAbits.TRISA3 = 0; } while(0)
#define IO_RA3_SetAnalogMode()  do { ANSELAbits.ANSA3 = 1; } while(0)
#define IO_RA3_SetDigitalMode() do { ANSELAbits.ANSA3 = 0; } while(0)

// get/set IO_RA4 aliases
#define IO_RA4_TRIS               TRISAbits.TRISA4
#define IO_RA4_LAT                LATAbits.LATA4
#define IO_RA4_PORT               PORTAbits.RA4
#define IO_RA4_SetHigh()            do { LATAbits.LATA4 = 1; } while(0)
#define IO_RA4_SetLow()             do { LATAbits.LATA4 = 0; } while(0)
#define IO_RA4_Toggle()             do { LATAbits.LATA4 = ~LATAbits.LATA4; } while(0)
#define IO_RA4_GetValue()           PORTAbits.RA4
#define IO_RA4_SetDigitalInput()    do { TRISAbits.TRISA4 = 1; } while(0)
#define IO_RA4_SetDigitalOutput()   do { TRISAbits.TRISA4 = 0; } while(0)

// get/set VDDEN aliases
#define VDDEN_TRIS               TRISAbits.TRISA6
#define VDDEN_LAT                LATAbits.LATA6
#define VDDEN_PORT               PORTAbits.RA6
#define VDDEN_SetHigh()            do { LATAbits.LATA6 = 1; } while(0)
#define VDDEN_SetLow()             do { LATAbits.LATA6 = 0; } while(0)
#define VDDEN_Toggle()             do { LATAbits.LATA6 = ~LATAbits.LATA6; } while(0)
#define VDDEN_GetValue()           PORTAbits.RA6
#define VDDEN_SetDigitalInput()    do { TRISAbits.TRISA6 = 1; } while(0)
#define VDDEN_SetDigitalOutput()   do { TRISAbits.TRISA6 = 0; } while(0)

// get/set VCAPEN aliases
#define VCAPEN_TRIS               TRISAbits.TRISA7
#define VCAPEN_LAT                LATAbits.LATA7
#define VCAPEN_PORT               PORTAbits.RA7
#define VCAPEN_SetHigh()            do { LATAbits.LATA7 = 1; } while(0)
#define VCAPEN_SetLow()             do { LATAbits.LATA7 = 0; } while(0)
#define VCAPEN_Toggle()             do { LATAbits.LATA7 = ~LATAbits.LATA7; } while(0)
#define VCAPEN_GetValue()           PORTAbits.RA7
#define VCAPEN_SetDigitalInput()    do { TRISAbits.TRISA7 = 1; } while(0)
#define VCAPEN_SetDigitalOutput()   do { TRISAbits.TRISA7 = 0; } while(0)

// get/set MODCS aliases
#define MODCS_TRIS               TRISBbits.TRISB0
#define MODCS_LAT                LATBbits.LATB0
#define MODCS_PORT               PORTBbits.RB0
#define MODCS_WPU                WPUBbits.WPUB0
#define MODCS_ANS                ANSELBbits.ANSB0
#define MODCS_SetHigh()            do { LATBbits.LATB0 = 1; } while(0)
#define MODCS_SetLow()             do { LATBbits.LATB0 = 0; } while(0)
#define MODCS_Toggle()             do { LATBbits.LATB0 = ~LATBbits.LATB0; } while(0)
#define MODCS_GetValue()           PORTBbits.RB0
#define MODCS_SetDigitalInput()    do { TRISBbits.TRISB0 = 1; } while(0)
#define MODCS_SetDigitalOutput()   do { TRISBbits.TRISB0 = 0; } while(0)
#define MODCS_SetPullup()      do { WPUBbits.WPUB0 = 1; } while(0)
#define MODCS_ResetPullup()    do { WPUBbits.WPUB0 = 0; } while(0)
#define MODCS_SetAnalogMode()  do { ANSELBbits.ANSB0 = 1; } while(0)
#define MODCS_SetDigitalMode() do { ANSELBbits.ANSB0 = 0; } while(0)

// get/set MODRST aliases
#define MODRST_TRIS               TRISBbits.TRISB1
#define MODRST_LAT                LATBbits.LATB1
#define MODRST_PORT               PORTBbits.RB1
#define MODRST_WPU                WPUBbits.WPUB1
#define MODRST_ANS                ANSELBbits.ANSB1
#define MODRST_SetHigh()            do { LATBbits.LATB1 = 1; } while(0)
#define MODRST_SetLow()             do { LATBbits.LATB1 = 0; } while(0)
#define MODRST_Toggle()             do { LATBbits.LATB1 = ~LATBbits.LATB1; } while(0)
#define MODRST_GetValue()           PORTBbits.RB1
#define MODRST_SetDigitalInput()    do { TRISBbits.TRISB1 = 1; } while(0)
#define MODRST_SetDigitalOutput()   do { TRISBbits.TRISB1 = 0; } while(0)
#define MODRST_SetPullup()      do { WPUBbits.WPUB1 = 1; } while(0)
#define MODRST_ResetPullup()    do { WPUBbits.WPUB1 = 0; } while(0)
#define MODRST_SetAnalogMode()  do { ANSELBbits.ANSB1 = 1; } while(0)
#define MODRST_SetDigitalMode() do { ANSELBbits.ANSB1 = 0; } while(0)

// get/set MISO_CTS aliases
#define MISO_CTS_TRIS               TRISBbits.TRISB2
#define MISO_CTS_LAT                LATBbits.LATB2
#define MISO_CTS_PORT               PORTBbits.RB2
#define MISO_CTS_WPU                WPUBbits.WPUB2
#define MISO_CTS_ANS                ANSELBbits.ANSB2
#define MISO_CTS_SetHigh()            do { LATBbits.LATB2 = 1; } while(0)
#define MISO_CTS_SetLow()             do { LATBbits.LATB2 = 0; } while(0)
#define MISO_CTS_Toggle()             do { LATBbits.LATB2 = ~LATBbits.LATB2; } while(0)
#define MISO_CTS_GetValue()           PORTBbits.RB2
#define MISO_CTS_SetDigitalInput()    do { TRISBbits.TRISB2 = 1; } while(0)
#define MISO_CTS_SetDigitalOutput()   do { TRISBbits.TRISB2 = 0; } while(0)
#define MISO_CTS_SetPullup()      do { WPUBbits.WPUB2 = 1; } while(0)
#define MISO_CTS_ResetPullup()    do { WPUBbits.WPUB2 = 0; } while(0)
#define MISO_CTS_SetAnalogMode()  do { ANSELBbits.ANSB2 = 1; } while(0)
#define MISO_CTS_SetDigitalMode() do { ANSELBbits.ANSB2 = 0; } while(0)

// get/set MOSI_RTS aliases
#define MOSI_RTS_TRIS               TRISBbits.TRISB3
#define MOSI_RTS_LAT                LATBbits.LATB3
#define MOSI_RTS_PORT               PORTBbits.RB3
#define MOSI_RTS_WPU                WPUBbits.WPUB3
#define MOSI_RTS_ANS                ANSELBbits.ANSB3
#define MOSI_RTS_SetHigh()            do { LATBbits.LATB3 = 1; } while(0)
#define MOSI_RTS_SetLow()             do { LATBbits.LATB3 = 0; } while(0)
#define MOSI_RTS_Toggle()             do { LATBbits.LATB3 = ~LATBbits.LATB3; } while(0)
#define MOSI_RTS_GetValue()           PORTBbits.RB3
#define MOSI_RTS_SetDigitalInput()    do { TRISBbits.TRISB3 = 1; } while(0)
#define MOSI_RTS_SetDigitalOutput()   do { TRISBbits.TRISB3 = 0; } while(0)
#define MOSI_RTS_SetPullup()      do { WPUBbits.WPUB3 = 1; } while(0)
#define MOSI_RTS_ResetPullup()    do { WPUBbits.WPUB3 = 0; } while(0)
#define MOSI_RTS_SetAnalogMode()  do { ANSELBbits.ANSB3 = 1; } while(0)
#define MOSI_RTS_SetDigitalMode() do { ANSELBbits.ANSB3 = 0; } while(0)

// get/set MODIRQ aliases
#define MODIRQ_TRIS               TRISBbits.TRISB4
#define MODIRQ_LAT                LATBbits.LATB4
#define MODIRQ_PORT               PORTBbits.RB4
#define MODIRQ_WPU                WPUBbits.WPUB4
#define MODIRQ_ANS                ANSELBbits.ANSB4
#define MODIRQ_SetHigh()            do { LATBbits.LATB4 = 1; } while(0)
#define MODIRQ_SetLow()             do { LATBbits.LATB4 = 0; } while(0)
#define MODIRQ_Toggle()             do { LATBbits.LATB4 = ~LATBbits.LATB4; } while(0)
#define MODIRQ_GetValue()           PORTBbits.RB4
#define MODIRQ_SetDigitalInput()    do { TRISBbits.TRISB4 = 1; } while(0)
#define MODIRQ_SetDigitalOutput()   do { TRISBbits.TRISB4 = 0; } while(0)
#define MODIRQ_SetPullup()      do { WPUBbits.WPUB4 = 1; } while(0)
#define MODIRQ_ResetPullup()    do { WPUBbits.WPUB4 = 0; } while(0)
#define MODIRQ_SetAnalogMode()  do { ANSELBbits.ANSB4 = 1; } while(0)
#define MODIRQ_SetDigitalMode() do { ANSELBbits.ANSB4 = 0; } while(0)

// get/set MODPWREN aliases
#define MODPWREN_TRIS               TRISBbits.TRISB5
#define MODPWREN_LAT                LATBbits.LATB5
#define MODPWREN_PORT               PORTBbits.RB5
#define MODPWREN_WPU                WPUBbits.WPUB5
#define MODPWREN_ANS                ANSELBbits.ANSB5
#define MODPWREN_SetHigh()            do { LATBbits.LATB5 = 1; } while(0)
#define MODPWREN_SetLow()             do { LATBbits.LATB5 = 0; } while(0)
#define MODPWREN_Toggle()             do { LATBbits.LATB5 = ~LATBbits.LATB5; } while(0)
#define MODPWREN_GetValue()           PORTBbits.RB5
#define MODPWREN_SetDigitalInput()    do { TRISBbits.TRISB5 = 1; } while(0)
#define MODPWREN_SetDigitalOutput()   do { TRISBbits.TRISB5 = 0; } while(0)
#define MODPWREN_SetPullup()      do { WPUBbits.WPUB5 = 1; } while(0)
#define MODPWREN_ResetPullup()    do { WPUBbits.WPUB5 = 0; } while(0)
#define MODPWREN_SetAnalogMode()  do { ANSELBbits.ANSB5 = 1; } while(0)
#define MODPWREN_SetDigitalMode() do { ANSELBbits.ANSB5 = 0; } while(0)

// get/set RB6 procedures
#define RB6_SetHigh()    do { LATBbits.LATB6 = 1; } while(0)
#define RB6_SetLow()   do { LATBbits.LATB6 = 0; } while(0)
#define RB6_Toggle()   do { LATBbits.LATB6 = ~LATBbits.LATB6; } while(0)
#define RB6_GetValue()         PORTBbits.RB6
#define RB6_SetDigitalInput()   do { TRISBbits.TRISB6 = 1; } while(0)
#define RB6_SetDigitalOutput()  do { TRISBbits.TRISB6 = 0; } while(0)
#define RB6_SetPullup()     do { WPUBbits.WPUB6 = 1; } while(0)
#define RB6_ResetPullup()   do { WPUBbits.WPUB6 = 0; } while(0)

// get/set RB7 procedures
#define RB7_SetHigh()    do { LATBbits.LATB7 = 1; } while(0)
#define RB7_SetLow()   do { LATBbits.LATB7 = 0; } while(0)
#define RB7_Toggle()   do { LATBbits.LATB7 = ~LATBbits.LATB7; } while(0)
#define RB7_GetValue()         PORTBbits.RB7
#define RB7_SetDigitalInput()   do { TRISBbits.TRISB7 = 1; } while(0)
#define RB7_SetDigitalOutput()  do { TRISBbits.TRISB7 = 0; } while(0)
#define RB7_SetPullup()     do { WPUBbits.WPUB7 = 1; } while(0)
#define RB7_ResetPullup()   do { WPUBbits.WPUB7 = 0; } while(0)

// get/set VCCEN aliases
#define VCCEN_TRIS               TRISCbits.TRISC0
#define VCCEN_LAT                LATCbits.LATC0
#define VCCEN_PORT               PORTCbits.RC0
#define VCCEN_SetHigh()            do { LATCbits.LATC0 = 1; } while(0)
#define VCCEN_SetLow()             do { LATCbits.LATC0 = 0; } while(0)
#define VCCEN_Toggle()             do { LATCbits.LATC0 = ~LATCbits.LATC0; } while(0)
#define VCCEN_GetValue()           PORTCbits.RC0
#define VCCEN_SetDigitalInput()    do { TRISCbits.TRISC0 = 1; } while(0)
#define VCCEN_SetDigitalOutput()   do { TRISCbits.TRISC0 = 0; } while(0)

// get/set IO_RC1 aliases
#define IO_RC1_TRIS               TRISCbits.TRISC1
#define IO_RC1_LAT                LATCbits.LATC1
#define IO_RC1_PORT               PORTCbits.RC1
#define IO_RC1_SetHigh()            do { LATCbits.LATC1 = 1; } while(0)
#define IO_RC1_SetLow()             do { LATCbits.LATC1 = 0; } while(0)
#define IO_RC1_Toggle()             do { LATCbits.LATC1 = ~LATCbits.LATC1; } while(0)
#define IO_RC1_GetValue()           PORTCbits.RC1
#define IO_RC1_SetDigitalInput()    do { TRISCbits.TRISC1 = 1; } while(0)
#define IO_RC1_SetDigitalOutput()   do { TRISCbits.TRISC1 = 0; } while(0)

// get/set IO_RC2 aliases
#define IO_RC2_TRIS               TRISCbits.TRISC2
#define IO_RC2_LAT                LATCbits.LATC2
#define IO_RC2_PORT               PORTCbits.RC2
#define IO_RC2_ANS                ANSELCbits.ANSC2
#define IO_RC2_SetHigh()            do { LATCbits.LATC2 = 1; } while(0)
#define IO_RC2_SetLow()             do { LATCbits.LATC2 = 0; } while(0)
#define IO_RC2_Toggle()             do { LATCbits.LATC2 = ~LATCbits.LATC2; } while(0)
#define IO_RC2_GetValue()           PORTCbits.RC2
#define IO_RC2_SetDigitalInput()    do { TRISCbits.TRISC2 = 1; } while(0)
#define IO_RC2_SetDigitalOutput()   do { TRISCbits.TRISC2 = 0; } while(0)
#define IO_RC2_SetAnalogMode()  do { ANSELCbits.ANSC2 = 1; } while(0)
#define IO_RC2_SetDigitalMode() do { ANSELCbits.ANSC2 = 0; } while(0)

// get/set I2C_EN aliases
#define I2C_EN_TRIS               TRISCbits.TRISC5
#define I2C_EN_LAT                LATCbits.LATC5
#define I2C_EN_PORT               PORTCbits.RC5
#define I2C_EN_ANS                ANSELCbits.ANSC5
#define I2C_EN_SetHigh()            do { LATCbits.LATC5 = 1; } while(0)
#define I2C_EN_SetLow()             do { LATCbits.LATC5 = 0; } while(0)
#define I2C_EN_Toggle()             do { LATCbits.LATC5 = ~LATCbits.LATC5; } while(0)
#define I2C_EN_GetValue()           PORTCbits.RC5
#define I2C_EN_SetDigitalInput()    do { TRISCbits.TRISC5 = 1; } while(0)
#define I2C_EN_SetDigitalOutput()   do { TRISCbits.TRISC5 = 0; } while(0)
#define I2C_EN_SetAnalogMode()  do { ANSELCbits.ANSC5 = 1; } while(0)
#define I2C_EN_SetDigitalMode() do { ANSELCbits.ANSC5 = 0; } while(0)

// get/set RC6 procedures
#define RC6_SetHigh()    do { LATCbits.LATC6 = 1; } while(0)
#define RC6_SetLow()   do { LATCbits.LATC6 = 0; } while(0)
#define RC6_Toggle()   do { LATCbits.LATC6 = ~LATCbits.LATC6; } while(0)
#define RC6_GetValue()         PORTCbits.RC6
#define RC6_SetDigitalInput()   do { TRISCbits.TRISC6 = 1; } while(0)
#define RC6_SetDigitalOutput()  do { TRISCbits.TRISC6 = 0; } while(0)
#define RC6_SetAnalogMode() do { ANSELCbits.ANSC6 = 1; } while(0)
#define RC6_SetDigitalMode()do { ANSELCbits.ANSC6 = 0; } while(0)

// get/set RC7 procedures
#define RC7_SetHigh()    do { LATCbits.LATC7 = 1; } while(0)
#define RC7_SetLow()   do { LATCbits.LATC7 = 0; } while(0)
#define RC7_Toggle()   do { LATCbits.LATC7 = ~LATCbits.LATC7; } while(0)
#define RC7_GetValue()         PORTCbits.RC7
#define RC7_SetDigitalInput()   do { TRISCbits.TRISC7 = 1; } while(0)
#define RC7_SetDigitalOutput()  do { TRISCbits.TRISC7 = 0; } while(0)
#define RC7_SetAnalogMode() do { ANSELCbits.ANSC7 = 1; } while(0)
#define RC7_SetDigitalMode()do { ANSELCbits.ANSC7 = 0; } while(0)

/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/