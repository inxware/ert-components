#ifndef INXWARE_unity
#define INXWARE_unity
#include "inx-component.h"

/*****************************************************
 * Controls the current Unity window
 *****************************************************/
#define INXWARE_FB_unity "unity"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(unity); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(unity); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(unity); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(unity_write); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(unity)

#endif /* INXWARE_unity */
