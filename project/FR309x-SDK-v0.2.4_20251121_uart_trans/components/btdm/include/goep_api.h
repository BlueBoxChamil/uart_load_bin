#ifndef _GOEP_API_H
#define _GOEP_API_H

#include "bt_types.h"
#include "sec_api.h"

/*---------------------------------------------------------------------------
 * ObStatus type
 *
 * Indicates the status of various OBEX Layer API functions.
 */
typedef BtStatus ObStatus;

/* The function call was successful. */
#define OB_STATUS_SUCCESS          BT_STATUS_SUCCESS 

/* The operation has failed to start. */
#define OB_STATUS_FAILED           BT_STATUS_FAILED 

/* The operation has successfully started, callback indicates end. */
#define OB_STATUS_PENDING          BT_STATUS_PENDING

/* The transport layer link is disconnected. */
#define OB_STATUS_DISCONNECT       BT_STATUS_DISCONNECT

/* No connection exists, when one is required */
#define OB_STATUS_NO_CONNECT       BT_STATUS_NO_LINK

/* IrDA Only: The media is busy. */
#define OB_STATUS_MEDIA_BUSY       BT_STATUS_MEDIA_BUSY

/* A request is pending or resources are in use. */
#define OB_STATUS_BUSY             BT_STATUS_BUSY

/* The necessary resources are not available. */
#define OB_STATUS_NO_RESOURCES     BT_STATUS_NO_RESOURCES

/* An invalid function parameter was provided. */
#define OB_STATUS_INVALID_PARM     BT_STATUS_INVALID_PARM

/* Bluetooth Only: The request failed because the specified device
 *                 is in the process of being disconnected.
 */
#define OB_STATUS_IN_PROGRESS      BT_STATUS_IN_PROGRESS

/* Feature not supported */
#define OB_STATUS_NOT_SUPPORTED    BT_STATUS_NOT_SUPPORTED

/* Bluetooth Only : Feature is restricted, due to a 
 *                  Bluetooth security failure 
 */
#define OB_STATUS_RESTRICTED       BT_STATUS_RESTRICTED

/* The object handle was invalid */
#define OB_STATUS_INVALID_HANDLE   (BT_STATUS_OPEN+0)

/* The OBEX packet is too small to support the request. */
#define OB_STATUS_PACKET_TOO_SMALL (BT_STATUS_OPEN+1)

/* No active session exists, when one is required. */
#define OB_STATUS_NO_SESSION       (BT_STATUS_OPEN+2)

/* End of ObStatus */

/*--------------------------------------------------------------------------
 * ObexRespCode type
 *
 *     Response codes used to signify the status of an OBEX operation. They
 *     are not standard function return codes, but rather are returned by
 *     ObStore functions to then be included within OBEX Response packets,
 *     which are sent out to other devices.
 *
 *     The first two response codes listed below are success responses;
 *     the remainder are considered failures. The failure codes are
 *     frequently associated with aborted operations.
 */
typedef uint8_t ObexRespCode;

/* Group: Successful response codes */

#define OBRC_CONTINUE             0x10 /* Continue */
#define OBRC_SUCCESS              0x20 /* OK, Success */

/* Group: Failure response codes */

#define OBRC_CREATED              0x21 /* Created */
#define OBRC_ACCEPTED             0x22 /* Accepted */
#define OBRC_NON_AUTHOR_INFO      0x23 /* Non-Authoritative Information */
#define OBRC_NO_CONTENT           0x24 /* No Content */
#define OBRC_RESET_CONTENT        0x25 /* Reset Content */
#define OBRC_PARTIAL_CONTENT      0x26 /* Partial Content */

#define OBRC_MULTIPLE_CHOICES     0x30 /* Multiple Choices */
#define OBRC_MOVED_PERMANENT      0x31 /* Moved Permanently */
#define OBRC_MOVED_TEMPORARY      0x32 /* Moved Temporarily */
#define OBRC_SEE_OTHER            0x33 /* See Other */
#define OBRC_NOT_MODIFIED         0x34 /* Not Modified */
#define OBRC_USE_PROXY            0x35 /* Use Proxy */

#define OBRC_BAD_REQUEST          0x40 /* Bad Request */
#define OBRC_UNAUTHORIZED         0x41 /* Unauthorized */
#define OBRC_PAYMENT_REQUIRED     0x42 /* Payment Required */
#define OBRC_FORBIDDEN            0x43 /* Forbidden - operation is understood
                                        * but refused */
#define OBRC_NOT_FOUND            0x44 /* Not Found */
#define OBRC_METHOD_NOT_ALLOWED   0x45 /* Method Not Allowed */
#define OBRC_NOT_ACCEPTABLE       0x46 /* Not Acceptable */
#define OBRC_PROXY_AUTHEN_REQ     0x47 /* Proxy Authentication Required */
#define OBRC_REQUEST_TIME_OUT     0x48 /* Request Timed Out */
#define OBRC_CONFLICT             0x49 /* Conflict */

#define OBRC_GONE                 0x4a /* Gone */
#define OBRC_LENGTH_REQUIRED      0x4b /* Length Required */
#define OBRC_PRECONDITION_FAILED  0x4c /* Precondition Failed */
#define OBRC_REQ_ENTITY_TOO_LARGE 0x4d /* Requested entity is too large */
#define OBRC_REQ_URL_TOO_LARGE    0x4e /* Requested URL is too large */
#define OBRC_UNSUPPORT_MEDIA_TYPE 0x4f /* Unsupported Media Type */

#define OBRC_INTERNAL_SERVER_ERR  0x50 /* Internal Server Error */
#define OBRC_NOT_IMPLEMENTED      0x51 /* Not Implemented */
#define OBRC_BAD_GATEWAY          0x52 /* Bad Gateway */
#define OBRC_SERVICE_UNAVAILABLE  0x53 /* Service Unavailable */
#define OBRC_GATEWAY_TIMEOUT      0x54 /* Gateway Timeout */
#define OBRC_HTTP_VER_NO_SUPPORT  0x55 /* HTTP version not supported */

#define OBRC_DATABASE_FULL        0x60 /* Database Full */
#define OBRC_DATABASE_LOCKED      0x61 /* Database Locked */

#define OBRC_TRANSPORT_SECURITY_FAILURE     0x70    /* Transport level security failure (not specified in OBEX spec) */

/* End of ObexRespCode */

/*---------------------------------------------------------------------------
 * OBS_Write() type
 *
 *      A function of this type is provided by the OBEX Object Store 
 *      and stored in the ObStoreFuncTable.  This function writes 
 *      data to the object store entry.
 *      
 *      This function is REQUIRED by the OBEX Protocol.
 *
 * Parameters:
 *      obs -   pointer to the handle of object store entry.
 *              Casting from void* to the object store entry
 *              must be done inside this function.
 *
 *      buff -  pointer to data to write
 *
 *      len -   length in bytes of data to write
 */
typedef ObexRespCode (*OBS_Write)(void *obs, uint8_t *buff, uint16_t len);

/*---------------------------------------------------------------------------
 * OBS_Read() type
 *
 *      A function of this type is provided by the OBEX Object Store 
 *      and stored in the ObStoreFuncTable.  This function reads 
 *      data from the object store entry.
 *      
 *      This function is REQUIRED by the OBEX Protocol.
 *
 * Parameters:
 *      obs -   pointer to the handle of object store entry.
 *              Casting from void* to the object store entry
 *              must be done inside this function.
 *
 *      buff -  pointer to location where data is read into.
 *
 *      len -   length in bytes of data to read.
 */
typedef ObexRespCode (*OBS_Read)(void *obs, uint8_t *buff, uint16_t len);

/*---------------------------------------------------------------------------
 * OBS_GetObjectLen() type
 *
 *      A function of this type is provided by the OBEX Object Store 
 *      and stored in the ObStoreFuncTable.  This function gets 
 *      the length of an object store item.
 *      
 *      If the object store contains objects which have unknown lengths,
 *      OBEX must be compiled with OBEX_DYNAMIC_OBJECT_SUPPORT enabled.
 *      The object store can then return UNKNOWN_OBJECT_LENGTH for those
 *      objects.
 *  
 *      This function is REQUIRED by the OBEX Protocol.
 *
 * Parameters:
 *      obs -   pointer to the handle of object store entry.
 *              Casting from void* to the object store entry
 *              must be done inside this function.
 */
typedef uint32_t (*OBS_GetObjectLen)(void *obs);

/*---------------------------------------------------------------------------
 * OBS_ReadFlex() type
 *
 *      A function of this type is provided by the OBEX Object Store 
 *      and stored in the ObStoreFuncTable.  This function reads 
 *      data from the object store entry. This function can read 
 *      any number of bytes, up to 'len'.
 *
 *      This function is only used for objects of unknown length (such an
 *      object will return UNKNOWN_OBJECT_LENGTH from the OBS_GetObjectLen
 *      function. For objects of known length, OBEX uses OBS_Read()
 *      instead.
 *
 * Requires:
 *      OBEX_DYNAMIC_OBJECT_SUPPORT set to XA_ENABLED.
 *
 * Parameters:
 *      obs -   pointer to the handle of object store entry.
 *              Casting from void* to the object store entry
 *              must be done inside this function.
 *
 *      buff -  pointer to location where data is read into.
 *
 *      len -   On input, contains the maximum length in bytes of data to read.
 *              Before returning, the object store must write the actual number
 *              of bytes read into "buff".
 *
 *      more -  Before returning, the object store must write "TRUE" or
 *              "FALSE" into the boolean referenced by this parameter. TRUE
 *              indicates that there is more data to read, FALSE indicates
 *              that all object data has been read.
 */
typedef ObexRespCode (*OBS_ReadFlex)(void *obs, uint8_t *buff, uint16_t *len, int *more);

/*---------------------------------------------------------------------------
 * ObexTpType type
 *
 *     Specifies the different transport layers supported by OBEX. Protocols
 *     can be ORed together if necessary.
 *
 *     If you are adding support for a new transport, you should
 *    modify obex.h to contain a new bit for the transport.
 *    
 */
typedef uint8_t ObexTpType;

/* Specifies the Bluetooth over RFCOMM protocol. */
#define OBEX_TP_BLUETOOTH       0x04

/*--------------------------------------------------------------------------
 * ObexHeaderType type
 *
 *     Used to specify the type of a header. The header's type implies the
 *     meaning of the contents and also the data format type, which is listed
 *     with each header type below.  When creating or parsing headers, be sure
 *     to use the correct function for the header's internal format.
 */
typedef uint8_t ObexHeaderType;

#define OBEXH_COUNT       0xC0 /* (4-byte) Number of objects */
#define OBEXH_NAME        0x01 /* (Unicode) Object name */
#define OBEXH_TYPE        0x42 /* (ByteSeq) MIME type of object */
#define OBEXH_LENGTH      0xC3 /* (4-byte) Object length */
#define OBEXH_TIME_ISO    0x44 /* (ByteSeq, ISO 8601 format) Creation or
                                * modification time for object (preferred format). */
#define OBEXH_TIME_COMPAT 0xC4 /* (4-byte) Creation or modification time for
                                * object for backward-compatibility. */
#define OBEXH_DESCRIPTION 0x05 /* (Unicode) Text description of object */
#define OBEXH_TARGET      0x46 /* (ByteSeq) Target ID for operation */
#define OBEXH_HTTP        0x47 /* (ByteSeq) An HTTP 1.x header (URL for object) */
#define OBEXH_BODY        0x48 /* Not for use by OBEX applications */
#define OBEXH_END_BODY    0x49 /* Not for use by OBEX applications */
#define OBEXH_WHO         0x4A /* (ByteSeq) Who ID identifies service providing
                                * the object */
#define OBEXH_CONNID      0xCB /* (4-byte) Identifies the connection for which
                                * the operation is directed */
#define OBEXH_APP_PARAMS  0x4C /* (ByteSeq) Application parameters */
#define OBEXH_AUTH_CHAL   0x4D /* (ByteSeq) Authentication challenge */
#define OBEXH_AUTH_RESP   0x4E /* (ByteSeq) Authentication response */
#define OBEXH_CREATOR_ID  0xCF /* (4-byte)  Creator ID contains Palm/MAC files
                                * creator code */
#define OBEXH_WAN_UUID    0x50 /* (ByteSeq) Network client UUID */
#define OBEXH_OBJ_CLASS   0x51 /* (ByteSeq) Object class */

/* End of ObexHeaderType */

/*---------------------------------------------------------------------------
 * GoepUniType type
 *
 *     This type is used for parameters that are encoded in UNICODE when
 *     sent over OBEX. The format of these parameters depends on whether
 *     the GOEP layer or the application is handling the UNICODE formatting.
 */
typedef uint8_t      GoepUniType;

/* End of GoepUniType */

/*---------------------------------------------------------------------------
 * GoepConnTypes
 *
 *     Generic Object Exchange connection types for connection management 
 *     (for Client and Server)
 */
typedef uint8_t GoepConnTypes;

#define GOEP_MUX_CONN       0x01    /* Multiplex an existing OBEX connection */
#define GOEP_NEW_CONN       0x02    /* Start a new OBEX connection, don't multiplex */

/*---------------------------------------------------------------------------
 * GoepEventType type
 *
 *     The GoepEventType type defines the events that may be indicated to
 *     the Client and Server applications.
 */
typedef uint8_t GoepEventType;

/* Reserved event type, unused by the GOEP protocol. 
 */
#define GOEP_EVENT_NONE                 0

/* Indicates the start of a new operation (Server Only).  In the case of an Abort
 * operation, this event may occur during another pending operation (Put or Get).
 * In the case where GOEP_ServerContinue() has not been called during a Put or 
 * Get, perhaps awaiting Server object creation or additional buffer space, it 
 * should be called now or the Abort operation will not complete.
 */
#define GOEP_EVENT_START                1

/* Delivered to the application when it is time to issue
 * another request or response. The application must call either
 * GOEP_ServerContinue() or GOEP_ClientContinue().
 */
#define GOEP_EVENT_CONTINUE             2

/* An OBEX header was received that was not handled internally in GOEP.
 */
#define GOEP_EVENT_HEADER_RX            5

/* Delivered to server applications before the last response is going
 * to be sent. It provides the server with an opportunity to complete 
 * the requested operation (such as a SetFolder request) and if necessary
 * set the response code before the final response is sent (Server Only).
 */
#define GOEP_EVENT_PRECOMPLETE          6

/* Instructs the server to provide an object store handle to the protocol.
 * This event is generated during the processing of Put and Get operations.
 * The server calls GOEP_ServerAccept() to provide the object. Failure to 
 * do so will cause the operation to be rejected by the protocol. (Server Only)
 */
#define GOEP_EVENT_PROVIDE_OBJECT       7

/* Instructs the server to delete an object. This event is generated 
 * during the processing of a Put-Delete operation. The delete is assumed
 * to be successful, so the server must call GOEP_ServerAbort() if the 
 * object could not be deleted. 
 */
#define GOEP_EVENT_DELETE_OBJECT        8

/* Indicates that the operation has completed successfully.
 */
#define GOEP_EVENT_COMPLETE             14

/* Indicates that the current operation has failed.
 */
#define GOEP_EVENT_ABORTED              15

/* A transport layer connection has been established. There is no
 * operation associated with this event.
 */
#define GOEP_EVENT_TP_CONNECTED         16

/* The transport layer connection has been disconnected. There is no
 * operation associated with this event.
 */
#define GOEP_EVENT_TP_DISCONNECTED      17

/* The transport layer discovery process has failed. (Client only)
 */
#define GOEP_EVENT_DISCOVERY_FAILED     18

/* The transport layer OBEX service cannot be found. (Client only)
 */
#define GOEP_EVENT_NO_SERVICE_FOUND     19

/* End of GoepEventType */

/*---------------------------------------------------------------------------
 * GoepOperation type
 *
 *     The GoepOperation type defines operations based on the Generic
 *     Object Exchange Profile's operations.
 */
typedef uint8_t GoepOperation;

/* This value is used when indicating events which do not occur during
 * an operation, such as TP Connect and TP Disconnect.
 */
#define GOEP_OPER_NONE              0

/* A Connect operation is in progress. The info.connect field in the
 * event structure is valid.
 */
#define GOEP_OPER_CONNECT           1

/* A Disconnect operation is in progress. */
#define GOEP_OPER_DISCONNECT        2

/* A Push operation is in progress. The info.pushpull or info.push field
 * in the event structure is valid.
 */
#define GOEP_OPER_PUSH              3

/* A Delete operation is in progress. The info.pushpull field in the 
 * event structure is valid.
 */
#define GOEP_OPER_DELETE            4

/* A Push operation is in progress. The info.pushpull field in the
 * event structure is valid.
 */
#define GOEP_OPER_PULL              5

/* A Set folder operation is in progress. The info.setfolder field in the
 * event structure is valid.
 */
#define GOEP_OPER_SETFOLDER         6

/* An Action operation is in progress. The info.action field in the
 * event structure is valid.
 */
#define GOEP_OPER_ACTION            7

/* A Session operation is in progress. The info.session field in the
 * event structure is valid.
 */
#define GOEP_OPER_SESSION           8

/* An abort operation is in progress. This event will be delivered while
 * another operation is in progress, if the ABORT is sent to stop that
 * operation.
 */
#define GOEP_OPER_ABORT             9

/* End of GoepOperation */

/*---------------------------------------------------------------------------
 * ObStoreFuncTable structure
 *
 *     Table of OBEX Object Store specific functions. This table is
 *     passed down to the OBEX protocol layer in the ObexAppHandle structure
 *     when OBEX_InitAppHandle() is called. It can be allocated in ROM.
 */
typedef struct _ObStoreFuncTable {
    OBS_Read            Read;
    OBS_Write           Write;
    OBS_GetObjectLen    GetObjectLen;
    OBS_ReadFlex        ReadFlex;
} ObStoreFuncTable;

/*---------------------------------------------------------------------------
 * ObexBtTarget structure
 *
 *     Defines device address and service information required for a 
 *     server to locate a remote Bluetooth OBEX service. The first
 *     3 parameters are identical to those of the same names used with
 *     ME_CreateLink().
 */
typedef struct _ObexBtTarget {
    BD_ADDR         addr;
    BtPageScanInfo  psi;
    int             allowRoleChange;
    const uint8_t  *sdpQuery;
    uint16_t        sdpQueryLen;
    uint8_t         sdpQueryType;
} ObexBtTarget;

/*---------------------------------------------------------------------------
 * ObexTpAddr structure
 *     
 *     Defines a transport-specific addressing information. This structure,
 *     and all of the fields used in it must remain valid for the duration
 *     of the OBEX_ClientTpConnect() call. This means that if the call returns
 *     OB_STATUS_PENDING, the structure must be maintained until the client
 *     application callback receives a completion event.
 */
typedef struct _ObexTpAddr {
    /* Specifies which address in the union is being provided. */
    ObexTpType  type;               

    /* This union contains entries for different transport types.
     * Add new transport types here if necessary.
     */
    union {
        ObexBtTarget    bt;
    } proto;

} ObexTpAddr;

/*---------------------------------------------------------------------------
 * GoepClientApp structure
 *
 *     This structure is used by the Client Application (or Profile) to 
 *     register with the GOEP Client multiplexer.
 *
 *     If registering directly, using the GOEP_RegisterClient() function all
 *     fields must be filled in prior to registration.
 *
 *     If registering via a profile manager, such as GOEP_RegisterFtpClient()
 *     only the 'callback' field should be filled in.
 */
typedef struct _GoepClientApp_t {
    /* Group: These fields must be filled in prior to registration */
    void *          callback;

    /* Group: These fields must be filled in only if using GOEP_RegisterClient() */
    uint8_t         type;       /* GOEP_PROFILE_xxx define */
    void *          appParent;  /* Must be set to zero if unused */

    GoepConnTypes   connFlags;   /* Type of connection - new or multiplexed */
    uint8_t         connId;      /* Id of the Obex connection - existing or new */

    /* === Internal use only === */
#if BT_SECURITY == XA_ENABLED
    BtSecurityRecord        secRecord;
    BtSecurityToken         secToken;
    int                     authorized;
    int                     registered;
#endif /* BT_SECURITY == XA_ENABLED */

    uint32_t        obexConnId;
    uint8_t         connState;

} GoepClientApp;

/*---------------------------------------------------------------------------
 * GoepConnectReq structure
 *
 *     This structure is used by the Client when sending an OBEX Connect.
 *     All fields are optional.
 */
typedef struct _GoepConnectReq {
    /* This field specifies the Target server application. */
    const uint8_t            *target;

    /* This field specifies the local client application. */
    const uint8_t            *who;

    /* The length of the string pointed to by the 'target' field. */
    uint8_t                   targetLen;

    /* The length of the string pointed to by the 'who' field. */
    uint16_t                  whoLen;
} GoepConnectReq;

/*---------------------------------------------------------------------------
 * GoepObjectReq structure
 *
 *     Used by the Client to provide information to GOEP during a Push,
 *     Pull or Delete request.
 */
typedef struct _GoepObjectReq {
    /* The Name of the object, must be null-terminated. The proper format
     * (ASCII/UNICODE) is based on the GOEP_DOES_UNICODE_CONVERSIONS option.
     */
    const uint8_t       *name;

    /* The Type of the object, null-terminated, in ASCII format. */
    const uint8_t       *type;

    /* The handle of the object.*/
    void                *object;

    /* In a PUSH request, the length of the object should be provided.
     * In PULL and DELETE requests this field is not used.
     */
    uint32_t                  objectLen;

} GoepObjectReq;

/*---------------------------------------------------------------------------
 * GoepPullCnf structure
 *
 *     This structure is indicated to the Client during PULL operations to
 *     provide information that may have been sent back to the client as 
 *     part of a pull operation.
 */
typedef struct _GoepPullCnf {
    /* This field indicates the length of the object being pulled. If zero,
     * then the server did not provide a value, or the object has no content.
     */
    uint32_t          objectLen;
} GoepPullCnf;

/*---------------------------------------------------------------------------
 * GoepHeaderInd structure
 * 
 *     This structure is used by the during Header indications.
 */
typedef struct _GoepHeaderInd {
    ObexHeaderType type;            /* OBEX Header Type */

    /* Group: These fields are valid for 1-byte and 4-byte headers only */
    uint32_t       value;           /* 1-byte or 4-byte header value */

    /* Group: These fields are valid for Unicode and Byte-Sequence headers only */
    uint8_t       *buffer;          /* Header buffer */
    uint16_t       len;             /* Length of the header buffer */

    /* Group: These fields are valid for Byte-Sequence headers only */
    uint16_t       totalLen;        /* Total header length */
    uint16_t       remainLen;       /* Remaining header length */

    /* Group: These fields are valid for Unicode headers only */

    /* The format of the unicode buffer (ASCII/UNICODE) is determined by the 
     * GOEP_DOES_UNICODE_CONVERSIONS option. 
     */
    uint8_t        unicodeBuffer[128]; /* "buffer" points to this data */

} GoepHeaderInd;

/*---------------------------------------------------------------------------
 * GoepClientEvent structure
 * 
 *     This structure is provided to the Client application each time an
 *     event is indicated. The 'event', 'oper' and 'handler' fields are
 *     always valid. Based on the 'oper' and/or the 'event' field the
 *     client can identify which of the 'info' structures is valid.
 */
typedef struct _GoepClientEvent {
    GoepEventType           event;      /* GOEP_EVENT_xxx */
    GoepOperation           oper;       /* GOEP_OPER_xxx */
    GoepClientApp          *handler;    /* App's registration structure */

    ObexRespCode            reason;     /* Check during GOEP_EVENT_ABORTED */

    union {
        GoepPullCnf         pull;       /* Use during GOEP_OPER_PULL */
    } info;

    GoepHeaderInd           header;     /* Use during GOEP_EVENT_HEADER_RX */
} GoepClientEvent;

/*---------------------------------------------------------------------------
 * GOEP_ClientAbort
 *
 *     Abort the current operation.
 *
 * Return:    ObStatus
 *
 */
ObStatus GOEP_ClientAbort(GoepClientApp *Client);

/*---------------------------------------------------------------------------
 * GOEP_Push
 *
 *    Issue an OBEX Put. To delete an object, set the Object Store
 *    Handle in the GoepObjectReq to zero.
 *
 * Return:    ObStatus
 *
 */
ObStatus GOEP_Push(GoepClientApp *Client, GoepObjectReq *Object);

/*---------------------------------------------------------------------------
 * GOEP_ClientContinue
 *
 *     Called to keep data flowing between the client and the server.
 *     This function must be called once for every GOEP_EVENT_CONTINUE
 *     event received. It does not have to be called in the context
 *     of the callback. It can be deferred for flow control reasons.
 *
 * Return:    ObStatus
 */
ObStatus GOEP_ClientContinue(GoepClientApp *Client);

#endif  // _GOEP_API_H
