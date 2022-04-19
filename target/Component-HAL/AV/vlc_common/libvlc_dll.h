/** @file libvlc_dll.h
 * description
 *
 * @author: inx limited
 * @version: $Revision: 1238 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 *
 * Copyright (c) inx limited, 2006. All rights reserved.
 */

#ifndef EHS_LIBVLC_H
#define EHS_LIBVLC_H


/*****************************************************************************/
/* Included files */

//#include <windows.h>

//#define VLC_PUBLIC_API extern
typedef short bool;
#include <vlc/vlc.h> // angle brackets mean that the depends make file doesn't try and build this.
#include "graphics.h" /* required for the rectangle data type */


/*****************************************************************************/
/* Define macros  */

/**
 * Helper macro for declaring DLL functions.
 */

//#ifdef windows
/*
 * #define DeclareLibFunction(returnType, name, params) \
	extern returnType (*DLL##name) params; \
*/

//#ifdef linux
#define DeclareLibFunction(returnType, name, params) returnType name params;


//#define EHSTV_LIBVLC_PATH "/usr/lib/libvlc.so"

/*****************************************************************************/
/* Declare global variables */


/*****************************************************************************/
/* Declare function prototypes  */


/********************* Declarations of global variables **********************/


EHS_GLOBAL float EhsTV_getPosition(libvlc_media_player_t* pMP);
EHS_GLOBAL void EhsTV_setPosition(libvlc_media_player_t* pMP,float nPos);
EHS_GLOBAL libvlc_time_t EhsTV_getTime(libvlc_media_player_t* pMP);
EHS_GLOBAL void EhsTV_setTime(libvlc_media_player_t* pMP, libvlc_time_t nTime);
EHS_GLOBAL libvlc_time_t EhsTV_getLength(libvlc_media_player_t* pMP);
EHS_GLOBAL float EhsTV_getSpeed(libvlc_media_player_t* pMP);
EHS_GLOBAL void EhsTV_setSpeed(libvlc_media_player_t* pMP,float nSpeed);
EHS_GLOBAL int EhsTV_pause(libvlc_media_player_t* pMP);
EHS_GLOBAL void EhsTV_stop(libvlc_media_player_t* pMP);
EHS_GLOBAL void callback(libvlc_event_t* type, int* input);
EHS_GLOBAL void EhsTV_setIndex(libvlc_media_player_t* pMP,ehs_sint32 nIndex);
EHS_GLOBAL ehs_sint32 EhsTV_getIndex(libvlc_media_player_t* pMP);
EHS_GLOBAL void EhsTV_initVlc(const EhsGraphicsRectangleClass* pBounds);

/**
 * Display resolution
 */
//#define EHS_TV_RES_X (544)
//#define EHS_TV_RES_Y (288)
#endif /*EHS_LIBVLC_H*/

