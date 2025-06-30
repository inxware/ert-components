#ifndef _EHS_TARGET_VIEWPORT_MODEB_H_
#define _EHS_TARGET_VIEWPORT_MODEB_H_

#include "widget.h"

/* extended ui widget */
void EhsTargetWidgetUi_create(struct EhsWidgetStruct* pWidget, EhsTVClass* pViewport);
void EhsTargetWidgetUi_destroy(struct EhsWidgetStruct* pWidget);

void EhsTargetWidgetUi_draw(struct EhsWidgetStruct* pWidget);

void EhsTargetWidgetUi_viewport_cleanup(struct EhsWidgetStruct* pWidget);

/* generic functions */
void EhsTargetWidget_show(struct EhsWidgetStruct* pWidget, ehs_uint8 nState);

#endif
