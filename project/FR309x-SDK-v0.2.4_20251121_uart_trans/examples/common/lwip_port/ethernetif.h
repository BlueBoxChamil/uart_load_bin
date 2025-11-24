#ifndef _ETHERNETIF_H
#define _ETHERNETIF_H

#include "pan_api.h"

void bnep_lwip_init(void);
int bnep_lwip_netif_up(PanUser *pan, BNEP_ADDR *local, BNEP_ADDR *remote);
int bnep_lwip_netif_down(void);
void bnep_lwip_timer_check(void);
void benp_lwip_recv_cb(uint8_t *buf, uint16_t len);

#endif  // _ETHERNETIF_H
