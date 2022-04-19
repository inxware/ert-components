/*
 * app_dhcp.h
 *
 *  Created on: 26.04.2017
 *      Author: Erich Styger
 */

#ifndef APP_DHCP_H_
#define APP_DHCP_H_

#include "netif.h"

u8_t DHCP_GetState(struct netif *netifp);

bool DHCP_IsBound(struct netif *netifp);

void DHCP_Start(struct netif *netifp);
void DHCP_Init(void);

#endif /* APP_DHCP_H_ */
