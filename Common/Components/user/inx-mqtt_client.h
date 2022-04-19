#ifndef INXWARE_mqtt_client
#define INXWARE_mqtt_client
#include "inx-component.h"

/*****************************************************
 * mqtt_client
 *****************************************************/
#define INXWARE_FB_mqtt_client "mqtt_client"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(mqtt_client); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(mqtt_client); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(mqtt_client); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(mqtt_client_disconnect); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(mqtt_client_connect); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(mqtt_client)

#endif /* INXWARE_mqtt_client */
