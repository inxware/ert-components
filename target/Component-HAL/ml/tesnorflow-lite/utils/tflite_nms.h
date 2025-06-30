#ifndef TFLITE_NMS_H
#define TFLITE_NMS_H

/* ---------- Non-Maximum-Suppression ------------------------------------- */

#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uint8_t */

/* Box is [x_center, y_center, width, height] */
typedef struct {
    float x, y, w, h;
    float score;      /* confidence (objectness * class prob) */
    int   class_id;   /* ≥0 */
} NMSBox;

/* --------------------------------------------------------------------------
 * TfLite_NMS()
 *
 * in_boxes      : array of N boxes
 * N             : number of boxes
 * conf_thr      : drop boxes if score < conf_thr
 * iou_thr       : IoU threshold for suppression
 * class_aware   : if non-zero, NMS is done per class
 *
 * keep_idx      : user-allocated array (length ≥ N) that will receive
 *                 the indices of boxes kept in the *original* order
 * max_keep      : capacity of keep_idx
 *
 * scratch_idx   : int buffer length N   (work area, will be overwritten)
 * scratch_flag  : uint8_t buffer length N   (work area, will be overwritten)
 *
 * returns number of boxes kept (written to keep_idx[0..ret-1])
 * -------------------------------------------------------------------------- */
size_t TfLite_NMS(const NMSBox* in_boxes,
                  size_t        N,
                  float         conf_thr,
                  float         iou_thr,
                  int           class_aware,
                  size_t*       keep_idx,
                  size_t        max_keep,
                  size_t*       scratch_idx,
                  uint8_t*      scratch_flag);

#endif /* TFLITE_NMS_H */
