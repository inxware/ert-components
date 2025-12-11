#ifndef _EHS_TARGET_VIEWPORT_MODEB_H_
#define _EHS_TARGET_VIEWPORT_MODEB_H_

#include "widget.h"

/* Extended UI widget functions for Mode B Qt */
void EhsTargetWidgetUi_create(struct EhsWidgetStruct* pWidget, EhsTVClass* pViewport);
void EhsTargetWidgetUi_destroy(struct EhsWidgetStruct* pWidget);
void EhsTargetWidgetUi_draw(struct EhsWidgetStruct* pWidget);
void EhsTargetWidgetUi_viewport_cleanup(struct EhsWidgetStruct* pWidget);

/* Generic functions */
void EhsTargetWidget_show(struct EhsWidgetStruct* pWidget, ehs_uint8 nState);

#endif /* _EHS_TARGET_VIEWPORT_MODEB_H_ */
