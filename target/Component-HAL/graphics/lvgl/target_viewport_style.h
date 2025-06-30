/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

/** @file target_viewport.h
 * The target-specific declarations required to support the HAL for viewport services
 * are defined here. This file should only be included by hal_viewport.h
 *
 * @author: inx limited
 *
 */

#ifndef EHS_TARGET_VIEWPORT_STYLE_H
#define EHS_TARGET_VIEWPORT_STYLE_H

#include "lvgl/lvgl.h"
#include "ehs_types.h"

#define EHS_CUSTOM_STYLE_8_ID 8
#define EHS_CUSTOM_STYLE_9_ID 9

/* custom gauge styles */
#define GAUGE_SCALE_MAIN_ARC  1
#define GAUGE_SCALE_LEFT_ARC  2
#define GAUGE_SCALE_RIGHT_ARC 3
#define GAUGE_SCALE_NEEDLE    4
lv_color_t ehs_gauge_color(ehs_uint16 part, ehs_uint16 custom_style);

/* default chart styles */
#define EHS_CHART_DATA1_ID    1
#define EHS_CHART_DATA2_ID    2
#define EHS_CHART_DATA3_ID    3

lv_color_t ehs_chart_color(ehs_uint16 data_id, ehs_uint16 custom_style);

#endif /* EHS_TARGET_VIEWPORT_STYLE_H */