#ifndef INXWARE_console_print
#define INXWARE_console_print
#include "inx-component.h"

/*****************************************************
 * Prints string to the operating systems console (if available)
 *****************************************************/
#define INXWARE_FB_ID_console_print 0xF458
#define INXWARE_FB_NAME_console_print "console_print"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(console_print); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(console_print); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(console_print); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(console_print_PrintFunc); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(console_print)

#endif /* INXWARE_console_print */
