#ifndef INXWARE_uart_config
#define INXWARE_uart_config
#include "inx-component.h"
/*****************************************************
 * uart_config
 *****************************************************/
#define INXWARE_FB_uart_config "uart_config"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(uart_config); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(uart_config); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(uart_config); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(uart_config_disable); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(uart_config_enable); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(uart_config)

#endif /* INXWARE_uart_config */
