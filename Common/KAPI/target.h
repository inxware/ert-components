/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file target.h
 * This file defines the core hardware abstraction layer for functions required by EHS.
 *
 * @author: inx limited
 *
 */

#ifndef EHS_TARGET_H
#define EHS_TARGET_H

/* 
Include target types first so that overrides can be avoided 
*/
#include "target_types.h"
/* 
Include anything defined for each platform/<TARGET>/target_config.h 
*/
#include "target_config.h"
/* 
Include the general inxware ert/kernel types 
*/
#include "ehs_types.h"
/*
 Include functions that are only available in some targets or implemented differently.
*/
#include "target_specific.h"

#endif
