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

/* Must precede every #include below (even transitively) — hal_logger.h locks
 * EHSH_LOG_CHECK to a no-op literal 0 if included before this is defined. */
#define EHSL_MODULE_ID EHSH_LOG_MODULE_GRAPHICS

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
#include "hal_logger.h"

/* -----------------------------------------------------------------------
 * LVGL camera canvas — renders camera frames inside the LVGL scene.
 *
 * One canvas + backing buffer per calling FB instance, not a single shared
 * one: multiple frame_show instances can be embedding simultaneously (e.g.
 * a raw feed and an annotated one side by side), and sharing a single
 * canvas means each one's render clobbers the other's ("flicker, only one
 * renders at a time"). Ownership is the caller's: *pHandle is NULL on that
 * instance's first call, this allocates a canvas for it and stores the
 * pointer back through *pHandle; the instance passes the same handle back
 * on every later call. lvgl_camera_frame_renderer_release() is called once
 * by the owning instance's destroy function.
 * --------------------------------------------------------------------- */
typedef struct {
    lv_obj_t*   canvas;
    lv_color_t* buf;
    ehs_sint32  w, h;
} EhsCamEmbedCanvas_t;

static void lvgl_camera_frame_renderer(
    void** pHandle,
    ehs_sint32 x, ehs_sint32 y, ehs_sint32 dst_w, ehs_sint32 dst_h,
    const void* data, ehs_sint32 frame_w, ehs_sint32 frame_h, ehs_sint32 channels)
{
    if (!pHandle || !data || dst_w <= 0 || dst_h <= 0 || frame_w <= 0 || frame_h <= 0) {
        EHSH_LOG_WARNING("lvgl_camera_frame_renderer: rejected pHandle=%p data=%p dst=%dx%d frame=%dx%d",
                          (void*)pHandle, data, dst_w, dst_h, frame_w, frame_h);
        return;
    }
    if (channels != 3 && channels != 1) {
        EHSH_LOG_WARNING("lvgl_camera_frame_renderer: rejected channels=%d", channels);
        return; /* BGR and grayscale only */
    }

    EhsTPMutex_lock(EhsTPMutex_viewport);

    EhsCamEmbedCanvas_t* slot = (EhsCamEmbedCanvas_t*)*pHandle;
    if (!slot) {
        slot = (EhsCamEmbedCanvas_t*)malloc(sizeof(*slot));
        if (!slot) {
            EHSH_LOG_ERROR("lvgl_camera_frame_renderer: malloc(%zu) failed for canvas handle", sizeof(*slot));
            EhsTPMutex_unlock(EhsTPMutex_viewport); return;
        }
        memset(slot, 0, sizeof(*slot));
        *pHandle = slot;
    }

    /* (Re)create canvas when newly allocated or its size changes */
    if (!slot->canvas || slot->w != dst_w || slot->h != dst_h) {
        EHSH_LOG_INFO("lvgl_camera_frame_renderer: (re)creating canvas at %d,%d size %dx%d (was %p %dx%d)",
                      x, y, dst_w, dst_h, (void*)slot->canvas, slot->w, slot->h);
        if (slot->canvas) { lv_obj_del(slot->canvas); slot->canvas = NULL; }
        if (slot->buf)    { free(slot->buf); slot->buf = NULL; }

        size_t buf_sz = (size_t)LV_CANVAS_BUF_SIZE_TRUE_COLOR(dst_w, dst_h);
        slot->buf = (lv_color_t*)malloc(buf_sz);
        if (!slot->buf) {
            EHSH_LOG_ERROR("lvgl_camera_frame_renderer: malloc(%zu) failed for canvas buffer", buf_sz);
            EhsTPMutex_unlock(EhsTPMutex_viewport); return;
        }
        memset(slot->buf, 0, buf_sz);

        slot->canvas = lv_canvas_create(lv_scr_act());
        lv_canvas_set_buffer(slot->canvas, slot->buf,
                             (lv_coord_t)dst_w, (lv_coord_t)dst_h,
                             LV_IMG_CF_TRUE_COLOR);
        lv_obj_set_pos(slot->canvas, (lv_coord_t)x, (lv_coord_t)y);
        lv_obj_move_background(slot->canvas); /* stays behind UI widgets */
        lv_obj_set_style_pad_all(slot->canvas, 0, 0);
        lv_obj_set_style_border_width(slot->canvas, 0, 0);
        slot->w = dst_w; slot->h = dst_h;
    } else {
        /* Size unchanged, but position may have moved (e.g. layout reflow) */
        lv_obj_set_pos(slot->canvas, (lv_coord_t)x, (lv_coord_t)y);
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
            slot->buf[cy * dst_w + cx] = color;
        }
    }
    lv_obj_invalidate(slot->canvas);

    EhsTPMutex_unlock(EhsTPMutex_viewport);
}

/* Called once by the owning frame_show instance's EHS_FB_DESTROY_FUNCTION.
 * Must tolerate a NULL handle (an instance that never rendered a frame). */
static void lvgl_camera_frame_renderer_release(void* handle)
{
    if (!handle) return;
    EhsCamEmbedCanvas_t* slot = (EhsCamEmbedCanvas_t*)handle;
    EhsTPMutex_lock(EhsTPMutex_viewport);
    if (slot->canvas) lv_obj_del(slot->canvas);
    if (slot->buf)    free(slot->buf);
    EhsTPMutex_unlock(EhsTPMutex_viewport);
    free(slot);
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
    EhsCameraFrameRegisterEmbeddedRenderer(lvgl_camera_frame_renderer, lvgl_camera_frame_renderer_release);
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
 * Perform setup before loading in a new application.
 *
 * Camera embed canvases are no longer tracked here: each frame_show FB
 * instance now owns its canvas via the opaque handle EhsCamEmbeddedRendererFn
 * hands back, and releases it directly from its own EHS_FB_DESTROY_FUNCTION
 * (which runs, for every instance of the outgoing app, before this is next
 * called) — see lvgl_camera_frame_renderer_release() above.
 */
void EhsTGfxApp_init(void)
{
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
