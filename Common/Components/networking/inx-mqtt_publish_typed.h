#ifndef INXWARE_mqtt_publish_typed
#define INXWARE_mqtt_publish_typed
#include "inx-component.h"

/*****************************************************
 * mqtt_publish_typed
 *****************************************************/
#define INXWARE_FB_ID_mqtt_publish_typed 0xE3D2
#define INXWARE_FB_NAME_mqtt_publish_typed "mqtt_publish_typed"
EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(mqtt_publish_typed);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(mqtt_publish_typed);
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(mqtt_publish_typed);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(mqtt_publish_typed_publish);
EHS_FB_FUNCTIONS(mqtt_publish_typed)

#endif /* INXWARE_mqtt_publish_typed */
