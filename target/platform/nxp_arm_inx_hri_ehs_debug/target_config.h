/** @file target_config.h
 * Base Target config to be included by target specific configs
 * INCLUDE AFTER DEFINING OVERRIDE PARAMETERS
 * @author: inx limited
 *
 * Copyright (c) inx Ltd, 2010. All rights reserved.
 */


#ifndef TARGET_CONFIG_H
#define TARGET_CONFIG_H

#define EHS_MAX_CONSOLE_QUEUE_SIZE 128u
#define EHS_STRING_LENGTH_MAX 64
//seems to only make 8 bytes at a time difference
#define EHS_MAX_TOOLKITS 3
#define EHS_MAX_WIDGET_INSTANCES 0
#define EHS_FILE_BUFF_SIZE 256u
#define EHSL_BUFFSIZE 4092
#define xEHS_GUI_SUPPORT 1
#include "../base_full/base_config.h"
#undef EHS_GUI_SUPPORT
#endif /* TARGET_CONFIG_H */
