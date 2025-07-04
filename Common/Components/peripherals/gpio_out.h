/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/
/**
 * @file gpio_out.h
 * This declares the hardware gpio control functions.
 *
 */

#ifndef _EHS_GPIO_OUT_H
#define _EHS_GPIO_OUT_H

#include "ehs_fb_types.h"
#include "inx-component.h"

#define EHS_FB_NAME_gpio_out "gpio_out"
#define EHS_FB_ID_gpio_out 0x87EF

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(gpio_out);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(gpio_out);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gpio_out_write);
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(gpio_out);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(gpio_out)

#endif /* _EHS_GPIO_OUT_H */
