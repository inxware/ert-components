/*
 * target_textdisplay.c
 *
 *  Created on: 08-Sep-2008
 *      Author: pbeaumont
 */
#include "target_textdisplay.h"
#include "usblcd.h"
#include "widgets.h"
#include "usblcd_util.h"
#include "target_config.h"
#ifdef USB_SUPPORT
usblcd_operations* mylcd;
#endif

EHS_GLOBAL void EhsInitLeds()
{
#ifdef USB_SUPPORT
	mylcd = getPanel();
#endif
}

EHS_GLOBAL void EhsSetLed(int led, ehs_bool state)
{
#ifdef USB_SUPPORT
	if(led == 6)
	{
		mylcd->backlight(mylcd, (int)state);
	}
	else
	{
		mylcd->setled(mylcd, led, (int)state);
	}
#endif
}
