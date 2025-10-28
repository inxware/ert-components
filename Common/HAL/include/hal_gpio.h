/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file hal_gpio.h
 * In this file, all of the hardware abstraction layer functions relating to GPIO are given.
 *
 * @author: inx limited
 *
 */

#ifndef _EHS_HAL_GPIO_H
#define _EHS_HAL_GPIO_H

/*****************************************************************************/
/* Included files */
#include "globals.h"

/*Define types*/

typedef struct
{
    ehs_sint32 pin_id;
    ehs_bool pin_value;
    ehs_bool open_drain_mode;
    ehs_bool error_state;
    ehs_uint8 initial_state; /* 0:false, 1:true  2:Hi-Z (unitialised), 255 - applied */
    void* target_data;
} ehs_gpio_out_state_type;

typedef struct
{
    ehs_sint32 pin_id;
    ehs_bool pin_value;
    void* target_data;
} ehs_gpio_in_state_type;


// Handle GPIO output
EHS_GLOBAL ehs_bool EhsInitOutputGPIO(ehs_gpio_out_state_type* pGPIO);

EHS_GLOBAL ehs_bool EhsWriteOutputGPIO(ehs_gpio_out_state_type* pGPIO);

EHS_GLOBAL ehs_bool EhsDestroyOutputGPIO(ehs_gpio_out_state_type* pGPIO);

// Handle GPIO input
EHS_GLOBAL ehs_bool EhsInitInputGPIO(ehs_gpio_in_state_type* pGPIO);

EHS_GLOBAL ehs_bool EhsReadInputGPIO(ehs_gpio_in_state_type* pGPIO);

EHS_GLOBAL ehs_bool EhsDestroyInputGPIO(ehs_gpio_in_state_type* pGPIO);

#endif /* _EHS_HAL_GPIO_H */