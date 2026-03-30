#ifndef INXWARE_mqtt_subscribe_typed
#define INXWARE_mqtt_subscribe_typed
#include "inx-component.h"

/*****************************************************
 * mqtt_subscribe_typed
 *****************************************************/
#define INXWARE_FB_ID_mqtt_subscribe_typed 0x2BAE
#define INXWARE_FB_NAME_mqtt_subscribe_typed "mqtt_subscribe_typed"
EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(mqtt_subscribe_typed);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(mqtt_subscribe_typed);
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(mqtt_subscribe_typed);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(mqtt_subscribe_typed_subscribe);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(mqtt_subscribe_typed_unsubscribe);
EHS_FB_FUNCTIONS(mqtt_subscribe_typed)

#endif /* INXWARE_mqtt_subscribe_typed */
