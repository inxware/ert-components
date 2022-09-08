/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

/** @file target.h
 * This file defines the core hardware abstraction layer for functions required by EHS.
 *
 * @author: inx limited
 *
 */

#ifndef EHS_TARGET_H
#define EHS_TARGET_H

/* target specific details -- include these first */
#include "target_config.h"
#include "target_types.h"
#include "target_specific.h"
#include "hal-api.h"

//#include "hal_time.h"
//#include "hal_file.h"
//#include "hal_console.h"
//#include "hal_process.h"



/* generic stuff */
#include "ehs_types.h"
//@todo PPP: is this legacy:
#ifdef EHS_GUI_SUPPORT
//#include "graphics.h"
#endif

#endif
