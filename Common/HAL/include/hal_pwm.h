/**
 * High-Level Hardware PWM API
 * file: inx_pwm.h
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 * 
 */

#ifndef _HAL_PWM_H_ 
#define _HAL_PWM_H_ 

#include "ehs_types.h"
#include "timer.h"

typedef struct {
    ehs_sint32 freq;
    ehs_uint32 duty;
    ehs_bool enabled;
} inx_hw_pwm_status_t;

typedef struct inx_hw_pwm {
    struct inx_hw_pwm *pPrev;
    struct inx_hw_pwm *pNext;
    ehs_sint32 channel;
    ehs_sint32 io_num;
    ehs_sint32 max_val;
    ehs_bool hw;
    EhsTimerType sw_timers[2];       // PWM Software On/Off timers. On first, off second.
    ehs_bool sw_timer_delay_triggered;    // Whether the Off (delayed) timer is triggerred. Reset and Checked every duty update.
    inx_hw_pwm_status_t *pStatus;
} inx_hw_pwm_t;

/**
 * Setup the HW PWM interface
 * Example code:
 *  #include "hal_pwm.h"
 *  ...
 *  inx_hw_pwm_t *pPwmObj = NULL;
 *  void func() {
 *      EhsTPwmSetup(0, 14, 4000, 8192, &pPwmObj);
 *      ...
 *  }
 */
ehs_sint32 EhsTPwmSetup(ehs_sint32 channel, ehs_sint32 io_num, ehs_sint32 freq, ehs_sint32 max_val, ehs_bool hw, inx_hw_pwm_t **obj);
ehs_sint32 EhsTPwmDeinit(inx_hw_pwm_t *obj);
ehs_sint32 EhsTPwmSetFreq(inx_hw_pwm_t *obj, ehs_sint32 freq);
ehs_sint32 EhsTPwmSetDuty(inx_hw_pwm_t *obj, ehs_uint32 duty);
ehs_sint32 EhsTPwmSetMaxRes(inx_hw_pwm_t *obj, ehs_uint32 duty);
ehs_sint32 EhsTPwmEnable( inx_hw_pwm_t *obj );
ehs_sint32 EhsTPwmDisable( inx_hw_pwm_t *obj );

/**
 * Set the PWM status
 * Example code:
 *  #include "inx_pwm.h"
 *  ...
 *  inx_hw_pwm_t *pPwmObj = NULL;
 *  void func() {
 *      EhsTPwmSetup(0, 14, 4000, 8192, &pPwmObj);
 *      inx_hw_pwm_status status = {
 *          .freq = 3000,
 *          .duty = 1024,
 *          .enabled = EHS_TRUE
 *      };
 *      EHsTPwmSetStatus(pPwmObj, status);
 *      ...
 *  }
 */
ehs_sint32 EhsTPwmSetStatus(inx_hw_pwm_t *obj, inx_hw_pwm_status_t status);

/**
 * Get the PWM status
 * Example code:
 *  #include "hal_pwm.h"
 *  ...
 *  inx_hw_pwm_t *pPwmObj = NULL;
 *  void func() {
 *      EhsTPwmSetup(0, 14, 4000, 8192, &pPwmObj);
 *      inx_hw_pwm_status_t status = {
 *          .freq = 3000,
 *          .duty = 1024,
 *          .enabled = EHS_TRUE
 *      };
 *      EHsTPwmSetStatus(pPwmObj, status);
 *      inx_hw_pwm_status_t state1;
 *      EhsTPwmGetStatus(pPwmObj, &state1);
 *      ...
 *  }
 */
ehs_sint32 EhsTPwmGetStatus(inx_hw_pwm_t *obj, inx_hw_pwm_status_t *pStatus);

#endif//_HAL_PWM_H_ 