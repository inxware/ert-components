#ifndef INXWARE_audio_input_level
#define INXWARE_audio_input_level
#include "inx-component.h"

/*****************************************************
 * This function block gets the audio input amplitude. The sampling frequency, bit rate and output interval period can be defined.
 *****************************************************/
#define INXWARE_FB_ID_audio_input_level 0xC5BF
#define INXWARE_FB_NAME_audio_input_level "audio_input_level"
/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(audio_input_level); // this relates to C file identify function
EHS_GLOBAL EHS_FB_INIT_FUNCTION(audio_input_level); // this relates to C-file init function
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(audio_input_level); //this relates to C-file destroy function
EHS_GLOBAL EHS_FB_RUN_FUNCTION(audio_input_level_start); //a function executed by a start port
EHS_GLOBAL EHS_FB_RUN_FUNCTION(audio_input_level_destroy); //a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(audio_input_level)

#endif /* INXWARE_audio_input_level */
