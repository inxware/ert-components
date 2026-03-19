# Machine Learning HAL — Architecture and Development Guide

---
## Introduction
Some Edge-AI inference engines provide a full chain of pre and post processing for machineleanring inference, but the scope and quality varies quite substantialy.
The inxware Machine Learning abstraction attempts to use engine technologies where available, but also provide a framework for filling gaps with optimised (hardware and model type) methods and identifying pipelines that are likely to be limited by the platforms's pipeline technologies (e.g. what is accelerated and what is not). 

## Terminology

These terms have overlapping everyday meanings; the definitions below are
specific to this codebase.

| Term                 | Definition in this codebase |
|---                   |---|
| **ML Framework**     | The toolchain or format used to *train and export* a model (TensorFlow/Lite, ONNX, Edge Impulse, PyTorch). Determines the file format (`.tflite`, `.onnx`, `.hef`, `.engine`) and constrains which inference engines can load it. Not always relevant at inference time — once compiled to a hardware-native format (e.g. Hailo HEF or TensorRT engine) the original framework is opaque. |
| **Inference Engine** | The *runtime backend* that executes inference on the target hardware: TFLite runtime, HailoRT SDK, TensorRT, TFLite Micro, RKNN, etc. Selection is a **build-time** decision via `EHS_ML_IE_IMAGE_SUPPORT` / `EHS_ML_HARDWARE_ACCELERATION` (see §Makefile Variables). |
| **Post-processing**  | Everything that converts raw inference engine tensor output into application-ready data: tensor unpacking, dequantisation, architecture decode, confidence filter, NMS, output serialisation. Model-specific, not engine-specific. |
| **Model Type** (AKA **Model**) | The top-level HAL API (`ml_common.*`). Dispatches `EhsML_Create` / `EhsML_RunOutputJson` / `EhsML_Destroy` to the correct model implementation by `EhsML_Type`. |

> **Naming note**: `EHS_ML_HARDWARE_ACCELERATION` still uses "hardware
> acceleration" rather than "inference engine" — this is intentionally kept
> distinct because hardware accelerators (Hailo NPU, Jetson GPU) sit alongside
> a CPU-based engine rather than replacing it.  The variable name remains
> appropriate.

---

## Pipeline Dependency Seperation

```
┌──────────────────────────────────────────────────────────────────┐
│  FUNCTION BLOCK:/Common/Components/ml/                           │
│  EhsML_Create(), EhsML_SetInputData(),EhsML_RunOutputJson()      │
└───────────────────────────┬──────────────────────────────────────┘
                            │ > dispatched by EhsML_Type
┌───────────────────────────▼──────────────────────────────────────┐
│  COMMON ML HAL:/target/Component-HAL/ml/ml_common.c              │
│  Creates a pipeline object and routes engine and model specifics │
└───────────────────────────┬──────────────────────────────────────┘
                            │ > per-model Create / RunOutputJson
┌───────────────────────────▼──────────────────────────────────────┐
│  MODEL TYPE POSTPROCESSING:/target/Component-HAL/ml/             │
│  Model-architecture-specific decoding, implies logical           |
|  post-processing and output data formatting                      │
│  EhsML_InfEngine_*(),  more TODO                                 │
└───────────────────────────┬──────────────────────────────────────┘
                            │ > TODO
┌───────────────────────────▼──────────────────────────────────────┐
│  Inference Engine Dispatch  (engine/ml_inf_engine.c)             │
│  Selects backend by ctx->hw_accel                                │
└──────┬──────────────────┬──────────────────────┬─────────────────┘
       │ TFLite           │ HailoRT              │ TensorRT
┌──────▼──────┐   ┌───────▼───────┐   ┌──────────▼──────────┐
│ engine/     │   │ engine/       │   │ engine/             │
│ tensorflow- │   │ hailo/        │   │ tensorrt/           │
│ lite/       │   │ ert_hal_hailo │   │ ert_hal_tensorrt    │
└─────────────┘   └───────────────┘   └─────────────────────┘
```

> Note other directories are assigned for model development framework dependencies, but are not generally needed.

---

## Post-Processing Pipeline Stages

When an inference engine delivers raw tensors, they must pass through these
stages before the application receives usable data.  If the engine SDK already
handles some stages internally (e.g. HailoRT performs dequantisation and NMS
before returning tensors), those stages are a no-op in our code.


> 1. **Inference** (engine runs the model arithmetic)
> 2. Engine-specific tensor **Unpacking**
> 3. **Dequantisation**  (scale × (raw − zero_point) → float)
> 4. Model-architecture **Decoding** (anchor, DFL, grid offsets, tensor routing)
> 5. **Logical** post-processing (confidence threshold, NMS, plausibility checks)
> 6. Output **Formatting**  (JSON, tensor binary, Protobuf, …)

### Stage ownership

| Stage | Owner |
|---|---|
| 1 — Inference | `framework/<engine>/ert_hal_*.c` via `engine/ml_inf_engine.c` dispatch |
| 2 — Engine tensor unpacking | `postprocessing/engine/<engine>/` |
| 3 — Dequantisation | `postprocessing/dequantise/` utilities; called from model layer |
| 4 — Architecture decode | `postprocessing/model/<model>.c` |
| 5 — Logical post-processing | `postprocessing/logical/` (NMS, confidence, etc.); called from model layer |
| 6 — Output formatting | `postprocessing/output/` and model `RunOutputJson` |

**Important**: if the engine SDK already covers stages 2–5 satisfactorily,
use the SDK output directly.  There is no requirement to split a monolithic
engine post-processor into these stages.

### Pipeline coverage by engine + model

Ownership labels: `engine` = inference engine SDK or its eRT wrapper;
`model` = model-specific eRT code; `generic` = shared eRT utilities;
`n/a` = stage eliminated by a preceding stage; `TODO` = not yet implemented.

| Engine                | Model   | Task   | \| | Infer  | Unpack | Dequant | Decode  | Logical | Format  |
|-----------------------|---------|--------|---|--------|--------|---------|---------|---------|---------|
| TFLite                | YOLOv5  | ObjDet | \| | engine | engine | model   | model   | generic | model   |
| TFLite                | YOLOv8  | ObjDet | \| | engine | engine | engine  | model   | model   | model   |
| TFLite                | YOLOv8  | Pose   | \| | engine | engine | engine  | model   | N/A     | model   |
| TFLite                | YOLOv10 | ObjDet | \| | engine | engine | engine  | ----    | ----    | ----    |
| TFLite Micro          | YOLOv5  | ObjDet | \| | engine | engine | model   | model   | generic | model   |
| HailoRT — NMS HEF     | YOLOv5  | ObjDet | \| | engine | engine | engine  | engine  | engine  | ----    |
| HailoRT — NMS HEF     | YOLOv8  | ObjDet | \| | engine | engine | engine  | engine  | engine  | ----    |
| HailoRT — NMS HEF     | YOLOv8  | Pose   | \| | engine | engine | engine  | engine  | engine  | ----    |
| HailoRT — NMS HEF     | YOLOv10 | ObjDet | \| | engine | engine | engine  | engine  | engine  | ----    |
| HailoRT — raw HEF     | YOLOv8  | ObjDet | \| | engine | engine | engine  | model   | ----    | ----    |
| HailoRT — raw HEF     | YOLOv8  | Pose   | \| | engine | engine | engine  | model   | ----    | ----    |
| TensorRT — NMS plugin | YOLOv8  | ObjDet | \| | engine | engine | engine  | engine  | engine  | model   |
| TensorRT — standard   | YOLOv8  | ObjDet | \| | engine | engine | engine  | ----    | N/A     | ----    |

Per-engine implementation detail is in the subsections below.

---

### Per-model implementation detail

The table below maps each stage to the actual function or mechanism for the
currently implemented model/engine combinations.

| Stage | YOLOv5 — TFLite | YOLOv8 ObjDet — TFLite | YOLOv5/v8 — Hailo |
|---|---|---|---|
| **1. Inference** | `EhsML_InfEngine_RunInference` → TFLite `TfLiteInterpreterInvoke` | same | HailoRT async pipeline (internal) |
| **2. Tensor unpack** | Single tensor `[1, N, 85]` → `ctx->output_tensor[0]` | Flat FP32 count-prefixed array | `EhsML_Postprocessing_Engine_Hailo_ProcessOutput` wraps HailoRT buffers |
| **3. Dequant** | `_EHS_ML_TYPED_DATA_ASSIGN` macro, all types | FP32 only (rejects quantised) | Internal to HailoRT |
| **4. Arch decode** | Flat index, objectness × class_score | Per-class count prefix, corner coords | `yolov8()` — DFL decode inside HailoRT |
| **5. Confidence / NMS** | `box.score >= conf_thres` + `EhsApply_Greedy_NMS()` | Threshold inline, no NMS (model-internal) | `det->get_confidence()`, NMS internal to HailoRT |
| **6. JSON** | `EhsSnprintf` centre-format `{x,y,w,h}` | `EhsSprintf` corner-format `{ymin,xmin,ymax,xmax}` | TODO |
| **Key file** | `postprocessing/model/yolov5_objdet.c` | `postprocessing/model/yolov8_objdet.c` | `postprocessing/engine/hailo/ml_postprocessing_engine_hailo.cpp` |

---

### Per-engine pipeline notes

#### TFLite (CPU)

- Inference is synchronous via `TfLiteInterpreterInvoke`.
- `ert_hal_tflite.c` maps TFLite output tensor pointers 1:1 into `ctx->output_tensor[]`.
- Dequantisation: TFLite delivers raw quantised integers; the model layer applies
  `scale × (raw − zero_point)` via `_EHS_ML_TYPED_DATA_ASSIGN`.  Float models skip this.
- `yolov8_objdet.c` currently only supports FP32 output; it rejects quantised models.
  `yolov5_objdet.c` handles all `EHS_ML_DATATYPE_*` types.

#### HailoRT (Hailo NPU)

- Inference is an async HailoRT pipeline via PCIe DMA; HailoRT schedules batches internally.
- HailoRT resolves tensor splits introduced by the Hailo compiler (the HEF may reorder/split
  outputs vs. the original ONNX); `postprocessing/engine/hailo/` wraps `HailoTensor` objects
  into `ctx->output_tensor[]`.
- HailoRT dequantises all output tensors before returning to host — model layer always
  receives FP32.
- **NMS-enabled HEF**: the model was compiled with Hailo's Dataflow Compiler using the
  `nms_postprocess` task node.  The HEF contains the NMS layer as silicon-executable code;
  HailoRT runs it on the NPU and returns a clean detection list (stages 4–5 handled by SDK).
- **Raw HEF** (no NMS node): requires stages 4–5 in software.  The choice is made at
  model-compile time, not at eRT build time.

#### TensorRT (NVIDIA Jetson GPU/DLA)

- Inference is TensorRT CUDA kernel execution on Jetson GPU or DLA.
- `ert_hal_tensorrt.cpp` copies each output binding from device (GPU) to host and fills
  `ctx->output_tensor[]`.
- TensorRT manages INT8/FP16 precision internally; output bindings are delivered as FP32.
- **NMS plugin** (`EfficientNMS_TRT`, `BatchedNMS_TRT`): can be fused into the `.engine`
  file by `trtexec` or ONNX post-processing tools.  If included, stages 4–5 are handled
  by the SDK.
- **Standard export**: raw box/score tensors require a model-layer decoder (TODO).
**Option B — DeepStream `NvDsInferContext` integration (NVIDIA ecosystem)**

DeepStream (available for Jetson via the JetPack apt feeds, separate from the
TensorRT base install) provides `NvDsInferContext` — a managed inference context
that handles engine loading, input pre-processing, output parsing, and detection
decoding natively, with plugin parsers for common architectures.

Features it would add over Option A:

| Feature | Notes |
|---|---|
| Built-in NMS output parsers | `NvDsInferParseCustomYoloV5/V8` and similar; no hand-written tensor layout code |
| Multi-class detection structs | Returns `NvDsInferObjectDetectionInfo` list directly |
| GPU-side pre/post-processing | `NvBufSurface` zero-copy path; avoids CPU round-trip for image data |
| Batched inference | Native batch dimension management |
| Model metadata introspection | `NvDsInferNetworkInfo` supplies input H/W/C without binding name guessing |
| Tracker integration | `NvDsTracker` can be attached to provide object ID persistence across frames |
| Analytics | `NvDsAnalytics` adds line-crossing, ROI occupancy, direction detection |

Costs and risks:

- **Large dependency**: DeepStream pulls in GStreamer, CUDA, NVMM buffer management,
  and a significant runtime footprint; unsuitable if the Jetson deployment is
  resource-constrained.
- **API coupling**: `NvDsInferContext` is a C++ class hierarchy; integrating it
  cleanly behind the `EhsML_*` C API requires a C++ shim layer similar to the
  existing `ert_hal_tensorrt.cpp` pattern, but substantially larger.
- **Versioning**: DeepStream versions are tightly coupled to JetPack versions;
  a JetPack upgrade may require a parallel DeepStream upgrade.
- **Licence**: DeepStream is NVIDIA proprietary (NVIDIA Software License Agreement),
  which may affect redistribution.

Effort: ~1–2 weeks for initial integration; ongoing maintenance overhead.

---

## Directory Tree

```
ml/
├── README.md                          ← this file
├── ml_common.c / .h / .mk             ← top-level HAL API (dispatch by EhsML_Type)
│
├── engine/                            ← all inference engine code
│   ├── ml_inf_engine.c / .h           ← dispatch: EhsML_InfEngine_Create/Destroy/SetInputData/RunInference
│   ├── ml_engine.mk                   ← engine selector + dispatch object (included by ml_common.mk)
│   ├── tflite/                        ← TFLite runtime (linux, CPU)
│   │   ├── ert_hal_tflite.c / .h
│   │   ├── ert_hal_tflite_meta.c / .h ← model metadata / GetModelInfoJson
│   │   └── ml_ie_tflite.mk
│   ├── tflite-micro/                  ← TFLite Micro (MCU bare-metal) — TODO
│   │   └── ml_ie_tflite_micro.mk
│   ├── hailo/                         ← HailoRT (Hailo NPU)
│   │   ├── ert_hal_hailo.c / .h
│   │   ├── hailo_thread.h
│   │   └── ml_ie_hailo.mk
│   └── tensorrt/                      ← TensorRT (NVIDIA Jetson GPU/DLA)
│       ├── ert_hal_tensorrt.cpp / .h
│       └── ml_ie_tensorrt.mk
│
├── frameworks/                        ← model development framework utilities
│   └── README.md                      ← placeholder; nothing implemented yet
│                                         (future: onnx/, edge-impulse/)
│
├── postprocessing/
│   ├── engine/                        ← engine-specific tensor unpacking
│   │   ├── ml_postprocessing_engine.mk
│   │   └── hailo/
│   │       ├── ml_postprocessing_engine_hailo.cpp / .hpp / .mk
│   │       ├── yolov8pose_postprocess.cpp / .hpp
│   │       └── common/                ← Hailo vendor headers (xtensor, nlohmann)
│   │
│   ├── dequantise/                    ← dequantisation utilities
│   │   ├── ml_postprocessing_dequantise.c / .h
│   │   └── ml_postprocessing_general.mk
│   │
│   ├── model/                         ← model-architecture-specific decoding
│   │   ├── ml_models.h                ← include gate for all model headers
│   │   ├── ml_model_template.c.template  ← starting point for new models
│   │   ├── ml_model_template.h.template
│   │   ├── ml_model.mk
│   │   ├── yolov5_objdet.c / .h       ← YOLOv5 object detection (implemented)
│   │   ├── yolov8_objdet.c / .h       ← YOLOv8 object detection (implemented)
│   │   ├── yolov8_pose.c / .h         ← YOLOv8 pose estimation (in progress)
│   │   ├── ml_model_common.c / .h     ← DEPRECATED — redirects to engine/ml_inf_engine.*
│   │   └── ml_utils/
│   │       ├── ehs_ml_nms.c / .h      ← NMSBox + EhsApply_Greedy_NMS()
│   │       └── ehs_ml_utils.c / .h    ← timing utilities
│   │
│   ├── logical/                       ← logical post-processing (NMS, confidence, etc.)
│   │   ├── ehs_ml_logical.c / .h
│   │   ├── ehs_ml_nms.c / .h
│   │   └── ml_postprocessing_logical.mk
│   │
│   └── output/                        ← output format serialisation
│       ├── ehs_ml_objdet_output.c / .h
│       ├── ehs_ml_postprocessing_output.h
│       └── ml_postprocessing_output.mk
│
└── stubbed/                           ← no-op stub when EHS_ML_SUPPORT=stubbed
    ├── stubbed_ml.c / .h
    └── ml_stubbed.mk
```

### How to add a new model

1. Copy `postprocessing/model/ml_model_template.c.template` and `.h.template`
   to `postprocessing/model/<model_name>.c/.h`.
2. Define a new `EhsML_Type` enum value in `Common/HAL/include/hal_ml.h`.
3. Add a `case` in `ml_common.c` dispatching to the new model's four functions.
4. Add the source to `ml_model.mk` under an `EHS_ML_MODEL_SUPPORT_*` guard.
5. Include the new header in `ml_models.h` under the same guard.
6. Document the expected output tensor layout (tensor count, dimensions, data
   type, coordinate system) in the new source file.

The four per-model functions and their responsibilities:

| Function | Responsibility |
|---|---|
| `EhsML_<Model>_Create` | Macro alias → `EhsML_InfEngine_Create` (no model-specific setup needed by default) |
| `EhsML_<Model>_Destroy` | Macro alias → `EhsML_InfEngine_Destroy` |
| `EhsML_<Model>_SetInputData` | Macro alias → `EhsML_InfEngine_SetInputData` |
| `EhsML_<Model>_RunOutputJson` | Calls `EhsML_InfEngine_RunInference` to populate `ctx->output_tensor[]`, then performs stages 4–6 (decode, Logical, JSON) |

---

## Makefile Variables

### Top-level

| Variable | Values | Description |
|---|---|---|
| `EHS_ML_SUPPORT` | `yes` / `stubbed` / _(absent)_ | Enables full ML, stub-only, or no ML support |
| `EHS_ML_LAYER_TENSORS_MAX` | integer (default 128) | Max input or output tensors per model |

### Inference engine backends (`EHS_ML_IE_IMAGE_SUPPORT`)

Selects the primary CPU-based runtime for image/vision models:

- `tensorflow-lite` — TFLite runtime (linux targets)
- `tensorflow-lite-micro` — TFLite Micro (MCU, TODO)
- `none` — no CPU runtime (hardware-only, e.g. Jetson TensorRT build)

### Hardware acceleration (`EHS_ML_HARDWARE_ACCELERATION`)

Selects the hardware accelerator backend:

- `hailo` — Hailo NPU (HailoRT SDK)
- `nvidia` — NVIDIA GPU/DLA (TensorRT)
- _(absent)_ — CPU-only (TFLite runtime)

### Model selection

Each model type has an opt-in make variable (value `yes`):

| Variable | Status |
|---|---|
| `EHS_ML_MODEL_SUPPORT_YOLOV5_OBJDET` | Implemented |
| `EHS_ML_MODEL_SUPPORT_YOLOV8_OBJDET` | Implemented |
| `EHS_ML_MODEL_SUPPORT_YOLOV8_POSE`   | In progress |
| `EHS_ML_MODEL_SUPPORT_YOLOV8_*` (others) | Not implemented |
| `EHS_ML_MODEL_SUPPORT_YOLOV9_*` through `YOLOV12_*` | Not implemented |
| `EHS_ML_MODEL_SUPPORT_SAM_IMGSEG` | Not implemented |

---

## Error Codes (`EhsML_Err`)

Defined in `Common/HAL/include/hal_ml.h`.

| Value | Enumerator | Description |
|---|---|---|
| 0 | `EHS_ML_OK` | Success |
| 1 | `EHS_ML_FAILED` | Generic unclassified failure |
| 2 | `EHS_ML_MEMORY_ERR` | Memory allocation failure |
| 3 | `EHS_ML_INIT_ERR` | Context or framework not initialised |
| 4 | `EHS_ML_INVALID_SIZE_ERR` | Invalid size argument |
| 5 | `EHS_ML_INVALID_QUANT_ERR` | Invalid quantisation parameters |
| 6 | `EHS_ML_MODEL_LOAD_ERR` | Framework failed to load the model file |
| 7 | `EHS_ML_MODEL_CTX_ERR` | NULL or invalid model context |
| 8 | `EHS_ML_MODEL_TENSOR_DIM_ERR` | Tensor dimension mismatch |
| 9 | `EHS_ML_MODEL_IN_USE` | `EhsML_Create` called while model already loaded |
| 10 | `EHS_ML_MODEL_PATH_ERR` | Model file path invalid |
| 11 | `EHS_ML_MODEL_NAME_ERR` | Model file name invalid |
| 12 | `EHS_ML_MODEL_TYPE_ERR` | `EhsML_Type` value not recognised |
| 13 | `EHS_ML_MODEL_OUTPUT_ERR` | Output tensor format not supported |
| 14 | `EHS_ML_INFERENCE_ERR` | Engine invoke failed |
| 15 | `EHS_ML_INVALID_FRAME_ID` | `frame_id` < −1 |
| 16 | `EHS_ML_INVALID_FRAME` | Camera frame not found |
| 17 | `EHS_ML_INVALID_DEP` | Library dependency invalid (version mismatch) |
| 18 | `EHS_ML_INVALID_TENSOR_ERR` | Tensor pointer or metadata invalid |
| 19 | `EHS_ML_JSON_STRSIZE_ERR` | JSON output buffer exhausted |
| 20 | `EHS_ML_NOT_IMPLEMENTED` | Called function is a stub |
| 21 | `EHS_ML_NOT_SUPPORTED` | Feature not supported by this build |
| 22 | `EHS_ML_NULL_CTX_ERR` | NULL `EhsML_Context*` |
| 23 | `EHS_ML_NULL_INPUT_ERR` | NULL input data pointer |
| 24 | `EHS_ML_NULL_JSON_BUF_ERR` | NULL JSON output buffer |
| 25 | `EHS_ML_INPUT_SIZE_MISMATCH_ERR` | Input byte size ≠ model tensor byte size |

> Integer values are positional — always compare against the named enumerator.
> The integer appears on the `load_errno` / `inference_errno` output port of
> the function block.

---

## TODOs

- [ ] Rename `EHS_ML_HARDWARE_ACCELERATION` values (`hailo`, `nvidia`) to use
      consistent engine identifiers if/when a third accelerator is added.
- [ ] `stubbed/` should be just another entry in `engine/` (a no-op backend
      `engine/stubbed/`) rather than a special-cased build path.
- [ ] Pipeline dispatch table: replace `switch(ctx->type)` in `ml_common.c`
      with a function-pointer table indexed by `EhsML_Type` to avoid
      growing switch statements.
- [ ] `EhsML_InfEngine_RunInference` currently passes `json_output` and
      `output_size` for early validation only.  Consider removing them from
      the engine-layer signature once the validation is moved to `ml_common.c`.
- [ ] Raw binary output path (`EhsML_RunOutputData`) — not yet implemented.
- [ ] AMD ROCm/MIGraphX and custom NPU backends — currently fall back to TFLite.

---

**For broader context see `docs/inxware-edge-ml.md`**
