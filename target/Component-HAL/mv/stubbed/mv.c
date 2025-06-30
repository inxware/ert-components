
#include "hal_mv.h"


EhsCameraError EhsCameraStart(EhsCamera* camera, const ehs_char* id)
{
    return EHS_CAM_OK;
}

void EhsCameraStop(EhsCamera* camera)
{
}

void EhsCameraDestroy(EhsCamera* camera)
{
}

ehs_bool EhsCameraGrabFrame(EhsCamera* camera, EhsCameraFrame* frame)
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

ehs_bool EhsCameraFrameResize(EhsCameraFrame* src, EhsCameraFrame* dst, ehs_uint32 width, ehs_uint32 height)
{
    return EHS_TRUE;
}

ehs_bool EhsCameraFrameFormat(EhsCameraFrame* src, EhsCameraFrame* dst, EhsCameraDataFormat fmt)
{
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