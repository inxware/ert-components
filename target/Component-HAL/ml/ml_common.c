/***************************************************************
* Copyright (C) 2008-2025 inx limited - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/** @file hal_common.c
 * Common code for Hailo and other ML backends.
 *
 * @author: inx limited
 */


/**
 * @page Verification Verification report
 * @section hal_graphics_font
 * @anchor hal_graphics_font
 * @subsection misra MISRA compliance:
 * test.c demonstrated MISRA compliant on
 * Last modified on $Date:$
 *
 * This file contained no derogations to the MISRA standard.
 */

/*****************************************************************************/
/* Included files */
#include "ml_common.h"
#include "hal_ml.h"
#include "hal-api.h"

/*****************************************************************************/
/* Declare macros and local typedefs used by this file */
#ifndef EHS_ML_HARDWARE_ACCELERATION
#define EHS_ML_HARDWARE_ACCELERATION EHS_ML_HWACCEL_NONE
#endif//EHS_ML_HARDWARE_ACCELERATION

/*****************************************************************************/
/* Declare prototypes of local functions */

/*****************************************************************************/
/* Variables defined with file-scope */

/*****************************************************************************/
/* Variables defined with global-scope */

/*****************************************************************************/
/* Function definitions */
EhsML_HWAccel_t EhsML_HWAccel_supported( void )
{
    EhsML_HWAccel_t ret = EHS_ML_HARDWARE_ACCELERATION;
    return ret;
}

EhsML_Err EhsML_Tensor_Alloc(EhsML_Tensor_t* tensor, EhsML_DataType_t data_type, const ehs_uint32* dims, ehs_uint32 num_dims)
{
    if (tensor == NULL) return EHS_ML_INIT_ERR;
    if (dims == NULL) return EHS_ML_INIT_ERR;
    // Tensor must NOT be allocated more than once
    if (tensor->data_ptr.ptr != NULL) return EHS_ML_MEMORY_ERR;
    if (data_type >= EHS_ML_DATATYPE_MAX || data_type < EHS_ML_DATATYPE_NONE) return EHS_ML_INVALID_QUANT_ERR;
    if (num_dims > EHS_ML_TENSOR_MAX_DIMS) return EHS_ML_INVALID_SIZE_ERR;

    size_t size = 1;
    int i = 0;
    for (i = 0 ; i < num_dims ; i++) size *= dims[i];
    switch (data_type)
    {
        case EHS_ML_DATATYPE_NONE:
        {
            size *= sizeof(ehs_char);
            tensor->data_ptr.raw = (ehs_char *) malloc(size);
            break;
        }
        case EHS_ML_DATATYPE_UINT8:
        {
            size *= sizeof(ehs_uint8);
            tensor->data_ptr.u8 = (ehs_uint8 *) malloc(size);
            break;
        }
        case EHS_ML_DATATYPE_INT8:
        {
            size *= sizeof(ehs_sint8);
            tensor->data_ptr.s8 = (ehs_sint8 *) malloc(size);
            break;
        }
        case EHS_ML_DATATYPE_UINT16:
        {
            size *= sizeof(ehs_uint16);
            tensor->data_ptr.u16 = (ehs_uint16 *) malloc(size);
            break;
        }
        case EHS_ML_DATATYPE_INT16:
        {
            size *= sizeof(ehs_sint16);
            tensor->data_ptr.s16 = (ehs_sint16 *) malloc(size);
            break;
        }
        case EHS_ML_DATATYPE_UINT32:
        {
            size *= sizeof(ehs_uint32);
            tensor->data_ptr.u32 = (ehs_uint32 *) malloc(size);
            break;
        }
        case EHS_ML_DATATYPE_INT32:
        {
            size *= sizeof(ehs_sint32);
            tensor->data_ptr.s32 = (ehs_sint32 *) malloc(size);
            break;
        }
        case EHS_ML_DATATYPE_UINT64:
        {
            size *= sizeof(ehs_uint64);
            tensor->data_ptr.u64 = (ehs_uint64 *) malloc(size);
            break;
        }
        case EHS_ML_DATATYPE_INT64:
        {
            size *= sizeof(ehs_sint64);
            tensor->data_ptr.s64 = (ehs_sint64 *) malloc(size);
            break;
        }
        case EHS_ML_DATATYPE_FP16:
        {
            size *= sizeof(EhsML_Float16_t);
            tensor->data_ptr.f16 = (EhsML_Float16_t *) malloc(size);
            break;
        }
        case EHS_ML_DATATYPE_FP32:
        {
            size *= sizeof(float);
            tensor->data_ptr.f32 = (float *) malloc(size);
            break;
        }
        case EHS_ML_DATATYPE_FP64:
        {
            size *= sizeof(double);
            tensor->data_ptr.f64 = (double *) malloc(size);
            break;
        }
        case EHS_ML_DATATYPE_COMPLEX64:
        {
            size *= sizeof(EhsML_Complex64_t);
            tensor->data_ptr.c64 = (EhsML_Complex64_t *) malloc(size);
            break;
        }
        case EHS_ML_DATATYPE_COMPLEX128:
        {
            size *= sizeof(EhsML_Complex128_t);
            tensor->data_ptr.c128 = (EhsML_Complex128_t *) malloc(size);
            break;
        }
        case EHS_ML_DATATYPE_BOOL:
        {
            size *= sizeof(ehs_bool);
            tensor->data_ptr.b = (ehs_bool *) malloc(size);
            break;
        }
        case EHS_ML_DATATYPE_RAW:
        {
            size *= sizeof(ehs_char);
            tensor->data_ptr.raw = (ehs_char *) malloc(size);
            break;
        }
        case EHS_ML_DATATYPE_CONST_RAW:
        {
            size *= sizeof(ehs_char);
            tensor->data_ptr.const_raw = (const ehs_char *) malloc(size);
            break;
        }
        default:
            return EHS_ML_INVALID_QUANT_ERR;
    }
    if (tensor->data_ptr.raw == NULL) return EHS_ML_MEMORY_ERR;
    tensor->size_in_bytes = size;
    tensor->num_dims = num_dims;
    EhsMemcpy(tensor->dims, dims, num_dims * sizeof(ehs_uint32));

    return EHS_ML_OK;
}

void EhsML_Tensor_Free(EhsML_Tensor_t* tensor)
{
    if (tensor == NULL) return;
    if (tensor->data_ptr.ptr == NULL) return;

    // Free the allocated memory
    free(tensor->data_ptr.ptr);
    // Reset everything to 0
    tensor->data_ptr.ptr = NULL;
    tensor->size_in_bytes = 0;
    tensor->num_dims = 0;
    EhsMemset(tensor->dims, 0, EHS_ML_TENSOR_MAX_DIMS * sizeof(ehs_uint32));
}

EhsML_Err EhsML_Tensor_FillRaw(EhsML_Tensor_t* tensor, ehs_char *value, size_t size_in_bytes)
{
    if (tensor == NULL) return EHS_ML_INIT_ERR;
    if (tensor->data_ptr.ptr == NULL) return EHS_ML_INIT_ERR;

    //{
    //    size_t size = 1;
    //    int _i = 0;
    //    for (_i = 0 ; _i < tensor->num_dims ; _i++) size *= tensor->dims[_i];
    //    if (size != size_in_bytes) return EHS_ML_MEMORY_ERR;
    //}

    EhsMemcpy(tensor->data_ptr.raw, value, size_in_bytes * sizeof(ehs_char));

    return EHS_ML_OK;
}

EhsML_Err EhsML_Create(EhsML_Context* ctx, const ehs_char* model_path, EhsML_Type model_type, ehs_float conf_thres, ehs_sint32 thread_count)
{
    if (ctx == NULL) return EHS_ML_FAILED;
    if (model_type >= EHS_ML_TYPE_MAX) return EHS_ML_MODEL_TYPE_ERR;
    #ifdef EHS_ML_SUPPORT_STUBBED
    return EhsML_Stubbed_Create(ctx, model_path, model_type, conf_thres, thread_count);
    #else
    ctx->type = model_type;
    switch (model_type)
    {
        /* Stubbed */
        case EHS_ML_TYPE_STUBBED:
        {
            return EhsML_Stubbed_Create(ctx, model_path, model_type, conf_thres, thread_count);
        }
        /* Image */
        case EHS_ML_YOLOV3_OBJ_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV3_OBJDET
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov3_ObjDet_Create(ctx, model_path, model_type, conf_thres, thread_count);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV3_OBJDET
        }
        case EHS_ML_YOLOV4_OBJ_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV4_OBJDET
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov4_ObjDet_Create(ctx, model_path, model_type, conf_thres, thread_count);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV4_OBJDET
        }
        case EHS_ML_YOLOV5_OBJ_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV5_OBJDET
            return EhsML_Yolov5_ObjDet_Create(ctx, model_path, model_type, conf_thres, thread_count);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV5_OBJDET
        }
        case EHS_ML_YOLOV6_OBJ_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV6_OBJDET
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov6_ObjDet_Create(ctx, model_path, model_type, conf_thres, thread_count);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV6_OBJDET
        }
        case EHS_ML_YOLOV7_OBJ_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV7_OBJDET
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov7_ObjDet_Create(ctx, model_path, model_type, conf_thres, thread_count);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV7_OBJDET
        }
        case EHS_ML_YOLOV8_OBJ_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV8_OBJDET
            return EhsML_Yolov8_ObjDet_Create(ctx, model_path, model_type, conf_thres, thread_count);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV8_OBJDET
        }
        case EHS_ML_YOLOV8_INSTANCE_SEGMENTER:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV8_INSTSEG
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov8_InstSeg_Create(ctx, model_path, model_type, conf_thres, thread_count);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV8_INSTSEG
        }
        case EHS_ML_YOLOV8_POSE_ESTIMATOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV8_POSE
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov8_Pose_Create(ctx, model_path, model_type, conf_thres, thread_count);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV8_POSE
        }
        case EHS_ML_YOLOV8_ORIENTED_BBOX_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV8_OOB
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov8_OOB_Create(ctx, model_path, model_type, conf_thres, thread_count);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV8_OOB
        }
        case EHS_ML_YOLOV8_CLASSIFIER:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV8_CLASS
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov8_Class_Create(ctx, model_path, model_type, conf_thres, thread_count);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV8_CLASS
        }
        case EHS_ML_YOLOV9_OBJ_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV9_OBJDET
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov9_ObjDet_Create(ctx, model_path, model_type, conf_thres, thread_count);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV9_OBJDET
        }
        case EHS_ML_YOLOV9_INSTANCE_SEGMENTER:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV9_INSTSEG
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov9_InstSeg_Create(ctx, model_path, model_type, conf_thres, thread_count);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV9_INSTSEG
        }
        case EHS_ML_YOLOV10_OBJ_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV10_OBJDET
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov10_ObjDet_Create(ctx, model_path, model_type, conf_thres, thread_count);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV10_OBJDET
        }
        case EHS_ML_YOLOV11_OBJ_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV11_OBJDET
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov11_ObjDet_Create(ctx, model_path, model_type, conf_thres, thread_count);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV11_OBJDET
        }
        case EHS_ML_YOLOV11_INSTANCE_SEGMENTER:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV11_INSTSEG
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov11_InstSeg_Create(ctx, model_path, model_type, conf_thres, thread_count);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV11_INSTSEG
        }
        case EHS_ML_YOLOV11_POSE_ESTIMATOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV11_POSE
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov11_Pose_Create(ctx, model_path, model_type, conf_thres, thread_count);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV11_POSE
        }
        case EHS_ML_YOLOV11_ORIENTED_BBOX_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV11_OOB
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov11_OOB_Create(ctx, model_path, model_type, conf_thres, thread_count);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV11_OOB
        }
        case EHS_ML_YOLOV11_CLASSIFIER:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV11_CLASS
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov11_Class_Create(ctx, model_path, model_type, conf_thres, thread_count);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV11_CLASS
        }
        case EHS_ML_YOLOV12_OBJ_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV12_OBJDET
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov12_ObjDet_Create(ctx, model_path, model_type, conf_thres, thread_count);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV12_OBJDET
        }
        case EHS_ML_YOLOV12_INSTANCE_SEGMENTER:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV12_INSTSEG
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov12_InstSeg_Create(ctx, model_path, model_type, conf_thres, thread_count);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV12_INSTSEG
        }
        case EHS_ML_YOLOV12_POSE_ESTIMATOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV12_POSE
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov12_Pose_Create(ctx, model_path, model_type, conf_thres, thread_count);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV12_POSE
        }
        case EHS_ML_YOLOV12_ORIENTED_BBOX_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV12_OOB
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov12_OOB_Create(ctx, model_path, model_type, conf_thres, thread_count);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV12_OOB
        }
        case EHS_ML_YOLOV12_CLASSIFIER:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV12_CLASS
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov12_Class_Create(ctx, model_path, model_type, conf_thres, thread_count);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV12_CLASS
        }
        case EHS_ML_YOLOV26_OBJ_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV26_OBJDET
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov26_ObjDet_Create(ctx, model_path, model_type, conf_thres, thread_count);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV26_OBJDET
        }
        case EHS_ML_YOLOV26_INSTANCE_SEGMENTER:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV26_INSTSEG
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov26_InstSeg_Create(ctx, model_path, model_type, conf_thres, thread_count);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV26_INSTSEG
        }
        case EHS_ML_YOLOV26_POSE_ESTIMATOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV26_POSE
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov26_Pose_Create(ctx, model_path, model_type, conf_thres, thread_count);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV26_POSE
        }
        case EHS_ML_YOLOV26_ORIENTED_BBOX_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV26_OOB
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov26_OOB_Create(ctx, model_path, model_type, conf_thres, thread_count);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV26_OOB
        }
        case EHS_ML_YOLOV26_CLASSIFIER:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV26_CLASS
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov26_Class_Create(ctx, model_path, model_type, conf_thres, thread_count);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV26_CLASS
        }
        case EHS_ML_SAM_IMAGE_SEGMENTER:
        {
#ifdef EHS_ML_MODEL_SUPPORT_SAM_IMGSEG
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_SAM_ImgSeg_Create(ctx, model_path, model_type, conf_thres, thread_count);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_SAM_IMGSEG
        }
        /* Text */
        case EHS_ML_TRANSFORMER_TEXT_CLASSIFIER:
        {
#ifdef EHS_ML_MODEL_SUPPORT_TRANSFORMER_TEXT_CLASS
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Stubbed_Create(ctx, model_path, model_type, conf_thres, thread_count);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_TRANSFORMER_TEXT_CLASS
        }
        case EHS_ML_TRANSFORMER_SENTIMENT_ANALYZER:
        {
#ifdef EHS_ML_MODEL_SUPPORT_TRANSFORMER_SENTIMENT_ANALYZER
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Stubbed_Create(ctx, model_path, model_type, conf_thres, thread_count);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_TRANSFORMER_SENTIMENT_ANALYZER
        }
        case EHS_ML_TRANSFORMER_NAMED_ENTITY_RECOGNITION:
        {
#ifdef EHS_ML_MODEL_SUPPORT_TRANSFORMER_NAMED_ENTITY_RECOGNITION
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Stubbed_Create(ctx, model_path, model_type, conf_thres, thread_count);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_TRANSFORMER_NAMED_ENTITY_RECOGNITION
        }
        case EHS_ML_TRANSFORMER_TEXT_SUMMARIZER:
        {
#ifdef EHS_ML_MODEL_SUPPORT_TRANSFORMER_TEXT_SUMMARIZER
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Stubbed_Create(ctx, model_path, model_type, conf_thres, thread_count);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_TRANSFORMER_TEXT_SUMMARIZER
        }
        case EHS_ML_TRANSFORMER_LANGUAGE_TRANSLATOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_TRANSFORMER_LANGUAGE_TRANSLATOR
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Stubbed_Create(ctx, model_path, model_type, conf_thres, thread_count);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_TRANSFORMER_LANGUAGE_TRANSLATOR
        }
        /* Audio */
        case EHS_ML_AUDIO_SPEECH_RECOGNITION:
        {
#ifdef EHS_ML_MODEL_SUPPORT_AUDIO_SPEECH_RECOGNITION
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Stubbed_Create(ctx, model_path, model_type, conf_thres, thread_count);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_AUDIO_SPEECH_RECOGNITION
        }
        case EHS_ML_AUDIO_KEYWORD_SPOTTING:
        {
#ifdef EHS_ML_MODEL_SUPPORT_AUDIO_KEYWORD_SPOTTING
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Stubbed_Create(ctx, model_path, model_type, conf_thres, thread_count);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_AUDIO_KEYWORD_SPOTTING
        }
        case EHS_ML_AUDIO_SPEAKER_IDENTIFICATION:
        {
#ifdef EHS_ML_MODEL_SUPPORT_AUDIO_SPEAKER_IDENTIFICATION
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Stubbed_Create(ctx, model_path, model_type, conf_thres, thread_count);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_AUDIO_SPEAKER_IDENTIFICATION
        }
        case EHS_ML_AUDIO_NOISE_SUPPRESSION:
        {
#ifdef EHS_ML_MODEL_SUPPORT_AUDIO_NOISE_SUPPRESSION
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Stubbed_Create(ctx, model_path, model_type, conf_thres, thread_count);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_AUDIO_NOISE_SUPPRESSION
        }
        case EHS_ML_AUDIO_SOUND_CLASSIFICATION:
        {
#ifdef EHS_ML_MODEL_SUPPORT_AUDIO_SOUND_CLASSIFICATION
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Stubbed_Create(ctx, model_path, model_type, conf_thres, thread_count);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_AUDIO_SOUND_CLASSIFICATION
        }
        case EHS_ML_AUDIO_VOICE_ACTIVITY_DETECTION:
        {
#ifdef EHS_ML_MODEL_SUPPORT_AUDIO_VOICE_ACTIVITY_DETECTION
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Stubbed_Create(ctx, model_path, model_type, conf_thres, thread_count);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_AUDIO_VOICE_ACTIVITY_DETECTION
        }
        //// ...
        default:
            return EHS_ML_MODEL_TYPE_ERR;
    }
    #endif//EHS_ML_SUPPORT_STUBBED
}

void EhsML_Destroy(EhsML_Context* ctx)
{
    if (ctx == NULL) return;
    if (ctx->type >= EHS_ML_TYPE_MAX) return;
    #ifdef EHS_ML_SUPPORT_STUBBED
    EhsML_Stubbed_Destroy(ctx);
    #else
    switch (ctx->type)
    {
        /* Stubbed */
        case EHS_ML_TYPE_STUBBED:
        {
            EhsML_Stubbed_Destroy(ctx);
            break;
        }
        /* Image */
        case EHS_ML_YOLOV3_OBJ_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV3_OBJDET
            EhsML_Yolov3_ObjDet_Destroy(ctx);
#endif//EHS_ML_MODEL_SUPPORT_YOLOV3_OBJDET
            break;
        }
        case EHS_ML_YOLOV4_OBJ_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV4_OBJDET
            EhsML_Yolov4_ObjDet_Destroy(ctx);
#endif//EHS_ML_MODEL_SUPPORT_YOLOV4_OBJDET
            break;
        }
        case EHS_ML_YOLOV5_OBJ_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV5_OBJDET
            EhsML_Yolov5_ObjDet_Destroy(ctx);
#endif//EHS_ML_MODEL_SUPPORT_YOLOV5_OBJDET
            break;
        }
        case EHS_ML_YOLOV6_OBJ_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV6_OBJDET
            EhsML_Yolov6_ObjDet_Destroy(ctx);
#endif//EHS_ML_MODEL_SUPPORT_YOLOV6_OBJDET
            break;
        }
        case EHS_ML_YOLOV7_OBJ_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV7_OBJDET
            EhsML_Yolov7_ObjDet_Destroy(ctx);
#endif//EHS_ML_MODEL_SUPPORT_YOLOV7_OBJDET
            break;
        }
        case EHS_ML_YOLOV8_OBJ_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV8_OBJDET
            EhsML_Yolov8_ObjDet_Destroy(ctx);
#endif//EHS_ML_MODEL_SUPPORT_YOLOV8_OBJDET
            break;
        }
        case EHS_ML_YOLOV8_INSTANCE_SEGMENTER:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV8_INSTSEG
            EhsML_Yolov8_InstSeg_Destroy(ctx);
#endif//EHS_ML_MODEL_SUPPORT_YOLOV8_INSTSEG
            break;
        }
        case EHS_ML_YOLOV8_POSE_ESTIMATOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV8_POSE
            EhsML_Yolov8_Pose_Destroy(ctx);
#endif//EHS_ML_MODEL_SUPPORT_YOLOV8_POSE
            break;
        }
        case EHS_ML_YOLOV8_ORIENTED_BBOX_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV8_OOB
            EhsML_Yolov8_OOB_Destroy(ctx);
#endif//EHS_ML_MODEL_SUPPORT_YOLOV8_OOB
            break;
        }
        case EHS_ML_YOLOV8_CLASSIFIER:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV8_CLASS
            EhsML_Yolov8_Class_Destroy(ctx);
#endif//EHS_ML_MODEL_SUPPORT_YOLOV8_CLASS
            break;
        }
        case EHS_ML_YOLOV9_OBJ_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV9_OBJDET
            EhsML_Yolov9_ObjDet_Destroy(ctx);
#endif//EHS_ML_MODEL_SUPPORT_YOLOV9_OBJDET
            break;
        }
        case EHS_ML_YOLOV9_INSTANCE_SEGMENTER:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV9_INSTSEG
            EhsML_Yolov9_InstSeg_Destroy(ctx);
#endif//EHS_ML_MODEL_SUPPORT_YOLOV9_INSTSEG
            break;
        }
        case EHS_ML_YOLOV10_OBJ_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV10_OBJDET
            EhsML_Yolov10_ObjDet_Destroy(ctx);
#endif//EHS_ML_MODEL_SUPPORT_YOLOV10_OBJDET
            break;
        }
        case EHS_ML_YOLOV11_OBJ_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV11_OBJDET
            EhsML_Yolov11_ObjDet_Destroy(ctx);
#endif//EHS_ML_MODEL_SUPPORT_YOLOV11_OBJDET
            break;
        }
        case EHS_ML_YOLOV11_INSTANCE_SEGMENTER:
        {   
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV11_INSTSEG
            EhsML_Yolov11_InstSeg_Destroy(ctx);
#endif//EHS_ML_MODEL_SUPPORT_YOLOV11_INSTSEG
            break;
        }
        case EHS_ML_YOLOV11_POSE_ESTIMATOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV11_POSE
            EhsML_Yolov11_Pose_Destroy(ctx);
#endif//EHS_ML_MODEL_SUPPORT_YOLOV11_POSE
            break;
        }
        case EHS_ML_YOLOV11_ORIENTED_BBOX_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV11_OOB
            EhsML_Yolov11_OOB_Destroy(ctx);
#endif//EHS_ML_MODEL_SUPPORT_YOLOV11_OOB
            break;
        }
        case EHS_ML_YOLOV11_CLASSIFIER:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV11_CLASS
            EhsML_Yolov11_Class_Destroy(ctx);
#endif//EHS_ML_MODEL_SUPPORT_YOLOV11_CLASS
            break;
        }
        case EHS_ML_YOLOV12_OBJ_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV12_OBJDET
            EhsML_Yolov12_ObjDet_Destroy(ctx);
#endif//EHS_ML_MODEL_SUPPORT_YOLOV12_OBJDET
            break;
        }
        case EHS_ML_YOLOV12_INSTANCE_SEGMENTER:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV12_INSTSEG
            EhsML_Yolov12_InstSeg_Destroy(ctx);
#endif//EHS_ML_MODEL_SUPPORT_YOLOV12_INSTSEG
            break;
        }
        case EHS_ML_YOLOV12_POSE_ESTIMATOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV12_POSE
            EhsML_Yolov12_Pose_Destroy(ctx);
#endif//EHS_ML_MODEL_SUPPORT_YOLOV12_POSE
            break;
        }
        case EHS_ML_YOLOV12_ORIENTED_BBOX_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV12_OOB
            EhsML_Yolov12_OOB_Destroy(ctx);
#endif//EHS_ML_MODEL_SUPPORT_YOLOV12_OOB
            break;
        }
        case EHS_ML_YOLOV12_CLASSIFIER:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV12_CLASS
            EhsML_Yolov12_Class_Destroy(ctx);
#endif//EHS_ML_MODEL_SUPPORT_YOLOV12_CLASS
            break;
        }
        case EHS_ML_YOLOV26_OBJ_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV26_OBJDET
            EhsML_Yolov26_ObjDet_Destroy(ctx);
#endif//EHS_ML_MODEL_SUPPORT_YOLOV26_OBJDET
            break;
        }
        case EHS_ML_YOLOV26_INSTANCE_SEGMENTER:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV26_INSTSEG
            EhsML_Yolov26_InstSeg_Destroy(ctx);
#endif//EHS_ML_MODEL_SUPPORT_YOLOV26_INSTSEG
            break;
        }
        case EHS_ML_YOLOV26_POSE_ESTIMATOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV26_POSE
            EhsML_Yolov26_Pose_Destroy(ctx);
#endif//EHS_ML_MODEL_SUPPORT_YOLOV26_POSE
            break;
        }
        case EHS_ML_YOLOV26_ORIENTED_BBOX_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV26_OOB
            EhsML_Yolov26_OOB_Destroy(ctx);
#endif//EHS_ML_MODEL_SUPPORT_YOLOV26_OOB
            break;
        }
        case EHS_ML_YOLOV26_CLASSIFIER:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV26_CLASS
            EhsML_Yolov26_Class_Destroy(ctx);
#endif//EHS_ML_MODEL_SUPPORT_YOLOV26_CLASS
            break;
        }
        case EHS_ML_SAM_IMAGE_SEGMENTER:
        {
#ifdef EHS_ML_MODEL_SUPPORT_SAM_IMGSEG
            EhsML_SAM_ImgSeg_Destroy(ctx);
#endif//EHS_ML_MODEL_SUPPORT_SAM_IMGSEG
            break;
        }
        /* Text */
        case EHS_ML_TRANSFORMER_TEXT_CLASSIFIER:
        {
#ifdef EHS_ML_MODEL_SUPPORT_TRANSFORMER_TEXT_CLASS
            break;
            EhsML_Stubbed_Destroy(ctx);
#endif//EHS_ML_MODEL_SUPPORT_TRANSFORMER_TEXT_CLASS
            break;
        }
        case EHS_ML_TRANSFORMER_SENTIMENT_ANALYZER:
        {
#ifdef EHS_ML_MODEL_SUPPORT_TRANSFORMER_SENTIMENT_ANALYZER
            break;
            EhsML_Stubbed_Destroy(ctx);
#endif//EHS_ML_MODEL_SUPPORT_TRANSFORMER_SENTIMENT_ANALYZER
            break;
        }
        case EHS_ML_TRANSFORMER_NAMED_ENTITY_RECOGNITION:
        {
#ifdef EHS_ML_MODEL_SUPPORT_TRANSFORMER_NAMED_ENTITY_RECOGNITION
            break;
            EhsML_Stubbed_Destroy(ctx);
#endif//EHS_ML_MODEL_SUPPORT_TRANSFORMER_NAMED_ENTITY_RECOGNITION
            break;
        }
        case EHS_ML_TRANSFORMER_TEXT_SUMMARIZER:
        {
#ifdef EHS_ML_MODEL_SUPPORT_TRANSFORMER_TEXT_SUMMARIZER
            break;
            EhsML_Stubbed_Destroy(ctx);
#endif//EHS_ML_MODEL_SUPPORT_TRANSFORMER_TEXT_SUMMARIZER
            break;
        }
        case EHS_ML_TRANSFORMER_LANGUAGE_TRANSLATOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_TRANSFORMER_LANGUAGE_TRANSLATOR
            break;
            EhsML_Stubbed_Destroy(ctx);
#endif//EHS_ML_MODEL_SUPPORT_TRANSFORMER_LANGUAGE_TRANSLATOR
            break;
        }
        /* Audio */
        case EHS_ML_AUDIO_SPEECH_RECOGNITION:
        {
#ifdef EHS_ML_MODEL_SUPPORT_AUDIO_SPEECH_RECOGNITION
            break;
            EhsML_Stubbed_Destroy(ctx);
#endif//EHS_ML_MODEL_SUPPORT_AUDIO_SPEECH_RECOGNITION
            break;
        }
        case EHS_ML_AUDIO_KEYWORD_SPOTTING:
        {
#ifdef EHS_ML_MODEL_SUPPORT_AUDIO_KEYWORD_SPOTTING
            break;
            EhsML_Stubbed_Destroy(ctx);
#endif//EHS_ML_MODEL_SUPPORT_AUDIO_KEYWORD_SPOTTING
            break;
        }
        case EHS_ML_AUDIO_SPEAKER_IDENTIFICATION:
        {
#ifdef EHS_ML_MODEL_SUPPORT_AUDIO_SPEAKER_IDENTIFICATION
            break;
            EhsML_Stubbed_Destroy(ctx);
#endif//EHS_ML_MODEL_SUPPORT_AUDIO_SPEAKER_IDENTIFICATION
            break;
        }
        case EHS_ML_AUDIO_NOISE_SUPPRESSION:
        {
#ifdef EHS_ML_MODEL_SUPPORT_AUDIO_NOISE_SUPPRESSION
            break;
            EhsML_Stubbed_Destroy(ctx);
#endif//EHS_ML_MODEL_SUPPORT_AUDIO_NOISE_SUPPRESSION
            break;
        }
        case EHS_ML_AUDIO_SOUND_CLASSIFICATION:
        {
#ifdef EHS_ML_MODEL_SUPPORT_AUDIO_SOUND_CLASSIFICATION
            break;
            EhsML_Stubbed_Destroy(ctx);
#endif//EHS_ML_MODEL_SUPPORT_AUDIO_SOUND_CLASSIFICATION
            break;
        }
        case EHS_ML_AUDIO_VOICE_ACTIVITY_DETECTION:
        {
#ifdef EHS_ML_MODEL_SUPPORT_AUDIO_VOICE_ACTIVITY_DETECTION
            break;
            EhsML_Stubbed_Destroy(ctx);
#endif//EHS_ML_MODEL_SUPPORT_AUDIO_VOICE_ACTIVITY_DETECTION
            break;
        }
        //// ...
        default:
            break;
    }
    #endif//EHS_ML_SUPPORT_STUBBED
    EhsML_Tensor_Free(&ctx->output_tensor);
}

EhsML_Err EhsML_SetInputData(EhsML_Context* ctx, const void* data, ehs_uint32 size)
{
    if (ctx == NULL) return EHS_ML_FAILED;
    if (ctx->type >= EHS_ML_TYPE_MAX) return EHS_ML_FAILED;
    #ifdef EHS_ML_SUPPORT_STUBBED
    return EhsML_Stubbed_SetInputData(ctx, data, size);
    #else
    switch (ctx->type)
    {
        /* Stubbed */
        case EHS_ML_TYPE_STUBBED:
        {
            return EhsML_Stubbed_SetInputData(ctx, data, size);
        }
        /* Image */
        case EHS_ML_YOLOV3_OBJ_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV3_OBJDET
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov3_ObjDet_SetInputData(ctx, data, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV3_OBJDET
        }
        case EHS_ML_YOLOV4_OBJ_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV4_OBJDET
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov4_ObjDet_SetInputData(ctx, data, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV4_OBJDET
        }
        case EHS_ML_YOLOV5_OBJ_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV5_OBJDET
            return EhsML_Yolov5_ObjDet_SetInputData(ctx, data, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV5_OBJDET
        }
        case EHS_ML_YOLOV6_OBJ_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV6_OBJDET
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov6_ObjDet_SetInputData(ctx, data, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV6_OBJDET
        }
        case EHS_ML_YOLOV7_OBJ_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV7_OBJDET
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov7_ObjDet_SetInputData(ctx, data, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV7_OBJDET
        }
        case EHS_ML_YOLOV8_OBJ_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV8_OBJDET
            return EhsML_Yolov8_ObjDet_SetInputData(ctx, data, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV8_OBJDET
        }
        case EHS_ML_YOLOV8_INSTANCE_SEGMENTER:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV8_INSTSEG
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov8_InstSeg_SetInputData(ctx, data, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV8_INSTSEG
        }
        case EHS_ML_YOLOV8_POSE_ESTIMATOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV8_POSE
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov8_Pose_SetInputData(ctx, data, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV8_POSE
        }
        case EHS_ML_YOLOV8_ORIENTED_BBOX_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV8_OOB
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov8_OOB_SetInputData(ctx, data, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV8_OOB
        }
        case EHS_ML_YOLOV8_CLASSIFIER:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV8_CLASS
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov8_Class_SetInputData(ctx, data, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV8_CLASS
        }
        case EHS_ML_YOLOV9_OBJ_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV9_OBJDET
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov9_ObjDet_SetInputData(ctx, data, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV9_OBJDET
        }
        case EHS_ML_YOLOV9_INSTANCE_SEGMENTER:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV9_INSTSEG
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov9_InstSeg_SetInputData(ctx, data, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV9_INSTSEG
        }
        case EHS_ML_YOLOV10_OBJ_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV10_OBJDET
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov10_ObjDet_SetInputData(ctx, data, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV10_OBJDET
        }
        case EHS_ML_YOLOV11_OBJ_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV11_OBJDET
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov11_ObjDet_SetInputData(ctx, data, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV11_OBJDET
        }
        case EHS_ML_YOLOV11_INSTANCE_SEGMENTER:
        {   
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV11_INSTSEG
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov11_InstSeg_SetInputData(ctx, data, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV11_INSTSEG
        }
        case EHS_ML_YOLOV11_POSE_ESTIMATOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV11_POSE
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov11_Pose_SetInputData(ctx, data, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV11_POSE
        }
        case EHS_ML_YOLOV11_ORIENTED_BBOX_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV11_OOB
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov11_OOB_SetInputData(ctx, data, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV11_OOB
        }
        case EHS_ML_YOLOV11_CLASSIFIER:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV11_CLASS
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov11_Class_SetInputData(ctx, data, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV11_CLASS
        }
        case EHS_ML_YOLOV12_OBJ_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV12_OBJDET
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov12_ObjDet_SetInputData(ctx, data, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV12_OBJDET
        }
        case EHS_ML_YOLOV12_INSTANCE_SEGMENTER:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV12_INSTSEG
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov12_InstSeg_SetInputData(ctx, data, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV12_INSTSEG
        }
        case EHS_ML_YOLOV12_POSE_ESTIMATOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV12_POSE
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov12_Pose_SetInputData(ctx, data, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV12_POSE
        }
        case EHS_ML_YOLOV12_ORIENTED_BBOX_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV12_OOB
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov12_OOB_SetInputData(ctx, data, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV12_OOB
        }
        case EHS_ML_YOLOV12_CLASSIFIER:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV12_CLASS
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov12_Class_SetInputData(ctx, data, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV12_CLASS
        }
        case EHS_ML_YOLOV26_OBJ_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV26_OBJDET
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov26_ObjDet_SetInputData(ctx, data, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV26_OBJDET
        }
        case EHS_ML_YOLOV26_INSTANCE_SEGMENTER:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV26_INSTSEG
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov26_InstSeg_SetInputData(ctx, data, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV26_INSTSEG
        }
        case EHS_ML_YOLOV26_POSE_ESTIMATOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV26_POSE
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov26_Pose_SetInputData(ctx, data, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV26_POSE
        }
        case EHS_ML_YOLOV26_ORIENTED_BBOX_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV26_OOB
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov26_OOB_SetInputData(ctx, data, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV26_OOB
        }
        case EHS_ML_YOLOV26_CLASSIFIER:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV26_CLASS
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov26_Class_SetInputData(ctx, data, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV26_CLASS
        }
        case EHS_ML_SAM_IMAGE_SEGMENTER:
        {
#ifdef EHS_ML_MODEL_SUPPORT_SAM_IMGSEG
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_SAM_ImgSeg_SetInputData(ctx, data, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_SAM_IMGSEG
        }
        /* Text */
        case EHS_ML_TRANSFORMER_TEXT_CLASSIFIER:
        {
#ifdef EHS_ML_MODEL_SUPPORT_TRANSFORMER_TEXT_CLASS
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Stubbed_SetInputData(ctx, data, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_TRANSFORMER_TEXT_CLASS
        }
        case EHS_ML_TRANSFORMER_SENTIMENT_ANALYZER:
        {
#ifdef EHS_ML_MODEL_SUPPORT_TRANSFORMER_SENTIMENT_ANALYZER
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Stubbed_SetInputData(ctx, data, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_TRANSFORMER_SENTIMENT_ANALYZER
        }
        case EHS_ML_TRANSFORMER_NAMED_ENTITY_RECOGNITION:
        {
#ifdef EHS_ML_MODEL_SUPPORT_TRANSFORMER_NAMED_ENTITY_RECOGNITION
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Stubbed_SetInputData(ctx, data, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_TRANSFORMER_NAMED_ENTITY_RECOGNITION
        }
        case EHS_ML_TRANSFORMER_TEXT_SUMMARIZER:
        {
#ifdef EHS_ML_MODEL_SUPPORT_TRANSFORMER_TEXT_SUMMARIZER
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Stubbed_SetInputData(ctx, data, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_TRANSFORMER_TEXT_SUMMARIZER
        }
        case EHS_ML_TRANSFORMER_LANGUAGE_TRANSLATOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_TRANSFORMER_LANGUAGE_TRANSLATOR
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Stubbed_SetInputData(ctx, data, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_TRANSFORMER_LANGUAGE_TRANSLATOR
        }
        /* Audio */
        case EHS_ML_AUDIO_SPEECH_RECOGNITION:
        {
#ifdef EHS_ML_MODEL_SUPPORT_AUDIO_SPEECH_RECOGNITION
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Stubbed_SetInputData(ctx, data, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_AUDIO_SPEECH_RECOGNITION
        }
        case EHS_ML_AUDIO_KEYWORD_SPOTTING:
        {
#ifdef EHS_ML_MODEL_SUPPORT_AUDIO_KEYWORD_SPOTTING
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Stubbed_SetInputData(ctx, data, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_AUDIO_KEYWORD_SPOTTING
        }
        case EHS_ML_AUDIO_SPEAKER_IDENTIFICATION:
        {
#ifdef EHS_ML_MODEL_SUPPORT_AUDIO_SPEAKER_IDENTIFICATION
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Stubbed_SetInputData(ctx, data, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_AUDIO_SPEAKER_IDENTIFICATION
        }
        case EHS_ML_AUDIO_NOISE_SUPPRESSION:
        {
#ifdef EHS_ML_MODEL_SUPPORT_AUDIO_NOISE_SUPPRESSION
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Stubbed_SetInputData(ctx, data, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_AUDIO_NOISE_SUPPRESSION
        }
        case EHS_ML_AUDIO_SOUND_CLASSIFICATION:
        {
#ifdef EHS_ML_MODEL_SUPPORT_AUDIO_SOUND_CLASSIFICATION
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Stubbed_SetInputData(ctx, data, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_AUDIO_SOUND_CLASSIFICATION
        }
        case EHS_ML_AUDIO_VOICE_ACTIVITY_DETECTION:
        {
#ifdef EHS_ML_MODEL_SUPPORT_AUDIO_VOICE_ACTIVITY_DETECTION
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Stubbed_SetInputData(ctx, data, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_AUDIO_VOICE_ACTIVITY_DETECTION
        }
        //// ...
        default:
            return EHS_ML_MODEL_TYPE_ERR;
    }
    #endif//EHS_ML_SUPPORT_STUBBED
}

EhsML_Err EhsML_RunOutputJson(EhsML_Context* ctx, ehs_char* json, ehs_uint32 size)
{
    if (ctx == NULL) return EHS_ML_FAILED;
    if (ctx->type >= EHS_ML_TYPE_MAX) return EHS_ML_FAILED;
    #ifdef EHS_ML_SUPPORT_STUBBED
    return EhsML_Stubbed_RunOutputJson(ctx, json, size);
    #else
    switch (ctx->type)
    {
        /* Stubbed */
        case EHS_ML_TYPE_STUBBED:
        {
            return EhsML_Stubbed_RunOutputJson(ctx, json, size);
        }
        /* Image */
        case EHS_ML_YOLOV3_OBJ_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV3_OBJDET
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov3_ObjDet_RunOutputJson(ctx, json, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV3_OBJDET
        }
        case EHS_ML_YOLOV4_OBJ_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV4_OBJDET
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov4_ObjDet_RunOutputJson(ctx, json, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV4_OBJDET
        }
        case EHS_ML_YOLOV5_OBJ_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV5_OBJDET
            return EhsML_Yolov5_ObjDet_RunOutputJson(ctx, json, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV5_OBJDET
        }
        case EHS_ML_YOLOV6_OBJ_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV6_OBJDET
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov6_ObjDet_RunOutputJson(ctx, json, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV6_OBJDET
        }
        case EHS_ML_YOLOV7_OBJ_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV7_OBJDET
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov7_ObjDet_RunOutputJson(ctx, json, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV7_OBJDET
        }
        case EHS_ML_YOLOV8_OBJ_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV8_OBJDET
            return EhsML_Yolov8_ObjDet_RunOutputJson(ctx, json, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV8_OBJDET
        }
        case EHS_ML_YOLOV8_INSTANCE_SEGMENTER:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV8_INSTSEG
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov8_InstSeg_RunOutputJson(ctx, json, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV8_INSTSEG
        }
        case EHS_ML_YOLOV8_POSE_ESTIMATOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV8_POSE
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov8_Pose_RunOutputJson(ctx, json, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV8_POSE
        }
        case EHS_ML_YOLOV8_ORIENTED_BBOX_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV8_OOB
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov8_OOB_RunOutputJson(ctx, json, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV8_OOB
        }
        case EHS_ML_YOLOV8_CLASSIFIER:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV8_CLASS
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov8_Class_RunOutputJson(ctx, json, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV8_CLASS
        }
        case EHS_ML_YOLOV9_OBJ_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV9_OBJDET
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov9_ObjDet_RunOutputJson(ctx, json, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV9_OBJDET
        }
        case EHS_ML_YOLOV9_INSTANCE_SEGMENTER:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV9_INSTSEG
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov9_InstSeg_RunOutputJson(ctx, json, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV9_INSTSEG
        }
        case EHS_ML_YOLOV10_OBJ_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV10_OBJDET
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov10_ObjDet_RunOutputJson(ctx, json, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV10_OBJDET
        }
        case EHS_ML_YOLOV11_OBJ_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV11_OBJDET
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov11_ObjDet_RunOutputJson(ctx, json, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV11_OBJDET
        }
        case EHS_ML_YOLOV11_INSTANCE_SEGMENTER:
        {   
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV11_INSTSEG
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov11_InstSeg_RunOutputJson(ctx, json, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV11_INSTSEG
        }
        case EHS_ML_YOLOV11_POSE_ESTIMATOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV11_POSE
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov11_Pose_RunOutputJson(ctx, json, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV11_POSE
        }
        case EHS_ML_YOLOV11_ORIENTED_BBOX_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV11_OOB
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov11_OOB_RunOutputJson(ctx, json, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV11_OOB
        }
        case EHS_ML_YOLOV11_CLASSIFIER:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV11_CLASS
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov11_Class_RunOutputJson(ctx, json, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV11_CLASS
        }
        case EHS_ML_YOLOV12_OBJ_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV12_OBJDET
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov12_ObjDet_RunOutputJson(ctx, json, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV12_OBJDET
        }
        case EHS_ML_YOLOV12_INSTANCE_SEGMENTER:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV12_INSTSEG
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov12_InstSeg_RunOutputJson(ctx, json, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV12_INSTSEG
        }
        case EHS_ML_YOLOV12_POSE_ESTIMATOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV12_POSE
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov12_Pose_RunOutputJson(ctx, json, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV12_POSE
        }
        case EHS_ML_YOLOV12_ORIENTED_BBOX_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV12_OOB
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov12_OOB_RunOutputJson(ctx, json, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV12_OOB
        }
        case EHS_ML_YOLOV12_CLASSIFIER:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV12_CLASS
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov12_Class_RunOutputJson(ctx, json, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV12_CLASS
        }
        case EHS_ML_YOLOV26_OBJ_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV26_OBJDET
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov26_ObjDet_RunOutputJson(ctx, json, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV26_OBJDET
        }
        case EHS_ML_YOLOV26_INSTANCE_SEGMENTER:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV26_INSTSEG
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov26_InstSeg_RunOutputJson(ctx, json, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV26_INSTSEG
        }
        case EHS_ML_YOLOV26_POSE_ESTIMATOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV26_POSE
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov26_Pose_RunOutputJson(ctx, json, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV26_POSE
        }
        case EHS_ML_YOLOV26_ORIENTED_BBOX_DETECTOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV26_OOB
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov26_OOB_RunOutputJson(ctx, json, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV26_OOB
        }
        case EHS_ML_YOLOV26_CLASSIFIER:
        {
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV26_CLASS
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Yolov26_Class_RunOutputJson(ctx, json, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_YOLOV26_CLASS
        }
        case EHS_ML_SAM_IMAGE_SEGMENTER:
        {
#ifdef EHS_ML_MODEL_SUPPORT_SAM_IMGSEG
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_SAM_ImgSeg_RunOutputJson(ctx, json, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_SAM_IMGSEG
        }
        /* Text */
        case EHS_ML_TRANSFORMER_TEXT_CLASSIFIER:
        {
#ifdef EHS_ML_MODEL_SUPPORT_TRANSFORMER_TEXT_CLASS
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Stubbed_RunOutputJson(ctx, json, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_TRANSFORMER_TEXT_CLASS
        }
        case EHS_ML_TRANSFORMER_SENTIMENT_ANALYZER:
        {
#ifdef EHS_ML_MODEL_SUPPORT_TRANSFORMER_SENTIMENT_ANALYZER
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Stubbed_RunOutputJson(ctx, json, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_TRANSFORMER_SENTIMENT_ANALYZER
        }
        case EHS_ML_TRANSFORMER_NAMED_ENTITY_RECOGNITION:
        {
#ifdef EHS_ML_MODEL_SUPPORT_TRANSFORMER_NAMED_ENTITY_RECOGNITION
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Stubbed_RunOutputJson(ctx, json, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_TRANSFORMER_NAMED_ENTITY_RECOGNITION
        }
        case EHS_ML_TRANSFORMER_TEXT_SUMMARIZER:
        {
#ifdef EHS_ML_MODEL_SUPPORT_TRANSFORMER_TEXT_SUMMARIZER
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Stubbed_RunOutputJson(ctx, json, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_TRANSFORMER_TEXT_SUMMARIZER
        }
        case EHS_ML_TRANSFORMER_LANGUAGE_TRANSLATOR:
        {
#ifdef EHS_ML_MODEL_SUPPORT_TRANSFORMER_LANGUAGE_TRANSLATOR
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Stubbed_RunOutputJson(ctx, json, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_TRANSFORMER_LANGUAGE_TRANSLATOR
        }
        /* Audio */
        case EHS_ML_AUDIO_SPEECH_RECOGNITION:
        {
#ifdef EHS_ML_MODEL_SUPPORT_AUDIO_SPEECH_RECOGNITION
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Stubbed_RunOutputJson(ctx, json, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_AUDIO_SPEECH_RECOGNITION
        }
        case EHS_ML_AUDIO_KEYWORD_SPOTTING:
        {
#ifdef EHS_ML_MODEL_SUPPORT_AUDIO_KEYWORD_SPOTTING
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Stubbed_RunOutputJson(ctx, json, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_AUDIO_KEYWORD_SPOTTING
        }
        case EHS_ML_AUDIO_SPEAKER_IDENTIFICATION:
        {
#ifdef EHS_ML_MODEL_SUPPORT_AUDIO_SPEAKER_IDENTIFICATION
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Stubbed_RunOutputJson(ctx, json, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_AUDIO_SPEAKER_IDENTIFICATION
        }
        case EHS_ML_AUDIO_NOISE_SUPPRESSION:
        { 
#ifdef EHS_ML_MODEL_SUPPORT_AUDIO_NOISE_SUPPRESSION
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Stubbed_RunOutputJson(ctx, json, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_AUDIO_NOISE_SUPPRESSION
        }
        case EHS_ML_AUDIO_SOUND_CLASSIFICATION:
        {
#ifdef EHS_ML_MODEL_SUPPORT_AUDIO_SOUND_CLASSIFICATION
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Stubbed_RunOutputJson(ctx, json, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_AUDIO_SOUND_CLASSIFICATION
        }
        case EHS_ML_AUDIO_VOICE_ACTIVITY_DETECTION:
        {
#ifdef EHS_ML_MODEL_SUPPORT_AUDIO_VOICE_ACTIVITY_DETECTION
            return EHS_ML_NOT_IMPLEMENTED;
            return EhsML_Stubbed_RunOutputJson(ctx, json, size);
#else
            return EHS_ML_NOT_SUPPORTED;
#endif//EHS_ML_MODEL_SUPPORT_AUDIO_VOICE_ACTIVITY_DETECTION
        }
        //// ...
        default:
            return EHS_ML_MODEL_TYPE_ERR;
    }
    #endif//EHS_ML_SUPPORT_STUBBED
}
