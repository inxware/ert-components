#ifndef INXWARE_ndc
#define INXWARE_ndc
#include "inx-component.h"

/*****************************************************
 * numeric_display_char
 *****************************************************/
#define INXWARE_FB_ndc "ndc"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(ndc); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(ndc); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(ndc); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(ndc_set); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ndc)

#endif /* INXWARE_ndc */
