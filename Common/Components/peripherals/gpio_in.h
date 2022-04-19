/**
 * @file gpio_in.h
 * This declares the hardware gpio control functions.
 *
 */

#ifndef _EHS_GPIO_IN_H
#define _EHS_GPIO_IN_H

#include "ehs_fb_types.h"
#include "inx-component.h"

#define EHS_FB_GPIO_IN_NAME "gpio_in"

EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(gpio_in);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(gpio_in);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(gpio_in_read);
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(gpio_in);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(gpio_in)

#endif /* _EHS_GPIO_IN_H */
