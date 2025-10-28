
 /***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/* @file target_textdisplay.c
 *
 * @author: inx limited
 *
 */
#include "globals.h"
#include "target_textdisplay.h"
//#include "usblcd.h"
//#include "usblcd_util.h"
#include "target_textdisplay.h"
#include "hal_process.h" //@todo move this too when the thread cancel function runs.

//usblcd_operations* mylcd;

EHS_GLOBAL void EhsInitTextdisplay()
{
    /*Initialise usb panel*/
//	mylcd = new_usblcd_operations();
//	mylcd->hid->debug(1);

    /* set usblcd debug level */
//	mylcd->debug(1);

    /* init the USB LCD */
//	mylcd->init(mylcd);

    /* clear the LCD screen */
//	mylcd->clear(mylcd);
}

EHS_GLOBAL void EhsWriteTextdisplay(ehs_sint32 row, ehs_sint32 col, char* text)
{
//	mylcd->settext(mylcd, row, col, text);
}

EHS_GLOBAL void EhsClearTextdisplay()
{
//	mylcd->clear(mylcd);
}

EHS_GLOBAL unsigned char EhsReadKeysTextdisplay()
{
    /*	usblcd_event *event;
    	if ((event = mylcd->read_events(mylcd)) != NULL)
    	{
    		if (event->type == 0)
    		{
    		    return(event->data[0]);
    		}
    	}
    	*/
    return 0xff;

}

EHS_GLOBAL void* getPanel()
{
    return NULL;
}
/*
EHS_GLOBAL usblcd_operations* getPanel()
{
	return mylcd;
}
*/

EHS_GLOBAL EhsTPThread pLcdThread;
void lcdThreadCancel() //@todo this is targe specific but should go eith the create code really
{
    if(pLcdThread != NULL) //@todo ??
    {
        if( pLcdThread != NULL)
        {
            /*The polling thread has been started so kill it*/
            /* Kill a thread !!! don't use if you can avoid !!! */
            #ifdef EHS_RE_INTRODUCE_THREAD_HANDLES
            EhsTPThread_terminate(pLcdThread);
            #endif
            #warning ("Not implementing the pLCthread killing")
        }
    }
}

