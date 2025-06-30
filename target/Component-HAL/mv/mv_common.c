#include "globals.h"
#include "hal_mv.h"
#include "hal-api.h"

static EhsCameraFrame* gCameraFrames[EHS_CAM_MAX_FRAMES] = {0};

ehs_sint32 EhsCameraFrameAdd(EhsCameraFrame* frame)
{
    if(frame && frame->id < 0){ // only add frames that were not added or have been removed
        // search for a free slot
        for(int i = 0; i < EHS_CAM_MAX_FRAMES; i++){
            if(0 == gCameraFrames[i]){
                frame->id = i;
                gCameraFrames[i] = frame;
                return i;
            }
        }
    }
    return -1;
}

void EhsCameraFrameRemove(EhsCameraFrame* frame)
{
    if(!frame || frame->id < 0 || frame->id >= EHS_CAM_MAX_FRAMES) return;
    if(gCameraFrames[frame->id] == frame){
        gCameraFrames[frame->id] = 0;
        frame->id = -1;
    }else{
        // error
    }
}

EhsCameraFrame* EhsCameraFrameGetById(ehs_sint32 id)
{
    if(id >= 0 && id < EHS_CAM_MAX_FRAMES){
        return gCameraFrames[id];
    }
    return NULL;
}

void EhsCameraFrameClearAll()
{
    for(int i = 0; i < EHS_CAM_MAX_FRAMES; i++){
        if(gCameraFrames[i] != 0){
            gCameraFrames[i]->id = -1;
            gCameraFrames[i] = 0;
        }
    }
}