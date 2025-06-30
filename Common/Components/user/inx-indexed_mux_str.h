#ifndef INXWARE_indexed_mux_str
#define INXWARE_indexed_mux_str
#include "inx-component.h"

/*****************************************************
 * Multiplex the string inputs according to the index.
 *****************************************************/
#define INXWARE_FB_ID_indexed_mux_str 0xDADA
#define INXWARE_FB_NAME_indexed_mux_str "indexed_mux_str"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(indexed_mux_str); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(indexed_mux_str); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(indexed_mux_str); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(indexed_mux_str_mux); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(indexed_mux_str)

#endif /* INXWARE_indexed_mux_str */
