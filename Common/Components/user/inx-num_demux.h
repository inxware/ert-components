#ifndef INXWARE_NumDemultiplexReal
#define INXWARE_NumDemultiplexReal
#include "inx-component.h"

/*****************************************************
 * This is a 8-output real number demultiplexer.
 *****************************************************/
#define INXWARE_FB_ID_NumDemultiplexReal 0xFA38
#define INXWARE_FB_NAME_NumDemultiplexReal "NumDemultiplexReal"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(NumDemultiplexReal); // this relates to C file identify function
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(NumDemultiplexReal); // this relates to C-file init function
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(NumDemultiplexReal); //this relates to C-file destroy function
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(NumDemultiplexReal_Run_Output); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(NumDemultiplexReal)

#endif /* INXWARE_NumDemultiplexReal */
