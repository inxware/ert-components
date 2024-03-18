#ifndef _LORAWAN_WIO_E5_H_
#define _LORAWAN_WIO_E5_H_
#include "target.h"
#include "lorawan.h"

int LoRaWAN_wioe5_init( void );

int LoRaWAN_wioe5_deinit( void );

int LoRaWAN_wioe5_connect(char *AppKey, char *AppEui, ehs_bool mode, char *DevAddr_ABP, char *AppSKey, char *NwkSKey, ehs_sint32 REPT, ehs_sint32 RETRY, char *region, char *DevAddr_OUT);

int LoRaWAN_wioe5_send_msg(char *payload, ehs_bool confirmed);

int LoRaWAN_wioe5_reset( void );

int LoRaWAN_wioe5_get_sysData(char *data, char *DevEui);

void Common_LoRaWAN_onReceive(char *recv_msg, ehs_bool has_message);

#endif
