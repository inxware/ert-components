/*
 * target_gpio.h
 */

#ifndef TARGET_GPIO_H_
#define TARGET_GPIO_H_

#include "target_types.h"

// Handle GPIO output
EHS_GLOBAL ehs_bool EhsInitOutputGPIO(ehs_sint32 pin_id);

EHS_GLOBAL ehs_bool EhsWriteOutputGPIO(ehs_sint32 pin_id, ehs_bool value);

EHS_GLOBAL ehs_bool EhsDestroyOutputGPIO(ehs_sint32 pin_id);

// Handle GPIO input
EHS_GLOBAL ehs_bool EhsInitInputGPIO(ehs_sint32 pin_id);

EHS_GLOBAL ehs_bool EhsReadInputGPIO(ehs_sint32 pin_id, ehs_bool* value);

EHS_GLOBAL ehs_bool EhsDestroyInputGPIO(ehs_sint32 pin_id);

#endif /* TARGET_GPIO_H_ */
