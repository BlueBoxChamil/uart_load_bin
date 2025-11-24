#ifndef __AVDTP_API_H_
#define __AVDTP_API_H_

#include "btconfig.h"

//#include "sys/avtp.h"
//#include "sys/avdtpcon.h"
#include "sec_api.h"
#include "bt_types.h"
#include "avctp_api.h"
/*---------------------------------------------------------------------------
 * AVDTP API layer
 *
 *    Audio/Video Distribution Transport Protocol (AVDTP) is the 
 *    transport layer for the Generic Audio/Video Distribution 
 *    Profile defined by the Bluetooth Audio Video Working Group.  
 *    This protocol provides a mechanism for streaming Audio and 
 *    Video content over a wireless Bluetooth connection.  
 *
 *    The AVDTP API is designed for audio and video 
 *    applications using the SDK core protocol stack.  
 *    It provides an API for connection management, stream discovery, 
 *    stream configuration and stream management.  
 */
 
#define AVDTP_MAX_CP_VALUE_SIZE     10
#define AVDTP_MAX_CODEC_ELEM_SIZE   6
/****************************************************************************
 *
 * Types
 *
 ****************************************************************************/

/*---------------------------------------------------------------------------
 * AvdtpEvent type
 *      
 *     All indications and confirmations are sent through a callback 
 *     function as events. The type of event will determine which 
 *     fields in the AvdtpCallbackParms structure are valid.  The 'channel', 
 *     'stream', 'len', and 'error' fields are always valid.  The 'event' 
 *     field contains the event associated with the callback.  
 */
typedef uint8_t AvdtpEvent;

/** This event is received by the application when a lower layer connection 
 *  (L2CAP) has been established.  Both the initiator and acceptor of the 
 *  connection are notified.  If the device is the initiator, this event is 
 *  received after calling AVDTP_Connect().  If the device is the acceptor, 
 *  this event is received after calling AVDTP_ConnectRsp().  
 * 
 * During this callback, the 'p.remDev' parameter is valid.  
 */
#define AVDTP_EVENT_CONNECT             1

/** This event is received by the application when a lower layer connection 
 *  (L2CAP) request has been received.  Only the acceptor of the connection 
 *  is notified.  The acceptor must call AVDTP_ConnectRsp() to either accept 
 *  or reject the connection.  
 * 
 * During this callback, the 'p.remDev' parameter is valid.  
 */
#define AVDTP_EVENT_CONNECT_IND         2

/** This event is received by the application when a lower layer connection 
 *  (L2CAP) has been disconnected.  Both the initiator and acceptor of the 
 *  connection are notified.  If the device is the initiator, this event is 
 *  received after calling AVDTP_Disconnect().  
 * 
 *  During this callback, the 'p.remDev' parameter is valid.  
 */
#define AVDTP_EVENT_DISCONNECT          3

/** This event is received by the application once for each stream discovered 
 *  on the remote device.  It is received after calling 
 *  AVDTP_DiscoverStreams().  
 * 
 *  During this callback, the 'p.streamInfo' parameter is valid.  
 */
#define AVDTP_EVENT_STREAM_INFO         4

/** Once all streams have been indicated to the application through the 
 *  AVDTP_EVENT_STREAM_INFO event, this event is received by the application 
 *  to indicate the completion of the discovery process.  
 */
#define AVDTP_EVENT_DISCOVER_CNF        5

/** After making a call to AVDTP_GetCapabilities(), this event is received by 
 *  the applications once for each capability advertised by the specified 
 *  stream.  
 * 
 *  During this callback, the 'p.capability' parameter is valid.  
 */
#define AVDTP_EVENT_CAPABILITY          6

/** Once all capabilities are received by the application after calling the 
 *  AVDTP_GetCapabilities() function, this event is received by the 
 *  application to indicated that all capabilities have been received.  
 */
#define AVDTP_EVENT_GET_CAP_CNF         7

/** After making a call to AVDTP_GetConfig(), this event is received by the 
 *  application once for each configured capability on the specified stream.  
 * 
 *  During this callback, the 'p.capability' parameter is valid.  
 */
#define AVDTP_EVENT_CFG_CAPABILITY      8

/** Once all capabilities are received by the application after calling the 
 *  AVDTP_GetConfig() function, this event is received by the application 
 *  to indicated that all capabilities have been received.  
 */
#define AVDTP_EVENT_GET_CFG_CNF         9

/** This event is received by the application when a request has been made by 
 *  the remote device to open a stream on the specified channel.  The 
 *  application can call the AVDTP_OpenStreamRsp() function to accept or 
 *  reject the request.  
 * 
 *  During this callback, the 'p.configReq' parameter is valid.  
 */
#define AVDTP_EVENT_STREAM_OPEN_IND     10

/** This event is received by the application when a stream is open.  This 
 *  can happen in response to a call from AVDTP_OpenStream() or 
 *  AVDTP_OpenStreamRsp().  
 * 
 *  During this callback, the 'p.configReq' parameter is valid.  
 */
#define AVDTP_EVENT_STREAM_OPEN         11

/** This event is received by the application when a request has been 
 *  received by the remote device to close an open stream.  
 */
#define AVDTP_EVENT_STREAM_CLOSE_IND    12

/** This event is received by the application when an open stream has been 
 *  closed.  This can happen as a result of a call to AVDTP_CloseStream(), a 
 *  call to AVDTP_Disconnect(), a link loss being detected by AVDTP, or the 
 *  remote device rejecting a request to open the stream.  If the stream is 
 *  being closed as a result of the channel being closed, all open streams 
 *  will receive this event before the AVDTP_EVENT_DISCONNECT event is 
 *  received.  
 * 
 *  During this callback, the 'p.capability' parameter will be valid if the 
 *  stream is closed because the configuration was rejected. The 
 *  'p.capability.type' field will be set to the capability type that was 
 *  rejected.  The 'error' field will contain the error code.  The 'len' 
 *  field will be non-zero if the 'p.capability' parameter is valid.  If the 
 *  remote device rejects the call to AVDTP_CloseStream(), the 'status' field 
 *  of the callback structure will be set to BT_STATUS_CANCELLED.  
 */
#define AVDTP_EVENT_STREAM_CLOSED       13

/** When the remote device requests streaming to begin, this event will be 
 *  received by the application.  The application can accept or reject 
 *  the streaming request by calling AVDTP_StartStreamRsp().  
 */
#define AVDTP_EVENT_STREAM_START_IND    14

/** When streaming has been started, this event will be received by the 
 *  application.  This can happen as the result to a call to 
 *  AVDTP_StartStream() or AVDTP_StartStreamRsp().  
 */
#define AVDTP_EVENT_STREAM_STARTED      15

/** When streaming has been suspended, this event will be received by the 
 *  application.  This happens as the result to a call to 
 *  AVDTP_SuspendStream().  
 */
#define AVDTP_EVENT_STREAM_SUSPENDED    16

/** When the remote device has requested that an open stream be reconfigured, 
 *  this even is received by the application.  The application can verify the 
 *  request by looking at the 'p.capability' parameter.  The application 
 *  responds to this request by calling AVDTP_ReconfigRsp().  If the 
 *  configuration is satisfactory, the application responds with the 'error' 
 *  parameter set to AVDTP_ERR_NO_ERROR, otherwise an appropriate error code 
 *  is used (see AvdtpError).  
 * 
 *  During this callback, the 'p.configReq' parameter will be valid.  
 */
#define AVDTP_EVENT_STREAM_RECONFIG_IND 17

/** When an open stream is reconfigured, this even is received by both the 
 *  initiator and acceptor of the reconfig request.  
 * 
 *  During this callback, the 'p.configReq' parameter is valid when the 
 *  'error' field is set to AVDTP_ERR_NO_ERROR.  Otherwise, 
 *  'p.capability.type' contains the capability that caused the failure.  
 */
#define AVDTP_EVENT_STREAM_RECONFIG_CNF 18

/** This event is received when the remote device requests the security 
 *  process to begin.  The application responds to this request by calling 
 *  AVDTP_SecurityControlRsp().  
 * 
 *  If 'error' contains no error, the 'p.data' parameter is valid.  
 */
#define AVDTP_EVENT_STREAM_SECURITY_IND 19

/** This event is received by the application when the remote device responds 
 *  to the security process request.  
 * 
 *  If 'error' contains no error, the 'p.data' parameter is valid.  
 */
#define AVDTP_EVENT_STREAM_SECURITY_CNF 20

/** When the stream is aborted, this event is received by the application.  
 *  This can happen in response to a request from the remote device to abort 
 *  the stream, or as the result of a call to AVDTP_AbortStream().  When a 
 *  stream is aborted, the stream is closed.  
 */
#define AVDTP_EVENT_STREAM_ABORTED      21

/** This event is received when stream data has been received from the remote 
 *  device.  The data is raw and is not parsed by AVDTP.  It should contain 
 *  a single media packet.  
 * 
 *  If 'error' contains no error, the 'p.data' parameter is valid.  
 */
#define AVDTP_EVENT_STREAM_DATA_IND     22

/** This event is received when raw data has been sent to the remote device.  
 *  This happens as the result of a call to AVDTP_StreamSendRawPacket().  
 * 
 *  During this callback, the 'p.packet' parameter is valid.  
 */
#define AVDTP_EVENT_STREAM_PACKET_SENT  23

/** This event is received when the Source device has acknowledged a Sink
 *  Delay command.  This happens as the result of a call to
 *  AVDTP_ReportSinkDelay() (only if the Delay Reporting feature is enabled).
 */
#define AVDTP_EVENT_STREAM_DELAY_CNF    24

/** This event is received when the Sink device has sent a Sink Delay command
 *  (only if the Delay Reporting feature is enabled).
 *  
 *  During this callback, the 'p.delayMs' parameter is valid.
 */
#define AVDTP_EVENT_STREAM_DELAY_IND    25

/* Confirms AVDTP has sent configuration information after an 
 * AVDTP_OpenStream() request.  
 *
 * During this callback, the 'p.configReq' parameter is valid when the 
 * 'error' field is set to AVDTP_ERR_NO_ERROR.  Otherwise, 
 * 'p.capability.type' contains the capability that caused the failure.  
 */
#define AVDTP_EVENT_SET_CFG_CNF         26

/* End of AvdtpEvent */ 

/*---------------------------------------------------------------------------
 * AvdtpError type
 * 
 * Error codes for the AVDTP protocol  
 */
typedef uint8_t AvdtpError;

/** No error occurred
 */
#define AVDTP_ERR_NO_ERROR                    0x00

/** Packet header format error
 */
#define AVDTP_ERR_BAD_HEADER_FORMAT           0x01

/** Bad length of a packet
 */
#define AVDTP_ERR_BAD_LENGTH                  0x11

/** Invalid acceptor SEID
 */
#define AVDTP_ERR_BAD_ACP_SEID                0x12

/** The stream is in use
 */
#define AVDTP_ERR_IN_USE                      0x13

/** The stream is not in use
 */
#define AVDTP_ERR_NOT_IN_USE                  0x14

/** Bad service category
 */
#define AVDTP_ERR_BAD_SERV_CATEGORY           0x17

/** Bad payload format
 */
#define AVDTP_ERR_BAD_PAYLOAD_FORMAT          0x18

/** Unsupported command
 */
#define AVDTP_ERR_NOT_SUPPORTED_COMMAND       0x19

/** Invalid capabilities
 */
#define AVDTP_ERR_INVALID_CAPABILITIES        0x1A

/** Bad recovery type
 */
#define AVDTP_ERR_BAD_RECOVERY_TYPE           0x22

/** Bad media transport format
 */
#define AVDTP_ERR_BAD_MEDIA_TRANSPORT_FORMAT  0x23

/** Bad recovery format
 */
#define AVDTP_ERR_BAD_RECOVERY_FORMAT         0x25

/** Bad format for header compression service capability
 */
#define AVDTP_ERR_BAD_ROHC_FORMAT             0x26

/** Bad content protection format
 */
#define AVDTP_ERR_BAD_CP_FORMAT               0x27

/** Bad multiplexing service format
 */
#define AVDTP_ERR_BAD_MULTIPLEXING_FORMAT     0x28

/** Unsupported configuration
 */
#define AVDTP_ERR_UNSUPPORTED_CONFIGURATION   0x29

/** Bad State
 */
#define AVDTP_ERR_BAD_STATE                   0x31

/** Codec media type not supported
 */
#define AVDTP_ERR_NOT_SUPPORTED_CODEC_TYPE    0xC2

/** Unknown error
 */
#define AVDTP_ERR_UNKNOWN_ERROR               0xFF

/* End of AvdtpError */

/*---------------------------------------------------------------------------
 * AvdtpCapabilityType type
 *
 * Defines the capability types for AVDTP.  
 */
typedef uint8_t AvdtpCapabilityType;

#define AVDTP_SRV_CAT_MEDIA_TRANSPORT      0x01
#define AVDTP_SRV_CAT_REPORTING            0x02
#define AVDTP_SRV_CAT_RECOVERY             0x03
#define AVDTP_SRV_CAT_CONTENT_PROTECTION   0x04
#define AVDTP_SRV_CAT_HEADER_COMPRESSION   0x05
#define AVDTP_SRV_CAT_MULTIPLEXING         0x06
#define AVDTP_SRV_CAT_MEDIA_CODEC          0x07
#define AVDTP_SRV_CAT_DELAY_REPORTING      0x08

/* End of AvdtpCapabilityType */ 

/*---------------------------------------------------------------------------
 * AvdtpStrmEndPointType type
 * 
 * Defines the stream endpoint types for AVDTP  
 */
typedef uint8_t AvdtpStrmEndPointType;

/* Stream Source */ 
#define AVDTP_STRM_ENDPOINT_SRC       0

/* Stream Sink */ 
#define AVDTP_STRM_ENDPOINT_SNK       1

/* End of AvdtpStrmEndPointType */ 

/*---------------------------------------------------------------------------
 * AvdtpCodecType type
 *
 * Defines codes types used by AVDTP.
 */
typedef uint8_t AvdtpCodecType;

#define AVDTP_CODEC_TYPE_SBC           0x00

#define AVDTP_CODEC_TYPE_MPEG1_2_AUDIO 0x01

#define AVDTP_CODEC_TYPE_MPEG2_4_AAC   0x02

#define AVDTP_CODEC_TYPE_ATRAC         0x04

#define AVDTP_CODEC_TYPE_H263          0x01

#define AVDTP_CODEC_TYPE_MPEG4_VSP     0x02

#define AVDTP_CODEC_TYPE_H263_PROF3    0x03

#define AVDTP_CODEC_TYPE_H263_PROF8    0x04

#define AVDTP_CODEC_TYPE_NON_A2DP      0xFF

/* End of AvdtpCodecType */

/*---------------------------------------------------------------------------
 * AvdtpMediaType type
 *
 * Defines the media types defined by AVDTP.
 */
typedef uint8_t AvdtpMediaType;

#define AVDTP_MEDIA_TYPE_AUDIO       0

#define AVDTP_MEDIA_TYPE_VIDEO       1

#define AVDTP_MEDIA_TYPE_MULTIMEDIA  2

/* End of AvdtpMediaType */ 

/*---------------------------------------------------------------------------
 * AvdtpContentProtType type
 *      
 * Defines the content protection supported in AVDTP.
 */
typedef uint16_t AvdtpContentProtType;

#define AVDTP_CP_TYPE_DTCP      0x0001

#define AVDTP_CP_TYPE_SCMS_T    0x0002

/* End of AvdtpContentProtType */ 


/*---------------------------------------------------------------------------
 * AvdtpScmsHeader type
 *      
 * Defines the bits used in a typical SCMS content protection header.
 */
typedef uint8_t AvdtpScmsHeader;

/* When set, the signal carried by AVDTP has no copyright asserted and may
 * be recorded freely. If not set, copyright is asserted, and a local
 * copy may be made only if the AVDTP_SCHM_PRERECORDED bit is set.
 */
#define AVDTP_SCMS_CONTENT_UNPROTECTED 0x01

/* When set, the signal carried by AVDTP corresponds to the original signal
 * and not a copy. When making a copy of this signal, the prerecorded bit
 * should be cleared to indicate it is a copy.
 */
#define AVDTP_SCMS_PRERECORDED         0x02     

/* End of AvdtpScmsHeader */

/*---------------------------------------------------------------------------
 * AvdtpStreamId type
 *      
 * Defines a stream Identifier.
 */
typedef uint8_t AvdtpStreamId;

/* End of AvdtpStreamId */ 

/*---------------------------------------------------------------------------
 * AvdtpVersion type
 *
 *     Describes the version of the AVDTP protocol (0x0100 means version 1.0).
 */
typedef uint16_t AvdtpVersion;

/* End of AvdtpVersion */ 

/*---------------------------------------------------------------------------
 * AvdtpStreamState type
 *
 * This type defines the state of the stream.  
 */
typedef uint8_t AvdtpStreamState;

/** The stream is idle and not configured.  Streaming is not possible.  
 */
#define AVDTP_STRM_STATE_IDLE           0

/** A stream is configured, but not open.  This state will only occur in 
 *  certain cases where a request to open the stream is rejected, and the 
 *  operation cannot be aborted.  
 * 
 *  AVDTP_AbortStream() must be called to exit this state.  
 */
#define AVDTP_STRM_STATE_CONFIGURED     1

/** The stream is open and configured.  Streaming can be initiated after the 
 *  stream is open by calling AVDTP_StartStream().  
 */
#define AVDTP_STRM_STATE_OPEN           2

/** The stream is active.  Stream data (media packets) can be sent only in 
 *  this state.  
 */
#define AVDTP_STRM_STATE_STREAMING      3

/** The stream is in the process of closing.  
 */
#define AVDTP_STRM_STATE_CLOSING        4

/** The stream is in the process of aborting.  
 */
#define AVDTP_STRM_STATE_ABORTING       5

/* End of AvdtpStreamState */ 

/****************************************************************************
 *
 * Data Structures
 *
 ****************************************************************************/

/*---------------------------------------------------------------------------
 * AvdtpStreamInfo structure
 *
 * Used in the stream discovery procedure to provide stream information.
 */
typedef struct _AvdtpStreamInfo {
    AvdtpStreamId          id;         /* The stream identifier */ 

    int                   inUse;      /* Indicates whether the stream is 
                                        * currently in use
                                        */

    AvdtpMediaType         mediaType;  /* Defines the media type of the 
                                        * stream 
                                        */ 

    AvdtpStrmEndPointType  streamType; /* Defines the role of the stream 
                                        * endpoint
                                        */
} AvdtpStreamInfo;

/*---------------------------------------------------------------------------
 * AvdtpContentProt structure
 *
 * Contains information about the stream content protection.  
 */
typedef struct _AvdtpContentProt {
    ListEntry  node;                   /* Used internally by AVDTP */ 

    AvdtpContentProtType cpType;       /* Defines the content protection 
                                        * type 
                                        */ 
    uint8_t                   dataLen;      /* The length of the security data */ 
    uint8_t                  *data;         /* Security data */ 
} AvdtpContentProt;

/*---------------------------------------------------------------------------
 * AvdtpCodec structure
 *
 * Used to describe the codec type and elements.
 */
typedef struct _AvdtpCodec {
    AvdtpCodecType  codecType;         /* The type of codec */ 
    uint8_t              elemLen;           /* Length of the codec elements */ 
    uint8_t             *elements;          /* Codec Elements */
     
    int            discoverable;      /* Used internally by AVDTP */
} AvdtpCodec;

/*---------------------------------------------------------------------------
 * AvdtpCapability structure
 *
 * Used to describe a stream capability.
 */
typedef struct _AvdtpCapability {

    AvdtpCapabilityType type;    /* The capability type */ 

    union {
        AvdtpCodec        codec; /* Codec capability.  Valid when type = 
                                  * AVDTP_SRV_CAT_MEDIA_CODEC 
                                  */ 
        
        AvdtpContentProt  cp;    /* Content protection capability. Valid
                                  * when type = AVDTP_SRV_CAT_CONTENT_PROTECTION 
                                  */ 
    } p;
} AvdtpCapability;

/*---------------------------------------------------------------------------
 * AvdtpConfigRequest structure
 *
 * Used to describe a configuration request in an AVDTP_EVENT_STREAM_OPEN_IND 
 * and an AVDTP_EVENT_RECONFIG_IND event.  
 */
typedef struct _AvdtpConfigRequest {
    AvdtpCodec       codec;            /* Codec information requested */ 
    AvdtpContentProt cp;               /* Content protection requested */ 
    int             delayReporting;   /* Delay reporting */
} AvdtpConfigRequest;


typedef struct _AvdtpConn AvdtpConn;
typedef struct _AvdtpStream AvdtpStream;
typedef struct _AvdtpCallbackParms AvdtpCallbackParms;

/*---------------------------------------------------------------------------
 * L2capFlowParms structure
 *
 *     Specifies the Enhanced L2CAP mode parameters for the PSM or Channel.
 */  
typedef struct _L2capFlowParms
{
    /* Requested operating modes for channels opened for this PSM. For
     * maximum compatiblity, this should include L2MODE_BASIC. If basic is
     * not included, connections to Bluetooth v1.1 devices will fail. An
     * L2CAP Information request may be issued to retrieve the extended
     * features mask to ascertain the remote devices capabilities. If the
     * PSM does not permit Enhanced L2CAP modes added in the Bluetooth v2.1
     * addendum, set this parameter to L2MODE_BASIC.
     *
     * The inbound and outbound link modes must be symmetric. For example:
     * if inbound packets can use Enhanced Retranmission mode, outbound data 
     * must use Enhanced Retransmission mode also. If the link is configured 
     * to use basic mode in one direction, it must use basic mode in the other 
     * direction as well. 
     */

    /* Specify the acceptable modes for the link. Both incoming and outgoing
     * links have the same mode since supported link modes are symmetric.
     */
    uint8_t   linkMode;

    /* Specify the preferred link mode for the link. L2CAP will begin
     * negotiating the channel configuration with this mode if more than
     * one mode is specified in "linkMode".
     */
    uint8_t   prefLinkMode;

    /* The size of the send window for retransmission modes. */
    uint8_t              txWindow;

    /* The size of the receive window for retransmission modes. */
    uint8_t              rxWindow;

    /* The maximum number of times the remote L2CAP can transmit a packet
     * before considering the channel lost. The minimum value of 1, indicates
     * no retransmissions shall be sent. Note that if the retransmit timer
     * expires and the maxTransmit value has been reached, the channel will
     * be disconnected. This value is not used by basic mode connections.
     */
    uint8_t              maxTransmit;

    /* The retransmission timeout to be used by the remote device when 
     * retransmission mode is enabled. This timeout should be set fairly
     * high in order to avoid unnecessary retransmissions due to delayed
     * acknowledgements. Suitable values, specified in milliseconds, could
     * be 100�s of milliseconds or higher. This value is not used by basic
     * mode connections.
     */
    uint16_t             retransTimeout;

    /* The monitor timeout to be used by the remote device when no activity
     * is present on the channel. It serves to ensure that lost acknowledgements
     * are retransmitted. Suitable values, specified in milliseconds, could
     * be 100�s of milliseconds or higher. This value is not used by basic
     * mode connections.
     */
    uint16_t             monitorTimeout;

    /* The FCS Option allows configuration of a channel with a preference
     * for no Frame Check Sequence (FCS) in the L2CAP packet. If this option
     * is L2FCS_NONE, the configuration will specify no FCS. Note that if either
     * side of the link requests FCS in a configuration request, then
     * FCS will be used for the link.
     */
    uint16_t  fcsOption;

    /* The txMps defines the maximum PDU size the local channel can send.
     * Each channel specifies this value separately. Note that L2CAP will
     * adjust this to be less than or equal to the remote device's rxMps.
     */
    uint16_t             txMps;

    /* The rxMps defines the maximum PDU size the local channel can accept.
     * Each channel specifies this value separately.
     */
    uint16_t             rxMps;

} L2capFlowParms;


/* Avdtp Connection State */ 
struct _AvdtpConn {
    uint16_t    l2ChannelId;
    uint8_t                state;
    void *callback;
    void   *remDev;

#if L2CAP_NUM_ENHANCED_CHANNELS > 0
    L2capFlowParms    flowParms;
#endif
};

/*---------------------------------------------------------------------------
 * AvdtpStream structure
 *
 * Defines a stream.
 */
struct _AvdtpStream {
    ListEntry              node;           /* Used internally by AVDTP */ 

    AvdtpMediaType         mediaType;     /* Stream Media Type */ 

    AvdtpStrmEndPointType  endPointType;  /* Stream Endpoint Type */ 

    /* === Internal use only === */ 

    /* Sink delay in milliseconds */
    uint16_t               delayMs;

    /* Stream Identifiers */ 
    AvdtpStreamId          locStrmId;
    AvdtpStreamId          remStrmId;

    /* Stream State */ 
    AvdtpStreamState       state;
    uint16_t               flags;

    /* Connection Handle */ 
    AvdtpConn              conn;

    /* Registered Capabilities */ 
    AvdtpCodec            *codec;
    ListEntry              cpList;

    /* Configured Capabilities */ 
    AvdtpContentProt       cpCfg;
    uint8_t                cpCfgValue[AVDTP_MAX_CP_VALUE_SIZE];
    AvdtpCodec             codecCfg;
    uint8_t                codecCfgElem[AVDTP_MAX_CODEC_ELEM_SIZE];
    int                    delayRepCfg;

    /* Requested Capabilities */ 
    AvdtpConfigRequest     cfgReq;
    uint8_t                cpReqValue[AVDTP_MAX_CP_VALUE_SIZE];
    uint8_t                codecReqElem[AVDTP_MAX_CODEC_ELEM_SIZE];

    /* Current Channel Allocation */ 
    void          *chnl;

    /* Last close status */ 
    AvdtpError             abError;
};

/*---------------------------------------------------------------------------
 * AvdtpCallbackParms structure
 *
 * Contains information about the application callback event.
 */
struct _AvdtpCallbackParms {
    AvdtpEvent    event;         /* The Callback Event */ 
    void         *channel;       /* Channel associated with this event */ 
    AvdtpStream  *stream;        /* Stream (if any) associated with this 
                                  * event 
                                  */ 
    uint16_t      len;           /* Length of any event data */ 
    BtStatus      status;        /* Transport status */ 
    AvdtpError    error;         /* Event error code */ 

    union {
        void               *remDev;     /* Device associated with the 
                                         * event 
                                         */ 
        uint8_t            *data;       /* Data associated with the event */ 
        void               *packet;     /* Packet associated with the 
                                         * event 
                                         */ 
        AvdtpStreamInfo    *streamInfo; /* Stream information */ 
        AvdtpCapability    *capability; /* Stream capability */ 
        AvdtpContentProt   *cp;         /* Content protection */ 
        AvdtpConfigRequest *configReq;  /* Configuration request */ 
        uint16_t            delayMs;    /* Delay report value */
    } p;
};

/*---------------------------------------------------------------------------
 * AvdtpMediaHeader structure
 *
 * Defines the fields of the Media Packet Header.  This structure can be
 * be used to create a media packet header before transmitting media
 * data using the AVDTP_StreamSendRawPacket().
 */
typedef struct _AvdtpMediaHeader {
    uint8_t version;         /* RTP Version */ 

    uint8_t padding;         /* If the padding bit is set, the packet contains 
                         * one or more additional padding octets at the end, 
                         * which are not parts of the payload.  The last 
                         * octet of the padding contains a count of how many 
                         * padding octets should be ignored.  
                         */

    uint8_t marker;          /* Profile dependent.  Used to mark significant 
                         * events such as frame boundaries in the packet 
                         * stream.  
                         */

    uint8_t payloadType;     /* Profile dependent.  Identifies the RTP payload 
                         * type.  
                         */

    uint16_t sequenceNumber; /* Incremented by one for each packet sent */ 

    uint32_t timestamp;      /* Time stamp of the sample */ 

    uint32_t ssrc;           /* Synchronization source */ 

    uint8_t csrcCount;       /* The number of CSRC (Contributing Source) 
                         * identifiers that follow the fixed header.  
                         */

    uint32_t csrcList[15];  /* List of CSRC identifiers */ 

} AvdtpMediaHeader;

typedef struct _AvdtpConn AvdtpConn;

#define MAX_CAPABILITY_SIZE  256

/* Parser state */
typedef struct _AvdtpParser {
    /* === Internal use only === */ 
    uint8_t  stageBuff[MAX_CAPABILITY_SIZE];  /* Staging buffer            */ 
    uint8_t  stageLen;                        /* Length of staged data     */ 
    uint8_t  curStageOff;                     /* Offset into staged data   */ 
    uint8_t *rxBuff;                          /* The Received buffer       */ 
    uint16_t rxSize;                          /* Remaining rx buff size    */ 
    uint8_t  rxState;                         /* Parser state              */ 
} AvdtpParser;
/*---------------------------------------------------------------------------
 * AvdtpChannel structure
 *
 * Defines the channel for sending stream signaling information.  
 */
struct _AvdtpChannel {
    /* === Internal use only === */ 
    ListEntry       node;

    /* Signal Connection Handle */ 
    AvdtpConn       conn;

    /* Signal Channel Handle */ 
    AvtpChannel     sigChnl;

    /* Transaction ID */ 
    uint8_t              txTransId;
    uint8_t              rxTransId;

    /* Signal Packets */ 
    AvtpPacket     *cmdSigPacket;
    AvtpPacket     *rspSigPacket;

    /* Active streams */ 
    ListEntry       streamList;
    AvdtpStream    *currentTxStream;
    AvdtpStream    *currentRxStream;

    /* Number of streams left to start/suspend */ 
    uint16_t       streamsLeft;
    int            strmStartRspFlag;

    /* Signal Receive State */ 
    uint8_t        rxState;

    /* Signal Transmit State */ 
    uint8_t        txState;

    /* Parser state information */ 
    AvdtpParser    parser;

    /* Avdtp Capability and Config Request Allocation used in callback */ 
    AvdtpCapability capability;

    /* Application callback function */ 
    void *   callback;
};
typedef struct _AvdtpChannel AvdtpChannel;

/****************************************************************************
 *
 * Function Reference

 *
 ****************************************************************************/
#if 0
/*---------------------------------------------------------------------------
 * AVDTP_Init()
 *
 *     Initialize the AVDTP SDK.  This function should only be called 
 *     once, normally at initialization time.  The calling of this function 
 *     can be specified in overide.h using the XA_LOAD_LIST macro 
 *     (i.e. #define XA_LOAD_LIST XA_MODULE(AVDTP) ... ).  
 *
 * Returns:
 *     TRUE - Initialization was successful.  
 *
 *     FALSE - Initialization failed.  
 */
int AVDTP_Init(void);

/*---------------------------------------------------------------------------
 * AVDTP_Deinit()
 *
 *     Deinitializes the AVDTP SDK. Must be called only when AVDTP
 *     has no active connections.
 */
void AVDTP_Deinit(void);

/*---------------------------------------------------------------------------
 * AVDTP_Register()
 *
 *      Registers an application callback to receive AVDTP events. This 
 *      function must be called before any other AVDTP functions.  Multiple 
 *      channel structures can be registered.  Once registered, the channel 
 *      structure's memory is "owned" by AVDTP.  This memory must not be 
 *      changed or freed.  
 *
 * Parameters:
 *
 *     Chnl - An AVDTP channel structure.  
 *
 *     Callback - Identifies the application function that will be called 
 *         with AVDTP events.  
 *
 * Returns:
 *      BT_STATUS_SUCCESS - The AVDTP application callback Function was 
 *      successfully registered.  
 *
 *      BT_STATUS_IN_USE - The specified channel is already registered.  
 *
 *      BT_STATUS_INVALID_PARM - The Chnl or Callback parameter does not 
 *          contain a valid pointer. (XA_ERROR_CHECK only).  
 */
BtStatus AVDTP_Register(void *Chnl, AvdtpCallback Callback);

/*---------------------------------------------------------------------------
 * AVDTP_Deregister()
 *
 *      Deregisters the AVDTP callback. After making this call 
 *      successfully, the callback specified in AVDTP_Register will 
 *      receive no further events.  In addition, the memory used by the 
 *      channel structure can be changed or freed.  
 *
 * Parameters:
 *
 *     Chnl - A registered and open AVDTP channel.  
 *
 * Returns:
 *      BT_STATUS_SUCCESS - The AVDTP callback was successfully 
 *          deregistered.  
 * 
 *      BT_STATUS_BUSY - The specified channel is connected.  
 *
 *      BT_STATUS_NOT_FOUND - An AVDTP callback was not previously 
 *          registered.  
 *     
 *      BT_STATUS_INVALID_PARM - The Chnl parameter does not contain a valid 
 *         pointer. (XA_ERROR_CHECK only).  
 */
BtStatus AVDTP_Deregister(void *Chnl);

/*---------------------------------------------------------------------------
 * AVDTP_Connect()
 * 
 *     Initiates a connection to a remote AVDTP device.  This function is 
 *     used to establish the lower layer connection (L2CAP), which allows 
 *     sending signaling messages, such as discover, configuration, and 
 *     stream management.  
 *
 * Parameters:
 *
 *     Chnl - A registered and open AVDTP channel.  
 *
 *     RemDev - A remote device structure to a connected device.  
 *
 * Returns:
 *
 *     BT_STATUS_PENDING - The connection process has been successfully 
 *         initialized. When the connection process is complete, the 
 *         application callback will receive either the AVDTP_EVENT_CONNECT 
 *         or AVDTP_EVENT_DISCONNECT event.  
 *
 *     BT_STATUS_BUSY - The connection is already connected.  
 *
 *     BT_STATUS_INVALID_PARM - The Chnl or Addr parameter does not contain a 
 *         valid pointer. (XA_ERROR_CHECK only).  
 *
 *     BT_STATUS_NOT_FOUND - The specified device was not found in the device 
 *         selector database.  The device must be discovered, paired, or 
 *         added manually using DS_AddDevice().  
 *
 *     Other - It is possible to receive other error codes, depending on the 
 *         lower layer service in use (L2CAP or Management Entity).  
 */
BtStatus AVDTP_Connect(void *Chnl, BtRemoteDevice *RemDev);

/*---------------------------------------------------------------------------
 * AVDTP_ConnectRsp()
 * 
 *     Responds to a connection request from the remote AVDTP device.  This 
 *     function is used to establish the lower layer connection (L2CAP), 
 *     which allows sending signaling messages, such as discover, 
 *     configuration, and stream management.  
 *
 * Parameters:
 *
 *     Chnl - A registered and open AVDTP channel.  
 *
 *     Accept - TRUE accepts the connect or FALSE rejects the connection.  
 *
 * Returns:
 *
 *     BT_STATUS_PENDING - The connection responses has been successfully 
 *         sent. When the connection process is complete, the application 
 *         callback will receive the AVDTP_EVENT_CONNECT event.  
 *
 *     BT_STATUS_BUSY - The connection is already connected.  
 *
 *     BT_STATUS_INVALID_PARM - The Chnl parameter does not contain a 
 *         valid pointer. (XA_ERROR_CHECK only).  
 *
 *     BT_STATUS_NOT_FOUND - The specified device was not found in the device 
 *         selector database.  The device must be discovered, paired, or 
 *         added manually using DS_AddDevice().  
 *
 *     Other - It is possible to receive other error codes, depending on the 
 *         lower layer service in use (L2CAP or Management Entity).  
 */
BtStatus AVDTP_ConnectRsp(void *Chnl, int Accept);

/*---------------------------------------------------------------------------
 * AVDTP_Disconnect()
 *
 *     Terminates a connection with a remote AVDTP device.  The lower layer 
 *     connection (L2CAP) is disconnected and signaling is no longer possible 
 *     on the specified channel.  
 *
 * Parameters:
 *
 *     Chnl - A registered and open AVDTP channel.  
 *
 * Returns:
 *
 *     BT_STATUS_PENDING - The disconnect process has been successfully 
 *         initialized. When the disconnect process is complete, the 
 *         application callback will receive the AVDTP_EVENT_DISCONNECT 
 *         event.  
 *
 *     BT_STATUS_INVALID_PARM - The Chnl parameter does not contain a valid 
 *         pointer. (XA_ERROR_CHECK only).  
 *
 *     BT_STATUS_NO_CONNECTION - No connection exists on the specified 
 *         channel.  
 *
 *     BT_STATUS_NOT_FOUND - The specified device was not found in the device 
 *         selector database.  The device must be discovered, paired, or 
 *         added manually using DS_AddDevice().  
 *
 *     It is possible to receive other error codes, depending on the lower 
 *     layer service in use (L2CAP or Management Entity).  
 */
BtStatus AVDTP_Disconnect(void *Chnl);

/*---------------------------------------------------------------------------
 * AVDTP_RegisterStream()
 *
 *     Registers a stream with the specified capabilities.  The Stream 
 *     structure must be initialized with the proper media type and 
 *     end point type.  Once registered, remote devices can discover and open 
 *     the stream.  In addition, streams must be registered before being used 
 *     to initiate streaming requests from the local device.  Once 
 *     registered, the stream structure's memory is "owned" by AVDTP.  This 
 *     memory must not be changed or freed.  
 *
 * Parameters:
 *
 *     Stream - An initialized AVDTP stream structure.  The 'mediaType' and 
 *         'endPointType' fields must be initialized.  
 *
 *     Codec - An initialized AVDTP codec structure.  All fields must be 
 *         initialized.  This structure advertises the codec capabilities on 
 *         the local device.  
 *
 * Returns:
 *
 *     BT_STATUS_SUCCESS - The stream was successfully registered.  
 *
 *     BT_STATUS_INVALID_PARM - The Stream or Codec parameter does not 
 *         contain a valid pointer. (XA_ERROR_CHECK only).  
 *
 *     BT_STATUS_NO_RESOURCES - The maximum number of streams have been 
 *         allocated.  
 *
 *     BT_STATUS_IN_USE - The specified stream is already registered.  
 */
BtStatus AVDTP_RegisterStream(AvdtpStream *Stream, AvdtpCodec *Codec);

/*---------------------------------------------------------------------------
 * AVDTP_DeregisterStream()
 *
 *     Deregisters a stream.  Once deregistered, the stream can no longer be 
 *     used for any operations.  In addition, the memory used by the stream 
 *     structure can be changed or freed.  
 *
 * Parameters:
 *
 *     Stream - A registered AVDTP stream.  
 *
 * Returns:
 *
 *     BT_STATUS_SUCCESS - The stream was successfully deregistered.  
 *
 *     BT_STATUS_INVALID_PARM - The Stream parameter does not contain 
 *         a valid pointer. (XA_ERROR_CHECK only).  
 *
 *     BT_STATUS_NOT_FOUND - The specified stream has not been registered.  
 *
 *     BT_STATUS_IN_USE - The specified stream is not idle.  
 */
BtStatus AVDTP_DeregisterStream(AvdtpStream *Stream);

/*---------------------------------------------------------------------------
 * AVDTP_AddContentProtection()
 *
 *     Adds content protection capabilities to a registered stream.  Multiple 
 *     capabilities can be added to a single stream.  These structures are 
 *     used to advertise the types of content protection available on this 
 *     device.  Once registered, the content protection structure's memory is 
 *     "owned" by AVDTP.  This memory must not be changed or freed.  Once a 
 *     stream is deregistered the content protection structure memory can be 
 *     changed or freed.
 *
 * Parameters:
 *
 *     Stream - A registered AVDTP stream.  
 *
 *     Cp - Structure that defines the type of content protection supported 
 *         by the stream.  
 *
 * Returns:
 *
 *     BT_STATUS_SUCCESS - The content protection was successfully 
 *         registered.  
 *
 *     BT_STATUS_INVALID_PARM - The Stream or Cp parameter does not contain 
 *         a valid pointer. (XA_ERROR_CHECK only).  
 *
 *     BT_STATUS_NOT_FOUND - The specified stream has not been registered.  
 *
 *     BT_STATUS_IN_USE - The content protection structure is in use.  
 *
 *     BT_STATUS_BUSY - The stream is not idle.  
 */
BtStatus AVDTP_AddContentProtection(AvdtpStream      *Stream, 
                                    AvdtpContentProt *Cp);

/*---------------------------------------------------------------------------
 * AVDTP_DiscoverStreams()
 *
 *     Initiates a discovery for streams on the remote device.  Both streams 
 *     that are in use and streams that are not in use will be enumerated.  
 *
 * Parameters:
 *
 *     Chnl - A registered and open AVDTP channel.  
 *
 * Returns:
 *
 *     BT_STATUS_PENDING - The Discovery operation was started successfully.  
 *         An AVDTP_EVENT_STREAM_INFO event will be received for each stream 
 *         discovered on the remote device.  When all streams have been 
 *         discovered, an AVDTP_EVENT_DISCOVER_CNF event will be received.  
 *
 *     BT_STATUS_INVALID_PARM - The 'Chnl' parameter does not contain a 
 *         valid pointer. (XA_ERROR_CHECK only).  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel is not registered.  
 *
 *     BT_STATUS_NO_CONNECTION - No connection exists on this channel.  
 *
 *     Other - It is possible to receive other error codes, depending on the 
 *         lower layer service in use (L2CAP or Management Entity).  
 */
BtStatus AVDTP_DiscoverStreams(void *Chnl);

/*---------------------------------------------------------------------------
 * AVDTP_GetCapabilities()
 *
 *     Discovers the capabilities (basic) of a stream on the remote device.  
 *     Capabilities can be discovered whether the stream is in use or not.  
 *
 * Parameters:
 *
 *     Chnl - A registered and open AVDTP channel.  
 *
 *     StreamId - The ID of a discovered stream.  
 *
 * Returns:
 *
 *     BT_STATUS_PENDING - The Get Capabilities operation was started 
 *         successfully.  An AVDTP_EVENT_CAPABILITY event will be received 
 *         for each capability supported by the remote device.  When all 
 *         capabilities have been discovered, an AVDTP_EVENT_GET_CAP_CNF 
 *         event will be received.
 *
 *     BT_STATUS_INVALID_PARM - The 'Chnl' parameter does not contain a 
 *         valid pointer. (XA_ERROR_CHECK only).  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel is not registered.  
 *
 *     BT_STATUS_NO_CONNECTION - No connection exists on this channel.  
 *
 *     Other - It is possible to receive other error codes, depending on the 
 *         lower layer service in use (L2CAP or Management Entity).  
 */
BtStatus AVDTP_GetCapabilities(void *Chnl, AvdtpStreamId StrmId);

/*---------------------------------------------------------------------------
 * AVDTP_GetAllCapabilities()
 *
 *     Discovers the capabilities (basic and extended) of a stream on the
 *     remote device. Capabilities can be discovered whether the stream is in
 *     use or not.
 *
 * Parameters:
 *
 *     Chnl - A registered and open AVDTP channel.  
 *
 *     StreamId - The ID of a discovered stream.  
 *
 * Returns:
 *
 *     BT_STATUS_PENDING - The Get Capabilities operation was started 
 *         successfully.  An AVDTP_EVENT_CAPABILITY event will be received 
 *         for each capability supported by the remote device.  When all 
 *         capabilities have been discovered, an AVDTP_EVENT_GET_CAP_CNF 
 *         event will be received.
 *
 *     BT_STATUS_INVALID_PARM - The 'Chnl' parameter does not contain a 
 *         valid pointer. (XA_ERROR_CHECK only).  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel is not registered.  
 *
 *     BT_STATUS_NO_CONNECTION - No connection exists on this channel.  
 *
 *     Other - It is possible to receive other error codes, depending on the 
 *         lower layer service in use (L2CAP or Management Entity).  
 */
BtStatus AVDTP_GetAllCapabilities(AvdtpChannel *Chnl, AvdtpStreamId StrmId);

/*---------------------------------------------------------------------------
 * AVDTP_GetConfig()
 *
 *     Discovers the configured capabilities of an open stream on the 
 *     remote device.  
 *
 * Parameters:
 *
 *     Stream - A registered and open AVDTP stream.  
 *
 * Returns:
 *
 *     BT_STATUS_PENDING - The Get Config operation was started 
 *         successfully.  An AVDTP_EVENT_CAPABILITY event will be received 
 *         for each capability configured on the remote device.  When all 
 *         capabilities have been discovered, an AVDTP_EVENT_GET_CFG_CNF 
 *         event will be received.
 *
 *     BT_STATUS_INVALID_PARM - The 'Chnl' or 'Stream' parameter does not 
 *         contain a valid pointer. (XA_ERROR_CHECK only).  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel is not registered.  
 *
 *     BT_STATUS_NO_CONNECTION - The stream is not allocated on a channel.  
 *
 *     Other - It is possible to receive other error codes, depending on the 
 *         lower layer service in use (L2CAP or Management Entity).  
 */
BtStatus AVDTP_GetConfig(AvdtpStream *Stream);

/*---------------------------------------------------------------------------
 * AVDTP_OpenStream()
 *
 *    Opens and configures a stream on the remote device.  The type of 
 *    content protection to use (if any) must be specified.  
 *
 * Parameters:
 *
 *     Chnl - A registered and open AVDTP channel.  
 *
 *     Stream - A registered and configured AVDTP stream.  
 *
 *     StrmId - ID of remote stream to open. This value is selected from the 
 *         set of discovered streams indicated by the AVDTP_EVENT_STREAM_INFO 
 *         event.  
 *
 *     Codec - Structure that defines the specific codec type to use for this 
 *         stream, and the specific codec elements to use.  Selected codec 
 *         elements must not be ambiguous.  For example, only one sampling 
 *         frequency should be selected.  
 *
 *     Cp - Structure that defines the type of content protection to 
 *         configure on the remote device.  If no content protection is 
 *         desired, this parameter may be set to 0.
 * 
 *     delayReporting - Indicates whether or not to enable delayReporting
 *         feature.
 * 
 *     delayMs - A value representing the delay in the Sink  
 *         buffering/processing.  If the Stream is a Source, this value is
 *         ignored.
 *
 * Returns:
 *
 *     BT_STATUS_PENDING - The Open Stream operation was started 
 *         successfully.  An AVDTP_EVENT_STREAM_OPEN event will be received 
 *         when the stream is open.  
 *
 *     BT_STATUS_INVALID_PARM - The 'Chnl', 'Stream', or 'Cp' parameter does 
 *         not contain a valid pointer. (XA_ERROR_CHECK only).  
 *
 *     BT_STATUS_IN_USE - The specified stream is already in use.  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel or stream is not 
 *         registered.  
 *
 *     BT_STATUS_NO_CONNECTION - The channel is not connected.  
 *
 *     Other - It is possible to receive other error codes, depending on the 
 *         lower layer service in use (L2CAP or Management Entity).  
 */
BtStatus AVDTP_OpenStream(AvdtpChannel     *Chnl, 
                          AvdtpStream      *Stream,
                          AvdtpStreamId     StrmId, 
                          AvdtpCodec       *Codec,
                          AvdtpContentProt *Cp,
                          int              delayReporting,
                          uint16_t               delayMs);

/*---------------------------------------------------------------------------
 * AVDTP_OpenStreamRsp()
 *
 *     Called in response to the AVDTP_EVENT_STEAM_OPEN_IND event.  Calling 
 *     this function will either accept or reject the connection.  
 *
 * Parameters:
 *
 *     Chnl - A registered and open AVDTP channel.  
 *
 *     Stream - A registered and configured AVDTP stream.  
 *
 *     Error - If the connection request is accepted, this parameter must be 
 *         set to AVDTP_ERR_NO_ERROR.  If the connection request is rejected, 
 *         this parameter must be set to the appropriate error defined by 
 *         AvdtpError.  
 *
 *     CapType - Capability type that was the reason for rejecting the 
 *         connection.  If 'Error' is set to AVDTP_ERR_NO_ERROR, this 
 *         parameter is ignored.  
 *
 *     delayMs - A value representing the delay in the Sink
 *         buffering/processing.  If the Stream is a Source, this value is
 *         ignored.
 *
 * Returns:
 *
 *     BT_STATUS_PENDING - The Accept Stream operation was started 
 *         successfully.  An AVDTP_EVENT_STREAM_OPEN event will be received 
 *         when the stream is open.  
 *
 *     BT_STATUS_INVALID_PARM - The 'Chnl' or 'Stream' parameter does not 
 *         contain a valid pointer. (XA_ERROR_CHECK only).  
 *
 *     BT_STATUS_IN_USE - The specified stream is already in use.  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel or stream is not 
 *         registered.  
 *
 *     BT_STATUS_NO_CONNECTION - The stream is not allocated on a channel.  
 *
 *     Other - It is possible to receive other error codes, depending on the 
 *         lower layer service in use (L2CAP or Management Entity).  
 */
BtStatus AVDTP_OpenStreamRsp(AvdtpChannel       *Chnl, 
                             AvdtpStream        *Stream, 
                             AvdtpError          Error, 
                             AvdtpCapabilityType CapType,
                             uint16_t                 delayMs);

/*---------------------------------------------------------------------------
 * AVDTP_CloseStream()
 *
 *     Initiate the closing an open stream.  
 *
 * Parameters:
 *
 *     Stream - A registered and open AVDTP stream.  
 *
 * Returns:
 *
 *     BT_STATUS_PENDING - The Close Stream operation was started 
 *         successfully.  An AVDTP_EVENT_STREAM_CLOSED event will be received 
 *         when the stream is closed.  
 *
 *     BT_STATUS_FAILED - The stream is not in the open or active state.  
 *
 *     BT_STATUS_INVALID_PARM - The Stream parameter does not 
 *         contain a valid pointer. (XA_ERROR_CHECK only).  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel or stream is not 
 *         registered.  
 *
 *     BT_STATUS_NO_CONNECTION - The stream is not allocated on a channel.  
 *
 *     Other - It is possible to receive other error codes, depending on the 
 *         lower layer service in use (L2CAP or Management Entity).  
 */
BtStatus AVDTP_CloseStream(AvdtpStream *Stream);

/*---------------------------------------------------------------------------
 * AVDTP_Reconfig()
 *
 *     Reconfigure an open stream.  This call can be made only if the stream 
 *     is in the open state.  In other words, a stream that is currently 
 *     streaming audio/video must be suspended before trying to reconfigure 
 *     it.  
 *
 * Parameters:
 *
 *     Stream - A registered and open AVDTP stream.  
 *
 *     Codec - Structure that defines the specific codec type to use for this 
 *         stream, and the specific codec elements to use.  Selected codec 
 *         elements must not be ambiguous.  For example, only one sampling 
 *         frequency should be selected.  
 *
 *     Cp - Structure that defines the type of content protection to 
 *         configure on the remote device.  If no content protection is 
 *         desired, this parameter may be set to NULL.  
 *
 * Returns:
 *
 *     BT_STATUS_PENDING - The Reconfig operation was started successfully.  
 *         An AVDTP_EVENT_STREAM_RECONFIG_CNF event will be received when the 
 *         stream is reconfigured.  
 *
 *     BT_STATUS_FAILED - The stream is not in the open state.  
 *
 *     BT_STATUS_INVALID_PARM - The Stream or Cp parameter does not 
 *         contain a valid pointer. (XA_ERROR_CHECK only).  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel or stream is not 
 *         registered.  
 *
 *     BT_STATUS_NO_CONNECTION - The stream is not allocated on a channel.  
 *
 *     Other - It is possible to receive other error codes, depending on the 
 *         lower layer service in use (L2CAP or Management Entity).  
 */
BtStatus AVDTP_Reconfig(AvdtpStream      *Stream, 
                        AvdtpCodec       *Codec,
                        AvdtpContentProt *Cp);

/*---------------------------------------------------------------------------
 * AVDTP_ReconfigRsp()
 *
 *     Respond to a request by the remote device to reconfigure the open 
 *     open stream.  This call can be made only in the open state, and is 
 *     made in response to the AVDTP_EVENT_STREAM_RECONFIG_IND.  
 *
 * Parameters:
 *
 *     Stream - A registered and open AVDTP stream.  
 *
 *     Error - An appropriate AVDTP error code.  
 *
 *     Type - Capability type that caused an error (if any).  This value 
 *            is ignored if 'Error' is set to AVDTP_ERR_NO_ERROR.  
 *
 * Returns:
 *
 *     BT_STATUS_PENDING - The Reconfig operation was started successfully.  
 *         An AVDTP_EVENT_STREAM_RECONFIG_CNF event will be received when the 
 *         stream is reconfigured.  
 *
 *     BT_STATUS_FAILED - The stream is not in the open state.  
 *
 *     BT_STATUS_INVALID_PARM - The Stream parameter does not contain a 
 *         valid pointer. (XA_ERROR_CHECK only).  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel or stream is not 
 *         registered.  
 *
 *     BT_STATUS_NO_CONNECTION - The stream is not allocated on a channel.  
 *
 *     Other - It is possible to receive other error codes, depending on the 
 *         lower layer service in use (L2CAP or Management Entity).  
 */
BtStatus AVDTP_ReconfigRsp(AvdtpStream        *Stream, 
                           AvdtpError          Error, 
                           AvdtpCapabilityType Type);

/*---------------------------------------------------------------------------
 * AVDTP_ReportSinkDelay()
 *---------------------------------------------------------------------------
 *
 *     Reports the delay in the Sink's buffering/processing.
 *
 * Parameters:
 * 
 *     Stream - A registered and configured stream.  
 *
 *     delayMs - A value representing the delay in the Sink
 *         buffering/processing.  If the Stream is a Source, this value is
 *         ignored.
 * 
 * Returns:
 *     BT_STATUS_PENDING - The Sink Delay was sent.  An
 *         AVDTP_EVENT_STREAM_DELAY_CNF event will be received when the
 *         remote device acknowledges the command.
 * 
 *     BT_STATUS_SUCCESS - The Sink Delay was set and will be sent when a
 *         reconfig procedure is done.
 * 
 *     BT_STATUS_FAILED - The stream is not a Sink or is not in the correct
 *         state (STREAMING) for sending this command.
 *
 *     BT_STATUS_INVALID_PARM - The 'Stream' parameter does not contain a 
 *         valid pointer.  (XA_ERROR_CHECK only).  
 *
 *     BT_STATUS_IN_USE - The specified streams is already in use.  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel or stream is not 
 *         registered.  
 *
 *     BT_STATUS_NO_CONNECTION - The stream is not allocated on a channel.  
 *
 *     Other - It is possible to receive other error codes, depending on the 
 *         lower layer service in use (L2CAP or Management Entity).  
 */
BtStatus AVDTP_ReportSinkDelay(AvdtpStream *Stream, uint16_t delayMs);

/*---------------------------------------------------------------------------
 * AVDTP_StartStream()
 *
 *     Initiates streaming on the open stream.  Calling this function puts 
 *     the stream into an active state, which allows media packets to be sent 
 *     on the stream.
 *
 * Parameters:
 *
 *     Stream - A registered and open AVDTP stream.  
 *
 * Returns:
 *
 *     BT_STATUS_PENDING - The operation was initiated successfully.  
 *         An AVDTP_EVENT_STREAM_STARTED event will be received when the 
 *         stream is ready for streaming.  
 *
 *     BT_STATUS_FAILED - The stream is not in the open state.  
 *
 *     BT_STATUS_INVALID_PARM - The Stream parameter does not contain a 
 *         valid pointer. (XA_ERROR_CHECK only).  
 *
 *     BT_STATUS_IN_USE - One of the specified streams is already in use.  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel or stream is not 
 *         registered.  
 *
 *     BT_STATUS_NO_CONNECTION - The stream is not allocated on a channel.  
 *
 *     Other - It is possible to receive other error codes, depending on the 
 *         lower layer service in use (L2CAP or Management Entity).  
 */
BtStatus AVDTP_StartStream(AvdtpStream *Stream);

/*---------------------------------------------------------------------------
 * AVDTP_StartStreamRsp()
 *
 *     Called in response to an AVDTP_EVENT_STREAM_START_IND event.  Calling 
 *     this function will either accept the streaming request or reject it.  
 *
 * Parameters:
 *
 *     Stream - A registered and open AVDTP stream.  
 *
 *     Error - If the streaming request is accepted, this parameter must be 
 *         set to AVDTP_ERR_NO_ERROR.  If the streaming request is rejected, 
 *         this parameter must be set to the appropriate error defined by 
 *         AvdtpError.  
 *
 * Returns:
 *
 *     BT_STATUS_PENDING - The Start Streams Response operation was started 
 *         successfully.  An AVDTP_EVENT_STREAM_STARTED event will be 
 *         received when the stream has been started.  If the start stream 
 *         request was rejected, the AVDTP_EVENT_STREAM_SUSPENDED event will 
 *         be received.  
 *
 *     BT_STATUS_FAILED - The stream is not in the open state.  
 *
 *     BT_STATUS_INVALID_PARM - The Stream parameter does not contain a 
 *         valid pointer. (XA_ERROR_CHECK only).  
 *
 *     BT_STATUS_IN_USE - One of the specified streams is already in use.  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel or stream is not 
 *         registered.  
 *
 *     BT_STATUS_NO_CONNECTION - The stream is not allocated on a channel.  
 *
 *     Other - It is possible to receive other error codes, depending on the 
 *         lower layer service in use (L2CAP or Management Entity).  
 */
BtStatus AVDTP_StartStreamRsp(AvdtpStream *Stream, AvdtpError Error);

/*---------------------------------------------------------------------------
 * AVDTP_SuspendStream()
 *
 *     Suspends a stream that is currently active.  
 *
 * Parameters:
 *
 *     Stream - A registered and active AVDTP stream.  
 *
 * Returns:
 *
 *     BT_STATUS_PENDING - The Suspend Streams Response operation was started 
 *         successfully.  An AVDTP_EVENT_STEAM_SUSPENDED event will be 
 *         received when the stream has been suspended.  
 *
 *     BT_STATUS_FAILED - The stream is not in an active state.  
 *
 *     BT_STATUS_INVALID_PARM - The Stream parameter does not contain a 
 *         valid pointer. (XA_ERROR_CHECK only).  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel or stream is not 
 *         registered.  
 *
 *     BT_STATUS_NO_CONNECTION - The stream is not allocated on a channel.  
 *
 *     Other - It is possible to receive other error codes, depending on the 
 *         lower layer service in use (L2CAP or Management Entity).  
 */
BtStatus AVDTP_SuspendStream(AvdtpStream *Stream);

/*---------------------------------------------------------------------------
 * AVDTP_SecurityControlReq()
 *
 *     Initiates a security procedure (content protection).  The security 
 *     procedure data is application specific and is not interpreted in any 
 *     way by AVDTP.  
 *
 * Parameters:
 *
 *     Stream - A registered AVDTP stream.  
 *
 *     Data - Application specific data.  
 *
 *     Len - Length of the security data.  
 *
 * Returns:
 *
 *     BT_STATUS_PENDING - The Security Request operation was started 
 *         successfully.  An AVDTP_EVENT_STREAM_SECURITY_CNF event will be 
 *         received when the security data has been sent.  
 *
 *     BT_STATUS_FAILED - The stream is not in the correct state or content 
 *         protection is not configured on the stream.  
 *
 *     BT_STATUS_INVALID_PARM - The Stream or Data parameter does not 
 *         contain a valid pointer. (XA_ERROR_CHECK only).  
 *
 *     BT_STATUS_NOT_SUPPORTED - The current stream is not configured for 
 *         content protection.  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel or stream is not 
 *         registered.  
 *
 *     BT_STATUS_NO_CONNECTION - The stream is not allocated on a channel.  
 *
 *     Other - It is possible to receive other error codes, depending on the 
 *         lower layer service in use (L2CAP or Management Entity).  
 */
BtStatus AVDTP_SecurityControlReq(AvdtpStream *Stream, uint8_t *Data, uint16_t Len);

/*---------------------------------------------------------------------------
 * AVDTP_SecurityControlRsp()
 *
 *     Called in response to an AVDTP_EVENT_STREAM_SECURITY_IND event.  The 
 *     security procedure data is application specific and is not interpreted 
 *     in any way by AVDTP.  
 *
 * Parameters:
 *
 *     Stream - A registered AVDTP stream.  
 *
 *     Data - Application specific data.  
 *
 *     Len - Length of the security data.  
 *
 *     Error - If the security request is accepted, this parameter must be 
 *         set to AVDTP_ERR_NO_ERROR.  If the security request is rejected, 
 *         this parameter must be set to the appropriate error defined by 
 *         AvdtpError.  
 *
 * Returns:
 *
 *     BT_STATUS_PENDING - The Security Request operation was started 
 *         successfully.  An AVDTP_EVENT_STREAM_SECURITY_CNF event will be 
 *         received when the security data has been sent.  This event will 
 *         be received even if the security request was rejected, however, 
 *         the 'error' field of the callback parameters will be set to the 
 *         value sent in the 'Error' parameter.  
 *
 *     BT_STATUS_FAILED - The stream is not in the correct state.  
 *
 *     BT_STATUS_INVALID_PARM - The Stream or Data parameter does not 
 *         contain a valid pointer. (XA_ERROR_CHECK only).  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel or stream is not 
 *         registered.  
 *
 *     BT_STATUS_NO_CONNECTION - The stream is not allocated on a channel.  
 *
 *     Other - It is possible to receive other error codes, depending on the 
 *         lower layer service in use (L2CAP or Management Entity).  
 */
BtStatus AVDTP_SecurityControlRsp(AvdtpStream *Stream, 
                                  uint8_t          *Data, 
                                  uint16_t          Len, 
                                  AvdtpError   Error);

/*---------------------------------------------------------------------------
 * AVDTP_AbortStream()
 *
 *     Aborts any open or active stream.  Once aborted, an open stream will 
 *     be in a closed state.  
 *
 * Parameters:
 *
 *     Stream - An open, or active AVDTP stream.  
 *
 * Returns:
 *
 *     BT_STATUS_PENDING - The Abort Stream operation was started 
 *         successfully.  An AVDTP_EVENT_STEAM_ABORTED event will be received 
 *         when the stream has been aborted.  
 *
 *     BT_STATUS_FAILED - The stream is not in the correct state.  
 *
 *     BT_STATUS_INVALID_PARM - The Stream parameter does not contain a 
 *         valid pointer. (XA_ERROR_CHECK only).  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel or stream is not 
 *         registered.  
 *
 *     BT_STATUS_NO_CONNECTION - The stream is not allocated on a channel.  
 *
 *     Other - It is possible to receive other error codes, depending on the 
 *         lower layer service in use (L2CAP or Management Entity).  
 */
BtStatus AVDTP_AbortStream(AvdtpStream *Stream);

/*---------------------------------------------------------------------------
 * AVDTP_StreamSendRawPacket()
 *
 *     Sends data on the specified stream.  The stream must be open and in 
 *     an active state.  No checking is done on the validity of 
 *     the data format.  Data is simply delivered to the stream raw.  It is 
 *     assumed that the data is properly formatted using the RTP media packet 
 *     format described in RFC1889.  It is further assumed that the payload 
 *     of the RTP media packet is in the proper format for the configured 
 *     stream.  
 *
 * Parameters:
 *
 *     Stream - A registered and active AVDTP stream.  
 *
 *     Packet - An initialized BtPacket structure.  The Packet->data, 
 *         Packet->dataLen, Packet->headerLen, and Packet->flags fields must 
 *         be initialized.  Packet->headerLen must be 0, and Packet->flags 
 *         be set to BTP_FLAG_INUSE or BTP_FLAG_NONE.  The data 
 *         must be formatted as an RTP media packet defined by RFC1889.  
 *
 * Returns:
 *
 *     BT_STATUS_PENDING - The Write operation was started 
 *         successfully.  An AVDTP_EVENT_STREAM_PACKET_SENT event will be 
 *         received when the data has been sent.  
 *
 *     BT_STATUS_FAILED - The stream is not in the active state.  
 *
 *     BT_STATUS_INVALID_PARM - The Stream or Packet parameter does not 
 *         contain a valid pointer. (XA_ERROR_CHECK only).  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel or stream is not 
 *         registered.  
 *
 *     BT_STATUS_NO_CONNECTION - The stream is not allocated on a channel.  
 *
 *     Other - It is possible to receive other error codes, depending on the 
 *         lower layer service in use (L2CAP or Management Entity).  
 */
BtStatus AVDTP_StreamSendRawPacket(AvdtpStream *Stream, BtPacket *Packet);

/*---------------------------------------------------------------------------
 * AVDTP_GetRemoteDevice()
 *
 *     Returns the remote device structure used by this channel.  Only valid 
 *     when a connection exists.  
 *
 * Parameters:
 *
 *     Channel - An AVDTP channel.  
 *
 * Returns:
 *
 *     A pointer to a Remote Device (BtRemoteDevice).  
 */
BtRemoteDevice * AVDTP_GetRemoteDevice(AvdtpChannel *Channel);

/*---------------------------------------------------------------------------
 * AVDTP_IsConnected()
 *
 *     Returns the current state of an AVDTP connection.  
 *
 * Parameters:
 *
 *     Channel - A registered AVDTP channel.  
 *
 * Returns:
 *
 *     TRUE - the channel is connected.  
 *     FALSE - the channel is not connected.  
 */
int AVDTP_IsConnected(AvdtpChannel *Channel);

/*---------------------------------------------------------------------------
 * AVDTP_MediaPacketSize()
 *
 *     Returns the negotiated L2CAP packet size for sending media packets.  
 *
 * Parameters:
 *
 *     Channel - A registered AVDTP channel.  
 *
 * Returns:
 *
 *     The maximum size for media packets.  
 */
uint16_t AVDTP_MediaPacketSize(AvdtpStream *Stream);
#define AVDTP_MediaPacketSize(s) (L2CAP_GetTxMtu((s)->conn.l2ChannelId))

/*---------------------------------------------------------------------------
 * AVDTP_StreamState()
 *
 *     Returns the current state of a stream.  
 *
 * Parameters:
 *
 *     Stream - A registered AVDTP stream.  
 *
 * Returns:
 *
 *     The stream state (see AvdtpStreamState)  
 *
 */
AvdtpStreamState AVDTP_GetStreamState(AvdtpStream *Stream);

/*---------------------------------------------------------------------------
 * AVDTP_CreateMediaHeader()
 *
 *     Used to create a media packet header before transmitting media data 
 *     using the AVDTP_StreamSendRawPacket().  A media packet header 
 *     can is typically 12 bytes, but can be up to 72 bytes.  Passing in a 
 *     NULL buffer will simply calculate the media header size.  
 *
 * Parameters:
 *
 *     Header - A media packet header structure initialized with the 
 *         appropriate values.  The media header values are specified by 
 *         the profile.  
 *
 *     Buffer - A buffer in which the media packet header is assembled.  If 
 *         NULL, the header is not created, but the size is calculated.  
 *
 * Returns:
 *
 *     The actual size of the media header.  
 */
uint16_t AVDTP_CreateMediaHeader(AvdtpMediaHeader *Header, uint8_t *Buffer);

/*---------------------------------------------------------------------------
 * AVDTP_ParseMediaHeader()
 *
 *     Used to parse a media packet header when a media packet has been 
 *     received from the remote device.  
 *
 * Parameters:
 *
 *     Buffer - A buffer received during the AVDTP_EVENT_STREAM_DATA_IND 
 *         event.  
 *
 *     Header - A media packet header structure to receive the header 
 *         values.  
 *
 * Returns:
 *
 *     The actual size of the media header.  
 */
uint16_t AVDTP_ParseMediaHeader(AvdtpMediaHeader *Header, uint8_t *Buffer);
#endif
/*---------------------------------------------------------------------------
 * AVDEV_RegisterSec()
 *
 *     Registers security for VDP and A2DP.
 * 
 * Parameters:
 * 
 *     SecParms - Security parameters.  If 0, baseline security
 *                parameters are used (BSL_SECURITY_L1_IN | BSL_SECURITY_L1_OUT).
 *
 * Returns:
 *     BT_STATUS_SUCCESS - security record is registered.
 *
 *     BT_STATUS_INVALID_PARM - operation failed because the
 *         security level is not properly formed. (error check only).
 *
 *     BT_STATUS_IN_USE - operation failed because A/V security is alread
 *                        registered. 
 *
 *     BT_STATUS_FAILED - operation failed because record is
 *         already on the list (XA_ERROR_CHECK only).
 */
BtStatus AVDEV_RegisterSec(BtSecurityParms *SecParms);

/*---------------------------------------------------------------------------
 * AVDTP_Reconfig()
 *
 *     Reconfigure an open stream.  This call can be made only if the stream 
 *     is in the open state.  In other words, a stream that is currently 
 *     streaming audio/video must be suspended before trying to reconfigure 
 *     it.  
 *
 * Parameters:
 *
 *     Stream - A registered and open AVDTP stream.  
 *
 *     Codec - Structure that defines the specific codec type to use for this 
 *         stream, and the specific codec elements to use.  Selected codec 
 *         elements must not be ambiguous.  For example, only one sampling 
 *         frequency should be selected.  
 *
 *     Cp - Structure that defines the type of content protection to 
 *         configure on the remote device.  If no content protection is 
 *         desired, this parameter may be set to NULL.  
 *
 * Returns:
 *
 *     BT_STATUS_PENDING - The Reconfig operation was started successfully.  
 *         An AVDTP_EVENT_STREAM_RECONFIG_CNF event will be received when the 
 *         stream is reconfigured.  
 *
 *     BT_STATUS_FAILED - The stream is not in the open state.  
 *
 *     BT_STATUS_INVALID_PARM - The Stream or Cp parameter does not 
 *         contain a valid pointer. (XA_ERROR_CHECK only).  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel or stream is not 
 *         registered.  
 *
 *     BT_STATUS_NO_CONNECTION - The stream is not allocated on a channel.  
 *
 *     Other - It is possible to receive other error codes, depending on the 
 *         lower layer service in use (L2CAP or Management Entity).  
 */
BtStatus AVDTP_Reconfig(AvdtpStream      *Stream, 
                        AvdtpCodec       *Codec,
                        AvdtpContentProt *Cp);
#endif /* __AVDTP_H_ */ 

