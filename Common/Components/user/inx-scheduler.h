#ifndef INXWARE_DevmanScheduler
#define INXWARE_DevmanScheduler
#include "inx-component.h"

/*****************************************************
 * This function will retrieve data from the schedule information stored on a device. 
This will be based on the NXP binary format for now, but doesn’t need to be specific to this.

 *****************************************************/
#define INXWARE_FB_ID_DevmanScheduler 0x142C
#define INXWARE_FB_NAME_DevmanScheduler "DevmanScheduler"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(DevmanScheduler); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(DevmanScheduler); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(DevmanScheduler); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DevmanScheduler_init_schedule); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DevmanScheduler_get_schedule); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(DevmanScheduler_set_schedule); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(DevmanScheduler)

#endif /* INXWARE_DevmanScheduler */
