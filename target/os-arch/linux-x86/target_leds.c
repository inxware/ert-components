/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

/* @file target_leds.c
 *
 * @author: inx limited
 *
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
