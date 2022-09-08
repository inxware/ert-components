/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

/** @file hal.h
 * Declares the interface for the general hal functions.
 *
 * @author: inx limited
 *
 */

#ifndef EHS_HAL_API_H
#define EHS_HAL_API_H

/*****************************************************************************/
/* Included files */
#include "globals.h"
#include "ehs_types.h"

/* General Component HAL API for core support */
#include "hal.h"
#include "hal_mem.h"
#include "hal_logger.h"
#include "hal_time.h"
#include "hal_process.h"
#include "hal_xml_minimal.h"
#include "hal_file.h"
#include "hal_string.h"
#include "hal_appstorage.h"

/* Kernel APIs that can be used */
#include "callback_queue.h"
#include "app_data.h"
/**
 * Flag to indicate loading of new sodl so that threads MUST be terminated (Owned by app_data.c in the kernel)
 */
//EHS_GLOBAL ehs_bool* bNewSodlFlagRef;

/* Component support specific */
#include "hal_media.h"

#include "messages.h"
#include "inx-parameters.h"

#ifdef  EHS_DEBUG_TCPIP_CONSOLE
#include "hal_console.h"
#endif
#ifdef EHS_WEBKIT_SUPPORT
#include "hal_webkit.h"
#endif
#ifdef EHS_LUA_SUPPORT
#include "hal_lua.h"
#endif
#ifdef EHS_NETWORKING_SUPPORT// do we have URL support?
#include "hal_network.h"
#include "hal_url.h"
#include "hal_devapps.h"
#endif
#ifdef EHS_DEVMAN_SUPPORT
#include "hal_devman.h"
#endif
#ifdef EHS_GUI_SUPPORT
#include "hal_viewport.h" /* This is for the GTK home key feature */
#endif


#endif /* EHS_HAL_H */
