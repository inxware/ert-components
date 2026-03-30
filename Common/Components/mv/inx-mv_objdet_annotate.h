/***************************************************************
* Copyright (C) 2008-2026 inx limited - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*   <https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

#ifndef _INX_MV_OBJDET_ANNOTATE_H_
#define _INX_MV_OBJDET_ANNOTATE_H_

#include "inx-component.h"

#define INXWARE_FB_ID_mv_objdet_annotate    0x08BF
#define INXWARE_FB_NAME_mv_objdet_annotate  "mv_objdet_annotate"

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(mv_objdet_annotate);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(mv_objdet_annotate);
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(mv_objdet_annotate);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(mv_objdet_annotate_annotate);

EHS_FB_FUNCTIONS(mv_objdet_annotate)

#endif /* _INX_MV_OBJDET_ANNOTATE_H_ */
