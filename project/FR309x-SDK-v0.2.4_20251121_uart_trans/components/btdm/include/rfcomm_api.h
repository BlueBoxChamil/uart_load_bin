#ifndef __RFCOMM_API_H
#define __RFCOMM_API_H

#include "btconfig.h"

#include "me_api.h"
#include "sec_api.h"

typedef struct _RfDeferEvent {
    ListEntry   node;
    void        *func;
    void        *channel;
} RfDeferEvent;

/*---------------------------------------------------------------------------
 * RfChannel structure
 *
 *     Represents a single RFCOMM channel. The RFCOMM user must create
 *     this structure and fill in the "callback", "maxFrameSize", and
 *     "priority" fields prior to calling RF_OpenClientChannel.  Only the
 *     "callback" filed must be set when calling RF_RegisterServerChannel.
 */
typedef struct _RfChannel
{
    ListEntry           node;            /* Used internally by RFCOMM. */

    /* Callback function for channel events */
    void*               callback;     
    
    /* Frame size for the channel. This value must be between
     * RF_MIN_FRAME_SIZE and RF_MAX_FRAME_SIZE.
     */
    uint16_t            maxFrameSize;
    
    /* Priority of the channel. Channels with higher priority have smaller
     * "priority" values. Generally, data on higher priority channels is
     * sent before lower priority channels. The priority must be between
     * RF_DEFAULT_PRIORITY (0) and RF_LOWEST_PRIORITY (63).
     */
    uint8_t             priority;

    /* User context.  In some environments the context of the application
     * during a callback is not known.  In these cases, a pointer to the
     * application's context can be stored in userContext and then
     * retrieved during the callback.
     */
    void               *userContext;

    /* === Internal use only === */
    BtSecurityToken     secToken;
    uint8_t             state;
    uint8_t             muxId;            
    uint8_t             dlcId;            
    uint8_t             dlci;             
    uint16_t            flags;            
    ListEntry           txQueue;          
    EvmTimer            timer;            
    uint8_t             cmdSent;
    uint8_t              rfSignals;
    int16_t             rxCredit;
    uint16_t            txCredit;
    uint8_t             grantedCredit;
    uint8_t             initialRxCredit;
    uint16_t            initialFrameSize;
    RfDeferEvent        deferEvent;
}RfChannel;

typedef uint8_t RfSignals;

/*---------------------------------------------------------------------------
 * RfModemStatus structure
 *
 * Represents the status of V.24 signals. To send signals on a connection,
 * create and fill in this structure, then call RF_SetModemStatus.
 *
 * When the remote device sends these signals, RFCOMM generates an
 * RFEVENT_MODEM_STATUS_IND event. During this event, you can read the
 * status in the RfCallbackParms "ptrs.modemStatus" field.
 */
typedef struct _RfModemStatus
{
    RfSignals signals;  /* Contains all signals that apply to this status
                         * message.
                         */
    uint8_t   breakLen; /* Indicates the length of the break signal in 200 ms
                         * units. If 0, no break signal was sent. Must be
                         * between 0 and 15 (inclusive).
                         */
} RfModemStatus;

typedef uint8_t RfLineStatus;

typedef uint8_t RfBaudRate;
typedef uint8_t RfDataFormat;
typedef uint8_t RfFlowControl;
typedef uint16_t RfPortSettingsMask;

/*---------------------------------------------------------------------------
 * RfPortSettings structure
 *
 *     Represents port settings for an RFCOMM channel. Port settings
 *     are used for RS232 emulation.  They are informational and have 
 *     no real effect on the data stream at the RFCOMM level.  To change 
 *     port settings for a channel, this structure is filled and provided
 *     to RFCOMM using the RF_RequestPortSettings function.
 *
 *     When the remote device changes port settings, RFCOMM provides
 *     this structure along with the RFEVENT_PORT_NEG_IND event. The
 *     RFCOMM user can then review the settings, then accept or reject
 *     them with RF_AcceptPortSettings.
 *
 *     Fields in this structure may be valid or invalid, depending on
 *     the contents of the "parmMask" field. 
 */
typedef struct _RfPortSettings
{
    RfBaudRate    baudRate;    /* Indicates the baud rate */
    RfDataFormat  dataFormat;  /* Contains data bits, stop bits, and
                                * parity settings.
                                */
    RfFlowControl flowControl; /* Indicates port flow control options */
    uint8_t       xonChar;     /* Indicates the XON character */
    uint8_t       xoffChar;    /* Indicates the XOFF character */

    /* Contains a bitmask of settings.
     *
     * When changing port settings with RF_RequestPortSettings, "parmMask"
     * indicates which port settings are being changed.
     *
     * During an RFEVENT_PORT_NEG_IND event, "parmMask" describes which
     * parameters the remote device sent.
     */
    RfPortSettingsMask    parmMask;
    
} RfPortSettings;

typedef struct _RfService
{
    /* Identifies the ID that corresponds to this service. This ID value
     * can be set to 0 before this structure is used if the desired RFCOMM 
     * Server ID not known.  If it is set to 0, then, after the service has been 
     * registered with RFCOMM, it will contain a valid  RFCOMM Server ID.  If
     * the desired RFCOMM Server ID is known, then this ID value can be set to
     * that value before registering with RFCOMM.  In either case, make sure to
     * register this ID with SDP so that remote devices can locate this RFCOMM
     * service. This ID can also be used as the channel in * BtSecurityRecords
     * (for incoming connections only).
     */
    uint8_t serviceId;
} RfService;

#endif
