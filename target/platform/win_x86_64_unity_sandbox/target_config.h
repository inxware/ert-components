/** @file target_config.h
 * Base Target config to be included by target specific configs
 * INCLUDE AFTER DEFINING OVERRIDE PARAMETERS
 * @author: inx limited
 *
 * Copyright (c) inx Ltd, 2010. All rights reserved.
 */


#ifndef TARGET_CONFIG_H
#define TARGET_CONFIG_H

#define NTDDI_WINXP 0
//#define _WIN32_WINNT 0x0501

/* Overrides go here */
#define EHS_CONFIG_DISPLAY_HEIGHT 768	/**< Height for the OSD part of the display */
#define EHS_CONFIG_DISPLAY_WIDTH 1024	/**< Width for the OSD part of the display */
#define EHS_TARGET_DEFAULT_ID "(x86/win32/Unity)"
#ifndef EHS_DEBUG_TCPIP_CONSOLE
#define EHS_DEBUG_TCPIP_CONSOLE
#endif

#include "../base_full/base_config.h"

#endif /* TARGET_CONFIG_H */
