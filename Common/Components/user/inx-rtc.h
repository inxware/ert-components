#ifndef INXWARE_rtc
#define INXWARE_rtc
#include "inx-component.h"

/*****************************************************
 * rtc
 *****************************************************/
#define INXWARE_FB_rtc "rtc"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(rtc); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(rtc); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(rtc); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(rtc_set); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(rtc_read); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(rtc)

#endif /* INXWARE_rtc */
