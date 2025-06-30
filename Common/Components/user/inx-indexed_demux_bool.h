#ifndef INXWARE_indexed_demux_bool
#define INXWARE_indexed_demux_bool
#include "inx-component.h"

/*****************************************************
 * Demultiplex the boolean according to the input index
 *****************************************************/
#define INXWARE_FB_ID_indexed_demux_bool 0xF811
#define INXWARE_FB_NAME_indexed_demux_bool "indexed_demux_bool"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(indexed_demux_bool); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(indexed_demux_bool); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(indexed_demux_bool); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(indexed_demux_bool_demux); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(indexed_demux_bool)

#endif /* INXWARE_indexed_demux_bool */
