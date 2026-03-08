#ifndef _ERT_HAL_TFLITE_META_H_
#define _ERT_HAL_TFLITE_META_H_

#include "ert_hal_tflite.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Build a JSON string describing a loaded TFLite model.
 *
 * Combines TFLite C-API tensor introspection with minimal FlatBuffer
 * parsing of the model file to produce a self-contained JSON object.
 *
 * Output format (example):
 * {
 *   "runtime":"2.14.0",
 *   "description":"...",
 *   "input_count":1, "output_count":1,
 *   "inputs":[{"name":"serving_default_x:0","dtype":"float32",
 *              "shape":[1,640,640,3],"bytes":4915200,
 *              "quant_scale":0.0,"quant_zp":0}],
 *   "outputs":[{"name":"PartitionedCall:0","dtype":"float32",
 *               "shape":[1,25200,85],"bytes":8568000,
 *               "quant_scale":0.0,"quant_zp":0}],
 *   "inferred_type":"yolov5-coco80",
 *   "xnnpack":true,
 *   "metadata":[{"name":"min_runtime_version","value":"1.5.0"}]
 * }
 *
 * @param tfl        Populated TfLiteModelCtx (interp must be allocated).
 * @param model_path Path to the .tflite file (for metadata parsing).
 * @param json_buf   Output buffer.
 * @param json_size  Size of output buffer in bytes.
 * @return EHS_ML_OK on success.
 */
EhsML_Err EhsML_TFLite_GetModelInfoJson(TfLiteModelCtx*  tfl,
                                         const ehs_char*  model_path,
                                         ehs_char*        json_buf,
                                         ehs_uint32       json_size);

#ifdef __cplusplus
}
#endif

#endif /* _ERT_HAL_TFLITE_META_H_ */
