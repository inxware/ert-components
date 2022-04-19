/*
 * Filename : BFMixPixel.h
 *
 *  Version : 6.4.0_2271s
 *     Date : 20080508-20.49.08
 *                
 *
 * (c) COPYRIGHT 2002- 2008 Open Text Corp. - BitFlash Division. ALL RIGHTS RESERVED
 *
 *
 * This software is the property and confidential information of Open Text and is furnished under
 * license by BitFlash Inc.  This software may be used only in accordance with the terms of said license.  
 *
 * This software may not be copied, transmitted, provided to or otherwise made available to any other person, 
 * company, corporation or other entity except as specified in the terms of said license.
 *
 * This copyright notice may not be removed or altered without the prior written permission of Open Text.
 *
 *
 */
/** \file BFMixPixel.h
 *
 * 
 * This file specifies the color system struct and associated types and #defines.
 *
 * Copyright ©2000-2002 BitFlash Graphics Inc. All rights reserved.
 */

#ifndef BFMIX_PIXEL_H
#define BFMIX_PIXEL_H

#ifdef __cplusplus
BF_BEGIN_EXTERN_C
#endif

/* ******************************************************************
    Color
   *******************************************************************/

/* The OpenVG rendering engine supports only 32 bit color. 16 bit is left here as a placeholder if
 * other depths in fact will have to be supported at compile time (we can trace where changes are
 * required)
 */
#if (BF_DEVICE_COLOR_DEPTH == 16)

typedef BF_UINT16   BFPixel;

#elif (BF_DEVICE_COLOR_DEPTH == 32)

typedef BF_UINT32   BFPixel;

#else

#error Unsupported buffer pixel depth

#endif  /* BF_DEVICE_COLOR_DEPTH */


BF_PLAYER_IMPORT BFPixel BFmixPixel( BFPlayer *pPlayer, BFPixel background, BFPixel forground, BF_UINT8 uAlpha );

#ifdef __cplusplus
BF_END_EXTERN_C
#endif

#endif

