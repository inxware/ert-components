#ifndef INXWARE_netSocket
#define INXWARE_netSocket
#include "inx-component.h"

/*****************************************************
 * provides tcpip and udp socket data comms.
 *****************************************************/
#define INXWARE_FB_netSocket "netSocket"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(netSocket); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(netSocket); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(netSocket); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(netSocket_open); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(netSocket_close); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(netSocket_send); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(netSocket)

#endif /* INXWARE_netSocket */
