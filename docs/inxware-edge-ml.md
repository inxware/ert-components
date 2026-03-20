# inxware Edge Machine Learning

## Table of Contents

1. [Use Cases](#1-use-cases)
2. [Terminology](#2-terminology)
3. [Model Training Frameworks](#3-model-training-frameworks)
   - [LiteRT (formerly TFLite)](#31-litert-formerly-tflite)
   - [PyTorch / ExecuTorch](#32-pytorch--executorch)
   - [Other Training Tools](#34-other-training-tools)
4. [Portable Model Exchange Formats](#4-portable-model-exchange-formats)
   - [Format Reference Table](#41-format-reference-table)
   - [ONNX (Open Neural Network Exchange)](#42-onnx-open-neural-network-exchange)
   - [Edge Impulse Formats](#43-edge-impulse-formats)
5. [Hardware-Specific Compilation](#5-hardware-specific-compilation)
   - [NVIDIA TensorRT](#51-nvidia-tensorrt) · [DeepStream SDK](#nvidia-deepstream-sdk)
   - [Hailo — HEF](#52-hailo--hef)
   - [DeepX — DXNN](#53-deepx--dxnn)
   - [Qualcomm — QNN / DLC (SNPE)](#54-qualcomm--qnn--dlc-snpe)
   - [Rockchip — RKNN](#55-rockchip--rknn)
   - [NXP — eIQ / Neutron NPU](#56-nxp--eiq--neutron-npu)
   - [Arm — Ethos-U (Vela)](#57-arm--ethos-u-vela)
   - [MediaTek — NeuroPilot / APU](#58-mediatek--neuropilot--apu)
   - [Kneron — NEF](#59-kneron--nef)
   - [Axelera — Metis (Voyager SDK)](#510-axelera--metis-voyager-sdk)
   - [Ambarella — CVflow](#511-ambarella--cvflow)
   - [BrainChip — Akida](#512-brainchip--akida)
   - [Large Integrations (Data Centre / Cloud)](#513-large-integrations-data-centre--cloud)
   - [Google Coral — Edge TPU](#514-google-coral--edge-tpu)
   - [RISC-V Based ML Processors](#515-risc-v-based-ml-processors)
6. [Inference Post-Processing Pipeline](#6-inference-post-processing-pipeline)
   - [Pipeline Overview](#61-pipeline-overview) — five-stage post-processing model
   - [Raw Output / Dequantisation](#62-raw-output--dequantisation) — stages 1 & 2 detail
   - [Model-Architecture Decoding](#63-model-type-decoding) — stage 3 detail (YOLO, pose models)
   - [Data Filtering — NMS](#64-data-filtering--nms) — stage 4 detail
   - [C++ and Python APIs per Format](#65-c-and-python-apis-per-format)
   - [Output Formats](#66-output-formats) — stage 5 detail
7. [LLM at the Edge](#7-llm-at-the-edge)
8. [inxware Implementation — Status, APIs and Source Roadmap](#8-inxware-implementation--status-apis-and-source-roadmap)
9. [Appendix: Runtime Dependencies Reference](#9-appendix-runtime-dependencies-reference)

---

# 1. Use Cases

Edge AI machine learning includes the following potential use-cases:

- **Machine Vision**
  - Object detection, recognition and location
  - Pose estimation of human movement (public space, sports & health)
  - Manufacturing QA and anomaly detection
  - Medical & bioscience devices
     - histology microscopy
     - cell culture incubation
     - radiography
- **Natural Language Processing (SLMs and LLMs)**
  - Human machine interfaces (SLM)
  - Automation and agentic (function calling LLMs)
  - Device-side reasoning and context awareness
- **Vector data classification prediction**
  - Time series anomaly detection (MLPs)
  - Sensor (oSVM)
- **Vector data value prediction (regression)**
  - Fixed model sensor fusion and data compression
  - Adaptive non-linear models for prediction (oSVM)
  - Adaptive linear/non-linear predictors (Kalman filters)
  - Model predictive control systems
  - Condition monitoring (time series analysis and system identification)
- **Voice recognition (various)**
  - Feature space transformations (FFTs, Wavelet, Periodogram)
  - Combinations of CNNs and LLMs


  There are many more applications of Edge-AI, where responsiveness, reliability, privacy and operating costs are improved sufficiently for operational deployment.
---

# 2. Terminology

| Term | Definition |
| :--- | :--- |
| **Model Creation** | Dataset input and model output. Includes data tagging, training, optimisation, and quantisation. |
| **Purpose** | Broad purpose of model usage: Text, Image, Audio, etc. |
| **Sub-Task** | What the ML model can do under a specific purpose. |
| **Primary Model Type** | Internal structure of a model with different task variants. E.g. `YOLOv8-pose` = YOLOv8 for pose estimation. |
| **SAM** | Segment Anything Model (Meta). |
| **Model Data Container** | How the inference model is stored on disk. |
| **Inference Data Container** | Container format of the inference output data. |
| **Model Data Structure** | Data structure of input/output data inside the container. |
| **Dequantisation** | Converting quantised INT8/INT4 outputs back to floating-point using stored scale and zero-point parameters. Formula: `float_value = (raw_value − zero_point) × scale`. `scale` (FP32) and `zero_point` (INT8 or UINT8) are per-tensor quantisation parameters stored in the model file's metadata. |
| **NMS** | Non-Maximum Suppression — filters duplicate/overlapping detections. |
| **QDQ** | QuantizeLinear / DequantizeLinear ONNX operator pair carrying quantisation parameters. |
| **HEF** | Hailo Executable Format — compiled binary for Hailo NPUs. |
| **EON** | Edge Optimised Neural — Edge Impulse's C++ code generator for ML models. |

## ML Workflow Entity

| Model Creation Examples                 | Purpose                      | Sub-Task | Primary Model Type | Model Data Container Format | Inference Data Container | Model Data Structure |
| :-------------------------------------- | :--------------------------- | :------- | :----------------- | :-------------------------- | :----------------------- | :---------- |
| Ultralytics, PyTorch, TensorFlow Lite, ROCm (AMD) | Image, Text, Audio | Detection, Image segmentation, Classification, Pose estimation | YOLOv3–11, YOLOv26, SAM (1–3), SVM, Transformer (LLM) | `.tflite` `.onnx` `.pb` `.hef` `.pte` | NumPy array, FlatBuffer, Protobuf | Combination of Primary Model Type and Task |
| NVIDIA TAO Toolkit, PyTorch, TensorFlow  | Image, Video | Detection, Segmentation, Classification, Pose, OCR, Tracking | YOLOv8, SSD, ResNet, EfficientDet, PeopleNet, BodyPoseNet | `.onnx` `.engine` `.plan` | CUDA tensor bindings (NumPy/CuPy), Protobuf (gRPC / Triton KServe v2) | TensorRT binding tensors; Triton inference protocol v2 |

### Potential ML Pipeline Flows

The diagram shows three tiers: **training frameworks and model zoos** (left) → **export formats, compilers, and middlewares** (centre) → **inference engine runtimes** (right).

> **Note on the numbers in source:** In Mermaid `sankey-beta` syntax the third value on each line (e.g. `,1`) is a required flow-width parameter — it controls arrow thickness proportionally and does not appear as a label in the rendered output. All values are set to `1` here for equal-width flows; increase individual values to emphasise higher-traffic paths. Per-flow colours cannot be overridden in `sankey-beta` — colours are assigned by the active Mermaid theme.

```mermaid
sankey-beta

TensorFlow/Keras,ONNX,1
TensorFlow/Keras,TFLite,1
TensorFlow/Keras,SavedModel,1
PyTorch,ONNX,1
PyTorch,ExecuTorch (.pte),1
PyTorch,TorchScript (.pt),1
Ultralytics (YOLO),ONNX,1
Ultralytics (YOLO),TFLite,1
NVIDIA TAO Toolkit,ONNX,1
NVIDIA TAO Toolkit,TRT Engine,1
Edge Impulse Studio,EI Package,1
scikit-learn,Pickle,1
JAX,ONNX,1
Hailo Model Zoo,Hailo HEF,1
Coral Model Zoo,Edge TPU Model,1
ONNX,ONNX Runtime,1
ONNX,TRT Engine,1
ONNX,Hailo HEF,1
ONNX,Qualcomm QNN,1
ONNX,Rockchip RKNN,1
ONNX,DeepX / Kneron,1
TFLite,LiteRT Runtime,1
TFLite,Hailo HEF,1
TFLite,Edge TPU Model,1
TFLite,NXP eIQ / Ethos-U,1
TFLite,Rockchip RKNN,1
TFLite,MediaTek APU,1
TRT Engine,TensorRT Runtime,1
TRT Engine,Jetson DLA,1
TRT Engine,DeepStream SDK,1
SavedModel,TF Serving,1
ExecuTorch (.pte),ExecuTorch Runtime,1
TorchScript (.pt),LibTorch,1
EI Package,Edge Impulse Runtime,1
EI Package,TensorRT Runtime,1
Pickle,Python Runtime,1
Hailo HEF,HailoRT,1
Edge TPU Model,Edge TPU Runtime,1
Qualcomm QNN,QNN Runtime,1
Rockchip RKNN,RKNN Runtime,1
```

---

# 3. Model Training Frameworks

Model building is generally not done on edge devices, except for adaptive model types, which are currently rarely deployed outside of aerospace, automotive and defence. The model building frameworks discussed here are almost exclusively Neural Network based or more specifically variants of the Multi-Layer-Perceptron (MLP) model.

These frameworks often set the scene of what options for deployment are available at the edge, which splits into different work-flow and processing pipelines sometimes independently of the training framework, used but unfortunately not always the case. This can cause various issues for building devices with established model types from **model zoos** or custom models built with specific tooling.

Unfortunately each silicon vendor, npu developer and ML technology has built end-2-end systems to improve accessibility and/or add "stickiness" to their technology, but results in a highly fragmented eco-system, where seemingly easy transformations and generalisations are not easily practical between model zoos, training frameworks, deplyment formats and inference execution.

## 3.1 LiteRT (formerly TFLite)

**TensorFlow** is Google's primary ML framework. Models are defined in Python using the Keras high-level API or the lower-level TF ops.

**LiteRT** (formerly TensorFlow Lite, still commonly called TFLite) is the lightweight inference runtime for mobile and edge devices, officially rebranded to LiteRT in September 2024. The model file format is `.tflite` — a FlatBuffer-serialised representation of the computation graph and weights — and this format is unchanged by the rebrand. GitHub: [github.com/google-ai-edge/LiteRT](https://github.com/google-ai-edge/LiteRT) · Docs: [ai.google.dev/edge/litert](https://ai.google.dev/edge/litert). In March 2026, a LiteRT update delivered 1.4× faster GPU execution and new NPU acceleration support.

**Training workflow:**

1. Train model in TensorFlow/Keras (Python)
2. Export via `tf.lite.TFLiteConverter` to `.tflite`
3. Optionally apply post-training quantisation (INT8, FP16, INT4) during conversion
4. Deploy with LiteRT C++ interpreter or TFLite Micro (TFLM) on microcontrollers

**LiteRT conversion (Python):**

```python
import tensorflow as tf

converter = tf.lite.TFLiteConverter.from_saved_model("saved_model/")
# Optional: quantise to INT8
converter.optimizations = [tf.lite.Optimize.DEFAULT]
converter.representative_dataset = representative_data_gen
converter.target_spec.supported_ops = [tf.lite.OpsSet.TFLITE_BUILTINS_INT8]
tflite_model = converter.convert()
with open("model.tflite", "wb") as f:
    f.write(tflite_model)
```

**LiteRT C++ inference:**

```cpp
// Load model
auto model = tflite::FlatBufferModel::BuildFromFile("model.tflite");
tflite::ops::builtin::BuiltinOpResolver resolver;
std::unique_ptr<tflite::Interpreter> interpreter;
tflite::InterpreterBuilder(*model, resolver)(&interpreter);
interpreter->AllocateTensors();

// Write input
float* input = interpreter->typed_input_tensor<float>(0);
// ... fill input ...

// Run inference
interpreter->Invoke();

// Read output
float* output = interpreter->typed_output_tensor<float>(0);
```

**TFLite Micro (TFLM):**
Designed for microcontrollers without OS or heap allocator. Memory arena is a static buffer. Op resolver is built by registering only the required ops.

```cpp
tflite::MicroMutableOpResolver<4> resolver;
resolver.AddConv2D();
resolver.AddDepthwiseConv2D();
// ... add only needed ops

tflite::MicroInterpreter interpreter(
    tflite::GetModel(model_data),
    resolver,
    tensor_arena,
    kTensorArenaSize
);
interpreter.AllocateTensors();
interpreter.Invoke();
float output = interpreter.output(0)->data.f[0];
```

**Documentation:** [TensorFlow Lite guide](https://www.tensorflow.org/lite/guide) · [TFLM](https://www.tensorflow.org/lite/microcontrollers)

**Flex runtime:** Extension allowing TFLite to run TF ops not natively supported, via TF runtime plugin. Not compatible with TFLM (too resource-heavy); not recommended for edge-only targets.

**Model conversion notes:**
- Keras `.h5` / SavedModel → `.tflite` via `TFLiteConverter`
- PyTorch models → `.tflite` via [ai-edge-torch](https://github.com/google-ai-edge/ai-edge-torch)
- Quantisation may reduce accuracy; calibration dataset needed for INT8 static quantisation


![TFLite runtime diagram](illustrations/edge-ml-tflite-runtime-diagram.png)

---

## 3.2 PyTorch / ExecuTorch

### TorchScript (`.pt`) — Deprecated

TorchScript serialises a `torch.jit.ScriptModule` to a `.pt` file containing the model graph and weights. **Status: deprecated in PyTorch 2.x.** Still functional but not recommended for new projects.

**C++ loading (LibTorch):**

```cpp
#include <torch/script.h>
auto module = torch::jit::load("model.pt");
auto output = module.forward({input_tensor}).toTensor();
```

**Limitations:** Not all PyTorch operations are scriptable; dynamic control flow causes issues; no standardised edge deployment path.

### `torch.export` — Recommended (PyTorch 2.x)

Captures a strict ahead-of-time graph (ExportedProgram) as a pure ATen/ATEN dialect IR — no Python interpreter at runtime. Multiple downstream uses:

- `torch.export` → AOTInductor → compiled `.so` (server-side AOT)
- `torch.export` → ExecuTorch (edge deployment, see below)
- `torch.export` → Qualcomm AI Engine backend
- `torch.export` → Arm Ethos-U backend

### ExecuTorch (`.pte`) — Edge Deployment

PyTorch's answer to TFLite. **ExecuTorch 1.0 GA** was released October 2025. Current version is **1.1.0** (January 2026).

**File format:** `.pte` — FlatBuffers-serialised model graph + weights.

**Build pipeline (Python):**

```python
from torch.export import export
from executorch.exir import to_edge_transform_and_lower
from executorch.backends.xnnpack.partition.xnnpack_partitioner import XnnpackPartitioner

exported = export(model, example_inputs)
edge = to_edge_transform_and_lower(exported, partitioner=[XnnpackPartitioner()])
pte_bytes = edge.to_executorch().buffer  # write to .pte file
```

**Runtime:** Minimal C++ runtime (~100 KB core); no dependency on LibTorch. Loadable via C++, Java (Android), Objective-C/Swift (iOS).

**Backends (delegates):**

| Backend | Hardware |
| :--- | :--- |
| XNNPACK | CPU, quantised INT8 (default in pip wheels >= 0.6) |
| Core ML | Apple Silicon |
| Qualcomm AI Engine | Snapdragon NPU |
| Arm Ethos-U | Via Vela compiler (Cortex-M embedded) |
| Vulkan | GPU (Android / Linux) |
| MPS | Apple GPU |

**LLM support:** Dedicated LLM C++/Java/ObjC/Swift APIs; KleidiAI INT4 micro-kernels integrated via XNNPACK for ARM.

**References:** [ExecuTorch GitHub](https://github.com/pytorch/executorch) · [Architecture](https://pytorch.org/executorch/stable/getting-started-architecture) · [C++ tutorial](https://pytorch.org/executorch/stable/running-a-model-cpp-tutorial.html)

### LibTorch (C++ Frontend)

Full PyTorch C++ API: tensors, autograd, `torch::jit::load`. **Not suitable for edge** — binary size ~500 MB+. Use ExecuTorch runtime for edge deployment.

---

## 3.4 Other Training Tools

### CMSIS-NN

> CMSIS-NN is an **inference kernel library**, not a training framework — it belongs here because it is the standard Cortex-M deployment target for TFLite Micro models, so it appears at the end of all training workflows targeting ARM microcontrollers.

**Repository:** [github.com/ARM-software/CMSIS-NN](https://github.com/ARM-software/CMSIS-NN) · **Current version:** v7.0.0 (November 2024) · **Licence:** Apache 2.0

CMSIS-NN provides optimised neural network inference kernels for Arm Cortex-M processors. It is the official hardware-accelerated backend for TFLite Micro: building TFLM with `OPTIMIZED_KERNEL_DIR=cmsis_nn` replaces all reference kernels with CMSIS-NN implementations. Kernels are bit-exact with TFLM reference kernels (except for an optional single-rounding mode).

#### Processor tiers and SIMD capability

| Tier | Processors | Capability | Reported gain |
|---|---|---|---|
| Scalar (pure C) | Cortex-M0, M0+, M3 | No SIMD | baseline |
| DSP extension | Cortex-M4, M7, M33 | 32-bit SIMD (`SMLAD`, packed INT16) | 4.6× throughput, 4.9× energy (vs scalar, arXiv:1801.06601) |
| MVE / Helium | Cortex-M55, M85 | 128-bit vector, 16 INT8 lanes, `vmladava.s8` | up to 15× ML vs pre-MVE Cortex-M |

The implementation tier is selected at compile time via `ARM_MATH_MVEI`, `ARM_MATH_DSP`, or neither.

#### Quantisation scheme

CMSIS-NN implements the **TFLite asymmetric INT8 scheme**:
- Signed INT8 activations, range [−128, 127]; INT32 biases (never INT8)
- Asymmetric zero-point per tensor (float 0.0 ≠ integer 0)
- **Per-channel** quantisation for convolution weights: `int32_t *multiplier` and `int32_t *shift` arrays, one per output channel
- **Per-tensor** quantisation for activations and (default) fully-connected layers
- Requantisation uses the GEMMLOWP fixed-point multiplier+shift scheme; controlled by `CMSIS_NN_USE_SINGLE_ROUNDING`
- INT4 weight + INT8 activation support added in v6/v7 for weight-only quantised models

#### Memory model — `cmsis_nn_context` and scratch buffers

CMSIS-NN never allocates memory internally. All scratch memory is caller-supplied via:

```c
typedef struct {
    void   *buf;   // pointer to caller-allocated scratch buffer
    int32_t size;  // size in bytes
} cmsis_nn_context;
```

Every stateful operator has a paired buffer-size query function. The pattern is:

```c
// 1. Query at init time
int32_t buf_size = arm_convolve_wrapper_s8_get_buffer_size(
    &conv_params, &input_dims, &filter_dims, &output_dims);

// 2. Allocate (static or dynamic)
static int8_t scratch[BUF_SIZE];  // or malloc(buf_size)

// 3. Pass at inference time
cmsis_nn_context ctx = { .buf = scratch, .size = buf_size };
arm_convolve_wrapper_s8(&ctx, ...);
```

Scratch is primarily used for **im2col** (input patch rearrangement before matrix multiply). Pooling, activation, softmax, and elementwise ops require zero or minimal scratch.

#### API function reference

All functions return `arm_cmsis_nn_status` (`ARM_CMSIS_NN_SUCCESS` or `ARM_CMSIS_NN_ARG_ERROR`).

**Convolution**

```c
// Standard Conv2D — INT8 in/filter, INT32 bias, INT8 out; per-channel quant
arm_cmsis_nn_status arm_convolve_s8(
    const cmsis_nn_context *ctx,
    const cmsis_nn_conv_params *conv_params,
    const cmsis_nn_per_channel_quant_params *quant_params,
    const cmsis_nn_dims *input_dims,  const int8_t *input_data,
    const cmsis_nn_dims *filter_dims, const int8_t *filter_data,
    const cmsis_nn_dims *bias_dims,   const int32_t *bias_data,
    const cmsis_nn_dims *output_dims, int8_t *output_data);

// Recommended wrapper — automatically selects optimal kernel for ISA
arm_cmsis_nn_status arm_convolve_wrapper_s8(...);  // same params as above
int32_t arm_convolve_wrapper_s8_get_buffer_size(
    const cmsis_nn_conv_params*, const cmsis_nn_dims *input,
    const cmsis_nn_dims *filter, const cmsis_nn_dims *output);
```

**Depthwise convolution**

```c
arm_cmsis_nn_status arm_depthwise_conv_s8(
    const cmsis_nn_context *ctx,
    const cmsis_nn_dw_conv_params *dw_conv_params,  // includes ch_mult
    const cmsis_nn_per_channel_quant_params *quant_params,
    const cmsis_nn_dims *input_dims,  const int8_t *input_data,
    const cmsis_nn_dims *filter_dims, const int8_t *filter_data,
    const cmsis_nn_dims *bias_dims,   const int32_t *bias_data,
    const cmsis_nn_dims *output_dims, int8_t *output_data);

arm_cmsis_nn_status arm_depthwise_conv_wrapper_s8(...);  // ISA-optimised wrapper
```

**Fully connected**

```c
// Per-tensor quantisation (single multiplier/shift for whole layer)
arm_cmsis_nn_status arm_fully_connected_s8(
    const cmsis_nn_context *ctx,
    const cmsis_nn_fc_params *fc_params,
    const cmsis_nn_per_tensor_quant_params *quant_params,
    const cmsis_nn_dims *input_dims,  const int8_t *input_data,
    const cmsis_nn_dims *filter_dims, const int8_t *filter_data,
    const cmsis_nn_dims *bias_dims,   const int32_t *bias_data,
    const cmsis_nn_dims *output_dims, int8_t *output_data);
```

**Pooling**

```c
arm_cmsis_nn_status arm_avgpool_s8(
    const cmsis_nn_context *ctx,
    const cmsis_nn_pool_params *pool_params,
    const cmsis_nn_dims *input_dims, const int8_t *input_data,
    const cmsis_nn_dims *filter_dims,
    const cmsis_nn_dims *output_dims, int8_t *output_data);

arm_cmsis_nn_status arm_max_pool_s8(...);  // same signature
// INT16 variants: arm_avgpool_s16, arm_max_pool_s16
```

**Activations**

```c
void arm_relu_q7(int8_t *data, uint32_t size);      // in-place ReLU, INT8
void arm_relu6_s8(int8_t *data, uint32_t size);     // in-place ReLU6, INT8
void arm_relu_q15(int16_t *data, uint32_t size);    // in-place ReLU, INT16
```

**Softmax**

```c
void arm_softmax_s8(const int8_t *input, int32_t num_rows, int32_t row_size,
                    int32_t mult, int32_t shift, int8_t *output);
void arm_softmax_s8_s16(...);    // INT8 in → INT16 out
arm_cmsis_nn_status arm_softmax_s16(...);
void arm_softmax_u8(...);        // unsigned INT8
```

**Elementwise**

```c
arm_cmsis_nn_status arm_elementwise_add_s8(
    const int8_t *in1, const int8_t *in2,
    int32_t in1_offset, int32_t in2_offset,
    int8_t *output, int32_t out_offset,
    int32_t out_mult, int32_t out_shift, int32_t out_size);

arm_cmsis_nn_status arm_elementwise_mul_s8(...);
// INT16 variants: arm_elementwise_add_s16, arm_elementwise_mul_s16
```

**LSTM** (unidirectional, INT8 activations / INT16 state)

```c
arm_cmsis_nn_status arm_lstm_unidirectional_s8(
    const cmsis_nn_context *ctx,
    const cmsis_nn_lstm_params *lstm_params,
    const cmsis_nn_dims *input_dims,   const int8_t *input_data,
    const cmsis_nn_dims *weights_dims, const int8_t *weights_data,
    const cmsis_nn_dims *bias_dims,    const int32_t *bias_data,
    const cmsis_nn_dims *output_dims,  int8_t *output_data);
```

**SVDF** (Singular Value Decomposition Filter — compact RNN for keyword spotting)

```c
arm_cmsis_nn_status arm_svdf_s8(
    const cmsis_nn_context *ctx,
    const cmsis_nn_svdf_params *svdf_params,
    const cmsis_nn_dims *input_dims,           const int8_t *input_data,
    const cmsis_nn_dims *state_dims,           int16_t *state_data,  // mutable ring-buffer
    const cmsis_nn_dims *weights_feature_dims, const int8_t *weights_feature_data,
    const cmsis_nn_dims *weights_time_dims,    const int8_t *weights_time_data,
    const cmsis_nn_dims *bias_dims,            const int32_t *bias_data,
    const cmsis_nn_dims *output_dims,          int8_t *output_data);
```

**Batch matmul** (added v7 — for attention layers)

```c
arm_cmsis_nn_status arm_batch_matmul_s8(
    const cmsis_nn_context *ctx,
    const cmsis_nn_batch_matmul_params *params,
    const cmsis_nn_dims *input_dims,   const int8_t *input_data,
    const cmsis_nn_dims *weights_dims, const int8_t *weights_data,
    const cmsis_nn_dims *bias_dims,    const int32_t *bias_data,
    const cmsis_nn_dims *output_dims,  int8_t *output_data);
```

**v7 additions:** `arm_pad_s8` (zero-padding), `arm_transpose_s8` (dimension permutation), per-channel FC quantisation, updated `arm_convolve_s8` with `upscale_dims` for transposed convolution.

#### KleidiAI

[KleidiAI](https://gitlab.arm.com/kleidi/kleidiai) is a companion Arm library providing optimised **INT4/INT8 micro-kernels for Cortex-A** (not Cortex-M). Targeted at LLM inference on mobile and embedded Linux SoCs. Integrated into ExecuTorch via XNNPACK.

![CMSIS inference diagram](illustrations/edge-ml-cmsis-inference-diagram.png)

### scikit-learn

Python ML library for classical algorithms (SVM, Random Forest, logistic regression, etc.). Models serialised to `.pkl` (Python Pickle) or `.pmml` (XML). Not suitable for embedded/bare-metal without additional tooling.

### JAX

Google's composable function transformations framework (grad, jit, vmap, pmap). Can export models to XLA HLO or via `jax.export` → ONNX for downstream deployment.

### FEDML

Unified ML library for distributed training, model serving, and federated learning. Not suited for microcontrollers or resource-constrained edge environments. [FEDML GitHub](https://github.com/FedML-AI/FedML)

---

# 4. Portable Model Exchange Formats

## 4.1 Format Reference Table

| Format          | Extension | Producer | Notes        |
| :-------------- | :-------- | :------- | :----------- |
| TFLite / LiteRT | `.tflite` | TensorFlow Lite | FlatBuffer, Portable across CPU, GPU delegate, NNAPI, Edge TPU; quantisation supported |
| TorchScript     | `.pt`     | PyTorch | Deprecated; graph + weights; loads via `torch::jit::load` |
| ExecuTorch      | `.pte`    | PyTorch | FlatBuffers; minimal runtime; edge/embedded deployment |
| SavedModel      | directory | TensorFlow | Directory with `saved_model.pb` + variables; full TF graph |
| Frozen graph    | `.pb`     | TensorFlow | Serialised computation graph (Protobuf); static, no variables |
| Keras HDF5      | `.h5`, `.hdf5` | Keras / TF | Architecture + weights; Python-centric |
| Pickle          | `.pkl`    | scikit-learn, Python | Python-only serialisation; unsafe for untrusted inputs |
| PMML            | `.xml`, `.pmml` | R, Python | XML-based model exchange for classical ML |
| GGUF            | `.gguf`   | llama.cpp | LLM weight format (successor to GGML); see §7 |
| ML.NET Native   | `.zip`    | ML.NET | Encapsulates model + preprocessing pipeline for .NET |
| Google Coral (Edge TPU) | `.tflite` (partitioned) | Edge TPU Compiler splits a quantised TFLite model into Edge-TPU-executable ops and CPU fallback ops; the output is still a `.tflite` file with custom ops — see §5.14 |

> **Note:** Hardware-specific compiled formats (HEF, RKNN, QNN context binary) are not listed here — see §5.

---

## 4.2 ONNX (Open Neural Network Exchange)

ONNX is a framework-agnostic, cross-platform model representation. It uses Protobuf serialisation and defines a standard operator set (`opset`). Most training frameworks can export to ONNX.

**File extension:** `.onnx`

**ONNX Runtime (ORT)** is the reference inference engine. Execution is dispatched to hardware via **Execution Providers (EPs)**; the same API is used regardless of EP.

**C++ API:**

```cpp
Ort::Env env(ORT_LOGGING_LEVEL_WARNING, "session");
Ort::SessionOptions session_options;
session_options.AppendExecutionProvider_CUDA(cuda_options); // optional EP
Ort::Session session(env, L"model.onnx", session_options);

std::vector<const char*> input_names = {"input"};
std::vector<const char*> output_names = {"output"};
auto outputs = session.Run(Ort::RunOptions{nullptr},
                           input_names.data(), &input_tensor, 1,
                           output_names.data(), 1);
```

**Python API:**

```python
import onnxruntime as ort
sess = ort.InferenceSession("model.onnx",
    providers=["CUDAExecutionProvider", "CPUExecutionProvider"])
outputs = sess.run(None, {"input": input_array})
```

**Execution Providers:**

| EP | Hardware |
| :--- | :--- |
| `CPUExecutionProvider` | Default x86/ARM CPU |
| `CUDAExecutionProvider` | NVIDIA GPU |
| `TensorRTExecutionProvider` | NVIDIA TensorRT |
| `QNNExecutionProvider` | Qualcomm Snapdragon NPU |
| `OpenVINOExecutionProvider` | Intel CPU/GPU/VPU |
| `VitisAIExecutionProvider` | AMD/Xilinx DPU |
| `CoreMLExecutionProvider` | Apple Silicon / ANE |
| `ROCMExecutionProvider` | AMD ROCm GPU |
| `DirectMLExecutionProvider` | Windows DirectML |
| `ArmNNExecutionProvider` | Arm Cortex / Mali |
| `NNAPIExecutionProvider` | Android NNAPI |

**Quantisation:**

- **Static PTQ:** `onnxruntime.quantization.quantize_static()` — requires calibration dataset; inserts QDQ nodes.
- **Dynamic quantisation:** `quantize_dynamic()` — quantises weights; activations quantised at runtime.
- **QDQ format:** `QuantizeLinear` + `DequantizeLinear` pairs carry per-tensor or per-channel `scale` (FP32) and `zero_point` (INT8/UINT8). Formula: `y = (x − zero_point) × scale`.
- **INT8 modes:** S8S8 with QDQ (default), U8S8, U8U8.
- **INT4:** Supported in ONNX opset 21+ via `QuantizeLinear` with 4-bit output; used with QNN EP and TRT EP for weight-only quantisation.




---

## 4.3 Edge Impulse Formats

Edge Impulse is an end-to-end MLOps platform covering data collection, DSP block design, model training, and deployment. It wraps trained models together with their signal-processing pipeline into hardware-specific deployment packages.

### Import Formats (Bring Your Own Model — BYOM)

Models trained outside Edge Impulse can be imported directly into a project:

| Input Format | Notes |
| :--- | :--- |
| **TFLite / LiteRT** | `.tflite` or `.lite` files; most common path for MCU deployment |
| **ONNX** | `.onnx`; automatic quantisation applied on import; input order converted automatically |
| **TensorFlow SavedModel** | `saved_model.zip` archive |
| **PyTorch** | Export to ONNX first, then import as ONNX |
| **Keras** | Export to SavedModel or TFLite, then import |
| **scikit-learn** | Via custom learning blocks (Python wrapper) |
| **Generic C++ / custom blocks** | Custom learning blocks can wrap any inference library |

### Built-in Learning Blocks

Edge Impulse provides the following native ML algorithms. No external training framework is required for these:

**Classification:**
- Softmax neural network classifier (tabular, image, audio)
- Transfer learning (MobileNet, EfficientNet backbones — image and audio)

**Object Detection:**

| Block | Description | Target |
| :--- | :--- | :--- |
| **FOMO** (Faster Objects, More Objects) | Lightweight centroid-based detection; 30× less compute/memory than MobileNet SSD; real-time on deeply constrained MCUs | Cortex-M, ESP32 |
| **YOLO-Pro** | Purpose-built edge YOLO family; optimised for latency and resource usage on embedded targets | MCU and Linux |
| **MobileNet SSD** | Single Shot MultiBox Detector with MobileNet V2 backbone; standard embedded object detection | MCU and Linux |

**Other blocks:** Anomaly detection (K-Means / GMM), time-series classification, audio event detection, regression.

### Export / Deployment Formats

| Format | Architectures / Targets | Notes |
| :--- | :--- | :--- |
| **C++ library** | Any with C++11 compiler | Standalone source; no OS, stdlib, or dynamic memory required; includes DSP + ML code as `.zip` |
| **Arduino library** | ARM-based Arduino boards | `.zip` for Arduino IDE / PlatformIO; includes examples |
| **Linux EIM executable** | x86_64, ARMv7, AARCH64 | Self-contained binary; IPC via Unix socket; NEON on ARM; 64-bit EIM does not run on 32-bit systems |
| **WebAssembly** | Browser, Node.js | Full in-browser inference; includes JS/WASM bundle |
| **Docker container** | x86_64, AARCH64 | Exposes HTTP inference server; hardware acceleration auto-compiled for Linux targets |
| **iOS SDK** | Apple ARM (A-series, M-series) | Native Objective-C/Swift; includes examples |
| **Android SDK** | ARM64-v8a, ARMv7 | Native Java/Kotlin; includes examples |
| **TensorRT library** | NVIDIA Jetson (AARCH64) | CUDA-accelerated model compiled to TensorRT for Jetson Nano/Xavier/Orin/Thor; see §Jetson below |
| **Rust FFI bindings** | x86_64, AARCH64 | FFI-safe Rust bindings wrapping the C++ library |
| **Pure C source** | Bare metal / any | MCU-friendly; no C++ runtime needed |
| **Platform SDKs** | STM32 (CubeAI), Syntiant NDP, Nordic nRF, Ethos-U | Vendor-specific bundles with HAL integration |

**EIM IPC (Linux):** The `.eim` binary opens a Unix socket. A client sends raw feature vectors; results arrive as JSON. No runtime dependency beyond glibc.

**Language SDKs for EIM (Linux / Jetson):** Python, Node.js, Go, C++.

### EON Compiler

The Edge Optimised Neural (EON) Compiler transforms TFLite-based ML models directly into **C++ source code** rather than shipping a runtime interpreter over a FlatBuffer.

| Mode | RAM saving vs TFLite Micro | ROM / Flash saving |
| :--- | :--- | :--- |
| Standard EON | 25–55% | up to 35% |
| RAM Optimised (2024+) | **40–65%** | up to 35% |

Zero accuracy loss in both modes.

**How it works:**
- Generates C++ that encodes the model graph statically; no TFLite FlatBuffer parsing at runtime.
- Slices the model graph into segments to minimise peak RAM — each slice reuses the same arena.
- Shifts weight data into ROM more aggressively than TFLite Micro.
- Uses the linker to eliminate dead code.

**Supported targets:** All ARM Cortex-M and ESP32/Xtensa embedded targets supported by Edge Impulse.

**EON Tuner:** Automatically searches the space of DSP block + model architecture + EON settings combinations for a given RAM/latency budget.

### Inference Runtimes (per target)

| Target class | Runtime | Notes |
| :--- | :--- | :--- |
| ARM Cortex-M MCUs | TFLite Micro (default) or EON Compiler | CMSIS-NN kernels; static arena; no OS |
| ESP32 / Xtensa | TFLite Micro or EON Compiler | ESP-NN SIMD kernels |
| Nordic nRF / STM32 | TFLite Micro or EON Compiler | CubeAI integration on STM32 |
| Syntiant NDP | Syntiant runtime | Dedicated NDP block |
| Arm Ethos-U | TFLite Micro + Vela-compiled kernels | Via Ethos-U driver |
| Linux x86_64 / ARM | Full TFLite or ONNX Runtime | `USE_FULL_TFLITE=1` build flag for full interpreter |
| NVIDIA Jetson | TensorRT (CUDA-accelerated) | AARCH64 EIM or TensorRT library export |
| Browser / Node.js | WASM runtime | Specialised WebAssembly inference path |

### NVIDIA Jetson Support

Edge Impulse provides official support for the full Jetson family (Nano, Xavier, Orin, Thor):

- **EIM deployment:** Download an AARCH64 `.eim` executable directly to the Jetson; run with the Linux SDK (Python, Node.js, Go, C++).
- **TensorRT library export:** Studio compiles the impulse to a TensorRT-optimised library with CUDA acceleration; link it into a custom C++ application.
- **Hardware acceleration:** GPU acceleration is automatically enabled when exporting or building for Jetson targets — no manual TRT configuration needed in the basic flow.
- **Docker:** Jetson Docker containers expose an HTTP inference endpoint.

**OTA model updates:** New impulses can be deployed as OTA updates; see [lifecycle management docs](https://docs.edgeimpulse.com/docs/tutorials/lifecycle-management/ota-model-updates).

### Supported Hardware (MCU / SoC Families)

| Family | Examples |
| :--- | :--- |
| ARM Cortex-M0+ | RAKwireless WisBlock RP2040, Raspberry Pi Pico |
| ARM Cortex-M4F | Nordic nRF52840, STM32F4 series (80 MHz default target) |
| ARM Cortex-M33 | Raspberry Pi Pico RP2350, STM32U5 |
| ARM Cortex-M7 | STM32H7, MIMXRT series |
| ARM Cortex-M55 + Ethos-U55/U65 | STM32N6 with AI accelerator |
| Espressif Xtensa | ESP32 (240 MHz LX6), ESP32-S3, ESP32-C3 |
| Nordic Semiconductor | nRF52840, nRF5340, nRF9160/9161/9151, nRF7002, Thingy:53/91 |
| STMicroelectronics | STM32N6570-DK, STM32F/H/U/L series |
| Syntiant | NDP101, NDP120 (always-on audio/sensor NPU) |
| NVIDIA Jetson | Nano, Xavier NX, AGX Xavier, Orin NX, AGX Orin, Thor |
| Linux x86_64 / ARM | Raspberry Pi, generic SBCs, desktop |

**References:** [Deployment docs](https://docs.edgeimpulse.com/docs/edge-impulse-studio/deployment) · [BYOM](https://docs.edgeimpulse.com/docs/edge-impulse-studio/bring-your-own-model-byom) · [EON Compiler](https://docs.edgeimpulse.com/docs/edge-impulse-studio/deployment/eon-compiler) · [Linux EIM](https://docs.edgeimpulse.com/docs/run-inference/linux-eim-executable) · [Jetson support](https://docs.edgeimpulse.com/hardware/boards/nvidia-jetson)

---

# 5. Hardware-Specific Compilation

Hardware-specific compilation converts a portable model (ONNX, TFLite, etc.) into a device-locked binary optimised for a particular chip family. This is a build-time step that must be re-run for each target device or firmware version. The resulting binary is not portable between chip generations.

## 5.1 NVIDIA TensorRT

TensorRT is NVIDIA's inference optimisation library. It parses ONNX (or other formats), applies layer fusion, kernel auto-tuning, and precision calibration, then serialises to a hardware-specific engine file.

**Builder API — Python (TensorRT 10.x):**

```python
import tensorrt as trt

logger = trt.Logger(trt.Logger.WARNING)
builder = trt.Builder(logger)
network = builder.create_network(1 << int(trt.NetworkDefinitionCreationFlag.EXPLICIT_BATCH))
parser = trt.OnnxParser(network, logger)
parser.parse_from_file("model.onnx")

config = builder.create_builder_config()
config.set_memory_pool_limit(trt.MemoryPoolType.WORKSPACE, 1 << 30)

# Optional INT8
config.set_flag(trt.BuilderFlag.INT8)
config.int8_calibrator = MyCalibrator(calibration_data)

serialized_engine = builder.build_serialized_network(network, config)
with open("model.engine", "wb") as f:
    f.write(serialized_engine)
```

**Builder API — C++ (TensorRT 10.x):**

```cpp
auto builder = std::unique_ptr<nvinfer1::IBuilder>(nvinfer1::createInferBuilder(logger));
auto network = std::unique_ptr<nvinfer1::INetworkDefinition>(
    builder->createNetworkV2(1U << static_cast<uint32_t>(
        nvinfer1::NetworkDefinitionCreationFlag::kEXPLICIT_BATCH)));
auto parser = std::unique_ptr<nvonnxparser::IParser>(
    nvonnxparser::createParser(*network, logger));
parser->parseFromFile("model.onnx", static_cast<int>(trt::Logger::Severity::kWARNING));

auto config = std::unique_ptr<nvinfer1::IBuilderConfig>(builder->createBuilderConfig());
auto serialized = std::unique_ptr<nvinfer1::IHostMemory>(
    builder->buildSerializedNetwork(*network, *config));
// write serialized->data() / serialized->size() to file
```

**Engine file format:** Conventionally `.engine` or `.plan` (interchangeable). **Not portable** — engines are specific to GPU architecture, TRT version (major.minor.patch.build), CUDA, and cuDNN. Cannot be shared between host types.

**INT8 calibration:** Requires a calibrator implementing `IInt8EntropyCalibrator2`; first run writes a `.cache` file reused on subsequent builds.

**TensorRT 10 notable changes:**
- INT4 weight-only quantisation (block quantisation)
- Weight-stripped engines: small `.engine` + separate weight file; refitted at runtime
- `IStreamWriter` for serialising engine to custom streams
- `build_serialized_network` returns `IHostMemory`; `buildEngineWithConfig` removed

**Standalone TensorRT:** Latest version is **10.16.0.72** (March 2026). TensorRT 11 has been announced for Q2 2026; breaking changes include removal of IPluginV2 APIs (replaced by IPluginV3).

**Jetson / JetPack versions:**

| JetPack | TensorRT | CUDA | DLA version | Jetson family |
| :--- | :--- | :--- | :--- | :--- |
| JP 6.2.1 (current, Orin) | 10.3 | 12.6 | 3.1 | Orin series |
| JP 7.1 (Thor, Aug 2025+) | TBD | 13.0 | — | Jetson Thor |

**DLA (Deep Learning Accelerator):** Fixed-function accelerator alongside the GPU on Jetson AGX Orin. Configured per-layer via `config.set_device_type(layer, trt.DeviceType.DLA)`. Primary benefit is energy efficiency and throughput, not latency vs GPU. Engine files built for GPU and DLA are not interchangeable.

**Upgradable Compute Stack (JP6+):** TensorRT and CUDA can be upgraded independently of the BSP.

**`trtexec`:** CLI tool on Jetson for building and benchmarking engines.

### NVIDIA DeepStream SDK

DeepStream is NVIDIA's streaming analytics toolkit built on top of TensorRT and GStreamer. It provides an end-to-end pipeline from video input to intelligent analytics with zero-copy GPU buffer management throughout.

**Architecture:**

```
Video source (RTSP / USB / file)
    │  GStreamer pipeline
    ▼
nvstreammux  — batches multiple streams into a single tensor
    │
    ▼
nvinfer  — TensorRT inference (wraps engine file; handles pre/post-process)
    │
    ▼
nvtracker  — multi-object tracking (NvDCF, DeepSORT, etc.)
    │
    ▼
nvdsanalytics — line-crossing, ROI occupancy, direction, crowd density
    │
    ▼
Output sink (display / RTSP re-stream / Kafka / file / custom callback)
```

**Key components:**

| Component | Role |
| :--- | :--- |
| `nvinfer` | TensorRT engine loader + inference; configurable batch size, precision (INT8/FP16/FP32), custom parser plugins |
| `nvtracker` | Object tracking across frames; NvDCF (correlation filter, default), DeepSORT, ByteTrack; maintains object IDs |
| `nvdsanalytics` | Spatial analytics: line crossing, ROI occupancy, direction, crowd density |
| `nvstreammux` | Batches N input streams into a single GPU tensor batch for inference |
| `NvDsInferContext` | C++ API for managing inference context; handles engine loading, input preprocessing, output parsing |
| Custom parser plugins | `NvDsInferParseCustomYoloV5/V8` and similar; replace default tensor layout parsing for non-standard models |
| `NvBufSurface` | Zero-copy GPU buffer surface; avoids CPU round-trip for image data throughout the pipeline |

**Inference configuration (`config_infer_primary.txt` excerpt):**

```ini
[property]
gpu-id=0
net-scale-factor=0.0039216
model-engine-file=model.engine
labelfile-path=labels.txt
batch-size=4
network-mode=0          # 0=FP32, 1=INT8, 2=FP16
num-detected-classes=80
interval=0              # process every frame
gie-unique-id=1

[class-attrs-all]
threshold=0.5
nms-iou-threshold=0.45
```

**Python API (pyds):**

```python
import pyds

def osd_sink_pad_buffer_probe(pad, info, u_data):
    frame_meta = pyds.gst_buffer_get_nvds_batch_meta(info.get_buffer())
    for frame in pyds.NvDsBatchMeta.cast(frame_meta).frame_meta_list:
        frame_meta = pyds.NvDsFrameMeta.cast(frame.data)
        for obj in pyds.NvDsFrameMeta.cast(frame_meta).obj_meta_list:
            obj_meta = pyds.NvDsObjectMeta.cast(obj.data)
            print(f"class={obj_meta.class_id} conf={obj_meta.confidence:.2f} "
                  f"bbox=({obj_meta.rect_params.left:.0f},{obj_meta.rect_params.top:.0f},"
                  f"{obj_meta.rect_params.width:.0f},{obj_meta.rect_params.height:.0f})")
```

**Platform and dependency:**
- Requires JetPack 6.x or later on Jetson; also available on dGPU (x86 + NVIDIA GPU) via separate installer
- Depends on: GStreamer 1.x, CUDA, TensorRT, cuDNN, OpenCV; significant runtime footprint (~several GB installed)
- License: NVIDIA proprietary (NVIDIA DeepStream SDK License Agreement); redistribution restricted
- Versioning is tightly coupled to JetPack: DeepStream 7.x targets JetPack 6.x; a JetPack major upgrade typically requires a parallel DeepStream upgrade

**When to use DeepStream vs plain TensorRT:**

| Requirement | Plain TensorRT | DeepStream |
| :--- | :--- | :--- |
| Single model, custom application | ✓ preferred — minimal deps | overkill |
| Multi-stream video analytics | complex to build manually | ✓ built-in |
| Multi-model cascade (detect → classify → track) | manual pipeline | ✓ native |
| Object tracking / ID persistence | manual | ✓ NvDCF / DeepSORT |
| Spatial analytics (line cross, ROI) | manual | ✓ nvdsanalytics |
| Resource-constrained Jetson (Orin Nano, etc.) | ✓ lower overhead | high overhead |

**References:** [DeepStream developer guide](https://docs.nvidia.com/metropolis/deepstream/dev-guide/) · [deepstream-app sample](https://github.com/NVIDIA-AI-IOT/deepstream_python_apps)

---

## 5.2 Hailo — HEF

**Toolchain:** Hailo AI Software Suite; central tool is the **Hailo Dataflow Compiler (DFC)**.

**Compilation pipeline:**
1. Input: ONNX, TensorFlow (frozen graph / checkpoint), TFLite.
2. DFC parses the model into **HAR** (Hailo Archive) — the intermediate format used for optimisation and quantisation.
3. DFC compiles HAR → **HEF** (Hailo Executable Format) binary — the device-executable artifact.

**HEF is hardware-specific:** separate HEF files are required per target chip.

**Supported targets:** `hailo8`, `hailo8r`, `hailo8l`, `hailo15h`, `hailo15m`, `hailo15l`, `hailo10h`

**DFC / HailoRT versions:**

| Suite Release | DFC | HailoRT |
| :--- | :--- | :--- |
| March 2024 | v3.27.0 | 4.17 |
| October 2024 | v3.29.0 | 4.19 |
| January 2025 | v3.30.0 | 4.20 |
| April 2025 | v3.31.x | 4.21 |
| July 2025 | v3.32.x | 4.22 |
| August 2025 | v3.33.x | 4.23 |

**Runtime:** HailoRT C/C++ and Python APIs consume HEF files. Outputs are quantised INT8; dequantisation uses `quant_info.qp_scale` and `quant_info.qp_zp` from the output VStream info (see §6.2).

**Post-processing:** NMS and other post-processing can be compiled into the HEF (on supported architectures) via model scripts, eliminating CPU post-processing overhead.

**Model Zoo:** Pre-compiled HEF files available; includes YOLOv8/v9/v10/v12, SigLIP (siglip_l_16_256, siglip2_l_16_256, siglip_b_16), PaddleOCR v5 (mobile detection and recognition), StereoNet (stereo depth estimation), and a dedicated GenAI zoo (hailo_model_zoo_genai) for generative AI models.

**References:** [Hailo Model Zoo](https://github.com/hailo-ai/hailo_model_zoo) · [Application Code Examples (C++)](https://github.com/hailo-ai/hailo-apps/tree/main/hailo_apps/cpp/)

![YOLOv8 pose Hailo model explorer](illustrations/edge-ml-yolo8-pose-hailo-model-explorer.png)

---

## 5.3 DeepX — DXNN

**Format:** Proprietary **DXNN** binary (`.dxnn`).

**SDK components (DXNN SDK):**

| Component | Role |
| :--- | :--- |
| **DX-COM** | Converts ONNX → DXNN; quantisation and graph optimisation |
| **DX-SIM** | Host-side model simulation |
| **DX-RT** | On-device C runtime; inference using DXNN binaries |
| **DX-NPU Driver** | Device driver layer |
| **DX-APP** | Sample application code |

**Core pipeline:** ONNX → Quantizer → DX-COM Compiler → `.dxnn` binary → DX-RT runtime

**Hardware targets:** DX-M1 (M.2 module), DX-H1 V-NPU

**References:** [DXNN SDK GitHub](https://github.com/DEEPX-AI/dx-all-suite) · [Developer Portal](https://developer.deepx.ai/)

---

## 5.4 Qualcomm — QNN / DLC (SNPE)

**Two SDK generations:**

| SDK | Format | Hardware | Portability |
| :--- | :--- | :--- | :--- |
| **SNPE** (legacy) | `.dlc` (Deep Learning Container) | CPU, GPU, DSP, NPU | Hardware-agnostic; forward-compatible across SDK versions |
| **QNN** (current) | Context binary (`.bin`) | NPU only (SOC-specific) | SOC-specific; not portable between chip generations |

**Key distinction:** DLC runs on any EP; QNN context binary targets only the NPU of a specific SOC.

**Pipeline:** `.onnx` → SNPE/QNN converter → `.dlc` → QNN compiler → `.bin`

**ONNX Runtime integration:** QNN EP embeds a pre-compiled QNN context binary within an ONNX model for deployment via ORT on Snapdragon devices.

**Qualcomm AI Hub:** Cloud-based compilation and profiling service for QNN/DLC targets.

**Quantisation:** INT8; W8A8 and W4A8 supported in QNN.

**References:** [QNN ORT EP](https://onnxruntime.ai/docs/execution-providers/QNN-ExecutionProvider.html) · [AI Hub](https://workbench.aihub.qualcomm.com/)

---

## 5.5 Rockchip — RKNN

**Format:** `.rknn` binary (device-specific).

**Toolchain:**

| Tool | Role |
| :--- | :--- |
| **RKNN-Toolkit2** (Python, host) | Model conversion, quantisation, simulation, performance profiling. Latest: v2.3.2 (April 2025). Python 3.6–3.12. |
| **RKNN-Toolkit-Lite2** | Python inference API for on-device use |
| **RKNN Runtime (C API)** | C/C++ inference on device |
| **rknn-llm** | Separate toolchain for LLM deployment on Rockchip NPUs |

**Input formats:** ONNX, TFLite, Caffe, PyTorch (via ONNX export), Keras

**Targets:** RK3588, RK3566, RK3568, RV1109, RV1126, etc.

**Quantisation:** INT8 (symmetric/asymmetric), INT4 (selected ops). Calibration dataset passed during conversion.

**Workflow:** `rknn-toolkit2` on host → `.rknn` file → deploy via RKNN Runtime C API or Toolkit-Lite2 Python API on device.

**References:** [RKNN-Toolkit2 GitHub](https://github.com/rockchip-linux/rknn-toolkit2) · [Ultralytics RKNN integration](https://docs.ultralytics.com/integrations/rockchip-rknn/)

---

## 5.6 NXP — eIQ / Neutron NPU

**Framework:** eIQ Toolkit (ML workflow tool, inference engines, neural network compilers).

**Primary NPU flow (i.MX93, MCX N-series with Neutron NPU):**

1. Train → Export quantised TFLite
2. Neutron Converter Tool → Neutron-optimised binary
3. Deploy on-device

**Note:** eIQ Portal (BYOM) does **not** support direct NPU export; separate conversion tool required after quantised TFLite export.

**Ethos-U path (i.MX93):** Uses Arm Vela compiler (see §5.7) after quantised TFLite export.

**Inference engines included in eIQ:** TensorFlow Lite / LiteRT, ONNX Runtime, PyTorch (via TFLite export).

**eIQ Toolkit current release:** Rev. 19 (April 2025)

**References:** [NXP eIQ Fact Sheet](https://www.nxp.com/docs/en/fact-sheet/EIQ-FS.pdf) · [eIQ Toolkit Release Notes](https://www.nxp.jp/docs/en/release-note/EIQTRN.pdf)

![NXP eIQ diagram](illustrations/edge-ml-nxp-eiq-diagram.png)

eIQ feeds into DeepViewRT, TensorFlow Lite, TFLite Micro, Glow, and Arm NN inference engines (Edgeverse processors).

---

## 5.7 Arm — Ethos-U (Vela)

**Target hardware:** Ethos-U55, Ethos-U65, Ethos-U85 (microNPU class, Cortex-M ecosystem).
Note: Ethos-N is a separate server/mobile-class NPU; Ethos-U is the embedded variant.

**Compiler:** **Ethos-U Vela** (Python package: `ethos-u-vela` on PyPI)

**Input formats:** Quantised TFLite / LiteRT, or TOSA. Quantised activations and weights are required.

**Output formats:**
- **TFLite output:** Replaces acceleratable ops with TFLite Custom operators for Ethos-U; CPU fallback ops left unchanged.
- **Raw (`.npz`):** Contains the Ethos-U command stream and weight data.

**ExecuTorch integration:** ExecuTorch has an Arm Ethos-U backend (`backends-arm-ethos-u`); workflow: `torch.export` → ExecuTorch edge dialect → Arm backend → Vela-compiled artifact.

**Deployment:** Ethos-U driver + compiled binary run via TFLite Micro or bare-metal runtimes.

**References:** [Vela compiler docs](https://developer.arm.com/documentation/109267/latest/) · [ethos-u-vela PyPI](https://pypi.org/project/ethos-u-vela/) · [ExecuTorch Arm backend](https://docs.pytorch.org/executorch/stable/backends-arm-ethos-u.html)

---

## 5.8 MediaTek — NeuroPilot / APU

**APU:** AI Processing Unit on Dimensity and Genio SoCs.

**Format:** **DLA (Deep Learning Archive)** — low-level binary for MDLA (Multi-Core DLA) and VPU compute devices.

**Compiler:** `ncc-tflite` (Neuron Compiler) transforms quantised TFLite → DLA binary.

**Two compilation modes:**

| Mode | Description |
| :--- | :--- |
| **On-device (JIT)** | TFLite loaded on device; compiled at first run via NNAPI or Neuron Adapter API |
| **Offline (AOT)** | TFLite compiled to DLA on host; DLA file deployed directly; platform-specific |

**2024 integration:** Google LiteRT + NeuroPilot Accelerator stack; MediaTek Dimensity NPUs become first-class LiteRT targets including LLM inference.

**Neuron SDK:** Available for Genio IoT platforms (Yocto-based).

**References:** [NeuroPilot](https://neuropilot.mediatek.com/) · [MediaTek NPU + LiteRT blog](https://developers.googleblog.com/mediatek-npu-and-litert-powering-the-next-generation-of-on-device-ai/)

---

## 5.9 Kneron — NEF

**Format:** **NEF (NPU Executable Format)** — can contain one or multiple models; hardware-specific to Kneron SoCs.

**Toolchain:**

| Component                        | Role                               |
| :------------------------------- | :--------------------------------- |
| `onnx2onnx.py` (ONNX Converter)  | Optimises ONNX graph; handles layer cutting via `editor.py` |
| **Quantizer**                    | INT8 post-training quantisation    |
| **Compiler**                     | ONNX (optimised) → NEF binary      |
| **Evaluator / Simulator**        | Host-side performance estimation   |

**Input:** ONNX (preferred); Caffe and TFLite via prior conversion.

**Hardware targets:** KL520, KL720, KL530, KL730 (KDP series NPUs).

**Runtime:** Kneron PLUS C/C++ API (`kp_*` functions) and Python bindings on host; on-device via KDP firmware. `kdp_host.h` C API for USB/PCIe device communication.

**References:** [Kneron Document Centre](https://doc.kneron.com/docs/)

---

## 5.10 Axelera — Metis (Voyager SDK)

**Hardware:** Metis AIPU — up to 214 TOPS on Metis Compute Board (paired with RK3588).

**SDK:** **Voyager SDK** — full compilation, optimisation, and deployment stack.

**Input formats:** PyTorch, ONNX, TensorFlow.

**Compilation:** Voyager SDK automatically quantises and compiles to Metis AIPU-optimised code; no separate PTQ step required by default.

**Pipeline description:** YAML files define AI pipeline topology; SDK auto-generates code runnable as GStreamer plugins or inference server components.

**Deployment:** One-click deployment to Metis-enabled devices from Voyager SDK.

**References:** [Axelera Metis](https://axelera.ai/) · [Technical deep-dive](https://community.axelera.ai/product-updates/the-metis-ai-platform-a-technical-deepdive-125)

---

## 5.11 Ambarella — CVflow

**Architecture:** Proprietary CNN accelerator, present in CV25, CV28, CV5, CV72, etc.

**Toolchain:** Ambarella CNN Generation Tool + Compiler (NDA tool, not publicly distributed).

**Input formats:** Caffe, TensorFlow, ONNX, PyTorch (via ONNX export), MXNet.

**Compilation pipeline:**
1. CNN Generation Tool: analyses, sparsifies, quantises the network.
2. Compiler: generates a high-level program → **DAG executable binary** for CVflow hardware.

**Output:** DAG executable binary (proprietary, device-specific).

**Cloud integration:** Amazon SageMaker Edge integration enables cloud-based compilation for Ambarella targets.

**References:** [Ambarella CVflow](https://www.ambarella.com/technology/) · [AWS SageMaker + CV25](https://aws.amazon.com/blogs/machine-learning/ml-inferencing-at-the-edge-with-amazon-sagemaker-edge-and-ambarella-cv25/)

---

## 5.12 BrainChip — Akida

Neuromorphic computing using Spiking Neural Networks (SNNs). AKD1000 is the production NPU chip. Low-power (milliwatt-class); suited for always-on inference.

Other neuromorphic / edge AI companies:
- **GrAI Matter Labs** (France/Netherlands): GrAI One chip, microsecond latency, NeuronFlow technology
- **SynSense** (Switzerland): Neuromorphic processors + Dynamic Vision Sensors
- **Prophesee** (France): Event-based Metavision sensors (Sony, Bosch partnerships)

---

## 5.13 Large Integrations (Data Centre / Cloud)

These are server/cloud inference platforms rather than edge NPUs, but relevant when edge devices offload to cloud or when models are compiled for deployment.

| Platform | Notes |
| :--- | :--- |
| **NVIDIA** (H100/A100/H200, DGX) | Dominant data-centre GPU; TensorRT for inference; CUDA ecosystem |
| **Google TPU** (v4, v5) | Tensor Processing Units via GCP; optimised for TF/JAX training and inference |
| **AMD** (MI300X) | ROCm software stack; growing as NVIDIA alternative |
| **Intel** (Gaudi, Xeon AI) | Gaudi accelerators (from Habana Labs); Arc + data centre GPU |
| **Qualcomm** (Cloud AI 100) | Edge and data-centre inference accelerators |
| **Cerebras** (WSE) | Wafer-scale engines; entire NN on single chip |
| **Graphcore** (IPU) | Intelligence Processing Units for sparse/graph ML workloads |
| **Groq** (TSP) | Tensor Streaming Processors; deterministic inference latency |
| **Tenstorrent**, **d-Matrix** | Specialised architectures in early market adoption |

---

## 5.14 Google Coral — Edge TPU

**Compiler:** Google Edge TPU Compiler (x86-64 host tool; `edgetpu_compiler` CLI)

**Compilation pipeline:**
1. Input: fully INT8-quantised TFLite model (all tensors, not just weights — full static quantisation required)
2. Edge TPU Compiler partitions the computation graph: operations supported by the Edge TPU are fused into a single `edgetpu-custom-op` TFLite Custom Op; unsupported operations remain as standard CPU-executed TFLite ops
3. Output: `.tflite` file with the partitioned graph (same extension — not a new file format)

**Key architectural constraint:** The Edge TPU contains approximately **8 MB of on-chip SRAM** that the compiler uses as a parameter cache. The compiler attempts to place as many weight tensors as possible into this SRAM at load time. Weights that fit remain resident across inferences ("cached"); weights that do not fit must stream from external memory on each inference, substantially degrading throughput. This is why small models (MobileNet v1: ~2.4 ms) are dramatically faster than large models (VGG19: ~357 ms) on the same chip.

**Performance:**
- 4 TOPS (INT8) peak throughput
- ~2 TOPS/watt power efficiency (~2 W at peak)
- On-chip SRAM: ~8 MB (compiler-managed scratchpad, not a traditional cache)

**Supported operations:** Conv2D, DepthwiseConv2D, FullyConnected, AveragePool2D, MaxPool2D, LSTM, ReLU, Sigmoid, SoftMax, and ~40 others — consult the [Edge TPU model requirements](https://coral.ai/docs/edgetpu/models-intro/) for the full list and constraints.

**Internal architecture:** The Edge TPU is a custom Google ASIC; its internal microarchitecture is not publicly documented. The chip is fabricated at TSMC. Whether internal RISC-V control cores are used for sequencing (as is common in similar ASICs) is unconfirmed — no architectural specification has been published by Google. See §5.15 for confirmed RISC-V based ML processors.

**Product line:**

| Product | Host processor | Notes |
| :--- | :--- | :--- |
| Coral Dev Board | NXP i.MX 8M (4× Cortex-A53 + M4) | Linux SBC with integrated Edge TPU |
| Coral Dev Board Mini | MediaTek MT8167S (4× Cortex-A35) | Compact SBC |
| Coral Dev Board Micro | NXP i.MX RT1176 (Cortex-M7 + M4) | MCU-class host; TFLite Micro runtime |
| Coral USB Accelerator | USB 3.0 attached | Plug into any x86 or ARM Linux host |
| Coral M.2 Accelerator | PCIe/USB M.2 module | Jetson, x86 platforms |
| Coral SOM | i.MX 8M based | System-on-Module for custom board designs |

**References:** [Coral product page](https://coral.ai/) · [Edge TPU Compiler docs](https://coral.ai/docs/edgetpu/compiler/) · [Model requirements](https://coral.ai/docs/edgetpu/models-intro/) · [Coral Model Zoo](https://coral.ai/models/)

---

## 5.15 RISC-V Based ML Processors

Several edge ML accelerators are built around multi-core RISC-V architectures. These are distinct from both ARM-based platforms (Coral, Hailo host SoCs) and proprietary NPU ISAs.

### GreenWaves Technologies — GAP8 / GAP9

The most prominent multi-core RISC-V ML processors in the embedded/IoT space.

**Architecture:**
- **GAP8:** 1× RISC-V Fabric Controller core (I/O + task dispatch) + 8× RISC-V compute cluster cores; all RV32IMC with DSP/SIMD extensions; Hardware Convolutional Engine (HWCE) in the cluster; ~50 GOPS at ~1/10 W
- **GAP9:** 1 FC + 9 cluster RISC-V cores; extended vector ISA; integrated neural engine; higher GOPS, lower power than GAP8; suited for always-on camera inference from battery

**Software:** GreenWaves NNTool converts TFLite models to GAP8/GAP9 optimised C code; supports INT8 and INT4 quantisation.

**References:** [GreenWaves GAP SDK](https://github.com/GreenWaves-Technologies/gap_sdk) · [NNTool](https://github.com/GreenWaves-Technologies/gap_sdk/tree/master/tools/nntool)

### Canaan — Kendryte K210

- **2× RISC-V RV64IMAFDC cores** (64-bit dual-core) at up to 800 MHz
- **KPU (Knowledge Processing Unit):** CNN accelerator, 0.23–1 TOPS INT8
- **APU:** audio processing unit for keyword detection
- Widely deployed in cheap ML devkits: Sipeed Maixduino, M5StickV, Sipeed Dock
- Supports TFLite and Caffe model inference via KPU C/C++ APIs
- Available from Seeed, SparkFun, M5Stack ecosystem

### Bouffalo Lab — BL808

- **3× RISC-V cores:** C906 (RV64, high-performance), E907 (RV32, application), E902 (RV32, real-time/always-on)
- **BLAI-100 NPU:** 100 GOPS INT8, on-chip
- Used in Sipeed M1S Dock and similar boards; marketed as an alternative to Coral USB Accelerator in resource-constrained Linux devkit contexts

### Comparison with Coral Edge TPU

| | Coral Edge TPU | GAP8/GAP9 | K210 | BL808 |
| :--- | :--- | :--- | :--- | :--- |
| Host CPU ISA | ARM (all products) | RISC-V | RISC-V | RISC-V |
| NPU/accelerator | Custom ASIC (4 TOPS) | HWCE in cluster | KPU (0.23–1 TOPS) | BLAI-100 (100 GOPS) |
| OS | Linux (Dev Board), bare-metal (Dev Board Micro) | Bare-metal / FreeRTOS | Bare-metal / FreeRTOS | Linux / bare-metal |
| Model format | INT8 TFLite (partitioned) | NNTool C code / TFLite | TFLite / Caffe | TFLite |
| Power class | ~2 W (NPU) | ~0.1 W | ~0.3 W | ~0.5 W |

---

# 6. Inference Post-Processing Pipeline

## 6.1 Pipeline Overview

A complete edge inference pipeline passes through up to five stages after the inference engine has computed output tensors. Vendor SDKs often conflate multiple stages into a single library or pipeline configuration. The inxware function block design tracks which stages a given glue-code module covers so that conflicting or duplicate stages are not selected.

```
Raw model output tensors (INT8 / UINT8 / FP16 / FP32)
         |
         v
1. Framework-specific raw output unpacking
         |
         v
2. Dequantisation  (scale × (value − zero_point) → float)
         |
         v
3. Model-architecture decoding  (anchor decode, DFL, grid offsets, tensor routing)
         |
         v
4. Logical post-processing  (confidence filter, NMS, plausibility checks)
         |
         v
5. Output formatting  (JSON, binary, Protobuf, ROS2 message, …)
```

**Example function block configuration options:**

*Stage 1 — Framework raw output unpacking:*
- `tflite-int8 (general)` / `tflite-fp16` / `tflite-fp32`
- `e-impulse-eon_yolopro_object`
- `hailo8-10_yolo8s_pose_json` / `hailo8-10_yolo8m_pose_json`
- `hailo8-10_yolo8_object_json`
- `hailo8-10_yolov5-11_objdet`
- `hailo8-10_onnxrt_general` (requires ONNX Runtime)

*Stage 3 — Model-architecture decoding:* YOLOv5-object, YOLOv8-object, YOLOv8-pose, YOLOv10, YOLOv11-objdet, YOLOv11-obb, MoveNet, SCDepthv3, CLIP, FCN16-ResNet-v1-18, ...

*Stage 4 — Logical post-processing:* None, Object Detection NMS, Pose OKS-NMS, ...

*Stage 5 — Output formatting:* JSON-flat, JSON-structured, Protobuf, FlatBuffers, ...

---

## 6.2 Raw Output / Dequantisation

### When Dequantisation Happens

| Scenario | Location |
| :--- | :--- |
| General runtimes (ORT, TFLite) | On-device; QDQ nodes dequantise between layers or at model output |
| NPU hardware (Hailo, Rockchip, etc.) | NPU computes INT8; **host-side post-processing** reads INT8 outputs and applies dequant using scale/zero_point from model metadata |
| Weight-only quantisation (LLMs, INT4) | Weights stored INT4; dequantised to FP16/BF16 just before the GEMM kernel |
| TensorRT INT8 | Calibration establishes per-layer scale; TRT fuses QDQ into layers — no explicit dequant at output unless FP32 result needed |

### Hailo-Specific Dequantisation

HEF stores per-output-tensor quantisation parameters:

```cpp
// HailoRT C++ API
auto output_info = output_vstream.get_info();
float qp_scale = output_info.quant_info.qp_scale;  // FP32 scale
int   qp_zp    = output_info.quant_info.qp_zp;     // INT zero-point

// Apply dequantisation
float float_val = (raw_int8_val - qp_zp) * qp_scale;
```

Raw output from the Hailo device is INT8/UINT8. Post-processing (NMS, decode) can be compiled into the HEF to operate on dequantised values internally (supported architectures only).

### ONNX Runtime QDQ Behaviour

- `QuantizeLinear` / `DequantizeLinear` ops are fused by the runtime with adjacent compute ops where possible (kernel fusion).
- For INT4 weight-only: weights read from memory as 4-bit, dequantised to FP16 in-register before matrix multiply — a **compute-time** dequant, not a graph-level op.
- NPUs often do not support arbitrary quantisation granularity; weights must be dequantised to a supported format (e.g. channel-wise INT8) before dispatch.

---

## 6.3 Model-Architecture Decoding

### Yolo8-Pose Output Tensors

The Hailo Model Zoo YOLOv8-pose model produces 9 output tensors arranged in three resolution heads (P3=80, P4=40, P5=20 grid cells). For each head:

| Tensor shape | Content |
| :--- | :--- |
| `H x W x 64` | Bounding box regression (DFL format) |
| `H x W x 1` | Bounding box confidence score |
| `H x W x 51` | 17 keypoints x 3 values (x, y, visibility) — COCO format |

Post-processing steps:
1. **Dequantisation** — INT8 → FP32 using per-tensor scale/zero-point
2. **DFL bounding box decode** — `dist2bbox` conversion using softmax + projection range
3. **Anchor/grid decode** — multiply by strides (8, 16, 32) to recover image-space coordinates
4. **Keypoint decode** — offsets relative to anchor centres, scaled back to image coordinates
5. **Confidence filtering** — remove detections below score threshold
6. **NMS** — filter overlapping detections

**Model trained on:** `coco-pose.yaml` ([source](https://github.com/ultralytics/ultralytics/blob/main/ultralytics/cfg/datasets/coco-pose.yaml)). 17 COCO keypoints x 3 = 51 values.

**Decoding reference:** [yolov8pose_postprocess.cpp](https://github.com/hailo-ai/Hailo-Application-Code-Examples/blob/55c64bf291053cf1038cd28757cbb95cb98e6876/runtime/hailo-8/cpp/pose_estimation/yolov8_pose/yolov8pose_postprocess.cpp#L319)

### Yolo5-Object Detection

Output format is 80xN: 80 classes. Each detection vector contains: class ID, number of objects, coordinates, size, confidence.

### Pose Estimation Models Reference

Full implementation details: [pose_estimation_models_detailed.xlsx](https://docs.google.com/spreadsheets/d/1J45pZ4jxjasOX_7qpJLm7wz1VFZaD-Mg/edit?gid=741875983#gid=741875983)

| Model | Type | Keypoints | NN Output Format | Optimised Accelerators |
| :--- | :--- | :--- | :--- | :--- |
| OpenPose | Bottom-up | 18/25 body, 21 hand, 70 face | Heatmaps + Part Affinity Fields (PAFs) | NVIDIA GPU (CUDA), TensorRT, CPU |
| MoveNet Lightning | Top-down | 17 (COCO) | Direct coordinate regression (heatmap-free) | TFLite, Edge TPU (Coral), CPU, Mobile, ONNX |
| MoveNet Thunder | Top-down | 17 (COCO) | Direct coordinate regression (heatmap-free) | TFLite, Edge TPU, CPU, Mobile, ONNX, TensorRT |
| BlazePose (MediaPipe) | Top-down | 33 (body + face/hands) | Direct coordinate regression (heatmap-free) | TFLite, GPU delegate, NNAPI, CoreML, WebGL, WASM |
| RTMPose | Top-down | 17/26/133 (body/wholebody) | SimCC coordinate classification (softmax over x/y bins) | TensorRT, ORT, ncnn, OpenVINO, RKNN, CPU, GPU, Snapdragon, Jetson |
| YOLOv8 Pose | Single-stage | 17 (COCO) | Direct coordinate regression (heatmap-free) | TensorRT, ONNX, CoreML, OpenVINO, TFLite, ncnn |
| YOLOv11 Pose | Single-stage | 17 (COCO) | Direct coordinate regression (heatmap-free) | TensorRT, ONNX, CoreML, OpenVINO, TFLite, ncnn |
| HRNet | Top-down | 17 (COCO) | Heatmaps | CUDA, TensorRT, ONNX, CPU |
| Lite-HRNet | Top-down | 17 (COCO) | Heatmaps | CUDA, TensorRT, ONNX, CPU, Mobile |
| HigherHRNet | Bottom-up | 17 (COCO) | Multi-scale heatmaps + associative embeddings | CUDA, TensorRT |
| ViTPose | Top-down | 17/133 | Heatmaps (or SimCC with modified head) | CUDA, TensorRT, ONNX |
| DEKR | Bottom-up | 17 (COCO) | Heatmaps + offset regression | CUDA, TensorRT |
| PoseNet | Single/Multi | 17 (COCO) | Heatmaps + offset vectors | TF.js (WebGL/WASM), TFLite, CPU |
| AlphaPose (RMPE) | Top-down | 17/26/136 | Heatmaps | CUDA, TensorRT |
| SimpleBaseline | Top-down | 17 (COCO) | Heatmaps | CUDA, TensorRT, ONNX |
| RTMO | One-stage | 17 (COCO) | Direct regression | CUDA, TensorRT |
| TRTPose | Top-down | 18 (COCO) | Heatmaps + PAFs | TensorRT (native), Jetson (Xavier, Nano, Orin) |
| VideoPose3D | 2D->3D Lifting | 17 (3D) | Direct 3D coordinate regression | CUDA |
| MotionBERT | 2D->3D Lifting | 17 (3D) | Direct 3D coordinate regression | CUDA |
| DWPose | Top-down | 133 (wholebody) | SimCC coordinate classification | TensorRT, ONNX, CPU, GPU |

### Model-Architecture Decoding Reference

| Model | Architecture | Key decoding step |
| :--- | :--- | :--- |
| YOLOv5 ObjDet | Single tensor `[1, N_anchors, 5 + C]` — x, y, w, h, objectness, class scores | `score = objectness × max_class_score`; centre-format box |
| YOLOv8 ObjDet | Pre-decoded per-class grouped boxes with count prefixes | Count prefix per class; corner-format coordinates (ymin, xmin, ymax, xmax) |
| YOLOv8 Pose | Three tensors per scale head (DFL boxes, scores, keypoints) | Tensor routing by feature dimension; DFL softmax regression decode; 17-keypoint COCO layout |
| MoveNet | Direct coordinate regression (heatmap-free) | Single tensor; 17 × 3 (y, x, confidence) |
| OpenPose / HRNet | Heatmaps + Part Affinity Fields or heatmaps only | Argmax or Gaussian peak fitting on spatial heatmap grids |
| YOLO26 | 25 variants; single-stage; native NMS-free end-to-end head | End-to-end detection head; no post-hoc NMS required |

New model types added to inxware must document their expected tensor layout and routing logic in the model implementation header.

---

## 6.4 Data Filtering — NMS

Post-processing for YOLO pose models follows a well-defined set of standard operations. The algorithm is conceptually consistent across deployments; implementation details vary by platform.

### Standard NMS Algorithms

| Algorithm | Used For | Standard Implementations | Key Parameters |
| :--- | :--- | :--- | :--- |
| **Greedy NMS (Box)** | Bounding box deduplication | `torchvision.ops.nms`, `cv2.dnn.NMSBoxes`, `tf.image.non_max_suppression` | `iou_threshold`, `score_threshold` |
| **Soft-NMS** | Box deduplication (gentler suppression) | `torchvision.ops.batched_nms`, custom | `iou_threshold`, `sigma` (Gaussian), `score_threshold` |
| **Heatmap NMS (Max-Pool)** | Keypoint peak detection | Max-pool + comparison (PyTorch/NumPy) | `kernel_size` (typically 3, 5, or 7) |
| **OKS-NMS** | Pose-level deduplication | `pycocotools`, `xtcocotools` | `oks_threshold`, `score_threshold` |

**NMS-free model architectures:**
- `YOLOv10` — NMS-free by design (dual label assignment, one-to-one inference head)
- `RT-DETR / RT-DETRv2` (Baidu) — NMS-free (DETR lineage)
- `YOLO26` — NMS-free (native end-to-end architecture, released January 2026; 25 variants; year-based naming scheme)

**Note:** `YOLO11` uses traditional NMS post-processing and is **not** NMS-free, despite following YOLOv10 chronologically. YOLOv5, YOLOv8, and all versions other than YOLOv10, RT-DETR, and YOLO26 also require NMS.

**Platform notes:**
- For yolov8_pose on Hailo, post-processing is done on the CPU from raw output tensors, which can be computationally costly ([Hailo Community discussion](https://community.hailo.ai/t/running-yolo-pose-with-nms-postprocessing-on-hailo8/15800)).
- Different platforms optimise differently: SIMD on CPU, GPU kernels, NPU-specific implementations.

### Comparison of Post-Processing Across Platforms

| Aspect | Standard Ultralytics | Hailo TAPPAS | Edge TPU/TFLite | DeepX (DXNN) |
| :--- | :--- | :--- | :--- | :--- |
| **DFL Decoding** | In model | Often in post-process | Sometimes removed | Unknown (model-dependent) |
| **NMS Location** | Python/GPU | CPU (C++) | CPU | CPU (host) |
| **Keypoint Format** | 17x3 | 17x3 | 17x3 | Likely same |
| **Quantisation** | Optional | Required (INT8) | Required (INT8) | Required (IQ8) |
| **Post-Process Execution** | Python/CUDA | C++ (.so library) | Python/C++ | Python/C++ via DX-RT |
| **Model Format** | `.pt` / ONNX | `.hef` | `.tflite` | `.dxnn` |

---

## 6.5 C++ and Python APIs per Format

| Format | C++ API entry point | Python API entry point |
| :--- | :--- | :--- |
| **TFLite / LiteRT** | `tflite::Interpreter` (`tflite/interpreter.h`) | `tf.lite.Interpreter` |
| **TFLite Micro** | `tflite::MicroInterpreter` | Python wrapper via CPython extension (`tflite_micro`) |
| **ONNX Runtime** | `Ort::Session` (`onnxruntime_cxx_api.h`) | `onnxruntime.InferenceSession` |
| **TensorRT** | `nvinfer1::IRuntime::deserializeCudaEngine` | `trt.Runtime().deserialize_cuda_engine` |
| **ExecuTorch** | `executorch::runtime::Module` | Python build pipeline only; runtime is C++ |
| **LibTorch (TorchScript)** | `torch::jit::load` (`torch/script.h`) | `torch.jit.load` |
| **HailoRT** | `hailort::Device`, `InferModel` (C++ API) | `hailo_platform` Python package |
| **RKNN Runtime** | `rknn_init`, `rknn_run`, `rknn_outputs_get` | `rknn_toolkit_lite` |
| **QNN / SNPE** | `zdl::SNPE::SNPEFactory`, QNN context APIs | `snpe-net-run`, `qnn-net-run` CLI |
| **Edge Impulse EIM** | IPC via Unix socket / stdio | `edge_impulse_linux` Python runner |
| **Edge Impulse C++ lib** | `ei::run_classifier()` | N/A (C++ only) |
| **DeepX DX-RT** | `dx_rt_*` C functions | Python bindings via DXNN SDK |
| **Kneron PLUS** | `kp_*` functions (`kdp_host.h`) | Kneron PLUS Python API |

---

## 6.6 Output Formats

### JSON

- **Flat JSON:** `{"label": "cat", "confidence": 0.97, "bbox": [x, y, w, h]}` — human readable, largest payload.
- **Structured JSON:** Nested objects per detection; widely used in REST APIs and Python pipelines.
- **Serialisation latency:** ~7,045 ns/op (vs ~1,827 ns Protobuf, ~711 ns FlatBuffers).
- **Use cases:** REST inference APIs, logging, debugging, web service interop.

### Protocol Buffers (proto3)

Binary, schema-enforced serialisation. ~6x faster than JSON; ~3x smaller payloads.

```proto
message Detection {
  string label = 1;
  float confidence = 2;
  BoundingBox bbox = 3;
}
```

Used natively in **TensorFlow Serving** (SavedModel prediction requests), **gRPC** inference APIs (KServe, Triton v2 protocol). Forward/backward compatible via field numbering.

### FlatBuffers / FlexBuffers

Zero-copy: data accessed directly from buffer without a deserialisation step — ~2.5x faster than Protobuf for read-heavy use cases.

Used natively by: **TFLite** (model format), **ExecuTorch** (`.pte` format), **ONNX** (alternative serialisation).

| Feature | Protobuf | Cap'n Proto | SBE | FlatBuffers |
| :---: | :---: | :---: | :---: | :---: |
| Schema evolution | yes | yes | caveats | yes |
| Zero-copy | no | yes | yes | yes |
| Random-access reads | no | yes | no | yes |
| Safe against malicious input | yes | yes | yes | opt-in upfront |
| Reflection / generic algorithms | yes | yes | yes | yes |
| Unknown field retention | removed in proto3 | yes | no | no |
| Object-capability RPC | no | yes | no | no |
| Usable as mutable state | yes | no | no | no |
| Padding takes space on wire? | no | optional | yes | yes |
| Unset fields take space on wire? | no | yes | yes | no |
| C++ | yes | yes | yes | yes |
| Java | yes | yes | yes | yes |
| Go | yes | yes | no | yes |
| Authors' preferred use case | distributed computing | platforms/sandboxing | financial trading | games |

**FlexBuffers:** Dynamically-typed variant of FlatBuffers; no schema required; suited for variable model parameters (e.g. SVM kernel parameters `K`, `SV`, `numsvs`) that change frequently.

**Serialisation latency comparison:**

| Format | Serialise + Deserialise (ns) |
| :--- | :--- |
| FlatBuffers | ~711 |
| Protocol Buffers | ~1,827 |
| JSON | ~7,045 |

**FlatBuffers build:**

```sh
git clone https://github.com/google/flatbuffers.git
cd flatbuffers && mkdir build && cd build && cmake .. && make
```

**Schema example (`.fbs`):**

```
namespace Hparams;
table TrainingParams {
  eta: double;
  rho: double;
  epsilon: double;
}
table Hyperparams {
  xdim: int;
  epochs: int;
  kernel: string;
  kernparm: double;
  training: TrainingParams;
}
root_type Hyperparams;
```

Compile with `flatc --cpp hyperparameters.fbs` to auto-generate `hyperparameters_generated.h`.

### ROS2 — `vision_msgs`

The `vision_msgs` package provides algorithm-agnostic computer vision messages. Pipeline should emit `*Array` variants as the forward-facing interface.

| Message | Contents |
| :--- | :--- |
| `Detection2D` | `Header`, `ObjectHypothesisWithPose[] results`, `BoundingBox2D bbox` |
| `Detection3D` | `Header`, `ObjectHypothesisWithPose[] results`, `BoundingBox3D bbox` |
| `Detection2DArray` / `Detection3DArray` | Array wrappers — preferred pipeline output |
| `ObjectHypothesis` | `string class_id`, `float64 score` |
| `BoundingBox2D` | `Pose2D center`, `float64 size_x`, `float64 size_y` |
| `VisionInfo` | Model metadata, database location |

**Pose:** `geometry_msgs/PoseWithCovariance` embedded in `ObjectHypothesisWithPose` for 6-DOF pose results.

**Input streams:** `sensor_msgs/Image`, `sensor_msgs/CameraInfo`, `sensor_msgs/PointCloud2` (upstream from inference).

**References:** [vision_msgs GitHub](https://github.com/ros-perception/vision_msgs)

### Apache Arrow

Columnar in-memory format with IPC protocol. Designed to eliminate (de)serialisation overhead via shared memory / memory-mapping. Best for batch inference pipelines where large tensors or frame metadata tables are passed between processes without copying. Not suited to low-latency single-result dispatch.

**Languages:** C++, Python (`pyarrow`), Rust, Java, Go.

### MessagePack

Binary JSON superset; schema-less. ~2x smaller than JSON; lower serialisation cost than Protobuf for dynamic scenarios. Suited for structured inference results over constrained channels (MQTT, serial, UDP). Libraries: `msgpack-c`, `msgpack` (Python), `rmp` (Rust). Lacks backward compatibility guarantees without custom versioning.

---

# 7. LLM at the Edge

Large language models can be run locally on edge hardware using quantised weight formats and CPU/NPU-optimised runtimes.

**llama.cpp** — C++ LLaMA re-write with no dependencies; CPU-only by default but also supports GPUs and NPUs. Uses **GGUF** file format.

- Comprehensive guide: [https://blog.steelph0enix.dev/posts/llama-cpp-guide/](https://blog.steelph0enix.dev/posts/llama-cpp-guide/)

**GGUF format:** Successor to GGML. A self-contained binary format storing model weights + metadata + tokeniser configuration. Supports variable quantisation per tensor layer (Q4_K, Q8_0, F16, etc.).

![GGUF LLM format diagram](illustrations/edge-ml-gguf-llm-format-diagram.png)

**LLM runners / servers:**

| Runner | API | Notes |
| :--- | :--- | :--- |
| **Ollama** | OpenAI-compatible HTTP API | Wraps llama.cpp; supports most GGUF models; easy model management |
| **vLLM** | OpenAI-compatible | HuggingFace suggestion; GPU-focused; paged KV-cache |
| **LiteLLM** | Unified proxy | Routes requests to different LLM APIs via single interface |
| **OpenRouter** | API proxy | Cloud router to multiple LLM providers |

**Models:** Gemma (Google), Llama 3.x (Meta), Qwen (Alibaba), LocalLLaMA community models.

**ExecuTorch LLM:** ExecuTorch 1.1.0 (January 2026) provides dedicated LLM inference APIs in C++, Java, Objective-C, Swift. KleidiAI INT4 micro-kernels via XNNPACK for ARM acceleration.

---

# 8. inxware Implementation — Status, APIs and Source Roadmap

This section gives a map of where the inxware ML implementation lives in the repository, summarises implementation status, documents the public C API, and describes the utility scripts. The authoritative narrative for the HAL structure is `target/Component-HAL/ml/README.md` — this section cross-references it without duplicating it.

---

## 8.1 Source Tree Overview

```
ert-components/
├── Common/HAL/include/
│   └── hal_ml.h                        ← Public C API: EhsML_* functions, types, enums
│
├── target/Component-HAL/ml/
│   ├── README.md                        ← Authoritative HAL structure reference
│   ├── ml_common.mk                     ← Top-level ML build entry point (EHS_ML_SUPPORT switch)
│   ├── ml_common.h / ml_common.c        ← Highest-level abstraction; called from function block
│   │
│   ├── engine/                          ← Inference engine (backend) implementations
│   │   ├── ml_engine.mk                 ← Engine selection logic
│   │   ├── tensorflow-lite/             ← TFLite C API wrapper          [implemented]
│   │   │   ├── ert_hal_tflite.h/.c
│   │   │   └── ml_ie_tflite.mk
│   │   ├── tensorflow-lite-micro/       ← TFLite Micro wrapper          [TODO]
│   │   │   ├── ert_hal_tflite_micro.h/.c
│   │   │   └── ml_ie_tflite_micro.mk
│   │   ├── hailo/                       ← HailoRT C++ wrapper           [implemented]
│   │   │   ├── ert_hal_hailo.h/.c
│   │   │   ├── hailo_thread.h
│   │   │   └── ml_ie_hailo.mk
│   │   └── tensorrt/                    ← TensorRT (NVIDIA Jetson)      [implemented]
│   │       ├── ert_hal_tensorrt.cpp / .h
│   │
│   ├── model/                           ← Post-processor per model type
│   │   ├── ml_models.h                  ← Conditional header aggregator
│   │   ├── ml_model.mk                  ← Model build configuration
│   │   ├── ml_model_common.h/.c         ← Boilerplate: create/destroy/setinput/run
│   │   ├── ml_model_template.{h,c}.template  ← Starting point for new models
│   │   ├── yolov5_objdet.h/.c           ← YOLOv5 object detection       [implemented]
│   │   ├── yolov8_objdet.h/.c           ← YOLOv8 object detection       [implemented]
│   │   ├── yolov8_pose.h/.c             ← YOLOv8 pose estimation        [implemented (Hailo and TFLite paths)]
│   │   └── ml_utils/
│   │       ├── ehs_ml_nms.h/.c          ← NMS implementation            [implemented]
│   │       └── ehs_ml_utils.h/.c        ← General ML utilities          [implemented]
│   │
│   ├── hailo/
│   │   └── hailo_ml.c                   ← Hailo ML session management   [implemented]
│   │
│   └── stubbed/                         ← No-op stubs for ML-less builds
│       ├── stubbed_ml.h/.c
│       └── ml_stubbed.mk
│
└── scripts/ai-utilities/
    ├── yolo-model-utils/                ← YOLO SavedModel → TFLite export scripts
    ├── hailo-utils/                     ← TFLite → HEF compilation script
    └── apriltag-model-trainer/          ← AprilTag dataset generator + training guide
```

---

## 8.2 HAL API (`Common/HAL/include/hal_ml.h`)

The public C API uses the `EhsML_` prefix throughout. All functions are callable from C and C++.

### Core lifecycle

```c
// Initialise a context for the given model type and backend.
EhsML_Err EhsML_Create(EhsML_Context* ctx,
                        const ehs_char* model_path,
                        EhsML_Type      model_type,   // e.g. EHS_ML_YOLOV8_OBJ_DETECTOR
                        ehs_float       conf_thres,   // 0.0–1.0
                        ehs_sint32      thread_count);

// Release all resources.
void EhsML_Destroy(EhsML_Context* ctx);
```

### Inference

```c
// Feed raw input data (image bytes, feature vector, etc.).
// Returns EHS_ML_BUSY if a pipeline is already executing.
EhsML_Err EhsML_SetInputData(EhsML_Context* ctx,
                              const void*   data,
                              ehs_uint32    size);

// Run pipeline only: engine inference + model decode → ctx->detections[].
// Returns EHS_ML_BUSY if already inferring.
EhsML_Err EhsML_Run(EhsML_Context* ctx);

// Serialise results to JSON: reads ctx->detections[] → json buffer.
// Clears the busy flag on completion.
// Structured: {"type":1000,"res":[{"cls":0,"cnf":0.92,"x":...},...]}
// Flat (ctx->enable_flat_json=true): {"type":0,...,"cls0":0,"cnf0":0.92,...}
EhsML_Err EhsML_GetOutput(EhsML_Context* ctx,
                           ehs_char*     json,
                           ehs_uint32    size);

// Convenience wrapper: EhsML_Run() then EhsML_GetOutput().
EhsML_Err EhsML_RunAndGetOutput(EhsML_Context* ctx,
                                 ehs_char*      json,
                                 ehs_uint32     size);

// @deprecated Use EhsML_RunAndGetOutput()
EhsML_Err EhsML_RunOutputJson(EhsML_Context* ctx,
                               ehs_char*     json,
                               ehs_uint32    size);

// Query which hardware accelerator is active on this build.
EhsML_HWAccel_t EhsML_HWAccel_supported(void);
```

**Note:** The busy guard (`ctx->inferring`) prevents concurrent inference — `EhsML_Run`, `EhsML_GetOutput`, and `EhsML_SetInputData` all return `EHS_ML_BUSY` (error code 26) immediately if a pipeline is already executing.

### Tensor helpers

```c
EhsML_Err EhsML_Tensor_Alloc  (EhsML_Tensor_t* t, EhsML_DataType_t dtype,
                                const ehs_uint32* dims, ehs_uint32 num_dims);
void      EhsML_Tensor_Free   (EhsML_Tensor_t* t);
EhsML_Err EhsML_Tensor_FillRaw(EhsML_Tensor_t* t, ehs_char* value, size_t size);
```

### Key types

| Type | Description |
| :--- | :--- |
| `EhsML_Context` | Per-session context: framework handle, input/output tensor arrays (up to `EHS_ML_LAYER_TENSORS_MAX` each), `EhsML_Type`, `EhsML_HWAccel_t`, `conf_thres`, `enable_flat_json` |
| `EhsML_Type` | Enum selecting model variant (see §8.4). Range `1000–1999` = image, `2000–2999` = text, `3000–3999` = audio |
| `EhsML_HWAccel_t` | `NONE`, `HAILO`, `NVIDIA`, `AMD`, `EIQ`, `DEEPX`, `GEMMA`, `CUSTOM_NPU` |
| `EhsML_Err` | `EHS_ML_OK`, `EHS_ML_FAILED`, `EHS_ML_INIT_ERR`, `EHS_ML_MODEL_LOAD_ERR`, `EHS_ML_NOT_IMPLEMENTED`, `EHS_ML_JSON_STRSIZE_ERR`, … |
| `EhsML_Tensor_t` | Data pointer union (u8/s8/f16/f32/…), `dims[4]`, `num_dims`, `data_type`, `quantisation_params` (scale, offset) |

---

## 8.3 Build Configuration

Set these variables in your platform `config.mk` or on the `make` command line.

### Top-level switch

| Variable | Values | Effect |
| :--- | :--- | :--- |
| `EHS_ML_SUPPORT` | `yes` | Enable ML with framework + model selection |
| | `stubbed` | Include stub symbols only — links without an ML backend |
| | `none` / unset | Exclude ML entirely |

### Framework selection

| Variable | Values | Notes |
| :--- | :--- | :--- |
| `EHS_ML_IE_IMAGE_SUPPORT` | `tensorflow-lite` | TFLite C API wrapper (implemented) |
| | `tensorflow-lite-micro` | TFLite Micro (TODO — emits build error if set) |
| `EHS_ML_HARDWARE_ACCELERATION` | `hailo` | Adds HailoRT backend alongside TFLite; sets `EHS_ML_HWACCEL_SUPPORT_HAILO` |
| | `nvidia` | TensorRT (CUDA, Jetson GPU/DLA); sets `EHS_ML_HWACCEL_SUPPORT_NVIDIA` |

**Note:** Only a single framework and single hardware accelerator are selected per build currently.

### Model selection

Enable models by adding `DEFS += EHS_ML_MODEL_SUPPORT_<NAME>` in `config.mk`:

| Macro | Status |
| :--- | :--- |
| `EHS_ML_MODEL_SUPPORT_YOLOV5_OBJDET` | **Implemented** |
| `EHS_ML_MODEL_SUPPORT_YOLOV8_OBJDET` | **Implemented** |
| `EHS_ML_MODEL_SUPPORT_YOLOV8_POSE` | Source present; post-processor in progress |
| `EHS_ML_MODEL_SUPPORT_YOLOV8_INSTSEG` | Not implemented |
| `EHS_ML_MODEL_SUPPORT_YOLOV9_*` | Not implemented |
| `EHS_ML_MODEL_SUPPORT_YOLOV10_*` | Not implemented |
| `EHS_ML_MODEL_SUPPORT_YOLOV11_*` | Not implemented |
| `EHS_ML_MODEL_SUPPORT_YOLOV12_*` | Not implemented |
| `EHS_ML_MODEL_SUPPORT_YOLOV26_*` | Not implemented |
| `EHS_ML_MODEL_SUPPORT_SAM_IMGSEG` | Not implemented |
| Text / Audio model types | Enumerated in `hal_ml.h`; no implementations yet |

### Tunable limits

Override in `config.mk` with `DEFS += <MACRO>=<value>`:

| Macro | Default | Purpose |
| :--- | :--- | :--- |
| `EHS_ML_OBJ_DETECTIONS_MAX` | 20 | Max objects returned per inference call |
| `EHS_ML_TENSOR_MAX_DIMS` | 4 | Max tensor rank |
| `EHS_ML_LAYER_TENSORS_MAX` | 128 | Max input or output tensors per model |

---

## 8.4 Utility Scripts (`scripts/ai-utilities/`)

### YOLO Model Export (`yolo-model-utils/`)

Three scripts convert a YOLO `best_saved_model/` directory (produced by Ultralytics training) to TFLite:

| Script | Output filename | Quantisation |
| :--- | :--- | :--- |
| `export_tflite.py` | `best.tflite` | Default optimisations (dynamic range) |
| `export_tflite_float16.py` | `best_float16.tflite` | FP16 weights |
| `export_tflite_int8.py` | `best_int8.tflite` | Full INT8 static — U8 input/output |

```sh
python3 export_tflite.py        <weights_dir>
python3 export_tflite_float16.py <weights_dir>
python3 export_tflite_int8.py   <weights_dir>
```

`<weights_dir>` is the Ultralytics run output directory containing `best_saved_model/`.

> **Note:** `export_tflite_int8.py` currently uses synthetic random calibration data. For production use replace `representative_data_gen()` with real representative input frames to improve quantisation accuracy.

### Hailo Compilation (`hailo-utils/`)

`tflite2hef.py` wraps the Hailo Dataflow Compiler (DFC) SDK to compile a `.tflite` model to a device-specific `.hef` binary.

**Setup:**
```sh
python3 -m venv venv && source venv/bin/activate
# Download the DFC wheel from https://hailo.ai/developer-zone/software-downloads/
pip install ./<hailo_dataflow_compiler_vX.Y.Z>.whl
pip install click
```

**Usage:**
```sh
python3 tflite2hef.py --hw_arch hailo8 -o model.hef <model_name> model.tflite
# --hw_arch choices: hailo8 (default), hailo8l, hailo8r
```

The script parses the TFLite model into HAR via `ClientRunner.translate_tf_model()`, then calls `runner.compile()` to produce the HEF binary. The optimisation pass between parse and compile is currently a no-op placeholder — add Hailo optimisation steps there as needed.

### AprilTag Model Trainer (`apriltag-model-trainer/`)

End-to-end pipeline for training a custom YOLOv5 object detection model on AprilTag fiducials, suited to robotic and industrial vision applications.

**Stages:**
1. Record or supply a 1:1-aspect-ratio video containing AprilTags.
2. Run `generate_dataset.sh <video> <N>` to extract labelled frames and produce a YOLO-format dataset (`dataset/xN/`). Use a stride-aligned N (multiples of 32); N=192 gives ~16–20 ms inference on RPi4.
3. Clone Ultralytics YOLOv5 and train with the generated `data.yaml`.
4. Export the resulting `best_saved_model/` using the YOLO model-utils scripts above.

See `apriltag-model-trainer/README.md` and `AprilTag_training.ipynb` for the full walkthrough.

---

## 8.5 Implementation Status Summary

| Component | Path | Status |
| :--- | :--- | :--- |
| Public C API | `Common/HAL/include/hal_ml.h` | Complete |
| ML common layer | `target/Component-HAL/ml/ml_common.*` | Implemented |
| TFLite C API backend | `…/engine/tensorflow-lite/` | Implemented (C API + XNNPACK delegate) |
| TFLite Micro backend | `…/engine/tensorflow-lite-micro/` | TODO |
| Hailo HRT backend | `…/engine/hailo/` | Implemented |
| TensorRT backend | `…/engine/tensorrt/` | Implemented (NMS plugin decoder) |
| Hailo session management | `…/hailo/hailo_ml.c` | Implemented |
| YOLOv5 object detection | `…/model/yolov5_objdet.*` | Implemented |
| YOLOv8 object detection | `…/model/yolov8_objdet.*` | Implemented |
| YOLOv8 pose estimation | `…/model/yolov8_pose.*` | Implemented (Hailo and TFLite paths) |
| NMS utilities | `…/model/ml_utils/ehs_ml_nms.*` | Implemented |
| ML utilities | `…/model/ml_utils/ehs_ml_utils.*` | Implemented |
| Stubbed (no-op) build | `…/stubbed/` | Implemented |
| YOLO → TFLite export | `scripts/ai-utilities/yolo-model-utils/` | Implemented (FP32, FP16, INT8) |
| TFLite → HEF compiler | `scripts/ai-utilities/hailo-utils/` | Implemented |
| AprilTag trainer pipeline | `scripts/ai-utilities/apriltag-model-trainer/` | Implemented |
| YOLO9–YOLO11 model types | `hal_ml.h` (enumerated only) | Not implemented |
| YOLO26 model type | `hal_ml.h` (enumerated only) | Not implemented |
| Text / Audio model types | `hal_ml.h` (enumerated only) | Not implemented |
| Raw data output path | `hal_ml.h` (TODO comment) | Not implemented (JSON only) |

---

# 9. Appendix: Runtime Dependencies Reference

| Library | Memory (RAM) | OS Support | Language | Notes |
| :--- | :--- | :--- | :--- | :--- |
| **TFLite / LiteRT** | ~50 KB – 2 MB | Android, iOS, Linux | C++, Python | Optimised for mobile/embedded; integrates with Google Cloud; can run TF, PyTorch (via ai-edge-torch), JAX models |
| **TFLite Micro (TFLM)** | ~10 KB – 256 KB | Bare metal, FreeRTOS | C++ | Microcontroller-first; static tensor arena; no heap allocator |
| **ExecuTorch** | ~100 KB core | Linux, Android, iOS, bare metal | C++, Java, ObjC/Swift | Minimal PyTorch edge runtime; XNNPACK default; Arm/Qualcomm backends |
| **ONNX Runtime** | ~2–20 MB | Linux, Windows, Android, iOS | C++, Python, C#, Java | Execution Provider system; QDQ quantisation; broad EP support |
| **TensorRT** | depends on model | Linux (NVIDIA GPU required) | C++, Python | Device-specific engine files; highest GPU inference performance |
| **HailoRT** | device-side | Linux, Windows | C++, Python | Hailo NPU runtime; HEF format; INT8 dequantisation API |
| **Edge Impulse C++ lib** | 30 KB – 1 MB | FreeRTOS, Zephyr, RTEMS, Linux | C++, Python (EIM) | Complete DSP + ML ecosystem; OTA updates; EON compiler |
| **CMSIS-NN** | ~10 KB – 1 MB | Bare metal, RTOS | C/C++ | Arm Cortex-M optimised kernels; fixed-model inference only |
| **KleidiAI** | minimal | Linux (Cortex-A) | C++ | Arm INT4/INT8 micro-kernels; integrated via XNNPACK in ExecuTorch |
| **ArduinoTensorFlowLite** | ~256 KB | Arduino, FreeRTOS | C++ | TFLite on Arduino ecosystem |
| **MicroPython** | ~256 KB | Bare metal, FreeRTOS | Python | General-purpose embedded Python; not primarily an ML runtime |
| **FEDML** | large | Linux, cloud | Python | Distributed/federated learning; not edge-embedded |
| **PyTorch LibTorch** | ~500 MB+ | Linux, Windows, macOS | C++ | Full C++ PyTorch; server-side only; not suited for edge |
| **NVIDIA TAO** | GPU-side | Linux (NVIDIA GPU) | Python | Transfer-learning toolkit; training/fine-tuning on edge GPU models |
