/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

/** @file hal_pid.h.
 *
 * @author: inx limited
 *
 */


#ifndef EHS_HAL_PID_H
#define EHS_HAL_PID_H

/*****************************************************************************/
/* Included files */
#include "ehs_types.h"
#include "callback_queue.h"

/*Define types*/

#define EHS_PID_CTRL_MAX_COUNT 3
#define EHS_PID_CTRL_MAX_INDEX (EHS_PID_CTRL_MAX_COUNT - 1)

typedef enum {
    PID_CTRL_NO_ERROR = 0,
    PID_CTRL_ADC_INIT_FAILED,
    PID_CTRL_GPIO_INIT_FAILED,
    PID_CTRL_BAD_PID_INDEX,
    PID_CTRL_INVALID_ELEMENT_MODE,
    PID_CTRL_INVALID_CTRL_OUTPUT_MODE,
    PID_CTRL_INVALID_CTRL_SENSOR_ID,
    PID_CTRL_ERROR_COUNT
} ehs_pid_ctrl_error_id;

typedef struct
{
    ehs_sint32 pid_no;
    ehs_bool ISR_mode;
    EhsCallbackQueueType* pCallback;
    void* target_data;

    /* Inputs */
    volatile ehs_sint32 nSetValue;
    volatile ehs_sint32 nMeasuredValue;
    ehs_float P;
    ehs_float I;
    ehs_float D;
    ehs_bool bEnableRelayCheck;
    ehs_sint32 nMaxGlobalValue;
    ehs_sint32 nMaxIValue;

    ehs_sint32 nChannelADC;
    ehs_sint32 nChannelGPIOOut1;
    ehs_sint32 nChannelGPIOOut2;
    ehs_sint32 nChannelGPIOOut3;

    ehs_sint32 nOutputMode;
    ehs_bool bEnableControl;

    /* Outputs */
    volatile ehs_sint32 nOutputPercent;
    volatile ehs_bool bPWMOut1;
    volatile ehs_bool bPWMOut2;
    volatile ehs_bool bPWMOut3;
    ehs_pid_ctrl_error_id nError;
} ehs_pid_ctrl_type;

EHS_GLOBAL ehs_bool EhsInitPIDCtrl(ehs_pid_ctrl_type* pid_ctrl);

EHS_GLOBAL ehs_bool EhsDestroyPIDCtrl(ehs_pid_ctrl_type* pid_ctrl);

EHS_GLOBAL ehs_bool EhsConfigurePIDCtrl(ehs_pid_ctrl_type* pid_ctrl);

EHS_GLOBAL ehs_bool EhsSetMeasuredValuePIDCtrl(ehs_pid_ctrl_type* pid_ctrl);

EHS_GLOBAL ehs_sint32 EhsGetAdcValuePIDCtrl(const ehs_pid_ctrl_type* pid_ctrl);

#endif /* EHS_HAL_PID_H */