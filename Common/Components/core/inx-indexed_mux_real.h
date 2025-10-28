#ifndef INXWARE_indexed_mux_real
#define INXWARE_indexed_mux_real
#include "inx-component.h"

/*****************************************************
 * Multiplex the real inputs according to the index.
 *****************************************************/
#define INXWARE_FB_ID_indexed_mux_real 0x7F07
#define INXWARE_FB_NAME_indexed_mux_real "indexed_mux_real"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(indexed_mux_real); // this relates to C file identify function
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(indexed_mux_real); // this relates to C-file init function
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(indexed_mux_real); //this relates to C-file destroy function
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(indexed_mux_real_mux); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(indexed_mux_real)

#endif /* INXWARE_indexed_mux_real */
