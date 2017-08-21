/* 
 * File:   sfxProtocoll.h
 * Author: pedro
 *
 * Created on 23 de marzo de 2017, 7:17
 */

#ifndef SFXPROTOCOLL_H
#define	SFXPROTOCOLL_H

/**
  Section: Included Files
*/

#include <xc.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef	__cplusplus
extern "C" {
#endif

#define HW_WARNING_EV 1
#define TIMER1_EV 2
#define TIMER2_EV 3

bool sigfoxNMEA_RMCParser(char *rmcSentence);

#ifdef	__cplusplus
}
#endif

#endif	/* SFXPROTOCOLL_H */

