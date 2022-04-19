/** @file target_config.h
 * In this file, all of the target-specific configuration required by EHS are given.
 * This file is normally used by being included by target.h
 *
 * @author: inx limited
 * @version: $Revision: 1261 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 *
 * Copyright (c) inx limited, 2007. All rights reserved.
 */


#ifndef BASE_CONFIG_H
#define BASE_CONFIG_H


/**
 * Defines the default target identifier for this instance of EHS
 */
#ifndef EHS_TARGET_DEFAULT_ID
#define EHS_TARGET_DEFAULT_ID "(base/device)"
#endif
/* #define EHS_TARGET_EVENT_MODEL */ /**< defined if an external event/interrupt drives the main EHS loop */
/* #define EHS_RTA_TRACE_SUPPORT *//**< Defined if RTA-TRACE is supported by the target */

//@todo all macros should be assigned values so that the override works
#ifndef EHS_TARGET_FP_SUPPORT
#define EHS_TARGET_FP_SUPPORT /**< defined if the target has floating point support */
#endif
/* #define EHS_TARGET_TRACE_SUPPORT */ /**< Defined if the target supports RTA-TRACE (experimental) */
/* #define EHS_CALLBACKS_CONFIGURED */ /**< Defined if the target uses callbacks */
/* #define EHS_TARGET_FIXED_TIMER_RESOLUTION */ /**< Defined if the timer resolution cannot be changed during (or before) init */

/* #define EHS_TARGET_BIGENDIAN */ /**< Defined if the target is bigendian */
#ifndef EHS_MAX_FONTS
#define EHS_MAX_FONTS 10 /**< Maximum number of fonts that can be held on this target at one time */
#endif

#ifndef EHS_CONFIG_MAX_THREADS
#define EHS_CONFIG_MAX_THREADS 25 /**< Maximum number of threads the application supports @todo remove this parameter*/
#endif

#ifndef EHS_CONFIG_MEM_CHECK
//#define EHS_CONFIG_MEM_CHECK	/**< Check for overrun of allocated memory. Takes additional time and memory */
#endif

#ifndef EHS_GROUP_PERIOD_MIN
#define EHS_GROUP_PERIOD_MIN 70 /**< Defines the minimum group period (in system ticks) */
#endif

#ifdef EHS_TOOLKIT_DEPRECATED  //@todo careful that compiler doesn't calculate vales that are overriden
#ifndef EHS_TOOLKIT_DEPRECATED_QTY
#define EHS_TOOLKIT_DEPRECATED_QTY 1
#endif
#else
#ifndef EHS_TOOLKIT_DEPRECATED_QTY
#define EHS_TOOLKIT_DEPRECATED_QTY 0
#endif
#endif
#ifdef EHS_TOOLKIT_SANDBOX
#ifndef EHS_TOOLKIT_SANDBOX_QTY
#define EHS_TOOLKIT_SANDBOX_QTY 1
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
#define EHS_MAX_WIDGET_INSTANCES 1000 /**< Defines the maximum number of widgets that are supported */
#endif

#ifndef EHS_FILE_BUFF_SIZE
#define EHS_FILE_BUFF_SIZE 1024 /**< Size of buffers used to read/write to files */
#endif
/**
 * Defines how allocated memory is to be aligned.
 * 1 is byte aligned, 2 is word aligned, 4 is dword aligned
 */
#ifndef EHS_TGT_MEMORY_ALLOC_ALIGN
#define EHS_TGT_MEMORY_ALLOC_ALIGN 1
#endif
/**
 * Maximum event queue size. Due to implementation approach,
 * this *must* be a power of 2, and less than 2<<15. See EhsEventQueueType
 */
#ifndef EHS_MAX_EVENT_QUEUE_SIZE
#define EHS_MAX_EVENT_QUEUE_SIZE (1u<<13) /* 8k */
#endif
/**
 * Maximum console queue size. This *must* be a power of 2 and less than 2<<31.
 */
#ifndef EHS_MAX_CONSOLE_QUEUE_SIZE
#define EHS_MAX_CONSOLE_QUEUE_SIZE 65536 // - give as preprocessor friendly value for validation  = ((ehs_uint32)((ehs_uint32)(1u)<<16)) /* 64k */
#endif

#ifndef EHS_CONFIG_DISPLAY_HEIGHT
#define EHS_CONFIG_DISPLAY_HEIGHT 768	/**< Height for the OSD part of the display */
#endif

#ifndef EHS_CONFIG_DISPLAY_WIDTH
#define EHS_CONFIG_DISPLAY_WIDTH 1024	/**< Width for the OSD part of the display */
#endif


#ifndef EHS_TIME_BETWEEN_FRAMES
#define EHS_TIME_BETWEEN_FRAMES 40 /*Desired framerate of GTK window*/
#endif


//#ifndef EHS_CONFIG_MEM_CHECK
//#define EHS_CONFIG_MEM_CHECK	/**< Check for overrun of allocated memory. Takes additional time and memory */
//#endif



#ifndef EHS_GROUP_PERIOD_MIN
#define EHS_GROUP_PERIOD_MIN 70 /**< Defines the minimum group period (in system ticks) */
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

/* Canonical System Paths */

/* These are optional overrides for the following defaults */

/* Most likely to change default parameters */


#endif /* TARGET_CONFIG_H */
