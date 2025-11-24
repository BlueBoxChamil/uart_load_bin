#include <string.h>

#include "lwip/opt.h"
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/init.h"
#include "lwip/dhcp.h"
#include "lwip/tcpip.h"
#include "lwip/timeouts.h"
#include "netif/ethernet.h"
#include "netif/etharp.h"
#include "lwip/ethip6.h"
#include "ethernetif.h"

#include "btdm_host.h"
#include "pan_api.h"
#include "co_log.h"

/* Network interface name */
#define IFNAME0 's'
#define IFNAME1 't'

static BNEP_ADDR _addr_local, _addr_remote;
static PanUser *_pan_user;

// lwip data
static struct netif btstack_netif;

static struct netif_ext_callback netif_callback;

static void netif_callback_fn(struct netif* netif, netif_nsc_reason_t reason, const netif_ext_callback_args_t* args)
{
    printf("netif_callback_fn: 0x%04x.\r\n", reason);
}

void benp_lwip_recv_cb(uint8_t *buf, uint16_t len)
{
//    printf("pan_recv_cb\r\n");
//    for (uint16_t i=0; i<len; ) {
//        printf("%02x ", buf[i]);
//        i++;
//        if ((i % 16) == 0) {
//            printf("\r\n");
//        }
//    }
//    printf("\r\n");
 
    if ((btstack_netif.flags & NETIF_FLAG_LINK_UP) == 0) {
        return;
    }
    
    /* We allocate a pbuf chain of pbufs from the pool. */
    struct pbuf * p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);
    if (p) {
        /* store packet in pbuf chain */
        struct pbuf * q = p;
        while (q != NULL && len){
            memcpy(q->payload, buf, q->len);
            buf += q->len;
            len -= q->len;
            q = q->next;
        }
        
        btstack_netif.input(p, &btstack_netif);
    }
}

/**
 * This function should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 * @param netif the lwip network interface structure
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *       strange results. You might consider waiting for space in the DMA queue
 *       to become availale since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 */

static err_t low_level_output( struct netif *netif, struct pbuf *p )
{
    LOG_INFO(NULL, "low_level_output: packet %p, len %u, total len %u ", p, p->len, p->tot_len);

//    // inc refcount
//    pbuf_ref( p );
    
//    uint8_t *__buffer = p->payload;
//    uint16_t __length = p->len;
    
//    printf("low_level_output\r\n");
//    for (uint16_t i=0; i<__length; ) {
//        printf("%02x ", __buffer[i]);
//        i++;
//        if ((i % 16) == 0) {
//            printf("\r\n");
//        }
//    }
//    printf("\r\n");

    /* Extract destination and source address from the ethernet packet */
    uint8_t *buffer = p->payload;
    uint16_t payload_len = p->len;
    BNEP_ADDR addr_dest, addr_src;
    uint16_t pos, header_pos, header_len;
    uint16_t network_protocol_type;
    bool has_src, has_dest;
    BnepPacketType bnep_pkt_type;
    PanPacket *pan_packet;
    PanBtPkt  *panBtPkt;
    
    pos = 0;
    memcpy(&addr_dest.addr[0], &buffer[pos], BNEP_ADDR_SIZE);
    pos += BNEP_ADDR_SIZE;
    memcpy(&addr_src.addr[0], &buffer[pos], BNEP_ADDR_SIZE);
    pos += BNEP_ADDR_SIZE;
    network_protocol_type = (buffer[pos] << 8) | (buffer[pos+1]);
    pos += 2;
    
    payload_len -= pos;
    
    has_dest = (memcmp(&addr_dest.addr[0], &_addr_remote.addr[0], BNEP_ADDR_SIZE) != 0);
    has_src = (memcmp(&addr_src.addr[0], &_addr_local.addr[0], BNEP_ADDR_SIZE) != 0);
    
    if (has_src && has_dest) {
        bnep_pkt_type = BNEP_GENERAL_ETHERNET;
    }
    else if (has_src && !has_dest) {
        bnep_pkt_type = BNEP_COMPRESSED_ETHERNET_SOURCE_ONLY;
    } 
    else if (!has_src && has_dest) {
        bnep_pkt_type = BNEP_COMPRESSED_ETHERNET_DEST_ONLY;
    } 
    else {
        bnep_pkt_type = BNEP_COMPRESSED_ETHERNET;
    }
    
    pan_packet = pvPortMalloc(sizeof(PanPacket) + payload_len);
    
    header_len = 0;
    header_pos = BNEP_GNRL_HDR_SIZE;
    /* Add protocol type */
    header_pos -= 2;
    pan_packet->header[header_pos] = network_protocol_type >> 8;
    pan_packet->header[header_pos + 1] = network_protocol_type >> 0;
    header_len += 2;
    
    /* Add the source address if needed */
    if (has_src) {
        header_pos -= BNEP_ADDR_SIZE;
        memcpy(&pan_packet->header[header_pos], &addr_src.addr[0], BNEP_ADDR_SIZE);
        header_len += BNEP_ADDR_SIZE;
    }
    
    /* Add the destination address if needed */
    if (has_dest) {
        header_pos -= BNEP_ADDR_SIZE;
        memcpy(&pan_packet->header[header_pos], &addr_dest.addr[0], BNEP_ADDR_SIZE);
        header_len += BNEP_ADDR_SIZE;
    }
    
    header_pos -= 1;
    pan_packet->header[header_pos] = bnep_pkt_type;
    header_len += 1;

    memcpy(pan_packet->body, &buffer[pos], payload_len);

    panBtPkt = PanFindFreeBtPkt();
    if (panBtPkt == 0) {
        LOG_ERR(NULL, "low_level_output: alloc buffer failed\r\n");
        while(1);
    }
    uint16_t *context = (uint16_t *)(&panBtPkt->btPkt) - 1;
    *context = PANBTPKT_INUSE_EXT;
    
    LOG_INFO(NULL, "low_level_output: send packet through BNEP, payload-%d, header-%d.\r\n", payload_len, header_len);
    BNEP_SendPkt(&_pan_user->bnepPanu, &panBtPkt->btPkt, &pan_packet->body[0] - header_len, payload_len + header_len);

    return ERR_OK;
}

static err_t bnep_lwip_netif_init(struct netif *netif){

    // interface short name
    netif->name[0] = IFNAME0;
    netif->name[1] = IFNAME1;

    // mtu
    netif->mtu = 1500;

    /* device capabilities */
    netif->flags |= NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP;

    /* We directly use etharp_output() here to save a function call.
     * You can instead declare your own function an call etharp_output()
     * from it if you have to do some checks before sending (e.g. if link
     * is available...)
     */
    netif->output = etharp_output;
#if LWIP_IPV6
    netif->output_ip6 = ethip6_output;
#endif
    netif->linkoutput = low_level_output;

    return ERR_OK;
}

int bnep_lwip_netif_up(PanUser *pan, BNEP_ADDR *local, BNEP_ADDR *remote)
{
    LOG_INFO(NULL, "bnep_lwip_netif_up start\r\n");
    
    _pan_user = pan;
    _addr_local = *local;
    _addr_remote = *remote;

    // set mac address
    btstack_netif.hwaddr_len = 6;
    memcpy(btstack_netif.hwaddr, &local->addr[0], 6);

    // link is up
    btstack_netif.flags |= NETIF_FLAG_LINK_UP;

    // if up
    netif_set_up(&btstack_netif);
    
    dhcp_start(&btstack_netif);

    return 0;
}

/**
 * @brief Bring up network interfacd
 * @param network_address
 * @return 0 if ok
 */
int bnep_lwip_netif_down(void)
{
    LOG_INFO(NULL, "bnep_lwip_netif_down\r\n");

    // link is down
    btstack_netif.flags &= ~NETIF_FLAG_LINK_UP;

    dhcp_stop(&btstack_netif);
    netif_set_down(&btstack_netif);

    return 0;
}

/**
 * @brief Initialize network interface
 * @param send_packet_callback
 */
void bnep_lwip_init(void)
{
    static uint8_t bnep_lwip_inited = 0;
    
    if (bnep_lwip_inited) {
        return;
    }
    bnep_lwip_inited = 1;

#if NO_SYS
    lwip_init();
#else
    tcpip_init(NULL, NULL);
#endif

    ip4_addr_t fsl_netif0_ipaddr, fsl_netif0_netmask, fsl_netif0_gw;
#if 1
    // when using DHCP Client, no address
    IP4_ADDR(&fsl_netif0_ipaddr, 0U, 0U, 0U, 0U);
    IP4_ADDR(&fsl_netif0_netmask, 0U, 0U, 0U, 0U);
#else
    // when playing DHCP Server, set address
    IP4_ADDR(&fsl_netif0_ipaddr, 192U, 168U, 7U, 1U);
    IP4_ADDR(&fsl_netif0_netmask, 255U, 255U, 255U, 0U);
#endif
    IP4_ADDR(&fsl_netif0_gw, 0U, 0U, 0U, 0U);

    // input function differs for sys vs nosys
    netif_input_fn input_function;
#if NO_SYS
    input_function = ethernet_input;
#else
    input_function = tcpip_input;
#endif

    netif_add(&btstack_netif, &fsl_netif0_ipaddr, &fsl_netif0_netmask, &fsl_netif0_gw, NULL, bnep_lwip_netif_init, input_function);
    netif_set_default(&btstack_netif);
    netif_add_ext_callback(&netif_callback, netif_callback_fn);
}

void bnep_lwip_timer_check(void)
{
    if ((btstack_netif.flags & NETIF_FLAG_LINK_UP) == 0) {
        return;
    }
    sys_check_timeouts();
}

uint32_t lwip_rand(void)
{
    return rand();
}
