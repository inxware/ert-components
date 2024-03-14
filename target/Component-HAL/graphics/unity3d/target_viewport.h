/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

/** @file target_viewport.h
 * The target-specific declarations required to support the HAL for viewport services
 * are defined here. This file should only be included by hal_viewport.h
 *
 * unity3d version
 * 
 * @author: inx limited
 *
 */

#ifndef EHS_TARGET_VIEWPORT_H
#define EHS_TARGET_VIEWPORT_H


#define EHS_TV_ALPHA8_SCALE(x) ((x) >> 1)

/**
 * Provide access to pixels representing the surface
 */
//#define EhsTVSurface_pixels(pSurface) ((EhsGraphicsColourClass*) (NULL) )

/**
 * Provide access to the bitmap representing the surface
 */
//#define EhsTVSurface_bitmap(pSurface) ((ehs_uint8*) ( NULL ) )


// nothing to do here as Unity is not used for standard GUI function bloclks currently nad has it's own.

#endif