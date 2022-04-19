#ifndef INXWARE_application_run
#define INXWARE_application_run
#include "inx-component.h"

/*****************************************************
 * Runs an app
 *****************************************************/
#define INXWARE_FB_application_run "application_run"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(application_run); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(application_run); // this relates to C-file init function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(application_run_runApp); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(application_run)

#endif /* INXWARE_application_run */
