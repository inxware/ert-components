/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/**
 * @file mv.c  (mv/android)
 * @brief Android MV HAL — frame lifecycle and CPU image operations.
 *
 * Frame representation
 * --------------------
 * EhsCameraFrame.frameObj points to an AndroidFrame (heap-allocated).
 * Pixel data is always RGB, 3 bytes per pixel, packed rows.
 * Camera2 NDK delivers YUV_420_888; camera.c converts to RGB on grab.
 *
 * All image operations (resize, crop, format-convert) are CPU-only
 * pixel loops.  This is sufficient for ML pre-processing at typical
 * inference resolutions (224×224, 320×320, 640×640).
 */

#include "globals.h"
#include "hal_mv.h"
#include "hal-api.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

/* -------------------------------------------------------------------------
 * Per-pixel channel count table
 * EHS_CAM_FMT_DEF = RGB, 3 bytes/pixel on this target.
 * ------------------------------------------------------------------------- */

ehs_uint8 gEhsCameraDataFormatChanLen[EHS_CAM_FMT_MAX] = {
    3,  /* EHS_CAM_FMT_DEF      — RGB uint8, 3 bytes/px */
    1,  /* EHS_CAM_FMT_8UC1     — greyscale uint8 */
    1,  /* EHS_CAM_FMT_32FC1_NORM — greyscale float32 normalised */
    3,  /* EHS_CAM_FMT_32FC3_NORM — RGB float32 normalised */
};

/* -------------------------------------------------------------------------
 * Per-frame private context
 * ------------------------------------------------------------------------- */

typedef struct {
    uint8_t  *buf;       /* CPU pixel buffer (heap-allocated, owned here) */
    size_t    buf_size;
    unsigned  width;
    unsigned  height;
    int       channels;  /* 3 = RGB (default), 1 = greyscale */
} AndroidFrame;

/* Allocate or grow the frame buffer if needed. */
static ehs_bool android_frame_ensure_buf(AndroidFrame *af, unsigned w, unsigned h, int ch)
{
    size_t needed = (size_t)w * h * (size_t)ch;
    if (af->buf && af->buf_size >= needed) {
        af->width    = w;
        af->height   = h;
        af->channels = ch;
        return EHS_TRUE;
    }
    if (af->buf) EhsHMem_permFree(af->buf);
    af->buf = (uint8_t *)EhsHMem_permAlloc(needed);
    if (!af->buf) { af->buf_size = 0; return EHS_FALSE; }
    af->buf_size  = needed;
    af->width     = w;
    af->height    = h;
    af->channels  = ch;
    return EHS_TRUE;
}

/* -------------------------------------------------------------------------
 * Frame lifecycle
 * ------------------------------------------------------------------------- */

void EhsCameraFrameCreate(EhsCameraFrame *frame)
{
    if (!frame) return;
    EhsMemset(frame, 0, sizeof(EhsCameraFrame));
    frame->id = -1;
    AndroidFrame *af = (AndroidFrame *)EhsHMem_permAlloc(sizeof(AndroidFrame));
    EhsMemset(af, 0, sizeof(AndroidFrame));
    frame->frameObj    = af;
    frame->opencl_mode = EHS_CAM_ACCELERATION_DISABLED;
}

void EhsCameraFrameDestroy(EhsCameraFrame *frame)
{
    if (!frame || !frame->frameObj) return;
    AndroidFrame *af = (AndroidFrame *)frame->frameObj;
    if (af->buf) EhsHMem_permFree(af->buf);
    EhsHMem_permFree(af);
    EhsMemset(frame, 0, sizeof(EhsCameraFrame));
}

/* -------------------------------------------------------------------------
 * Data access
 * ------------------------------------------------------------------------- */

ehs_bool EhsCameraFrameGetData(EhsCameraFrame *frame, void **frame_data, ehs_uint32 *frame_size)
{
    if (!frame || !frame->frameObj || !frame_data || !frame_size) return EHS_FALSE;
    AndroidFrame *af = (AndroidFrame *)frame->frameObj;
    if (!af->buf || af->buf_size == 0) return EHS_FALSE;
    *frame_data = af->buf;
    *frame_size = (ehs_uint32)af->buf_size;
    return EHS_TRUE;
}

/* Always CPU on this target — no-op. */
ehs_bool EhsCameraFrameEnsureCPU(EhsCameraFrame *frame)
{
    if (!frame) return EHS_FALSE;
    frame->opencl_mode = EHS_CAM_ACCELERATION_DISABLED;
    return EHS_TRUE;
}

/* -------------------------------------------------------------------------
 * File I/O — not yet implemented; use adb push + test via function block.
 * ------------------------------------------------------------------------- */

ehs_bool EhsCameraFrameReadFromFile(EhsCameraFrame *frame, const ehs_char *filepath)
{
    (void)frame; (void)filepath;
    EHSH_LOG_ERROR("EhsCameraFrameReadFromFile: not implemented on Android target");
    return EHS_FALSE;
}

ehs_bool EhsCameraFrameWriteToFile(EhsCameraFrame *frame, const ehs_char *filepath)
{
    (void)frame; (void)filepath;
    EHSH_LOG_ERROR("EhsCameraFrameWriteToFile: not implemented on Android target");
    return EHS_FALSE;
}

/* -------------------------------------------------------------------------
 * Crop
 * ------------------------------------------------------------------------- */

ehs_bool EhsCameraFrameCrop(EhsCameraFrame *src, EhsCameraFrame *dst,
                             ehs_uint32 x1, ehs_uint32 y1,
                             ehs_uint32 x2, ehs_uint32 y2)
{
    if (!src || !src->frameObj || !dst || !dst->frameObj) return EHS_FALSE;
    if (x1 >= x2 || y1 >= y2) return EHS_FALSE;

    AndroidFrame *sf = (AndroidFrame *)src->frameObj;
    if (!sf->buf || x2 > sf->width || y2 > sf->height) return EHS_FALSE;

    ehs_uint32 w = x2 - x1;
    ehs_uint32 h = y2 - y1;
    int ch = sf->channels;

    AndroidFrame *df = (AndroidFrame *)dst->frameObj;
    if (!android_frame_ensure_buf(df, w, h, ch)) return EHS_FALSE;

    for (unsigned row = 0; row < h; row++) {
        const uint8_t *srow = sf->buf + ((y1 + row) * sf->width + x1) * ch;
        uint8_t       *drow = df->buf + (row * w) * ch;
        memcpy(drow, srow, (size_t)w * ch);
    }

    dst->width        = w;
    dst->height       = h;
    dst->fmt          = src->fmt;
    dst->opencl_mode  = EHS_CAM_ACCELERATION_DISABLED;
    return EHS_TRUE;
}

/* -------------------------------------------------------------------------
 * Resize — nearest-neighbour (fastest; adequate for ML pre-processing)
 * ------------------------------------------------------------------------- */

ehs_bool EhsCameraFrameResize(EhsCameraFrame *src, EhsCameraFrame *dst,
                               ehs_uint32 width, ehs_uint32 height, ehs_sint32 interp)
{
    (void)interp; /* nearest-neighbour only */
    if (!src || !src->frameObj || !dst || !dst->frameObj) return EHS_FALSE;

    AndroidFrame *sf = (AndroidFrame *)src->frameObj;
    if (!sf->buf || sf->width == 0 || sf->height == 0) return EHS_FALSE;

    int ch = sf->channels;
    AndroidFrame *df = (AndroidFrame *)dst->frameObj;
    if (!android_frame_ensure_buf(df, width, height, ch)) return EHS_FALSE;

    float sx = (float)sf->width  / (float)width;
    float sy = (float)sf->height / (float)height;

    for (unsigned dy = 0; dy < height; dy++) {
        unsigned sy_px = (unsigned)(dy * sy);
        if (sy_px >= sf->height) sy_px = sf->height - 1;
        for (unsigned dx = 0; dx < width; dx++) {
            unsigned sx_px = (unsigned)(dx * sx);
            if (sx_px >= sf->width) sx_px = sf->width - 1;
            const uint8_t *sp = sf->buf + (sy_px * sf->width + sx_px) * ch;
            uint8_t       *dp = df->buf + (dy    * width    + dx)    * ch;
            for (int c = 0; c < ch; c++) dp[c] = sp[c];
        }
    }

    dst->width       = width;
    dst->height      = height;
    dst->fmt         = src->fmt;
    dst->opencl_mode = EHS_CAM_ACCELERATION_DISABLED;
    return EHS_TRUE;
}

/* -------------------------------------------------------------------------
 * Format conversion
 * Source is RGB (EHS_CAM_FMT_DEF on this target).
 * Greyscale coefficients: BT.601 Y = 0.299R + 0.587G + 0.114B
 * ------------------------------------------------------------------------- */

ehs_bool EhsCameraFrameFormat(EhsCameraFrame *src, EhsCameraFrame *dst, EhsCameraDataFormat fmt)
{
    if (!src || !src->frameObj || !dst || !dst->frameObj) return EHS_FALSE;

    AndroidFrame *sf = (AndroidFrame *)src->frameObj;
    if (!sf->buf) return EHS_FALSE;

    unsigned w  = sf->width;
    unsigned h  = sf->height;
    unsigned np = w * h;

    switch (fmt) {
    case EHS_CAM_FMT_8UC1: {
        AndroidFrame *df = (AndroidFrame *)dst->frameObj;
        if (!android_frame_ensure_buf(df, w, h, 1)) return EHS_FALSE;
        const uint8_t *s = sf->buf;
        uint8_t       *d = df->buf;
        for (unsigned i = 0; i < np; i++, s += sf->channels, d++)
            *d = (uint8_t)(s[0] * 0.299f + s[1] * 0.587f + s[2] * 0.114f);
        dst->width = w; dst->height = h; dst->fmt = fmt;
        dst->opencl_mode = EHS_CAM_ACCELERATION_DISABLED;
        return EHS_TRUE;
    }
    case EHS_CAM_FMT_32FC1_NORM: {
        AndroidFrame *df = (AndroidFrame *)dst->frameObj;
        size_t needed = np * sizeof(float);
        if (df->buf) EhsHMem_permFree(df->buf);
        df->buf = (uint8_t *)EhsHMem_permAlloc(needed);
        if (!df->buf) return EHS_FALSE;
        df->buf_size = needed;
        df->width = w; df->height = h; df->channels = 1;
        float *d = (float *)df->buf;
        const uint8_t *s = sf->buf;
        for (unsigned i = 0; i < np; i++, s += sf->channels)
            d[i] = (s[0] * 0.299f + s[1] * 0.587f + s[2] * 0.114f) / 255.0f;
        dst->width = w; dst->height = h; dst->fmt = fmt;
        dst->opencl_mode = EHS_CAM_ACCELERATION_DISABLED;
        return EHS_TRUE;
    }
    case EHS_CAM_FMT_32FC3_NORM: {
        AndroidFrame *df = (AndroidFrame *)dst->frameObj;
        size_t needed = np * 3 * sizeof(float);
        if (df->buf) EhsHMem_permFree(df->buf);
        df->buf = (uint8_t *)EhsHMem_permAlloc(needed);
        if (!df->buf) return EHS_FALSE;
        df->buf_size = needed;
        df->width = w; df->height = h; df->channels = 3;
        float *d = (float *)df->buf;
        const uint8_t *s = sf->buf;
        for (unsigned i = 0; i < np; i++, s += sf->channels) {
            d[i*3+0] = s[0] / 255.0f;
            d[i*3+1] = s[1] / 255.0f;
            d[i*3+2] = s[2] / 255.0f;
        }
        dst->width = w; dst->height = h; dst->fmt = fmt;
        dst->opencl_mode = EHS_CAM_ACCELERATION_DISABLED;
        return EHS_TRUE;
    }
    default:
        return EHS_FALSE;
    }
}

/* -------------------------------------------------------------------------
 * Deep copy
 * ------------------------------------------------------------------------- */

ehs_bool EhsCameraFrameCopy(const EhsCameraFrame *src, EhsCameraFrame *dst)
{
    if (!src || !src->frameObj || !dst || !dst->frameObj) return EHS_FALSE;
    AndroidFrame *sf = (AndroidFrame *)src->frameObj;
    if (!sf->buf) return EHS_FALSE;
    AndroidFrame *df = (AndroidFrame *)dst->frameObj;
    if (!android_frame_ensure_buf(df, sf->width, sf->height, sf->channels)) return EHS_FALSE;
    memcpy(df->buf, sf->buf, sf->buf_size);
    dst->width       = src->width;
    dst->height      = src->height;
    dst->fmt         = src->fmt;
    dst->opencl_mode = EHS_CAM_ACCELERATION_DISABLED;
    return EHS_TRUE;
}

/* -------------------------------------------------------------------------
 * Bounding box — not implemented (no font library; log results instead)
 * ------------------------------------------------------------------------- */

ehs_bool EhsCameraFrameDrawBBox(EhsCameraFrame *frame,
                                 ehs_sint32 x1, ehs_sint32 y1,
                                 ehs_sint32 x2, ehs_sint32 y2,
                                 ehs_uint8 r, ehs_uint8 g, ehs_uint8 b,
                                 ehs_sint32 thickness,
                                 const ehs_char *label)
{
    (void)frame; (void)x1; (void)y1; (void)x2; (void)y2;
    (void)r; (void)g; (void)b; (void)thickness; (void)label;
    return EHS_FALSE;
}

/* -------------------------------------------------------------------------
 * Embedded renderer — not wired up on this target
 * ------------------------------------------------------------------------- */

static EhsCamEmbeddedRendererFn s_renderer = NULL;

void EhsCameraFrameRegisterEmbeddedRenderer(EhsCamEmbeddedRendererFn fn) { s_renderer = fn; }
EhsCamEmbeddedRendererFn EhsCameraFrameGetEmbeddedRenderer(void) { return s_renderer; }
