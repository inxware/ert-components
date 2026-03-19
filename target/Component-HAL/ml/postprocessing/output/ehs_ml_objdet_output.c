/***************************************************************
* Copyright (C) 2008-2025 inx limited - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*   <https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/** @file ehs_ml_objdet_output.c
 * Shared output-formatting implementation for object-detection post-processors.
 *
 * See ehs_ml_objdet_output.h for the full API description.
 */

#include "hal-api.h"
#include "ehs_ml_objdet_output.h"

/*****************************************************************************/
/* COCO-80 class label table
 *
 * Standard Ultralytics / COCO ordering, 0-indexed.
 * Shared by all model post-processors — do not duplicate this table.
 * To add support for a different label set, add a new function alongside
 * EhsML_Coco80_Label() rather than modifying this table.
 */
#define EHS_ML_COCO80_COUNT 80
static const char* const kCoco80Labels[EHS_ML_COCO80_COUNT] = {
    /* 0–4  */  "person",       "bicycle",      "car",          "motorcycle",   "airplane",
    /* 5–9  */  "bus",          "train",        "truck",        "boat",         "traffic light",
    /* 10–14 */ "fire hydrant", "stop sign",    "parking meter","bench",        "bird",
    /* 15–19 */ "cat",          "dog",          "horse",        "sheep",        "cow",
    /* 20–24 */ "elephant",     "bear",         "zebra",        "giraffe",      "backpack",
    /* 25–29 */ "umbrella",     "handbag",      "tie",          "suitcase",     "frisbee",
    /* 30–34 */ "skis",         "snowboard",    "sports ball",  "kite",         "baseball bat",
    /* 35–39 */ "baseball glove","skateboard",  "surfboard",    "tennis racket","bottle",
    /* 40–44 */ "wine glass",   "cup",          "fork",         "knife",        "spoon",
    /* 45–49 */ "bowl",         "banana",       "apple",        "sandwich",     "orange",
    /* 50–54 */ "broccoli",     "carrot",       "hot dog",      "pizza",        "donut",
    /* 55–59 */ "cake",         "chair",        "couch",        "potted plant", "bed",
    /* 60–64 */ "dining table", "toilet",       "tv",           "laptop",       "mouse",
    /* 65–69 */ "remote",       "keyboard",     "cell phone",   "microwave",    "oven",
    /* 70–74 */ "toaster",      "sink",         "refrigerator", "book",         "clock",
    /* 75–79 */ "vase",         "scissors",     "teddy bear",   "hair drier",   "toothbrush"
};

const char* EhsML_Coco80_Label(int class_id)
{
    if (class_id >= 0 && class_id < EHS_ML_COCO80_COUNT)
    {
        return kCoco80Labels[class_id];
    }
    return "";
}

/*****************************************************************************/
/* JSON formatting */

int EhsML_ObjDet_Json_AppendCentre(char* buf, int size, int idx,
                                    int class_id, float score,
                                    float cx, float cy, float w, float h)
{
    return EhsSnprintf(buf, size,
        ",\"cls%d\":%d,\"lbl%d\":\"%s\",\"cnf%d\":%.2f"
        ",\"x%d\":%.1f,\"y%d\":%.1f,\"w%d\":%.1f,\"h%d\":%.1f",
        idx, class_id,
        idx, EhsML_Coco80_Label(class_id),
        idx, score,
        idx, cx,
        idx, cy,
        idx, w,
        idx, h
    );
}

int EhsML_ObjDet_Json_AppendCorner(char* buf, int size, int idx,
                                    int class_id, float score,
                                    float ymin, float xmin,
                                    float ymax, float xmax)
{
    return EhsSnprintf(buf, size,
        "\"cls%d\":\"%s\",\"cnf%d\":%.2f"
        ",\"ymin%d\":%.2f,\"xmin%d\":%.2f,\"ymax%d\":%.2f,\"xmax%d\":%.2f,",
        idx, EhsML_Coco80_Label(class_id),
        idx, score,
        idx, ymin,
        idx, xmin,
        idx, ymax,
        idx, xmax
    );
}

EhsML_Err EhsML_ObjDet_Json_FromDetections(EhsML_Context *ctx,
                                            ehs_char      *json_buf,
                                            ehs_uint32     json_size)
{
    ehs_uint32 used = EhsSnprintf(json_buf, json_size,
        "{\"type\":%d,\"det_cnt\":%d",
        (int)ctx->type, (int)ctx->detection_count);
    if (used >= json_size) return EHS_ML_JSON_STRSIZE_ERR;

    for (ehs_uint32 i = 0; i < ctx->detection_count; i++)
    {
        const EhsML_Detection_t *d = &ctx->detections[i];
        if (d->filtered) continue;
        int remaining = (int)(json_size - used - 1);
        if (remaining <= 0) break;
        int len = EhsML_ObjDet_Json_AppendCentre(
            json_buf + used, remaining, (int)i,
            (int)d->cls, d->conf,
            d->x, d->y, d->w, d->h);
        if (len > 0) used += (ehs_uint32)len;
    }
    EhsSnprintf(json_buf + used, json_size - used, "}");
    return EHS_ML_OK;
}
