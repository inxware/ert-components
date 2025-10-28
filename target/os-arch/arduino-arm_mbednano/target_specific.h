/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file target_specific.h
 * In this file, all of the hardware abstraction layer functions provided for EHS are given.
 *
 * @author: inx limited
 *
 */

#ifndef EHS_TARGET_SPECIFIC_H
#define EHS_TARGET_SPECIFIC_H

#ifndef EHS_GLOBALS_H
#error "This file should only be included by globals.h" 
#endif

#include "target_arduino_all.h"


#include <mbed_config.h>

/**
 * Forces the declared variable to be aligned in the way specified by EHS_TGT_MEMORY_ALLOC_ALIGN
 */
#define EHS_TGT_MEMORY_ALIGNED(type,name) type name

#define vTaskDelay(x) delay(x)


#endif /* EHS_TARGET_SPECIFIC_H */
