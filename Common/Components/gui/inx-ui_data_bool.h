#ifndef INXWARE_ui_data_bool
#define INXWARE_ui_data_bool
#include "inx-component.h"

/* Bind a boolean property on a UI object
 *
 * Functions:
 *   set              function
 *   changed_cb       callback
 */
#define INXWARE_FB_ID_ui_data_bool   0xBA59
#define INXWARE_FB_NAME_ui_data_bool "ui_data_bool"

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(ui_data_bool);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(ui_data_bool);
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(ui_data_bool);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ui_data_bool_set);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ui_data_bool_changed_cb);

EHS_FB_FUNCTIONS(ui_data_bool)

#endif /* INXWARE_ui_data_bool */
