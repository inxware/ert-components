#ifndef INXWARE_unity2
#define INXWARE_unity2
#include "inx-component.h"

/*****************************************************
 * unity
 *****************************************************/
#define INXWARE_FB_unity2 "unity2"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(unity2); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(unity2); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(unity2); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(unity2_create); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(unity2_destroy);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(unity2_setmedia);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(unity2_stop);
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(unity2)

#endif /* INXWARE_unity2 */
