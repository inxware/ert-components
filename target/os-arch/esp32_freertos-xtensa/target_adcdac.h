/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
 ***************************************************************/

#ifndef EHS_TARGET_ADCCAD_H
#define EHS_TARGET_ADCCAD_H
#include "ehs_types.h"

/* Asynchrounously reads a sample from the specified channel (with no averaging)
    returns true on success
*/

#define ADC1 1
#define ADC2 2

ehs_bool target_read_adc_sample(ehs_uint8 channel, ehs_float *value,
                                ehs_uint8 config);
ehs_bool configure_adc(ehs_uint8 channel, ehs_uint8 configuration,
                       ehs_uint8 *config);
/*
declare the adc init function needed to configure the cahnnel/adc.
pass in specific primitive values for now rather than a struct.
target_read_adc_init(....);
*/
#endif
