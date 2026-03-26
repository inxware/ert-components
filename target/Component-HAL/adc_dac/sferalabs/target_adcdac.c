/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/**
 * @file target_adcdac.c
 * @brief Sfera Labs Analog I/O HAL — reads/writes via sysfs only.
 *
 * No Sfera Labs kernel headers or libraries are used.
 * See target_adcdac.h for the ADC channel mapping conventions.
 */

#include "globals.h"
#include "target_adcdac.h"
#include "hal_logger.h"

/* Required by the common adc_dac header */
ehs_adc_config_t g_ehs_adc_configs[EHS_TARGET_ADC_UNIT_NUMBER] = EHS_DEFAULT_ADC_CONFIG();
ehs_uint16 g_ehs_adc_continuous_enabled_bitmask[EHS_TARGET_ADC_UNIT_NUMBER] = EHS_TARGET_ADC_UNIT_DEFAULT(0);

/* -------------------------------------------------------------------------
 * Internal helpers — build sysfs paths for the selected board
 * ------------------------------------------------------------------------- */

/**
 * Resolve a logical ADC channel index to a sysfs read path and a scale factor.
 *
 * @param channel   0-based ADC channel number (see header for mapping)
 * @param path_buf  Output buffer for the sysfs file path
 * @param scale     Output: divide raw integer by this to get the SI float value
 * @return EHS_TRUE on success, EHS_FALSE if channel is out of range for this board
 */
static ehs_bool resolve_adc_path(ehs_uint8 channel, char *path_buf, float *scale)
{
#if defined(EHS_SFERALABS_BOARD_IONOPI)
    /* ai1-ai6 in millivolts — no scale needed */
    if (channel < SFERALABS_AI_VOLT_CHANNELS)
    {
        snprintf(path_buf, SFERALABS_SYSFS_BUF_SIZE, SFERALABS_AI_VOLT_FMT, (int)channel + 1);
        *scale = 1.0f;
        return EHS_TRUE;
    }
    EHSH_LOG_ERROR("SferaLabs ADC: channel %d out of range for IONOPI (max %d)",
                   (int)channel, SFERALABS_AI_VOLT_CHANNELS - 1);
    return EHS_FALSE;

#elif defined(EHS_SFERALABS_BOARD_IONOPIMAX)
    if (channel < SFERALABS_AI_VOLT_CHANNELS)
    {
        /* av1-av4: value is mV*100, divide by 100 to get mV */
        snprintf(path_buf, SFERALABS_SYSFS_BUF_SIZE, SFERALABS_AI_VOLT_FMT, (int)channel + 1);
        *scale = 100.0f;
        return EHS_TRUE;
    }
    int curr_ch = (int)channel - SFERALABS_AI_VOLT_CHANNELS;
    if (curr_ch >= 0 && curr_ch < SFERALABS_AI_CURR_CHANNELS)
    {
        /* ai1-ai4: value in µA */
        snprintf(path_buf, SFERALABS_SYSFS_BUF_SIZE, SFERALABS_AI_CURR_FMT, curr_ch + 1);
        *scale = 1.0f;
        return EHS_TRUE;
    }
    int temp_ch = (int)channel - SFERALABS_AI_VOLT_CHANNELS - SFERALABS_AI_CURR_CHANNELS;
    if (temp_ch >= 0 && temp_ch < SFERALABS_AI_TEMP_CHANNELS)
    {
        /* at1-at2: value is °C*100, divide by 100 to get °C */
        snprintf(path_buf, SFERALABS_SYSFS_BUF_SIZE, SFERALABS_AI_TEMP_FMT, temp_ch + 1);
        *scale = 100.0f;
        return EHS_TRUE;
    }
    EHSH_LOG_ERROR("SferaLabs ADC: channel %d out of range for IONOPIMAX (volt<4, curr<8, temp<10)",
                   (int)channel);
    return EHS_FALSE;

#elif defined(EHS_SFERALABS_BOARD_STRATOPIMAX)
    /* Slot encoded in upper byte of channel; lower byte is channel within slot */
    int slot    = ((int)channel >> 4) & 0x0F;   /* upper nibble = slot 1-4 */
    int slot_ch = (int)channel & 0x0F;           /* lower nibble = ch within slot 1-4 */
    if (slot < 1 || slot > 4)
    {
        EHSH_LOG_ERROR("SferaLabs ADC: channel=0x%02x invalid slot=%d (expected 1-4)",
                       (int)channel, slot);
        return EHS_FALSE;
    }
    if (slot_ch < 1 || slot_ch > SFERALABS_AI_VOLT_CHANNELS)
    {
        EHSH_LOG_ERROR("SferaLabs ADC: channel=0x%02x invalid slot_ch=%d (expected 1-%d)",
                       (int)channel, slot_ch, SFERALABS_AI_VOLT_CHANNELS);
        return EHS_FALSE;
    }
    snprintf(path_buf, SFERALABS_SYSFS_BUF_SIZE, SFERALABS_AI_VOLT_FMT, slot, slot_ch);
    *scale = 100.0f;
    return EHS_TRUE;

#else
    (void)channel; (void)path_buf; (void)scale;
    EHSH_LOG_ERROR("SferaLabs ADC: no board variant defined (EHS_SFERALABS_BOARD_* not set)");
    return EHS_FALSE;
#endif
}

/**
 * Build the sysfs path for an analog output channel (DAC).
 * @param channel  0-based DAC channel (0 = ao1, 1 = ao2)
 */
static ehs_bool resolve_dac_path(ehs_uint8 channel, char *path_buf)
{
#if SFERALABS_AO_CHANNELS > 0
    if (channel >= SFERALABS_AO_CHANNELS)
    {
        EHSH_LOG_ERROR("SferaLabs DAC: channel %d out of range (max %d)", channel, SFERALABS_AO_CHANNELS);
        return EHS_FALSE;
    }
#if defined(EHS_SFERALABS_BOARD_STRATOPIMAX)
    /* Slot from upper nibble, channel from lower */
    int slot    = ((int)channel >> 4) & 0x0F;
    int slot_ch = (int)channel & 0x0F;
    snprintf(path_buf, SFERALABS_SYSFS_BUF_SIZE, SFERALABS_AO_FMT, slot, slot_ch);
#else
    snprintf(path_buf, SFERALABS_SYSFS_BUF_SIZE, SFERALABS_AO_FMT, (int)channel + 1);
#endif
    return EHS_TRUE;
#else
    (void)channel; (void)path_buf;
    EHSH_LOG_ERROR("SferaLabs DAC: no analog outputs on this board");
    return EHS_FALSE;
#endif
}

/* -------------------------------------------------------------------------
 * Configure ADC input mode (Iono Pi Max / Strato Pi Max only)
 * ------------------------------------------------------------------------- */

static void configure_adc_mode(ehs_uint8 channel, ehs_uint8 configuration)
{
#if defined(EHS_SFERALABS_BOARD_IONOPIMAX)
    char mode_path[SFERALABS_SYSFS_BUF_SIZE];
    const char *mode_str = "U"; /* default: unipolar */

    if (channel < SFERALABS_AI_VOLT_CHANNELS)
    {
        snprintf(mode_path, SFERALABS_SYSFS_BUF_SIZE, SFERALABS_AI_VOLT_MODE_FMT, (int)channel + 1);
        mode_str = (configuration == SFERALABS_ADC_MODE_VOLT_BI) ? "B" : "U";
    }
    else
    {
        int curr_ch = (int)channel - SFERALABS_AI_VOLT_CHANNELS;
        if (curr_ch >= 0 && curr_ch < SFERALABS_AI_CURR_CHANNELS)
        {
            snprintf(mode_path, SFERALABS_SYSFS_BUF_SIZE, SFERALABS_AI_CURR_MODE_FMT, curr_ch + 1);
            mode_str = (configuration == SFERALABS_ADC_MODE_CURR_BI) ? "B" : "U";
        }
        else
        {
            int temp_ch = (int)channel - SFERALABS_AI_VOLT_CHANNELS - SFERALABS_AI_CURR_CHANNELS;
            if (temp_ch >= 0 && temp_ch < SFERALABS_AI_TEMP_CHANNELS)
            {
                snprintf(mode_path, SFERALABS_SYSFS_BUF_SIZE, SFERALABS_AI_TEMP_MODE_FMT, temp_ch + 1);
                mode_str = (configuration == SFERALABS_ADC_MODE_TEMP_PT1000) ? "2" : "1";
            }
            else
            {
                return; /* out of range */
            }
        }
    }
    if (sferalabs_sysfs_write(mode_path, mode_str, 1) != 0)
        EHSH_LOG_ERROR("SferaLabs ADC: failed to set mode for channel %d", channel);
#else
    (void)channel; (void)configuration;
#endif
}

/* -------------------------------------------------------------------------
 * Public ADC API
 * ------------------------------------------------------------------------- */

EHS_GLOBAL ehs_bool legacy_configure_adc(ehs_uint8 channel, ehs_bool continuous,
                                   ehs_float f_s, ehs_sint32 num_samples,
                                   ehs_float bias, ehs_uint8 configuration,
                                   ehs_uint8 *config)
{
    (void)continuous; (void)f_s; (void)num_samples; (void)bias;

    char path[SFERALABS_SYSFS_BUF_SIZE];
    float scale;
    if (!resolve_adc_path(channel, path, &scale))
    {
        EHSH_LOG_ERROR("SferaLabs ADC: channel %d not available on this board", channel);
        return EHS_FALSE;
    }

    configure_adc_mode(channel, configuration);

    if (config) *config = configuration;
    EHSH_LOG_INFO("SferaLabs ADC: configured channel %d -> %s", channel, path);
    return EHS_TRUE;
}

EHS_GLOBAL ehs_bool legacy_target_read_adc_sample(ehs_uint8 channel, ehs_float *value, ehs_uint8 config)
{
    (void)config;
    char path[SFERALABS_SYSFS_BUF_SIZE];
    float scale;
    if (!resolve_adc_path(channel, path, &scale))
    {
        EHSH_LOG_ERROR("SferaLabs ADC: channel %d not available on this board", channel);
        *value = 0.0f;
        return EHS_FALSE;
    }

    long raw;
    if (sferalabs_sysfs_read_int(path, &raw) != 0)
    {
        EHSH_LOG_ERROR("SferaLabs ADC: failed to read channel %d from %s: errno=%d (%s)",
                       channel, path, errno, strerror(errno));
        *value = 0.0f;
        return EHS_FALSE;
    }

    *value = (float)raw / scale;
    return EHS_TRUE;
}

EHS_GLOBAL ehs_bool legacy_destroy_adc(ehs_uint8 channel)
{
    (void)channel;
    return EHS_TRUE;
}

EHS_GLOBAL ehs_sint32 EhsTAdcUnitConfigure(ehs_uint8 unit)
{
    /* Validate every channel the adc_config block has mapped (non-negative
     * values in unit_config.channel[]).  Two checks per channel:
     *   1. resolve_adc_path — confirms the index is in range for this board
     *      variant.  Returns EHS_ADC_ERR_INVALID_CHANNEL if not.
     *   2. sferalabs_sysfs_read_int probe — confirms the sysfs node exists and
     *      is readable.  A failure here means the kernel module is not loaded
     *      or the hardware is absent.  Returns EHS_ADC_ERR_HAL_INIT_FAILED.
     */
    ehs_bool any_channel = EHS_FALSE;
    for (int i = 0; i < EHS_TARGET_ADC_CHANNEL_NUMBER; i++)
    {
        ehs_sint16 ch_val = g_ehs_adc_configs[unit].unit_config.channel[i];
        if (ch_val < 0) continue;   /* -1 = channel not in use */
        any_channel = EHS_TRUE;

        char path[SFERALABS_SYSFS_BUF_SIZE];
        float scale;
        if (!resolve_adc_path((ehs_uint8)ch_val, path, &scale))
        {
            EHSH_LOG_ERROR("SferaLabs ADC: channel %d not available on this board", (int)ch_val);
            return EHS_ADC_ERR_INVALID_CHANNEL;
        }
        long dummy;
        if (sferalabs_sysfs_read_int(path, &dummy) != 0)
        {
            EHSH_LOG_ERROR("SferaLabs ADC: sysfs node not readable for channel %d (%s) - kernel module loaded?",
                           (int)ch_val, path);
            return EHS_ADC_ERR_HAL_INIT_FAILED;
        }
    }
    if (!any_channel)
        EHSH_LOG_INFO("SferaLabs ADC: unit %d configured with no active channels", (int)unit);
    return EHS_ADC_ERR_NONE;
}

EHS_GLOBAL ehs_uint32 EhsTAdcChannelSingleRead(ehs_uint8 unit, ehs_uint8 channel)
{
    ehs_float value = 0.0f;
    ehs_bool ok = legacy_target_read_adc_sample(channel, &value, 0);
    if (!ok)
        EHSH_LOG_ERROR("SferaLabs ADC: single read failed for unit=%d channel=%d", (int)unit, (int)channel);
    return (ehs_uint32)value;
}

EHS_GLOBAL ehs_bool EhsTAdcUnitDestroy(ehs_uint8 unit)
{
    (void)unit;
    return EHS_TRUE;
}

/* Weak default — the calling component (function block, PID, etc.) must define this */
EHS_MEMORY_ATTRIB __attribute__((weak))
void EhsTHAdcChannelContinuousConvertCB(ehs_uint8 unit, ehs_uint8 channel,
                                         ehs_sint32 mean, ehs_uint32 variance,
                                         ehs_uint32 mean_squared)
{
    (void)unit; (void)channel; (void)mean; (void)variance; (void)mean_squared;
}

/* -------------------------------------------------------------------------
 * Public DAC API
 * ------------------------------------------------------------------------- */

EHS_GLOBAL ehs_bool EhsTDacConfigure(ehs_uint8 channel, ehs_sint32 max, ehs_sint32 sample_rate)
{
    (void)max; (void)sample_rate;
    char path[SFERALABS_SYSFS_BUF_SIZE];
    if (!resolve_dac_path(channel, path))
        return EHS_FALSE;

#if defined(SFERALABS_AO_MODE_FMT)
    /* Default to voltage mode */
    char mode_path[SFERALABS_SYSFS_BUF_SIZE];
#if defined(EHS_SFERALABS_BOARD_STRATOPIMAX)
    int slot    = ((int)channel >> 4) & 0x0F;
    int slot_ch = (int)channel & 0x0F;
    snprintf(mode_path, SFERALABS_SYSFS_BUF_SIZE, SFERALABS_AO_MODE_FMT, slot, slot_ch);
#else
    snprintf(mode_path, SFERALABS_SYSFS_BUF_SIZE, SFERALABS_AO_MODE_FMT, (int)channel + 1);
#endif
    sferalabs_sysfs_write(mode_path, "V", 1);
#endif

    EHSH_LOG_INFO("SferaLabs DAC: configured channel %d -> %s", channel, path);
    return EHS_TRUE;
}

EHS_GLOBAL ehs_bool EhsTDacSet(ehs_uint8 channel, ehs_sint32 value)
{
    char path[SFERALABS_SYSFS_BUF_SIZE];
    if (!resolve_dac_path(channel, path))
        return EHS_FALSE;

    if (sferalabs_sysfs_write_int(path, (long)value) != 0)
    {
        EHSH_LOG_ERROR("SferaLabs DAC: failed to set channel %d = %d", channel, value);
        return EHS_FALSE;
    }
    return EHS_TRUE;
}

EHS_GLOBAL ehs_bool EhsTDacDestroy(ehs_uint8 channel)
{
    /* Zero the output on destroy for safety */
    char path[SFERALABS_SYSFS_BUF_SIZE];
    if (resolve_dac_path(channel, path))
        sferalabs_sysfs_write_int(path, 0L);
    return EHS_TRUE;
}
