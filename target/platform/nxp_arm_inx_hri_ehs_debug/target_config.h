/** @file target_config.h
 * Base Target config to be included by target specific configs
 * INCLUDE AFTER DEFINING OVERRIDE PARAMETERS
 * @author: Pierre
 * @version: $Revision: 1245 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 *
 * Copyright (c) inx Ltd, 2010. All rights reserved.
 */


#ifndef TARGET_CONFIG_H
#define TARGET_CONFIG_H

#define EHS_TARGET_EVENT_MODEL 1
#define EHS_MAX_CONSOLE_QUEUE_SIZE 128u
#define EHS_TARGET_EVENT_MODEL 1
#define EHS_STRING_LENGTH_MAX 64
//seems to only make 8 bytes at a time difference
#define EHS_MAX_TOOLKITS 3
#define EHS_MAX_WIDGET_INSTANCES 0
#define EHS_FILE_BUFF_SIZE 256u
#define EHSL_BUFFSIZE 4092
#define xEHS_GUI_SUPPORT 1
#define EHS_MQTT_SUPPORT 1
#include "../base_full/base_config.h"
#undef EHS_GUI_SUPPORT
#endif /* TARGET_CONFIG_H */