/*
 * target_textdisplay.c
 *
 *  Created on: 08-Sep-2008
 *      Author: pbeaumont
 */
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

extern EhsTPThread pLcdThread;

void lcdThreadCancel() //@todo this is targe specific but should go eith the create code really
{
	if(pLcdThread != NULL) //@todo ??
	{
		if( pLcdThread != NULL)
		{
			/*The polling thread has been started so kill it*/
			/* Kill a thread !!! don't use if you can avoid !!! */
			EhsTPThread_terminate(pLcdThread);
		}
	}
}

