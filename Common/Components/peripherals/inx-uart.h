#ifndef INXWARE_UART
#define INXWARE_UART
#include "inx-component.h"

/*****************************************************
 * The UART transceiving interface that is configured by the UART Config function block.
 *****************************************************/
#define INXWARE_FB_ID_UART 0x477D
#define INXWARE_FB_NAME_UART "UART"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(UART); // this relates to C file identify function
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(UART); // this relates to C-file init function
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(UART); //this relates to C-file destroy function
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(UART_start); //a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(UART_close); //a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(UART_send); //a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(UART_recv_cb); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(UART)

#endif /* INXWARE_UART */
