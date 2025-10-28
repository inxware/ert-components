/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
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
#include "globals.h"
#include "callback_queue.h"

/*Define types*/

#define EHS_PID_CTRL_MAX_COUNT 3
#define EHS_PID_CTRL_MAX_INDEX (EHS_PID_CTRL_MAX_COUNT - 1)

typedef enum {
    PID_CTRL_NO_ERROR = 0,
    PID_CTRL_ADC_INIT_FAILED,
    PID_CTRL_GPIO_INIT_FAILED,
    PID_CTRL_GPIO_DESTROY_FAILED,
    PID_CTRL_BAD_PID_INDEX,
    PID_CTRL_INVALID_ELEMENT_MODE,
    PID_CTRL_INVALID_CTRL_OUTPUT_MODE,
    PID_CTRL_INVALID_CTRL_SENSOR_ID,
    PID_CTRL_INVALID_CTRL_SENSOR_TYPE,
    PID_CTRL_INVALID_HDWR_CONFIG_ID,
    PID_CTRL_INVALID_HDWR_CONFIG_VALUE,
    PID_CTRL_ERROR_COUNT
} ehs_pid_ctrl_error_id;

typedef enum {
    PID_CTRL_RELAY_TYPE_UNKOWN = 0,
    PID_CTRL_RELAY_TYPE_INTERNAL,
    PID_CTRL_RELAY_TYPE_EXTERNAL,
    PID_CTRL_RELAY_TYPE_COUNT
} ehs_pid_ctrl_relay_type_id;

typedef struct
{
    ehs_sint32 pid_no;
    ehs_bool ISR_mode;
    ehs_bool calib_mode;
    void* target_data;

    /* Inputs */
    volatile ehs_float setpointValue;
    volatile ehs_float measuredValue;
    ehs_float P;
    ehs_float I;
    ehs_float D;
    ehs_float maxGlobalValue;
    ehs_float maxIValue;

    ehs_sint32 nSensorID;
    ehs_sint32 nOutputMode;
    ehs_sint32 nChannelOutput1;
    ehs_sint32 nChannelOutput2;
    ehs_sint32 nChannelOutput3;
    ehs_bool bEnableRelayCheck;

    ehs_bool bDisableControl;

    /* Outputs */
    ehs_sint32 nOutputPercent;
    ehs_bool bPWMOut1;
    ehs_bool bPWMOut2;
    ehs_bool bPWMOut3;

    ehs_pid_ctrl_error_id nError;
} ehs_pid_ctrl_type;

ehs_bool EhsPIDCtrlInit(ehs_pid_ctrl_type* pid_ctrl);

ehs_bool EhsPIDCtrlDestroy(ehs_pid_ctrl_type* pid_ctrl);

ehs_bool EhsPIDCtrlConfigurePID(ehs_pid_ctrl_type* pid_ctrl);

ehs_bool EhsPIDCtrlConfigureIO(ehs_pid_ctrl_type* pid_ctrl);

ehs_bool EhsPIDCtrlSetSetpointValue(ehs_pid_ctrl_type* pid_ctrl);

ehs_bool EhsPIDCtrlSetMeasuredValue(ehs_pid_ctrl_type* pid_ctrl);

ehs_bool EhsPIDCtrlUpdateOutputs(ehs_pid_ctrl_type* pid_ctrl);

ehs_bool EhsPIDCtrlDisable(ehs_pid_ctrl_type* pid_ctrl);

ehs_float EhsPIDCtrlGetSensorValue(const ehs_pid_ctrl_type* pid_ctrl, ehs_bool* isConnected);

ehs_sint32 EhsPIDCtrlCalibrate(ehs_sint32 id, ehs_bool bScale, float scale, ehs_bool bOffset, float offset);

ehs_bool EhsPIDCtrlConfigureRelays(ehs_pid_ctrl_relay_type_id id, ehs_sint32 idx, ehs_sint32 rel, ehs_float cyc_len);

ehs_bool EhsPIDCtrlConfigureHardware(ehs_sint32 id, ehs_sint32 value, ehs_uint32* error_no);

#endif /* EHS_HAL_PID_H */