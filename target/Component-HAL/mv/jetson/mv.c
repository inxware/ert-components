/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/* TODO - review the applicability of some of these functions for gstreamer and GPU-based pipelines, where these 
   operations are not aligned with the gstreamer pipeline processing. 
*/

/**
 * @file mv.c  (mv/jetson)
 * @brief Jetson MV HAL — frame management and image-processing operations.
 *
 * Frame representation
 * --------------------
 * EhsCameraFrame.frameObj points to a JetsonFrame (heap-allocated).  It carries
 * a CPU pixel buffer and metadata.  When a frame is grabbed from the camera (see
 * camera.c) the camera layer fills the JetsonFrame and optionally registers the
 * CUDA device pointer in the shared jetson_frame_pool so that TensorRT can read it
 * without a device→CPU copy.
 *
 * Image operations (resize, crop, format-convert) are performed on the CPU buffer
 * using NvBufSurface transform where available, or by plain C loops.  A future
 * revision can route these through nvvidconv pipelines for VIC-hardware acceleration.
 *
 * EhsCameraFrameAdd / EhsCameraFrameRemove / EhsCameraFrameGetById are in the
 * target-independent mv_common.c and are not duplicated here.
 */

#include "globals.h"
#include "hal_mv.h"
#include "hal-api.h"
#include "jetson_frame_pool.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* -------------------------------------------------------------------------
 * Per-pixel channel count table (matches OpenCV mv.c convention)
 * ------------------------------------------------------------------------- */

#ifndef EHS_CAMERA_DATAFORMAT_CHANLEN_DEF
#define EHS_CAMERA_DATAFORMAT_CHANLEN_DEF 3
#endif

ehs_uint8 gEhsCameraDataFormatChanLen[EHS_CAM_FMT_MAX] = {
    EHS_CAMERA_DATAFORMAT_CHANLEN_DEF, /* EHS_CAM_FMT_DEF  — BGR, 3 bytes/px */
    1,                                  /* EHS_CAM_FMT_8UC1 — greyscale */
    1,                                  /* EHS_CAM_FMT_32FC1_NORM */
    3,                                  /* EHS_CAM_FMT_32FC3_NORM */
};

/* -------------------------------------------------------------------------
 * Frame pool — global table shared with ml/tensorrt
 * ------------------------------------------------------------------------- */

JetsonFrameSlot g_jetson_frame_pool[EHS_CAM_MAX_FRAMES];

int jetson_frame_pool_alloc(void)
{
    for (int i = 0; i < EHS_CAM_MAX_FRAMES; i++) {
        if (g_jetson_frame_pool[i].ref_count == 0) {
            memset(&g_jetson_frame_pool[i], 0, sizeof(JetsonFrameSlot));
            g_jetson_frame_pool[i].ref_count = 1;
            return i;
        }
    }
    return -1;
}

void jetson_frame_pool_ref(int idx)
{
    if (idx >= 0 && idx < EHS_CAM_MAX_FRAMES)
        g_jetson_frame_pool[idx].ref_count++;
}

void jetson_frame_pool_unref(int idx)
{
    if (idx < 0 || idx >= EHS_CAM_MAX_FRAMES) return;
    JetsonFrameSlot *s = &g_jetson_frame_pool[idx];
    if (s->ref_count <= 0) return;
    if (--s->ref_count == 0) {
        /* Release GstSample reference */
        if (s->gst_sample) {
            /* gst_sample_unref is a GStreamer function; include gst/gst.h only
             * where GStreamer types are actually used. Here we cast to avoid a
             * hard dependency on gst/gst.h in this file (camera.c includes it). */
            extern void gst_sample_unref(void *);
            gst_sample_unref(s->gst_sample);
            s->gst_sample = NULL;
        }
        /* cpu_ptr is either a malloc'd copy or a GstMemory mapping.
         * camera.c sets gst_sample != NULL when cpu_ptr is a mapped view
         * (already released above).  If cpu_ptr was malloc'd independently
         * (non-NVMM path) it is freed here. */
        if (s->cpu_ptr && !s->gst_sample) {
            free(s->cpu_ptr);
            s->cpu_ptr = NULL;
        }
        s->cuda_dev_ptr = NULL;
    }
}

/* -------------------------------------------------------------------------
 * Per-frame private context
 * ------------------------------------------------------------------------- */

typedef struct {
    uint8_t  *buf;        /* CPU pixel buffer (malloc'd; owned by this struct) */
    size_t    buf_size;
    unsigned  width;
    unsigned  height;
    int       channels;
    int       pool_slot;  /* index in g_jetson_frame_pool; -1 if not from camera */
} JetsonFrame;

/* -------------------------------------------------------------------------
 * EhsCameraFrame lifecycle
 * ------------------------------------------------------------------------- */

void EhsCameraFrameCreate(EhsCameraFrame *frame)
{
    if (!frame) return;
    EhsMemset(frame, 0, sizeof(EhsCameraFrame));
    frame->id = -1;
    JetsonFrame *jf = (JetsonFrame *)EhsHMem_permAlloc(sizeof(JetsonFrame));
    EhsMemset(jf, 0, sizeof(JetsonFrame));
    jf->pool_slot = -1;
    frame->frameObj = jf;
    frame->opencl_mode = EHS_CAM_ACCELERATION_CUDA;
}

void EhsCameraFrameDestroy(EhsCameraFrame *frame)
{
    if (!frame || !frame->frameObj) return;
    JetsonFrame *jf = (JetsonFrame *)frame->frameObj;
    if (jf->pool_slot >= 0) {
        jetson_frame_pool_unref(jf->pool_slot);
        jf->pool_slot = -1;
        jf->buf = NULL; /* owned by pool slot */
    } else if (jf->buf) {
        EhsHMem_permFree(jf->buf);
        jf->buf = NULL;
    }
    EhsHMem_permFree(jf);
    EhsMemset(frame, 0, sizeof(EhsCameraFrame));
}

/* -------------------------------------------------------------------------
 * Data access
 * ------------------------------------------------------------------------- */

ehs_bool EhsCameraFrameGetData(EhsCameraFrame *frame, void **frame_data, ehs_uint32 *frame_size)
{
    if (!frame || !frame->frameObj || !frame_data || !frame_size) return EHS_FALSE;
    JetsonFrame *jf = (JetsonFrame *)frame->frameObj;
    if (!jf->buf || jf->buf_size == 0) return EHS_FALSE;
    *frame_data = jf->buf;
    *frame_size = (ehs_uint32)jf->buf_size;
    return EHS_TRUE;
}

/**
 * For Jetson frames the CPU buffer is always kept up-to-date by camera.c.
 * The opencl_mode field is set to EHS_CAM_ACCELERATION_CUDA to signal that
 * a CUDA device pointer is available in the frame pool, but the CPU buffer
 * is always valid and no additional download step is required.
 */
ehs_bool EhsCameraFrameEnsureCPU(EhsCameraFrame *frame)
{
    if (!frame || !frame->frameObj) return EHS_FALSE;
    frame->opencl_mode = EHS_CAM_ACCELERATION_DISABLED;
    return EHS_TRUE;
}

/* -------------------------------------------------------------------------
 * File I/O — requires libjpeg/libpng or stb_image; provided as stubs here.
 * For production use, link against a suitable image library.
 * ------------------------------------------------------------------------- */

ehs_bool EhsCameraFrameReadFromFile(EhsCameraFrame *frame, const ehs_char *filepath)
{
    (void)frame; (void)filepath;
    EHSH_LOG_ERROR("EhsCameraFrameReadFromFile: not implemented on Jetson target");
    return EHS_FALSE;
}

ehs_bool EhsCameraFrameWriteToFile(EhsCameraFrame *frame, const ehs_char *filepath)
{
    (void)frame; (void)filepath;
    EHSH_LOG_ERROR("EhsCameraFrameWriteToFile: not implemented on Jetson target");
    return EHS_FALSE;
}

/* -------------------------------------------------------------------------
 * Image operations — CPU implementations
 *
 * For best performance these should be replaced with NvBufSurface transform
 * calls (VIC hardware) or CUDA kernels once the pipeline is validated.
 * ------------------------------------------------------------------------- */

static ehs_bool jetson_frame_ensure_buf(JetsonFrame *jf, unsigned width, unsigned height, int channels)
{
    size_t needed = (size_t)width * height * channels;
    if (jf->buf && jf->buf_size >= needed) {
        jf->width    = width;
        jf->height   = height;
        jf->channels = channels;
        return EHS_TRUE;
    }
    if (jf->pool_slot >= 0) {
        /* dst frame came from pool — detach before reallocating */
        jetson_frame_pool_unref(jf->pool_slot);
        jf->pool_slot = -1;
        jf->buf = NULL;
    } else if (jf->buf) {
        EhsHMem_permFree(jf->buf);
    }
    jf->buf = (uint8_t *)EhsHMem_permAlloc(needed);
    if (!jf->buf) { jf->buf_size = 0; return EHS_FALSE; }
    jf->buf_size  = needed;
    jf->width     = width;
    jf->height    = height;
    jf->channels  = channels;
    return EHS_TRUE;
}

ehs_bool EhsCameraFrameCrop(EhsCameraFrame *src, EhsCameraFrame *dst,
                             ehs_uint32 x1, ehs_uint32 y1,
                             ehs_uint32 x2, ehs_uint32 y2)
{
    if (!src || !src->frameObj || !dst || !dst->frameObj) return EHS_FALSE;
    if (x1 >= x2 || y1 >= y2) return EHS_FALSE;

    JetsonFrame *sf = (JetsonFrame *)src->frameObj;
    if (!sf->buf) return EHS_FALSE;

    ehs_uint32 w = x2 - x1;
    ehs_uint32 h = y2 - y1;
    int ch = sf->channels;
    if (x2 > sf->width || y2 > sf->height) return EHS_FALSE;

    JetsonFrame *df = (JetsonFrame *)dst->frameObj;
    if (!jetson_frame_ensure_buf(df, w, h, ch)) return EHS_FALSE;

    for (unsigned row = 0; row < h; row++) {
        const uint8_t *srow = sf->buf + ((y1 + row) * sf->width + x1) * ch;
        uint8_t       *drow = df->buf  + (row * w) * ch;
        memcpy(drow, srow, (size_t)w * ch);
    }

    dst->width  = w;
    dst->height = h;
    dst->fmt    = src->fmt;
    dst->opencl_mode = EHS_CAM_ACCELERATION_DISABLED;
    return EHS_TRUE;
}

ehs_bool EhsCameraFrameResize(EhsCameraFrame *src, EhsCameraFrame *dst,
                               ehs_uint32 width, ehs_uint32 height, ehs_sint32 interp)
{
    (void)interp; /* nearest-neighbour only for now */
    if (!src || !src->frameObj || !dst || !dst->frameObj) return EHS_FALSE;

    JetsonFrame *sf = (JetsonFrame *)src->frameObj;
    if (!sf->buf || sf->width == 0 || sf->height == 0) return EHS_FALSE;

    int ch = sf->channels;
    JetsonFrame *df = (JetsonFrame *)dst->frameObj;
    if (!jetson_frame_ensure_buf(df, width, height, ch)) return EHS_FALSE;

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

    dst->width  = width;
    dst->height = height;
    dst->fmt    = src->fmt;
    dst->opencl_mode = EHS_CAM_ACCELERATION_DISABLED;
    return EHS_TRUE;
}

ehs_bool EhsCameraFrameFormat(EhsCameraFrame *src, EhsCameraFrame *dst, EhsCameraDataFormat fmt)
{
    if (!src || !src->frameObj || !dst || !dst->frameObj) return EHS_FALSE;

    JetsonFrame *sf = (JetsonFrame *)src->frameObj;
    if (!sf->buf) return EHS_FALSE;

    unsigned w  = sf->width;
    unsigned h  = sf->height;
    unsigned np = w * h;

    switch (fmt) {
    case EHS_CAM_FMT_8UC1: {
        /* BGR → greyscale:  Y = 0.114*B + 0.587*G + 0.299*R */
        JetsonFrame *df = (JetsonFrame *)dst->frameObj;
        if (!jetson_frame_ensure_buf(df, w, h, 1)) return EHS_FALSE;
        const uint8_t *s = sf->buf;
        uint8_t       *d = df->buf;
        for (unsigned i = 0; i < np; i++, s += sf->channels, d++) {
            *d = (uint8_t)(s[0] * 0.114f + s[1] * 0.587f + s[2] * 0.299f);
        }
        dst->width  = w;
        dst->height = h;
        dst->fmt    = fmt;
        dst->opencl_mode = EHS_CAM_ACCELERATION_DISABLED;
        return EHS_TRUE;
    }
    case EHS_CAM_FMT_32FC1_NORM: {
        JetsonFrame *df = (JetsonFrame *)dst->frameObj;
        size_t needed = np * sizeof(float);
        if (df->pool_slot >= 0) { jetson_frame_pool_unref(df->pool_slot); df->pool_slot=-1; df->buf=NULL; }
        else if (df->buf) EhsHMem_permFree(df->buf);
        df->buf = (uint8_t *)EhsHMem_permAlloc(needed);
        if (!df->buf) return EHS_FALSE;
        df->buf_size = needed;
        df->width = w; df->height = h; df->channels = 1;
        float *d = (float *)df->buf;
        const uint8_t *s = sf->buf;
        for (unsigned i = 0; i < np; i++, s += sf->channels)
            d[i] = (s[0]*0.114f + s[1]*0.587f + s[2]*0.299f) / 255.0f;
        dst->width  = w;
        dst->height = h;
        dst->fmt    = fmt;
        dst->opencl_mode = EHS_CAM_ACCELERATION_DISABLED;
        return EHS_TRUE;
    }
    case EHS_CAM_FMT_32FC3_NORM: {
        JetsonFrame *df = (JetsonFrame *)dst->frameObj;
        size_t needed = np * 3 * sizeof(float);
        if (df->pool_slot >= 0) { jetson_frame_pool_unref(df->pool_slot); df->pool_slot=-1; df->buf=NULL; }
        else if (df->buf) EhsHMem_permFree(df->buf);
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
        dst->width  = w;
        dst->height = h;
        dst->fmt    = fmt;
        dst->opencl_mode = EHS_CAM_ACCELERATION_DISABLED;
        return EHS_TRUE;
    }
    default:
        return EHS_FALSE;
    }
}

ehs_bool EhsCameraFrameDrawBBox(EhsCameraFrame *frame,
                                 ehs_sint32 x1, ehs_sint32 y1,
                                 ehs_sint32 x2, ehs_sint32 y2,
                                 ehs_uint8 r, ehs_uint8 g, ehs_uint8 b,
                                 ehs_sint32 thickness,
                                 const ehs_char* label)
{
    /* Text rendering is not available without a font library on Jetson.
     * Box drawing on the raw CPU buffer is possible in principle but would
     * require per-format stride logic for every supported pixel type.
     * Stubbed for now — annotated frames are typically displayed via OpenCV. */
    (void)frame; (void)x1; (void)y1; (void)x2; (void)y2;
    (void)r; (void)g; (void)b; (void)thickness; (void)label;
    return EHS_FALSE;
}

ehs_bool EhsCameraFrameCopy(const EhsCameraFrame* src, EhsCameraFrame* dst)
{
    (void)src; (void)dst;
    return EHS_FALSE; /* not yet implemented on Jetson */
}

/* Embedded renderer stubs — not supported on Jetson */
void EhsCameraFrameRegisterEmbeddedRenderer(EhsCamEmbeddedRendererFn fn, EhsCamEmbeddedRendererReleaseFn releaseFn) { (void)fn; (void)releaseFn; }
EhsCamEmbeddedRendererFn EhsCameraFrameGetEmbeddedRenderer(void) { return NULL; }
EhsCamEmbeddedRendererReleaseFn EhsCameraFrameGetEmbeddedRendererRelease(void) { return NULL; }
