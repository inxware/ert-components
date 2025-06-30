#ifndef INXWARE_lorawan
#define INXWARE_lorawan
#include "inx-component.h"

/*****************************************************
 * This function block describes the LoRaWAN communication with external LoRaWAN module. The communication between the platform and this module is UART. The UART port on the platform is hardware to the first instance (e.g. UART0). The authentication mode is OTAA mode only. This is Class C (continuous) LoRaWAN communication only.
 *****************************************************/
#define INXWARE_FB_ID_lorawan 0xC89A
#define INXWARE_FB_NAME_lorawan "lorawan"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(lorawan); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(lorawan); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(lorawan); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(lorawan_reset); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(lorawan_connect_cb); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(lorawan_send_msg_cb); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(lorawan_reset_cb); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(lorawan_get_statusData_cb); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(lorawan_set_datarate); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(lorawan_set_datarate_cb); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(lorawan_disable); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(lorawan_connect); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(lorawan_send_msg); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(lorawan_get_statusData); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(lorawan_get_payload_length); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(lorawan_get_payload_length_cb); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(lorawan_disable_cb); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(lorawan_on_receive_msg); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(lorawan)

#endif /* INXWARE_lorawan */
