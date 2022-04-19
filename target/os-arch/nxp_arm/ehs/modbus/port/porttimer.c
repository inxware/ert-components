/*
 * FreeModbus Libary: BARE Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id$
 */

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"
#include "heatrod_config.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "timers.h"
/* ----------------------- static functions ---------------------------------*/
//static void prvvTIMERExpiredISR( void );

/* ----------------------- Start implementation -----------------------------*/
static TimerHandle_t gTimer=0;
void timerCallback( TimerHandle_t xTimer ){
	//we do a poll here if needed so that gpio is turned on quicker if needed
	BOOL needPoll=pxMBPortCBTimerExpired();
	if(needPoll){
		eMBPoll();
	}
}
extern uint32_t gUARTBaudRate;
BOOL
xMBPortTimersInit(){
	u8_t usTimerT35_50us = 2;
        if( gUARTBaudRate > 19200 )
        {
            //setting this to 1 seems to break receiving, seem to miss bytes
            //modbus spec says response time is 1.750ms min, this should give us 2.0ms
            usTimerT35_50us = 2;
        }
        else
        {
            /* The timer reload value for a character is given by:
             *
             * ChTimeValue = Ticks_per_1s / ( Baudrate / 11 )
             *             = 11 * Ticks_per_1s / Baudrate
             *             = 220000 / Baudrate
             * The reload for t3.5 is 1.5 times this value and similary
             * for t3.5.
             */
            //usTimerT35_50us = ( 7UL * 220000UL ) / ( 2UL * ulBaudRate );
            //(11(bits) * 3.5(ct))/(baudRate)
            usTimerT35_50us = (38500 / gUARTBaudRate)+1;
        }
        //printf("PBB 120 %d\n",usTimerT35_50us);
	//HW_DIAG_UART_printf("xMBPortTimersInit %d\n",usTim1Timerout50us);
	if(gTimer){
		xTimerDelete(gTimer,0);
		gTimer=0;
	}
	gTimer = xTimerCreate
	                 ( "modbus timer",
	                		 usTimerT35_50us,
	                   false,
	                   NULL,
	                   timerCallback );
    return TRUE;
}


inline void
vMBPortTimersEnable(  )
{
    /* Enable the timer with the timeout passed to xMBPortTimersInit( ) */
	if(gTimer){
		xTimerStartFromISR(gTimer,0);
	}
}

inline void
vMBPortTimersDisable(  )
{
    /* Disable any pending timers. */
	if(gTimer){
		xTimerStopFromISR(gTimer,0);
	}
}

/* Create an ISR which is called whenever the timer has expired. This function
 * must then call pxMBPortCBTimerExpired( ) to notify the protocol stack that
 * the timer has expired.
 */
/*static void prvvTIMERExpiredISR( void )
{
    ( void )pxMBPortCBTimerExpired(  );
}
*/
