#ifndef INXWARE_indexed_demux_real
#define INXWARE_indexed_demux_real
#include "inx-component.h"

/*****************************************************
 * Demultiplex the floating point number according to the input index
 *****************************************************/
#define INXWARE_FB_ID_indexed_demux_real 0x5A31
#define INXWARE_FB_NAME_indexed_demux_real "indexed_demux_real"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(indexed_demux_real); // this relates to C file identify function
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(indexed_demux_real); // this relates to C-file init function
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(indexed_demux_real); //this relates to C-file destroy function
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(indexed_demux_real_demux); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(indexed_demux_real)

#endif /* INXWARE_indexed_demux_real */
