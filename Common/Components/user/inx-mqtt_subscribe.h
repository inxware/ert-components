#ifndef INXWARE_mqtt_subscribe
#define INXWARE_mqtt_subscribe
#include "inx-component.h"

/*****************************************************
 * mqtt_subscribe
 *****************************************************/
#define INXWARE_FB_mqtt_subscribe "mqtt_subscribe"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(mqtt_subscribe); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(mqtt_subscribe); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(mqtt_subscribe); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(mqtt_subscribe_subscribe); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(mqtt_subscribe_unsubscribe); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(mqtt_subscribe)

#endif /* INXWARE_mqtt_subscribe */
