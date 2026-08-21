//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-uart.h"
#include "target_uart.h"
#include <string.h>
//ICB HEADER MACRO END -- DO NOT ALTER
//ICB STATE VAR MACRO START -- DO NOT ALTER
/* My Component state data structure. - Use this in your code! */
typedef struct inx_UART_state
{
	EhsCallbackQueueEntryType xEntry;
	EhsCallbackQueueEntryType xEntry1;
	ehs_sint32 GPIO_switch;
	ehs_bool RS485;
	ehs_sint32 ComPort;
	ehs_sint32 port;
} inx_UART_state_type; //Reference this, maybe store your config parameters in here too.
//ICB STATE VAR MACRO END -- DO NOT ALTER

static EhsCallbackQueueType xUartRecvCallbackQueue;
//static EhsCallbackQueueType xUartStatusCallbackQueue;
static EhsDataflowStringType gEHsUartRecvMsg = NULL;
static int gEhsUartRecvLen = 0;
static int gSendReturnCode = 1;

#ifndef EHS_TARGET_UART_COUNT 
#define EHS_TARGET_UART_COUNT 1
#endif//EHS_TARGET_UART_COUNT 

//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
/* Populate the data structure used by EHS and map the function names to strings identified in CDF */
EHS_FB_FUNCTIONS_START(UART)
EHS_FB_FUNCTION_ENTRY("start", 0x01, UART_start)
EHS_FB_FUNCTION_ENTRY("close", 0x02, UART_close)
EHS_FB_FUNCTION_ENTRY("recv_cb", 0x03, UART_recv_cb)
EHS_FB_FUNCTION_ENTRY("send", 0x04, UART_send)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER
//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
/* Friendly labels for the run function data and event function argument enumerations */
#define INX_UART_ARG_start_port 1
#define INX_UART_ARG_start_start_OK 1
#define INX_UART_ARG_close_closed 1
#define INX_UART_ARG_close_error 2
#define INX_UART_ARG_recv_cb_recv_data 1
#define INX_UART_ARG_recv_cb_recv_size 2
#define INX_UART_ARG_recv_cb_receved 1
#define INX_UART_ARG_send_send_data 1
#define INX_UART_ARG_send_send_size 2
#define INX_UART_ARG_send_errno 1
#define INX_UART_ARG_send_send_OK 1
#define INX_UART_ARG_send_send_error 2
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER
//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
/* Parameters */
/* Create some macros for the default parameters */
#define INX_FB_UART_GPIO_switch -1
#define INX_FB_UART_RS485 0
#define INX_FB_UART_ComPort 0
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
/**
 * Identify the function block to EHS.
 * This function provides access to:
 *  - string containing parameter text
 * EHS_FB_IDENTIFY_MEMORY - variable to store the memory requirements for this function block's context
 */
EHS_FB_IDENTIFY_FUNCTION(UART)
{
/* Uncomment the following if you need to parse the parameters to calculate memory required */
/*
	ehs_sint32 GPIO_switch;
	ehs_bool RS485;
	EhsSscanf(EHS_FB_IDENTIFY_PARAMETERS,"%d %d",&GPIO switch,&RS485); */
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_UART_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
/**
 * Initialise the function block. Populate the context area for the function block.
 * This function provides access to:
 *  EHS_FB_INIT_CONTEXT - pointer to the context area provided by EHS for this function block
 *  EHS_FB_INIT_PARAMETERS - string containing the parameter text
 */

EHS_FB_INIT_FUNCTION(UART)
{
	ehs_sint32 GPIO_switch;
	ehs_bool RS485;
	ehs_sint32 ComPort;
	ehs_bool bRet = EHS_TRUE; /* assume success */
	//this is the reference to the object data for this instance of the function block
	inx_UART_state_type* inx_UART_state = (inx_UART_state_type*)EHS_FB_INIT_CONTEXT;

	//EhsCallbackQueue_register(&xUartStatusCallbackQueue,
	//		   EHS_FB_RUN_NAME(UART_status_cb),
	//		   EHS_FB_INIT_CALLBACK_FUNCTION_INSTANCE(-2),
	//		   &(inx_UART_state->xEntry)
	//);
	EhsCallbackQueue_register(&xUartRecvCallbackQueue,
			   EHS_FB_RUN_NAME(UART_recv_cb),
			   EHS_FB_INIT_CALLBACK_FUNCTION_INSTANCE(-1),
			   &(inx_UART_state->xEntry1)
	);
	/* read the initialisation parameters */
	EhsSscanf(EHS_FB_INIT_PARAMETERS,"%d %hhu %d",&GPIO_switch,&RS485,&ComPort); /* RS485 is ehs_bool */
	inx_UART_state->GPIO_switch = GPIO_switch;
	inx_UART_state->RS485= RS485;
	inx_UART_state->ComPort = ComPort;
	inx_UART_state->port = ComPort;

	/* Add any further intialisation code here */
	return bRet; /* initialisation always succeeds */
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER
//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(UART)
{
	//inx_UART_state_type *inx_UART_state = (inx_UART_state_type*)EHS_FB_DESTROY_CONTEXT;
	//Your code below here
	return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION start MACRO START -- DO NOT ALTER
/**
 * Definition of UART_start.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(UART_start)
{
	inx_UART_state_type* inx_UART_state = (inx_UART_state_type*)EHS_FB_RUN_CONTEXT;

	/* Port input overrides the ComPort parameter default */
	int port = inx_UART_state->ComPort;
	if (EHS_FB_IN_CONNECTED_API2(INX_UART_ARG_start_port))
		port = EHS_FB_IN_I_API2(INX_UART_ARG_start_port);
	inx_UART_state->port = port;
	TgtUart_Stage0(port);
	/* On Windows, map the logical UART port to the physical COM port number */
	if (inx_UART_state->ComPort > 0)
		TgtUart_SetComPort(port, inx_UART_state->ComPort);
	TgtUart_Start(port,
				  gTargetUartPinTx[port],
				  gTargetUartPinRx[port],
				  gTargetUartPinRts[port],
				  gTargetUartPinCts[port],
				  gUARTBaudRate[port],
				  //TODO2024 fix the constant parameters. See the docstring in target_uart.c
				  // Data Length
				  3,
				  // Parity
				  gUARTParity[port],
				  // Stop Bits
				  gUARTStopBits[port],
				  // Flow Control
				  gUARTHWCTRL[port]);
	if (inx_UART_state->RS485 == EHS_TRUE)
		TgtUart_RS485Setup(port);
	TgtUART_Intr_register(port, Common_UART_onReceive);
	EHS_FB_FINISH(INX_UART_ARG_start_start_OK);
}//ICB FUNCTION start MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION close MACRO START -- DO NOT ALTER
/**
 * Definition of UART_close.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(UART_close)
{
	//inx_UART_state_type* inx_UART_state = (inx_UART_state_type*)EHS_FB_RUN_CONTEXT;

	// Your code here
	EHS_FB_FINISH(INX_UART_ARG_close_closed);
	EHS_FB_FINISH(INX_UART_ARG_close_error);
}//ICB FUNCTION close MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION recv_cb MACRO START -- DO NOT ALTER
/**
 * Definition of UART_recv_cb.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(UART_recv_cb)
{
	//inx_UART_state_type* inx_UART_state = (inx_UART_state_type*)EHS_FB_RUN_CONTEXT;
	
	// Your code here
	if (EHS_FB_OUT_CONNECTED_API2(INX_UART_ARG_recv_cb_recv_data))
	{
		//memset(memset(EHS_FB_OUT_S_API2(INX_UART_ARG_recv_cb_recv_data), gEHsUartRecvMsg[0], 1) + 1, 0, 1) ;
		((char *)EHS_FB_OUT_S_API2(INX_UART_ARG_recv_cb_recv_data))[0] = gEHsUartRecvMsg[0];
		((char *)EHS_FB_OUT_S_API2(INX_UART_ARG_recv_cb_recv_data))[1] = '\0';
	}
	
	if (EHS_FB_OUT_CONNECTED_API2(INX_UART_ARG_recv_cb_recv_size))
		EHS_FB_OUT_I_API2(INX_UART_ARG_recv_cb_recv_size) = gEhsUartRecvLen ;
	EHS_FB_FINISH(INX_UART_ARG_recv_cb_receved);
}//ICB FUNCTION recv_cb MACRO END -- DO NOT ALTER THIS LINE
//ICB FUNCTION send MACRO START -- DO NOT ALTER
/**
 * Definition of UART_send.
 * [User's info entered in ICB added here]
 * This function can access the object data shared using the following macros:
 *  EHS_FB_RUN_CONTEXT - pointer to the context area for this function block
 *  EHS_FB_RUN_CONTEXT_REF - pointer to the address of the context area for this function block
 */
EHS_FB_RUN_FUNCTION(UART_send)
{
	inx_UART_state_type* inx_UART_state = (inx_UART_state_type*)EHS_FB_RUN_CONTEXT;

	// Your code here
	if (EHS_FB_IN_CONNECTED_API2(INX_UART_ARG_send_send_data) && EHS_FB_IN_CONNECTED_API2(INX_UART_ARG_send_send_size))
	{
		TgtUART_SendThread(inx_UART_state->port, EHS_FB_IN_S_API2(INX_UART_ARG_send_send_data), EHS_FB_IN_I_API2(INX_UART_ARG_send_send_size));
		if (EHS_FB_OUT_CONNECTED_API2(INX_UART_ARG_send_errno))
			EHS_FB_OUT_I_API2(INX_UART_ARG_send_errno) = TgtUART_OK;
		EHS_FB_FINISH(INX_UART_ARG_send_send_OK);
	}
	else
	{
		if (EHS_FB_OUT_CONNECTED_API2(INX_UART_ARG_send_errno))
			EHS_FB_OUT_I_API2(INX_UART_ARG_send_errno) = TgtUART_CONFIG_ERROR;
		EHS_FB_FINISH(INX_UART_ARG_send_send_error);
	}
}//ICB FUNCTION send MACRO END -- DO NOT ALTER THIS LINE

/**
 * When the UART receives the messages. This is called in target's receive callback
 * */
void Common_UART_onReceive(char *recv_msg, int length)
{
	if (recv_msg == NULL)
	{
		printf("NULL pointer");
		return;
	}
	gEHsUartRecvMsg = recv_msg;
	gEhsUartRecvLen = length;
	EhsCallbackQueue_execute(&xUartRecvCallbackQueue);
}

/**
 * When the Send in thread is completed. This is called in target's thread send function
 * */
void Common_UART_onSendComplete(int retCode)
{
	gSendReturnCode = retCode;
	if (retCode <= 0)
	{
		//EhsCallbackQueue_execute(&xUartStatusCallbackQueue);
	}
}
