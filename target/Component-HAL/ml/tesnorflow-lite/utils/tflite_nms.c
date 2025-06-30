#include "utils/tflite_nms.h"
#include <math.h>     /* fmaxf, fminf */

/* IoU for YOLO-style boxes ---------------------------------------------- */
static inline float iou(const NMSBox* a, const NMSBox* b)
{
    const float half = 0.5f;
    float ax1 = a->x - a->w * half, ay1 = a->y - a->h * half;
    float ax2 = a->x + a->w * half, ay2 = a->y + a->h * half;
    float bx1 = b->x - b->w * half, by1 = b->y - b->h * half;
    float bx2 = b->x + b->w * half, by2 = b->h * half + b->y;

    float iw = fmaxf(0.f, fminf(ax2, bx2) - fmaxf(ax1, bx1));
    float ih = fmaxf(0.f, fminf(ay2, by2) - fmaxf(ay1, by1));
    float inter = iw * ih;
    float uni = a->w * a->h + b->w * b->h - inter + 1e-6f;
    return inter / uni;
}

/* simple swap util for sorting indices by score ------------------------- */
static inline void swap(size_t* a, size_t* b)
{
    size_t t = *a; *a = *b; *b = t;
}

/* ---------------------------------------------------------------------- */
size_t TfLite_NMS(const NMSBox* in_boxes,
                  size_t        N,
                  float         conf_thr,
                  float         iou_thr,
                  int           class_aware,
                  size_t*       keep_idx,
                  size_t        max_keep,
                  size_t*       idx,          /* scratch */
                  uint8_t*      suppressed)   /* scratch */
{
    /* 1) build list of candidates above conf threshold ------------------ */
    size_t pool = 0;
    for(size_t i = 0; i < N; ++i)
        if(in_boxes[i].score >= conf_thr)
            idx[pool++] = i;

    if(pool == 0) return 0;

    /* 2) insertion-sort indices by score (descending) ------------------- */
    for(size_t i = 1; i < pool; ++i){
        size_t j = i;
        while(j && in_boxes[idx[j-1]].score < in_boxes[idx[j]].score){
            swap(&idx[j-1], &idx[j]);
            --j;
        }
    }

    /* 3) greedy NMS ----------------------------------------------------- */
    size_t kept = 0;
    for(size_t i = 0; i < pool && kept < max_keep; ++i) suppressed[i] = 0;

    for(size_t _i = 0; _i < pool && kept < max_keep; ++_i){
        if(suppressed[_i]) continue;

        size_t i_idx = idx[_i];
        keep_idx[kept++] = i_idx;

        for(size_t _j = _i + 1; _j < pool; ++_j){
            if(suppressed[_j]) continue;

            size_t j_idx = idx[_j];
            if(class_aware &&
               in_boxes[i_idx].class_id != in_boxes[j_idx].class_id)
                continue;

            if(iou(&in_boxes[i_idx], &in_boxes[j_idx]) > iou_thr)
                suppressed[_j] = 1;
        }
    }
    return kept;
}
