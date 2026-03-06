#include "ehs_ml_logical.h"

static void filter_confidence(EhsML_Detection_t* detection, float conf_threshold)
{
    // Your confidence filtering code here
    if (detection->conf < conf_threshold)
    {
        detection->filtered = EHS_TRUE; // Mark as filtered out
    }
    else
    {
        detection->filtered = EHS_FALSE; // Mark as valid
    }
}

void logical_postprocess(EhsML_Context* ctx)
{
    // Your logical post-processing code here
    // This function should take the output tensors from the context, apply any necessary logical operations (e.g., NMS), and update the context with the final results.
    for (size_t i = 0; i < ctx->detection_count; i++)
    {
        filter_confidence(&ctx->detections[i], ctx->conf_thres);
    }
}