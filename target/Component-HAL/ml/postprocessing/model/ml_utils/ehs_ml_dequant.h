/***************************************************************
* Copyright (C) 2008-2025 inx limited - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*   <https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/** @file ehs_ml_dequant.h
 * Shared dequantisation macro for ML model post-processing.
 *
 * Provides EHS_ML_TENSOR_DEQUANT — a type-dispatched macro that reads a
 * single element from any EhsML_Tensor_t and converts it to a double using
 * the affine dequantisation formula:
 *
 *   result = (raw_value - zero_point) * scale
 *
 * For floating-point tensor types (FP32, FP64) the scale and zero_point are
 * ignored and the value is cast directly to double.
 *
 * Usage:
 * @code
 *   double val;
 *   EHS_ML_TENSOR_DEQUANT(val, &ctx->output_tensor[0], flat_index, zero_point, scale);
 * @endcode
 *
 * Include this header in any model post-processor that needs to read from a
 * typed tensor. Do NOT use bare data_ptr casts — they silently break for
 * quantised (INT8 / UINT8) models.
 */

#ifndef _C_HEADER_EHS_TARGET_HAL_ML_MODEL_ML_UTILS_EHS_ML_DEQUANT_H_
#define _C_HEADER_EHS_TARGET_HAL_ML_MODEL_ML_UTILS_EHS_ML_DEQUANT_H_

#include "hal_ml.h"

/**
 * @brief Read and dequantise one element from a typed tensor.
 *
 * @param assignee    lvalue of type double that receives the result.
 * @param tensor_ptr  Pointer to EhsML_Tensor_t.
 * @param index       Flat element index into the tensor data buffer.
 * @param zero_point  Quantisation zero-point (from tensor.quantisation_params.offset).
 * @param scale       Quantisation scale     (from tensor.quantisation_params.scale).
 */
#define EHS_ML_TENSOR_DEQUANT(assignee, tensor_ptr, index, zero_point, scale) do { \
    switch ((tensor_ptr)->data_type) \
    {   \
        case EHS_ML_DATATYPE_NONE:  \
        {   \
            assignee = (double)(((tensor_ptr)->data_ptr.raw[index] - (zero_point)) * (scale));  \
            break;  \
        }   \
        case EHS_ML_DATATYPE_UINT8: \
        {   \
            assignee = (double)(((tensor_ptr)->data_ptr.u8[index] - (zero_point)) * (scale));   \
            break;  \
        }   \
        case EHS_ML_DATATYPE_INT8:  \
        {   \
            assignee = (double)(((tensor_ptr)->data_ptr.s8[index] - (zero_point)) * (scale));   \
            break;  \
        }   \
        case EHS_ML_DATATYPE_UINT16:    \
        {   \
            assignee = (double)(((tensor_ptr)->data_ptr.u16[index] - (zero_point)) * (scale));  \
            break;  \
        }   \
        case EHS_ML_DATATYPE_INT16: \
        {   \
            assignee = (double)(((tensor_ptr)->data_ptr.s16[index] - (zero_point)) * (scale));  \
            break;  \
        }   \
        case EHS_ML_DATATYPE_UINT32:    \
        {   \
            assignee = (double)(((tensor_ptr)->data_ptr.u32[index] - (zero_point)) * (scale));  \
            break;  \
        }   \
        case EHS_ML_DATATYPE_INT32: \
        {   \
            assignee = (double)(((tensor_ptr)->data_ptr.s32[index] - (zero_point)) * (scale));  \
            break;  \
        }   \
        case EHS_ML_DATATYPE_UINT64:    \
        {   \
            assignee = (double)(((tensor_ptr)->data_ptr.u64[index] - (zero_point)) * (scale));  \
            break;  \
        }   \
        case EHS_ML_DATATYPE_INT64: \
        {   \
            assignee = (double)(((tensor_ptr)->data_ptr.s64[index] - (zero_point)) * (scale));  \
            break;  \
        }   \
        case EHS_ML_DATATYPE_FP16:  \
        {   \
            /* TODO: convert FP16 to double */  \
            break;  \
        }   \
        case EHS_ML_DATATYPE_FP32:  \
        {   \
            assignee = (double)((tensor_ptr)->data_ptr.f32[index]); \
            break;  \
        }   \
        case EHS_ML_DATATYPE_FP64:  \
        {   \
            assignee = (double)((tensor_ptr)->data_ptr.f64[index]); \
            break;  \
        }   \
        case EHS_ML_DATATYPE_COMPLEX64: \
        {   \
            /* TODO: convert complex64 */ \
            break;  \
        }   \
        case EHS_ML_DATATYPE_COMPLEX128:    \
        {   \
            /* TODO: convert complex128 */  \
            break;  \
        }   \
        case EHS_ML_DATATYPE_BOOL:  \
        {   \
            assignee = (double)(((tensor_ptr)->data_ptr.b[index] - (zero_point)) * (scale));    \
            break;  \
        }   \
        case EHS_ML_DATATYPE_RAW:   \
        {   \
            assignee = (double)(((tensor_ptr)->data_ptr.raw[index] - (zero_point)) * (scale));  \
            break;  \
        }   \
        case EHS_ML_DATATYPE_CONST_RAW: \
        {   \
            assignee = (double)(((tensor_ptr)->data_ptr.const_raw[index] - (zero_point)) * (scale));    \
            break;  \
        }   \
        default:    \
        {   \
            assignee = (double)(((tensor_ptr)->data_ptr.raw[index] - (zero_point)) * (scale));  \
            break;  \
        }   \
    }   \
} while (0)

#endif /* _C_HEADER_EHS_TARGET_HAL_ML_MODEL_ML_UTILS_EHS_ML_DEQUANT_H_ */
