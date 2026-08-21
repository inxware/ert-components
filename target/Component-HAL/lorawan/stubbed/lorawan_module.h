#ifndef _LORAWAN_MODULE_H_
#define _LORAWAN_MODULE_H_

/* Generic LoRaWAN module API, stubbed flavour.
 * All functions are no-ops returning E_LWAPIERRNO_OK — use this when the
 * LoRaWAN function block must link but no physical modem is present. */

#include "globals.h"
#include "lorawan.h"

ehs_lorawan_api_errno_t LoRaWAN_module_init( ehs_sint32 com_port );

ehs_lorawan_api_errno_t LoRaWAN_module_deinit( void );

ehs_lorawan_api_errno_t LoRaWAN_module_connect(char *AppKey, char *AppEui, ehs_bool mode, char *DevAddr_ABP, char *AppSKey, char *NwkSKey, ehs_sint32 REPT, ehs_sint32 RETRY, e_ehs_lw_region_t region, ehs_bool ADR, ehs_sint32 DR, ehs_sint32 autoJoin, char *DevAddr_OUT, e_ehs_lw_class_t class_type, ehs_sint32 subband, ehs_float rxwin2_freq, ehs_sint32 rxwin2_dr, ehs_sint32 tx_power);

ehs_lorawan_api_errno_t LoRaWAN_module_send_msg(char *payload, ehs_uint8 fport, ehs_bool confirmed);

ehs_lorawan_api_errno_t LoRaWAN_module_reset( void );

ehs_lorawan_api_errno_t LoRaWAN_module_get_sysData(char *data, char *DevEui);

ehs_lorawan_api_errno_t LoRaWAN_module_set_datarate(ehs_sint32 datarate);

ehs_lorawan_api_errno_t LoRaWAN_module_get_payloadLength(ehs_sint32 *length);

ehs_lorawan_api_errno_t LoRaWAN_module_disable( void );

ehs_lorawan_api_errno_t LoRaWAN_module_set_class(e_ehs_lw_class_t class_type);

ehs_lorawan_api_errno_t LoRaWAN_module_set_txpower(ehs_sint32 tx_power);

ehs_lorawan_api_errno_t LoRaWAN_module_link_check( void );

/* Synchronous accessors for console "?l" — see rak3112/lorawan_module.h. */
void LoRaWAN_module_peekDevEui(ehs_uint8 out_8[8]);
ehs_bool LoRaWAN_module_peekJoined(void);
ehs_uint32 LoRaWAN_module_peekDevAddr(void);

void Common_LoRaWAN_onReceive(char *recv_msg, ehs_bool has_message);

#endif /* _LORAWAN_MODULE_H_ */
