/***************************************************************
* Copyright (C) 2008-2025 inx limited - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/** @file ml_common.c  (stubbed variant — target/Component-HAL/ml/stubbed/)
 * Stub implementation of the ml_common public API.
 *
 * Selected by ml_common.mk when EHS_ML_SUPPORT=stubbed: the stubbed/
 * directory is placed first on VPATH so make picks up this file instead
 * of the real ml_common.c.  Provides the same function signatures with
 * no-op bodies so that platforms without ML support (e.g. Windows) link
 * cleanly without any #ifdef in the real implementation.
 *
 * @author: inx limited
 */

#include "hal_ml.h"

EhsML_HWAccel_t EhsML_HWAccel_supported(void)
{
    return EHS_ML_HWACCEL_NONE;
}

EhsML_Err EhsML_Tensor_Alloc(EhsML_Tensor_t* tensor, EhsML_DataType_t data_type, const ehs_uint32* dims, ehs_uint32 num_dims)
{
    (void)tensor; (void)data_type; (void)dims; (void)num_dims;
    return EHS_ML_OK;
}

void EhsML_Tensor_Free(EhsML_Tensor_t* tensor)
{
    (void)tensor;
}

EhsML_Err EhsML_Tensor_FillRaw(EhsML_Tensor_t* tensor, ehs_char* value, size_t size_in_bytes)
{
    (void)tensor; (void)value; (void)size_in_bytes;
    return EHS_ML_OK;
}

EhsML_Err EhsML_Create(EhsML_Context* ctx, const ehs_char* model_path, EhsML_Type model_type, ehs_float conf_thres, ehs_sint32 thread_count)
{
    (void)ctx; (void)model_path; (void)model_type; (void)conf_thres; (void)thread_count;
    return EHS_ML_OK;
}

void EhsML_Destroy(EhsML_Context* ctx)
{
    (void)ctx;
}

EhsML_Err EhsML_SetInputData(EhsML_Context* ctx, const void* data, ehs_uint32 size)
{
    (void)ctx; (void)data; (void)size;
    return EHS_ML_OK;
}

EhsML_Err EhsML_Run(EhsML_Context* ctx)
{
    (void)ctx;
    return EHS_ML_OK;
}

EhsML_Err EhsML_GetOutput(EhsML_Context* ctx, ehs_char* buf, ehs_uint32 size)
{
    (void)ctx; (void)buf; (void)size;
    return EHS_ML_OK;
}

EhsML_Err EhsML_RunAndGetOutput(EhsML_Context* ctx, ehs_char* buf, ehs_uint32 size)
{
    (void)ctx; (void)buf; (void)size;
    return EHS_ML_OK;
}

EhsML_Err EhsML_RunOutputJson(EhsML_Context* ctx, ehs_char* json, ehs_uint32 size)
{
    (void)ctx; (void)json; (void)size;
    return EHS_ML_OK;
}

EhsML_Err EhsML_GetModelInfoJson(EhsML_Context* ctx, const ehs_char* model_path, ehs_char* json_buf, ehs_uint32 json_size)
{
    (void)ctx; (void)model_path; (void)json_buf; (void)json_size;
    return EHS_ML_OK;
}
