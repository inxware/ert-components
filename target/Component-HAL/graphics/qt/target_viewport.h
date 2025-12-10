#ifndef EHS_TARGET_VIEWPORT_H
#define EHS_TARGET_VIEWPORT_H

#ifndef EHS_HAL_VIEWPORT_H
#error "This file should only be included by hal_viewport.h"
#endif

#include "ertqt.h"
#include "graphics.h"

// Qt-specific widget data stored in EhsWidgetStruct
typedef struct
{
    ertqt_object_handle qt_handle;
    const char * widget_type;  // "button", "label", "textfield", etc.
} EhsWidgetQtData;

// Qt surface representation (minimal for PoC)
struct EhsTVSurfaceStruct
{
    EhsGraphicsColourFormatEnum eFormat;
    // Qt renders everything, surfaces not heavily used
    struct EhsTVSurfaceStruct * pNext;
} __attribute__((packed));

#endif /* EHS_TARGET_VIEWPORT_H */
