/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

/** @file ml_components.c
 * Contains the static definition of the function block reference table for EHS
 *
 * @author: inx limited
 *
 */

#include "blockref_table.h"


#ifdef EHS_ML_SUPPORT

#include "inx-tf_lite_frame.h"
#include "inx-tflm.h"

extern const EhsBlockRefType EhsBlockRefTable_ml[] =
{
#ifdef EHS_MV_SUPPORT // this FB rely on the camera support to obtain frame
	EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_TF_Lite_from_frame,INXWARE_FB_ID_TF_Lite_from_frame,TF_Lite_from_frame),
#endif
	EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_TFLM,INXWARE_FB_ID_TFLM,TFLM),
	{0}
};

#endif
