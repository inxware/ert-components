# Model Development Frameworks

This directory holds code that is specific to the **model development
framework** — the toolchain or file format used to train and export a model —
but is independent of the inference engine runtime.

## Distinction from `engine/`

| Concern | Location |
|---|---|
| Running inference (TFLite runtime, HailoRT SDK, TensorRT) | `engine/` |
| Parsing or introspecting the model *file format* at runtime | `frameworks/<framework>/` |

## What belongs here (examples)

- **ONNX**: utilities for reading ONNX model metadata without a full runtime
  (e.g. inspecting input/output tensor shapes from `.onnx` files)
- **Edge Impulse**: descriptor parsers for the Edge Impulse model archive format
- **TFLite FlatBuffer**: schema-level utilities beyond what the TFLite C API
  exposes (e.g. reading custom metadata blobs). Note: most TFLite format work
  lives in `engine/tflite/ert_hal_tflite_meta.c` because the format and
  runtime are tightly coupled for TFLite.

## Current state

No framework-specific code is implemented yet.  This directory exists as a
structural placeholder.  Add a subdirectory per framework as needed:

```
frameworks/
├── onnx/          ← ONNX metadata utilities (future)
├── edge-impulse/  ← Edge Impulse format support (future)
└── README.md      ← this file
```
