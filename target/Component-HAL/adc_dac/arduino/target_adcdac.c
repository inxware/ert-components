
#include "globals.h"
#include "Arduino.h"
#include "target_adcdac.h"
#include "target_logger.h"


/***************************** ADC START *****************************/

/* ! Arduino seems to have memory corruption issue when creating large static buffers ??? */
ehs_adc_config_t* g_ehs_adc_configs = NULL;
ehs_uint16* g_ehs_adc_continuous_enabled_bitmask = NULL;

void EhsTgtAdcDacInit()
{
    if(g_ehs_adc_configs == NULL){
        //EhsStdioPrintf("Initalise ADC config units=%d channels=%d\n",(int)EHS_TARGET_ADC_UNIT_NUMBER,(int)EHS_TARGET_ADC_CHANNEL_NUMBER);
        //todo2024 - we need to use the temp ehs malloc here...
        g_ehs_adc_configs = (ehs_adc_config_t*)malloc(sizeof(ehs_adc_config_t) * EHS_TARGET_ADC_UNIT_NUMBER);
        for(int i = 0; i < EHS_TARGET_ADC_UNIT_NUMBER; i++){
            g_ehs_adc_configs[i].unit_config = EHS_DEFAULT_ADC_UNIT_CONFIG();
            for(int j = 0; j < EHS_TARGET_ADC_CHANNEL_NUMBER; j++){
                g_ehs_adc_configs[i].channel_configs[j] = EHS_DEFAULT_ADC_CHANNEL_CONFIG();
            }
        }
    }
    if(g_ehs_adc_continuous_enabled_bitmask == NULL){
        // @TODO - make sure to initalise this when using with the continous mode
    }
}

// A0 - A3 or A0 - A7
ehs_uint32 readArduinoAdc(ehs_sint16 pin)
{
    ehs_uint32 value = 0;
    switch (pin)
    {
        case 0:{
            value = (ehs_uint32)analogRead(A0);
            break;
        }
        case 1:{
            value = (ehs_uint32)analogRead(A1);
            break;
        }
        case 2:{
            value = (ehs_uint32)analogRead(A2);
            break;
        }
        case 3:{
            value = (ehs_uint32)analogRead(A3);
            break;
        }
#if EHS_TARGET_ADC_CHANNEL_NUMBER == 8
        case 4:{
            value = (ehs_uint32)analogRead(A4);
            break;
        }
        case 5:{
            value = (ehs_uint32)analogRead(A5);
            break;
        }
        case 6:{
            value = (ehs_uint32)analogRead(A6);
            break;
        }
        case 7:{
            value = (ehs_uint32)analogRead(A7);
            break;
        }
#endif
        default:{
            break;
        }
    }
    return value;
}

// A0 - An
bool isArduinoADCPinValid(int pin) 
{
    return (pin >= 0 && pin <= EHS_TARGET_ADC_CHANNEL_NUMBER-1);
}

ehs_bool legacy_target_read_adc_sample(ehs_uint8 channel, ehs_float *value, ehs_uint8 config)
{
    return EHS_TRUE;
}

ehs_bool legacy_configure_adc(ehs_uint8 channel, ehs_bool continuous, ehs_float f_s, ehs_sint32 num_samples, 
                       ehs_float bias, ehs_uint8 configuration, ehs_uint8 *config)
{
    return EHS_TRUE;
}

ehs_bool legacy_destroy_adc(ehs_uint8 channel)
{
    return EHS_TRUE;
}

ehs_sint32 EhsTAdcUnitConfigure(ehs_uint8 unit)
{
    // Total supported unit count is 2
    if (unit >= EHS_TARGET_ADC_UNIT_NUMBER) return EHS_ADC_ERR_INVALID_DEVICE;
    if (g_ehs_adc_configs[unit].unit_config.init == 1) return EHS_ADC_ERR_ALREADY_INIT;
    switch (g_ehs_adc_configs[unit].unit_config.mode) {
        case 0: // Single-Shot
        {
            // Single Shot Configuration
            for (int i = 0 ; i < EHS_TARGET_ADC_CHANNEL_NUMBER ; i++)
            {
                if (g_ehs_adc_configs[unit].unit_config.channel[i] != -1)
                {
                    //EhsStdioPrintf("Unit %d Channel %d | Single Shot\n", (int)unit, i);
                    if(isArduinoADCPinValid(g_ehs_adc_configs[unit].unit_config.channel[i]) == false){
                        EhsStdioPrintf("ADC Error Single Shot | Unit %d Channel %d | Invalid Pin (%d)\n", (int)unit, i, g_ehs_adc_configs[unit].unit_config.channel[i]);
                        return EHS_ADC_ERR_INVALID_CHANNEL;
                    }
                }
            }
            break;
        }
        case 1: // Continuous
        {
            // Continuous Configuration
            // @TODO
            EhsStdioPrintf("Continuous Configuration N/A\n");
            // ADC continuous is not available atm
            return EHS_ADC_ERR_HAL_INIT_FAILED;
            //break;
        }
        default:
            return EHS_ADC_ERR_HAL_INIT_FAILED;
    }
    g_ehs_adc_configs[unit].unit_config.init = 1;
    return EHS_ADC_ERR_NONE;
}

ehs_uint32 EhsTAdcChannelSingleRead(ehs_uint8 unit, ehs_uint8 channel)
{
    // Total supported unit count is 2
    if (unit >= EHS_TARGET_ADC_UNIT_NUMBER) return EHS_FALSE;
    // This ADC unit is not initialised
    if (g_ehs_adc_configs[unit].unit_config.init == 0) return 0;
    // The mode of this ADC is not Single-shot
    if (g_ehs_adc_configs[unit].unit_config.mode != 0) return 0;
    // This ADC channel is not configured
    if (g_ehs_adc_configs[unit].unit_config.channel[channel] == -1) return 0;
    ehs_uint32 value = 0;

    value = readArduinoAdc(g_ehs_adc_configs[unit].unit_config.channel[channel]);
    //EhsStdioPrintf("EhsTAdcChannelSingleRead %d %d %d %d\n", (int)unit, (int)channel, g_ehs_adc_configs[unit].unit_config.channel[channel], value);

    return value;
    
}

ehs_bool EhsTAdcUnitDestroy(ehs_uint8 unit)
{
    // Total supported unit count is 2
    if (unit >= EHS_TARGET_ADC_UNIT_NUMBER) return EHS_FALSE;
    if (g_ehs_adc_configs[unit].unit_config.init == 0) return EHS_FALSE;

    return EHS_TRUE;
}

/***************************** ADC ENDED *****************************/

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
