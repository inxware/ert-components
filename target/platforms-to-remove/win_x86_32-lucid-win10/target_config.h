/** @file target_config.h
 * Base Target config to be included by target specific configs
 * INCLUDE AFTER DEFINING OVERRIDE PARAMETERS
 * @author: inx limited
 *
 * Copyright (c) inx Ltd, 2010. All rights reserved.
 */


#ifndef TARGET_CONFIG_H
#define TARGET_CONFIG_H

//Set this to 0 if we don't care what version of windows this will run on
#define NTDDI_WINXP 0

//#define NTDDI_WINXP

//#ifdef EHS_MINGW_i586_MINGW
//#define _WIN32_WINNT 0x0501
//#endif


/* Overrides go here */
#define EHS_CONFIG_DISPLAY_HEIGHT 768	/**< Height for the OSD part of the display */
#define EHS_CONFIG_DISPLAY_WIDTH 1024	/**< Width for the OSD part of the display */
#define EHS_TARGET_DEFAULT_ID "(x86/win32/lvgl)"

#include "../base_full/base_config.h"

#endif /* TARGET_CONFIG_H */
