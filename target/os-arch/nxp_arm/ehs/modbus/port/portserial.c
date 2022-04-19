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

#define CONFIG_INX_UART_DELAY 8388607/512
//#define CONFIG_INX_UART_DELAY 0
#include "inx_uart.h"
#include "heatrod_config.h"
#include "port.h"
#include "fsl_gpio.h"
#include "fsl_uart.h"
#include "timers.h"
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- static functions ---------------------------------*/
//static void prvvUARTTxReadyISR( void );
//static void prvvUARTRxISR( void );
static status_t uartStatus=kStatus_UART_Error;
static bool alreadyEnabled = false;
static TimerHandle_t gTimer=0;
static void timerCallback( TimerHandle_t xTimer ){
	UART_EnableInterrupts(DEMO_UART, kUART_RxDataRegFullInterruptEnable | kUART_RxOverrunInterruptEnable);
	EnableIRQ(DEMO_UART_IRQn);
	//this needs to be after enabling the irq or we get lock ups
	GPIO_PortClear(GPIOB, 1U << 2);
}
/* ----------------------- Start implementation -----------------------------*/

void gotoTransmit(const bool dogpio){
	alreadyEnabled =false;
	if(gTimer){
		xTimerStopFromISR(gTimer,0);
	}
	DisableIRQ(DEMO_UART_IRQn);
	//turn on interrupts for receiving, set the GPIO pin to low
	//PBB it is important to disable the other interrupts or you get infinitely firing interrupts that you don't want
	UART_DisableInterrupts(DEMO_UART, kUART_RxDataRegFullInterruptEnable | kUART_RxOverrunInterruptEnable);
	if(dogpio){
		GPIO_PortSet(GPIOB, 1U << 2);		
		//brief delay to let the driver chip switch modes before transmitting
		for(uint32_t i=0;i<8388607/512;i++){
			__ASM("nop");
		}
	}
	UART_EnableInterrupts(DEMO_UART, kUART_TxDataRegEmptyInterruptEnable);
	EnableIRQ(DEMO_UART_IRQn);
}


void vMBPortSerialReceiveReset(){
	//go to transmit
	gotoTransmit(false);
	vMBPortSerialEnable( 1, 0 );
}

void
vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
	/* If xRXEnable enable serial receive interrupts. If xTxENable enable
     * transmitter empty interrupts.
     */
	HW_DIAG_UART_printf("vMBPortSerialEnable: %d %d %d\n",uartStatus,xRxEnable,xTxEnable);
	if(uartStatus==kStatus_Success){
		if(xRxEnable && !alreadyEnabled){
			alreadyEnabled = true;
			//brief delay to let any transmission finish
			//8388607/512 115200
			/*for(uint32_t i=0;i<(8388607/335);i++){
				__ASM("nop");
			}*/

			DisableIRQ(DEMO_UART_IRQn);	

			//turn on interrupts for receiving, set the GPIO pin to low
			//PBB it is important to disable the other interrupts or you get infinitely firing interrupts that you don't want
			UART_DisableInterrupts(DEMO_UART, kUART_TxDataRegEmptyInterruptEnable);

			//this is needed to stop fifo buffer problems from data we have received before modbus was ready
			//https://community.nxp.com/thread/321175
			//disable transmit and receive
			DEMO_UART->C2 &= ~UART_C2_RE_MASK;
			DEMO_UART->C2 &= ~UART_C2_TE_MASK;

			DEMO_UART-> PFIFO = UART_PFIFO_RXFE_MASK | UART_PFIFO_RXFIFOSIZE(2);

			//uint32_t status = (DEMO_UART-> S1) | UART_S1_OR_MASK; // clear OR
			//uint8_t get = DEMO_UART->D;    //clear OR

			//clear the receive buffer
			DEMO_UART->CFIFO |= UART_CFIFO_RXFLUSH_MASK;
			DEMO_UART->SFIFO |= UART_SFIFO_RXOF_MASK;

			//enable them again
			DEMO_UART->C2 |= UART_C2_RE_MASK;
			DEMO_UART->C2 |= UART_C2_TE_MASK;
			if(gTimer){
				xTimerStartFromISR(gTimer,0);
			}else{
				//we have to make sure read is enabled again or we end up stuck in disabled
				timerCallback(gTimer);
			}
		}else if(xTxEnable){
			gotoTransmit(true);
		}
	}
}

extern uint32_t gUARTBaudRate;

BOOL
//xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
xMBPortSerialInit()
{
	//again, we need a minimum of 2 ticks or it just doesnt work
	if(gTimer==0){
		gTimer = xTimerCreate( "serial enable timer", 2, false, NULL, timerCallback );
	}
	uartStatus=inxUARTEnable();
	
	//set LSB first mode
	/*LWIP_PLATFORM_DIAG(("S2=%d",DEMO_UART->S2));
	DEMO_UART->S2 = 0;
	LWIP_PLATFORM_DIAG(("S2=%d",DEMO_UART->S2));*/
	//vMBPortSerialEnable(true,false);
    return TRUE;
}

void xMBPortSerialSetBaudRate(){
	//can't get this bit working yet
	/*
	UART_Deinit(DEMO_UART);
	uartStatus=kStatus_UART_Error;
	xMBPortTimersInit();
	xMBPortSerialInit();
	*/
	/*if(uartStatus==kStatus_Success){
		uint32_t baudRate=inxBaudRateToBPS();
		UART_SetBaudRate(DEMO_UART,baudRate,DEMO_UART_CLK_FREQ);
	}*/
}

BOOL
xMBPortSerialPutByte( CHAR ucByte )
{
	//LWIP_PLATFORM_DIAG(("xMBPortSerialPutByte: %d",ucByte));
    /* Put a byte in the UARTs transmit buffer. This function is called
     * by the protocol stack if pxMBFrameCBTransmitterEmpty( ) has been
     * called. */
	//HW_DIAG_UART_printf("uart write %x\n",ucByte);
	UART_WriteByte(DEMO_UART, ucByte);
    return TRUE;
}

extern uint8_t inxUartByte;
BOOL
xMBPortSerialGetByte( CHAR * pucByte )
{
	//LWIP_PLATFORM_DIAG(("xMBPortSerialGetByte"));
    /* Return the byte in the UARTs receive buffer. This function is called
     * by the protocol stack after pxMBFrameCBByteReceived( ) has been called.
     */
	//uint8_t byte=UART_ReadByte(DEMO_UART);
	*pucByte=(CHAR)inxUartByte;
	//HW_DIAG_UART_printf("xMBPortSerialGetByte: %d\n",*pucByte);
    return TRUE;
}

/* Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call 
 * xMBPortSerialPutByte( ) to send the character.
 */
/*static void prvvUARTTxReadyISR( void )
{
    pxMBFrameCBTransmitterEmpty(  );
}*/

/* Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
/*static void prvvUARTRxISR( void )
{
    pxMBFrameCBByteReceived(  );
}
*/
