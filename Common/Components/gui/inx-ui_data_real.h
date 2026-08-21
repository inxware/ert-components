#ifndef INXWARE_ui_data_real
#define INXWARE_ui_data_real
#include "inx-component.h"

/* Bind a real (float) property on a UI object
 *
 * Functions:
 *   set              function
 *   changed_cb       callback
 */
#define INXWARE_FB_ID_ui_data_real   0x1879
#define INXWARE_FB_NAME_ui_data_real "ui_data_real"

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(ui_data_real);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(ui_data_real);
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(ui_data_real);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ui_data_real_set);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ui_data_real_changed_cb);

EHS_FB_FUNCTIONS(ui_data_real)

#endif /* INXWARE_ui_data_real */
