#ifndef INXWARE_ui_event
#define INXWARE_ui_event
#include "inx-component.h"

/* Bind a signal on a UI object
 *
 * Functions:
 *   emit             function
 *   received_cb      callback
 */
#define INXWARE_FB_ID_ui_event   0xAA58
#define INXWARE_FB_NAME_ui_event "ui_event"

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(ui_event);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(ui_event);
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(ui_event);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ui_event_emit);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ui_event_received_cb);

EHS_FB_FUNCTIONS(ui_event)

#endif /* INXWARE_ui_event */
