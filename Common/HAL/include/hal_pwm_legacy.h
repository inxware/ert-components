/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file hal_pwm_legacy.h.
 *
 * @author: inx limited
 *
 */


#ifndef EHS_HAL_PWM_LEGACY_H
#define EHS_HAL_PWM_LEGACY_H

/*****************************************************************************/
/* Included files */
#include "ehs_types.h"


ehs_bool EhsPWMConfig(const ehs_uint8 pin, const ehs_uint32 periodHz);
ehs_bool EhsPWMEnable(const ehs_uint8 pin, const ehs_bool enable);
ehs_bool EhsPWMDuty(const ehs_uint8 pin, const ehs_uint8 powerPercent);

#endif /* EHS_HAL_PWM_H */