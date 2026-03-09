/***************************************************************
* Copyright (C) 2008-2025 inx limited - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*   <https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/** @file ehs_ml_objdet_output.h
 * Shared output-formatting utilities for object-detection model post-processors.
 *
 * Provides:
 *  - EhsML_Coco80_Label()            — COCO-80 class label lookup (0-indexed)
 *  - EhsML_ObjDet_Json_AppendCentre() — per-detection JSON fragment, centre format (x,y,w,h)
 *  - EhsML_ObjDet_Json_AppendCorner() — per-detection JSON fragment, corner format (ymin,xmin,ymax,xmax)
 *
 * All model post-processors that emit detection JSON should use these functions
 * rather than embedding format strings locally. This ensures:
 *  - Consistent field names across all model types
 *  - A single COCO label source of truth
 *  - Easier schema changes (one place to edit)
 *
 * JSON format conventions:
 *
 *   Centre format (YOLOv5-style):
 *     ,"cls0":0,"lbl0":"person","cnf0":0.87,"x0":320.0,"y0":240.0,"w0":100.0,"h0":80.0
 *     Leading comma. Coordinates are pixel-space centre + width/height.
 *     class_id is the raw integer index; lbl is the decoded label string.
 *
 *   Corner format (SSD/YOLOv8-style):
 *     "cls0":"person","cnf0":0.87,"ymin0":45.2,"xmin0":120.1,"ymax0":380.0,"xmax0":290.4,
 *     No leading comma, trailing comma. Coordinates are pixel-space corners.
 *     Only the label string is emitted (no separate integer class_id field).
 *
 * Both functions use EhsSnprintf internally and return the number of bytes
 * written (not including the null terminator), matching snprintf semantics.
 * Return value ≤ 0 indicates a full or errored buffer.
 */

#ifndef _C_HEADER_EHS_TARGET_HAL_ML_OUTPUT_EHS_ML_OBJDET_OUTPUT_H_
#define _C_HEADER_EHS_TARGET_HAL_ML_OUTPUT_EHS_ML_OBJDET_OUTPUT_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Look up the COCO-80 class label for a given class index.
 *
 * Uses the standard Ultralytics/COCO ordering, 0-indexed:
 *   0 = "person", 1 = "bicycle", ..., 79 = "toothbrush"
 *
 * @param class_id  0-based class index (0–79).
 * @return Pointer to a static string label, or "" for out-of-range indices.
 */
const char* EhsML_Coco80_Label(int class_id);

/**
 * @brief Append one detection in centre format to a JSON output buffer.
 *
 * Writes the fragment:
 *   ,"cls<idx>":<class_id>,"lbl<idx>":"<label>","cnf<idx>":<score>,
 *   "x<idx>":<cx>,"y<idx>":<cy>,"w<idx>":<w>,"h<idx>":<h>
 *
 * Leading comma is included so this can be appended directly after the
 * JSON object header (e.g. after {"type":N,"det_cnt":N).
 *
 * @param buf       Buffer to write into (at current write position).
 * @param size      Remaining space in the buffer including null terminator.
 * @param idx       Detection index (used as suffix in field names).
 * @param class_id  COCO-80 class index (0-indexed); used for both integer field and label lookup.
 * @param score     Confidence score (0.0–1.0).
 * @param cx        Box centre x in pixels.
 * @param cy        Box centre y in pixels.
 * @param w         Box width in pixels.
 * @param h         Box height in pixels.
 * @return Number of bytes written, or ≤ 0 if buffer is full.
 */
int EhsML_ObjDet_Json_AppendCentre(char* buf, int size, int idx,
                                    int class_id, float score,
                                    float cx, float cy, float w, float h);

/**
 * @brief Append one detection in corner format to a JSON output buffer.
 *
 * Writes the fragment:
 *   "cls<idx>":"<label>","cnf<idx>":<score>,
 *   "ymin<idx>":<ymin>,"xmin<idx>":<xmin>,"ymax<idx>":<ymax>,"xmax<idx>":<xmax>,
 *
 * No leading comma; trailing comma is included so the final "det_cnt" field
 * can follow directly.
 *
 * @param buf       Buffer to write into (at current write position).
 * @param size      Remaining space in the buffer including null terminator.
 * @param idx       Detection index (used as suffix in field names).
 * @param class_id  COCO-80 class index (0-indexed) for label lookup.
 * @param score     Confidence score (0.0–1.0).
 * @param ymin      Top edge in pixels.
 * @param xmin      Left edge in pixels.
 * @param ymax      Bottom edge in pixels.
 * @param xmax      Right edge in pixels.
 * @return Number of bytes written, or ≤ 0 if buffer is full.
 */
int EhsML_ObjDet_Json_AppendCorner(char* buf, int size, int idx,
                                    int class_id, float score,
                                    float ymin, float xmin,
                                    float ymax, float xmax);

#ifdef __cplusplus
}
#endif

#endif /* _C_HEADER_EHS_TARGET_HAL_ML_OUTPUT_EHS_ML_OBJDET_OUTPUT_H_ */
