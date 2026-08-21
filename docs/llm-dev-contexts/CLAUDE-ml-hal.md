# ML HAL Development Guide

Reference for working on the machine learning HAL layer and ML function blocks in eRT.

> **CDF port argument numbers:** canonical spec in `../ert-porting-guide.md` § "Port Argument Numbers" — each port type has its own independent 1..N range; `argument="0"` is the trigger.

---

## Architecture Overview

```
inx-tf_lite_frame.c          (function block — Common/Components/ml/)
    └─ EhsML_Create / EhsML_SetInputData / EhsML_RunOutputJson / EhsML_GetModelInfoJson
           └─ ml_common.c                (HAL public API — target/Component-HAL/ml/)
                  └─ yolov5_objdet.c / yolov8_objdet.c   (model-type dispatch)
                         └─ ml_model_common.c             (hw_accel dispatch)
                                └─ ert_hal_tflite.c       (TFLite C API)
                                   ert_hal_tflite_meta.c  (model metadata / JSON)
```

### Key files

| File | Purpose |
|---|---|
| `Common/HAL/include/hal_ml.h` | Public API — `EhsML_*` functions, error enum, context struct |
| `target/Component-HAL/ml/ml_common.c` | Implements public API, dispatches by model type |
| `target/Component-HAL/ml/ml_common.h` | Includes model-type headers (yolov5, yolov8, etc.) |
| `target/Component-HAL/ml/postprocessing/model/ml_model_common.c` | hw_accel dispatch (Hailo / TFLite fallback) |
| `target/Component-HAL/ml/framework/tensorflow-lite/ert_hal_tflite.c` | TFLite C API integration |
| `target/Component-HAL/ml/framework/tensorflow-lite/ert_hal_tflite_meta.c` | FlatBuffer parser + model info JSON |
| `target/Component-HAL/ml/framework/tensorflow-lite/ml_fw_tflite.mk` | Build — adds `ert_hal_tflite.$(OBJ)` and `ert_hal_tflite_meta.$(OBJ)` |

---

## Error Codes (`EhsML_Err` in `hal_ml.h`)

| Code | Meaning |
|---|---|
| `EHS_ML_OK` | Success |
| `EHS_ML_FAILED` | Generic failure |
| `EHS_ML_INIT_ERR` | Model not loaded / context not initialised |
| `EHS_ML_MODEL_PATH_ERR` | Invalid or unresolvable model path |
| `EHS_ML_MODEL_LOAD_ERR` | TFLite failed to load model from file |
| `EHS_ML_MODEL_IN_USE` | `EhsML_Create` called when model already loaded |
| `EHS_ML_INPUT_SIZE_MISMATCH_ERR` | Camera frame size ≠ model input tensor size |
| `EHS_ML_NULL_CTX_ERR` | NULL `EhsML_Context*` passed |
| `EHS_ML_NULL_INPUT_ERR` | NULL input data pointer |
| `EHS_ML_NULL_JSON_BUF_ERR` | NULL json output buffer |
| `EHS_ML_INVALID_FRAME_ID` | frame_id < -1 |
| `EHS_ML_INVALID_FRAME` | `EhsCameraFrameGetById` returned NULL or data unavailable |
| `EHS_ML_INFERENCE_ERR` | `TfLiteInterpreterInvoke` failed |

---

## TFLite Input Requirements

TFLite models require exact byte-size match between input data and the model's input tensor.

**To diagnose mismatches**, look for `[TFLITE_DBG] SetInputData:` log lines which print:
```
frame_size=921600, tensor_expects=4915200 *** SIZE MISMATCH ***
```

### Common cases

| Camera delivers | Model expects | Ratio | Cause |
|---|---|---|---|
| uint8 RGB [H×W×3] | float32 RGB [H×W×3×4] | 4× | Model input is float32, not uint8 |
| uint8 RGB [H×W×3] | float16 RGB [H×W×3×2] | 2× | Model input is float16 |
| Wrong resolution | Correct type but wrong size | other | Camera not resized to model dims |

**Pre-processing required before `EhsML_SetInputData`:**
1. Resize frame to model input height × width
2. Convert uint8 → float32 (divide by 255.0 for [0.0, 1.0] normalisation)
3. Ensure channel order matches training convention (usually RGB)

Quantisation scale and zero_point are logged at model load (`[TFLITE]` lines). If `scale=0.0`, the model expects raw float input (no dequantisation).

---

## Model Info JSON (`EhsML_GetModelInfoJson`)

Declared in `hal_ml.h`, implemented in `ml_common.c` (dispatches to `EhsML_TFLite_GetModelInfoJson` in `ert_hal_tflite_meta.c`).

Combines TFLite C-API tensor introspection with minimal FlatBuffer parsing of the model file. No malloc in the call path — caller passes a pre-allocated buffer (use `EHS_STRING_LENGTH_MAX` for eRT port buffers).

**Output example:**
```json
{
  "runtime": "2.14.0",
  "description": "",
  "input_count": 1,
  "output_count": 1,
  "inputs": [{"name":"serving_default_x:0","dtype":"float32","shape":[1,640,640,3],"bytes":4915200,"quant_scale":0.0,"quant_zp":0}],
  "outputs": [{"name":"PartitionedCall:0","dtype":"float32","shape":[1,25200,85],"bytes":8568000,"quant_scale":0.0,"quant_zp":0}],
  "inferred_type": "yolov5-coco80",
  "xnnpack": true,
  "metadata": [{"name":"min_runtime_version","value":"1.5.0"}]
}
```

**Inferred model type heuristic** (from output tensor shape):

| Output shape | Inferred type |
|---|---|
| `[1, N, classes+5]` where N >> classes+5 | `yolov5-family` / `yolov5-coco80` |
| `[1, classes+4, N]` where N >> classes+4 | `yolov8-family` / `yolov8-coco80` |

Note: `"fp16"` in a model filename typically refers to weight storage, not the input/output interface. Verify actual interface dtype from the JSON output — it is usually `float32`.

---

## tf_lite_frame Function Block Ports

### load_model (ERT1_ID=1)

| arg | PortType | DataType | CName | Direction |
|---|---|---|---|---|
| 0 | StartPort | — | `load_model` | trigger |
| 1 | InputPort | S | `model_file_path` | in |
| 1 | FinishPort | — | `load_ok` | success exit |
| 1 | OutputPort | I | `load_errno` | out |
| 2 | FinishPort | — | `load_error` | error exit |
| 2 | OutputPort | S | `model_info` | out — JSON from `EhsML_GetModelInfoJson` |

### do_inference (ERT1_ID=2)

| arg | PortType | DataType | CName | Direction |
|---|---|---|---|---|
| 0 | StartPort | — | `do_inference` | trigger |
| 1 | InputPort | I | `frame_id` | in |
| 1 | FinishPort | — | `done_inference` | success exit |
| 1 | OutputPort | I | `inference_errno` | out |
| 2 | FinishPort | — | `inference_error` | error exit |
| 2 | OutputPort | S | `output` | out — JSON detection results |

### Parameters

| Name | Type | Default | Description |
|---|---|---|---|
| Model Type | I | 0 | `EhsML_Type` enum value |
| Conf Thres | F | 0.5 | Confidence threshold (0.0–1.0) |
| Thread Count | I | 2 | TFLite interpreter thread count |
| Flat JSON | B | 0 | Use flat (non-nested) JSON output format |
| Use Application Dir | B | 0 | Load model from app dir instead of user data dir |
