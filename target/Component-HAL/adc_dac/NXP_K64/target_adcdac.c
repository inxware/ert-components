/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/
/* TODO2024
ADC DACs are currently hard-configured for specific products for NXP 
so there is no configuration passed in from the function blocks,
nor are there 
*/

#include "globals.h"
#include "target_adcdac.h"

ehs_uint8 unit = 0;

static ehs_bool set_ADC_unit(ehs_uint8 value)
{
    return EHS_TRUE;
}

static ehs_bool set_ADC_width(ehs_uint8 width)
{
    return EHS_TRUE;
}

static ehs_bool get_ADC_width(ehs_uint8 *width)
{
    return EHS_TRUE;
}

static ehs_bool set_atten(ehs_uint8 channel, ehs_uint8 atten)
{
    return EHS_TRUE;
}

// For ADC1, configure desired precision and attenuation by calling functions
static ehs_bool configure_ADC1(ehs_uint8 channel)
{
    return EHS_TRUE;
}

// Configure the ADC2 channel, including setting attenuation.
static ehs_bool configure_ADC2(ehs_uint8 channel)
{
	return EHS_TRUE;
}

ehs_bool legacy_configure_adc(ehs_uint8 channel, ehs_bool continuous, ehs_float f_s, ehs_sint32 num_samples, ehs_float bias, ehs_uint8 configure,
                                  ehs_uint8 *config)
{
    *config=configure;// something random for now... not sure what this is supposed to do...
    return EHS_TRUE;
}

ehs_bool legacy_target_read_adc_sample(ehs_uint8 channel, ehs_float *value,
        ehs_uint8 config)
{
    return EHS_TRUE;
}

ehs_bool legacy_destroy_adc(ehs_uint8 channel)
{
    return EHS_TRUE;
}

/***************************** DAC START *****************************/

ehs_bool EhsTDacConfigure(ehs_uint8 channel, ehs_sint32 max, ehs_sint32 sample_rate)
{
    return EHS_TRUE;
}

ehs_bool EhsTDacSet(ehs_uint8 channel, ehs_sint32 value)
{
    return EHS_TRUE;
}

ehs_bool EhsTDacDestroy(ehs_uint8 channel)
{
    return EHS_TRUE;
}

/***************************** DAC ENDED *****************************/


