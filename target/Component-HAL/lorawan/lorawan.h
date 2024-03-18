#ifndef _LORAWAN_H_
#define _LORAWAN_H_
#include "globals.h"

#ifndef LORA_UART_PORT
#define LORA_UART_PORT 0
#endif

int LoRaWAN_init( ehs_sint32 target );

int LoRaWAN_deinit( void );

int LoRaWAN_connect(char *AppKey, char *AppEui, ehs_bool mode, char *DevAddr_ABP, char *AppSKey, char *NwkSKey, ehs_sint32 REPT, ehs_sint32 RETRY, char *region, char *DevAddr_OUT);

int LoRaWAN_send_msg(char *payload, ehs_bool confirmed);

int LoRaWAN_reset( void );

int LoRaWAN_get_sysData(char *sysData, char *DevAddr);

void Common_LoRaWAN_onReceive(char *recv_msg, ehs_bool has_message);

#endif
