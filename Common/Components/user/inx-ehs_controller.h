#ifndef INXWARE_ehs_controller
#define INXWARE_ehs_controller
#include "inx-component.h"

/*****************************************************
 * Controls the current EHS window
 *****************************************************/
#define INXWARE_FB_ehs_controller "ehs_controller"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ehs_controller); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ehs_controller); // this relates to C-file init function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ehs_controller_exit_ehs); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ehs_controller_move_to_foreground); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ehs_controller_move_to_background); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ehs_controller)

#endif /* INXWARE_ehs_controller */
