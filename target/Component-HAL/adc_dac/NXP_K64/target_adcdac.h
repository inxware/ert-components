/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

#ifndef EHS_TARGET_ADCCAD_H
#define EHS_TARGET_ADCCAD_H
#include "globals.h"

/* Asynchrounously reads a sample from the specified channel (with no averaging)
    returns true on success
*/



ehs_bool target_read_adc_sample(ehs_uint8 channel, ehs_float *value,
                                ehs_uint8 config);
ehs_bool configure_adc(ehs_uint8 channel, ehs_bool continuous, ehs_float f_s, ehs_sint32 num_samples, ehs_float bias, ehs_uint8 configuration,
                       ehs_uint8 *config);
/*
declare the adc init function needed to configure the cahnnel/adc.
pass in specific primitive values for now rather than a struct.
target_read_adc_init(....);
*/

ehs_bool destroy_adc(ehs_uint8 channel);

/***************************** DAC START *****************************/

// DAC macro definitions
#ifndef EHS_TARGET_DAC_NUMBER
#define EHS_TARGET_DAC_NUMBER 100
#endif

ehs_bool EhsTDacConfigure(ehs_uint8 channel, ehs_sint32 max, ehs_sint32 sample_rate);
ehs_bool EhsTDacSet(ehs_uint8 channel, ehs_sint32 value);
ehs_bool EhsTDacDestroy(ehs_uint8 channel);

/***************************** DAC ENDED *****************************/


#endif
