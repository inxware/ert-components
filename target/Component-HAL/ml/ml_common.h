/***************************************************************
* Copyright (C) 2008-2025 inx limited - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/** @file ml_common.h
 * Common code for Hailo and other ML backends.
 *
 * @author: inx limited
 *
 */

#ifndef _C_HEADER_EHS_TARGET_HAL_ML_ML_COMMON_H_
#define _C_HEADER_EHS_TARGET_HAL_ML_ML_COMMON_H_

/*****************************************************************************/
/* Included files */
#include "hal_ml.h"
#include "stubbed_ml.h"
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV5_OBJDET
#include "yolov5_objdet.h"
#endif//EHS_ML_MODEL_SUPPORT_YOLOV5_OBJDET
#ifdef EHS_ML_MODEL_SUPPORT_YOLOV8_OBJDET
#include "yolov8_objdet.h"
#endif//EHS_ML_MODEL_SUPPORT_YOLOV8_OBJDET

/*****************************************************************************/
/* Define macros  */

/*****************************************************************************/
/* Define types */

/*****************************************************************************/
/* Declare global variables */

/*****************************************************************************/
/* Declare function prototypes  */

#endif /*_C_HEADER_EHS_TARGET_HAL_ML_ML_COMMON_H_*/
