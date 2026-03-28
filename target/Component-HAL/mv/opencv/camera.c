
#include "globals.h"
#include "hal_mv.h"
#include "hal-api.h"
#include "hal_logger.h"

#include <opencv/opencv_wrapper.h>

// Enable this for displaying every grabbed frame in a preview window
// NOTE ! - this works only with a single camera instance
//#ifndef EHS_OPENCV_FRAMEGRAB_DEBUG_PREVIEW
//#define EHS_OPENCV_FRAMEGRAB_DEBUG_PREVIEW
//#endif

// If set to '1' it uses a thread to capture frames, so EhsCameraGrabFrame captures without blocking.
// This seems to make things much more stable, when grabbing frames from the EhsMain thread.
#ifndef EHS_OPENCV_ASYNC_FRAME_CAP
#define EHS_OPENCV_ASYNC_FRAME_CAP 1
#endif

// Number of buffers in the camera queue. Set this to low if 'dropping frames' is prefered over 'laggy stream'
// Set to '0' for - use whatever opencv does by default
#ifndef EHS_OPENCV_CAMERA_BUFFER_COUNT
#define EHS_OPENCV_CAMERA_BUFFER_COUNT 0
#endif

int is_unsigned_integer(const char *str, unsigned int *out_value) {
    if (str == NULL || *str == '\0') return 0;

    unsigned int value = 0;

    while (*str) {
        if (!isdigit((unsigned char)*str)) return 0;

        value = value * 10 + (*str - '0');
        str++;
    }

    if (out_value) *out_value = value;

    return 1;
}
// check if the camera id is a +ve integer which indicates opencv camera id
int EhsCamera_is_opencv_device_id(const char *str, unsigned int *device_id) {
    if (str == NULL || *str == '\0' || device_id == NULL) return 0;

    const char *start = str;
    while (*str) {
        if (!isdigit((unsigned char)*str)) return 0;
        str++;
    }

    *device_id = (unsigned int)atol(start);

    return 1;
}

EhsCameraError EhsCameraStart(EhsCamera* camera, const ehs_char* id)
{
    EhsCameraError err = EHS_CAM_FAILED;
    if(!camera) goto cam_fail;

    // check if open
    if(camera->camera_ctx){
        err = EHS_CAM_IN_USE;
        goto cam_fail;
    }

    camera->camera_ctx = (void*)EhsHMem_permAlloc(sizeof(cv_camera));
    if(!camera->camera_ctx){
        err = EHS_CAM_MEM_ERR;
        goto cam_fail;
    }
    EhsMemset(camera->camera_ctx, 0, sizeof(cv_camera));
    int ret; unsigned int device_id;
    if(EhsCamera_is_opencv_device_id(id, &device_id)){
        ret = cv_cam_open((cv_camera*)camera->camera_ctx, device_id, 
                camera->width, camera->height, camera->fps, EHS_OPENCV_CAMERA_BUFFER_COUNT, camera->greyscale, EHS_OPENCV_ASYNC_FRAME_CAP);
    }else{
        ret = cv_cam_path((cv_camera*)camera->camera_ctx, id,
                camera->width, camera->height, camera->fps, EHS_OPENCV_CAMERA_BUFFER_COUNT, camera->greyscale, EHS_OPENCV_ASYNC_FRAME_CAP);
    }
    if(ret != CV_CAM_OK){
        if (ret == CV_CAM_NOT_FOUND_ERR) {
            EHSH_LOG_ERROR("camera: device '%s' not found", id ? id : "(null)");
            err = EHS_CAM_NOT_FOUND;
        } else if (ret == CV_CAM_EXCEPTION_ERR) {
            EHSH_LOG_ERROR("camera: library exception opening device '%s' (libcamera OpenCamera failed)", id ? id : "(null)");
            err = EHS_CAM_OPEN_ERR;
        } else {
            EHSH_LOG_ERROR("camera: failed to open device '%s' (cv_cam err=%d)", id ? id : "(null)", ret);
            err = EHS_CAM_OPEN_ERR;
        }
        EhsHMem_permFree(camera->camera_ctx);
        camera->camera_ctx = 0;
        goto cam_fail;
    }

    return EHS_CAM_OK;

cam_fail:
    return err;
}

void EhsCameraStop(EhsCamera* camera)
{
    if(camera && camera->camera_ctx){
        cv_cam_close((cv_camera*)camera->camera_ctx);
        EhsHMem_permFree(camera->camera_ctx);
        camera->camera_ctx = 0;
    }
}

void EhsCameraDestroy(EhsCamera* camera)
{
    EhsCameraStop(camera);
    EhsMemset(camera,0,sizeof(EhsCamera));

#ifdef EHS_OPENCV_FRAMEGRAB_DEBUG_PREVIEW
    cv_mat_destroy_all_windows();
#endif
}

ehs_bool EhsCameraGrabFrame(EhsCamera* camera, EhsCameraFrame* frame, ehs_bool show_image)
{
    if(!camera || !camera->camera_ctx || !frame || !frame->frameObj) return EHS_FALSE;

    cv_mat* cv_frameObj = (cv_mat*)frame->frameObj;
    cv_mat_release(cv_frameObj);
    /* Use OpenCL (UMat) or CPU (Mat) capture depending on frame->opencl_mode. */
    /* For OpenCL builds the default acceleration mode is OpenCL */
    int read_result = ((frame->opencl_mode == EHS_CAM_ACCELERATION_ENABLED) || (frame->opencl_mode == EHS_CAM_ACCELERATION_OPENCL))
        ? cv_cam_read_opencl((cv_camera*)camera->camera_ctx, cv_frameObj)
        : cv_cam_read       ((cv_camera*)camera->camera_ctx, cv_frameObj);
    if(read_result == CV_CAM_OK){
        frame->width = cv_frameObj->width;
        frame->height = cv_frameObj->height;
        frame->fmt = EHS_CAM_FMT_DEF;
#ifdef EHS_OPENCV_FRAMEGRAB_DEBUG_PREVIEW
//Note we don't want to do this usually unless we are debugging
        if (show_image)
        {
            static char label[256];
            static EhsTickType sFpsTimer = 0;
            static float sAvrgFpsSample = 0, sAvrgFpsShow = 0; // avarage of a number of samples
            static long sAvrgFpsCounter = 0;
            EhsTickType curr = EHS_CURRENT_TIME;
            float fps = 1000000.f/(float)(curr - sFpsTimer);
            if(sAvrgFpsCounter > 10) { // set number of samples for avrage
                sAvrgFpsShow = sAvrgFpsSample/(float)sAvrgFpsCounter;
                sAvrgFpsSample = 0;
                sAvrgFpsCounter = 0;
            }else {
                sAvrgFpsSample += fps;
                sAvrgFpsCounter++;
            }
            sFpsTimer = curr;
            if(snprintf(label, sizeof(label), "fps: %.2f (%.2f)", fps, sAvrgFpsShow)){
                cv_mat_draw_text(cv_frameObj, label, 10, 20, 0.5, 255, 0, 0, 1);
            }        
            if(snprintf(label, sizeof(label), "debug_cam_%d", frame->id)){
                cv_mat_show(label, cv_frameObj, 1);
            }
        }
#endif
        return EHS_TRUE;
    }
    return EHS_FALSE;
}