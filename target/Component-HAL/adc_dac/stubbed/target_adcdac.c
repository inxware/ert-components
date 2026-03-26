/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/
#include "globals.h"
#include "target_adcdac.h"

ehs_adc_config_t g_ehs_adc_configs[EHS_TARGET_ADC_UNIT_NUMBER] = EHS_DEFAULT_ADC_CONFIG();

ehs_uint16 g_ehs_adc_continuous_enabled_bitmask[EHS_TARGET_ADC_UNIT_NUMBER] = EHS_TARGET_ADC_UNIT_DEFAULT(0);

// todo - what is the *config needed for in generic code?
EHS_GLOBAL ehs_bool legacy_configure_adc(ehs_uint8 channel, ehs_bool continuous, ehs_float f_s, ehs_sint32 num_samples, ehs_float bias, ehs_uint8 configure, ehs_uint8 *config)
{
    return EHS_TRUE;
}

EHS_GLOBAL ehs_bool legacy_target_read_adc_sample(ehs_uint8 channel, ehs_float *value, ehs_uint8 config)
{
    *value = 0.0;
    return EHS_TRUE;
}

EHS_GLOBAL ehs_bool legacy_destroy_adc(ehs_uint8 channel)
{
    return EHS_TRUE;
}

ehs_sint32 EhsTAdcUnitConfigure(ehs_uint8 unit)
{
    (void)unit;
    return EHS_ADC_ERR_NONE;
}

ehs_uint32 EhsTAdcChannelSingleRead(ehs_uint8 unit, ehs_uint8 channel)
{
    return 0;
}

ehs_bool EhsTAdcUnitDestroy(ehs_uint8 unit)
{
    return EHS_TRUE;
}

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