/**
 * @file gpio_out.h
 * This declares the hardware gpio control functions.
 *
 */

#ifndef _EHS_GPIO_OUT_H
#define _EHS_GPIO_OUT_H

#include "ehs_fb_types.h"
#include "inx-component.h"

#define EHS_FB_GPIO_OUT_NAME "gpio_out"

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(gpio_out);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(gpio_out);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gpio_out_write);
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(gpio_out);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(gpio_out)

#endif /* _EHS_GPIO_OUT_H */
