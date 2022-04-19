/** @file target.h
 * This file defines the core hardware abstraction layer for functions required by EHS.
 * 
 * @author: inx limited
 * @version: $Revision: 2186 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 * 
 * Copyright (c) inx limited, 2006. All rights reserved.
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
