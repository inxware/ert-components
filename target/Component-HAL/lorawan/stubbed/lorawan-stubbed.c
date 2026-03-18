/***************************************************************
 * Copyright (C) 2008-2026 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license.
 ***************************************************************/

/**
 * @file lorawan-stubbed.c
 * @brief No-op stub for the LoRaWAN module HAL.
 *
 * Selected when EHS_LORAWAN_SUPPORT=stubbed in the platform config.
 * All functions return E_LWAPIERRNO_OK without performing any operation.
 * Use this for platforms where no LoRaWAN modem is physically present
 * but the LoRaWAN function block must still compile and link.
 */

#include "globals.h"
#include "lorawan-stubbed.h"

ehs_lorawan_api_errno_t LoRaWAN_wioe5_init( ehs_sint32 com_port )
{
    (void)com_port;
    return E_LWAPIERRNO_OK;
}

ehs_lorawan_api_errno_t LoRaWAN_wioe5_deinit( void )
{
    return E_LWAPIERRNO_OK;
}

ehs_lorawan_api_errno_t LoRaWAN_wioe5_connect(char *AppKey, char *AppEui, ehs_bool mode, char *DevAddr_ABP, char *AppSKey, char *NwkSKey, ehs_sint32 REPT, ehs_sint32 RETRY, e_ehs_lw_region_t region, ehs_bool ADR, ehs_sint32 DR, ehs_sint32 autoJoin, char *DevAddr_OUT, e_ehs_lw_class_t class_type, ehs_sint32 subband, ehs_float rxwin2_freq, ehs_sint32 rxwin2_dr, ehs_sint32 tx_power)
{
    (void)AppKey; (void)AppEui; (void)mode; (void)DevAddr_ABP;
    (void)AppSKey; (void)NwkSKey; (void)REPT; (void)RETRY;
    (void)region; (void)ADR; (void)DR; (void)autoJoin;
    (void)DevAddr_OUT; (void)class_type; (void)subband;
    (void)rxwin2_freq; (void)rxwin2_dr; (void)tx_power;
    return E_LWAPIERRNO_OK;
}

ehs_lorawan_api_errno_t LoRaWAN_wioe5_send_msg(char *payload, ehs_uint8 fport, ehs_bool confirmed)
{
    (void)payload; (void)fport; (void)confirmed;
    return E_LWAPIERRNO_OK;
}

ehs_lorawan_api_errno_t LoRaWAN_wioe5_reset( void )
{
    return E_LWAPIERRNO_OK;
}

ehs_lorawan_api_errno_t LoRaWAN_wioe5_get_sysData(char *data, char *DevEui)
{
    (void)data; (void)DevEui;
    return E_LWAPIERRNO_OK;
}

ehs_lorawan_api_errno_t LoRaWAN_wioe5_set_datarate(ehs_sint32 datarate)
{
    (void)datarate;
    return E_LWAPIERRNO_OK;
}

ehs_lorawan_api_errno_t LoRaWAN_wioe5_get_payloadLength(ehs_sint32 *length)
{
    (void)length;
    return E_LWAPIERRNO_OK;
}

ehs_lorawan_api_errno_t LoRaWAN_wioe5_disable( void )
{
    return E_LWAPIERRNO_OK;
}

ehs_lorawan_api_errno_t LoRaWAN_wioe5_set_class(e_ehs_lw_class_t class_type)
{
    (void)class_type;
    return E_LWAPIERRNO_OK;
}

ehs_lorawan_api_errno_t LoRaWAN_wioe5_set_txpower(ehs_sint32 tx_power)
{
    (void)tx_power;
    return E_LWAPIERRNO_OK;
}

ehs_lorawan_api_errno_t LoRaWAN_wioe5_link_check( void )
{
    return E_LWAPIERRNO_OK;
}

void Common_LoRaWAN_onReceive(char *recv_msg, ehs_bool has_message)
{
    (void)recv_msg; (void)has_message;
}
