#ifndef __PBAP_API_H
#define __PBAP_API_H
#include "bt_types.h"
#include "sec_api.h"
#include "sdp_api.h"
#include "goep_api.h"

/*---------------------------------------------------------------------------
 * PBAP_MAX_APP_PARMS_LEN Constant
 *
 *  Defines the maximum length of the application parameter header information
 *  used within PBAP.
 *
 *  The default value is 30.
 */
#ifndef PBAP_MAX_APP_PARMS_LEN
#define PBAP_MAX_APP_PARMS_LEN      50
#endif

/*---------------------------------------------------------------------------
 * PBAP_FILTER_SIZE constant
 *
 *     This constant defines the size (in bytes) of the vCard filter used 
 *     in the Pull Phonebook and Pull Vcard Entry operations.
 */
#define PBAP_FILTER_SIZE                8

/****************************************************************************
 *
 * Section: Default general constants that can be overridden in overide.h
 *
 ***************************************************************************/

/*----------------------------------------------------------------------------
 * PULL_PHONEBOOK_TYPE constant
 *
 *     The PULL_PHONEBOOK_TYPE specifies the value that is sent by
 *     the client in an OBEX Type header when requesting a phonebook
 *     object.
 */
#define PULL_PHONEBOOK_TYPE            "x-bt/phonebook"

 /*---------------------------------------------------------------------------
 * VCARD_LISTING_OBJECT_TYPE constant
 *
 *     The VCARD_LISTING_OBJECT_TYPE specifies the value that is sent by
 *     the client in an OBEX Type header when requesting the vCard listing
 *     object.
 */
#define VCARD_LISTING_OBJECT_TYPE      "x-bt/vcard-listing"

/*----------------------------------------------------------------------------
 * VCARD_OBJECT_TYPE constant
 *
 *     The VCARD_OBJECT_TYPE specifies the value that is sent by
 *     the client in an OBEX Type header when requesting a vCard
 *     object.
 */
#define VCARD_OBJECT_TYPE              "x-bt/vcard"


/*---------------------------------------------------------------------------
 * PB_LOCAL_STORE_NAME constant
 *
 *     String constant for the local phonebook name.
 */
#define PB_LOCAL_STORE_NAME     "telecom/pb.vcf"

/*---------------------------------------------------------------------------
 * PB_LOCAL_OCH_NAME constant
 *
 *     String constant for the local phonebook outgoing call history.
 */
#define PB_LOCAL_OCH_NAME       "telecom/och.vcf"

/*---------------------------------------------------------------------------
 * PB_LOCAL_ICH_NAME constant
 *
 *     String constant for the local phonebook incoming call history.
 */
#define PB_LOCAL_ICH_NAME       "telecom/ich.vcf"

/*---------------------------------------------------------------------------
 * PB_LOCAL_MCH_NAME constant
 *
 *     String constant for the local phonebook missed call history.
 */
#define PB_LOCAL_MCH_NAME       "telecom/mch.vcf"

/*---------------------------------------------------------------------------
 * PB_LOCAL_CCH_NAME constant
 *
 *     String constant for the local phonebook combined call history.
 */
#define PB_LOCAL_CCH_NAME       "telecom/cch.vcf"

/*---------------------------------------------------------------------------
 * PB_LOCAL_SPD_NAME constant
 *
 *     String constant for the local phonebook speed dial.
 */
#define PB_LOCAL_SPD_NAME       "telecom/spd.vcf"

/*---------------------------------------------------------------------------
 * PB_LOCAL_FAV_NAME constant
 *
 *     String constant for the local phonebook favorite number .
 */
#define PB_LOCAL_FAV_NAME       "telecom/fav.vcf"

/*---------------------------------------------------------------------------
 * PB_SIM_STORE_NAME constant
 *
 *     String constant for the SIM phonebook name.
 */
#define PB_SIM_STORE_NAME       "SIM1/telecom/pb.vcf"

/*---------------------------------------------------------------------------
 * PB_SIM_OCH_NAME constant
 *
 *     String constant for the SIM phonebook outgoing call history.
 */
#define PB_SIM_OCH_NAME         "SIM1/telecom/och.vcf"

/*---------------------------------------------------------------------------
 * PB_SIM_ICH_NAME constant
 *
 *     String constant for the SIM phonebook incoming call history.
 */
#define PB_SIM_ICH_NAME         "SIM1/telecom/ich.vcf"

/*---------------------------------------------------------------------------
 * PB_SIM_MCH_NAME constant
 *
 *     String constant for the SIM phonebook missed call history.
 */
#define PB_SIM_MCH_NAME         "SIM1/telecom/mch.vcf"

/*---------------------------------------------------------------------------
 * PB_SIM_CCH_NAME constant
 *
 *     String constant for the SIM phonebook combined call history.
 */
#define PB_SIM_CCH_NAME         "SIM1/telecom/cch.vcf"

/*---------------------------------------------------------------------------
 * PB_SIM_CCH_NAME constant
 *
 *     String constant for the SIM phonebook combined call history.
 */
#define PB_SIM_SPD_NAME         "SIM1/telecom/spd.vcf"
/*---------------------------------------------------------------------------
 * PB_SIM_CCH_NAME constant
 *
 *     String constant for the SIM phonebook combined call history.
 */
#define PB_SIM_FAV_NAME         "SIM1/telecom/fav.vcf"

/****************************************************************************
 *
 * Section: Types
 *
 ***************************************************************************/

/*---------------------------------------------------------------------------
 * PbapEvent type
 *
 *     Indicates the current event type. All event indications and 
 *     confirmations are passed to a callback function of type 
 *     PbapClientCallback or PbapServerCallback. The "PbapCallbackParms.event" 
 *     field indicates one of the event types below.  The 
 *     "PbapCallbackParms.oper" field indicates the applicable event 
 *     operation.
 */
typedef uint8_t PbapEvent;

/* A transport layer connection has been established. There is no
 * operation associated with this event.
 */
#define PBAP_EVENT_TP_CONNECTED         0x01

/* The transport layer connection has been disconnected. There is no
 * operation associated with this event.
 */
#define PBAP_EVENT_TP_DISCONNECTED      0x02

/* Indicates that a phonebook operation (see PbapOp) has 
 * completed successfully.
 */
#define PBAP_EVENT_COMPLETE             0x03

/* Indicates that the current operation has failed or was rejected
 * by the remote device.
 */
#define PBAP_EVENT_ABORTED              0x04

/* Delivered to the application when it is time to issue
 * another request or response. The application must call either
 * PBAP_ServerContinue() or PBAP_ClientContinue().
 */
#define PBAP_EVENT_CONTINUE             0x05

/* Delivered to the application when the Application Parameters
 * header information has been fully parsed.
 */
#define PBAP_EVENT_PARAMS_RX            0x06

/* Informs the client application of data that has been received. The
 * data length and data buffer pointer that are provided must be handled
 * during this callback, as the memory is only good until this callback
 * returns (Client only). 
 */
#define PBAP_EVENT_DATA_IND             0x0c

/*---------------------------------------------------------------------------
 * PbapOp type
 *
 *     Indicates the operation type of the current event. Each event 
 *     indication has an associated operation passed to a callback function 
 *     of type PbapClientCallback or PbapServerCallback. The 
 *     "PbapCallbackParms.oper" field will indicate one of the operation 
 *     types below.  Since the Pull Phonebook, Pull Vcard Listing, and Pull
 *     Vcard Entry operations cannot be known until the actual OBEX headers
 *     are processed, the initial operation start event will indicate merely
 *     that a generic Pull operation is occurring, until further information
 *     arrives.  The exact operation will be known either during the 
 *     PBAP_EVENT_PARAMS_RX or PBAP_EVENT_OBJECT_LENGTH_REQ events
 */
typedef uint32_t PbapOp;

#define PBAPOP_NONE                0x0000 /* No current operation */
#define PBAPOP_CONNECT             0x0001 /* Connect operation */
#define PBAPOP_DISCONNECT          0x0002 /* Disconnect operation */
#define PBAPOP_SET_PHONEBOOK       0x0004 /* Set Phonebook operation */
#define PBAPOP_PULL                0x0008 /* Generic Pull operation  */
#define PBAPOP_PULL_PHONEBOOK      0x0010 /* Pull Phonebook operation */
#define PBAPOP_PULL_VCARD_LISTING  0x0020 /* Pull vCard listing operation */
#define PBAPOP_PULL_VCARD_ENTRY    0x0040 /* Pull vCard entry operation */

/* End of PbapOp */

/*---------------------------------------------------------------------------
 * PbapRespCode type
 *
 *     Response codes used to signify the status of a PBAP operation. They
 *     are included within OBEX Response packets, which are sent out to other 
 *     devices.
 *
 *     The first two response codes listed below are success responses; the 
 *     remainder are considered failures. The failure codes are frequently 
 *     associated with aborted operations.
 */
typedef uint8_t PbapRespCode;

/* Group: Successful response codes */

#define PBRC_CONTINUE              0x10 /* Continue */
#define PBRC_STATUS_SUCCESS        0x20 /* Success */

/* Group: Failure response codes */

#define PBRC_BAD_REQUEST           0x40 /* Bad Request */
#define PBRC_UNAUTHORIZED          0x41 /* Unauthorized */
#define PBRC_FORBIDDEN             0x43 /* Forbidden - operation is understood */
#define PBRC_NOT_FOUND             0x44 /* Not Found */
#define PBRC_NOT_ACCEPTABLE        0x46 /* Not Acceptable */
#define PBRC_PRECONDITION_FAILED   0x4c /* Precondition Failed */
#define PBRC_NOT_IMPLEMENTED       0x51 /* Not Implemented */
#define PBRC_SERVICE_UNAVAILABLE   0x53 /* Service Unavailable */
#define PBRC_LINK_DISCONNECT       0x80 /* Transport connection has been disconnected. */

/* End of PbapRespCode */

/*---------------------------------------------------------------------------
 * PbapVcardFilterBit type
 * 
 *     Describes the bit location pertaining to each filter value in 
 *     the 64-bit vCard filter.
 */
typedef uint8_t PbapVcardFilterBit;

#define VCARD_FILTER_VER       0    /* Version (Bit 0) */
#define VCARD_FILTER_FN        1    /* Formatted Name (Bit 1) */
#define VCARD_FILTER_N         2    /* Structured Presentation of Name (Bit 2) */
#define VCARD_FILTER_PHOTO     3    /* Associated Image or Photo (Bit 3) */
#define VCARD_FILTER_BDAY      4    /* Birthday (Bit 4) */
#define VCARD_FILTER_ADR       5    /* Delivery Address (Bit 5) */
#define VCARD_FILTER_LABEL     6    /* Delivery (Bit 6) */
#define VCARD_FILTER_TEL       7    /* Telephone (Bit 7) */
#define VCARD_FILTER_EMAIL     8    /* Electronic Mail Address (Bit 8) */
#define VCARD_FILTER_MAILER    9    /* Electronic Mail (Bit 9) */
#define VCARD_FILTER_TZ        10   /* Time Zone (Bit 10) */
#define VCARD_FILTER_GEO       11   /* Geographic Position (Bit 11) */
#define VCARD_FILTER_TITLE     12   /* Job (Bit 12) */
#define VCARD_FILTER_ROLE      13   /* Role within the Organization (Bit 13) */
#define VCARD_FILTER_LOGO      14   /* Organization Logo (Bit 14) */
#define VCARD_FILTER_AGENT     15   /* vCard of Person Representing (Bit 15) */
#define VCARD_FILTER_ORG       16   /* Name of Organization (Bit 16) */
#define VCARD_FILTER_NOTE      17   /* Comments (Bit 17) */
#define VCARD_FILTER_REV       18   /* Revision (Bit 18) */
#define VCARD_FILTER_SOUND     19   /* Pronunciation of Name (Bit 19) */
#define VCARD_FILTER_URL       20   /* Uniform Resource Locator (Bit 20) */
#define VCARD_FILTER_UID       21   /* Unique ID (Bit 21) */
#define VCARD_FILTER_KEY       22   /* Public Encryption Key (Bit 22) */
#define VCARD_FILTER_NICK      23   /* Nickname (Bit 23) */
#define VCARD_FILTER_CAT       24   /* Categories (Bit 24) */
#define VCARD_FILTER_PRODID    25   /* Product Id (Bit 25) */
#define VCARD_FILTER_CLASS     26   /* Class Information (Bit 26) */
#define VCARD_FILTER_SORT_STR  27   /* Sort string (Bit 27) */
#define VCARD_FILTER_TIMESTAMP 28   /* Time stamp (Bit 28) */
/* Bits 29-38 Reserved for future use */
#define VCARD_FILTER_PROP      39   /* Use of a proprietary filter (Bit 39) */ 
/* Bits 40-63 Reserved for proprietary filter usage */

/* End of PbapVcardFilterBit */

/*---------------------------------------------------------------------------
 * PbapVcardFormat type
 * 
 *     Describes the 1-byte vCard format value sent in the Application 
 *     Parameters OBEX header from the Phonebook Access client to the 
 *     server to dictate which vCard format it wishes to be returned for 
 *     each vCard object. This format is used for both the Pull Phonebook 
 *     and Pull Phonebook Entry operations.
 */
typedef uint8_t PbapVcardFormat;

#define VCARD_FORMAT_21        0x00       /* Version 2.1 format */
#define VCARD_FORMAT_30        0x01       /* Version 3.0 format */

/* End of PbapVcardFormat */

/*---------------------------------------------------------------------------
 * PbapVcardSortOrder type
 * 
 *     Describes the 1-byte vCard sorting order value sent in the Application 
 *     Parameters OBEX header from the Phonebook Access client to the 
 *     server to dictate the ordering of the vCard entries returned in the 
 *     vCard listing. This format is used for the Pull Vcard Listing operation.
 */
typedef uint8_t PbapVcardSortOrder;

#define VCARD_SORT_ORDER_INDEXED        0x00       /* Indexed sorting */
#define VCARD_SORT_ORDER_ALPHA          0x01       /* Alphabetical sorting */
#define VCARD_SORT_ORDER_PHONETICAL     0x02       /* Phonetical sorting */

/* End of PbapVcardSortOrder */

/*---------------------------------------------------------------------------
 * PbapVcardSearchAttribute type
 *
 *      Describes the 1-byte vCard search attribute value sent in the
 *      Application Parameters OBEX header from the Phonebook Access client
 *      to the server to dictate the type of search to be performed on
 *      the vCard entries on the Phonebook Access server.  This format is
 *      used for the Pull Vcard Listing operation.
 */
typedef uint8_t PbapVcardSearchAttribute;

#define VCARD_SEARCH_ATTRIB_NAME        0x00        /* Search by Name */
#define VCARD_SEARCH_ATTRIB_NUMBER      0x01        /* Search by Number */
#define VCARD_SEARCH_ATTRIB_SOUND       0x02        /* Search by Sound */

/* End of PbapVcardSearchAttribute */

/*---------------------------------------------------------------------------
 * PbapAppParmsTag type
 *
 *     Describes the tag values used in the Application Parameters OBEX header
 *     that are used on both the Phonebook Access client and server side.
 */
typedef uint8_t PbapAppParmsTag;

#define PBAP_TAG_ORDER              0x01  /* 1-byte, 0x00 (indexed), 0x01 (alpha), or 0x02 (phonetic) */
#define PBAP_TAG_SEARCH_VALUE       0x02  /* Variable length text string */
#define PBAP_TAG_SEARCH_ATTRIB      0x03  /* 1-byte, 0x00 (Name), 0x01 (Number), or 0x02 (Sound) */
#define PBAP_TAG_MAX_LIST_COUNT     0x04  /* 2-bytes, 0x0000 to 0xFFFF */
#define PBAP_TAG_LIST_OFFSET        0x05  /* 2-bytes, 0x0000 to 0xFFFF */
#define PBAP_TAG_FILTER             0x06  /* 8-bytes, 64 bit mask */
#define PBAP_TAG_FORMAT             0x07  /* 1-byte, 0x00 = 2.1, 0x01 = 3.0 */
#define PBAP_TAG_PHONEBOOK_SIZE     0x08  /* 2-bytes, 0x0000 to 0xFFFF */
#define PBAP_TAG_MISSED_CALLS       0x09  /* 1-byte, 0x00 to 0xFF */

/* End of PbapAppParmsTag */

/*---------------------------------------------------------------------------
 * PbapSetPbFlags type
 *
 *     Flags used in the SetFolder operation.  PBAP specification requires
 *     that the PBAP_SETPB_DONT_CREATE flag is always set.
 */
typedef uint8_t PbapSetPbFlags;

#define PBAP_SETPB_NONE           0x00    /* No flags */
#define PBAP_SETPB_BACKUP         0x01    /* Back up one level. */
#define PBAP_SETPB_DONT_CREATE    0x02    /* Don't create folder if it doesn't exist. */

/* End of PbapSetPbFlags */

/*---------------------------------------------------------------------------
 * PbapSetPbParamFlags type
 *
 *     Flags used in the PbapSetPbParams structure.
 */
typedef uint8_t PbapSetPbParamFlags;

#define PBAP_PARAM_NONE             0x00    /* No flags */
#define PBAP_PARAM_PB_SIZE          0x01    /* Phonebook Size. */
#define PBAP_PARAM_NEW_MISSED_CALLS 0x02    /* New Missed Calls. */

/* End of PbapSetPbParamFlags */

/* Forward reference to PbapClientCallbackParms defined below */
typedef struct _PbapClientCallbackParms PbapClientCallbackParms;

/*---------------------------------------------------------------------------
 * PbapClientCallback type
 *
 *  Phonebook Access Client events along with data indications are 
 *  passed to the application through a callback function of this type 
 *  defined by the application.
 */
typedef void (*PbapClientCallback)(PbapClientCallbackParms *parms);

/* End of PbapClientCallback */

/*---------------------------------------------------------------------------
 * PbapVcardFilter structure
 * 
 *  Describes the 64-bit filter value sent in the Application Parameters
 *  OBEX header from the Phonebook Access client to the server to dictate
 *  which fields it wishes to be returned for each vCard object.  This
 *  filter is used for both the Pull Phonebook and Pull Phonebook Entry 
 *  operations.
 */
typedef struct _PbapVcardFilter 
{
    /* Array of 8 bytes for this 64-bit filter value */
    uint8_t                  byte[PBAP_FILTER_SIZE];
} PbapVcardFilter;

/*----------------------------------------------------------------------
 * PbapClientSession structure
 *
 *  Maintains Phonebook Access client info and status during an 
 *  application session.
 */
 typedef struct _PbapClientSession 
 {
    /* Bluetooth security level - set by the application 
     * prior to PBAP_RegisterClient.  An application must have a
     * pairing handler registered in order to receive PIN requests.
     * This can be done through the BlueMgr API or done directly
     * by the application.
     */
    BtSecurityLevel     secLevel;

    /* GOEP Client instance */
    GoepClientApp       cApp;       

    /* === Internal use only === */
    PbapOp              currOp;
    uint8_t             flags;
    uint8_t             appParms[PBAP_MAX_APP_PARMS_LEN];
    SdpRecord           record;
    SdpAttribute        attributes[5];
    
    /* Registration status for this PBAP Client instance */
    int                registered;
    /* GOEP Connect information */
    GoepConnectReq      connect;
    /* GOEP Pull information */
    GoepObjectReq       pull;

} PbapClientSession;
 
/*---------------------------------------------------------------------------
 * PbapPullPbParms structure
 *
 *  Describes the parameters required to issue a Pull Phonebook operation.
 *  These parameters must be filled in by the client application prior to 
 *  calling PBAP_PullPhonebook.
 */
typedef struct _PbapPullPbParms
{
    /* Full path information including the phonebook name (null-terminated) 
     * (e.g. "telecom\pb.vcf") 
     */
    const uint8_t  *pbName;
    /* List of the vCard fields the client wishes the server to return for
     * each phonebook entry.
     */
    PbapVcardFilter     filter;           
    /* Format of vCard (version 2.1 or version 3.0) */
    PbapVcardFormat     format;
    /* Maximum number of vCard entries supported by the client */
    uint16_t                 maxListCount;
    /* Offset of the first vCard entry */
    uint16_t                 listStartOffset; 
} PbapPullPbParms;

/*---------------------------------------------------------------------------
 * PbapPullVcardListingParms structure
 *
 *  Describes the parameters required to issue a Pull VCard Listing operation.
 *  These parameters must be filled in by the client application prior to 
 *  calling PBAP_PullVcardListing.
 */
typedef struct _PbapPullVcardListingParms
{
    /* Subfolder name (null-terminated) - empty string is used to return the
     * current folder listing.
     */
    const uint8_t          *folderName;
    /* Phonebook entries are returned based on the search attribute which
     * can be either Name (0x00), Number (0x01), or Sound (0x02). The name
     * attribute is assumed if no value is provided.
     */
    PbapVcardSearchAttribute    searchAttribute;
    /* Text string that is used to compare against the specified search 
     * attribute for each remote phonebook entry to determine if the entry 
     * will be returned. If this value is not provided, all entries will be 
     * returned.
     */
    const char                  *searchValue;
    /* Indicates the requested sorting method. If no value is provided, 
     * indexed sorting is assumed. Valid sorting methods are 
     * Indexed/Alphabetical/Phonetical 
     */
    PbapVcardSortOrder          order;
    /* Maximum number of vCard entries supported by the client */
    uint16_t                         maxListCount;
    /* Offset of the first vCard entry */
    uint16_t                         listStartOffset;
} PbapPullVcardListingParms;

/*---------------------------------------------------------------------------
 * PbapPullVcardEntryParms structure
 *
 *  Describes the parameters required to issue a Pull VCard Entry operation.
 *  These parameters must be filled in by the client application prior to 
 *  calling PBAP_PullVcardEntry.
 */
typedef struct _PbapPullVcardEntryParms
{
    /* Object name (null-terminated) (e.g. "1.vcf") */
    const uint8_t  *objectName;
    /* List of the vCard fields the client wishes the server to return for
     * each phonebook entry.
     */
    PbapVcardFilter     filter;           
    /* Format of vCard (version 2.1 or version 3.0) */
    PbapVcardFormat     format;
} PbapPullVcardEntryParms;

/*---------------------------------------------------------------------------
 * PbapSetPhonebookParms structure
 *
 *  Describes the parameters required to issue a Set Phonebook operation.
 *  These parameters must be filled in by the client application prior to 
 *  calling PBAP_SetPhonebook.
 */
typedef struct _PbapSetPhonebookParms
{
    /* Name of the folder (null-terminated) */
    const uint8_t  *folderName;

    /* Set Phonebook operation options. PBAP specification requires
     * that the PBAP_SETPB_DONT_CREATE flag is always set. 
     */
    PbapSetPbFlags      flags;

    /* Set this flag to true to reset the path to the root folder. 
     * No path flags may be specified if this is TRUE.  In addition,
     * any name provided will be ignored if this is TRUE, since reset 
     * uses an empty name header.
     */
    int                reset;
} PbapSetPhonebookParms;

/*---------------------------------------------------------------------------
 * PbapClientCallbackParms structure
 *
 *  Describes a callback event and any data that relates to the event. These
 *  callback parameters are used for all Phonebook Access Clients.  
 *  Determination of the valid fields can be done by evaluating which event 
 *  is being indicated as well as which client it is intended for.
 */
struct _PbapClientCallbackParms
{
    PbapEvent               event;          /* PBAP event */
    PbapOp                  oper;           /* PBAP operation */
    PbapClientSession      *client;         /* PBAP client */

    union {
#if OBEX_PROVIDE_SDP_RESULTS == XA_ENABLED
        /* Group: During a PBAP_EVENT_TP_CONNECTED event, contains 
         * the SDP parsed information 
         */
        struct {
            uint16_t                     profileVersion; /* PBAP profile version */
            PbapSupportedPhonebooks suppPhonebooks; /* PBAP supported phonebooks */
        } connect;
#endif /* OBEX_PROVIDE_SDP_RESULTS == XA_ENABLED */

        /* Group: Valid during PBAP_EVENT_TP_DISCONNECTED event */
        PbapRespCode        discReason;     /* PBAP disconnect reason code */

        /* Group: Valid during PBAP_EVENT_ABORTED event */
        PbapRespCode        abortReason;    /* PBAP abort reason code */

#if OBEX_AUTHENTICATION == XA_ENABLED
        /* Group: Valid during PBAP_EVENT_AUTH_RESULT event */
        struct {
            int                    result;         /* Result of the PBAP Authentication attempt */
            PbapAuthFailReason      reason;         /* PBAP Authentication failure reason */
        } auth;
#endif /* OBEX_AUTHENTICATION == XA_ENABLED */

        /* Group: Valid during PBAP_EVENT_PARAMS_RX event - provides Application
         * Parameter header information.  Valid for Pull Phonebook and
         * Pull Vcard Listing operations only.
         */
        struct {
            /* Number of new missed calls */
            uint8_t             newMissedCalls;
            /* Provides the size of the requested phonebook. The client 
             * should set its MaxListCount based on the phonebook size, 
             * if it is nonzero. 
             */
            uint16_t            phonebookSize;
        } paramsRx;

        /* Group: Valid during PBAP_EVENT_DATA_IND event */
        struct {
            /* Object name (null-terminated, ASCII or UTF-16) */
            uint8_t    *name;           /* Name pointer */
            /* Data Indication */
            uint8_t             *buffer;         /* Data pointer */
            uint16_t             len;            /* Length of data */
        } dataInd;
    } u;
};
/* End of PbapClientCallbackParms */


/****************************************************************************
 *
 * Section: Function Reference
 *
 ****************************************************************************/

/*---------------------------------------------------------------------------
 * PBAP_Init()
 *
 *      Initialize the PBAP component.  This must be the first PBAP 
 *      function called by the application layer, or if multiple 
 *      PBAP applications exist, this function should be called
 *      at system startup (see XA_LOAD_LIST in config.h).  GOEP and 
 *      OBEX must also be initialized separately.
 *
 * Returns:
 *     TRUE - Initialization was successful.
 *
 *     FALSE - Initialization failed.
 */
int PBAP_Init(void);
/*---------------------------------------------------------------------------
 * PBAP_RegisterClient()
 *
 *     NOTE: This API is deprecated.  Use PBAP_RegisterClientSec instead.
 *
 *     Registers the Phonebook Access Client with the GOEP multiplexor.
 *     All of the events specified in the PBAP layer are delivered to the
 *     PBAP client.
 *     
 * Parameters:
 *     Client - The client application's registration structure.
 *
 *     Callback - callback function for the Phonebook Access client.
 *
 * Returns:
 *     OB_STATUS_INVALID_PARM - Invalid or in use profile type.
 *
 *     OB_STATUS_SUCCESS - The PBAP Client is initialized.
 *
 *     OB_STATUS_FAILED - Problem initializing the PBAP Client.
 *
 *     OB_STATUS_BUSY - A PBAP client is already registered.
 *     
 */
ObStatus PBAP_RegisterClnt(PbapClientSession *Client, PbapClientCallback Callback,
                           BtSecurityParms *SecParms, int UseOldSecApis);
ObStatus PBAP_RegisterClient(PbapClientSession *Client, 
                             PbapClientCallback Callback);
#define PBAP_RegisterClient(_CLIENT, _CALLBACK) \
        PBAP_RegisterClnt(_CLIENT, _CALLBACK, 0, TRUE);

/*---------------------------------------------------------------------------
 * PBAP_RegisterClientSec()
 *
 *     Registers the Phonebook Access Client with the GOEP multiplexor.
 *     All of the events specified in the PBAP layer are delivered to the
 *     PBAP client.
 *     
 * Parameters:
 *     Client - The client application's registration structure.
 *
 *     Callback - callback function for the Phonebook Access client.
 *
 *     SecParms - Pointer to the Security Parameters (level and PIN length)
 *          used by the PBAP Client.
 *
 * Returns:
 *     OB_STATUS_INVALID_PARM - Invalid or in use profile type.
 *
 *     OB_STATUS_SUCCESS - The PBAP Client is initialized.
 *
 *     OB_STATUS_FAILED - Problem initializing the PBAP Client.
 *
 *     OB_STATUS_BUSY - A PBAP client is already registered.
 *     
 */
ObStatus PBAP_RegisterClientSec(PbapClientSession *Client, 
                                PbapClientCallback Callback,
                                BtSecurityParms *SecParms);
#define PBAP_RegisterClientSec(_CLIENT, _CALLBACK, _PARMS) \
        PBAP_RegisterClnt(_CLIENT, _CALLBACK, _PARMS, FALSE);

#if OBEX_DEINIT_FUNCS == XA_ENABLED
/*---------------------------------------------------------------------------
 * PBAP_DeregisterClient()
 *
 *     Deregisters the Phonebook Access Client from the GOEP multiplexor.
 *     
 * Parameters:
 *     Client - The structure used to register the client.
 *
 * Returns:
 *     OB_STATUS_SUCCESS - The client was deregistered.
 *
 *     OB_STATUS_INVALID_PARM - The client is not registered.
 *
 *     OB_STATUS_BUSY - The client could not be deregistered because
 *          it is currently sending an operation.
 *
 *     OB_STATUS_FAILED - The PBAP client failed to deinitialize.
 *
 */
ObStatus PBAP_DeregisterClient(PbapClientSession *Client);
#endif /* OBEX_DEINIT_FUNCS == XA_ENABLED */

/*---------------------------------------------------------------------------
 * PBAP_ClientConnect()
 *
 *     This function is used by the client to create a transport connection
 *     to the specified device and issue an OBEX Connect Request. If the 
 *     transport connection was issued by the server, this function will only
 *     issue the OBEX Connect Request.
 *
 * Parameters:
 *     Client - The structure used to register the client.
 *     
 *     addr   - The remote adress to which the client
 *         wishes to connect.
 *
 * Returns:
 *     OB_STATUS_PENDING - Connection was successfully started. Completion
 *         will be signaled via a call to the application callback.
 *
 *         If the connection is successful, a PBAP_EVENT_COMPLETE event for
 *         the PBAPOP_CONNECT operation will be signaled.
 *
 *         If the transport connection is successful, but the OBEX Connect
 *         failed, the completion event will be PBAP_EVENT_TP_DISCONNECTED
 *         for the operation PBAP_OPER_CONNECT. At this point the transport
 *         is DISCONNECTED. 
 * 
 *         If the transport connection is unsuccessful, the completion event
 *         will be PBAP_EVENT_TP_DISCONNECTED.
 *
 *     OB_STATUS_SUCCESS - The client is now connected.
 *
 *     OB_STATUS_FAILED - Unable to start the operation because the client
 *         is in the middle of starting up a connection.
 *     
 *     OB_STATUS_BUSY - The client is currently executing an operation.
 *
 */
ObStatus PBAP_ClientConnect(PbapClientSession *Client, BD_ADDR *addr);

/*---------------------------------------------------------------------------
 * PBAP_ClientDisconnect()
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
 *         Completion will be signaled with the PBAP_EVENT_TP_DISCONNECTED 
 *         event to the application callback.
 *
 *     OB_STATUS_BUSY - Operation was not started because
 *         the client is currently executing another operation.
 *
 *     OB_STATUS_NO_CONNECT - Operation was not started because
 *         the client is not connected.
 */
ObStatus PBAP_ClientDisconnect(PbapClientSession *Client);

/*---------------------------------------------------------------------------
 * PBAP_ClientTpDisconnect()
 *
 *     This function is used by the client to initiate a transport 
 *     disconnection. PBAP_ClientDisconnect will cause the transport to be 
 *     brought down automatically, but if the transport must be brought 
 *     down immediately, perhaps due an error condition, this routine may 
 *     be used. 
 *     
 * Parameters:
 *     Client - The client application's registration structure.
 *
 * Returns:
 *     OB_STATUS_PENDING - Operation was successfully started.
 *         Completion will be signaled with the PBAP_EVENT_TP_DISCONNECTED 
 *         event to the application callback.
 *
 *     OB_STATUS_NO_CONNECT - Operation was not started because
 *         the client is not connected.
 */
ObStatus PBAP_ClientTpDisconnect(PbapClientSession *Client);

/*---------------------------------------------------------------------------
 * PBAP_ClientAbort()
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
ObStatus PBAP_ClientAbort(PbapClientSession *Client);

/*---------------------------------------------------------------------------
 * PBAP_PullPhonebook()
 *
 *     Initiates the OBEX "Get" operation to retrieve a phonebook object from
 *     the remote Phonebook Access Server.  
 *
 * Parameters:
 *     Client - The client application's registration structure.
 *
 *     Parms - Parameters to specify the application specific parameters
 *             including the phonebook name.
 *
 * Returns:
 *     OB_STATUS_PENDING - Operation was successfully started. Completion
 *         will be signaled via an event to the application callback.
 *
 *     OB_STATUS_BUSY - Operation was not started because
 *         the client is currently executing another operation.
 *     
 *     OB_STATUS_NO_CONNECT - Operation was not started because
 *         the client is not connected.
 *     
 *     OB_STATUS_PACKET_TOO_SMALL - Operation was not started
 *         because the OBEX packet size was insufficient to transmit
 *         the provided headers.
 *
 *     OB_STATUS_INVALID_HANDLE - The object store handle was invalid.
 */
ObStatus PBAP_PullPhonebook(PbapClientSession *Client, PbapPullPbParms *Parms);

/*---------------------------------------------------------------------------
 * PBAP_PullVcardListing()
 *
 *     Initiates the OBEX "Get" operation to retrieve a vCard folder listing 
 *     from the remote Phonebook Access Server.  
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
 *     OB_STATUS_BUSY - Operation was not started because
 *         the client is currently executing another operation.
 *     
 *     OB_STATUS_NO_CONNECT - Operation was not started because
 *         the client is not connected.
 *     
 *     OB_STATUS_PACKET_TOO_SMALL - Operation was not started
 *         because the OBEX packet size was insufficient to transmit
 *         the provided headers.
 *
 *     OB_STATUS_INVALID_HANDLE - The object store handle was invalid.
 */
ObStatus PBAP_PullVcardListing(PbapClientSession *Client, 
                               PbapPullVcardListingParms *Parms);

/*---------------------------------------------------------------------------
 * PBAP_PullVcardEntry()
 *
 *     Initiates the OBEX "Get" operation to retrieve a vCard entry object 
 *     from the remote Phonebook Access Server.  
 *
 * Parameters:
 *     Client - The client application's registration structure.
 *
 *     Parms - Parameters to specify the application specific parameters
 *             including the object name.
 *
 * Returns:
 *     OB_STATUS_PENDING - Operation was successfully started. Completion
 *         will be signaled via an event to the application callback.
 *
 *     OB_STATUS_BUSY - Operation was not started because
 *         the client is currently executing another operation.
 *     
 *     OB_STATUS_NO_CONNECT - Operation was not started because
 *         the client is not connected.
 *     
 *     OB_STATUS_PACKET_TOO_SMALL - Operation was not started
 *         because the OBEX packet size was insufficient to transmit
 *         the provided headers.
 *
 *     OB_STATUS_INVALID_HANDLE - The object store handle was invalid.
 */
ObStatus PBAP_PullVcardEntry(PbapClientSession *Client, 
                             PbapPullVcardEntryParms *Parms);

/*---------------------------------------------------------------------------
 * PBAP_SetPhonebook()
 *
 *     Performs the OBEX "SetPath" operation to set the path to the current
 *     phonebook.  Afterwards, all operations are based on this phonebook
 *     until this function is called again.
 *     
 * Parameters:
 *     Parms - Parameters to specify the application specific parameters
 *             including the folder name.
 *
 * Returns:
 *     OB_STATUS_PENDING - Operation was successfully started. Completion
 *         will be signaled via an event to the application callback.
 *
 *     OB_STATUS_BUSY - Operation was not started because
 *         the client is currently executing another operation.
 *
 *     OB_STATUS_NO_CONNECT - Operation was not started because
 *         the client is not connected.
 *
 *     OB_STATUS_PACKET_TOO_SMALL - Operation was not started
 *         because the OBEX packet size was insufficient to transmit
 *         the provided headers.
 */
ObStatus PBAP_SetPhonebook(PbapClientSession *Client, 
                           PbapSetPhonebookParms *Parms);

/*---------------------------------------------------------------------------
 * PBAP_ClientContinue()
 *
 *     This function is called by the client in response to a received
 *     PBAP_EVENT_CONTINUE event. It must be called once for every
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
ObStatus PBAP_ClientContinue(PbapClientSession *Client);


/*---------------------------------------------------------------------------
 * PBAP_SetFilterBit()
 *
 *     Sets the appropriate filter bit in the 64-bit vCard filter.
 *
 * Parameters:
 *     Bit - Bit to set in the vCard filter.
 *
 *     Filter - vCard filter structure.
 */
void PBAP_SetFilterBit(PbapVcardFilterBit Bit, PbapVcardFilter *Filter);

#define PBAP_SetFilterBit(_BIT, _FILTER)   do {                 \
        Assert((_FILTER));                                      \
        (_FILTER)->byte[(_BIT)/8] |= (uint8_t)(1 << ((_BIT)%8));     \
        } while (0);

/*---------------------------------------------------------------------------
 * PBAP_ClearFilterBit()
 *
 *     Clears the appropriate filter bit in the 64-bit vCard filter.
 *
 * Parameters:
 *     Bit - Bit to clear in the vCard filter.
 *
 *     Filter - vCard filter structure.
 */
void PBAP_ClearFilterBit(PbapVcardFilterBit Bit, PbapVcardFilter *Filter);

#define PBAP_ClearFilterBit(_BIT, _FILTER)  do {                \
        Assert((_FILTER));                                      \
        (_FILTER)->byte[(_BIT)/8] &= ~(uint8_t)(1 << ((_BIT)%8));    \
        } while (0);

/*---------------------------------------------------------------------------
 * PBAP_ClearAllFilterBits()
 *
 *     Clears all of the filter bits in the 64-bit vCard filter. This should
 *     be done prior to setting any of the filter bits, just to ensure the
 *     memory is properly initialized prior to issuing a Pull Phonebook or 
 *     Pull Vcard Entry operation.
 *
 * Parameters:
 *     Filter - vCard filter structure.
 */
void PBAP_ClearAllFilterBits(PbapVcardFilter *Filter);

#define PBAP_ClearAllFilterBits(_FILTER)  do {                  \
        Assert((_FILTER));                                      \
        OS_MemSet((uint8_t *)(_FILTER)->byte, 0, PBAP_FILTER_SIZE);  \
        } while (0);

/*---------------------------------------------------------------------------
 * PBAP_IsSetFilterBit()
 *
 *     Returns the status of the appropriate filter bit in the 64-bit 
 *     vCard filter.
 *
 * Parameters:
 *     Bit - Bit to check in the vCard filter.
 *
 *     Filter - vCard filter structure.
 *
 * Returns:
 *     TRUE or FALSE
 */
int PBAP_IsSetFilterBit(PbapVcardFilterBit Bit, PbapVcardFilter *Filter);

#define PBAP_IsSetFilterBit(_BIT, _FILTER)                      \
        (Assert((_FILTER)),                                      \
        ((_FILTER)->byte[(_BIT)/8] & (1 << ((_BIT)%8)) ? TRUE : FALSE))
        
#endif