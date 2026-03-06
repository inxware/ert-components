#include "ml_postprocessing_engine_hailo.hpp"

//#include "hailo_toolbox.hpp"

#include "hailo_infer.hpp"
#include "hailo/hailort.hpp"

#include "hailo_common.hpp"
#include "hailo_objects.hpp"

#include "yolov8pose_postprocess.hpp"

//using namespace hailo_utils;

constexpr size_t MAX_QUEUE_SIZE = 60;

#ifdef __cplusplus
extern "C" {
#endif

EhsML_Err EhsML_Postprocessing_Engine_Hailo_ProcessOutput(EhsML_Context* ctx)
{
    if (ctx == nullptr)
    {
        EHSH_LOG_ERROR("Invalid argument: ctx is NULL\n");
        return EHS_ML_MODEL_CTX_ERR;
    }
    EhsML_Tensor_t *intermediate_tensors = ctx->intermediate_tensor;
    EhsML_Tensor_t *output_tensors = ctx->output_tensor;
    EhsML_Tensor_t *input_tensor = ctx->input_tensor;

    const int org_width = input_tensor[0].dims[2];
    const int org_height = input_tensor[0].dims[1];

    // 1) ROI
    HailoROIPtr roi = std::make_shared<HailoROI>(HailoBBox(0.0f, 0.0f, 1.0f, 1.0f));

    // 2) Sort like the old pipeline:
    //    small -> large by area (20->40->80), then features 64->1->51
    //

    // 3) Add tensors ONCE (HailoTensor ctor takes hailo_tensor_metadata_t)
    //for (const auto &p : output_tensors)
    for (int i = 0; i < ctx->output_tensor_count; i++)
    {
        auto p = output_tensors[i];
        //auto hailo_tensor = std::make_shared<HailoTensor>(p.data_ptr.raw, p);
        auto hailo_vstream_info = static_cast<hailo_vstream_info_t*>(p.handle);
        auto hailo_tensor = std::make_shared<HailoTensor>(p.data_ptr.u8, static_cast<hailo_vstream_info_t&>(*hailo_vstream_info));
        roi->add_tensor(hailo_tensor);
    }

    // 4) Pose (requires dtype-aware decoder in yolov8pose_postprocess)
    auto keypoints_and_pairs = yolov8(roi);

    // 5) Detections
    auto detections = hailo_common::get_hailo_detections(roi);
    for (const auto &det : detections)
    {
        if (!det || det->get_confidence() <= 0.0f) continue;
        const HailoBBox b = det->get_bbox();
        printf("bbox: xmin=%f, ymin=%f, xmax=%f, ymax=%f\n", 
            b.xmin() * org_width, 
            b.ymin() * org_height, 
            b.xmax() * org_width, 
            b.ymax() * org_height
        );
        printf("label: %s, confidence: %f%%\n", det->get_label().c_str(), det->get_confidence() * 100.0f);
        ctx->detections[ctx->detection_count].x = b.xmin() * org_width;
        ctx->detections[ctx->detection_count].y = b.ymin() * org_height;
        ctx->detections[ctx->detection_count].w = (b.xmax() - b.xmin()) * org_width;
        ctx->detections[ctx->detection_count].h = (b.ymax() - b.ymin()) * org_height;
        strncpy(ctx->detections[ctx->detection_count].label, det->get_label().c_str(), sizeof(ctx->detections[ctx->detection_count].label) - 1);
        ctx->detection_count++;
    }

    for (PairPairs &p : keypoints_and_pairs.second)
    {
        float x1 = p.pt1.first * float(org_width);
        float y1 = p.pt1.second * float(org_height);
        float x2 = p.pt2.first * float(org_width);
        float y2 = p.pt2.second * float(org_height);
        printf("keypoint pair: (%f, %f) <-> (%f, %f)\n", x1, y1, x2, y2);
    }

    for (auto &keypoint : keypoints_and_pairs.first)
    {
        float x = keypoint.xs * float(org_width);
        float y = keypoint.ys * float(org_height);
        printf("keypoint: (%f, %f)\n", x, y);
    }

    return EHS_ML_OK;
}

#ifdef __cplusplus
}
#endif