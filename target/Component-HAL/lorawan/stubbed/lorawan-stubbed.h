#ifndef _LORAWAN_STUBBED_H_
#define _LORAWAN_STUBBED_H_
#include "globals.h"
#include "lorawan.h"

/* Stubbed LoRaWAN module interface — all functions are no-ops.
 * Function names mirror the wio_e5 implementation so that lorawan.c compiles
 * unchanged regardless of which module directory is selected by the build.
 *
 * TODO: when lorawan.c is refactored to call common LoRaWAN_module_* names
 * (removing the switch-on-target dispatch), this header and the wio_e5 one
 * should be unified into a single lorawan_module.h with the common API.
 */

ehs_lorawan_api_errno_t LoRaWAN_wioe5_init( ehs_sint32 com_port );

ehs_lorawan_api_errno_t LoRaWAN_wioe5_deinit( void );

ehs_lorawan_api_errno_t LoRaWAN_wioe5_connect(char *AppKey, char *AppEui, ehs_bool mode, char *DevAddr_ABP, char *AppSKey, char *NwkSKey, ehs_sint32 REPT, ehs_sint32 RETRY, e_ehs_lw_region_t region, ehs_bool ADR, ehs_sint32 DR, ehs_sint32 autoJoin, char *DevAddr_OUT, e_ehs_lw_class_t class_type, ehs_sint32 subband, ehs_float rxwin2_freq, ehs_sint32 rxwin2_dr, ehs_sint32 tx_power);

ehs_lorawan_api_errno_t LoRaWAN_wioe5_send_msg(char *payload, ehs_uint8 fport, ehs_bool confirmed);

ehs_lorawan_api_errno_t LoRaWAN_wioe5_reset( void );

ehs_lorawan_api_errno_t LoRaWAN_wioe5_get_sysData(char *data, char *DevEui);

ehs_lorawan_api_errno_t LoRaWAN_wioe5_set_datarate(ehs_sint32 datarate);

ehs_lorawan_api_errno_t LoRaWAN_wioe5_get_payloadLength(ehs_sint32 *length);

ehs_lorawan_api_errno_t LoRaWAN_wioe5_disable( void );

ehs_lorawan_api_errno_t LoRaWAN_wioe5_set_class(e_ehs_lw_class_t class_type);

ehs_lorawan_api_errno_t LoRaWAN_wioe5_set_txpower(ehs_sint32 tx_power);

ehs_lorawan_api_errno_t LoRaWAN_wioe5_link_check( void );

void Common_LoRaWAN_onReceive(char *recv_msg, ehs_bool has_message);

#endif /* _LORAWAN_STUBBED_H_ */
