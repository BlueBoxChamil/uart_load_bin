#ifndef __AVCTP_API_H_
#define __AVCTP_API_H_

#include "btconfig.h"

#include "bt_types.h"
#include "me_api.h"
/*---------------------------------------------------------------------------
 * AVCTP API layer
 *
 *     The Audio/Video Remote Control Transport Protocol (AVCTP) defines 
 *     procedures for exchanging 1394 Trade Association AV/C commands between 
 *     Bluetooth enabled Audio/Video devices.
 *
 *     This API is designed to support AV remote control applications using
 *     the iAnywhere Blue SDK core protocol stack.  It provides an API
 *     for connection management and message handling.
 */

/****************************************************************************
 *
 * Types
 *
 ****************************************************************************/

/*---------------------------------------------------------------------------
 * AvctpEvent type
 *      
 */
typedef uint8_t AvctpEvent;

/** The transport layer is connected and commands/responses can now
 * be exchanged.
 * 
 *  During this callback, the 'p.remDev' parameter is valid.
 */
#define AVCTP_EVENT_CONNECT             1

/** A remote device is attempting to connect the transport layer.
 *  Only the acceptor of the connection is notified.  The acceptor must
 *  call AVCTP_ConnectRsp() to either accept or reject the connection.
 * 
 *  During this callback, the 'p.remDev' parameter is valid.
 */
#define AVCTP_EVENT_CONNECT_IND         2

/** The transport layer been disconnected. 
 * 
 *  During this callback, the 'p.remDev' parameter is valid.
 */
#define AVCTP_EVENT_DISCONNECT          3

/** A command was received from the remote AVCTP device (controller).
 * 
 *  During this callback, the 'p.cmdFrame' parameter is valid. It contains the
 *  the AVCTP command header information, including operands. If the "more"
 *  value is TRUE then this event only signals the first part of the operands.
 *  Subsequent AVCTP_EVENT_OPERANDS events will follow this event with
 *  additional operand data.
 */
#define AVCTP_EVENT_COMMAND             4

/** A response was received from the remote AVCTP device (target).
 * 
 * During this callback, the 'p.rspFrame' parameter is valid. It contains the
 * the AVCTP response header information, including operands. If the "more"
 * value is TRUE then this event only signals the first part of the operands.
 * Subsequent AVCTP_EVENT_OPERANDS events will follow this event with
 * additional operand data.
 */
#define AVCTP_EVENT_RESPONSE            5

/** The remote device (target) rejected the AVCTP command.
 * 
 * During this callback, the 'p.rspFrame' parameter is valid. It contains the
 * the AVCTP reject header information, including operands. If the "more"
 * value is TRUE then this event only signals the first part of the operands.
 * Subsequent AVCTP_EVENT_OPERANDS events will follow this event with
 * additional operand data.
 */
#define AVCTP_EVENT_REJECT              6

/** A command (see AVCTP_SendCommand) or response (see AVCTP_SendResponse)
 * has been sent. Memory allocated for the operation can be freed or reused
 * after receiving this event.
 * 
 * During this callback, the 'p.cmdFrame' or 'p.rspFrame' parameter associated
 * with the sent command or response is valid. In addition, "status" will be
 * set to indicate "BT_STATUS_SUCCESS" or "BT_STATUS_FAILED" to indicate
 * whether the event was properly delivered.
 */
#define AVCTP_EVENT_TX_DONE             8

/** Additional operand data has been received for the previous
 * AVCTP_EVENT_COMMAND or AVCTP_EVENT_RESPONSE.
 * 
 * During this callback, the 'p.cmdFrame' or 'p.rspFrame' parameter associated
 * with the received command or response is valid. The "operands" and
 * "operandLen" fields indicate the chunk of operands being received for
 * the command or response. If the "more" field is set to TRUE, the full
 * operand buffer will be received in multiple _OPERANDS events and the
 * last operand buffer indicated with the "more" field set to FALSE.
 */
#define AVCTP_EVENT_OPERANDS            9

/* End of AvctpEvent */

#define AVCTP_EVENT_LAST                9

/*---------------------------------------------------------------------------
 * AvctpCtype type
 *
 * This type defines the AV/C ctype (command type) codes.
 */
typedef uint8_t AvctpCtype;

#define AVCTP_CTYPE_CONTROL               0x00
#define AVCTP_CTYPE_STATUS                0x01
#define AVCTP_CTYPE_SPECIFIC_INQUIRY      0x02
#define AVCTP_CTYPE_NOTIFY                0x03
#define AVCTP_CTYPE_GENERAL_INQUIRY       0x04
#define AVCTP_CTYPE_BROWSING              0x80
/* End of AvctpCtype */

/*---------------------------------------------------------------------------
 * AvctpResponse type
 *
 * This type defines the AV/C response codes.
 */
typedef uint8_t AvctpResponse;

#define AVCTP_RESPONSE_NOT_IMPLEMENTED    0x08
#define AVCTP_RESPONSE_ACCEPTED           0x09
#define AVCTP_RESPONSE_REJECTED           0x0A
#define AVCTP_RESPONSE_IN_TRANSITION      0x0B
#define AVCTP_RESPONSE_IMPLEMENTED_STABLE 0x0C
#define AVCTP_RESPONSE_CHANGED            0x0D
#define AVCTP_RESPONSE_INTERIM            0x0F
#define AVCTP_RESPONSE_BROWSING           0x40
/* End of AvctpResponse */

/*---------------------------------------------------------------------------
 * AvctpOpcode type
 *
 * This type defines the AV/C Opcodes.
 */
typedef uint8_t AvctpOpcode;

#define AVCTP_OPCODE_VENDOR_DEPENDENT     0x00
#define AVCTP_OPCODE_UNIT_INFO            0x30
#define AVCTP_OPCODE_SUBUNIT_INFO         0x31
#define AVCTP_OPCODE_PASS_THROUGH         0x7C
/* End of AvctpOpcode */


/* Forward references */
typedef struct _AvctpCallbackParms AvctpCallbackParms;
typedef struct _AvctpChannel AvctpChannel;
typedef struct _AvctpCmdFrame AvctpCmdFrame;
typedef struct _AvctpRspFrame AvctpRspFrame;

/*---------------------------------------------------------------------------
 * AvctpCallback type
 *
 * A function of this type is called to indicate events to the application.
 */
typedef void (*AvctpCallback)(AvctpChannel *chnl, AvctpCallbackParms *Parms);

/* End of AvctpCallback */

/* Forward references */ 
typedef struct _AvctpConnCallbackParms AvctpConnCallbackParms;
typedef struct _AvctpConn AvctpConn;

/* Conn callback function */ 
typedef void (*AvctpConnCallback)(AvctpConn *Conn, const AvctpConnCallbackParms *Parms);

/* Conn calback parameters */ 
struct _AvctpConnCallbackParms {
    uint8_t         event;
    uint16_t        dataLen;
    BtStatus        status;
    union {
        BtRemoteDevice *remDev;
        uint8_t        *data;
    } ptrs;
};

/* Connection State */ 
struct _AvctpConn {
    uint16_t            l2ChannelId;
    uint16_t            psm;
    uint8_t             state;
    AvctpConnCallback   callback;
    BtRemoteDevice     *remDev;
};

typedef struct _AvtpChannel AvtpChannel;
typedef struct _AvtpCallbackParms AvtpCallbackParms;

/* Channel Manager Callback */ 
typedef void (*AvtpCallback)(AvtpChannel *chnl, AvtpCallbackParms *Parms);

/* Channel Packet */ 
typedef struct _AvtpPacket {
    ListEntry        node;

    uint8_t          transId;

    uint8_t          msgType;

    uint8_t          msgHdrLen;
    uint8_t          msgHdr[10];

    uint8_t          txIdSize;
    uint16_t         txId;

    uint16_t         txDataLen;
    uint8_t         *txData;
    uint32_t         context;
} AvtpPacket;

/* Channel */ 
struct _AvtpChannel {

    uint32_t            context;

    /* Identifier */ 
    uint16_t            rxId;

    /* Transmit Packet */ 
    ListEntry           avPacketList;
    AvtpPacket         *curAvPacket;

    /* Transmit State */ 
    uint8_t             txState;
    uint16_t            offset;
    uint16_t            packetSize;
    uint8_t             txPacketsLeft;
                   
    /* Channel Receive State */ 
    uint8_t             rxState;
    uint8_t             rxPacketsLeft;

    /* Channel Resources */ 
    BtPacket            packet;
    uint16_t            l2ChannelId;

    /* Command Timeout */ 
    uint32_t            txTimeout;
    EvmTimer            txTimer;

    /* Channel Callback */ 
    AvtpCallback        callback;

};

/* Channel Callback Parameters */ 
struct _AvtpCallbackParms {

    uint8_t          event;       /* Callback event type               */ 
    BtStatus         status;      /* Transport status                  */ 
    uint8_t          transId;     /* Transaction ID                    */ 
    uint8_t          msgType;     /* Message type                      */ 
    uint8_t          pktType;     /* Packet type                       */ 
    uint16_t         rxId;        /* RX ID                             */ 

    uint8_t          packetsLeft; /* Number of packet still to receive */ 
    uint16_t         len;         /* Length of the current data        */ 
    AvtpPacket      *packet;      /* Pointer to the transmitted packet */ 
    uint8_t         *data;        /* Pointer to the received data      */ 
};


/****************************************************************************
 *
 * Data Structures
 *
 ****************************************************************************/

/*---------------------------------------------------------------------------
 * AvctpCmdFrame structure
 *
 * Defines the parameters required for an AVCTP command.
 */
struct _AvctpCmdFrame {
    ListEntry        node;           /* Used internally by AVCTP. */
    uint8_t          transId;        /* Transaction ID */
    AvctpCtype       ctype;          /* 4 bits */
    uint8_t          subunitType;    /* 5 bits */
    uint8_t          subunitId;      /* 3 bits */
    AvctpOpcode      opcode;         /* 8 bits */
    uint8_t          headerLen;      /* Header length */
    uint8_t          header[6];      /* Header information */
    uint16_t         operandLen;     /* Length of buffer in "operands" */
    uint8_t         *operands;       /* Buffer containing the command data */
    int              more;           /* Indicates whether to expect additional
                                      * frames containing more operand data. */
    AvtpPacket       avtpPacket;     /* For sending over AVTP */
    EvmTimer         timer;          /* Timer for the command */
};

/*---------------------------------------------------------------------------
 * AvctpRspFrame structure
 *
 * Defines the parameters required for an AVCTP response.
 */
struct _AvctpRspFrame {
    ListEntry          node;           /* Used internally by AVCTP. */
    uint8_t            transId;        /* Transaction ID */
    AvctpResponse      response;       /* 4 bits */         
    uint8_t            subunitType;    /* 5 bits */         
    uint8_t            subunitId;      /* 3 bits */         
    AvctpOpcode        opcode;         /* 8 bits */         
    uint8_t            headerLen;      /* Header length */
    uint8_t            header[6];      /* Header information */
    uint16_t           operandLen;     /* Length of buffer in "operands" */
    uint8_t           *operands;       /* Buffer containing the response data */
    int                more;           /* Indicates whether to expect additional
                                        * frames containing more operand data. */
    AvtpPacket         avtpPacket;     /* For sending over AVTP */
    EvmTimer           padding;        /* Timer for the command */
};

/*---------------------------------------------------------------------------
 * AvctpChannel structure
 *
 * Defines the AVCTP channel.
 */
struct _AvctpChannel {

    /* === Internal use only === */
    ListEntry       node;

    /* Transmit queue */
    ListEntry       txQueue;

    /* Connection Handle */
    AvctpConn       conn;

    /* Channel Handle */
    AvtpChannel     avtpChnl;

    /* Current Transmitting Command */
    void           *curCmd;

    /* Current Waiting Command ID */
    uint8_t              curId;

    /* Current Transmitting Response */
    void           *curRsp;

    /* Rx Frame */
    union {
        AvctpCmdFrame  cmdFrame;
        AvctpRspFrame  rspFrame;
    } rx;

    /* System response packet */
    AvtpPacket         rspPacket;

    /* Transaction ID */
    uint8_t                 txTransId;

    /* Receive State */
    uint8_t                 rxState;

    /* Transmit State */
    uint8_t                 txState;

    /* Application callback function */
    AvctpCallback      callback;
};

/*---------------------------------------------------------------------------
 * AvctpCallbackParms structure
 *
 * Contains information for the application callback event.
 * 
 */
struct _AvctpCallbackParms {
    /* AVCTP event */
    AvctpEvent      event;

    /* AVCTP channel associated with the event */
    AvctpChannel   *channel;

    /* Status of event (valid only for certain events)  */
    BtStatus        status;

    /* Callback parameter object, depending on "event" */
    union {
        /* Remote device associated with the event */
        BtRemoteDevice *remDev;

        /* Command frame associated with the event */
        AvctpCmdFrame  *cmdFrame;

        /* Response frame associated with the event */
        AvctpRspFrame  *rspFrame;
    } p;
};

/****************************************************************************
 *
 * Function Reference
 *
 ****************************************************************************/
#if 0

/*---------------------------------------------------------------------------
 * AVCTP_Register()
 *
 *      Registers an application callback to receive AVCTP events. This 
 *      function must be called before any other AVCTP functions.
 *
 * Parameters:
 *
 *      chnl - Channel structure that receives or initiates connections.
 *
 *      psm - The type of PSM to register (BT_PSM_AVCTP)
 *
 *      callback - Identifies the application function that will be called
 *          with AVCTP events.
 *
 * Returns:
 *      BT_STATUS_SUCCESS - The AVCTP application callback Function was
 *          successfully registered.
 *
 *      BT_STATUS_IN_USE - The specified channel is already in use.
 *
 *      BT_STATUS_INVALID_PARM - The chnl or Callback parameter does not 
 *         contain a valid pointer (XA_ERROR_CHECK only), or psm is not a
 *         valid PSM value.
 */
BtStatus AVCTP_Register(AvctpChannel *chnl,
                        L2capPsmValue psm,
                        AvctpCallback callback);

/*---------------------------------------------------------------------------
 * AVCTP_Deregister()
 *
 *      De-registers the AVCTP callback. After making this call
 *      successfully, the callback specified in AVCTP_Register will
 *      receive no further events.
 *
 * Parameters:
 *
 *      chnl - Channel structure that receives or initiates connections.
 *
 * Returns:
 *      BT_STATUS_SUCCESS - The AVCTP callback was successfully deregistered.
 * 
 *      BT_STATUS_IN_USE - The specified channel is still in use.
 *
 *      BT_STATUS_NOT_FOUND - An AVCTP callback was not previously registered.
 *     
 *      BT_STATUS_INVALID_PARM - The chnl parameter does not contain a valid 
 *         pointer. (XA_ERROR_CHECK only).
 */
BtStatus AVCTP_Deregister(AvctpChannel *chnl);

/*---------------------------------------------------------------------------
 * AVCTP_Connect()
 * 
 *     Initiates a signal channel connection to a remote AVCTP device.  This 
 *     function is used to establish the lower layer connection (L2CAP), which 
 *     allows sending signal messages.  Only one connection can exist between
 *     two devices.
 *
 *     If the connection attempt is successful, the AVCTP_EVENT_CONNECT event
 *     will be received.  If the connection attempt is unsuccessful, the
 *     AVCTP_EVENT_DISCONNECT event will be received.
 *
 * Parameters:
 *
 *      chnl - Channel structure that receives or initiates connections.
 *
 *      remDev - A connected remote device.
 *
 * Returns:
 *
 *     BT_STATUS_PENDING - The connection process has been successfully
 *         started. When the connection process is complete, the
 *         application callback will receive either the AVCTP_EVENT_CONNECT or 
 *         AVCTP_EVENT_DISCONNECT event.
 *
 *     BT_STATUS_IN_USE - The connection already exists.
 *
 *     BT_STATUS_RESTRICTED - A connection of this type already exists with the
 *         remote device.
 *
 *     BT_STATUS_NOT_FOUND - An AVCTP channel was not previously registered.
 *
 *     BT_STATUS_INVALID_PARM - The chnl or addr parameter does not contain a 
 *         valid pointer (XA_ERROR_CHECK only).
 *
 *     Other - It is possible to receive other error codes, depending on the 
 *         lower layer service in use (L2CAP or Management Entity).
 */
BtStatus AVCTP_Connect(AvctpChannel *chnl, BtRemoteDevice *RemDev);

/*---------------------------------------------------------------------------
 * AVCTP_ConnectRsp()
 * 
 *     Responds to a connection request from the remote AVCTP device.  This 
 *     function is used to establish the lower layer connection (L2CAP), 
 *     which allows sending signaling messages, such as discover, 
 *     configuration, and stream management.
 *
 * Parameters:
 *
 *     Chnl - A registered and open AVCTP channel.
 *
 *     Accept - TRUE accepts the connect or FALSE rejects the connection.
 *
 * Returns:
 *
 *     BT_STATUS_PENDING - The connection responses has been successfully
 *         sent. When the connection process is complete, the application 
 *         callback will receive the AVCTP_EVENT_CONNECT event.
 *
 *     BT_STATUS_BUSY - The connection is already connected.
 *
 *     BT_STATUS_INVALID_PARM - The Chnl parameter does not contain a 
 *         valid pointer. (XA_ERROR_CHECK only).
 *
 *     BT_STATUS_NOT_FOUND - The specified device was not found in the device
 *         selector database.  The device must be discovered, paired, or added
 *         manually using DS_AddDevice().  
 *
 *     Other - It is possible to receive other error codes, depending on the 
 *         lower layer service in use (L2CAP or Management Entity).
 */
BtStatus AVCTP_ConnectRsp(AvctpChannel *Chnl, int Accept);

/*---------------------------------------------------------------------------
 * AVCTP_Disconnect()
 *
 *     Terminates a connection with a remote AVCTP device.  The lower layer
 *     connection (L2CAP) is disconnected.
 *
 * Parameters:
 *
 *     chnl - A registered and open AVCTP channel.
 *
 * Returns:
 *
 *     BT_STATUS_PENDING - The disconnect process has been successfully
 *         started. When the disconnect process is complete, the
 *         application callback will receive the AVCTP_EVENT_DISCONNECT event.
 *
 *     BT_STATUS_INVALID_PARM - The chnl parameter does not contain a valid 
 *         pointer. (XA_ERROR_CHECK only).
 *
 *     BT_STATUS_NO_CONNECTION - No connection exists on the specified 
 *         channel.
 *
 *     BT_STATUS_NOT_FOUND - The specified device was not found in the device
 *         selector database.  The device must be discovered, paired, or added
 *         manually using DS_AddDevice().  
 *
 *     It is possible to receive other error codes, depending on the lower 
 *     layer service in use (L2CAP or Management Entity).
 */
BtStatus AVCTP_Disconnect(AvctpChannel *chnl);

/*---------------------------------------------------------------------------
 * AVCTP_SendCommand()
 *
 *     Sends an AVCTP command on the specified channel. The channel must be
 *     connected and in the open state. The "cmdFrame" parameter must be
 *     set with valid AVCTP command parameters.
 *
 * Parameters:
 *
 *     chnl - A registered and open AVCTP channel.
 *
 *     cmdFrame - An AvctpCmdFrame structure initialized with valid
 *         AVCTP command parameters.
 *
 * Returns:
 *
 *     BT_STATUS_PENDING - The send command operation has been started
 *         successfully. When the associated packet has been sent,
 *         the application callback will receive the AVCTP_EVENT_TX_DONE
 *         event.
 *
 *     BT_STATUS_IN_USE - The command frame is alread in use on this channel.
 *
 *     BT_STATUS_INVALID_PARM - The chnl parameter does not contain a valid 
 *         pointer. (XA_ERROR_CHECK only).
 *
 *     BT_STATUS_NOT_FOUND - The specified channel is not registered.
 */
BtStatus AVCTP_SendCommand(AvctpChannel *chnl, AvctpCmdFrame *cmdFrame);

/*---------------------------------------------------------------------------
 * AVCTP_SendResponse()
 *
 *     Sends an AVCTP response on the specified channel. The channel must be
 *     connected and in the open state. The "rspFrame" parameter must be
 *     set with valid AVCTP response parameters.
 *
 * Parameters:
 *
 *     chnl - A registered and open AVCTP channel.
 *
 *     rspFrame - An AvctpRspFrame structure initialized with valid
 *         AVCTP response parameters.
 *
 * Returns:
 *
 *     BT_STATUS_PENDING - The send response operation has been started
 *         successfully. When the associated packet has been sent,
 *         the application callback will receive the AVCTP_EVENT_TX_DONE
 *         event.
 *
 *     BT_STATUS_IN_USE - The response frame is alread in use on this channel.
 *
 *     BT_STATUS_INVALID_PARM - The chnl parameter does not contain a valid 
 *         pointer. (XA_ERROR_CHECK only).
 *
 *     BT_STATUS_NOT_FOUND - The specified channel is not registered.
 */
BtStatus AVCTP_SendResponse(AvctpChannel *chnl, AvctpRspFrame *rspFrame);

/*---------------------------------------------------------------------------
 * AVCTP_GetRemoteDevice()
 *
 *     Returns a pointer to the current remote device.
 *
 * Parameters:
 *
 *     chnl - An AVCTP channel.
 *
 * Returns:
 *
 *     A pointer to a remote device.
 */
BtRemoteDevice * AVCTP_RemoteDevice(AvctpChannel *chnl);
#endif
#endif /* __AVCTP_H_ */
