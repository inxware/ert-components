/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file targetos_init.c
 * Initialization/reset/termination code for the OS
 *
 * @author: inx limited
 *
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
#define EHS_TARGET_CODE /* Ensure header files include target-internal values */
#include "targetgfx_init.h"
#include "blockref_table.h"
#include "hal_viewport.h"
#include "messages.h"

#ifdef EHS_MV_SUPPORT__opencv
#include <stdlib.h>      /* malloc, free */
#include "lvgl/lvgl.h"
#include "hal_mv.h"
#include "hal_process.h"

/* -----------------------------------------------------------------------
 * LVGL camera canvas — renders camera frames inside the LVGL scene.
 *
 * Created lazily on the first frame_show call; re-created if the
 * requested position or size changes.  The buffer is owned by this
 * module and released on application reset so the next SODL run starts
 * with a clean state.
 * --------------------------------------------------------------------- */
static lv_obj_t*   g_cam_canvas = NULL;
static lv_color_t* g_cam_buf    = NULL;
static ehs_sint32  g_cam_x = -1, g_cam_y = -1, g_cam_w = 0, g_cam_h = 0;

static void lvgl_camera_frame_renderer(
    ehs_sint32 x, ehs_sint32 y, ehs_sint32 dst_w, ehs_sint32 dst_h,
    const void* data, ehs_sint32 frame_w, ehs_sint32 frame_h, ehs_sint32 channels)
{
    if (!data || dst_w <= 0 || dst_h <= 0 || frame_w <= 0 || frame_h <= 0) return;
    if (channels != 3 && channels != 1) return; /* BGR and grayscale only */

    EhsTPMutex_lock(EhsTPMutex_viewport);

    /* (Re)create canvas when position or size changes */
    if (!g_cam_canvas || g_cam_x != x || g_cam_y != y ||
        g_cam_w != dst_w || g_cam_h != dst_h) {
        if (g_cam_canvas) { lv_obj_del(g_cam_canvas); g_cam_canvas = NULL; }
        if (g_cam_buf)    { free(g_cam_buf); g_cam_buf = NULL; }

        size_t buf_sz = (size_t)LV_CANVAS_BUF_SIZE_TRUE_COLOR(dst_w, dst_h);
        g_cam_buf = (lv_color_t*)malloc(buf_sz);
        if (!g_cam_buf) { EhsTPMutex_unlock(EhsTPMutex_viewport); return; }
        memset(g_cam_buf, 0, buf_sz);

        g_cam_canvas = lv_canvas_create(lv_scr_act());
        lv_canvas_set_buffer(g_cam_canvas, g_cam_buf,
                             (lv_coord_t)dst_w, (lv_coord_t)dst_h,
                             LV_IMG_CF_TRUE_COLOR);
        lv_obj_set_pos(g_cam_canvas, (lv_coord_t)x, (lv_coord_t)y);
        lv_obj_move_background(g_cam_canvas); /* stays behind UI widgets */
        lv_obj_set_style_pad_all(g_cam_canvas, 0, 0);
        lv_obj_set_style_border_width(g_cam_canvas, 0, 0);
        g_cam_x = x; g_cam_y = y; g_cam_w = dst_w; g_cam_h = dst_h;
    }

    /* Nearest-neighbour scale + BGR→lv_color_t conversion */
    const ehs_uint8* src = (const ehs_uint8*)data;
    ehs_sint32 cy, cx;
    for (cy = 0; cy < dst_h; cy++) {
        ehs_sint32 fy = cy * frame_h / dst_h;
        for (cx = 0; cx < dst_w; cx++) {
            ehs_sint32 fx = cx * frame_w / dst_w;
            const ehs_uint8* px = src + (fy * frame_w + fx) * channels;
            lv_color_t color = (channels == 3)
                ? lv_color_make(px[2], px[1], px[0])    /* BGR → RGB */
                : lv_color_make(px[0], px[0], px[0]);   /* grayscale  */
            g_cam_buf[cy * dst_w + cx] = color;
        }
    }
    lv_obj_invalidate(g_cam_canvas);

    EhsTPMutex_unlock(EhsTPMutex_viewport);
}
#endif /* EHS_MV_SUPPORT__opencv */

/*****************************************************************************/
/* Declare macros and local typedefs used by this file */

/*****************************************************************************/
/* Declare prototypes of local functions */

/*****************************************************************************/
/* Variables defined with file-scope */

/*****************************************************************************/
/* Variables defined with global-scope */

extern EhsBlockRefType EhsBlockRefTable_Dtv[];


/*****************************************************************************/
/* Function definitions */

/**
 * Perform necessary Operating system setup upon system initialisation, early call - too early to call EhsTV_init
 */
void EhsTGfxSys_init()
{
    // Use EhsL_viewport as the Mutex lock before and after the LVGL function usages
    EHSH_LOG_INFO("early GFX init");
    if (1)
    {
        if (!EhsTV_init(&EhsTV))
        {
            exit(1);
        }
    }
#ifdef EHS_MV_SUPPORT__opencv
    EhsCameraFrameRegisterEmbeddedRenderer(lvgl_camera_frame_renderer);
#endif
}

/**
 * Perform necessary Operating system setup upon system initialisation, late call - ok to call EhsTV_init now
 * OS graphics handles data passed in here to be used by rendering functions
 */
void EhsTGfxSys_initLate(void * userdata)
{
 //   EHSH_LOG_INFO("late GFX init");
 //   struct EhsTVStruct* pViewport = &EhsTV;
 //   pViewport->engine = userdata;
  //  if (!EhsTV_init(&EhsTV))
  //  {
   //     EHSH_LOG_ERROR("Target viewport initialisation failed");
   //     exit(1);
   // }
}

/**
 * Called to shutdown the target prior to shutting down EHS
 */
void EhsTGfxSys_term(void)
{
}

/**
 * Perform setup before loading in a new application
 */
void EhsTGfxApp_init(void)
{
#ifdef EHS_MV_SUPPORT__opencv
    /* LVGL destroys all objects between app runs — the canvas pointer is now
     * stale.  Reset state so lvgl_camera_frame_renderer recreates it fresh. */
    g_cam_canvas = NULL;
    if (g_cam_buf) { free(g_cam_buf); g_cam_buf = NULL; }
    g_cam_x = -1; g_cam_y = -1; g_cam_w = 0; g_cam_h = 0;
#endif
}

/**
 * Called to clean up after the execution of an application in order to make
 * sure everything has been stopped.
 */
void EhsTGfxApp_term(void)
{
    //EhsTV_reset(&EhsTV); //not generic - moved to common code
}


/**
 * Perform necessary target setup per application execution
 */
void EhsTGfxApp_reset(void)
{
}
