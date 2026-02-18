/***************************************************************
 * Copyright (C) 2025 inx limited, UK - All Rights Reserved.
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license
 * with this file. If not, please visit:
 *  <https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

#ifndef EHS_TARGET_VIEWPORT_H
#define EHS_TARGET_VIEWPORT_H

#ifndef EHS_HAL_VIEWPORT_H
#error "This file should only be included by hal_viewport.h"
#endif

#include "ertqt.h"
#include "graphics.h"

// Qt-specific widget data stored in EhsWidgetStruct - not used
//typedef struct
//{
//    ertqt_object_handle qt_handle;
//    const char * widget_type;  // "button", "label", "textfield", etc.
//} EhsWidgetQtData;

// Qt surface representation (minimal for PoC)
struct EhsTVSurfaceStruct
{
    EhsGraphicsColourFormatEnum eFormat;
    // Qt renders everything, surfaces not heavily used
    struct EhsTVSurfaceStruct * pNext;
} __attribute__((packed));

#endif /* EHS_TARGET_VIEWPORT_H */
