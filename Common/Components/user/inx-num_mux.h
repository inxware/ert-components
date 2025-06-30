#ifndef INXWARE_NumMultiplexReal
#define INXWARE_NumMultiplexReal
#include "inx-component.h"

/*****************************************************
 * This is a 8-input real multiplexer.
 *****************************************************/
#define INXWARE_FB_ID_NumMultiplexReal 0xDAA7
#define INXWARE_FB_NAME_NumMultiplexReal "NumMultiplexReal"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(NumMultiplexReal); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(NumMultiplexReal); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(NumMultiplexReal); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(NumMultiplexReal_Run_Input); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(NumMultiplexReal)

#endif /* INXWARE_NumMultiplexReal */
