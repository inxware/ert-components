/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/**
 * @file camera.c  (mv/android)
 * @brief Android Camera2 NDK camera capture.
 *
 * Camera ID format (passed to EhsCameraStart)
 * --------------------------------------------
 *   ""  or "back"  — back-facing camera (default)
 *   "front"        — front-facing camera
 *   "0", "1", …   — camera by list index (0 = back on most devices)
 *
 * Output
 * ------
 *   YUV_420_888 frames acquired from AImageReader are converted to packed
 *   RGB (3 bytes/pixel, R first) before filling the EhsCameraFrame buffer.
 *   This matches the TFLite YOLO models' expected input channel order.
 *
 * Session startup
 * ---------------
 *   ACameraManager_openCamera (NDK C API) is synchronous — it blocks until
 *   the device is opened and fills the device pointer before returning.
 *   The capture session is configured asynchronously; EhsCameraGrabFrame
 *   returns EHS_FALSE until sess_ready fires, then retries on the next tick.
 *
 * Permission
 * ----------
 *   android.permission.CAMERA must be declared in AndroidManifest.xml and
 *   granted at runtime (API 23+).  Grant once via adb for testing:
 *     adb shell pm grant com.inx.ehs android.permission.CAMERA
 */

#include "globals.h"
#include "hal_mv.h"
#include "hal-api.h"

/* stdbool.h must come before NDK camera headers — NdkCameraMetadata.h uses
 * 'bool' without including stdbool.h itself (Android NDK bug / omission). */
#include <stdbool.h>

#include <camera/NdkCameraManager.h>
#include <camera/NdkCameraDevice.h>
#include <camera/NdkCaptureRequest.h>
#include <camera/NdkCameraMetadata.h>
#include <camera/NdkCameraMetadataTags.h>
#include <media/NdkImageReader.h>
#include <media/NdkImage.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

/* -------------------------------------------------------------------------
 * Camera context
 * ------------------------------------------------------------------------- */

typedef enum {
    CAM_STATE_CLOSED    = 0,
    CAM_STATE_STARTING  = 1,  /* session being configured */
    CAM_STATE_STREAMING = 2,
    CAM_STATE_ERROR     = 3
} CamState;

typedef struct {
    ACameraManager                  *manager;
    ACameraDevice                   *device;
    AImageReader                    *reader;
    ANativeWindow                   *window;
    ACaptureSessionOutput           *session_output;
    ACaptureSessionOutputContainer  *output_container;
    ACameraOutputTarget             *output_target;
    ACaptureRequest                 *request;
    ACameraCaptureSession           *session;
    unsigned                         width;
    unsigned                         height;
    volatile CamState                state;
} AndroidCamera;

/* -------------------------------------------------------------------------
 * AndroidFrame forward declaration (defined in mv.c)
 * Used to write directly into the pixel buffer on each grab.
 * ------------------------------------------------------------------------- */

typedef struct {
    uint8_t  *buf;
    size_t    buf_size;
    unsigned  width;
    unsigned  height;
    int       channels;
} AndroidFrame;

/* -------------------------------------------------------------------------
 * YUV_420_888 → packed RGB conversion
 *
 * Y plane:  y_row_stride bytes per row, 1 byte per pixel
 * U/V planes: row stride + pixel stride (accounts for NV12/NV21 interleave)
 * Output: packed RGB, R at byte 0 per pixel
 * ------------------------------------------------------------------------- */

static inline uint8_t clamp_u8(int v)
{
    return (uint8_t)(v < 0 ? 0 : v > 255 ? 255 : v);
}

static void yuv420_to_rgb(
    const uint8_t *y_data,  int32_t y_row_stride,
    const uint8_t *u_data,  int32_t u_row_stride, int32_t u_px_stride,
    const uint8_t *v_data,  int32_t v_row_stride, int32_t v_px_stride,
    int width, int height,
    uint8_t *rgb_out)
{
    for (int row = 0; row < height; row++) {
        const uint8_t *y_row  = y_data + row * y_row_stride;
        const uint8_t *u_row  = u_data + (row >> 1) * u_row_stride;
        const uint8_t *v_row  = v_data + (row >> 1) * v_row_stride;
        uint8_t       *out    = rgb_out + row * width * 3;

        for (int col = 0; col < width; col++) {
            int y = (int)y_row[col];
            int u = (int)u_row[(col >> 1) * u_px_stride] - 128;
            int v = (int)v_row[(col >> 1) * v_px_stride] - 128;

            /* BT.601 full-range */
            out[0] = clamp_u8(y + (int)(1.402f  * v));
            out[1] = clamp_u8(y - (int)(0.344f  * u) - (int)(0.714f * v));
            out[2] = clamp_u8(y + (int)(1.772f  * u));
            out += 3;
        }
    }
}

/* -------------------------------------------------------------------------
 * Device state callbacks — disconnect / error only.
 * (ACameraManager_openCamera in the NDK C API is synchronous; device is
 *  returned directly from the call — no onOpened callback exists.)
 * ------------------------------------------------------------------------- */

static void dev_disconnected(void *ctx, ACameraDevice *dev)
{
    (void)dev;
    AndroidCamera *c = (AndroidCamera *)ctx;
    EHSH_LOG_ERROR("Android camera: device disconnected");
    c->state = CAM_STATE_ERROR;
}

static void dev_error(void *ctx, ACameraDevice *dev, int error)
{
    (void)dev;
    AndroidCamera *c = (AndroidCamera *)ctx;
    EHSH_LOG_ERROR("Android camera: device error %d", error);
    c->state = CAM_STATE_ERROR;
}

/* -------------------------------------------------------------------------
 * Capture session state callbacks
 * ------------------------------------------------------------------------- */

static void sess_ready(void *ctx, ACameraCaptureSession *sess)
{
    AndroidCamera *c = (AndroidCamera *)ctx;
    camera_status_t s = ACameraCaptureSession_setRepeatingRequest(sess, NULL, 1, &c->request, NULL);
    if (s != ACAMERA_OK) {
        EHSH_LOG_ERROR("Android camera: setRepeatingRequest failed: %d", s);
        c->state = CAM_STATE_ERROR;
    } else {
        c->state = CAM_STATE_STREAMING;
        EHSH_LOG_INFO("Android camera: streaming started (%ux%u)", c->width, c->height);
    }
}

static void sess_closed(void *ctx, ACameraCaptureSession *sess)
{
    (void)ctx; (void)sess;
}

static void sess_active(void *ctx, ACameraCaptureSession *sess)
{
    (void)ctx; (void)sess;
}

/* -------------------------------------------------------------------------
 * Camera enumeration — find the camera ID matching the requested facing/index
 * ------------------------------------------------------------------------- */

static const char *find_camera_id(ACameraManager *mgr, const char *id,
                                   char *out_buf, size_t out_len)
{
    ACameraIdList *list = NULL;
    if (ACameraManager_getCameraIdList(mgr, &list) != ACAMERA_OK || !list)
        return NULL;

    int want_facing = -1;
    int want_index  = -1;

    if (!id || *id == '\0' || strcmp(id, "back") == 0) {
        want_facing = ACAMERA_LENS_FACING_BACK;
    } else if (strcmp(id, "front") == 0) {
        want_facing = ACAMERA_LENS_FACING_FRONT;
    } else {
        char *endp = NULL;
        long idx = strtol(id, &endp, 10);
        if (endp && *endp == '\0' && idx >= 0) {
            want_index = (int)idx;
        } else {
            /* Exact camera ID string — pass through directly */
            ACameraManager_deleteCameraIdList(list);
            return id;
        }
    }

    const char *result = NULL;
    for (int i = 0; i < list->numCameras && !result; i++) {
        const char *cam_id = list->cameraIds[i];

        if (want_index >= 0) {
            if (i == want_index) {
                snprintf(out_buf, out_len, "%s", cam_id);
                result = out_buf;
            }
            continue;
        }

        ACameraMetadata *meta = NULL;
        if (ACameraManager_getCameraCharacteristics(mgr, cam_id, &meta) == ACAMERA_OK) {
            ACameraMetadata_const_entry entry;
            if (ACameraMetadata_getConstEntry(meta, ACAMERA_LENS_FACING, &entry) == ACAMERA_OK
                    && (int)entry.data.u8[0] == want_facing) {
                snprintf(out_buf, out_len, "%s", cam_id);
                result = out_buf;
            }
            ACameraMetadata_free(meta);
        }
    }

    if (!result && list->numCameras > 0) {
        EHSH_LOG_INFO("Android camera: requested facing not found, falling back to camera 0");
        snprintf(out_buf, out_len, "%s", list->cameraIds[0]);
        result = out_buf;
    }

    ACameraManager_deleteCameraIdList(list);
    return result;
}

/* -------------------------------------------------------------------------
 * EhsCameraStart
 * Opens the camera synchronously (ACameraManager_openCamera blocks) then
 * starts a repeating capture session asynchronously.
 * ------------------------------------------------------------------------- */

EhsCameraError EhsCameraStart(EhsCamera *camera, const ehs_char *id)
{
    if (!camera) return EHS_CAM_FAILED;
    if (camera->camera_ctx) return EHS_CAM_IN_USE;

    unsigned w = camera->width  > 0 ? camera->width  : 640;
    unsigned h = camera->height > 0 ? camera->height : 480;

    AndroidCamera *ctx = (AndroidCamera *)EhsHMem_permAlloc(sizeof(AndroidCamera));
    if (!ctx) return EHS_CAM_MEM_ERR;
    memset(ctx, 0, sizeof(AndroidCamera));
    ctx->width = w;
    ctx->height = h;

    /* 1. Camera manager */
    ctx->manager = ACameraManager_create();
    if (!ctx->manager) {
        EHSH_LOG_ERROR("Android camera: ACameraManager_create failed");
        goto err_free;
    }

    /* 2. Locate camera */
    char id_buf[64] = {0};
    const char *cam_id = find_camera_id(ctx->manager, id ? id : "", id_buf, sizeof(id_buf));
    if (!cam_id) {
        EHSH_LOG_ERROR("Android camera: no camera available");
        goto err_manager;
    }
    EHSH_LOG_INFO("Android camera: opening id='%s' %ux%u", cam_id, w, h);

    /* 3. Open device — ACameraManager_openCamera is synchronous in the NDK C
     *    API: it blocks until the device is ready and fills ctx->device.
     *    ACameraDevice_StateCallbacks carries disconnect/error callbacks only;
     *    there is no onOpened field. */
    ACameraDevice_StateCallbacks dev_cbs;
    memset(&dev_cbs, 0, sizeof(dev_cbs));
    dev_cbs.context        = ctx;
    dev_cbs.onDisconnected = dev_disconnected;
    dev_cbs.onError        = dev_error;

    camera_status_t status = ACameraManager_openCamera(ctx->manager, cam_id,
                                                        &dev_cbs, &ctx->device);
    if (status != ACAMERA_OK || !ctx->device) {
        EHSH_LOG_ERROR("Android camera: openCamera failed %d "
                       "(check android.permission.CAMERA is granted)", status);
        goto err_manager;
    }

    /* 4. Image reader — YUV_420_888, max 4 buffered images */
    if (AImageReader_new((int32_t)w, (int32_t)h,
                         AIMAGE_FORMAT_YUV_420_888, 4,
                         &ctx->reader) != AMEDIA_OK) {
        EHSH_LOG_ERROR("Android camera: AImageReader_new failed");
        goto err_device;
    }
    AImageReader_getWindow(ctx->reader, &ctx->window);
    /* window is owned by the reader — do NOT acquire/release separately */

    /* 5. Capture session outputs */
    ACaptureSessionOutput_create(ctx->window, &ctx->session_output);
    ACaptureSessionOutputContainer_create(&ctx->output_container);
    ACaptureSessionOutputContainer_add(ctx->output_container, ctx->session_output);

    /* 6. Capture request */
    ACameraDevice_createCaptureRequest(ctx->device, TEMPLATE_RECORD, &ctx->request);
    ACameraOutputTarget_create(ctx->window, &ctx->output_target);
    ACaptureRequest_addTarget(ctx->request, ctx->output_target);

    /* 7. Create session — asynchronous; sess_ready starts repeating request
     *    and transitions state to CAM_STATE_STREAMING. */
    ACameraCaptureSession_stateCallbacks sess_cbs;
    memset(&sess_cbs, 0, sizeof(sess_cbs));
    sess_cbs.context  = ctx;
    sess_cbs.onClosed = sess_closed;
    sess_cbs.onReady  = sess_ready;
    sess_cbs.onActive = sess_active;

    ctx->state = CAM_STATE_STARTING;
    status = ACameraDevice_createCaptureSession(ctx->device, ctx->output_container,
                                                &sess_cbs, &ctx->session);
    if (status != ACAMERA_OK) {
        EHSH_LOG_ERROR("Android camera: createCaptureSession failed: %d", status);
        goto err_request;
    }

    camera->camera_ctx = ctx;
    EHSH_LOG_INFO("Android camera: session starting — GrabFrame returns false while warming up");
    return EHS_CAM_OK;

err_request:
    if (ctx->output_target)    ACameraOutputTarget_free(ctx->output_target);
    if (ctx->request)          ACaptureRequest_free(ctx->request);
    if (ctx->output_container) ACaptureSessionOutputContainer_free(ctx->output_container);
    if (ctx->session_output)   ACaptureSessionOutput_free(ctx->session_output);
    if (ctx->reader)           AImageReader_delete(ctx->reader);  /* releases window */
err_device:
    ACameraDevice_close(ctx->device);
err_manager:
    ACameraManager_delete(ctx->manager);
err_free:
    EhsHMem_permFree(ctx);
    return EHS_CAM_OPEN_ERR;
}

/* -------------------------------------------------------------------------
 * EhsCameraStop / EhsCameraDestroy
 * ------------------------------------------------------------------------- */

void EhsCameraStop(EhsCamera *camera)
{
    if (!camera || !camera->camera_ctx) return;
    AndroidCamera *ctx = (AndroidCamera *)camera->camera_ctx;

    ctx->state = CAM_STATE_CLOSED;

    if (ctx->session) {
        ACameraCaptureSession_stopRepeating(ctx->session);
        ACameraCaptureSession_close(ctx->session);
        ctx->session = NULL;
    }
    if (ctx->output_target) {
        ACameraOutputTarget_free(ctx->output_target);
        ctx->output_target = NULL;
    }
    if (ctx->request) {
        ACaptureRequest_free(ctx->request);
        ctx->request = NULL;
    }
    if (ctx->output_container) {
        ACaptureSessionOutputContainer_free(ctx->output_container);
        ctx->output_container = NULL;
    }
    if (ctx->session_output) {
        ACaptureSessionOutput_free(ctx->session_output);
        ctx->session_output = NULL;
    }
    if (ctx->device) {
        ACameraDevice_close(ctx->device);
        ctx->device = NULL;
    }
    if (ctx->reader) {
        AImageReader_delete(ctx->reader);  /* also releases window */
        ctx->reader = NULL;
        ctx->window = NULL;
    }
    if (ctx->manager) {
        ACameraManager_delete(ctx->manager);
        ctx->manager = NULL;
    }

    EhsHMem_permFree(ctx);
    camera->camera_ctx = NULL;
}

void EhsCameraDestroy(EhsCamera *camera)
{
    EhsCameraStop(camera);
    if (camera) EhsMemset(camera, 0, sizeof(EhsCamera));
}

/* -------------------------------------------------------------------------
 * EhsCameraGrabFrame
 *
 * Acquires the latest image from the reader, converts YUV_420_888 to RGB,
 * and writes it into frame->frameObj.  Returns EHS_FALSE if no frame is
 * ready yet — normal during warm-up or when tick rate > camera frame rate.
 * ------------------------------------------------------------------------- */

ehs_bool EhsCameraGrabFrame(EhsCamera *camera, EhsCameraFrame *frame, ehs_bool show_image)
{
    (void)show_image;
    if (!camera || !camera->camera_ctx || !frame || !frame->frameObj) return EHS_FALSE;

    AndroidCamera *ctx = (AndroidCamera *)camera->camera_ctx;

    if (ctx->state == CAM_STATE_STARTING) return EHS_FALSE;
    if (ctx->state != CAM_STATE_STREAMING) {
        if (ctx->state == CAM_STATE_ERROR)
            EHSH_LOG_ERROR("Android camera: GrabFrame called in ERROR state");
        return EHS_FALSE;
    }

    AImage *image = NULL;
    media_status_t ms = AImageReader_acquireLatestImage(ctx->reader, &image);
    if (ms == AMEDIA_IMGREADER_NO_BUFFER_AVAILABLE || !image) return EHS_FALSE;
    if (ms != AMEDIA_OK) {
        EHSH_LOG_ERROR("Android camera: acquireLatestImage error %d", ms);
        return EHS_FALSE;
    }

    int32_t img_w = 0, img_h = 0;
    AImage_getWidth(image, &img_w);
    AImage_getHeight(image, &img_h);

    /* Y plane */
    uint8_t *y_data = NULL; int y_len = 0; int32_t y_stride = 0;
    AImage_getPlaneData(image, 0, &y_data, &y_len);
    AImage_getPlaneRowStride(image, 0, &y_stride);

    /* U (Cb) plane */
    uint8_t *u_data = NULL; int u_len = 0; int32_t u_stride = 0, u_px = 0;
    AImage_getPlaneData(image, 1, &u_data, &u_len);
    AImage_getPlaneRowStride(image, 1, &u_stride);
    AImage_getPlanePixelStride(image, 1, &u_px);

    /* V (Cr) plane */
    uint8_t *v_data = NULL; int v_len = 0; int32_t v_stride = 0, v_px = 0;
    AImage_getPlaneData(image, 2, &v_data, &v_len);
    AImage_getPlaneRowStride(image, 2, &v_stride);
    AImage_getPlanePixelStride(image, 2, &v_px);

    /* Grow output buffer if needed */
    AndroidFrame *af = (AndroidFrame *)frame->frameObj;
    size_t needed = (size_t)img_w * img_h * 3;
    if (!af->buf || af->buf_size < needed) {
        if (af->buf) EhsHMem_permFree(af->buf);
        af->buf = (uint8_t *)EhsHMem_permAlloc(needed);
        if (!af->buf) { AImage_delete(image); return EHS_FALSE; }
        af->buf_size = needed;
    }
    af->width    = (unsigned)img_w;
    af->height   = (unsigned)img_h;
    af->channels = 3;

    yuv420_to_rgb(y_data, y_stride,
                  u_data, u_stride, u_px,
                  v_data, v_stride, v_px,
                  img_w, img_h,
                  af->buf);

    AImage_delete(image);

    frame->width       = (ehs_uint32)img_w;
    frame->height      = (ehs_uint32)img_h;
    frame->fmt         = EHS_CAM_FMT_DEF;
    frame->opencl_mode = EHS_CAM_ACCELERATION_DISABLED;
    return EHS_TRUE;
}
