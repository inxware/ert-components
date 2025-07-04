/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/** @file camera_components.c
 * Contains the static definition of the function block reference table for EHS
 *
 * @author: inx limited
 *
 */

#include "blockref_table.h"

#ifdef EHS_MV_SUPPORT

#include "inx-camera.h"
#include "inx-frame_crop.h"
#include "inx-frame_resize.h"
#include "inx-frame_fmt.h"
#include "inx-frame_read_file.h"

extern const EhsBlockRefType EhsBlockRefTable_mv[] =
{
	EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_Camera,INXWARE_FB_ID_Camera,Camera),
	EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_FrameCrop,INXWARE_FB_ID_FrameCrop,FrameCrop),
	EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_FrameResize,INXWARE_FB_ID_FrameResize,FrameResize),
	EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_FrameFormatter,INXWARE_FB_ID_FrameFormatter,FrameFormatter),
	EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_FrameReadFromFile,INXWARE_FB_ID_FrameReadFromFile,FrameReadFromFile),
	{0}
};

#endif // EHS_MV_SUPPORT
