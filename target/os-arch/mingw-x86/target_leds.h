/*
 * target_textdisplay.h
 *
 *  Created on: 08-Sep-2008
 *      Author: pbeaumont
 */

#ifndef TARGET_LEDS_H_
#define TARGET_LEDS_H_
#include "target_types.h"
/*Declare global functions for working with textdisplay*/
EHS_GLOBAL void EhsInitLeds();
EHS_GLOBAL void EhsSetLed(ehs_sint led, ehs_bool state);
#endif /* TARGET_TEXTDISPLAY_H_ */
