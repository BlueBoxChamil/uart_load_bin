#ifndef __SPP_API_H
#define __SPP_API_H

#include <stdbool.h>

#include "bt_types.h"
#include "rfcomm_api.h"
#include "sdp_api.h"

/*---------------------------------------------------------------------------
 * Serial Port Profile (SPP) layer
 *
 *     The Serial Port Profile (SPP) specification defines procedures
 *     and protocols for Bluetooth devices emulating RS232 or other serial
 *     connections.
 */


/****************************************************************************
 *
 * Section: Configuration Constants
 *
 * The following defines are configuration constants that allow
 * an implementer to include/exclude functionality from SPP.
 *
 ****************************************************************************/
 
/*---------------------------------------------------------------------------
 * SPP_SERVER constant
 *      Configuration constant to enable code for Serial Port Profile
 *      server. If the device is client-only, this should be defined as
 *      XA_DISABLED in overide.h.
 */
#ifndef SPP_SERVER
#define SPP_SERVER      XA_ENABLED
#endif

/*---------------------------------------------------------------------------
 * SPP_CLIENT constant
 *      Configuration constant to enable code for Serial Port Profile
 *      client. If the device is server-only, this should be defined as
 *      XA_DISABLED in overide.h.
 */
#ifndef SPP_CLIENT
#define SPP_CLIENT      XA_ENABLED
#endif


/****************************************************************************
 *
 * Types
 *
 ****************************************************************************/


/*---------------------------------------------------------------------------
 * SppEvent type
 *
 *     The application is notified of various indications and confirmations 
 *     through a callback function.  Depending on the event, different 
 *     elements of the SppCallbackParms "SppCallbackParms.p" union will be 
 *     valid.
 */
typedef uint16_t SppEvent;

/** A connection has been established with a remote device.  
 *
 *  When this callback is received, the "SppCallbackParms.p.remDev" field 
 *  contains a pointer to the remote device context.  
 */
#define SPP_EVENT_REMDEV_CONNECTED    0

/** A connection has been terminated for a remote device.  
 *
 *  When this callback is received, the "SppCallbackParms.p.other" field 
 *  contains a 0.  
 */
#define SPP_EVENT_REMDEV_DISCONNECTED 1

#define SPP_EVENT_SEND_COMPLETE       2

#define SPP_EVENT_DATA_IND            3

/* End of SppEvent */ 


/*---------------------------------------------------------------------------
 * SppPortType type
 * 
 *     Identifies the serial port as a client (outgoing) or server (incoming). 
 */
typedef uint8_t SppPortType;

#define SPP_SERVER_PORT      0x01
#define SPP_CLIENT_PORT      0x02

/* End of SppPortType */

#if SPP_SERVER == XA_ENABLED
/*---------------------------------------------------------------------------
 * SppService structure
 *
 *      Servers need this data type for registering with RFCOMM. A particular
 *      service may have one or more ports available. For example, a device
 *      might provide access to 2 modems. It is providing a single service
 *      (modem) via 2 serial ports.
 */
typedef struct _SppService {
    RfService    service;       /* Required for registering service w/RFCOMM */
    const uint8_t     *name;         /* name of service */
    uint16_t          nameLen;       /* Length of name */
    uint16_t          numPorts;      /* Number of ports in this service */
    SdpRecord    *sdpRecord;    /* Description of the service */
} SppService;

#endif /* SPP_SERVER == XA_ENABLED */

#if SPP_CLIENT == XA_ENABLED
/*---------------------------------------------------------------------------
 * SppClient structure
 *      Contains all fields unique to clients. SppClient is a data type in
 *      device structures (SppDev) for clients.
 */
typedef struct _SppClient {
    /* === Internal use only === */
    BtRemoteDevice      *remDev;
    uint8_t                  serverId;
    SdpQueryToken       sdpToken;
} SppClient;
#endif


typedef struct _SppCallbackParms SppCallbackParms;
typedef struct _SppDev SppDev;

enum spp_type_t{
    SPP_DATA_SBC,
    SPP_DATA_AAC,
    SPP_DATA_RAW,
    SPP_CMD,
};

enum spp_cmd_type_t{
    SPP_CMD_TWS_SLAVE_INF,
    SPP_CMD_BUTTON_EVENT,
    //SPP_CMD_AUDIO_PLAY,
    //SPP_CMD_AUDIO_PAUSE,
    //SPP_CMD_AUDIO_NEXT,
    //SPP_CMD_AUDIO_PREV,
    //SPP_CMD_VOLUME_DOWN,
    //SPP_CMD_VOLUME_UP,
    //SPP_CMD_VOLUME_ADJUST,
    SPP_CMD_POWER_OFF,
};
/*---------------------------------------------------------------------------
 * SppCallbackParms structure
 *
 * A pointer to this structure is sent to the application's callback function 
 * notifying the application of state changes or important events.  
 */
struct _SppCallbackParms {
    SppEvent            event;   /* Event associated with the callback */ 

    BtStatus            status;  /* Status of the callback event       */ 

    /* For certain events, a single member of this union will be valid.  
     * See SppEvent documentation for more information.  
     */
    union {
        void           *other;
        BtRemoteDevice *remDev;
        BtPacket       *pkt;
    } p;
};


/*---------------------------------------------------------------------------
 * SppCallback type
 *
 * A function of this type is called to indicate events to the application.
 */
typedef void (*SppCallback)(SppDev *locDev, SppCallbackParms *Info);

/* End of SppCallback */ 


typedef struct _SppModeStruct {
    BtRemoteDevice *remDev;
    SppCallback     callback;
} SppModeStruct;


/*---------------------------------------------------------------------------
 * SppDev structure
 *      This structure defines an SPP device. A user of SPP must allocate
 *      one SppDev for each serial device. Members should not be accessed
 *      directly.
 */
struct _SppDev {
    /* Pointer to platform-specific data. This pointer is passed to all
     * platform-specific functions (prefaced SPPOS_).
     */
    void            *osDev;

    SppPortType     portType;       /* SPP_SERVER_PORT or SPP_CLIENT_PORT */

    /* Fields specific to clients and servers */
    union {

#if SPP_CLIENT == XA_ENABLED

        SppClient   client;

#endif

#if SPP_SERVER == XA_ENABLED

        SppService  *sppService;

#endif

    } type;

    /* === Internal use only === */

    SppCallback     callback;       /* application callback function */ 

    BtSecurityRecord sppSec;

    /* Server / Client elements */
    uint8_t              state;          /* device state */
    RfChannel       channel;
    CmgrHandler     cmgrHandler;
    RfModemStatus   rModemStatus;   /* remote modem status */
    RfModemStatus   lModemStatus;   /* local modem status */
    RfLineStatus    lineStatus;   
    RfPortSettings  portSettings;

    uint8_t              xonChar;
    uint8_t              xoffChar;
    int16_t             highWater;      /* when rRing.dataLen >= highWater,
                                       flow off rx */
    uint8_t              credit;         /* rx credits outstanding */
    uint8_t              msr;            /* Modem Status Register */
    int16_t             breakLen;
};

/* SPP_Open has not been called or SPP_Closed was last called */
#define DEVICE_STATE_CLOSED         0

/* SPP_Open has been called but the device is not connected. */
#define DEVICE_STATE_DISCONNECTED   1

/* The device is opened and connected to a remote device. */
#define DEVICE_STATE_CONNECTED      2

//bool spp_Init(void);
BtStatus spp_send(SppDev *dev, uint8_t *data, uint16_t datalen);
BtStatus spp_flow_control_set(SppDev *dev, int32_t on);
BtStatus spp_connect(SppDev *dev, BD_ADDR *addr);
BtStatus spp_disconnect(SppDev *dev);
BtStatus spp_register(SppDev *dev, BtSecurityParms *sec_parm, uint8_t role, SppCallback callback);
BtStatus spp_deregister(SppDev *dev);
BtStatus spp_register_v2(SppDev *dev, BtSecurityParms *sec_parm, uint8_t role, SppCallback callback, uint16_t max_frame_size, uint8_t credit);

#endif /* __SPP_API_H */

