#ifndef INXWARE_indexed_mux_int
#define INXWARE_indexed_mux_int
#include "inx-component.h"

/*****************************************************
 * Multiplex the integer inputs according to the index.
 *****************************************************/
#define INXWARE_FB_ID_indexed_mux_int 0x7F70
#define INXWARE_FB_NAME_indexed_mux_int "indexed_mux_int"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(indexed_mux_int); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(indexed_mux_int); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(indexed_mux_int); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(indexed_mux_int_mux); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(indexed_mux_int)

#endif /* INXWARE_indexed_mux_int */
