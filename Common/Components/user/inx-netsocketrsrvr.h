#ifndef INXWARE_netsocketsrvr
#define INXWARE_netsocketsrvr
#include "inx-component.h"

/*****************************************************
 * Listens on specified port
 *****************************************************/
#define INXWARE_FB_netsocketsrvr "netsocketsrvr"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(netsocketsrvr); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(netsocketsrvr); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(netsocketsrvr); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(netsocketsrvr_open); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(netsocketsrvr_close); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(netsocketsrvr_sendData); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(netsocketsrvr)

#endif /* INXWARE_netsocketsrvr */
