#ifndef INXWARE_FrameReadFromFile
#define INXWARE_FrameReadFromFile
#include "inx-component.h"

/*****************************************************
 * Reads frame from a file
 *****************************************************/
#define INXWARE_FB_ID_FrameReadFromFile 0x5772
#define INXWARE_FB_NAME_FrameReadFromFile "FrameReadFromFile"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(FrameReadFromFile); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(FrameReadFromFile); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(FrameReadFromFile); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(FrameReadFromFile_run_read_file); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(FrameReadFromFile)

#endif /* INXWARE_FrameReadFromFile */
