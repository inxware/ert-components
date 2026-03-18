/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/**
 * @file jetson_frame_pool.h
 * @brief Shared Jetson CUDA frame slot table — visible to both mv/jetson and ml/tensorrt.
 *
 * When the GStreamer camera pipeline delivers a frame via appsink, mv/jetson maps
 * the NvBufSurface buffer to a CUDA device pointer and registers it here.
 * ml/tensorrt reads the CUDA device pointer directly, avoiding a device→CPU→device
 * round-trip when running TensorRT inference on a frame that was captured via NVMM.
 *
 * Slots are reference-counted: the MV layer calls jetson_frame_pool_ref() when it
 * registers a frame and jetson_frame_pool_unref() when the frame is discarded.
 * The ML layer calls jetson_frame_pool_ref() before using a slot and
 * jetson_frame_pool_unref() when inference is complete.
 */

#ifndef JETSON_FRAME_POOL_H
#define JETSON_FRAME_POOL_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef EHS_CAM_MAX_FRAMES
#define EHS_CAM_MAX_FRAMES 100
#endif

/**
 * One slot in the Jetson frame pool.
 *
 * mv/jetson fills these fields when grabbing frames from the GStreamer appsink.
 * ml/tensorrt reads cuda_dev_ptr to feed TensorRT without a device→CPU→device copy.
 *
 * If cuda_dev_ptr is NULL the frame was captured without NVMM (e.g. USB camera via
 * v4l2src without nvvidconv NVMM output) and the ML layer must fall back to the
 * cpu_ptr path.
 */
typedef struct {
    void       *gst_sample;     /* GstSample* — held ref, released in unref */
    void       *cuda_dev_ptr;   /* NvBufSurface CUDA device pointer (NULL for non-NVMM) */
    void       *cpu_ptr;        /* CPU-mapped pixel buffer (may be same allocation) */
    size_t      cpu_size;       /* byte size of cpu_ptr */
    unsigned    width;
    unsigned    height;
    int         channels;       /* bytes per pixel */
    int         ref_count;      /* 0 = free; >0 = in use by MV and/or ML */
} JetsonFrameSlot;

/** Global frame pool — defined in target/Component-HAL/mv/jetson/mv.c */
extern JetsonFrameSlot g_jetson_frame_pool[EHS_CAM_MAX_FRAMES];

/**
 * Claim a free slot and set ref_count to 1.
 * @return Slot index [0, EHS_CAM_MAX_FRAMES), or -1 if all slots are occupied.
 */
int  jetson_frame_pool_alloc(void);

/**
 * Increment the reference count of slot @p idx.
 * Call this before handing a slot index to the ML layer.
 */
void jetson_frame_pool_ref(int idx);

/**
 * Decrement the reference count of slot @p idx.
 * When the count reaches zero the slot is cleared and the GstSample is released.
 */
void jetson_frame_pool_unref(int idx);

/**
 * Convenience: return a pointer to slot @p idx without bounds checking.
 * The caller must hold at least one reference.
 */
static inline JetsonFrameSlot *jetson_frame_pool_get(int idx)
{
    return &g_jetson_frame_pool[idx];
}

#ifdef __cplusplus
}
#endif

#endif /* JETSON_FRAME_POOL_H */
