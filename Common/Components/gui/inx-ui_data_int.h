#ifndef INXWARE_ui_data_int
#define INXWARE_ui_data_int
#include "inx-component.h"

/* Bind an integer property on a UI object
 *
 * Functions:
 *   set              function
 *   changed_cb       callback
 */
#define INXWARE_FB_ID_ui_data_int   0x41FB
#define INXWARE_FB_NAME_ui_data_int "ui_data_int"

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(ui_data_int);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(ui_data_int);
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(ui_data_int);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ui_data_int_set);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ui_data_int_changed_cb);

EHS_FB_FUNCTIONS(ui_data_int)

#endif /* INXWARE_ui_data_int */
