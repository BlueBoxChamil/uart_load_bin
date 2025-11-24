#ifndef __PAN_PKT_H
#define __PAN_PKT_H
/*****************************************************************************
 *
 * File:
 *     $Id: pan_pkt.h 1284 2009-04-08 22:49:58Z suneelk $
 *     $Product: iAnywhere PAN SDK v1.x $
 *     $Revision: 1284 $
 *
 * Description: This file contains code for the PAN profile. 
 *
 * Created:     Feb 7, 2003
 *
 * Copyright 2000-2004 Extended Systems, Inc.  ALL RIGHTS RESERVED.
 * Portions copyright 2005-2008 iAnywhere Solutions, Inc.  
 *
 * Unpublished Confidential Information of iAnywhere Solutions, Inc.  
 * Do Not Disclose.
 *
 * No part of this work may be used or reproduced in any form or by any means, 
 * or stored in a database or retrieval system, without prior written 
 * permission of iAnywhere Solutions, Inc.
 * 
 * Use of this work is governed by a license granted by iAnywhere Solutions, Inc.  
 * This work contains confidential and proprietary information of iAnywhere 
 * Solutions, Inc. which is protected by copyright, trade secret, trademark and 
 * other intellectual property rights.
 *
 ****************************************************************************/

#define IPPROTO_IP              0           /* dummy for IP */
#define IPPROTO_ICMP            1           /* Control Message Protocol */
#define IPPROTO_IPV4            4           /* IPv4 */
#define IPPROTO_TCP             6           /* TCP */
#define IPPROTO_UDP             17          /* UDP */

#define NET_PROTOCOL_TYPE_ARP   0x0806      /* Address Resolution Protocol */
#define NET_PROTOCOL_TYPE_VLAN  0x8100      /* VLAN( Virtual lan)-tagged frame (IEEE 802.1Q)*/
#define NET_PROTOCOL_TYPE_IPV4  0x0800      /* Internet Protocol, Version 4 (IPv4)*/

#define ICMP_ECHO               8
#define ICMP_ECHOREPLY          0
#define ICMP_MIN                8           /* minimum icmp packet size (just header) */

#define BOOTP_SERVER            0x0043
#define BOOTP_CLIENT            0x0044
#define DHCPDISCOVER            1 
#define DHCPOFFER               2 
#define DHCPREQUEST             3 
#define DHCPDECLINE             4 
#define DHCPACK                 5 
#define DHCPNAK                 6 
#define DHCPRELEASE             7 

/* ---------------------------------------------------------------------------
 * ARP header
 */
typedef __PACKED_STRUCT _ArpHeader {
    uint16_t     macType;        /* MAC address type */
    uint16_t     protoType;      /* Protocol address type */
    uint8_t      macLen;         /* MAC address length */
    uint8_t      protoLen;       /* Protocol address length */
    uint16_t     op;             /* ARP operation (1=request, 2=reply) */
    uint8_t      srcMac[6];      /* MAC source address */
    uint32_t     srcIP;          /* IP source address */
    uint8_t      dstMac[6];      /* MAC destination address */
    uint32_t     dstIP;          /* IP destination address */
} ArpHeader;

/* ---------------------------------------------------------------------------
 * IP header
 */
typedef __PACKED_STRUCT _IpHeader {
    uint32_t     srcIP;          /* source address */
    uint32_t     dstIP;          /* destination address */
    uint8_t      hdrLen:4;       /* length of the header in 32 bit units */
    uint8_t      version:4;      /* Version of IP */
    uint8_t      tos;            /* Type of service */
    uint16_t     totalLen;       /* total length of the packet */
    uint16_t     id;             /* unique identifier */
    uint16_t     offset;         /* fragment offset */
    uint8_t      ttl;            /* time to live */
    uint8_t      proto;          /* protocol (TCP, UDP etc) */
    uint16_t     cksum;          /* IP checksum */

} IpHeader;

/* ---------------------------------------------------------------------------
 * ICMP header
 */
typedef __PACKED_STRUCT _IcmpHeader {
    uint8_t      type;
    uint8_t      code;           /* type sub code */
    uint16_t     cksum;
    uint16_t     id;
    uint16_t     seq;

    uint32_t     timestamp;      /* Timestamp at the beginning of data */
} IcmpHeader;

/* ---------------------------------------------------------------------------
 * UDP header
 */
typedef __PACKED_STRUCT _UdpHeader {
    uint16_t     srcPort;        /* Source port # */
    uint16_t     dstPort;        /* Destination port # */
    uint16_t     len;            /* Udp packet length */
    uint16_t     cksum;          /* Udp checksum (optional) */
} UdpHeader;

/* ---------------------------------------------------------------------------
 * DHCP header
 */
typedef __PACKED_STRUCT _DhcpHeader {
    uint8_t      op;
    uint8_t      htype;
    uint8_t      hlen;
    uint8_t      hops;
    uint32_t     xid;
    uint16_t     secs;
    uint16_t     flags;
    uint32_t     ciaddr;
    uint32_t     yiaddr;
    uint32_t     siaddr;
    uint32_t     giaddr;
    uint8_t      chaddr[16];
    uint8_t      sname[64];
    uint8_t      file[128];
    uint8_t      cookie[4];
    uint8_t      options[312];
} DhcpHeader;

/* ---------------------------------------------------------------------------
 * TCP header
 */
typedef __PACKED_STRUCT _TcpHeader {
    uint16_t     srcPort;        /* Source port # */
    uint16_t     dstPort;        /* Destination port # */
    uint32_t     seq;
    uint32_t     ack;
    uint8_t      reserved:4;     /* reserved */
    uint8_t      offset:4;       /* length of the header, in 32 bit units */
    uint8_t      flags;
    uint16_t     window;
    uint16_t     cksum;          /* calculated from TCP Pseudo Header */
    uint16_t     urgent;
    uint8_t      options[];
    /* 0 to 44 bytes of options may be placed at the end of the TCP header.
     * The options must be padded to make the TCP header length a multiple
     * of 32 bits.
     */
} TcpHeader;

/* ---------------------------------------------------------------------------
 * TCP Pseudo header (used for checksum calculation)
 */
typedef __PACKED_STRUCT _TcpPseudoHeader {
    uint32_t     srcIP;          /* Source IP address */
    uint32_t     dstIP;          /* Destination IP address */
    uint8_t      zero;
    uint8_t      proto;          /* IP protocol */
    uint16_t     len;            /* totoal length */
} TcpPseudoHeader;

#endif /* __PAN_PKT_H */


