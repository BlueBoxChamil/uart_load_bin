#ifndef __OVERIDE_H
#define __OVERIDE_H

/* WARNING: The values in this overide.h file were selected specifically for
 * this sample application. If you change them, the sample application may fail
 * to compile or not work properly.
 */
#define BT_STACK                XA_ENABLED
#define TCP_STACK               XA_DISABLED
#define IRDA_STACK              XA_DISABLED

#define XA_DEBUG                XA_DISABLED

#define HCI_REMOVE_STATIC_BUFFER
#define L2CAP_REMOVE_STATIC_BUFFER
#define RFCOMM_REMOVE_STATIC_BUFFER
#define SDP_REMOVE_STATIC_BUFFER
#define AVRCP_REMOVE_STATIC_BUFFER

#define RADIO_INIT_SIMPLIFY

#define EVM_REPLACE_TIMER
#define RFCOMM_TIMER_OPT        XA_ENABLED

/* set default access to non-discover and non-connecte, leave configuration based on pskeys to decide which state will be */
//#define BT_DEFAULT_ACCESS_MODE_NC   BAM_GENERAL_ACCESSIBLE
#define BT_DEFAULT_ACCESS_MODE_NC       BAM_NOT_SET
#define BT_ALLOW_SCAN_WHILE_CON         XA_ENABLED
#define BT_DEFAULT_PAGE_SCAN_WINDOW     0x20
#define BT_DEFAULT_PAGE_SCAN_INTERVAL   0x400
#define BT_DEFAULT_INQ_SCAN_WINDOW      0x20
#define BT_DEFAULT_INQ_SCAN_INTERVAL    0x400

//enable multilink
//#define BT_DEFAULT_ACCESS_MODE_C  BAM_GENERAL_ACCESSIBLE

/*
 * Set number of devices.
 */
#ifndef NUM_BT_DEVICES
#define NUM_BT_DEVICES          2
#endif
#define NUM_KNOWN_DEVICES       8
#define NUM_SCO_CONNS           NUM_BT_DEVICES

#define HCI_ALLOW_PRESCAN       XA_DISABLED
#define HCI_NUM_EVENTS          8
#define HCI_NUM_PACKETS         6
#define HCI_NUM_ACL_BUFFERS     10
#define HCI_ACL_DATA_SIZE       (1021) 
#define BT_SCO_HCI_DATA         XA_ENABLED

/* L2CAP definitions */
#define L2CAP_NUM_PROTOCOLS     10
#define L2CAP_PING_SUPPORT      XA_DISABLED
#define BT_PACKET_HEADER_LEN    28 /* Required for MP3 */

#define L2CAP_NUM_CHANNELS      16
//#define L2CAP_NUM_ENHANCED_CHANNELS (NUM_BT_DEVICES*2)  // avdtp_stream and avrcp_browsing
#define L2CAP_MTU               1691
//#define L2CAP_MPS                    512
#define L2CAP_MANGLER_TESTING XA_DISABLED
#define L2CAP_ENHANCED_IOP_TESTING XA_DISABLED
#define L2CAP_PRELUDE_SIZE      7

#define SDP_SERVER_SEND_SIZE    L2CAP_MTU

#define RFCOMM_PROTOCOL         XA_ENABLED

#define XA_LOAD_LIST            XA_MODULE(AVDTP) XA_MODULE(CMGR) \
                                XA_MODULE(AVDEV) XA_MODULE(A2DP) \
                                XA_MODULE(AVRCP) XA_MODULE(HF)   \
                                XA_MODULE(PBAP) XA_MODULE(OBEX) XA_MODULE(GOEP) \
                                XA_MODULE(HFG)
                                
//#define BT_ALLOW_SCAN_WHILE_CON XA_ENABLED

#define AT_ROLE_TERMINAL        XA_ENABLED
#define AT_ROLE_MOBILE          XA_ENABLED
#define AT_HEADSET              XA_DISABLED
#define AT_HANDSFREE            XA_ENABLED
#define AT_PHONEBOOK            XA_DISABLED
#define AT_SMS                  XA_DISABLED
#define AT_DUN                  XA_DISABLED

#define HF_USE_CALL_MANAGER         XA_DISABLED//XA_ENABLED
#define HF_USE_MESSAGING_COMMANDS   XA_DISABLED
#define HF_USE_RESP_HOLD            XA_DISABLED
#define HF_USE_PHONEBOOK_COMMANDS   XA_DISABLED//XA_ENABLED
#define HF_SNIFF_TIMER              -1
#define HF_WIDE_BAND_SPEECH         XA_ENABLED

/* -------------used to do simplity begin---------------- */
#define HF_USE_IIA                  XA_DISABLED
#define HF_TX_BUFFER_SIZE           32
#define HF_RECV_BUFFER_SIZE         512

#define HFG_USE_IIA                  XA_ENABLED
#define HFG_TX_BUFFER_SIZE           256
#define HFG_RECV_BUFFER_SIZE         256
#define HFG_CODEC_NEG                XA_ENABLED


/* -------------used to do simplity end----------------- */

// TODO
//#define HF_SDK_FEATURES     (HF_FEATURE_CLI_PRESENTATION     | \
//                          HF_FEATURE_VOICE_RECOGNITION    | \
//                          HF_FEATURE_VOLUME_CONTROL)

/* SBC definitions */                   
#define SBC_ENCODER                 XA_DISABLED
#define SBC_DECODER                 XA_DISABLED

/* A2DP definitions */
#define A2DP_SOURCE                 XA_ENABLED
#define A2DP_SINK                   XA_ENABLED

/* AVRCP definitions */
#define AVRCP_VERSION_1_3_ONLY      XA_DISABLED
#define AVRCP_ADVANCED_TARGET       XA_ENABLED
#define AVRCP_ADVANCED_CONTROLLER   XA_ENABLED
#define AVRCP_BROWSING_TARGET       XA_DISABLED
#define AVRCP_BROWSING_CONTROLLER   XA_DISABLED

#define AVRCP_LIST_PLAYERS_ENABLED  XA_DISABLED
#ifndef SIMPLIFY_AVRCP
#define AVRCP_ADVANCED_RESPONSE_SIZE    256//128
#else
#define AVRCP_ADVANCED_RESPONSE_SIZE    32
#endif

/* add by owen. when sniff is enable, accept event for press will not be discard because of timeout */
#define AVRCP_RTX_CMD_TIMEOUT       2000

/* Select the appropriate math operation type */
//#define SBC_MATH_FUNCTIONS          SBC_MATH_USE_FIXED_HI_RES
/* #define SBC_MATH_FUNCTIONS SBC_MATH_USE_FIXED_LO_RES */
/* #define SBC_MATH_FUNCTIONS SBC_MATH_USE_FIXED_ASM */  
/* #define SBC_MATH_FUNCTIONS SBC_MATH_USE_FLOAT */  

#define PBAP_NUM_SERVERS                0
#define PBAP_NUM_CLIENTS                NUM_BT_DEVICES
#define MAP_NUM_SERVERS                 0
#define MAP_NUM_CLIENTS                 NUM_BT_DEVICES

#define SPP_SERVER	                    XA_ENABLED
#define SPP_CLIENT	                    XA_ENABLED


#endif /* __OVERIDE_H */

