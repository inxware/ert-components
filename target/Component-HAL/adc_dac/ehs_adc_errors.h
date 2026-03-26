/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/**
 * @file ehs_adc_errors.h
 * @brief ADC configuration error codes shared between the adc_config function
 *        block and all platform HAL implementations.
 *
 * EhsTAdcUnitConfigure() returns ehs_sint32.
 * Zero (EHS_ADC_ERR_NONE) means success; all negative values are errors.
 * These codes are exposed on the adc_config block's error_id output port so
 * that an application can distinguish the failure reason.
 *
 * Code  Meaning
 * ----  -------
 *    0  Success
 *   -1  Platform does not support adc_config (g_ehs_adc_configs unavailable)
 *   -2  device_id is out of range for this HAL (too many ADC units requested)
 *   -3  ADC unit is already initialised — call adc_config only once per unit
 *   -4  Sampling frequency is outside the [EHS_TARGET_ADC_FREQ_MIN,
 *       EHS_TARGET_ADC_FREQ_MAX] range for this platform
 *   -5  One or more configured channel indices are not available on this board
 *       (e.g. channel number exceeds the physical channel count, or an ADC pin
 *       is not valid for the MCU package in use)
 *   -6  HAL / hardware initialisation failed for a channel that is in range —
 *       e.g. a Sfera Labs sysfs node does not exist (kernel module not loaded),
 *       or an MCU ADC peripheral could not be opened for a configured pin
 */

#ifndef EHS_ADC_ERRORS_H
#define EHS_ADC_ERRORS_H

typedef enum ehs_adc_err
{
    EHS_ADC_ERR_NONE            =  0,  /**< success                                      */
    EHS_ADC_ERR_NOT_SUPPORTED   = -1,  /**< platform does not support adc_config          */
    EHS_ADC_ERR_INVALID_DEVICE  = -2,  /**< device_id out of range                        */
    EHS_ADC_ERR_ALREADY_INIT    = -3,  /**< ADC unit already initialised                  */
    EHS_ADC_ERR_FREQ_RANGE      = -4,  /**< sampling frequency outside [MIN, MAX]          */
    EHS_ADC_ERR_INVALID_CHANNEL = -5,  /**< channel not available on this board / pin     */
    EHS_ADC_ERR_HAL_INIT_FAILED = -6,  /**< sysfs node missing or MCU peripheral error    */
} ehs_adc_err_t;

#endif /* EHS_ADC_ERRORS_H */
