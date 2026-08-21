#ifndef INXWARE_ui_data_string
#define INXWARE_ui_data_string
#include "inx-component.h"

/* Bind a string property on a UI object
 *
 * Functions:
 *   set              function
 *   changed_cb       callback
 */
#define INXWARE_FB_ID_ui_data_string   0x47A7
#define INXWARE_FB_NAME_ui_data_string "ui_data_string"

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(ui_data_string);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(ui_data_string);
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(ui_data_string);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ui_data_string_set);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ui_data_string_changed_cb);

EHS_FB_FUNCTIONS(ui_data_string)

#endif /* INXWARE_ui_data_string */
