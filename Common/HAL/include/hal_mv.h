/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
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
#include "ehs_types.h"

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
    EHS_CAM_MEM_ERR
} EhsCameraError;

typedef enum {
    EHS_CAM_FMT_DEF = 0,       // default
    EHS_CAM_FMT_8UC1,          // 8-bit unsigned, 1 channel (grayscale)
    EHS_CAM_FMT_32FC1_NORM,    // 32-bit float, 1 channels (normalised)
    EHS_CAM_FMT_32FC3_NORM,    // 32-bit float, 3 channels  (normalised)
    //EHS_CAM_FMT_8UC3,        // 8-bit unsigned, 3 channels
    //EHS_CAM_FMT_32FC3,       // 32-bit float, 3 channels
    //EHS_CAM_FMT_32FC1,       // 32-bit float, 1 channels
    // ...
} EhsCameraDataFormat;

typedef struct {
    void* frameObj;
    ehs_sint32 id; // or index
    ehs_uint32 width;
    ehs_uint32 height;
    EhsCameraDataFormat fmt;
} EhsCameraFrame;

typedef struct {
    void* camera_ctx;
    ehs_uint16 fps;
    ehs_uint32 width;
    ehs_uint32 height;
    ehs_bool   async;
    // ehs_bool monochrome; // needed?
    // ... other ?
} EhsCamera;

//////////////////////////////////////////////////////////////////////////////////
// Camera functions

// Starts the camera with a given ID (e.g., device path or index).
// Returns an error code indicating success or failure.
EHS_GLOBAL EhsCameraError EhsCameraStart(EhsCamera* camera, const ehs_char* id);

// Stops a running camera and releases internal resources.
EHS_GLOBAL void EhsCameraStop(EhsCamera* camera);

// Completely destroys and deallocates a camera instance.
EHS_GLOBAL void EhsCameraDestroy(EhsCamera* camera);

// Captures a new frame from the camera and stores it into the given frame object.
// Returns true on success, false if no frame was captured.
EHS_GLOBAL ehs_bool EhsCameraGrabFrame(EhsCamera* camera, EhsCameraFrame* frame);

//////////////////////////////////////////////////////////////////////////////////
// Camera Frame (Stream) functions

// Initializes an empty camera frame structure (allocates buffers, sets defaults).
EHS_GLOBAL void EhsCameraFrameCreate(EhsCameraFrame* frame);

// Destroys a frame structure, releasing any associated memory.
EHS_GLOBAL void EhsCameraFrameDestroy(EhsCameraFrame* frame);

// Adds a frame to a global buffer and returns a unique frame ID.
EHS_GLOBAL ehs_sint32 EhsCameraFrameAdd(EhsCameraFrame* frame);

// Removes a previously added frame (by ID or reference) from a global buffer.
EHS_GLOBAL void EhsCameraFrameRemove(EhsCameraFrame* frame);

// Clears all managed frames from a global buffer.
EHS_GLOBAL void EhsCameraFrameClearAll();

// Retrieves a frame by its ID from a global buffer.
// Returns a pointer to the frame, or NULL if not found.
EHS_GLOBAL EhsCameraFrame* EhsCameraFrameGetById(ehs_sint32 id);

// Retrieves the raw frame data buffer and its size from a frame.
// Returns true on success, false if data is unavailable.
EHS_GLOBAL ehs_bool EhsCameraFrameGetData(EhsCameraFrame* frame, void** frame_data, ehs_uint32* frame_size);

// Reads file image into 'frame' object from a file , suports jpg, png ...
// Returns true on success.
EHS_GLOBAL ehs_bool EhsCameraFrameReadFromFile(EhsCameraFrame* frame, const ehs_char* filepath);

// Writes 'frame' into an image file.
// Returns true on success.
EHS_GLOBAL ehs_bool EhsCameraFrameWriteToFile(EhsCameraFrame* frame, const ehs_char* filepath);

//////////////////////////////////////////////////////////////////////////////////
// Machine Vision Algorithm functions

// Crops a rectangular region from the source frame into the destination frame.
// Coordinates are inclusive: (x1, y1) to (x2, y2).
// Returns true on success.
EHS_GLOBAL ehs_bool EhsCameraFrameCrop(EhsCameraFrame* src, EhsCameraFrame* dst, ehs_uint32 x1, ehs_uint32 y1, ehs_uint32 x2, ehs_uint32 y2);

// Resizes the source frame to the given width and height, storing result in destination frame.
// Returns true on success.
EHS_GLOBAL ehs_bool EhsCameraFrameResize(EhsCameraFrame* src, EhsCameraFrame* dst, ehs_uint32 width, ehs_uint32 height);

// Converts the frame format (e.g., RGB to grayscale) and stores result in destination frame.
// Returns true on success.
EHS_GLOBAL ehs_bool EhsCameraFrameFormat(EhsCameraFrame* src, EhsCameraFrame* dst, EhsCameraDataFormat fmt);

// @TODO - add filtering functions e.g. Sobel, Threshold, Gauss, Morph etc.


#endif // _EHS_HAL_MV_H