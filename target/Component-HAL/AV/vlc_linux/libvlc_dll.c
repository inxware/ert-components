/** @file libvlc_dll.c
 * Imports all of the libvlc functions from the libvlc DLL
 *
 * @author: inx limited
 * @version: $Revision: 1238 $
 * @date: $Date$
 *
 * Copyright (c) inx limited, 2007. All rights reserved.
 */

/**
 * @page Verification Verification report
 * @section hal_graphics_font
 * @anchor hal_graphics_font
 * @subsection misra MISRA compliance:
 * test.c demonstrated MISRA compliant on
 * Last modified on $Date:$
 *
 * This file contained no derogations to the MISRA standard.
 */

/*****************************************************************************/
/* Included files */
#include "messages.h"
#include "hal_string.h"
#include "hal_time.h"
#include "libvlc_dll.h"
#include <X11/Xlib.h>
#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include "graphics.h"
#include "widget.h"
#include "hal_process.h"

/*****************************************************************************/
/* Declare macros and local typedefs used by this file */

/**
 * Helper macro for defining DLL functions.
 */

/*
 #define DefineLibFunction(returnType, name, params) \
	returnType (*DLL##name) params; \
	typedef returnType (*name##Type) params;
 */

#define DefineLibFunction(x,y,z)

/*****************************************************************************/
/* Declare prototypes of local functions */

/*****************************************************************************/
/* Variables defined with file-scope */

/**
 * Video subpicture used for displaying EHS graphics
 */
//EHS_LOCAL subpicture_t* EhsL_pSubpic;

/**
 * Video output thread
 */
//EHS_LOCAL vout_thread_t* EhsL_pVout;

/*****************************************************************************/
/* Variables defined with global-scope */




/* @todo This function also moves the video window if it has laready been initialised - this is more confusing than just having a new function just for moving */
void EhsTV_initVlc(const EhsGraphicsRectangleClass* pBounds)
{	


}

/*****************************************************************************/
/* Local  Function definitions */

/**
 * Test for an exception from libvlc functions, and report the
 * exception if it occurs.
 */

