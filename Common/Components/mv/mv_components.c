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
#include "inx-apriltag.h"
#include "inx-frame_show.h"
#include "inx-mv_objdet_annotate.h"

EHS_C_CPP_EXPORT const EhsBlockRefType EhsBlockRefTable_mv[] =
{
	EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_Camera,INXWARE_FB_ID_Camera,Camera),
	EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_FrameCrop,INXWARE_FB_ID_FrameCrop,FrameCrop),
	EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_FrameResize,INXWARE_FB_ID_FrameResize,FrameResize),
	EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_FrameFormatter,INXWARE_FB_ID_FrameFormatter,FrameFormatter),
	EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_FrameReadFromFile,INXWARE_FB_ID_FrameReadFromFile,FrameReadFromFile),
	EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_Camera,INXWARE_FB_ID_apriltag,apriltag),
	EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_frame_show,INXWARE_FB_ID_frame_show,frame_show),
	EHS_BLOCKREF_ENTRY_WITH_DESTROY(INXWARE_FB_NAME_mv_objdet_annotate,INXWARE_FB_ID_mv_objdet_annotate,mv_objdet_annotate),
	{0}
};

#endif // EHS_MV_SUPPORT
