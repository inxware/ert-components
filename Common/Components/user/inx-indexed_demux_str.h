#ifndef INXWARE_indexed_demux_str
#define INXWARE_indexed_demux_str
#include "inx-component.h"

/*****************************************************
 * Demultiplex the string according to the input index
 *****************************************************/
#define INXWARE_FB_ID_indexed_demux_str 0x6CA8
#define INXWARE_FB_NAME_indexed_demux_str "indexed_demux_str"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(indexed_demux_str); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(indexed_demux_str); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(indexed_demux_str); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(indexed_demux_str_demux); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(indexed_demux_str)

#endif /* INXWARE_indexed_demux_str */
