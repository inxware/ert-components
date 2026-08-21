/***************************************************************
* Copyright (C) 2008-2025 inx limited - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/** @file ert_hal_axelera.h
 * Axelera Voyager SDK (axruntime) inference engine backend.
 * Wraps the axruntime C API for Axelera Metis NPU accelerators.
 *
 * @author: inx limited
 */

#ifndef _C_HEADER_EHS_TARGET_HAL_ML_ENGINE_AXELERA_ERT_HAL_AXELERA_H_
#define _C_HEADER_EHS_TARGET_HAL_ML_ENGINE_AXELERA_ERT_HAL_AXELERA_H_

#include "hal_ml.h"

#ifdef __cplusplus
extern "C" {
#endif

EhsML_Err EhsML_FW_Axelera_Create(EhsML_Context *ctx, const ehs_char *model_path, ehs_float conf_thres, ehs_sint32 thread_count);

void EhsML_FW_Axelera_Destroy(EhsML_Context *ctx);

EhsML_Err EhsML_FW_Axelera_SetInputData(EhsML_Context *ctx, const void *input_data, ehs_uint32 data_size);

EhsML_Err EhsML_FW_Axelera_GetOutputData(EhsML_Context *ctx);

#ifdef __cplusplus
}
#endif

#endif /* _C_HEADER_EHS_TARGET_HAL_ML_ENGINE_AXELERA_ERT_HAL_AXELERA_H_ */
