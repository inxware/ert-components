/** @file target_config.h
 * Base Target config to be included by target specific configs
 * INCLUDE AFTER DEFINING OVERRIDE PARAMETERS
 * @author: inx limited
 *
 * Copyright (c) inx Ltd, 2010. All rights reserved.
 */


#ifndef TARGET_CONFIG_WITHTOOLSH
#define TARGET_CONFIG_WITHTOOLSH

#include "../linux_x86_gtk_gst/target_config.h"

/* Overrides go here */
//#define EHS_CONFIG_DISPLAY_HEIGHT 768	/**< Height for the OSD part of the display */
//#define EHS_CONFIG_DISPLAY_WIDTH 1024	/**< Width for the OSD part of the display */
//#define EHS_TARGET_DEFAULT_ID "(x86/Linux/gtk)"

#define EHS_DEBUG_TCPIP_CONSOLE

#endif /* TARGET_CONFIG_H */
