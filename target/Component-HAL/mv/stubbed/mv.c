
#include "hal_mv.h"

/* For MV hello world function */
#ifdef EHS_CPPMV_SUPPORT_TEST
#include "mv_hello.h"
#endif

/* This is used by clients of machine vision systems */
/* todo move this to a cpmmon file as this is not target specific */
ehs_uint8 gEhsCameraDataFormatChanLen[EHS_CAM_FMT_MAX] = {
    1,      // EHS_CAM_FMT_DEF
    1,      // EHS_CAM_FMT_8UC1
    1,      // EHS_CAM_FMT_32FC1_NORM
    3,      // EHS_CAM_FMT_32FC3_NORM
    //3,      // EHS_CAM_FMT_8UC3
    //3,      // EHS_CAM_FMT_32FC3
    //1,      // EHS_CAM_FMT_32FC1
};

EhsCameraError EhsCameraStart(EhsCamera* camera, const ehs_char* id)
{
//Test C++ APIs in the stubbed version only
#ifdef EHS_CPPMV_SUPPORT_TEST
 hello_world();
#endif

    return EHS_CAM_OK;
}

void EhsCameraStop(EhsCamera* camera)
{
}

void EhsCameraDestroy(EhsCamera* camera)
{
}

ehs_bool EhsCameraGrabFrame(EhsCamera* camera, EhsCameraFrame* frame, ehs_bool show_image)
{
    return EHS_TRUE;
}

void EhsCameraFrameCreate(EhsCameraFrame* frame)
{
}

void EhsCameraFrameDestroy(EhsCameraFrame* frame)
{
}

ehs_bool EhsCameraFrameGetData(EhsCameraFrame* frame, void** frame_data, ehs_uint32* frame_size)
{
    return EHS_TRUE;
}

ehs_bool EhsCameraFrameCrop(EhsCameraFrame* src, EhsCameraFrame* dst, ehs_uint32 x1, ehs_uint32 y1, ehs_uint32 x2, ehs_uint32 y2)
{
    return EHS_TRUE;
}

ehs_bool EhsCameraFrameResize(EhsCameraFrame* src, EhsCameraFrame* dst, ehs_uint32 width, ehs_uint32 height, ehs_sint32 interp)
{
    (void)interp; /* stubbed — interp unused */
    return EHS_TRUE;
}

ehs_bool EhsCameraFrameFormat(EhsCameraFrame* src, EhsCameraFrame* dst, EhsCameraDataFormat fmt)
{
    return EHS_TRUE;
}

ehs_bool EhsCameraFrameEnsureCPU(EhsCameraFrame* frame)
{
    (void)frame; /* stubbed — no GPU on this target, frame is always CPU */
    return EHS_TRUE;
}

ehs_bool EhsCameraFrameReadFromFile(EhsCameraFrame* frame, const ehs_char* filepath)
{
    return EHS_TRUE;
}

ehs_bool EhsCameraFrameWriteToFile(EhsCameraFrame* frame, const ehs_char* filepath)
{
    return EHS_TRUE;
}