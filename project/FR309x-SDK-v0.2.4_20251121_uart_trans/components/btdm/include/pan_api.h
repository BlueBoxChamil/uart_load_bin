#ifndef _PAN_API
#define _PAN_API

#include "bt_types.h"
#include "me_api.h"

#include "pan_pkt.h"

#define NUM_BNEP_PANUS              1
#define NUM_PAN_ARP_ADDRS           10
#define NUM_BT_PKTS                 (NUM_BNEP_PANUS * 4)

#define BNEP_ADDR_SIZE              BD_ADDR_SIZE
#define BNEP_NUM_TIMERS             3

/*---------------------------------------------------------------------------
 * BnepEthernetProtocolType type
 *
 *  Some of the more common Ethernet Protocol types in BNEP packets.
 */
typedef uint16_t BnepEthernetProtocolType;

#define ETHER_TYPE_IPV4         0x0800
#define ETHER_TYPE_ARP          0x0806
#define ETHER_TYPE_IPV6         0x86DD

/*---------------------------------------------------------------------------
 * BnepPacketType type
 *
 *      The high order bit of the BNEP packet type is the Extension Flag. The
 *      one bit Extension Flag indicates if one or more extension headers
 *      follow the BNEP header before the data payload.
 *
 */
typedef uint8_t BnepPacketType;

#define BNEP_GENERAL_ETHERNET                   0x00
#define BNEP_CONTROL                            0x01
#define BNEP_COMPRESSED_ETHERNET                0x02
#define BNEP_COMPRESSED_ETHERNET_SOURCE_ONLY    0x03
#define BNEP_COMPRESSED_ETHERNET_DEST_ONLY      0x04

/*---------------------------------------------------------------------------
 * BnepControlType type
 *
 */
typedef uint8_t BnepControlType;

#define BNEP_CONTROL_COMMAND_NOT_UNDERSTOOD 0x00
#define BNEP_SETUP_CONNECTION_REQUEST_MSG   0x01
#define BNEP_SETUP_CONNECTION_RESPONSE_MSG  0x02
#define BNEP_FILTER_NET_TYPE_SET_MSG        0x03
#define BNEP_FILTER_NET_TYPE_RESPONSE_MSG   0x04
#define BNEP_FILTER_MULTI_ADDR_SET_MSG      0x05
#define BNEP_FILTER_MULTI_ADDR_RESPONSE_MSG 0x06

/*---------------------------------------------------------------------------
 * PanPacketType type
 *
 *      Indicates the packet type when sending or receiving a data packet.
 *      All packets include their associated IP header unless otherwise
 *      indicated.
 */
typedef BnepPacketType PanPacketType;

/* Packet type unknown.
 */
#define PANPKT_UNKNOWN              0x40

/* Packet type is General Ethernet and includes the MAC header. The packet
 * must be parsed to determine higher layer protocols. This packet is
 * normally forwarded to the remote network.
 */
#define PANPKT_ETHER_PKT            0x41

/* Packet is an ARP request.
 */
#define PANPKT_ARP_REQUEST          0x42

/* Packet is an ARP reply.
 */
#define PANPKT_ARP_REPLY            0x43

/* Packet type is IP.
 */
#define PANPKT_IP                   0x44

/* Packet is an ICMP request (Ping echo.)
 */
#define PANPKT_ICMP_REQUEST         0x45

/* Packet is an ICMP reply (Ping echo reply.)
 */
#define PANPKT_ICMP_REPLY           0x46

/* Packet type is TCP.
 */
#define PANPKT_TCP                  0x47

/* Packet type is UDP.
 */
#define PANPKT_UDP                  0x48

/* Packet is a DHCP request.
 */
#define PANPKT_DHCP_REQUEST         0x49

/* Packet is a DHCP reply.
 */
#define PANPKT_DHCP_REPLY           0x4a

/* Packet is a BNEP network filter (no IP header.)
 */
#define PANPKT_NET_FILTER           0x4b

/* Packet is a BNEP multicast filter (no IP header.)
 */
#define PANPKT_MCAST_FILTER         0x4c
/* End of PanPacketType */

#define ETHER_HDR_SIZE              (BNEP_ADDR_SIZE*2 + sizeof(BnepEthernetProtocolType))
#define ETHER_RANGE_SIZE            (sizeof(BnepEthernetProtocolType)*2)

#define BNEP_TYPE_SIZE              sizeof(BnepPacketType)
#define BNEP_GNRL_HDR_SIZE          (BNEP_TYPE_SIZE + ETHER_HDR_SIZE)
#define BNEP_CONTROL_TYPE_SIZE      sizeof(BnepControlType)
#define BNEP_CONTROL_HDR_SIZE       (BNEP_TYPE_SIZE + BNEP_CONTROL_TYPE_SIZE)

#define PAN_PANU_NUM_ATTRIBUTES     9
#define PAN_GN_NUM_ATTRIBUTES       11
#define PAN_NAP_NUM_ATTRIBUTES      13
#define PAN_NUM_SEARCH_ATTRIBUTES   21

/*---------------------------------------------------------------------------
 * BnepFilterResponseMsg type
 *
 */
typedef uint16_t BnepFilterResponseMsg;

#define BNEP_FR_OPERATION_SUCCESS   0x0000
#define BNEP_FR_UNSUPPORTED         0x0001
#define BNEP_FR_INVALID_PROTOCOL    0x0002
#define BNEP_FR_FAILED_MAX_FILTERS  0x0003
#define BNEP_FR_FAILED_SECURITY     0x0004

/** Indicates an open request is in progress on a client. Use 
 *  PAN_SetupConnection to complete the connection with a BNEP Setup
 *  Connection control message. This event precedes the PANEVENT_OPEN
 *  event on a client.
 */
#define PANEVENT_SETUP_CONNECTION   0x81

/** Indicates an open request on a server. Use PAN_ControlResponse to
 *  complete the connection with a BNEP Setup Connection response. This
 *  event will follow a PANEVENT_OPEN event on a server.
 */
#define PANEVENT_SETUP_CONN_REQ     0x82

/** Indicates a BNEP control packet other than SETUP_CONNECTION has been
 *  received.
 */
#define PANEVENT_CONTROL_IND        0x83

/** Indicates an Open request is in progress on a server or complete on
 *  a client.
 */
#define PANEVENT_OPEN               0x87

/** Indicates the connection is closed.
 */
#define PANEVENT_CLOSED             0x88

/** Indicates an SDP service query failed on an open.
 */
#define PANEVENT_SDP_FAILED         0x89

/** Indicates a data packet is available. The parms.pkt parameter points
 *  to the packet. The packet is of type PanPacketType indicated in the
 *  parms.type parameter. The packet length is indicated in the
 *  parms.rxPktLen parameter.
 */
#define PANEVENT_DATA_IND           0x8a

/** Indicates the PanPacket used to send data has been released and may
 *  be freed or reused.
 */
#define PANEVENT_DATA_HANDLED       0x8b
/* End of PanEvent */

#define PANBTPKT_FREE       0
#define PANBTPKT_INUSE      1
#define PANBTPKT_INUSE_INT  2
#define PANBTPKT_INUSE_EXT  3

/* Forward reference to PanCallbackParms defined below */
typedef struct _PanCallbackParms PanCallbackParms;

/*---------------------------------------------------------------------------
 * PanCallback type
 *
 *      PAN Events and data are passed to the application through a callback
 *      function of this type defined by the application.
 */
typedef void (*PanCallback)(PanCallbackParms *parms);

/*---------------------------------------------------------------------------
 * BNEP_ADDR structure
 *
 *      Represents a 48-bit Bluetooth or Ethernet device address.
 */
typedef struct _BNEP_ADDR
{
    uint8_t    addr[BNEP_ADDR_SIZE];
}   BNEP_ADDR;

/*---------------------------------------------------------------------------
 * BnepPanu structure
 *
 *      Maintains connection information for a PANU channel.
 */
typedef struct _BnepPanu {
    BNEP_ADDR       bnepAddr;       /* BNEP_ADDR of remote device */
    uint16_t        l2ChannelId;    /* L2CAP channel ID for connection */
    uint8_t         state;          /* PANU state */
    BtRemoteDevice *remDev;         /* Remote device of connection */
    EvmTimer        timer[BNEP_NUM_TIMERS]; /* Timers for Control commands */
    uint32_t        controlTimeout; /* Timeout value for Control commands */
}   BnepPanu;

/*----------------------------------------------------------------------
 * PanService structure
 *
 *  PAN service registration structure used to register a service
 *  through the PAN_Register function.
 */
typedef struct _PanService {
    PanCallback     callback;
    uint16_t        type;
} PanService;
/* End of PanService */

/*----------------------------------------------------------------------
 * PanPacket structure
 *
 *  PAN Packet allocation structure.
 */
typedef struct _PanPacket {
    uint8_t  header[BNEP_GNRL_HDR_SIZE];
    uint8_t  body[];
} PanPacket;
/* End of PanPacket */

/* ---------------------------------------------------------------------------
 * BtPacket container
 */
typedef struct _PanBtPkt
{
    uint16_t    context;
    BtPacket    btPkt;
} PanBtPkt;

/*----------------------------------------------------------------------
 * PanSetupConnPkt structure
 *
 *  PAN Setup Connection Request allocation structure.
 */
typedef struct _PanSetupConnPkt {
    uint8_t  header[BNEP_CONTROL_HDR_SIZE];
    uint8_t  uuidSize;
    uint8_t  dstUuid[2];
    uint8_t  srcUuid[2];
} PanSetupConnPkt;
/* End of PanSetupConnPkt */

/*----------------------------------------------------------------------
 * PanControlRespPkt structure
 *
 *  PAN Control response message allocation structure.
 */
typedef struct _PanControlRespPkt {
    uint8_t  header[BNEP_CONTROL_HDR_SIZE];
    uint8_t  message[2];
} PanControlRespPkt;
/* End of PanControlRespPkt */

/*---------------------------------------------------------------------------
 * PanRecvCb type
 *
 *      Describes the PAN Receive data callback function. Used when user decide to
 *      deal receicved data from remote device directly.
 */
typedef void (* PanRecvCb)(uint8_t *, uint16_t);
/* End of PanRecvCb */

/*--------------------------------------------------------------------------
 * PanUser structure
 *
 *      Maintains PANU connection info and status during an application
 *      session. A NAP, GN or PANU uses this structure to maintain
 *      information on connected PANU(s). The PanUser structure is always
 *      allocated by the application and is passed into the SDK through
 *      the PAN_Open function by a client or the PAN_Listen function by a
 *      server.
 */
typedef struct _PanUser {
    /* === Internal use only === */
    BnepPanu        bnepPanu;       /* remote BnepPanu */
    BNEP_ADDR       destMac;        /* storage for dest MAC address */
    uint8_t         state;
    PanRecvCb       recv_cb;        /* callback for receive data */

    /* NAP */
    uint32_t        remIpAddr;              
    uint32_t        remAddrContext;              

    /* SDP Client */
    uint16_t        dstService;
    SdpQueryToken   sdpQueryToken;
    uint8_t         sdpSrchAttrib[PAN_NUM_SEARCH_ATTRIBUTES];
} PanUser;
/* End of PanUser */

/*---------------------------------------------------------------------------
 * PanCallbackParms structure
 *
 *      Describes a callback event and any data that relates to the event.
 */
typedef struct _PanCallbackParms
{
    /* Completion status based on the event type. Reason will be
     * L2capDiscReason for PANEVENT_CLOSED events or BtStatus for others.
     */
    uint16_t        status;

    uint8_t         event;      /* Type of PANEVENT_XX event */

    /* Handle to the PanUser structure associated with this event. Valid
     * after a connection is established.
     */
    PanUser        *pan;

    /* Type of packet associated with this event. Valid on
     * PANEVENT_DATA_HANDLED and PANEVENT_DATA_IND events.
     */
    uint8_t         type;

    /* On PANEVENT_DATA_HANDLED event, points to the sent data packet.
     * On PANEVENT_DATA_IND event, points to the received packet.
     * The PanPacketType field above indicates the type.
     */
    uint8_t        *pkt;

    /* Length of received data packet. Valid on
     * PANEVENT_DATA_IND events.
     */
    uint16_t        rxPktLen;

} PanCallbackParmsT;
/* End of PanCallbackParms */

/*---------------------------------------------------------------------------
 * bnep_addr_ntoa()
 *
 *      Convert a MSB BNEP_ADDR to MSB ASCII notation aa:bb:cc:dd:ee:ff.
 *      AddrString should point to no less than 20 bytes. It will be
 *      null-terminated upon return.
 *
 * Parameters:
 *      bnepAddr - Pointer to BNEP_ADDR to convert.
 *      addrString - Pointer to storage for the converted BNEP address string.
 *
 * Returns:
 *      pointer to the converted/provided AddrString
 */
char *bnep_addr_ntoa(const BNEP_ADDR *bnepAddr, char *addrString);

/*---------------------------------------------------------------------------
 * BNEP_Init()
 *
 *     Initializes the BNEP layer.
 *
 * Returns:
 *     TRUE - BNEP was successfully initialized.
 *
 *     FALSE  - BNEP failed to initialize.  This can
 *              happen when BNEP cannot be registered with L2CAP.
 */
int BNEP_Init(void);

/*---------------------------------------------------------------------------
 * PAN_Init()
 *
 *      Initialize PAN. This should be the first function called for PAN. 
 *
 * Returns:
 *      TRUE - PAN was successfully initialized.
 *      FALSE  - PAN failed to initialize.
 */
int PAN_Init(void);

/*---------------------------------------------------------------------------
 * PAN_Register()
 *
 *      Register a callback for events associated with the PanService. An
 *      application may register for only one service in a session.
 *
 * Parameters:
 *      service - pointer to PanService structure with the PanCallback and
 *                PanServiceType fields initialized for the desired service.
 *
 * Returns:
 *      BT_STATUS_SUCCESS - The registration succeeded.
 *
 *      BT_STATUS_FAILED - The registration failed.
 */
BtStatus PAN_Register(PanService *service);

/*---------------------------------------------------------------------------
 * PAN_Open()
 *
 *      Initiate an open request for a PAN service connection.
 *
 * Parameters:
 *      remDev - The target remote device for the connection.
 *      pan - handle to the PanUser structure used to maintain the
 *          connection.
 *      cb - callback function used to handle received data directly
 *      tgtService - Service required for the connection.
 *
 * Returns:
 *      BT_STATUS_PENDING - The open request succeeded.
 *
 *      BT_STATUS_FAILED - The open request failed.
 */
BtStatus PAN_Open(BtRemoteDevice *remDev, PanUser *pan, PanRecvCb cb, uint16_t tgtService);

/*---------------------------------------------------------------------------
 * PAN_SetupConnection()
 *
 *      Send a BNEP Setup Connection message from a client to complete an
 *      open connection request.
 *
 * Parameters:
 *      pan - handle to the PanUser structure used to maintain the
 *          connection.
 *      setupPkt - Setup connection packet.
 *
 * Returns:
 *      BT_STATUS_PENDING - The send succeeded. (If accepted by the remote
 *          device, a PANEVENT_OPEN event will occur.)
 *
 *      BT_STATUS_FAILED - The send failed.
 */
BtStatus PAN_SetupConnection(PanUser *pan, PanSetupConnPkt *setupPkt);

/*---------------------------------------------------------------------------
 * PAN_ControlResponse()
 *
 *      Send a BNEP Setup Connection message from a server to complete an
 *      open connection request.
 *
 * Parameters:
 *      pan - handle to the PanUser structure used to maintain the
 *          connection.
 *      type - BNEP Control response type.
 *      controlRespPkt - control response packet.
 *
 * Returns:
 *      BT_STATUS_PENDING - The send succeeded.
 *
 *      BT_STATUS_FAILED - The send failed.
 */
BtStatus PAN_ControlResponse(PanUser *pan, BnepControlType type,
                             PanControlRespPkt *controlRespPkt);

/*---------------------------------------------------------------------------
 * PAN_ForwardPkt()
 *
 *      Forward a data packet received from the BNEP protocol stack. The
 *      packet must include the Ethernet MAC header. No PANEVENT_DATA_HANDLED
 *      event will follow this function, as it was received from the BNEP
 *      stack and does not require allocation from the application memory
 *      pool.
 *
 * Parameters:
 *      pan - handle to the PanUser structure used to maintain the
 *          connection.
 *      pkt - pointer to the Ethernet data packet received from the BNEP
 *          stack.
 *      len - length of data packet.
 *
 * Returns:
 *      BT_STATUS_PENDING - The send succeeded.
 *
 *      BT_STATUS_FAILED - The send failed.
 */
BtStatus PAN_ForwardPkt(PanUser *pan, uint8_t *pkt, uint16_t len);

/*---------------------------------------------------------------------------
 * PAN_Close()
 *
 *      Close the connection associated with the PanUser structure.
 *
 * Parameters:
 *      pan - handle to the PanUser structure used to maintain the
 *          connection.
 *
 * Returns:
 *      BT_STATUS_SUCCESS - The close succeeded.
 *
 *      BT_STATUS_FAILED - The close failed.
 */
BtStatus PAN_Close(PanUser *pan);

/*---------------------------------------------------------------------------
 * PAN_SendPkt()
 *
 *      Send a data packet created by the application. A PANEVENT_DATA_HANDLED
 *      will be sent to the registered callback to notify when the
 *      packet can be returned to the memory pool.
 *
 * Parameters:
 *      pan - handle to the PanUser structure used to maintain the
 *          connection.
 *      panPkt - pointer to the data packet encapsulated in the PanPacket
 *          structure.
 *      type - data packet type.
 *      len - length of data packet.
 *
 * Returns:
 *      BT_STATUS_PENDING - The send succeeded.
 *
 *      BT_STATUS_FAILED - The send failed.
 */
BtStatus PAN_SendPkt(PanUser *pan, PanPacket *panPkt, PanPacketType type, uint16_t len);

/*---------------------------------------------------------------------------
 * PAN_SendArp()
 *
 *      Send an ARP to resolve the specified IP address to a MAC address.
 *
 * Parameters:
 *      pan - handle to the PanUser structure associated with this address.
 *      destIP - IP address to resolve to a MAC address.
 *
 * Returns:
 *      The MAC address in a BNEP_ADDR structure.
 */
BNEP_ADDR  *PAN_SendArp(PanUser *pan, uint32_t destIP);

/*---------------------------------------------------------------------------
 * PAN_MakeArpHdr()
 *
 *      Helper function to build an ARP header.
 *
 * Parameters:
 *      arpHdr - pointer to memory buffer large enough for ARP header.
 *      dstIP - IP address of destination.
 *
 * Returns:
 *      BT_STATUS_SUCCESS - The operation succeeded.
 *
 *      BT_STATUS_FAILED - The operation failed.
 */
BtStatus PAN_MakeArpHdr(ArpHeader *arpHdr, uint32_t dstIP);

/*---------------------------------------------------------------------------
 * PAN_GetLocalIpAddr_v2()
 *
 *      Get the local IP address.
 *
 * Parameters:
 *      None
 *
 * Returns:
 *      The local IP address as an uint32_t.
 */
uint32_t PAN_GetLocalIpAddr_v2(void);

/*---------------------------------------------------------------------------
 * PAN_SetLocalIpAddr_v2()
 *
 *      Set the local IP address.
 *
 * Parameters:
 *      addr - local IP address.
 *
 * Returns:
 *      None.
 */
void PAN_SetLocalIpAddr_v2(uint32_t addr);

/*---------------------------------------------------------------------------
 * PAN_GetRemoteIpAddr_v2()
 *
 *      Get the remote IP address.
 *
 * Parameters:
 *      pan - handle to the PanUser structure used to maintain the
 *          connection.
 *
 * Returns:
 *      The remote IP address as an uint32_t.
 */
uint32_t PAN_GetRemoteIpAddr_v2(PanUser *pan);

/*---------------------------------------------------------------------------
 * PAN_SetRemoteMacAddr()
 *
 *      Set the MAC address of the remote PAN device.
 *
 * Parameters:
 *      pan - handle to the PanUser structure used to maintain the
 *          connection.
 *      macAddr - MAC address of remote PAN device.
 *
 * Returns:
 *      BT_STATUS_SUCCESS - The registration succeeded.
 *
 *      BT_STATUS_FAILED - The registration failed.
 */
BtStatus PAN_SetRemoteMacAddr(PanUser *pan, uint8_t *macAddr);

/*---------------------------------------------------------------------------
 * PAN_SetDhcpAddr_v2()
 *
 *      Set the address to use for DHCP requests.
 *
 * Parameters:
 *      addr - DHCP address to use.
 *
 * Returns:
 *      None.
 */
void PAN_SetDhcpAddr_v2(uint32_t addr);

/*---------------------------------------------------------------------------
 *            PanFindFreeBtPkt()
 *---------------------------------------------------------------------------
 *
 * Synopsis:  Find available packet.
 *
 * Return:    packet pointer or 0.
 */
PanBtPkt *PanFindFreeBtPkt(void);

/*---------------------------------------------------------------------------
 * BNEP_SendPkt()
 *
 *      Send a BNEP packet to the remote device. The bnepPkt parameter
 *      points to one of the valid BNEP packet types: General, Compressed,
 *      Source only compressed or Destination only compressed and may
 *      contain extension headers.
 *
 * Parameters:
 *      panu - Identifies the BNEP PANU channel that owns the packet.
 *      btPacket - A pointer to the BtPacket used to send the BNEP packet
 *                 through L2CAP.
 *      bnepPkt - A pointer to the BNEP packet to send.
 *      bnepPktLen - Length of the BNEP packet.
 *
 * Returns:    
 *      BT_STATUS_PENDING - The send packet process has been successfully 
 *          started.
 *
 *      BT_STATUS_INVALID_PARM - An invalid parameter was used.
 *
 *      BT_STATUS_FAILED - L2CAP_Send failure.
 *
 *      BT_STATUS_IN_PROGRESS - BNEP is busy sending another packet.
 */
BtStatus BNEP_SendPkt(BnepPanu *panu,
                      BtPacket *btPacket,
                      uint8_t *bnepPkt,
                      uint16_t bnepPktLen);

#endif  // _PAN_API

