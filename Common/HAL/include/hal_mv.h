/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file hal_mv.h
 * In this file, all of the abstraction layer for machine vision.
 *
 * @author: inx limited
 *
 */


#ifndef _EHS_HAL_MV_H
#define _EHS_HAL_MV_H

/*****************************************************************************/
/* Included files */
#include "globals.h"

// Max number of frames that can be assigned to
// an image processing or camera function block
#ifndef EHS_CAM_MAX_FRAMES
#define EHS_CAM_MAX_FRAMES 100
#endif

typedef enum {
    EHS_CAM_OK = 0,
    EHS_CAM_FAILED,
    EHS_CAM_IN_USE,
    EHS_CAM_OPEN_ERR,
    EHS_CAM_MEM_ERR,
    EHS_CAM_NOT_FOUND   /* device node / path does not exist */
} EhsCameraError;

/**
 * OpenCL acceleration mode for camera frames.
 * When enabled, MV pipeline operations (resize, crop, convert) use cv::UMat
 * and run on the OpenCL GPU. The frame data stays on the GPU between operations.
 * ML inference frameworks (TFLite, Hailo) require CPU data — call
 * EhsCameraFrameEnsureCPU() before EhsML_SetInputData to sync GPU→CPU once.
 *
 * EHS_CAM_ACCELERATION_DISABLED (0) — default, all operations use cv::Mat on CPU.
 * EHS_CAM_ACCELERATION_ENABLED  (1) — MV operations use cv::UMat via OpenCL.
 */
typedef enum {
    EHS_CAM_ACCELERATION_DISABLED = 0,
    EHS_CAM_ACCELERATION_ENABLED  = 1,
    EHS_CAM_ACCELERATION_OPENCL   = 2,
    EHS_CAM_ACCELERATION_OPENVX   = 3,
    EHS_CAM_ACCELERATION_CUDA
} EhsCameraOpenCL_t;

typedef enum {
    EHS_CAM_FMT_DEF = 0,       // default
    EHS_CAM_FMT_8UC1,          // 8-bit unsigned, 1 channel (grayscale)
    EHS_CAM_FMT_32FC1_NORM,    // 32-bit float, 1 channels (normalised)
    EHS_CAM_FMT_32FC3_NORM,    // 32-bit float, 3 channels  (normalised)
    //EHS_CAM_FMT_8UC3,        // 8-bit unsigned, 3 channels
    //EHS_CAM_FMT_32FC3,       // 32-bit float, 3 channels
    //EHS_CAM_FMT_32FC1,       // 32-bit float, 1 channels
    // ...
    EHS_CAM_FMT_MAX
} EhsCameraDataFormat;

extern ehs_uint8 gEhsCameraDataFormatChanLen[EHS_CAM_FMT_MAX];

typedef struct {
    void*              frameObj;
    ehs_sint32         id;          /* frame slot index */
    ehs_uint32         width;
    ehs_uint32         height;
    EhsCameraDataFormat fmt;
    EhsCameraOpenCL_t  opencl_mode; /* EHS_CAM_ACCELERATION_DISABLED = cv::Mat (CPU);
                                     * EHS_CAM_ACCELERATION_ENABLED  = cv::UMat (OpenCL GPU).
                                     * Set by the Camera FB at startCamera time.
                                     * Propagated to all downstream MV frames (resize, crop, etc.).
                                     * Call EhsCameraFrameEnsureCPU() before passing to ML. */
} EhsCameraFrame;

typedef struct {
    void* camera_ctx;
    ehs_uint16 fps;
    ehs_uint32 width;
    ehs_uint32 height;
    ehs_bool   async;
    ehs_bool greyscale;
    // ... other ?
} EhsCamera;

//////////////////////////////////////////////////////////////////////////////////
// Camera functions

// Starts the camera with a given ID (e.g., device path or index).
// Returns an error code indicating success or failure.
EhsCameraError EhsCameraStart(EhsCamera* camera, const ehs_char* id);

// Stops a running camera and releases internal resources.
void EhsCameraStop(EhsCamera* camera);

// Completely destroys and deallocates a camera instance.
void EhsCameraDestroy(EhsCamera* camera);

// Captures a new frame from the camera and stores it into the given frame object.
// Returns true on success, false if no frame was captured.
ehs_bool EhsCameraGrabFrame(EhsCamera* camera, EhsCameraFrame* frame, ehs_bool show_image);

//////////////////////////////////////////////////////////////////////////////////
// Camera Frame (Stream) functions

// Initializes an empty camera frame structure (allocates buffers, sets defaults).
void EhsCameraFrameCreate(EhsCameraFrame* frame);

// Destroys a frame structure, releasing any associated memory.
void EhsCameraFrameDestroy(EhsCameraFrame* frame);

// Adds a frame to a global buffer and returns a unique frame ID.
ehs_sint32 EhsCameraFrameAdd(EhsCameraFrame* frame);

// Removes a previously added frame (by ID or reference) from a global buffer.
void EhsCameraFrameRemove(EhsCameraFrame* frame);

// Clears all managed frames from a global buffer.
void EhsCameraFrameClearAll();

// Retrieves a frame by its ID from a global buffer.
// Returns a pointer to the frame, or NULL if not found.
EhsCameraFrame* EhsCameraFrameGetById(ehs_sint32 id);

// Retrieves the raw frame data buffer and its size from a frame.
// Returns true on success, false if data is unavailable.
ehs_bool EhsCameraFrameGetData(EhsCameraFrame* frame, void** frame_data, ehs_uint32* frame_size);

/**
 * @brief Ensure frame data is in CPU-accessible memory (cv::Mat).
 *
 * If the frame was captured/processed with OpenCL (opencl_mode=EHS_CAM_ACCELERATION_ENABLED),
 * its internal buffer is a cv::UMat on the GPU. Call this once before passing
 * the frame to TFLite or Hailo via EhsML_SetInputData — it downloads the GPU
 * buffer to a CPU cv::Mat and clears opencl_mode so subsequent calls are no-ops.
 *
 * Safe to call on CPU frames (opencl_mode=EHS_CAM_ACCELERATION_DISABLED) — returns immediately.
 *
 * @return EHS_TRUE on success, EHS_FALSE if the download failed.
 */
ehs_bool EhsCameraFrameEnsureCPU(EhsCameraFrame* frame);

// Reads file image into 'frame' object from a file , suports jpg, png ...
// Returns true on success.
ehs_bool EhsCameraFrameReadFromFile(EhsCameraFrame* frame, const ehs_char* filepath);

// Writes 'frame' into an image file.
// Returns true on success.
ehs_bool EhsCameraFrameWriteToFile(EhsCameraFrame* frame, const ehs_char* filepath);

//////////////////////////////////////////////////////////////////////////////////
// Machine Vision Algorithm functions

// Crops a rectangular region from the source frame into the destination frame.
// Coordinates are inclusive: (x1, y1) to (x2, y2).
// Returns true on success.
ehs_bool EhsCameraFrameCrop(EhsCameraFrame* src, EhsCameraFrame* dst, ehs_uint32 x1, ehs_uint32 y1, ehs_uint32 x2, ehs_uint32 y2);

// Resizes the source frame to the given width and height, storing result in destination frame.
// interp: interpolation method (maps to cv::InterpolationFlags):
//   0 = INTER_NEAREST  fastest, recommended for ML pre-processing
//   1 = INTER_LINEAR   bilinear (default)
//   2 = INTER_CUBIC    bicubic, highest quality, slowest
//   3 = INTER_AREA     best quality when shrinking
// Returns true on success.
ehs_bool EhsCameraFrameResize(EhsCameraFrame* src, EhsCameraFrame* dst, ehs_uint32 width, ehs_uint32 height, ehs_sint32 interp);

// Converts the frame format (e.g., RGB to grayscale) and stores result in destination frame.
// Returns true on success.
ehs_bool EhsCameraFrameFormat(EhsCameraFrame* src, EhsCameraFrame* dst, EhsCameraDataFormat fmt);

// Deep-copy src frame into dst (dst must already have been created with EhsCameraFrameCreate).
// dst gets its own independent pixel buffer; src is not modified.
// Returns true on success.
ehs_bool EhsCameraFrameCopy(const EhsCameraFrame* src, EhsCameraFrame* dst);

// @TODO - add filtering functions e.g. Sobel, Threshold, Gauss, Morph etc.

/**
 * @brief Draw a bounding box and optional label on a frame in-place.
 *
 * Draws a rectangle from (x1, y1) to (x2, y2) with the given RGB colour and
 * line thickness. If label is non-NULL and non-empty, the text is drawn just
 * above the box top edge in the same colour.
 *
 * The frame is modified in-place. Coordinates are clamped to the frame bounds
 * by the HAL implementation. Coordinates may be signed to allow for detections
 * that partially overflow the frame edge.
 *
 * @param frame     Target frame (must have a valid frameObj).
 * @param x1        Left edge of the box in pixels.
 * @param y1        Top edge of the box in pixels.
 * @param x2        Right edge of the box in pixels (exclusive).
 * @param y2        Bottom edge of the box in pixels (exclusive).
 * @param r         Red channel (0-255).
 * @param g         Green channel (0-255).
 * @param b         Blue channel (0-255).
 * @param thickness Box outline thickness in pixels (>= 1).
 * @param label     Null-terminated label string drawn above the box, or NULL.
 * @return EHS_TRUE on success, EHS_FALSE on failure.
 */
ehs_bool EhsCameraFrameDrawBBox(EhsCameraFrame* frame,
                                 ehs_sint32 x1, ehs_sint32 y1,
                                 ehs_sint32 x2, ehs_sint32 y2,
                                 ehs_uint8 r, ehs_uint8 g, ehs_uint8 b,
                                 ehs_sint32 thickness,
                                 const ehs_char* label);


/**
 * @brief Callback type for embedded frame renderers.
 *
 * An embedded renderer (e.g. LVGL canvas) receives raw pixel data from a
 * camera frame and displays it within the native UI toolkit's scene, at the
 * specified position and scaled to the destination size.
 *
 * The callback is invoked from the frame_show display worker thread.
 * Implementations must be thread-safe (e.g. hold EhsTPMutex_viewport while
 * touching LVGL objects).
 *
 * @param pHandle  In/out opaque handle owned by the calling FB instance.
 *                 *pHandle is NULL on that instance's first call; the
 *                 implementation allocates whatever it needs (e.g. a canvas
 *                 plus backing buffer) and stores it back through *pHandle.
 *                 Later calls from the same instance pass the same handle
 *                 back, so each instance gets and keeps its own render
 *                 target rather than instances sharing (and fighting over)
 *                 one. Release it via EhsCamEmbeddedRendererReleaseFn when
 *                 the owning instance is destroyed.
 * @param x        Left edge of the target region in screen/UI coordinates.
 * @param y        Top edge of the target region.
 * @param dst_w    Width of the rendered image in pixels (0 = use frame width).
 * @param dst_h    Height of the rendered image in pixels (0 = use frame height).
 * @param data     Pointer to raw pixel bytes (BGR uint8 or grayscale uint8).
 * @param frame_w  Pixel width of the source frame.
 * @param frame_h  Pixel height of the source frame.
 * @param channels Bytes per pixel (3 = BGR, 1 = grayscale).
 */
typedef void (*EhsCamEmbeddedRendererFn)(
    void** pHandle,
    ehs_sint32 x, ehs_sint32 y,
    ehs_sint32 dst_w, ehs_sint32 dst_h,
    const void* data,
    ehs_sint32 frame_w, ehs_sint32 frame_h,
    ehs_sint32 channels);

/**
 * @brief Releases a handle previously produced by EhsCamEmbeddedRendererFn.
 * Called once, by the owning FB instance's destroy function. Must tolerate
 * a NULL handle (an instance that never actually rendered a frame).
 */
typedef void (*EhsCamEmbeddedRendererReleaseFn)(void* handle);

/**
 * Register an embedded frame renderer and its matching release function.
 * Pass NULL for both to clear. Only one renderer can be registered at a time.
 */
void EhsCameraFrameRegisterEmbeddedRenderer(EhsCamEmbeddedRendererFn fn, EhsCamEmbeddedRendererReleaseFn releaseFn);

/** Return the currently registered embedded renderer (may be NULL). */
EhsCamEmbeddedRendererFn EhsCameraFrameGetEmbeddedRenderer(void);

/** Return the currently registered renderer's release function (may be NULL). */
EhsCamEmbeddedRendererReleaseFn EhsCameraFrameGetEmbeddedRendererRelease(void);

#endif // _EHS_HAL_MV_H
