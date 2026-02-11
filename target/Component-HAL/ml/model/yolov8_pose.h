/***************************************************************
* Copyright (C) 2008-2025 inx limited - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/** @file ml_model_template.h
 * Model implementation template header file.
 *
 * @author: inx limited
 *
 */

#ifndef _C_HEADER_EHS_TARGET_HAL_ML_MODEL_YOLOV8_POSE_H_
#define _C_HEADER_EHS_TARGET_HAL_ML_MODEL_YOLOV8_POSE_H

/*****************************************************************************/
/* Included files */

/*****************************************************************************/
/* Define macros  */
#define EhsML_Yolov8_Pose_Create EhsML_Model_Boilerplate_Create
#define EhsML_Yolov8_Pose_Destroy EhsML_Model_Boilerplate_Destroy
#define EhsML_Yolov8_Pose_SetInputData EhsML_Model_Boilerplate_SetInputData

/*****************************************************************************/
/* Define types */


/*****************************************************************************/
/* Declare global variables */


/*****************************************************************************/
/* Declare function prototypes  */

EhsML_Err EhsML_Yolov8_Pose_RunOutputJson(EhsML_Context* ctx, ehs_char* json_output, ehs_uint32 output_size);


#endif /*_C_HEADER_EHS_TARGET_HAL_ML_MODEL_YOLOV8_POSE_H*/