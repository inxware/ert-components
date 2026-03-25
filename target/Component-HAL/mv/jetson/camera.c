/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/**
 * @file camera.c  (mv/jetson)
 * @brief Jetson camera HAL — GStreamer-based camera capture.
 *
 * Camera ID format
 * ----------------
 * - "csi:N"     — MIPI CSI-2 camera, sensor-id N (uses nvarguscamerasrc).
 *                 Requires the Jetson argus daemon to be running.
 * - "/dev/videoN" or a numeric string "N" — USB/V4L2 camera on /dev/videoN
 *                 (uses v4l2src).  nvvidconv converts to BGR if available.
 *
 * Pipeline (CSI):
 *   nvarguscamerasrc sensor-id=N
 *     ! video/x-raw(memory:NVMM),width=W,height=H,framerate=FPS/1
 *     ! nvvidconv
 *     ! video/x-raw,format=BGRx
 *     ! videoconvert
 *     ! video/x-raw,format=BGR
 *     ! appsink name=sink sync=false max-buffers=2 drop=true
 *
 * Pipeline (V4L2):
 *   v4l2src device=/dev/videoN
 *     ! video/x-raw,width=W,height=H,framerate=FPS/1
 *     ! videoconvert
 *     ! video/x-raw,format=BGR
 *     ! appsink name=sink sync=false max-buffers=2 drop=true
 *
 * Output format is BGR, 3 bytes per pixel, width×height packed rows.
 * This matches the OpenCV convention used by the rest of the MV pipeline.
 */

#include "globals.h"
#include "hal_mv.h"
#include "hal-api.h"
#include "jetson_frame_pool.h"

#include <gst/gst.h>
#include <gst/app/gstappsink.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

/* -------------------------------------------------------------------------
 * Camera context
 * ------------------------------------------------------------------------- */

typedef struct {
    GstElement   *pipeline;
    GstElement   *appsink;
    unsigned      width;
    unsigned      height;
    int           channels;    /* 3 = BGR */
} JetsonCamera;

/* -------------------------------------------------------------------------
 * Helper: parse camera ID string
 * Returns 1 if CSI (fills *sensor_id), 0 if V4L2 (fills *v4l2_dev).
 * ------------------------------------------------------------------------- */

static int parse_camera_id(const char *id, int *sensor_id, char *v4l2_dev, size_t v4l2_dev_len)
{
    if (!id || *id == '\0') { *sensor_id = 0; return 1; }

    /* "csi:N" */
    if (strncmp(id, "csi:", 4) == 0) {
        *sensor_id = atoi(id + 4);
        return 1;
    }

    /* Pure numeric "N" → /dev/videoN */
    int all_digit = 1;
    for (const char *p = id; *p; p++) {
        if (!isdigit((unsigned char)*p)) { all_digit = 0; break; }
    }
    if (all_digit) {
        snprintf(v4l2_dev, v4l2_dev_len, "/dev/video%s", id);
        return 0;
    }

    /* Path string e.g. /dev/video0 */
    snprintf(v4l2_dev, v4l2_dev_len, "%s", id);
    return 0;
}

/* -------------------------------------------------------------------------
 * EhsCameraStart
 * ------------------------------------------------------------------------- */

EhsCameraError EhsCameraStart(EhsCamera *camera, const ehs_char *id)
{
    if (!camera) return EHS_CAM_FAILED;
    if (camera->camera_ctx) return EHS_CAM_IN_USE;

    if (!gst_is_initialized())
        gst_init(NULL, NULL);

    unsigned w   = camera->width  > 0 ? camera->width  : 1280;
    unsigned h   = camera->height > 0 ? camera->height : 720;
    unsigned fps = camera->fps    > 0 ? camera->fps    : 30;

    int  sensor_id = 0;
    char v4l2_dev[64];
    int  is_csi = parse_camera_id(id, &sensor_id, v4l2_dev, sizeof(v4l2_dev));

    char pipeline_str[512];

    if (is_csi) {
        snprintf(pipeline_str, sizeof(pipeline_str),
            "nvarguscamerasrc sensor-id=%d"
            " ! video/x-raw(memory:NVMM),width=%u,height=%u,framerate=%u/1"
            " ! nvvidconv"
            " ! video/x-raw,format=BGRx"
            " ! videoconvert"
            " ! video/x-raw,format=BGR"
            " ! appsink name=sink sync=false max-buffers=2 drop=true",
            sensor_id, w, h, fps);
//todo check the number of chars is less than 512 to be sure this has worked..
    } else {
        snprintf(pipeline_str, sizeof(pipeline_str),
            "v4l2src device=%s"
            " ! video/x-raw,width=%u,height=%u,framerate=%u/1"
            " ! videoconvert"
            " ! video/x-raw,format=BGR"
            " ! appsink name=sink sync=false max-buffers=2 drop=true",
            v4l2_dev, w, h, fps);
//todo check the number of chars is less than 512 to be sure this has worked..
    }

    EHSH_LOG_INFO("Jetson camera: pipeline: %s", pipeline_str);

    GError *gerr = NULL;
    GstElement *pipeline = gst_parse_launch(pipeline_str, &gerr);
    if (!pipeline || gerr) {
        EHSH_LOG_ERROR("Jetson camera: gst_parse_launch failed: %s",
                       gerr ? gerr->message : "(unknown)");
        if (gerr) g_error_free(gerr);
        return EHS_CAM_OPEN_ERR;
    }

    GstElement *appsink = gst_bin_get_by_name(GST_BIN(pipeline), "sink");
    if (!appsink) {
        EHSH_LOG_ERROR("Jetson camera: could not find appsink element");
        gst_object_unref(pipeline);
        return EHS_CAM_OPEN_ERR;
    }

    if (gst_element_set_state(pipeline, GST_STATE_PLAYING) == GST_STATE_CHANGE_FAILURE) {
        EHSH_LOG_ERROR("Jetson camera: failed to set pipeline to PLAYING");
        gst_object_unref(appsink);
        gst_object_unref(pipeline);
        return EHS_CAM_OPEN_ERR;
    }

    JetsonCamera *ctx = (JetsonCamera *)EhsHMem_permAlloc(sizeof(JetsonCamera));
    if (!ctx) {
        gst_element_set_state(pipeline, GST_STATE_NULL);
        gst_object_unref(appsink);
        gst_object_unref(pipeline);
        return EHS_CAM_MEM_ERR;
    }
    EhsMemset(ctx, 0, sizeof(JetsonCamera));
    ctx->pipeline = pipeline;
    ctx->appsink  = appsink;
    ctx->width    = w;
    ctx->height   = h;
    ctx->channels = 3; /* BGR */

    camera->camera_ctx = ctx;
    EHSH_LOG_INFO("Jetson camera: started %ux%u @ %u fps (%s)",
                  w, h, fps, is_csi ? "CSI" : v4l2_dev);
    return EHS_CAM_OK;
}

/* -------------------------------------------------------------------------
 * EhsCameraStop / EhsCameraDestroy
 * ------------------------------------------------------------------------- */

void EhsCameraStop(EhsCamera *camera)
{
    if (!camera || !camera->camera_ctx) return;
    JetsonCamera *ctx = (JetsonCamera *)camera->camera_ctx;
    gst_element_set_state(ctx->pipeline, GST_STATE_NULL);
    gst_object_unref(ctx->appsink);
    gst_object_unref(ctx->pipeline);
    EhsHMem_permFree(ctx);
    camera->camera_ctx = NULL;
}

void EhsCameraDestroy(EhsCamera *camera)
{
    EhsCameraStop(camera);
    EhsMemset(camera, 0, sizeof(EhsCamera));
}

/* -------------------------------------------------------------------------
 * EhsCameraGrabFrame
 *
 * Pulls one sample from appsink, maps it to a CPU buffer, and fills the
 * EhsCameraFrame.  Also registers the sample in the jetson_frame_pool so that
 * TensorRT can (optionally) access the CUDA device pointer directly.
 * ------------------------------------------------------------------------- */

ehs_bool EhsCameraGrabFrame(EhsCamera *camera, EhsCameraFrame *frame, ehs_bool show_image)
{
    (void)show_image;
    if (!camera || !camera->camera_ctx || !frame || !frame->frameObj) return EHS_FALSE;

    JetsonCamera *ctx = (JetsonCamera *)camera->camera_ctx;

    /* Pull with 100 ms timeout */
    GstSample *sample = gst_app_sink_try_pull_sample(GST_APP_SINK(ctx->appsink), 100 * GST_MSECOND);
    if (!sample) return EHS_FALSE;

    GstBuffer *buf  = gst_sample_get_buffer(sample);
    GstMapInfo map;
    if (!gst_buffer_map(buf, &map, GST_MAP_READ)) {
        gst_sample_unref(sample);
        return EHS_FALSE;
    }

    size_t expected = (size_t)ctx->width * ctx->height * ctx->channels;
    if (map.size < expected) {
        EHSH_LOG_ERROR("Jetson camera: buffer size %zu < expected %zu", map.size, expected);
        gst_buffer_unmap(buf, &map);
        gst_sample_unref(sample);
        return EHS_FALSE;
    }

    /* Get the JetsonFrame private context */
    typedef struct {
        uint8_t  *buf;
        size_t    buf_size;
        unsigned  width;
        unsigned  height;
        int       channels;
        int       pool_slot;
    } JetsonFrame;
    JetsonFrame *jf = (JetsonFrame *)frame->frameObj;

    /* Release previous pool slot if any */
    if (jf->pool_slot >= 0) {
        jetson_frame_pool_unref(jf->pool_slot);
        jf->pool_slot = -1;
        jf->buf       = NULL;
    }

    /* Allocate or reuse CPU buffer */
    if (!jf->buf || jf->buf_size < expected) {
        if (jf->buf) EhsHMem_permFree(jf->buf);
        jf->buf = (uint8_t *)EhsHMem_permAlloc(expected);
        if (!jf->buf) {
            gst_buffer_unmap(buf, &map);
            gst_sample_unref(sample);
            return EHS_FALSE;
        }
        jf->buf_size = expected;
    }
    memcpy(jf->buf, map.data, expected);

    gst_buffer_unmap(buf, &map);

    /* Register in frame pool (for TensorRT zero-copy path) */
    int slot = jetson_frame_pool_alloc();
    if (slot >= 0) {
        JetsonFrameSlot *s = jetson_frame_pool_get(slot);
        s->gst_sample   = (void *)sample; /* pool holds GstSample ref */
        s->cpu_ptr      = jf->buf;
        s->cpu_size     = expected;
        s->width        = ctx->width;
        s->height       = ctx->height;
        s->channels     = ctx->channels;
        s->cuda_dev_ptr = NULL; /* NVMM CUDA pointer not extracted for now */
        jf->pool_slot   = slot;
        /* sample ref transferred to pool — do NOT unref here */
    } else {
        /* Pool full — still return valid CPU frame, just without pool registration */
        gst_sample_unref(sample);
    }

    jf->width    = ctx->width;
    jf->height   = ctx->height;
    jf->channels = ctx->channels;

    frame->width       = ctx->width;
    frame->height      = ctx->height;
    frame->fmt         = EHS_CAM_FMT_DEF;
    frame->opencl_mode = (slot >= 0) ? EHS_CAM_ACCELERATION_CUDA : EHS_CAM_ACCELERATION_DISABLED;

    return EHS_TRUE;
}
