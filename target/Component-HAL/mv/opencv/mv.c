
#include "globals.h"
#include "hal_mv.h"
#include "hal-api.h"

#include <opencv/opencv_wrapper.h>

// Machine Vision Algorithms

void EhsCameraFrameCreate(EhsCameraFrame* frame)
{
    if(frame){
        EhsMemset(frame, 0, sizeof(EhsCameraFrame));
        frame->id = -1; // new frame, needs to be assigned to the frame list 
        frame->frameObj =  (void*)EhsHMem_permAlloc(sizeof(cv_mat));
        EhsMemset(frame->frameObj, 0, sizeof(cv_mat));
    }
}

void EhsCameraFrameDestroy(EhsCameraFrame* frame)
{
    if(frame && frame->frameObj){
        cv_mat_release((cv_mat*)frame->frameObj);
        EhsHMem_permFree(frame->frameObj);
        EhsMemset(frame, 0, sizeof(EhsCameraFrame));
    }
}


ehs_bool EhsCameraFrameGetData(EhsCameraFrame* frame, void** frame_data, ehs_uint32* frame_size)
{
    if(!frame || !frame->frameObj || !frame_data || !frame_size) return EHS_FALSE;

    cv_mat* cv_frameObj = (cv_mat*)frame->frameObj;
    
    *frame_size = cv_mat_size_bytes(cv_frameObj);    
    
    *frame_data = cv_mat_data(cv_frameObj);

    return EHS_TRUE;
}

ehs_bool EhsCameraFrameReadFromFile(EhsCameraFrame* frame, const ehs_char* filepath)
{
    if(!frame || !frame->frameObj || !filepath || *filepath == '\0') return EHS_FALSE;
    cv_mat_release((cv_mat*)frame->frameObj);
    return (cv_mat_read(filepath, (cv_mat*)frame->frameObj) == CV_CAM_OK) ? EHS_TRUE : EHS_FALSE;
}

ehs_bool EhsCameraFrameWriteToFile(EhsCameraFrame* frame, const ehs_char* filepath)
{
    if(!frame || !frame->frameObj || !filepath || *filepath == '\0') return EHS_FALSE;

    return cv_mat_write(filepath, (cv_mat*)frame->frameObj) == CV_CAM_OK ? EHS_TRUE : EHS_FALSE;
}

ehs_bool EhsCameraFrameCrop(EhsCameraFrame* src, EhsCameraFrame* dst, ehs_uint32 x1, ehs_uint32 y1, ehs_uint32 x2, ehs_uint32 y2)
{
    if(!src || !src->frameObj || !dst || !dst->frameObj || (x1 > x2) || (y1 > y2)) return EHS_FALSE;
    ehs_uint32 width = x2-x1, height = y2-y1;
    cv_mat_release((cv_mat*)dst->frameObj);
    if(CV_CAM_OK == cv_mat_crop((cv_mat*)src->frameObj, (cv_mat*)dst->frameObj, x1, y1, width, height)){
#ifdef EHS_OPENCV_DEBUG_PREVIEW
 //      cv_mat_show("debug_crop", (cv_mat*)dst->frameObj, 1);
#endif
        return EHS_TRUE;
    }
    return EHS_FALSE;
}

ehs_bool EhsCameraFrameResize(EhsCameraFrame* src, EhsCameraFrame* dst, ehs_uint32 width, ehs_uint32 height)
{
    if(!src || !src->frameObj || !dst || !dst->frameObj) return EHS_FALSE;
    cv_mat_release((cv_mat*)dst->frameObj);
    if(CV_CAM_OK == cv_mat_resize((cv_mat*)src->frameObj, (cv_mat*)dst->frameObj, width, height)){
#ifdef EHS_OPENCV_DEBUG_PREVIEW
//       cv_mat_show("debug_resize", (cv_mat*)dst->frameObj, 1);
#endif
        return EHS_TRUE;
    }
    return EHS_FALSE;
}

ehs_bool EhsCameraFrameFormat(EhsCameraFrame* src, EhsCameraFrame* dst, EhsCameraDataFormat fmt)
{
    if(!src || !src->frameObj || !dst || !dst->frameObj) return EHS_FALSE;
    ehs_bool ret = EHS_TRUE;
    cv_mat_release((cv_mat*)dst->frameObj);
    switch (fmt)
    {
    case EHS_CAM_FMT_8UC1:
        ret = (CV_CAM_OK == cv_mat_to_grayscale((cv_mat*)src->frameObj, (cv_mat*)dst->frameObj)) ? EHS_TRUE : EHS_FALSE;
        break;
    case EHS_CAM_FMT_32FC1_NORM:
        ret = (CV_CAM_OK == cv_mat_convert_to((cv_mat*)src->frameObj, (cv_mat*)dst->frameObj, CV_TYPE_32FC1, 1 / 255.0, 0)) ? EHS_TRUE : EHS_FALSE;
        break;
    case EHS_CAM_FMT_32FC3_NORM:
        ret = (CV_CAM_OK == cv_mat_convert_to((cv_mat*)src->frameObj, (cv_mat*)dst->frameObj, CV_TYPE_32FC3, 1 / 255.0, 0)) ? EHS_TRUE : EHS_FALSE;
        break;
    default:
        ret = EHS_FALSE;
        break;
    }
    return ret;
}
