/*
 * app_dhcp.c
 *
 *      Author: Erich Styger
 */

#include "app_dhcp.h"
#include "lwip/dhcp.h"
#include "lwip/prot/dhcp.h"

u8_t DHCP_GetState(struct netif *netifp) {
  struct dhcp *dhcp;

  if (netifp!=NULL) {
    dhcp = (struct dhcp *)netif_get_client_data(netifp, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP);
    if (dhcp!=NULL) {
      return dhcp->state;
    }
  }
  return DHCP_STATE_OFF;
}

bool DHCP_IsBound(struct netif *netifp) {
  return DHCP_GetState(netifp)==DHCP_STATE_BOUND;
}


void DHCP_Start(struct netif *netifp) {
  dhcp_start(netifp);
}

void DHCP_Init(void) {
}
