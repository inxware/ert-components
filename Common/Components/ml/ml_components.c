/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/** @file ml_components.c
 * Contains the static definition of the function block reference table for EHS
 *
 * @author: inx limited
 *
 */

#include "blockref_table.h"


#ifdef EHS_ML_SUPPORT

#include "inx-ml_image_inference.h"
#include "inx-ml_svm_inference.h"

EHS_C_CPP_EXPORT const EhsBlockRefType EhsBlockRefTable_ml[] =
{
#ifdef EHS_MV_SUPPORT /* these FBs rely on camera support to obtain frames */
	EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_ml_image_inference,INXWARE_FB_ID_ml_image_inference,ml_image_inference),
#endif
	EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_ml_svm_inference,INXWARE_FB_ID_ml_svm_inference,ml_svm_inference),
	{0}
};

#endif
