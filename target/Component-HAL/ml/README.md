# Machine Learning Implementation
This directory contains specific implementations of machine learning frameworks and models.

# Structure
## Terminology

- Framework    : This is the Inference Engine, which might be hardware accelerated a library or a compiledin executable model.
- Model        : defines the model  type, infers the input data format, inference Post Processor.
- Optimisation : (Not implemented yet) this may be code the transforms data types (float>int) and should support scaling factors for a particular model optimisation 

# TODO
The stubbed code should jsut be another framework that does nothing with inouts, models and inference. Remove the weird build options  

## Overview

```
-- ml
  |- framework
    |- tensorflow-lite
      |- ml_fw_tflite.mk
      |- xxx.h
      |- xxx.c
    |- hailo
      |- ml_fw_hailo.mk
      |- xxx.h
      |- xxx.c
    ...
  |- model
    |- ml_models.h
    |- ml_model_template.c.template
    |- ml_model_template.h.template
    |- ml_model.mk
    |- yolov5_objdet.c
    |- yolov5_objdet.h
    |- yolov11_instseg.c
    |- yolov11_instseg.h
    ...
  |- stubbed # TODO THIS SHOULD JUST BE A NEW FRAMEWORK.
    |- ml.mk
    |- stubbed_ml.c
  |- ml_common.mk
  |- ml_common.h
  |- ml_common.c
  |- README.md
```

## Framework
This is the backend where the model data is processed. A few examples are Tensorflow Lite and Hailo.
The `framework` folder contains the code where each framework is implemented in its own folder.

## Model
This defines how to parse the inference output while choosing appropriate framework according to parameter configurations.
New model implemtation code might be created from `ml_model_template.c.template` and `ml_model_template.h.template`.
Newly added source needs to be added to `ml_model.mk` with suitable build flags.
Newly added header needs to be included in `ml_models.h` with suitable build flags.
New type must be defined in `hal_ml.h` in the `Common/HAL/include` directory and used in `ml_common.c` switch statements.
The `model` folder contains all the model implementation in a flat structure.

## Stubbed
This contains universal code when Machine Learning feature is stubbed. It must run on any type of target and platform.

## ML Common
The `ml_common.*` contains the highest level of abstraction to be called from inference function block. It is to make sure that all the exported functions must be universal and not linked to a specific target.

## README
This file, which describes the structure of EHS Machine Learning infrastructure.

# Makefile Definition (to e reviewed)
## General
`EHS_ML_SUPPORT` states overall state of machine learning support. There are three values:
- `yes`
- `stubbed`
- `none` or undefined
#TODO propose this also chooses the framework instead of the thing below which seem to choose the data type, which is a model paramter not really a framework.
EHS_ML_SUPPORT=none|stubbed|tflite|hailo|...
 
`EHS_ML_LAYER_TENSORS_MAX` defines the maximum number of input or output tensors a model can have. If the actual number exceeds it, this parameter must increase or initialisation will throw init error. The default value is 128. In order to set this, add `DEFS += EHS_ML_LAYER_TENSORS_MAX=<number>` in platform makefile.

## Framework (??? not really??)
The following are the makefile variables representing three types of machine learning framework support. The items of sub-list are the available values for the support.
- `EHS_ML_FRAMEWORK_IMAGE_SUPPORT`
  - tensorflow-lite
  - tensorflow-lite-micro (TODO)
- `EHS_ML_FRAMEWORK_TEXT_SUPPORT`
- `EHS_ML_FRAMEWORK_AUDIO_SUPPORT`

Suggest we use another make config (that is probably set for a specific model that defines the inout data type. i.e.:
 `EHS_ML_MODEL_INPUT_DATA_TYPE=vector|image|text`

we also want an optimisation level paramter
`EHS_ML_MODEL_

### Hardware Acceleration
`EHS_ML_HARDWARE_ACCELERATION` defines the available hardware acceleration supported on the platform. This should be defined in the platform `config.mk` file as a single choice. It can be chosen from one of the choices in the following list:
- `hailo`

TODO: The following shouldn't be build time variables necessarilly (unless it is an MCU executable model) 
If we do need to know at build time  then this should just use a make variable EHS_ML_MODEL=EHS_ML_MODEL_SUPPORT_YOLOV3_OBJDET, which may set the following (or we define an enumeration)

## Model
The following are the list of makefile variables. The value is either `yes` or `no`:
- `EHS_ML_MODEL_SUPPORT_YOLOV3_OBJDET` (Not Implemented)
- `EHS_ML_MODEL_SUPPORT_YOLOV4_OBJDET` (Not Implemented)
- `EHS_ML_MODEL_SUPPORT_YOLOV5_OBJDET`
- `EHS_ML_MODEL_SUPPORT_YOLOV6_OBJDET` (Not Implemented)
- `EHS_ML_MODEL_SUPPORT_YOLOV7_OBJDET` (Not Implemented)
- `EHS_ML_MODEL_SUPPORT_YOLOV8_OBJDET`
- `EHS_ML_MODEL_SUPPORT_YOLOV8_INSTSEG` (Not Implemented)
- `EHS_ML_MODEL_SUPPORT_YOLOV8_POSE` (Not Implemented)
- `EHS_ML_MODEL_SUPPORT_YOLOV8_OOB` (Not Implemented)
- `EHS_ML_MODEL_SUPPORT_YOLOV8_CLASS` (Not Implemented)
- `EHS_ML_MODEL_SUPPORT_YOLOV9_OBJDET` (Not Implemented)
- `EHS_ML_MODEL_SUPPORT_YOLOV9_INSTSEG` (Not Implemented)
- `EHS_ML_MODEL_SUPPORT_YOLOV10_OBJDET` (Not Implemented)
- `EHS_ML_MODEL_SUPPORT_YOLOV11_OBJDET` (Not Implemented)
- `EHS_ML_MODEL_SUPPORT_YOLOV11_INSTSEG` (Not Implemented)
- `EHS_ML_MODEL_SUPPORT_YOLOV11_POSE` (Not Implemented)
- `EHS_ML_MODEL_SUPPORT_YOLOV11_OOB` (Not Implemented)
- `EHS_ML_MODEL_SUPPORT_YOLOV11_CLASS` (Not Implemented)
- `EHS_ML_MODEL_SUPPORT_YOLOV12_OBJDET` (Not Implemented)
- `EHS_ML_MODEL_SUPPORT_YOLOV12_INSTSEG` (Not Implemented)
- `EHS_ML_MODEL_SUPPORT_YOLOV12_POSE` (Not Implemented)
- `EHS_ML_MODEL_SUPPORT_YOLOV12_OOB` (Not Implemented)
- `EHS_ML_MODEL_SUPPORT_YOLOV12_CLASS` (Not Implemented)
- `EHS_ML_MODEL_SUPPORT_YOLOV26_OBJDET` (Not Implemented)
- `EHS_ML_MODEL_SUPPORT_YOLOV26_INSTSEG` (Not Implemented)
- `EHS_ML_MODEL_SUPPORT_YOLOV26_POSE` (Not Implemented)
- `EHS_ML_MODEL_SUPPORT_YOLOV26_OOB` (Not Implemented)
- `EHS_ML_MODEL_SUPPORT_YOLOV26_CLASS` (Not Implemented)
- `EHS_ML_MODEL_SUPPORT_YOLOV26_CLASS` (Not Implemented)
- `EHS_ML_MODEL_SUPPORT_SAM_IMGSEG` (Not Implemented)
