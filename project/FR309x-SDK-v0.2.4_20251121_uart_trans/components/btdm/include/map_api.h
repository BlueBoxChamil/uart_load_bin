#ifndef __MAP_API_H
#define __MAP_API_H
#include "bt_types.h"
#include "sec_api.h"
#include "sdp_api.h"
#include "goep_api.h"

/*---------------------------------------------------------------------------
 * Message Access Profile (MAP) Layer 
 *
 *     The MAP layer provides functions for building and sending message 
 *     Access compliant messages.  These messages are based on the OBEX 
 *     protocol and are written directly on top of our GOEP profile layer.
 */

/****************************************************************************
 *
 * Section: Default general constants that can be overridden in overide.h
 *
 ***************************************************************************/

/*---------------------------------------------------------------------------
 * MAP_NUM_CLIENTS Constant
 *
 *  Defines the number of concurrent clients supported.
 *
 *  The default value is 1. Set to 0 to disable client support.
 */
#ifndef MAP_NUM_CLIENTS
#define MAP_NUM_CLIENTS            NUM_BT_DEVICES
#endif

/*---------------------------------------------------------------------------
 * MAP_NUM_SERVERS Constant
 *
 *  Defines the number of concurrent servers supported.
 *
 *  The default value is 1. Set to 0 to disable server support.
 */
#ifndef MAP_NUM_SERVERS
#define MAP_NUM_SERVERS            0
#endif

/*---------------------------------------------------------------------------
 * MAP_MAX_PASSWORD_LEN Constant
 *
 *  Defines the maximum length of the OBEX authentication password.
 *
 *  The default value is 20.
 */
#ifndef MAP_MAX_PASSWORD_LEN
#define MAP_MAX_PASSWORD_LEN       20
#endif

/*---------------------------------------------------------------------------
 * MAP_MAX_USERID_LEN Constant
 *
 *  Defines the maximum length of the OBEX authentication username.
 *
 *  The default value is 20.
 */
#ifndef MAP_MAX_USERID_LEN
#define MAP_MAX_USERID_LEN         20
#endif

/*---------------------------------------------------------------------------
 * MAP_MAX_REALM_LEN Constant
 *
 *  Defines the maximum length of the OBEX authentication realm.
 *
 *  The default value is 20.
 */
#ifndef MAP_MAX_REALM_LEN
#define MAP_MAX_REALM_LEN          20
#endif

/*---------------------------------------------------------------------------
 * MAP_MAX_APP_PARMS_LEN Constant
 *
 *  Defines the maximum length of the application parameter header information
 *  used within MAP.
 *
 *  The default value is 30.
 */
#ifndef MAP_MAX_APP_PARMS_LEN
#define MAP_MAX_APP_PARMS_LEN      30
#endif

#if MAP_NUM_CLIENTS == 0 && MAP_NUM_SERVERS == 0
#error Either MAP_NUM_CLIENTS or MAP_NUM_SERVERS should be enabled
#endif


/*---------------------------------------------------------------------------
 * Checks for correct configuration values.
 */
 
/*----------------------------------------------------------------------------
 * GET_FOLDER_LISTING_TYPE constant
 *
 *     The GET_FOLDER_LISTING_TYPE specifies the value that is sent by
 *     the client in an OBEX Type header when requesting a Folder-Listing
 *     object.
 */
#define GET_FOLDER_LISTING_TYPE            "x-obex/folder-listing"

 /*---------------------------------------------------------------------------
 * GET_MESSAGE_LISTING_TYPE constant
 *
 *     The GET_MESSAGE_LISTING_TYPE specifies the value that is sent by
 *     the client in an OBEX Type header when requesting the Messages-Listing
 *     object.
 */
#define GET_MESSAGE_LISTING_TYPE      "x-bt/MAP-msg-listing"

 /*---------------------------------------------------------------------------
 * SET_NOTIFICATION_REGISTRATION_TYPE constant
 *
 *     The SET_NOTIFICATION_REGISTRATION_TYPE specifies the value that is sent by
 *     the client in an OBEX Type header when register itself for being notified of
 *     the arrival of new messages.
 */
#define SET_NOTIFICATION_REGISTRATION_TYPE      "x-bt/MAP-NotificationRegistration"

 /*---------------------------------------------------------------------------
 * GET_MESSAGE_TYPE constant
 *
 *     The GET_MESSAGE_TYPE specifies the value that is sent by
 *     the client in an OBEX Type header when requesting the Messages
 *     object.
 */
#define GET_MESSAGE_TYPE      "x-bt/message"



/****************************************************************************
 *
 * Section: Types
 *
 ***************************************************************************/

/*---------------------------------------------------------------------------
 * MAPEvent type
 *
 *     Indicates the current event type. All event indications and 
 *     confirmations are passed to a callback function of type 
 *     MAPClientCallback or MAPServerCallback. The "MAPCallbackParms.event" 
 *     field indicates one of the event types below.  The 
 *     "MAPCallbackParms.oper" field indicates the applicable event 
 *     operation.
 */
typedef uint8_t MapEvent;

/* A transport layer connection has been established. There is no
 * operation associated with this event.
 */
#define MAP_EVENT_TP_CONNECTED         0x01

/* The transport layer connection has been disconnected. There is no
 * operation associated with this event.
 */
#define MAP_EVENT_TP_DISCONNECTED      0x02

/* Indicates that a map operation (see MapOp) has 
 * completed successfully.
 */
#define MAP_EVENT_COMPLETE             0x03

/* Indicates that the current operation has failed or was rejected
 * by the remote device.
 */
#define MAP_EVENT_ABORTED              0x04

/* Delivered to the application when it is time to issue
 * another request or response. The application must call either
 * MAP_ServerContinue() or MAP_ClientContinue(), internal use!.
 */
#define MAP_EVENT_CONTINUE             0x05

/* Delivered to the application when the Application Parameters
 * header information has been fully parsed, internal use!.
 */
#define MAP_EVENT_PARAMS_RX            0x06

/* Delivered to the application when MAP profile connected completely.*/
#define MAP_EVENT_CONNECTED            0x07

/* Delivered to the application when MAP data received.*/
#define MAP_EVENT_DATA_IND             0x0c



#if MAP_NUM_SERVERS > 0
/* Delivered to server application before the last response is going
 * to be sent. It provides the server with an opportunity to complete 
 * the requested operation (such as a SetPhonebook request) and if necessary
 * set an abort response code before the final response is sent (Server Only).
 */
#define MAP_EVENT_PRECOMPLETE          0x07

/* Indicates the start of a new operation. In the case of an Abort
 * operation, this event may occur during another pending operation 
 * (e.g. PullPhonebook).  In the case where MAP_ServerContinue() 
 * has not been called during this operation, perhaps awaiting Server 
 * object creation or additional buffer space, it should be called now 
 * or the Abort operation will not complete (Server Only).
 */
#define MAP_EVENT_START                0x08
    
/* Indicates a request for the phonebook parameters needed for the next
 * response. The application should provide these parameters through the 
 * Phonebook Access server callback parameters. This situation occurs only 
 * during Pull Phonebook and Pull VCard Listing operations.  The phonebook
 * size parameter is only needed when a maxListCount of zero is 
 * received from the client device during the MAP_EVENT_PARAMS_RX event
 * (Server only).
 */
#define MAP_EVENT_PROVIDE_PARAMS       0x09

/* Indicates a request for the phonebook object data length. The 
 * application should provide the length of the object being 
 * requested through the Phonebook Access server callback parameters
 * Failure to do so will cause the operation to not send an object. 
 * (Server Only)
 */
#define MAP_EVENT_OBJECT_LENGTH_REQ    0x0a

/* Indicates a request for data. The application should provide the
 * data length requested, and provide a valid buffer pointer through the
 * Phonebook Access server callback parameters. Different callback 
 * parameters exist based on whether MAPOBS_Read or MAPOBS_ReadFlex 
 * have triggered this event (Server only).
 */
#define MAP_EVENT_DATA_REQ             0x0b
#endif /* MAP_NUM_SERVERS > 0 */

#if MAP_NUM_CLIENTS > 0
/* Informs the client application of data that has been received. The
 * data length and data buffer pointer that are provided must be handled
 * during this callback, as the memory is only good until this callback
 * returns (Client only). 
 */
#define MAP_EVENT_DATA_IND             0x0c
#endif /* MAP_NUM_CLIENTS > 0 */

/* End of MAPEvent */

/*---------------------------------------------------------------------------
 * MAPOp type
 *
 *     Indicates the operation type of the current event. Each event 
 *     indication has an associated operation passed to a callback function 
 *     of type MAPClientCallback or MAPServerCallback. The 
 *     "MAPCallbackParms.oper" field will indicate one of the operation 
 *     types below.  Since the Pull Phonebook, Pull Vcard Listing, and Pull
 *     Vcard Entry operations cannot be known until the actual OBEX headers
 *     are processed, the initial operation start event will indicate merely
 *     that a generic Pull operation is occurring, until further information
 *     arrives.  The exact operation will be known either during the 
 *     MAP_EVENT_PARAMS_RX or MAP_EVENT_OBJECT_LENGTH_REQ events
 */
typedef uint32_t MapOp;

#define MAPOP_NONE                      0x00 /* No current operation */
#define MAPOP_CONNECT                   0x01 /* Connect operation */
#define MAPOP_DISCONNECT                0x02 /* Disconnect operation */
#define MAPOP_GET_FOLDER_LISTING        0x03 /* Get folder listing */
#define MAPOP_SET_NOTIFY_RESTRATION     0x04 /* Notification registration */
#define MAPOP_SET_FOLDER                0x05 /* Set folder*/
/* End of MAPOp */

/*---------------------------------------------------------------------------
 * MapRespCode type
 *
 *     Response codes used to signify the status of a MAP operation. They
 *     are included within OBEX Response packets, which are sent out to other 
 *     devices.
 *
 *     The first two response codes listed below are success responses; the 
 *     remainder are considered failures. The failure codes are frequently 
 *     associated with aborted operations.
 */
typedef uint8_t MapRespCode;
#if 1
/* Group: Successful response codes */

#define MRC_CONTINUE              0x10 /* Continue */
#define MRC_STATUS_SUCCESS        0x20 /* Success */

/* Group: Failure response codes */

#define MRC_BAD_REQUEST           0x40 /* Bad Request */
#define MRC_UNAUTHORIZED          0x41 /* Unauthorized */
#define MRC_FORBIDDEN             0x43 /* Forbidden - operation is understood */
#define MRC_NOT_FOUND             0x44 /* Not Found */
#define MRC_NOT_ACCEPTABLE        0x46 /* Not Acceptable */
#define MRC_PRECONDITION_FAILED   0x4c /* Precondition Failed */
#define MRC_NOT_IMPLEMENTED       0x51 /* Not Implemented */
#define MRC_SERVICE_UNAVAILABLE   0x53 /* Service Unavailable */
#define MRC_LINK_DISCONNECT       0x80 /* Transport connection has been disconnected. */
#endif
/* End of MapRespCode */

/*---------------------------------------------------------------------------
 * MapAppParmsTag type
 *
 *     Describes the tag values used in the Application Parameters OBEX header
 *     that are used on both the Message Access client and server side.
 */
typedef uint8_t MapAppParmsTag;

#define MAP_TAG_MAX_LIST_COUNT          0x01
#define MAP_TAG_LIST_START_OFFSET       0x02
#define MAP_TAG_FILTER_MESSAGE_TYPE     0x03
#define MAP_TAG_FILTER_PERIOD_BEGIN     0x04
#define MAP_TAG_FILTER_PERIOD_END       0x05
#define MAP_TAG_FILTER_READ_STATUS      0x06
#define MAP_TAG_FILTER_RECIPIENT        0x07
#define MAP_TAG_FILTER_ORIGINATOR       0x08
#define MAP_TAG_FILTER_PRIORITY         0x09
#define MAP_TAG_ATTACHMENT              0x0a
#define MAP_TAG_TRANSPARENT             0x0b
#define MAP_TAG_RETRY                   0x0c
#define MAP_TAG_NEW_MESSAGE             0x0d
#define MAP_TAG_NOTIFICATION_STATUS     0x0e
#define MAP_TAG_MAS_INSTANCE_ID         0x0f
#define MAP_TAG_PARAMETER_MASK          0x10
#define MAP_TAG_FOLDER_LISTING_SIZE     0x11
#define MAP_TAG_LISTING_SIZE            0x12
#define MAP_TAG_SUBJECT_LENGTH          0x13
#define MAP_TAG_CHARSET                 0x14
#define MAP_TAG_FRACTION_REQUEST        0x15
#define MAP_TAG_FRACTION_DELIVER        0x16
#define MAP_TAG_STATUS_INDICATOR        0x17
#define MAP_TAG_STATUS_VALUE            0x18
#define MAP_TAG_MSE_TIME                0x19
#define MAP_TAG_DATABASE_IDENTIFIER     0x1a
#define MAP_TAG_LISTING_VERSION_COUNTER 0x1b
#define MAP_TAG_PRESENCE_AVAILABILITY   0x1c
#define MAP_TAG_PRESENCE_TEXT           0x1d
#define MAP_TAG_LAST_ACTIVITY           0x1e
#define MAP_TAG_FILTER_LAST_ACT_BEGIN   0x1f
#define MAP_TAG_FILTER_LAST_ACT_END     0x20
#define MAP_TAG_CHAT_STATE              0x21
#define MAP_TAG_CONVERSATION_ID         0x22
#define MAP_TAG_FOLDER_VERSION_COUNTER  0x23
#define MAP_TAG_FILTER_MESSAGE_HANDLE   0x24
#define MAP_TAG_NOTIFICAION_FILTER_MASK 0x25
#define MAP_TAG_CONV_PARAMETER_MAST     0x26
#define MAP_TAG_OWNER_UCI               0x27
#define MAP_TAG_EXTENDED_DATA           0x28
#define MAP_TAG_MAP_SUPPORTED_FEATURES  0x29
#define MAP_TAG_MESSAGE_HANDLE          0x2a
#define MAP_TAG_MODIFY_TEXT             0x2b

/* End of MapAppParmsTag */

/*---------------------------------------------------------------------------
 * MapSetFolderFlags type
 *
 *     Flags used in the SetFolder operation. 
 */
typedef uint8_t MapSetFolderFlags;

#define MAP_SET_FLAG_ROOT              0x02    /* reset to root directory */
#define MAP_SET_FLAG_GO_DOWN           0x02    /* go down to child folder depends on the name header */
#define MAP_SET_FLAG_GO_UP             0x03    /* go up to pararent folder or brother folder depends on the name header */

/* End of MapSetFolderFlags */

/*---------------------------------------------------------------------------
 * MapAttachment type
 *
 *     Flags used in the Get message operation. 
 */
typedef uint8_t MapAttachment;

#define MAP_ATTACH_ON               0x01
#define MAP_ATTACH_OFF              0x00

/* End of MapAttachment */

/*---------------------------------------------------------------------------
 * MapCharset type
 *
 *     Flags used in the Get message operation. 
 */
typedef uint8_t MapCharset;

#define MAP_CHARSET_NATIVE               0x00
#define MAP_CHARSET_UTF8                 0x01

/* End of MapCharset */



#if MAP_NUM_CLIENTS > 0

/* Forward reference to MapClientCallbackParms defined below */
typedef struct _MapClientCallbackParms MapClientCallbackParms;

/*---------------------------------------------------------------------------
 * MapClientCallback type
 *
 *  Message Access Client events along with data indications are 
 *  passed to the application through a callback function of this type 
 *  defined by the application.
 */
typedef void (*MapClientCallback)(MapClientCallbackParms *parms);

/* End of MapClientCallback */
#endif /* MAP_NUM_CLIENTS > 0 */

#if MAP_NUM_SERVERS > 0

/* Forward reference to MapServerCallbackParms defined below */
typedef struct _MapServerCallbackParms MapServerCallbackParms;

/*---------------------------------------------------------------------------
 * MapServerCallback type
 *
 *  Message Access Server events along with data requests are 
 *  passed to the application through a callback function of this type 
 *  defined by the application.
 */
typedef void (*MapServerCallback)(MapServerCallbackParms *parms);

/* End of MapServerCallback */
#endif /* MAP_NUM_SERVERS > 0 */

typedef uint8_t MapAuthFlags;   /* Used Internally */

/****************************************************************************
 *
 * Section: Data Structures
 *
 ****************************************************************************/

#if MAP_NUM_CLIENTS > 0
/*----------------------------------------------------------------------
 * MapClientSession structure
 *
 *  Maintains Message Access client info and status during an 
 *  application session.
 */
 typedef struct _MapClientSession 
 {
#if BT_SECURITY == XA_ENABLED
    /* Bluetooth security level - set by the application 
     * prior to MAP_RegisterClient.  An application must have a
     * pairing handler registered in order to receive PIN requests.
     * This can be done through the BlueMgr API or done directly
     * by the application.
     */
    BtSecurityLevel     secLevel;
#endif /* BT_SECURITY == XA_ENABLED */

    /* GOEP Client instance */
    GoepClientApp       cApp;       

    /* === Internal use only === */
    MapOp              currOp;
    MapAuthFlags       flags;
    uint8_t            appParms[MAP_MAX_APP_PARMS_LEN];
    SdpRecord           record;
    SdpAttribute        attributes[5];
    
    /* Registration status for this MAP Client instance */
    int                registered;
    /* GOEP Connect information */
    GoepConnectReq      connect;
    /* GOEP Pull information */
    GoepObjectReq       pull;

} MapClientSession;
#endif /* MAP_NUM_CLIENTS > 0 */


/*---------------------------------------------------------------------------
 * MapGetFolderListParms structure
 *
 *  Describes the parameters required to issue a MAP_ClientGetFolderListing operation.
 *  These parameters must be filled in by the client application prior to 
 *  calling MAP_ClientGetFolderListing.
 */
typedef struct _MapGetFolderListParms
{
    /* Maximum number of vCard entries supported by the client */
    uint16_t                 maxListCount;
    /* Offset of the first vCard entry */
    uint16_t                 listStartOffset; 
} MapGetFolderListParms;


/*---------------------------------------------------------------------------
 * MapGetMessagesListParms structure
 *
 *  Describes the parameters required to issue a MAP_ClientGetMessagesListing operation.
 *  These parameters must be filled in by the client application prior to 
 *  calling MAP_ClientGetMessagesListing.
 */
typedef struct MapGetMessagesListParms
{
    /* Full path information including the phonebook name (null-terminated) 
     * (e.g. "telecom\pb.vcf") 
     */
    const uint8_t  *mapName;
    /* Maximum number of vCard entries supported by the client */
    uint16_t                 maxListCount;
    /* Offset of the first vCard entry */
    uint16_t                 listStartOffset; 

    uint8_t                  subjectLen;
    uint32_t                 paramMask;
    uint8_t                  filterMessageType;
    uint8_t                  *filterPeriodBegin;
    uint8_t                  *filterPeriodEnd;
    uint8_t                  filterReadStatus;
    uint8_t                  *filterRecipient;
    uint8_t                  *filterOriginator;
    uint8_t                  filterPriority;
    uint8_t                  *conversationId;
    uint8_t                  *filterMessageHandle;
} MapGetMessagesListParms;

/*---------------------------------------------------------------------------
 * MapSetFolderParms structure
 *
 *  Describes the parameters required to issue a Set Phonebook operation.
 *  These parameters must be filled in by the client application prior to 
 *  calling MAP_ClientSetFolder.
 */
typedef struct _MapSetFolderParms
{
    /* Name of the folder (null-terminated) */
    const uint8_t  *folderName;

    /* Set Phonebook operation options. PBAP specification requires
     * that the PBAP_SETPB_DONT_CREATE flag is always set. 
     */
    MapSetFolderFlags      flags;

    /* Set this flag to true to reset the path to the root folder. 
     * No path flags may be specified if this is TRUE.  In addition,
     * any name provided will be ignored if this is TRUE, since reset 
     * uses an empty name header.
     */
    int                reset;
} MapSetFolderParms;

/*---------------------------------------------------------------------------
 * MapGetMessageParms structure
 *
 *  Describes the parameters required to issue a Set Phonebook operation.
 *  These parameters must be filled in by the client application prior to 
 *  calling MAP_ClientGetMessage.
 */
typedef struct _MapGetMessageParms
{
    /* Name of the message handle (null-terminated) */
    const int  *handleName;

    /* Show message attachment or not */
    MapAttachment      attachment;
    
    /* use native charset or utf8 */
    MapCharset         charset;
} MapGetMessageParms;



#if MAP_NUM_SERVERS > 0
/*---------------------------------------------------------------------------
 * MapServerParamsRx structure
 * 
 *     This structure is used by the Server to store the received Application
 *     Parameters.
 */
typedef struct _MapServerParamsRx {
    uint8_t              *name; /* Object name, null terminated ASCII or UTF-16 */
} MapServerParamsRx;

/*----------------------------------------------------------------------
 * MapServerSession structure
 *
 *  Maintains Phonebook Access server info and status during an 
 *  application session.
 */
typedef struct _MapServerSession 
{
#if BT_SECURITY == XA_ENABLED
    /* Bluetooth security level - set by the application 
     * prior to MAP_RegisterServer.  An application must have a
     * pairing handler registered in order to receive PIN requests.
     * This can be done through the BlueMgr API or done directly
     * by the application.
     */
    BtSecurityLevel     secLevel;
#endif /* BT_SECURITY == XA_ENABLED */

    /* GOEP Server instance */
    GoepServerApp       sApp;

    /* === Internal use only === */   
    MapOp              currOp;
    BOOL                rcvdAppParms;
    int                handledAppParms;
    //MapAuthFlags       flags;
    MapServerParamsRx  paramsRx;
    uint8_t                  appParms[MAP_MAX_APP_PARMS_LEN];
    SdpRecord           record;
    SdpAttribute        attributes[7];

    /* MAP object store handle */
    //MapObStoreHandle   object;
    /* Registration status for this MAP Server instance */
    int                registered;
    /* GOEP Connect information */
    ObexConnection      obexConn;       

    uint16_t                 maxListCount;
} MapServerSession;
#endif /* MAP_NUM_SERVERS > 0 */

#if MAP_NUM_CLIENTS > 0
/*---------------------------------------------------------------------------
 * MapClientCallbackParms structure
 *
 *  Describes a callback event and any data that relates to the event. These
 *  callback parameters are used for all Message Access Clients.  
 *  Determination of the valid fields can be done by evaluating which event 
 *  is being indicated as well as which client it is intended for.
 */
struct _MapClientCallbackParms
{
    MapEvent               event;          /* MAP event */
    MapOp                  oper;           /* MAP operation */
    MapClientSession      *client;         /* MAP client */

    union {

        /* Group: Valid during MAP_EVENT_TP_DISCONNECTED event */
        MapRespCode        discReason;     /* MAP disconnect reason code */

        /* Group: Valid during MAP_EVENT_ABORTED event */
        MapRespCode        abortReason;    /* MAP abort reason code */
        
        #if 1
        struct {
            uint8_t      new_msg;
            uint16_t     listing_size;
            uint8_t      *mse_time;
            uint8_t      mse_time_len;
        } paramsRx;
        #endif
        
        /* Group: Valid during MAP_EVENT_DATA_IND event */
        struct {
            /* Object name (null-terminated, ASCII or UTF-16) */
            uint8_t    *name;           /* Name pointer */
            /* Data Indication */
            uint8_t             *buffer;         /* Data pointer */
            uint16_t             len;            /* Length of data */
        } dataInd;
    } u;
};
/* End of MapClientCallbackParms */
#endif /* MAP_NUM_CLIENTS > 0 */

#if MAP_NUM_SERVERS > 0

/*---------------------------------------------------------------------------
 * MapServerCallbackParms structure
 *
 *  Describes a callback event and any data that relates to the event. These
 *  callback parameters are used for all Message Access Servers.  
 *  Determination of the valid fields can be done by evaluating which event 
 *  is being indicated as well as which server it is intended for.
 */
struct _MapServerCallbackParms
{
    MapEvent               event;          /* MAP event */
    MapOp                  oper;           /* MAP operation */
    MapServerSession      *server;         /* MAP server */

    union {

        /* Group: Valid during MAP_EVENT_TP_DISCONNECTED event */
        MapRespCode        discReason;     /* MAP disconnect reason code */

        /* Group: Valid during MAP_EVENT_PROVIDE_PARAMS event - requests 
         * the application to provide Application Parameter header information 
         * for the next response packet. Valid for Pull Phonebook and Pull 
         * Vcard Listing operations only.
         */
        struct {
            /* Number of new missed calls - to be set by the application */
            uint8_t              newMissedCalls;
            /* Size of the requested phonebook. The server should provide 
             * this value if the received client maxListCount was zero 
             */
            uint16_t             phonebookSize;
            /* Indicates that the phonebook parameters above are not
             * included during the context of this event, but will instead 
             * be provided asynchronously through a call to the 
             * MAP_ServerSetMParams function. 
             */
            int            asynchronous;
        } provideParams;

        /* Group: Valid during MAP_EVENT_PARAMS_RX event - provides Application
         * Parameter header information.
         */
        MapServerParamsRx  paramsRx;

        /* Group: Valid during MAP_EVENT_OBJECT_LENGTH_REQ event */
        struct {
            /* Object name (null-terminated, ASCII) */
            uint8_t    *name;
            /* Object Length - to be set by the application */
            uint32_t             objectLen;
        } objectReq;

        /* Group: Valid during MAP_EVENT_DATA_REQ event */
        struct {

            /* Object name (null-terminated, ASCII or UTF16) */
            uint8_t    *name;

            /* Data buffer pointer - to be assigned by the application */
            uint8_t             *buffer;     

            /* Group: Valid for MAP_DATAREQ_READ type only */
            struct {
                /* Length of the requested data */
                uint16_t         len;
            } req;
        } dataReq;

        #if 0
        /* Group: Valid during MAP_EVENT_PRECOMPLETE and MAP_EVENT_COMPLETE events */
        union {
            MapPullInd     pull;           /* Valid during Pull operations */
            MapSetMInd    setM;          /* Valid during Set Phonebook operation */
        } info;
        #endif
    } u;
};
/* End of MapServerCallbackParms */
#endif /* MAP_NUM_SERVERS > 0 */

/****************************************************************************
 *
 * Section: Function Reference
 *
 ****************************************************************************/

/*---------------------------------------------------------------------------
 * MAP_Init()
 *
 *      Initialize the MAP component.  This must be the first MAP 
 *      function called by the application layer, or if multiple 
 *      MAP applications exist, this function should be called
 *      at system startup (see XA_LOAD_LIST in config.h).  GOEP and 
 *      OBEX must also be initialized separately.
 *
 * Returns:
 *     TRUE - Initialization was successful.
 *
 *     FALSE - Initialization failed.
 */
int MAP_Init(void);

#if MAP_NUM_SERVERS > 0
/*---------------------------------------------------------------------------
 * MAP_RegisterServer()
 *
 *     NOTE: This API is deprecated.   Use MAP_RegisterServerSec instead.
 *
 *     Registers the Phonebook Access Server with the GOEP multiplexor. This 
 *     includes registering the SDP records for the Phonebook Access Service.
 *     All of the events specified in the GOEP layer are delivered to the
 *     MAP server.
 *
 * Parameters:
 *     Server - The server application's registration structure. The
 *          'connFlags' and/or 'connId' can be filled in to overide
 *          the automatic multiplexing behavior.
 *
 *     Callback - callback function for the Phonebook Access server.
 *
 * Returns:
 *     OB_STATUS_INVALID_PARM - Invalid or in use profile type.
 *
 *     OB_STATUS_SUCCESS - The MAP Server is initialized.
 *
 *     OB_STATUS_FAILED - Problem initializing the MAP Server.
 *     
 *     OB_STATUS_BUSY - A MAP server is already registered.
 *     
 */
ObStatus MAP_RegisterServer(MapServerSession *Server, 
                             MapServerCallback Callback);
#define MAP_RegisterServer(_SERVER, _CALLBACK) \
        MAP_RegisterSrvr(_SERVER, _CALLBACK, 0, TRUE);

/*---------------------------------------------------------------------------
 * MAP_RegisterServerSec()
 *
 *     Registers the Phonebook Access Server with the GOEP multiplexor. This 
 *     includes registering the SDP records for the Phonebook Access Service.
 *     All of the events specified in the GOEP layer are delivered to the
 *     MAP server.
 *
 * Parameters:
 *     Server - The server application's registration structure. The
 *          'connFlags' and/or 'connId' can be filled in to overide
 *          the automatic multiplexing behavior.
 *
 *     Callback - callback function for the Phonebook Access server.
 *
 *     SecParms - Pointer to the Security Parameters (level and PIN length)
 *          used by the MAP Server.
 *
 * Returns:
 *     OB_STATUS_INVALID_PARM - Invalid or in use profile type.
 *
 *     OB_STATUS_SUCCESS - The MAP Server is initialized.
 *
 *     OB_STATUS_FAILED - Problem initializing the MAP Server.
 *     
 *     OB_STATUS_BUSY - A MAP server is already registered.
 *     
 */
ObStatus MAP_RegisterServerSec(MapServerSession *Server, 
                                MapServerCallback Callback,
                                BtSecurityParms *SecParms);
#define MAP_RegisterServerSec(_SERVER, _CALLBACK, _PARMS) \
        MAP_RegisterSrvr(_SERVER, _CALLBACK, _PARMS, FALSE);

#if OBEX_DEINIT_FUNCS == XA_ENABLED
/*---------------------------------------------------------------------------
 * MAP_DeregisterServer()
 *
 *     Deregisters the Phonebook Access Server from the GOEP multiplexor.
 *     This includes removing the SDP records for the Phonebook Access Service.
 *     
 * Parameters:
 *     Server - The server used in the registration call.
 *
 * Returns:
 *     OB_STATUS_SUCCESS - The server was deregistered.
 *
 *     OB_STATUS_INVALID_PARM - The server is not registered.
 *
 *     OB_STATUS_BUSY - The server could not be deregistered because
 *          it is currently processing an operation.
 *
 *     OB_STATUS_FAILED - The MAP server failed to deinitialize.
 */
ObStatus MAP_DeregisterServer(MapServerSession *Server);
#endif /* OBEX_DEINIT_FUNCS == XA_ENABLED */

/*---------------------------------------------------------------------------
 * MAP_ServerContinue()
 *
 *     This function is called by the server in response to a received
 *     MAP_EVENT_CONTINUE event. It must be called once for every
 *     CONTINUE event received. It may or may not be called in the context
 *     of the callback and can be deferred for flow control purposes.
 *     
 * Parameters:
 *     Server - The server application's registration structure.
 *
 * Returns:
 *     OB_STATUS_FAILED - The server is not expecting a continue.
 *
 *     OB_STATUS_SUCCESS - The continue was successful.
 */
ObStatus MAP_ServerContinue(MapServerSession *Server);

/*---------------------------------------------------------------------------
 * MAP_ServerAbort()
 *
 *     Aborts the current server operation.
 *     
 * Parameters:
 *     Server - The server application's registration structure.
 *
 *     Resp - The response code to send to the client expressing the reason
 *         for the abort.
 *
 * Returns:
 *     OB_STATUS_SUCCESS - The abort was successfully requested.
 *
 *     OB_STATUS_FAILED - The abort operation failed.
 *            
 *     OB_STATUS_NO_CONNECT - No transport connection exists.
 *
 *     OB_STATUS_INVALID_PARM - Invalid parameters.
 */
ObStatus MAP_ServerAbort(MapServerSession *Server, MapRespCode Resp);



/*---------------------------------------------------------------------------
 * MAP_ServerSetMParams()
 *
 *     This function is used by the Server to setup the phonebook parameters
 *     required in the Pull Phonebook and Pull Vcard Listing operations.  This
 *     function is called prior to MAP_ServerContinue to ensure the phonebook 
 *     parameters are setup and built as an Application Parameters OBEX header 
 *     for the MAP response.
 *
 *     This function is only needed when the phonebook parameters cannot be
 *     provided during the context of the MAP_EVENT_PROVIDE_PARAMS event, 
 *     such as in cases where asynchronous access to the phonebook storage is
 *     required.
 *
 * Parameters:
 *     Server - The server application's registration structure.
 *
 *     Params - The server Message parameters.
 *
 * Returns:
 *     OB_STATUS_INVALID_PARM - Invalid parameter.
 *
 *     OB_STATUS_SUCCESS - The phonebook parameters have been set.
 *
 *     OB_STATUS_FAILED - Problem setting the phonebook parameters.
 */
ObStatus MAP_ServerSetMParams(MapServerSession *Server, MapSetMParams *Params);

#endif /* MAP_NUM_SERVERS > 0 */

#if MAP_NUM_CLIENTS > 0
/*---------------------------------------------------------------------------
 * MAP_RegisterClient()
 *
 *     NOTE: This API is deprecated.  Use MAP_RegisterClientSec instead.
 *
 *     Registers the Phonebook Access Client with the GOEP multiplexor.
 *     All of the events specified in the MAP layer are delivered to the
 *     MAP client.
 *     
 * Parameters:
 *     Client - The client application's registration structure.
 *
 *     Callback - callback function for the Phonebook Access client.
 *
 * Returns:
 *     OB_STATUS_INVALID_PARM - Invalid or in use profile type.
 *
 *     OB_STATUS_SUCCESS - The MAP Client is initialized.
 *
 *     OB_STATUS_FAILED - Problem initializing the MAP Client.
 *
 *     OB_STATUS_BUSY - A MAP client is already registered.
 *     
 */
ObStatus MAP_RegisterClient(MapClientSession *Client, 
                             MapClientCallback Callback);
#define MAP_RegisterClient(_CLIENT, _CALLBACK) \
        MAP_RegisterClnt(_CLIENT, _CALLBACK, 0, TRUE);

/*---------------------------------------------------------------------------
 * MAP_RegisterClientSec()
 *
 *     Registers the Phonebook Access Client with the GOEP multiplexor.
 *     All of the events specified in the MAP layer are delivered to the
 *     MAP client.
 *     
 * Parameters:
 *     Client - The client application's registration structure.
 *
 *     Callback - callback function for the Phonebook Access client.
 *
 *     SecParms - Pointer to the Security Parameters (level and PIN length)
 *          used by the MAP Client.
 *
 * Returns:
 *     OB_STATUS_INVALID_PARM - Invalid or in use profile type.
 *
 *     OB_STATUS_SUCCESS - The MAP Client is initialized.
 *
 *     OB_STATUS_FAILED - Problem initializing the MAP Client.
 *
 *     OB_STATUS_BUSY - A MAP client is already registered.
 *     
 */
ObStatus MAP_RegisterClientSec(MapClientSession *Client, 
                                MapClientCallback Callback,
                                BtSecurityParms *SecParms);
ObStatus MAP_RegisterClnt(MapClientSession *Client, MapClientCallback Callback,
                           BtSecurityParms *SecParms, int UseOldSecApis);
#define MAP_RegisterClientSec(_CLIENT, _CALLBACK, _PARMS) \
        MAP_RegisterClnt(_CLIENT, _CALLBACK, _PARMS, FALSE);


/*---------------------------------------------------------------------------
 * MAP_ClientConnect()
 *
 *     This function is used by the client to create a transport connection
 *     to the specified device and issue an OBEX Connect Request. If the 
 *     transport connection was issued by the server, this function will only
 *     issue the OBEX Connect Request.
 *
 * Parameters:
 *     Client - The structure used to register the client.
 *     
 *     Target - This structure describes the server to which the client
 *         wishes to connect.
 *
 * Returns:
 *     OB_STATUS_PENDING - Connection was successfully started. Completion
 *         will be signaled via a call to the application callback.
 *
 *         If the connection is successful, a MAP_EVENT_COMPLETE event for
 *         the MAPOP_CONNECT operation will be signaled.
 *
 *         If the transport connection is successful, but the OBEX Connect
 *         failed, the completion event will be MAP_EVENT_TP_DISCONNECTED
 *         for the operation MAP_OPER_CONNECT. At this point the transport
 *         is DISCONNECTED. 
 * 
 *         If the transport connection is unsuccessful, the completion event
 *         will be MAP_EVENT_TP_DISCONNECTED.
 *
 *     OB_STATUS_SUCCESS - The client is now connected.
 *
 *     OB_STATUS_FAILED - Unable to start the operation because the client
 *         is in the middle of starting up a connection.
 *     
 *     OB_STATUS_BUSY - The client is currently executing an operation.
 *
 */
ObStatus MAP_ClientConnect(MapClientSession *Client, ObexTpAddr *Target);

/*---------------------------------------------------------------------------
 * MAP_ClientDisconnect()
 *
 *     This function is used by the client to issue an OBEX Disconnect Request.
 *     When the OBEX Disconnect is completed, the transport connection will
 *     be disconnected automatically.
 *     
 * Parameters:
 *     Client - The client application's registration structure.
 *
 * Returns:
 *     OB_STATUS_PENDING - Operation was successfully started.
 *         Completion will be signaled with the MAP_EVENT_TP_DISCONNECTED 
 *         event to the application callback.
 *
 *     OB_STATUS_BUSY - Operation was not started because
 *         the client is currently executing another operation.
 *
 *     OB_STATUS_NO_CONNECT - Operation was not started because
 *         the client is not connected.
 */
ObStatus MAP_ClientDisconnect(MapClientSession *Client);

/*---------------------------------------------------------------------------
 * MAP_ClientTpDisconnect()
 *
 *     This function is used by the client to initiate a transport 
 *     disconnection. MAP_ClientDisconnect will cause the transport to be 
 *     brought down automatically, but if the transport must be brought 
 *     down immediately, perhaps due an error condition, this routine may 
 *     be used. 
 *     
 * Parameters:
 *     Client - The client application's registration structure.
 *
 * Returns:
 *     OB_STATUS_PENDING - Operation was successfully started.
 *         Completion will be signaled with the MAP_EVENT_TP_DISCONNECTED 
 *         event to the application callback.
 *
 *     OB_STATUS_NO_CONNECT - Operation was not started because
 *         the client is not connected.
 */
ObStatus MAP_ClientTpDisconnect(MapClientSession *Client);

/*---------------------------------------------------------------------------
 * MAP_ClientAbort()
 *
 *     Aborts the current client operation. The completion event will signal
 *     the status of the operation in progress, either COMPLETE or ABORTED.
 *     
 * Parameters:
 *     Client - The client application's registration structure.
 *
 * Returns:
 *     OB_STATUS_SUCCESS - Operation was successful.
 *
 *     OB_STATUS_FAILED - Operation failed to start.
 *
 *     OB_STATUS_NO_CONNECT - Operation was not started because
 *         the client is not connected.
 *
 *     OB_STATUS_INVALID_PARM - Invalid parameter.
 */
ObStatus MAP_ClientAbort(MapClientSession *Client);

/*---------------------------------------------------------------------------
 * MAP_ClientContinue()
 *
 *     This function is called by the client in response to a received
 *     MAP_EVENT_CONTINUE event. It must be called once for every
 *     CONTINUE event received. It may or may not be called in the context
 *     of the callback and can be deferred for flow control purposes.
 *     
 * Parameters:
 *     Client - The client application's registration structure.
 *
 * Returns:
 *     OB_STATUS_FAILED - The client is not expecting a continue.
 *
 *     OB_STATUS_SUCCESS - The continue was successful.
 */
ObStatus MAP_ClientContinue(MapClientSession *Client);

/*---------------------------------------------------------------------------
 * MAP_ClientGetMessagesListing()
 *
 *     Initiates the OBEX "Get" operation to retrieve Messages-Listing objects 
 *     from the remote Message Access Server.  
 *
 * Parameters:
 *     Client - The client application's registration structure.
 *
 *     Parms - Parameters to specify the application specific parameters
 *             including the folder name.
 *
 * Returns:
 *     OB_STATUS_PENDING - Operation was successfully started. Completion
 *         will be signaled via an event to the application callback.
 *
 *     OB_STATUS_FAILED - Operation was failed to start 
 *     
 *     OB_STATUS_NO_CONNECT - Operation was not started because
 *         the client is not connected.
 *     
 *     OB_STATUS_INVALID_PARM - An invalid function parameter was provided.
 */
ObStatus MAP_ClientGetMessagesListing(MapClientSession *Client, MapGetMessagesListParms *Parms);

/*---------------------------------------------------------------------------
 * MAP_ClientSetFolder()
 *
 *     Initiates the OBEX "SetPath" operation to to set the path to navigate
 *     the folders of the MSE. 
 *
 * Parameters:
 *     Client - The client application's registration structure.
 *
 *     Parms - Parameters to specify the application specific parameters
 *             including the folder name.
 *
 * Returns:
 *     OB_STATUS_PENDING - Operation was successfully started. Completion
 *         will be signaled via an event to the application callback.
 *
 *     OB_STATUS_FAILED - Operation was failed to start 
 *     
 *     OB_STATUS_NO_CONNECT - Operation was not started because
 *         the client is not connected.
 *     
 *     OB_STATUS_INVALID_PARM - An invalid function parameter was provided.
 */
ObStatus MAP_ClientSetFolder(MapClientSession *Client, MapSetFolderParms *Parms);

#endif /* MAP_NUM_CLIENTS > 0 */

#endif /* __MAP_H */
