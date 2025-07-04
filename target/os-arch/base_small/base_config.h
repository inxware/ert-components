/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file base_config.h
 * In this file, all of the base-specific configuration required by EHS are given.
 * This file is normally used by being included by target.h
 *
 * @author: inx limited
 *
 */

/* IMPORTANT THIS MUST THE SAME VALUES BETWEEEN EHS-KERNEL AND ert-components curently*/
//todo2022 remove the need for this inEHS kernel so the config comes from ert-components.



#ifndef BASE_CONFIG_H
#define BASE_CONFIG_H

//This is probablt the ultr-tiny ehs file system path length restriction if we are using this file system
#ifndef EHS_FILENAME_SIZE
#define EHS_FILENAME_SIZE 32
#endif

#ifndef EHS_STRING_LENGTH_MAX
#define EHS_STRING_LENGTH_MAX 256
#endif

#ifndef EHS_MAXPATHLENGTH
#define EHS_MAXPATHLENGTH 128
#endif

#ifndef EHS_SYS_MAXPATHLENGTH
#define EHS_SYS_MAXPATHLENGTH 128
#endif

#ifndef EHS_MAX_URL_LENGTH
#define EHS_MAX_URL_LENGTH 128
#endif

#ifndef EHS_TD_FILES_MAX_PATH
#define EHS_TD_FILES_MAX_PATH 128
#endif

#ifndef EHS_TD_FILES_MAX_FILENAME
#define EHS_TD_FILES_MAX_FILENAME 32
#endif

#ifndef EHS_DEVMAN_FILE_PATH_LENGTH
#define EHS_DEVMAN_FILE_PATH_LENGTH 128
#endif

#ifndef EHS_DATA_TABLE_STRING_DEFAULT_LENGTH
#define EHS_DATA_TABLE_STRING_DEFAULT_LENGTH EHS_STRING_LENGTH_MAX
#endif

#if EHS_DATA_TABLE_STRING_DEFAULT_LENGTH < EHS_STRING_LENGTH_MAX
#error "EHS_DATA_TABLE_STRING_DEFAULT_LENGTH is smaller than EHS_STRING_LENGTH_MAX, is there something wrong?"
#endif

#ifndef EHS_MAXDEVMANNAMELEN
#define EHS_MAXDEVMANNAMELEN EHS_STRING_LENGTH_MAX
#endif

#if EHS_STRING_LENGTH_MAX != EHS_MAXDEVMANNAMELEN
#error "EHS_MAXDEVMANNAMELEN must be the same as EHS_STRING_LENGTH_MAX"
#endif


#ifndef EHS_IP_ADDR_LENGTH_MAX
#define EHS_IP_ADDR_LENGTH_MAX 32
#endif

#ifndef EHS_NET_MAC_ID_LENGTH_MAX
#define EHS_NET_MAC_ID_LENGTH_MAX 32
#endif

#ifndef EHS_DEVICE_ID_LENGTH_MAX
#define EHS_DEVICE_ID_LENGTH_MAX 32
#endif

#ifndef EHS_INST_ROOT_DIR_LENGTH_MAX
#define EHS_INST_ROOT_DIR_LENGTH_MAX 16
#endif

#ifndef EHS_APPS_DIR_LENGTH_MAX
#define EHS_APPS_DIR_LENGTH_MAX 16
#endif

#ifndef EHS_USER_DIR_LENGTH_MAX
#define EHS_USER_DIR_LENGTH_MAX 16
#endif

#ifndef EHS_VERSION_LENGTH_MAX
#define EHS_VERSION_LENGTH_MAX 128
#endif

#ifndef EHS_BUILD_DATE_LENGTH_MAX
#define EHS_BUILD_DATE_LENGTH_MAX 64
#endif

#ifndef EHS_START_DATE_LENGTH_MAX
#define EHS_START_DATE_LENGTH_MAX 64
#endif

#ifndef EHS_TARGET_VARIANT_LENGTH_MAX
#define EHS_TARGET_VARIANT_LENGTH_MAX 64
#endif

#ifndef EHS_MODULE_LIST_LENGTH_MAX
#define EHS_MODULE_LIST_LENGTH_MAX 64
#endif

/* on small devices optimise Widget structure for memory */
#define EHS_OPTIMIZE_WIDGET_MEM 1

/* no need to create buffer for the os version */
#define EHS_EXCLUDE_OS_VERSION 1

/**
 * Defines the default target identifier for this instance of EHS
 */

//@todo all macros should be assigned values so that the override works
#ifndef EHS_TARGET_FP_SUPPORT
#define EHS_TARGET_FP_SUPPORT /**< defined if the target has floating point support */
#endif
/* #define EHS_TARGET_TRACE_SUPPORT */ /**< Defined if the target supports RTA-TRACE (experimental) */
/* #define EHS_CALLBACKS_CONFIGURED */ /**< Defined if the target uses callbacks */
/* #define EHS_TARGET_FIXED_TIMER_RESOLUTION */ /**< Defined if the timer resolution cannot be changed during (or before) init */

/* #define EHS_TARGET_BIGENDIAN */ /**< Defined if the target is bigendian */
#ifndef EHS_MAX_FONTS
#define EHS_MAX_FONTS 0 /**< Maximum number of fonts that can be held on this target at one time */
#endif




/* Warning! these must match the kernel values (until they can be published to the kernel */
#ifdef EHS_TOOLKIT_DEPRECATED  //@todo careful that compiler doesn't calculate vales that are overriden
#ifndef EHS_TOOLKIT_DEPRECATED_QTY
#define EHS_TOOLKIT_DEPRECATED_QTY 0
#endif
#else
#ifndef EHS_TOOLKIT_DEPRECATED_QTY
#define EHS_TOOLKIT_DEPRECATED_QTY 0
#endif
#endif
#ifdef EHS_TOOLKIT_SANDBOX
#ifndef EHS_TOOLKIT_SANDBOX_QTY
#define EHS_TOOLKIT_SANDBOX_QTY 0
#endif
#else
#ifndef EHS_TOOLKIT_SANDBOX_QTY
#define EHS_TOOLKIT_SANDBOX_QTY 0
#endif
#endif

#ifndef EHS_MAX_TOOLKITS
#define EHS_MAX_TOOLKITS (10+EHS_TOOLKIT_DEPRECATED_QTY+EHS_TOOLKIT_SANDBOX_QTY) /**< Defines the maximum number of toolkits that EHS supports */
#endif

#ifndef EHS_MAX_WIDGET_INSTANCES
#define EHS_MAX_WIDGET_INSTANCES 100 /**< Defines the maximum number of widgets that are supported */
#endif

#ifndef EHS_FILE_BUFF_SIZE
#define EHS_FILE_BUFF_SIZE 256 /**< Size of buffers used to read/write to files */
#endif

/**
 * Maximum console queue size. This *must* be a power of 2 and less than 2<<31.
 */
#ifndef EHS_DEBUG_CONSOLE_BUFFER_SIZE
#define EHS_DEBUG_CONSOLE_BUFFER_SIZE (1u<<9)  //512 bytes 256 - give as preprocessor friendly value for validation  = ((ehs_uint32)((ehs_uint32)(1u)<<16)) /* 64k */
#endif

#ifndef EHS_CONFIG_DISPLAY_HEIGHT
#define EHS_CONFIG_DISPLAY_HEIGHT 0	/**< Height for the OSD part of the display */
#endif

#ifndef EHS_CONFIG_DISPLAY_WIDTH
#define EHS_CONFIG_DISPLAY_WIDTH 0	/**< Width for the OSD part of the display */
#endif


#ifndef EHS_TIME_BETWEEN_FRAMES
#define EHS_TIME_BETWEEN_FRAMES 40 /*Desired framerate of GTK window*/
#endif

/*Amount of memory to initially let SVG player have*/
/*
#ifndef EHS_SVG_MEMORY_INIT_SIZE
#define EHS_SVG_MEMORY_INIT_SIZE 1310720
#endif
*/

/*Indicates target's X server is capable of compositing/transparency effects*/
#ifndef COMPOSITING_AVAILABLE //@todo should this be switched off?
#define COMPOSITING_AVAILABLE
#endif

/* Thread priorities */

#ifndef EHS_PRI_EHS_MAIN
#define EHS_PRI_EHS_MAIN                   18
#endif

#ifndef EHS_PRI_TCP_IP_CONSOLE
#define EHS_PRI_TCP_IP_CONSOLE              0
#endif

#ifndef EHS_PRI_SERIAL_CMD
#define EHS_PRI_SERIAL_CMD                  0
#endif

#ifndef EHS_PRI_LVGL_TICK
#define EHS_PRI_LVGL_TICK                  19
#endif

#ifndef EHS_PRI_LVGL_GUI
#define EHS_PRI_LVGL_GUI                    0
#endif

#ifndef EHS_PRI_MCU_SLOW_HP_THR
#define EHS_PRI_MCU_SLOW_HP_THR            17
#endif

#ifndef EHS_PRI_MCU_SLOW_LP_THR
#define EHS_PRI_MCU_SLOW_LP_THR             1
#endif

#ifndef EHS_PRI_UART
#define EHS_PRI_UART                       12
#endif

#ifndef EHS_PRI_MODBUS_MASTER_CTRL
#define EHS_PRI_MODBUS_MASTER_CTRL          2
#endif

/* Canonical System Paths */

/* These are optional overrides for the following defaults */

/* Most likely to change default parameters */

#endif /* TARGET_CONFIG_H */
