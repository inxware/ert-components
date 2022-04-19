/*
 * target_textdisplay.c
 *
 *  Created on: 08-Sep-2008
 *      Author: pbeaumont
 */
#include "target_textdisplay.h"
//#include "usblcd.h"
//#include "widgets.h"
//#include "usblcd_util.h"

//usblcd_operations* mylcd;

EHS_GLOBAL void EhsInitLeds()
{
//	mylcd = getPanel();
}

EHS_GLOBAL void EhsSetLed(int led, ehs_bool state)
{
	if(led == 6)
	{
//		mylcd->backlight(mylcd, (int)state);
	}
	else
	{
//		mylcd->setled(mylcd, led, (int)state);
	}
}
