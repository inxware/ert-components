#ifndef INXWARE_wifi_station
#define INXWARE_wifi_station
#include "inx-component.h"

/*****************************************************
 * The device works as a Wi-Fi station to connect to an access point.
This function block supports WPA2-PSK, open network and WPA2/3-Enterprise.
 *****************************************************/
#define INXWARE_FB_ID_wifi_station 0xED92
#define INXWARE_FB_NAME_wifi_station "wifi_station"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(wifi_station); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(wifi_station); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(wifi_station); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(wifi_station_do_connect); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(wifi_station_do_disconnect); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(wifi_station_internal_thread); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(wifi_station)

#endif /* INXWARE_wifi_station */
