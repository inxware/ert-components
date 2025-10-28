#ifndef _LORAWAN_WIO_E5_H_
#define _LORAWAN_WIO_E5_H_
#include "globals.h"
#include "lorawan.h"

ehs_lorawan_api_errno_t LoRaWAN_wioe5_init( void );

ehs_lorawan_api_errno_t LoRaWAN_wioe5_deinit( void );

ehs_lorawan_api_errno_t LoRaWAN_wioe5_connect(char *AppKey, char *AppEui, ehs_bool mode, char *DevAddr_ABP, char *AppSKey, char *NwkSKey, ehs_sint32 REPT, ehs_sint32 RETRY, e_ehs_lw_region_t region, ehs_bool ADR, ehs_sint32 DR, ehs_sint32 autoJoin, char *DevAddr_OUT);

ehs_lorawan_api_errno_t LoRaWAN_wioe5_send_msg(char *payload, ehs_uint8 fport, ehs_bool confirmed);

ehs_lorawan_api_errno_t LoRaWAN_wioe5_reset( void );

ehs_lorawan_api_errno_t LoRaWAN_wioe5_get_sysData(char *data, char *DevEui);

ehs_lorawan_api_errno_t LoRaWAN_wioe5_set_datarate(ehs_sint32 datarate);

ehs_lorawan_api_errno_t LoRaWAN_wioe5_get_payloadLength(ehs_sint32 *length);

ehs_lorawan_api_errno_t LoRaWAN_wioe5_disable( void );

void Common_LoRaWAN_onReceive(char *recv_msg, ehs_bool has_message);

#endif
