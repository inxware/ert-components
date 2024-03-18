/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

/* @file target_leds.h
 *
 * @author: inx limited
 *
 */


#ifndef TARGET_LEDS_H_
#define TARGET_LEDS_H_
#include "target_types.h"
/*Declare global functions for working with textdisplay*/
EHS_GLOBAL void EhsInitLeds();
EHS_GLOBAL void EhsSetLed(ehs_sint led, ehs_bool state);
#endif /* TARGET_TEXTDISPLAY_H_ */
