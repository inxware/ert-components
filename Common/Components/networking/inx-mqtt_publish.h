#ifndef INXWARE_mqtt_publish
#define INXWARE_mqtt_publish
#include "inx-component.h"

/*****************************************************
 * mqtt_publish
 *****************************************************/
#define INXWARE_FB_ID_mqtt_publish 0x16F8
#define INXWARE_FB_NAME_mqtt_publish "mqtt_publish"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(mqtt_publish); // this relates to C file identify function
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(mqtt_publish); // this relates to C-file init function
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(mqtt_publish); //this relates to C-file destroy function
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(mqtt_publish_publish); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(mqtt_publish)

#endif /* INXWARE_mqtt_publish */
