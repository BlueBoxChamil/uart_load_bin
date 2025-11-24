#ifndef __HFG_API_H_
#define __HFG_API_H_

#include "btconfig.h"
#include "atconfig.h"

#include "bt_types.h"
#include "me_api.h"
#include "sec_api.h"
#include "rfcomm_api.h"
#include "atp_api.h"

/*---------------------------------------------------------------------------
 * Hands-Free Audio Gateway SDK layer
 *
 * This API allows an application to act in the "Hands-Free Audio Gateway" 
 * (AG) role of the Hands-Free Profile.  Devices in this role are 
 * responsible for providing an interface to a telephone network (such as 
 * GSM) under the direct control of a "Hands-Free Unit".  For example, the 
 * AG may be a mobile phone interacting with a Hands-Free Unit installed in 
 * a car.  
 *
 * Within this API, there are two "levels" of links:  service and audio, 
 * each with their own function calls and events.  A service-level 
 * connection allows the Hands-Free unit to send commands to the AG, such as 
 * HFG_SendRing().  An audio-level link allows full-duplex sound to be 
 * exchanged between the Hands-Free unit and the AG.  Using this API, the 
 * service link must be established BEFORE attempting to open an audio 
 * link.  
 */

/****************************************************************************
 *
 * Constants
 *
 ****************************************************************************/
/* Channel States */ 
#define HFG_STATE_CLOSED             0
#define HFG_STATE_CONN_PENDING       1
#define HFG_STATE_CONN_INCOMING      2
#define HFG_STATE_OPEN               3

/*---------------------------------------------------------------------------
 * HFG_FEATURE_THREE_WAY_CALLS constant
 *     This define indicates whether this device supports three way calls.  
 *     This value can be used when defining the capabilities of the Audio 
 *     Gateway application.  See HFG_SDK_FEATURES.  
 */
#define HFG_FEATURE_THREE_WAY_CALLS      0x00000001

/*---------------------------------------------------------------------------
 * HFG_FEATURE_ECHO_NOISE constant
 *     This define indicates whether this device supports echo canceling 
 *     and/or noise reduction capability.  This value can be used when 
 *     defining the capabilities of the Audio Gateway application.  See 
 *     HFG_SDK_FEATURES.  
 */
#define HFG_FEATURE_ECHO_NOISE           0x00000002

/*---------------------------------------------------------------------------
 * HFG_FEATURE_VOICE_RECOGNITION constant
 *     This define indicates whether this device supports the Voice 
 *     Recognition capability.  This value can be used when defining the 
 *     capabilities of the audio gateway application.  See 
 *     HFG_SDK_FEATURES.  
 */
#define HFG_FEATURE_VOICE_RECOGNITION    0x00000004

/*---------------------------------------------------------------------------
 * HFG_FEATURE_RING_TONE constant
 *     This define indicates whether this device supports In-Band Ring 
 *     Tones.  This value can be used when defining the capabilities of the 
 *     Audio Gateway application.  See HFG_SDK_FEATURES.  
 */
#define HFG_FEATURE_RING_TONE            0x00000008

/*---------------------------------------------------------------------------
 * HFG_FEATURE_VOICE_TAG constant
 *     This define indicates whether this device allows a voice tag to be 
 *     attached to a phone number.  This value can be used when defining the 
 *     capabilities of the audio gateway application.  See 
 *     HFG_SDK_FEATURES.  
 */
#define HFG_FEATURE_VOICE_TAG            0x00000010

/*---------------------------------------------------------------------------
 * HFG_FEATURE_REJECT constant
 *     This define indicates whether this device supports call rejection.  
 *     This value can be used when defining the capabilities of the Audio 
 *     Gateway application.  See HFG_SDK_FEATURES.  
 */
#define HFG_FEATURE_REJECT               0x00000020

/*---------------------------------------------------------------------------
 * HFG_FEATURE_ENHANCED_CALL_STATUS constant
 *     This define indicates whether this device supports enhanced call 
 *     status features like call listing and call held indications.  See 
 *     HFG_SDK_FEATURES.  
 */
#define HFG_FEATURE_ENHANCED_CALL_STATUS 0x00000040

/*---------------------------------------------------------------------------
 * HFG_FEATURE_ENHANCED_CALL_CTRL constant
 *     This define indicates whether this device supports enhanced call 
 *     control features like call specifying specific lines to put on hold.  
 *     See HFG_SDK_FEATURES.  
 */
#define HFG_FEATURE_ENHANCED_CALL_CTRL   0x00000080

/*---------------------------------------------------------------------------
 * HFG_FEATURE_EXTENDED_ERRORS constant
 *     This define indicates whether this device supports sending extended 
 *     error codes.  See HFG_SDK_FEATURES.  
 */
#define HFG_FEATURE_EXTENDED_ERRORS      0x00000100

/*---------------------------------------------------------------------------
 * HFG_FEATURE_CODEC_NEGOTIATON constant
 *     This define indicates whether this device supports codec negotiation. 
 *     See HFG_SDK_FEATURES.  
 */
#define HFG_FEATURE_CODEC_NEGOTIATON     0x00000200

/*---------------------------------------------------------------------------
 * HFG_SDK_FEATURES constant
 *     This define indicates which Hands Free Audio Gateway features are 
 *     supported by the application and device.  Features that are supported 
 *     by the application must be advertised to the Hands-Free Unit, so that 
 *     it knows the capabilities of the Audio Gateway.  If this value needs 
 *     to be changed, a modified definition can be placed in overide.h with 
 *     the features supported by the Audio Gateway application.  
 */
#if 0
#define HFG_SDK_FEATURES  (HFG_FEATURE_THREE_WAY_CALLS      | \
                           HFG_FEATURE_ECHO_NOISE           | \
                           HFG_FEATURE_VOICE_RECOGNITION    | \
                           HFG_FEATURE_RING_TONE            | \
                           HFG_FEATURE_VOICE_TAG            | \
                           HFG_FEATURE_REJECT               | \
                           HFG_FEATURE_ENHANCED_CALL_STATUS | \
                           HFG_FEATURE_ENHANCED_CALL_CTRL   | \
                           HFG_FEATURE_EXTENDED_ERRORS)
#else
#define HFG_SDK_FEATURES   (HFG_FEATURE_ECHO_NOISE           | \
                            HFG_FEATURE_CODEC_NEGOTIATON     | \
                            HFG_FEATURE_REJECT)
#endif

/*---------------------------------------------------------------------------
 * HFG_TX_BUFFER_SIZE constant
 *     HFG_TX_BUFFER_SIZE defines the maximum size of AT response data that 
 *     can be transmitted.  The default is large enough to handle all AT 
 *     responses supported by the Hands-Free SDK.  If raw AT commands are 
 *     sent that are larger than the default value, then this number must be 
 *     increased.  
 */
#ifndef HFG_TX_BUFFER_SIZE
#define HFG_TX_BUFFER_SIZE       256
#endif

/*---------------------------------------------------------------------------
 * HFG_RECV_BUFFER_SIZE constant
 *     HFG_RECV_BUFFER_SIZE defines the maximum size of AT command data that 
 *     can be received.  The default is large enough to handle all AT 
 *     commands supported by the Hands-Free profile.  
 */
#ifndef HFG_RECV_BUFFER_SIZE
#define HFG_RECV_BUFFER_SIZE     256
#endif

/*---------------------------------------------------------------------------
 * HFG_USE_RESP_HOLD constant
 *     HFG_USE_RESP_HOLD enables the use of the Response and Hold feature, 
 *     which occurs in the Japanese market.  
 */
#ifndef HFG_USE_RESP_HOLD
#define HFG_USE_RESP_HOLD XA_DISABLED
#endif

/*---------------------------------------------------------------------------
 * HFG_USE_IIA constant
 *     HFG_USE_IIA enables the use of the Individual Indicators Activation 
 *     (IIA) feature.  This allows the reporting for certain indicators such 
 *     as signal strength to be turned off to save battery life.  This 
 *     feature is enabled by default, but it can instead be defined as 
 *     XA_DISABLED in the overide.h file.  
 */
#ifndef HFG_USE_IIA
#define HFG_USE_IIA XA_DISABLED
#endif
/*---------------------------------------------------------------------------
 * HFG_II_DESC_MAX constant
 *     HFG_II_DESC_MAX defines the number of characters that can occur in an 
 *     individual indicator description string.  This value could probably 
 *     be as small as INDICATOR_STAGE_LEN, but it is defined as a larger 
 *     value just to be safe.  
 */
#ifndef HFG_II_DESC_MAX
#define HFG_II_DESC_MAX 128
#endif

/****************************************************************************
 *
 * Types
 *
 ****************************************************************************/

/*---------------------------------------------------------------------------
 * HfgEvent type
 *
 *     All indications and confirmations are sent through a callback 
 *     function.  Depending on the event, different elements of 
 *     HfgCallbackInfo "parms" union will be valid as described below.  
 */
typedef uint8_t HfgEvent;

/** An incoming service level connection is being established.  This happens 
 *  when the hands-free device establishes the service connection.  
 *  The data connection is not available yet for sending to the hands-free 
 *  device.  When the HFG_EVENT_SERVICE_CONNECTED event is received, the 
 *  channel is available for sending.  
 *
 *  When this callback is received, the "HfgCallbackParms.p.remDev" field 
 *  contains a pointer to the remote device context.  
 */
#define HFG_EVENT_SERVICE_CONNECT_REQ        0

/** A service level connection has been established.  This can happen as the 
 *  result of a call to HFG_CreateServiceLink, or if the hands-free device 
 *  establishes the service connection.  When this event has been received, 
 *  a data connection is available for sending to the hands-free device.  
 *
 *  This event can also occur when an attempt to create a service level 
 *  connection (HFG_CreateServiceLink) fails.  
 *
 *  When this callback is received, the "HfgCallbackParms.p.remDev" field 
 *  contains a pointer to the remote device context.  In addition, the 
 *  "HfgCallbackParms.errCode" fields contains the reason for disconnect.  
 */
#define HFG_EVENT_SERVICE_CONNECTED          1

/** The service level connection has been released.  This can happen 
 *  as the result of a call to HFG_DisconnectServiceLink, or if the 
 *  hands-free device releases the service connection.  Communication 
 *  with the hands-free device is no longer possible.  In order to 
 *  communicate with the hands-free device, a new service level 
 *  connection must be established.  
 *
 *  This event can also occur when an attempt to create a service level 
 *  connection (HFG_CreateServiceLink) fails.  
 *
 *  When this callback is received, the "HfgCallbackParms.p.remDev" 
 *  field contains a pointer to the remote device context.  If the 
 *  "HfgCallbackParms.p.remDev" field contains a NULL, then the ACL 
 *  connection attempt failed and a remote device was never established.  
 *  In addition, the "HfgCallbackParms.errCode" fields contains the 
 *  reason for the disconnect.  
 */
#define HFG_EVENT_SERVICE_DISCONNECTED       2

/** An audio connection process has terminated, either successfully or 
 *  unsuccessfully.  On success, this event occurs whenever the audio 
 *  channel (SCO) comes up, whether it is initiated by the audio gateway or 
 *  the hands-free unit.  
 *
 *  When this callback is received, the "HfgCallbackParms.p.status" field 
 *  will contain BT_STATUS_SUCCESS if the connection was successfully 
 *  established, or it will otherwise contain a failure status.   The 
 *  "HfgCallbackParms.p.remDev" field contains a pointer to the remote 
 * device context.  
 */
#define HFG_EVENT_AUDIO_CONNECTED            3

/** An audio connection has been released.  This event occurs whenever the 
 *  audio channel (SCO) goes down, whether it is terminated by the audio 
 *  gateway or the hands-free unit.  
 *
 *  When this callback is received, the "HfgCallbackParms.p.remDev" field 
 *  contains a pointer to the remote device context.  In addition, the 
 *  "HfgCallbackParms.errCode" fields contains the reason for disconnect.  
 */
#define HFG_EVENT_AUDIO_DISCONNECTED         4
 
/** After the service level connection has been established, this event will 
 *  indicate the features supported on the hands-free unit.  
 *
 *  When this callback is received, the "HfgCallbackParms.p.features" field 
 *  contains the features (see HfgFeatures).  
 */
#define HFG_EVENT_HANDSFREE_FEATURES         5

/** Only valid if BT_SCO_HCI_DATA is set to XA_ENABLED.  Audio data has been 
 *  received from the remote device.  The data is only valid during the 
 *  callback.  
 *
 *  When this callback is received, the "HfCallbackParms.p.audioData" field 
 *  contains the audio data.  
 */
#define HFG_EVENT_AUDIO_DATA                 6

/** Only valid if BT_SCO_HCI_DATA is set to XA_ENABLED.  Audio data has been 
 *  sent to the remote device.  This event is received by the application 
 *  when the data sent by HFG_SendAudioData has been successfully sent.  
 *
 *  When this callback is received, the "HfCallbackParms.p.audioPacket" 
 *  field contains the result.  
 */
#define HFG_EVENT_AUDIO_DATA_SENT            7

/** The hands-free unit has requested the audio gateway to answer the 
 *  incoming call.  When the call has been answered, the application should 
 *  call HFG_SendOK().  In addition, the appropriate indicators should be 
 *  updated.  
 */
#define HFG_EVENT_ANSWER_CALL                9

/** The hands-free unit has requested the audio gateway to place a call. The 
 *  application should dial the number and respond with a call to 
 *  HFG_SendOK().  In addition, the appropriate indicators should be updated 
 *  (see HFG_SetIndicatorValue()).  
 *
 *  When this callback is received, the "HfgCallbackParms.p.phoneNumber" 
 *  parameter is valid for this event.  
 */
#define HFG_EVENT_DIAL_NUMBER               10

/** The hands-free unit has requested the audio gateway to place a call to 
 *  the phone number associated with the given memory location.  The 
 *  application should dial the number and respond with a call to 
 *  HFG_SendOK().  In addition, the appropriate indicators should be updated 
 *  (see HFG_SetIndicatorValue()).  
 *
 *  When this callback is received, the "HfgCallbackParms.p.memory" 
 *  parameter is valid for this event.  
 */
#define HFG_EVENT_MEMORY_DIAL               11

/** The hands-free unit has requested the audio gateway to place a call to 
 *  the last number dialed.  The application should dial the last number 
 *  dialed and respond with a call to HFG_SendOK().  In addition, the 
 *  appropriate indicators should be updated (see HFG_SetIndicatorValue()).  
 */
#define HFG_EVENT_REDIAL                    12

/** The hands-free unit has requested the audio gateway to place the current 
 *  call on hold.  The application should make the appropriate hold action 
 *  and respond with a call to HFG_SendOK().  In addition, the appropriate 
 *  indicators should be updated (see HFG_SetIndicatorValue()).  
 *
 *  When this callback is received, the "HfgCallbackParms.p.hold" parameter 
 *  is valid for this event.  
 */
#define HFG_EVENT_CALL_HOLD                 13

#if HFG_USE_RESP_HOLD == XA_ENABLED

/** This event is only available when HFG_USE_RESP_HOLD is set to 
 *  XA_ENABLED.  The Response and Hold state has been requested from the 
 *  Hands-Free device.  If the audio gateway is in a Response and Hold 
 *  state, then the application should respond by calling 
 *  HFG_SendRespHoldState() with the state set to HFG_RESP_HOLD_HOLD, 
 *  followed by a call to HFG_SendOK(), otherwise the application should 
 *  simply call HFG_SendOK().  
 */
#define HFG_QUERY_RESPONSE_HOLD             14

/** This event is only available when HFG_USE_RESP_HOLD is set to 
 *  XA_ENABLED.  A Response and Hold request has been received from the 
 *  Hands-Free device.  The audio gateway should take the appropriate action 
 *  and respond by calling HFG_SendRespHoldState() with the state set to 
 *  the requested state.  In addition, the appropriate indicators should be 
 *  updated (see HFG_SetIndicatorValue()).  
 *
 *  When this callback is received, the "HfCallbackParms.p.respHold" field 
 *  contains the result.  
 */
#define HFG_RESPONSE_HOLD                   15

#endif /* HFG_USE_RESP_HOLD == XA_ENABLED */ 

/** The hands-free unit has requested the audio gateway to hang up the 
 *  current call.  The application should hang up the call and respond with 
 *  a call to HFG_SendOK().  In addition, the appropriate indicators should 
 *  be updated (see HFG_SetIndicatorValue()).  
 */
#define HFG_EVENT_HANGUP                    16

/** The list of current calls has been requested from the hands-free 
 *  device.  The audio gateway should respond by calling 
 *  HFG_SendCallListRsp() once for each line supported by the device.  
 */ 
#define HFG_EVENT_LIST_CURRENT_CALLS        17

/** The Hands-Free Unit has requested that Calling Line Identification 
 *  notification be enabled or disabled.  
 *
 *  The "HfgCallbackParms.p.enabled" parameter indicates the type of 
 *  request.  If "enabled" is FALSE, the application may call 
 *  HFG_SendCallerId(), but no indication will be sent to the remote 
 *  device.  
 */
#define HFG_EVENT_ENABLE_CALLER_ID          18

/** The Hands-Free Unit has requested that Call Waiting notification be 
 *  enabled or disabled.  
 *
 *  The "HfgCallbackParms.p.enabled" parameter indicates the type of 
 *  request.  If "enabled" is FALSE, the application may call 
 *  HFG_CallWaiting(), but no indication will be sent to the remote device.  
 */
#define HFG_EVENT_ENABLE_CALL_WAITING       19

/** The Hands-Free Unit has requested that the Audio Gateway transmit a 
 *  specific DTMF code to its network connection.  The "dtmf" parameter 
 *  will contain the requested DTMF code.  
 *
 *  When the tone has been sent to the network, call HFG_SendOk().  
 */
#define HFG_EVENT_GENERATE_DTMF             20

/** The Hands-Free Unit has requested the phone number associated with the 
 *  last voice tag in the Audio Gateway in order to link its own voice tag 
 *  to the number.  The Hands-Free unit may then dial the linked phone 
 *  numbers when a voice tag is recognized.  (This procedure is only 
 *  applicable for Hands-Free Units that support their own voice recognition 
 *  functionality).  The HFG_FEATURE_VOICE_TAG bit must be set in 
 *  HFG_SDK_FEATURES in order to use this event.  
 *
 *  In response to this event, call HFG_VoiceTagRsp() with a number for 
 *  tagging, or call HFG_SendError() to reject the request.  
 */
#define HFG_EVENT_GET_LAST_VOICE_TAG        21

/** This define enables or disables the voice recognition function resident 
 *  in the Audio Gateway (as indicated by the "enabled" parameter).  If the 
 *  HF enables voice recognition, the Audio Gateway must keep the voice 
 *  recognition enabled until either:  
 *
 *      1) The HF disables it.  
 *      2) The service link is disconnected.  
 *      3) The duration of time supported by the Audio Gateway's 
 *         implementation has elapsed.  
 *
 *  In this last case the Audio Gateway must notify the Hands-Free Unit that 
 *  it has disabled voice recognition by calling HFG_DisableVoiceRec().  
 *  The HFG_FEATURE_VOICE_RECOGNITION bit must be set in HFG_SDK_FEATURES in 
 *  order to receive this event.  
 */
#define HFG_EVENT_ENABLE_VOICE_RECOGNITION  22

/** The Hands-Free Unit has requested that the Audio Gateway disable the 
 *  noise reduction and echo canceling (NREC) functions resident in the 
 *  Audio Gateway.  
 *
 *  If the Audio Gateway supports NREC it must disable these features for 
 *  the duration of the service link.  The HFG_FEATURE_ECHO_NOISE bit must be 
 *  set in HFG_SDK_FEATURES in order to receive this event.  
 */
#define HFG_EVENT_DISABLE_NREC              23

/** The Hands-Free Unit has informed the Audio Gateway of its microphone 
 *  gain level.  
 *
 *  The "HfgCallbackParms.p.gain" parameter is valid.  
 */
#define HFG_EVENT_REPORT_MIC_VOLUME         24

/** The Hands-Free Unit has informed the Audio Gateway of its speaker volume 
 *  level.  
 *
 *  The "HfgCallbackParms.p.gain" parameter is valid.  
 */
#define HFG_EVENT_REPORT_SPK_VOLUME         25

/** The Hands-Free Unit has requested the network operator from the 
 *  Audio Gateway.  The Audio Gateway should respond by calling 
 *  HFG_SendNetworkOperatorRsp().  
 */
#define HFG_EVENT_QUERY_NETWORK_OPERATOR    26
 
/** The Hands-Free Unit has requested the subscriber number from the 
 *  Audio Gateway.  The Audio Gateway should respond by calling 
 *  HFG_SendSubscriberNumberRsp().  
 */
#define HFG_EVENT_QUERY_SUBSCRIBER_NUMBER   27

/** The Hands-Free Unit has requested that extended error codes be 
 *  enabled.  When extended errors are enabled, a call to HFG_SendError() 
 *  will send extended errors, otherwise it will only respond with an 
 *  "ERROR" response.  
 */
#define HFG_EVENT_ENABLE_EXTENDED_ERRORS    28

/** An unsupported AT command has been received from the Hands Free Unit.  
 *  This event is received for AT commands that are not handled by the 
 *  internal Hands-free AT parser.  The application must complete the 
 *  response by doing one of two things.  If the application does not 
 *  understand or cannot process the command, then it must invoke 
 *  HFG_SendError().  Otherwise, if the application understands the command 
 *  and needs to process it, the application must make an appropriate 
 *  response and call HFG_SendOK().  
 *
 *  When this callback is received, the "HfgCallbackParms.p.raw.str" field 
 *  contains the raw AT command string, and the "HfgCallbackParms.p.raw.len" 
 *  field indicates the number of characters in the command string.  
 */
#define HFG_EVENT_AT_COMMAND_DATA           29

/** Whenever a response has been set to the remote device, this event is 
 *  received to confirm that the response was sent.  
 * 
 *  When this event is received, the "HfgCallbackParms.p.response" field 
 *  contains a pointer to the response structure that was used to send 
 *  the response.  
 */
#define HFG_EVENT_RESPONSE_COMPLETE         30

/** This event is only available when HFG_USE_IIA is set to XA_ENABLED.  The 
 *  Hands-Free Unit has requested that the individual indicators be 
 *  activated or deactivated via an AT+BIA message.  The Audio Gateway 
 *  profile code will have already modified its reporting states and replied 
 *  with an OK message.  Therefore, this event requires no further action on 
 *  the part of the application except for updating its GUI accordingly.  
 */
#define HFG_EVENT_IIA                       31

/** Supported codecs has been received from the Hands Free Unit.
*/
#define HFG_EVENT_SUPPORTED_CODEC           32

/** Codec connection rsp has been received from the Hands Free Unit.
*/
#define HFG_EVENT_CODEC_CONNECTION_RSP      33
/* End of HfgEvent */ 

/*--------------------------------------------------------------------------
 * HfgCmeError type
 */
typedef uint8_t HfgCmeError;

#define HFG_CME_NO_CONNECTION                 ATCME_NO_CONNECTION
#define HFG_CME_OP_NOT_ALLOWED                ATCME_OP_NOT_ALLOWED
#define HFG_CME_OP_NOT_SUPPORTED              ATCME_OP_NOT_SUPPORTED
#define HFG_CME_PH_SIM_PIN_REQUIRED           ATCME_PH_SIM_PIN_REQUIRED
#define HFG_CME_SIM_NOT_INSERTED              ATCME_SIM_NOT_INSERTED
#define HFG_CME_SIM_PIN_REQUIRED              ATCME_SIM_PIN_REQUIRED
#define HFG_CME_SIM_PUK_REQUIRED              ATCME_SIM_PUK_REQUIRED
#define HFG_CME_SIM_FAILURE                   ATCME_SIM_FAILURE
#define HFG_CME_SIM_BUSY                      ATCME_SIM_BUSY
#define HFG_CME_INCORRECT_PASSWORD            ATCME_INCORRECT_PASSWORD
#define HFG_CME_SIM_PIN2_REQUIRED             ATCME_SIM_PIN2_REQUIRED
#define HFG_CME_SIM_PUK2_REQUIRED             ATCME_SIM_PUK2_REQUIRED
#define HFG_CME_MEMORY_FULL                   ATCME_MEMORY_FULL
#define HFG_CME_INVALID_INDEX                 ATCME_INVALID_INDEX
#define HFG_CME_MEMORY_FAILURE                ATCME_MEMORY_FAILURE
#define HFG_CME_TEXT_STRING_TOO_LONG          ATCME_TEXT_STRING_TOO_LONG
#define HFG_CME_INVALID_CHARS_IN_TEXT_STRING  ATCME_INVALID_CHARS_IN_TEXT_STRING
#define HFG_CME_DIAL_STRING_TOO_LONG          ATCME_DIAL_STRING_TOO_LONG
#define HFG_CME_INVALID_CHARS_IN_DIAL_STRING  ATCME_INVALID_CHARS_IN_DIAL_STRING
#define HFG_CME_NO_NETWORK_SERVICE            ATCME_NO_NETWORK_SERVICE

/** The HFG_CME_NETWORK_NOT_ALLOWED error should be used when 
 *  there is no roaming agreement with the requested network.  
 *  In such cases, only emergency calls are allowed.  
 */
#define HFG_CME_NETWORK_NOT_ALLOWED           ATCME_NETWORK_NOT_ALLOWED

#define HFG_CME_UNKNOWN                       ATCME_UNKNOWN

/* End of HfgCmeError */ 

/*--------------------------------------------------------------------------
 * HfgHandsFreeVersion type
 *
 *     HfgHandsFreeVersion enumerates the possible Hands-Free profile 
 *     versions discovered during the SDP query.  The service connection 
 *     will be limited to the capabilities of this profile version.  
 */
typedef uint16_t HfgHandsFreeVersion;

/** Unable to determine the Hands Free Profile version that is supported 
 */ 
#define HFG_HF_VERSION_UNKNOWN 0x0000

/** Supports Version 0.96 of the Hands Free Profile 
 */ 
#define HFG_HF_VERSION_0_96    0x0100

/** Supports Version 1.0 of the Hands Free Profile 
 */ 
#define HFG_HF_VERSION_1_0     0x0101

/** Supports Version 1.5 of the Hands Free Profile 
 */ 
#define HFG_HF_VERSION_1_5     0x0105

/* End of HfgHandsFreeVersion */ 

/*---------------------------------------------------------------------------
 * HfgHandsFreeFeatures type
 *
 *  HfgHandsFreeFeatures is a bit mask specifying the Hand-Free Unit's 
 *  optional feature set.  
 */
typedef uint32_t HfgHandsFreeFeatures;

/** Echo canceling and/or noise reduction function */ 
#define HFG_HANDSFREE_FEATURE_ECHO_NOISE            0x00000001

/** Call-waiting and 3-way calling */ 
#define HFG_HANDSFREE_FEATURE_CALL_WAITING          0x00000002

/** CLI presentation capability */ 
#define HFG_HANDSFREE_FEATURE_CLI_PRESENTATION      0x00000004

/** Voice recognition function */ 
#define HFG_HANDSFREE_FEATURE_VOICE_RECOGNITION     0x00000008

/** Remote volume control */ 
#define HFG_HANDSFREE_FEATURE_VOLUME_CONTROL        0x00000010

/** Enhanced call status */ 
#define HFG_HANDSFREE_FEATURE_ENHANCED_CALL_STATUS  0x00000020

/** Enhanced call control */ 
#define HFG_HANDSFREE_FEATURE_ENHANCED_CALL_CTRL    0x00000040

/** Codec negotiation */
#define HFG_HANDSFREE_FEATURE_CODEC_NEGOTIATION     0x00000080

/* End of HfgHandsFreeFeatures */ 

#if AT_HEADSET == XA_ENABLED

/*---------------------------------------------------------------------------
 * AtHeadsetResult
 * 
 *     This structure is used to specify the parameters associated with 
 *     Headset results.  Headset results are sent from the Audio Gateway 
 *     to the Headset device.  As such, this structure is used by the Audio 
 *     Gateway encoder, and the Headset device decoder functions.  
 */
typedef union _AtHeadsetResult {

    /* AT_MICROPHONE_GAIN */ 
    struct {
        uint8_t     gain;
    } mic;

    /* AT_SPEAKER_GAIN */ 
    struct {
        uint8_t     gain;
    } speaker;

} AtHeadsetResult;

#else /* AT_HEADSET == XA_ENABLED */ 

/* Stub structures to keep #defines out 
 * of AtResults/AtCommands structures. 
 */ 
typedef uint8_t AtHeadsetResult;

#endif /* AT_HEADSET == XA_ENABLED */ 

#if AT_HANDSFREE == XA_ENABLED

/*---------------------------------------------------------------------------
 * AtHandsfreeResult
 * 
 *     This structure is used to specify the parameters associated with 
 *     Hands-Free results.  Hands-Free results are sent from the Audio 
 *     Gateway to the Hands-Free unit.  As such, this structure is used by 
 *     the Audio Gateway encoder, and the Hands-Free unit decoder 
 *     functions.  
 */
typedef union _AtHandsfreeResult {

    /* AT_CALL_WAIT_NOTIFY */ 
    struct {
        const char     *number;
        uint8_t         classmap;
        uint8_t         type;
    } wait;

    /* AT_CALL_HOLD | AT_TEST */ 
    struct {
        uint8_t         flags;
    } hold_test;

    /* AT_INDICATORS | AT_TEST */ 
    struct {

        /* Number of indicators reported. */ 
        uint8_t         num;

        struct {

            /* Description of indicator */ 
            const char *description;

            /* Range of values supported by indicator (e.g. "0,1").
             * Quotes must not be included in this string.
             */ 
            const char *range;

        } ind[AT_MAX_INDICATORS];

    } ind_test;

    /* AT_INDICATORS | AT_READ */ 
    struct {

        /* Number of indicators reported. */ 
        uint8_t         num;

        struct {

            /* Value of indicator. */ 
            uint8_t     value;

        } ind[AT_MAX_INDICATORS];
    } indicators_read;

    /* AT_CALL_ID */ 
    struct {
        const char     *number;
        uint8_t         type;
    } callId;

    /* AT_INDICATOR_EVENT */ 
    struct {

        /* One based indicator index. */ 
        uint8_t         indicator;

        /* Current value of this indicator. */ 
        uint8_t         value;

    } indEvent;

    /* AT_VOICE_TAG */ 
    struct {
        const char     *number;
    } voiceTag;

    /* AT_VOICE_RECOGNITION */ 
    struct {
        int             enabled;
    } vrec;

    /* AT_SUPPORTED_FEATURES */ 
    struct {
        uint8_t         bitmap;
    } features;

    /* AT_IN_BAND_RING_TONE */ 
    struct {
        int             enabled;
    } inBandRing;

    /* AT_RESPONSE_AND_HOLD */ 
    struct {
        uint8_t         setting;
    } btrh;

    /* AT_SUBSCRIBER_NUM */ 
    struct {

        /* String phone number of format specified by "type". */ 
        const char     *number;

        /* Phone number format */ 
        uint8_t         type;

        /* Service related to the phone number. */ 
        uint8_t         service;

    } subscribeNum;

    /* AT_LIST_CURRENT_CALLS */ 
    struct {

        /* The numbering (starting with 1) of the call given by the sequence 
         * of setting up or receiving the calls (active, held or waiting) as 
         * seen by the served subscriber.  Calls hold their number until 
         * they are released. New calls take the lowest available number.  
         */
        uint8_t         index;

        /* Direction: 0 (Mobile Originated), 1 (Mobile Terminated), 
         * 0xFF (Value not present) 
         */
        uint8_t         dir;

        /* Call State */ 
        uint8_t         state;

        /* Mode: 0=Voice, 1=Data, 2=FAX */ 
        uint8_t         mode;

        /* Multiparty 0=Not Multiparty, 1=Multiparty. */ 
        uint8_t         multiParty;

        /* Phone number */ 
        const char     *number;

        /* Phone number type */ 
        uint8_t         type;

    } currentCalls;

    /* AT_NETWORK_OPERATOR */ 
    struct {

        /* 0 = automatic, 1 = manual, 2 = deregister, 3 = set format only, 
         * 4 = manual/automatic.  
         */
        uint8_t         mode;

        /* Format of "oper" parameter (0-3) */ 
        uint8_t         format;

        /* Numeric or long or short alphanumeric name of operator */ 
        const char     *oper;

    } networkOper_read;

    /* AT_BCS */
    struct {

        /* 1 = CVSD, 2 = mSBC, other =Invalid */
        uint8_t         id;
        
    } codecID;
} AtHandsfreeResult;

#else /* AT_HANDSFREE == XA_ENABLED */ 

/* Stub structures to keep #defines out 
 * of AtResults/AtCommands structures. 
 */ 
typedef uint8_t AtHandsfreeResult;

#endif /* AT_HANDSFREE == XA_ENABLED */ 

#if AT_PHONEBOOK == XA_ENABLED

/*---------------------------------------------------------------------------
 * AtPhonebookResult
 *
 */
typedef union _AtPhonebookResult {

    /* AT_SELECT_PHONEBOOK_STORAGE|AT_READ */ 
    struct {

        /* Phonebook storage currently selected. */ 
        uint16_t            selected;

        /* Number of used locations. */ 
        uint16_t            used;

        /* Total number of locations in the memory. */ 
        uint16_t            total;

    } storage_read;

    /* AT_SELECT_PHONEBOOK_STORAGE|AT_TEST */ 
    struct {

        /* Bitmap of supported storage types. */ 
        uint16_t            supported;

    } storage_test;

    /* AT_READ_PHONEBOOK_ENTRY */ 
    /* AT_FIND_PHONEBOOK_ENTRY */ 
    struct {

        /* Index of this entry. */ 
        uint16_t            index;

        /* Phone number format. */ 
        uint8_t             type;

        /* Phone number. */ 
        const char         *number;

        /* Text associated with phone number.  The character set used with 
         * this parameter is specified by AT_SELECT_CHARACTER_SET command.  
         */
        const char         *text;

    } read, find;

    /* AT_READ_PHONEBOOK_ENTRY|AT_TEST */
    struct {

        /* Location range supported by the current storage (e.g. "0-99").  
         * Quotes must not be included in this string.  
         */
        const char         *range;

        /* The maximum length of the phonebook "number" field. */ 
        uint16_t            numLength;

        /* The maximum length of the phonebook "text" field. */ 
        uint16_t            textLength;

    } read_test;

} AtPhonebookResult;

#else /* AT_PHONEBOOK != XA_ENABLED */ 

/* Stub structures to keep #defines out 
 * of AtResults/AtCommands structures.  
 */
typedef uint8_t AtPhonebookResult;

#endif /* else AT_PHONEBOOK != XA_ENABLED */ 

#if AT_SMS == XA_ENABLED

/*---------------------------------------------------------------------------
 * AtSmsResult
 *
 */
typedef union _AtSmsResult {
    struct {
        uint8_t         type;

        /* Support for receiving (mobile terminated) messages. */ 
        int             recv;

        /* Support for sending (mobile originated) messages. */ 
        int             send;
        
        /* Support for cell broadcast messages.*/ 
        int             bcast;

    } service;

    struct {

        /* Memory from which messages are read and deleted. */ 
        uint16_t        readUsed;
        uint16_t        readTotal;

        /* Memory to which writing and sending operations are made. */ 
        uint16_t        writeUsed;
        uint16_t        writeTotal;
        
        /* Memory to which received SMs are preferred to be stored. */ 
        uint16_t        recvUsed;
        uint16_t        recvTotal;

    } preferred;

    struct {

        /* Memory from which messages are read and deleted. */ 
        uint16_t        read;
        uint16_t        readUsed;
        uint16_t        readTotal;

        /* Memory to which writing and sending operations are made. */ 
        uint16_t        write;
        uint16_t        writeUsed;
        uint16_t        writeTotal;

        /* Memory to which received SMs are preferred to be stored. */ 
        uint16_t        recv;
        uint16_t        recvUsed;
        uint16_t        recvTotal;

    } preferred_read;

} AtSmsResult;

#else /* AT_SMS != XA_ENABLED */ 

/* Stub structures to keep #defines out 
 * of AtResults/AtCommands structures.  
 */ 
typedef uint8_t AtSmsResult;

#endif /* else AT_SMS != XA_ENABLED */ 

#if AT_DUN == XA_ENABLED

/*---------------------------------------------------------------------------
 * 
 *
 */
typedef union _AtDunResult {
    uint8_t      junk;
} AtDunResult;

#else /* AT_DUN == XA_ENABLED */ 

/* Stub structures to keep #defines out of AtResults/AtCommands structures. */
typedef uint8_t AtDunResult;

#endif /* AT_DUN == XA_ENABLED */ 

/*---------------------------------------------------------------------------
 * AtResults structure
 *
 *     This structure is used to specify the parameters associated with 
 *     all supported AT results.  Results, solicited or unsolicited are sent 
 *     by the Mobile Entity (e.g. Gateway) and received on the Terminal 
 *     Entity (e.g. Hands-Free unit).  
 */
typedef struct _AtResults {

    /* This field is intended for use by the structure owner. */ 
    ListEntry           node;

    /* AT_??? type from any specified command group.  The type may include 
     * the AT_READ or AT_TEST flags.  
     */
    uint16_t            type;

    union {
        /* Headset AT result group parameters. */ 
        AtHeadsetResult     hs;

        /* Hands-Free AT result group parameters. */ 
        AtHandsfreeResult   hf;

        /* Phonebook AT command group parameters. */ 
        AtPhonebookResult   pb;

        /* Short Message Service AT command group parameters. */ 
        AtSmsResult         sms;

        /* Dial Up Networking AT command group parameters. */ 
        AtDunResult         dun;
        
        /* Raw data response */ 
        uint8_t                 *data;

        /* 1 byte generic parameter. */ 
        uint8_t                  parm8;

        /* 2 byte generic parameter. */ 
        uint16_t                 parm16;

        /* 4 byte generic parameter. */ 
        uint32_t                 parm32;

        /* BOOL generic parameter. */ 
        int                parmB;

        /* AT_EXTENDED_ERROR */ 
        struct {
            uint8_t         type;
        } error;

    } p;

} AtResults;

/*---------------------------------------------------------------------------
 * HfgResponse type
 *
 *  Structure used to send AT responses.  
 */
typedef AtResults HfgResponse;

/* End of HfgResponse */ 

/*--------------------------------------------------------------------------
 * HfgAtMode type
 *
 *     HfgAtMode enumerates the possible modes for AT result code event 
 *     reporting from the Audio Gateway.  Only mode 3 is covered in the HF 
 *     specification.  (See errata 2855.)  
 */
typedef uint8_t HfgAtMode;

/** Forward unsolicited result codes directly to the Hands-Free unit; 
 *  this is an audio gateway hands-free link specific in-band technique used 
 *  to embed result codes and data when the gateway is in an on-line data 
 *  mode.  
 */
#define HFG_AT_FORWARD_RESULTS          0x03

/* End of HfgAtMode */ 


/*--------------------------------------------------------------------------
 * HfgAtReport type
 *
 *     HfgAtReport enumerates the possible states for AT event reporting 
 *     from the Audio Gateway.  Only modes 0 and 1 are covered in the HF 
 *     specification.  
 */
typedef uint8_t HfgAtReport;

/** No indicator event reporting.  
 */
#define HFG_AT_NO_EVENT_REPORTING       0x00

/** Indicator event reporting using the +CIEV message.  
 */
#define HFG_AT_EVENT_REPORTING          0x01

/* End of HfgAtReport */ 

/*--------------------------------------------------------------------------
 * HfgIndicator type
 *
 *     HfgIndicator enumerates the possible indicator events to be reported 
 *     to the Hands Free Unit via the HFG_SetIndicatorValue() function.  
 */
typedef uint8_t HfgIndicator;

/** Service indicator.  The type associated with this event is a BOOL.  The 
 *  value associated with this event is TRUE if service exists or FALSE 
 *  if no service exists.  
 */
#define HFG_IND_SERVICE       1

/** Call indicator.  The type associated with this event is a BOOL.  The 
 *  value associated with this event is TRUE if a call exists or FALSE 
 *  if no call exists.  
 */
#define HFG_IND_CALL          2

/** Call setup.  The type associated with this event is an HfgCallSetup.  
 */
#define HFG_IND_CALL_SETUP    3

/** Call held.  The type associated with this event is an HfgHoldState.  The 
 *  value associated with this event is as follows:  
 *   0 - no call is held
 *   1 - one call is held and one call is active
 *   2 - either one call is held and one is inactive, or two calls are held
 */
#define HFG_IND_CALL_HELD     4

/** Battery level.  The type associated with this indicator is a uint8_t.  
 *  The value associated with this indicator is a number between 0 and 5.  
 */
#define HFG_IND_BATTERY       5

/** Signal Strength.  The type associated with this indicator is a uint8_t.  
 *  The value associated with this indicator is a number between 0 and 5.  
 */
#define HFG_IND_SIGNAL        6

/** Roaming.  The type associated with this indicator is a BOOL.  
 *  The value associated with this indicator is TRUE when roaming and 
 *  FALSE when not roaming.  
 */
#define HFG_IND_ROAMING       7

/* End of HfgIndicator */ 

/*--------------------------------------------------------------------------
 * HfgRejectFeature type
 *
 *     Used as the reject feature in the SDP record and BRSF command.  
 */
typedef uint8_t HfgRejectFeature;

/** No ability to reject call 
 */
#define HFG_REJECT_NOT_SUPPORTED      0x00

/** Ability to reject supported 
 */
#define HFG_REJECT_SUPPORTED          0x01

/* End of HfgRejectFeature */ 


/*--------------------------------------------------------------------------
 * HfgHoldAction type
 *
 *     HfgHoldAction enumerates the possible hold actions.  
 */
typedef uint8_t HfgHoldAction;

/** Releases all held calls or sets the User Determined User Busy 
 *  (UDUB) indication for a waiting call.  
 */
#define HFG_HOLD_RELEASE_HELD_CALLS      0x00

/** Releases all active calls (if any exist) and accepts the other 
 *  (held or waiting) call.  
 */
#define HFG_HOLD_RELEASE_ACTIVE_CALLS    0x01

/** Places all active calls (if any exist) on hold and accepts the 
 *  other (held or waiting) call.  
 */
#define HFG_HOLD_HOLD_ACTIVE_CALLS       0x02

/** Adds a held call to the conversation.  
 */
#define HFG_HOLD_ADD_HELD_CALL           0x03

/** Connects the two calls and disconnects the Audio Gateway from 
 *  both calls (Explicit Call Transfer).  
 */
#define HFG_HOLD_EXPLICIT_TRANSFER       0x04

/* End of HfgHoldAction */ 

#if HFG_USE_RESP_HOLD == XA_ENABLED

/*---------------------------------------------------------------------------
 * HfgResponseHold type
 *
 * HfgResponseHold enumerates the possible actions or states of calls that 
 * are in the Response and Hold state.  These are only valid when 
 * HFG_USE_RESP_HOLD is set to XA_ENABLED.  
 */
typedef U8 HfgResponseHold;

/** Incoming call is put on hold 
 */ 
#define HFG_RESP_HOLD_HOLD   0

/** Held Incoming call is accepted 
 */ 
#define HFG_RESP_HOLD_ACCEPT 1

/** Held Incoming call is rejected 
 */ 
#define HFG_RESP_HOLD_REJECT 2

/* End of HfgResponseHoldAction */ 

#endif /* HFG_USE_RESP_HOLD == XA_ENABLED */ 

/* Forward References */ 
typedef struct _HfgCallbackParms HfgCallbackParms;
typedef struct _HfgChannel HfgChannel;

/*---------------------------------------------------------------------------
 * HfgCallback type
 *
 * A function of this type is called to indicate Hands-Free events to 
 * the application.  
 */
typedef void (*HfgCallback)(HfgChannel *Chan, HfgCallbackParms *Parms);

/* End of HfgCallback */ 

#if BT_SCO_HCI_DATA == XA_ENABLED

/*---------------------------------------------------------------------------
 * HfgAudioData type
 *
 * This type is only available when BT_SCO_HCI_DATA is set to XA_ENABLED.  
 * It contains audio data received from the remote device.  
 */
typedef CmgrAudioData HfgAudioData;

/* End of HfgAudioData */ 

#endif /* BT_SCO_HCI_DATA == XA_ENABLED */ 

/*---------------------------------------------------------------------------
 * HfgCallStatus type
 *
 * HfgCallStatus enumerates the possible state of a call.  
 */
typedef uint8_t HfgCallStatus;

/** The call is active.  
 */
#define HFG_CALL_STATUS_ACTIVE     0

/** The call is held.  
 */
#define HFG_CALL_STATUS_HELD       1

/** A call is outgoing.  
 */
#define HFG_CALL_STATUS_DIALING    2

/** The remote parting is being alerted.  
 */
#define HFG_CALL_STATUS_ALERTING   3

/** A call is incoming.  
 */
#define HFG_CALL_STATUS_INCOMING   4

/** The call is waiting.  This state occurs only when the audio gateway 
 *  supports 3-Way calling.  
 */
#define HFG_CALL_STATUS_WAITING    5

#if HFG_USE_RESP_HOLD == XA_ENABLED

/** A call is in the response and hold state.  This can be thought of as a 
 *  special muted "active call" state.  This state is not used by most 
 *  networks, but it can occur in Japanese networks.  See the Hands Free 
 *  Specification Errata 2716, which adds this new call state to the CLCC 
 *  messages.  
 */ 
#define HFG_CALL_STATUS_RESPHOLD   6

#endif /* HFG_USE_RESP_HOLD == XA_ENABLED */ 

/** No active call 
 */
#define HFG_CALL_STATUS_NONE       0xFF

/* End of HfgCallStatus */ 

/*---------------------------------------------------------------------------
 * HfgCallMode type
 *
 * HfgCallMode enumerates the possible modes of a call.  
 */
typedef uint8_t HfgCallMode;

/** Voice Call 
 */ 
#define HFG_CALL_MODE_VOICE     0

/** Data Call 
 */ 
#define HFG_CALL_MODE_DATA      1

/** FAX Call 
 */ 
#define HFG_CALL_MODE_FAX       2

/* End of HfgCallMode */ 

/*---------------------------------------------------------------------------
 * HfgCallSetupState type
 *
 * HfgCallSetupState enumerates the possible call setup state indicated by 
 * the Audio Gateway.  
 */
typedef uint8_t HfgCallSetupState;

/** No outgoing or incoming call is present on the audio gateway 
 */ 
#define HFG_CALL_SETUP_NONE     0

/** An incoming call is present on the audio gateway 
 */ 
#define HFG_CALL_SETUP_IN       1

/** An outgoing call is present on the audio gateway 
 */ 
#define HFG_CALL_SETUP_OUT      2

/** An outgoing call is being alerted on the audio gateway 
 */ 
#define HFG_CALL_SETUP_ALERT    3

/* End of HfgCallSetupState */ 

/*---------------------------------------------------------------------------
 * HfgHoldState type
 *
 * HfgHoldState enumerates the possible call hold state indicated by the 
 * Audio Gateway.  
 */
typedef uint8_t HfgHoldState;

/** No held call is present on the audio gateway 
 */ 
#define HFG_HOLD_NO_HLD         0

/** There is one held and one active call present on the audio gateway 
 */ 
#define HFG_HOLD_HLD_ACT        1

/** There is at least one held call and no 
 *  active calls present on the audio gateway 
 */
#define HFG_HOLD_HLD_NO_ACT     2

/* End of HfgHoldState */ 

/*---------------------------------------------------------------------------
 * HfgAudioReason type
 *
 * HfgAudioReason enumerates the possible reasons for establishing or 
 * tearing down an (e)SCO audio link.  
 */
typedef uint8_t HfgAudioReason;

/** A connection has just been made to a remote device and a call is 
 *  ongoing.  
 */ 
#define HFG_AR_CONNECTING_ONGOING_CALL 0

/** An audio handover is occurring.  
 */ 
#define HFG_AR_AUDIO_HANDOVER          1

/** The remote Hands Free Unit has toggled the voice recognition setting.  
 */ 
#define HFG_AR_REMOTE_VOICE_REC        2

/** The local Audio Gateway has toggled the voice recognition setting.  
 */ 
#define HFG_AR_LOCAL_VOICE_REC         3

/** The user has made an audio connection request on the local Audio 
 *  Gateway 
 */ 
#define HFG_AR_LOCAL_USER_ACTION       4

/** The call states have changed requiring a change to the audio 
 *  connection.  
 */ 
#define HFG_AR_CALL_STATE_CHANGE       5

/* End of HfgAudioReason */ 

/*---------------------------------------------------------------------------
 * HfgIIReportingState type
 *
 * This type indicates the reporting state for an individual indicator.  
*/
typedef uint8_t HfgIIReportingState;

/** Actively reporting the individual indicator.  */ 
#define HFG_IIA_ACTIVE   0

/** Reporting for the individual indicator has been deactivated.  */ 
#define HFG_IIA_INACTIVE 1

/** Used for indices that are not applicable because they exceed 
 *  the number of indicators reported by the CIND message.  
 */ 
#define HFG_IIA_NA       2

/* End of HfgIIReportingState */ 


/****************************************************************************
 *
 * Data Structures
 *
 ****************************************************************************/

/*---------------------------------------------------------------------------
 * HfgAtData structure
 *
 *     Structures of type HfgAtData can be used to store raw AT data.  
 */
typedef struct _HfgAtData {
    uint8_t         *rawData;
    uint8_t          dataLen;
} HfgAtData;

/*---------------------------------------------------------------------------
 * HfgCallWaitParms structure
 *
 *     Structures of type HfgCallWaitParms can be used to store waiting call 
 *     identity information.  
 */
typedef struct _HfgCallWaitParms {

    /* Phone number of the waiting call */ 
    const char     *number;

    /* Voice parameters */ 
    uint8_t         classmap;

    /* Type of address */ 
    uint8_t         type;

} HfgCallWaitParms;

/*---------------------------------------------------------------------------
 * HfgCallerIdParms structure
 *
 *     Structures of type HfgCallerIdParms can be used to store calling 
 *     number identification information.  HfgCallerIdParms must correlate 
 *     to the callId structure type defined in AtHandsfreeResult.  
 */
typedef struct _HfgCallerIdParms {

    /* Phone number of the caller */ 
    const char     *number;

    /* Type of address */ 
    uint8_t         type;

} HfgCallerIdParms;

/*---------------------------------------------------------------------------
 * HfgCallListParms structure
 *
 *     Structures of type HfgCallListParms can be used to store information 
 *     about the listed calls on the Audio Gateway.  HfgCallListParms must 
 *     correlate to the currentCalls structure defined in 
 *     AtHandsfreeResult.  
 */
typedef struct _HfgCallListParms {

    /* Index of the call on the audio gateway (1 based) */ 
    uint8_t         index;

    /* 0 - Mobile Originated, 1 = Mobile Terminated */ 
    uint8_t         dir;

    /* Call state (see HfgCallStatus). */ 
    HfgCallStatus   state;

    /* Call mode (see HfgCallMode). */ 
    HfgCallMode     mode;

    /* 0 - Not Multiparty, 1 - Multiparty */ 
    uint8_t         multiParty;

    /* Phone number of the call */ 
    const char     *number;

    /* Type of address */ 
    uint8_t         type;

} HfgCallListParms;

/*---------------------------------------------------------------------------
 * HfgSubscriberNum structure
 *
 *     Structures of type HfgSubscriberNum can be used to store subscriber 
 *     number identification information.  HfgSubscriberNum must correlate 
 *     to the subscribeNum structure type defined in AtHandsfreeResult.  
 */
typedef struct _HfgSubscriberNum {

    /* String phone number of format specified by "type". */ 
    const char     *number;

    /* Phone number format */ 
    uint8_t         type;

    /* Service related to the phone number. */ 
    uint8_t         service;

} HfgSubscriberNum;

/*---------------------------------------------------------------------------
 * HfgNetworkOperator structure
 *
 *     Structures of type HfgNetworkOperator can be used to store network 
 *     operator identification information.  HfgNetworkOperator must 
 *     correlate to the networkOper_read structure type defined in 
 *     AtHandsfreeResult.  
 */
typedef struct _HfgNetworkOperator {

    /* 0 = automatic, 1 = manual, 2 = deregister, 3 = set format only, 
     * 4 = manual/automatic.  
     */ 
    uint8_t     mode;

    /* Format of "oper" parameter (should be set to 0). */ 
    uint8_t     format;

    /* Numeric or long or short alphanumeric name of operator */ 
    const char *oper;

} HfgNetworkOperator;

/*---------------------------------------------------------------------------
 * HfgHold structure
 *
 *     Structures of type HfgHold can be used to describe the hold action 
 *     and optionally the line for that action.  
 */
typedef struct _HfgHold {

    /* Hold action to execute */ 
    HfgHoldAction action;

    /* Index of the call on the audio gateway (1 based).  If 0, the action 
     * does not apply to any particular call.  
     */ 
    uint8_t       index;

} HfgHold;

/*---------------------------------------------------------------------------
 * HfgVoiceTag structure
 *
 *     Structures of type HfgVoiceTag can be used to hold a voice tag 
 *     number.  This must correlate to the voiceTag structure type defined 
 *     in AtHandsfreeResult.  
 */
typedef struct _HfgVoiceTag {
    char *number;
} HfgVoiceTag;

#if HFG_USE_IIA == XA_ENABLED

/*---------------------------------------------------------------------------
 * HfgIndicatorActivity structure
 *
 * Structures of type HfgIndicatorActivity can be used to store the 
 * reporting state for the various individual indicators.  
 */
typedef struct _HfgIndicatorActivity {
    HfgIIReportingState individual[AT_MAX_INDICATORS];
} HfgIndicatorActivity;

#endif /* HFG_USE_IIA == XA_ENABLED */ 

/*---------------------------------------------------------------------------
 * HfgIndDesc structure
 *
 * Structures of type HfgIndDesc can be used to store a description string 
 * for an individual indicator.  
 */
typedef struct _HfgIndDesc {
    char buff[HFG_II_DESC_MAX];
} HfgIndDesc;

/*---------------------------------------------------------------------------
 * HfgChannel structure
 *
 *     Structures of type HfgChannel can be used to store Audio Gateway 
 *     channel information, including information about a connection to the 
 *     Hands-Free Unit.  
 */
struct _HfgChannel {

    /* === Internal use only === */ 

    /* Registration variables */ 
    ListEntry            node;  
    HfgCallback          callback;         /* Application callback 
                                            * function 
                                            */ 
    RfChannel            rfChannel;        /* RFCOMM channel used for audio 
                                            * gateway connection.  
                                            */

    /* Transmit Queue */
    ListEntry            rspQueue;         /* List of AT responses        */ 

    /* Connection State Variables */ 
    uint8_t              state;            /* Current connection state    */ 
    uint16_t             flags;            /* Current connection flags    */ 
    uint8_t              linkFlags;        /* Levels of service connected */ 

    /* HandsFree State Information */ 
    uint8_t              srvcClssIDLst[6]; /* The Service Class ID List 
                                            * parsed from SDP 
                                            */ 
    uint8_t              scilLen;          /* The number of bytes parsed 
                                            * into the Service Class ID List 
                                            * from SDP 
                                            */ 
    HfgHandsFreeVersion  version;          /* Profile version parsed from 
                                            * SDP  
                                            */
    HfgHandsFreeFeatures hfFeatures;       /* Profile features parsed from 
                                            * SDP for the Hands-Free Unit.  
                                            */ 

    /* Indicator status */ 
    int                 indEnabled;       /* Sending of indicators is 
                                            * enabled 
                                            */ 
    int                 service;          /* Service indicator           */ 
    int                 call;             /* The call indicator          */ 
    HfgCallSetupState   callSetup;        /* Call setup indicator        */ 
    HfgHoldState        held;             /* Call held indicator         */ 
    uint8_t             battery;          /* Battery charge indicator    */ 
    uint8_t             signal;           /* Signal strength indicator   */ 
    uint8_t             roaming;          /* Roaming indicator           */ 

    /* Feature State Information */ 
    int                 callWaiting;      /* Call wait notification 
                                            * active 
                                            */ 
    int                 callId;           /* Caller ID notification 
                                            * active 
                                            */ 
    int                 extendedErrors;   /* Extended errors enabled     */ 
    int                 nrecDisable;      /* Noise Red/Echo Cancel 
                                            * disabled 
                                            */ 
    int                 voiceRec;         /* Voice Recogintion active    */ 
    int                 ibRing;           /* In-band ringing active      */ 
    uint8_t                   codecID;           /* codec id                   */

    /* SDP variables for client */ 
    SdpQueryToken       sdpQueryToken;    /* Used to query the service   */ 
    uint8_t             queryFlags;       /* Defines which SDP entries 
                                            * were parsed from the gateway.  
                                            */ 
    uint8_t             rfServerChannel;  /* When connecting AG client   */ 

    /* Channel Resources */ 
    CmgrHandler         cmgrHandler;
    AtResults           atResults;
    AtResults          *currentAtResults;
    AtResults          *lastAtResults;
    XaBufferDesc        atBuffer;
    BtPacket            atTxPacket;
    uint8_t             atTxData[HFG_TX_BUFFER_SIZE];
    uint16_t            bytesToSend;
    uint8_t             atRxBuffer[HFG_RECV_BUFFER_SIZE];
    uint16_t            atRxLen;

#if HFG_USE_IIA == XA_ENABLED

    /* This contains the current individual 
     * indicator active or inactive reporting states.  
     */ 
    HfgIndicatorActivity indicators;

#endif /* HFG_USE_IIA == XA_ENABLED */ 
};

/*---------------------------------------------------------------------------
 * HfgCallbackParms structure
 *
 * Structures of type HfgCallbackParms are sent to the application's 
 * callback function to notify the application of state changes.  
 */
struct _HfgCallbackParms {
    HfgEvent                 hfgEvent;    /* Event associated with the 
                                           * callback
                                           */ 

    BtStatus                 status;      /* Status of the callback event */ 
    BtErrorCode              errCode;     /* Error code (reason) on 
                                           * disconnect events 
                                           */ 

    /* For certain events, a single member of this union will be valid.  
     * See HfgEvent documentation for more information.  
     */ 
    union {
        void                *ptr;
        BtRemoteDevice      *remDev;
        HfgHandsFreeFeatures features;
        int                  enabled;
        const char          *phoneNumber;
        const char          *memory;
        uint8_t              dtmf;
        uint8_t              gain;
        HfgHold             *hold;
        HfgHandsFreeVersion  version;
        HfgResponse         *response;

#if HFG_USE_RESP_HOLD == XA_ENABLED

        HfgResponseHold      respHold;    /* Only valid if 
                                           * HFG_USE_RESP_HOLD is set to 
                                           * XA_ENABLED.  
                                           */ 

#endif /* HFG_USE_RESP_HOLD == XA_ENABLED */ 

#if BT_SCO_HCI_DATA == XA_ENABLED

        HfgAudioData        *audioData;   /* Only valid if 
                                           * BT_SCO_HCI_DATA is set to 
                                           * XA_ENABLED.  
                                           */ 
        BtPacket            *audioPacket; /* Only valid if 
                                           * BT_SCO_HCI_DATA is set to 
                                           * XA_ENABLED.  
                                           */ 

#endif /* BT_SCO_HCI_DATA == XA_ENABLED */ 

        HfgAtData           *data;
    } p;

};

/****************************************************************************
 *
 * Function Reference
 *
 ****************************************************************************/

/*---------------------------------------------------------------------------
 * HFG_Init()
 *
 *     This function initializes the Audio Gateway SDK.  This function 
 *     should only be called once, normally at system initialization time.  
 *     The calling of this function can be specified in overide.h using the 
 *     XA_LOAD_LIST macro.  
 *
 * Returns:
 *     TRUE - Initialization was successful.  
 *
 *     FALSE - Initialization failed.  
 */
int HFG_Init(void);

/*---------------------------------------------------------------------------
 * HFG_Register()
 *
 *     HFG_Register() has been deprecated.  The HFG_RegisterSec() function 
 *     should be used instead.  For the time being, if HFG_Register() is 
 *     used, it is remapped as a macro to the HFG_RegisterSec().  However, 
 *     the HFG_Register() macro will be deleted from future releases of the 
 *     Hands Free profile.  This macro registers and initializes a channel 
 *     for use in creating and receiving service level connections.  It 
 *     registers the Hands-Free profile audio gateway with SDP.  The 
 *     application callback function is also bound to the channel.  This 
 *     call also registers a security record using the values set in 
 *     HFG_SECURITY_SETTINGS.  To set the values in the security settings 
 *     for the security record, it is better to use HFG_RegisterSec() 
 *     instead of HFG_Register(). 
 *
 * Parameters:
 *     Chan - Contains a pointer to the channel structure that will be 
 *         initialized and registered.  
 *
 *     Callback - The application callback function that will receive 
 *         events.  
 * 
 * Returns:
 *     BT_STATUS_SUCCESS - The operation completed successfully.  
 *
 *     BT_STATUS_IN_USE - The operation failed because the channel has 
 *         already been initialized. 
 *
 *     BT_STATUS_FAILED - The operation failed because either the RFCOMM 
 *         channel or the SDP record could not be registered.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter is invalid or not properly 
 *         initialized (XA_ERROR_CHECK only).  
 */
BtStatus HFG_Register(HfgChannel *Chan, HfgCallback Callback);
#define HFG_Register(ch, cb) HFG_RegisterSec(ch, cb, 0)

/*---------------------------------------------------------------------------
 * HFG_RegisterSec()
 *
 *     This function registers and initializes a channel for use in creating 
 *     and receiving service level connections.  Registers the Hands-Free 
 *     profile audio gateway with SDP.  The application callback function is 
 * 	   also bound to the channel.  A security record is registered using the 
 * 	   values set in the secParms parameter.  
 *
 * Parameters:
 *     Chan - Contains a pointer to the channel structure that will be 
 *         initialized and registered.  
 *
 *     Callback - The application callback function that will receive 
 *         events.  
 * 
 * 	   secParms - BtSecurityParms values for registering a security record
 * 	   the profile.
 *
 * Returns:
 *     BT_STATUS_SUCCESS - The operation completed successfully.  
 *
 *     BT_STATUS_IN_USE - The operation failed because the channel has 
 *         already been initialized. 
 *
 *     BT_STATUS_FAILED - The operation failed because either the RFCOMM 
 *         channel or the SDP record could not be registered.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter is invalid or not properly 
 *         initialized (XA_ERROR_CHECK only).  
 */
BtStatus HFG_RegisterSec(HfgChannel      *Chan, 
                         HfgCallback      Callback, 
                         BtSecurityParms *secParms);

/*---------------------------------------------------------------------------
 * HFG_Deregister()
 *
 *     This function deregisters the channel.  The channel becomes unbound 
 *     from RFCOMM and SDP, and can no longer be used for creating service 
 *     level connections.  
 *
 * Parameters:
 *     Chan - Contains a pointer to the channel structure that will be 
 *         deregistered.  
 *
 * Returns:
 *     BT_STATUS_SUCCESS - The operation completed successfully.  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_BUSY - The operation failed because a service level 
 *         connection is still open to the audio gateway.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter is invalid or not properly 
 *         initialized (XA_ERROR_CHECK only).  
 */
BtStatus HFG_Deregister(HfgChannel *Chan);

/*---------------------------------------------------------------------------
 * HFG_CreateServiceLink()
 *
 *     This function creates a service level connection with the Hands-Free 
 *     Unit.  This includes performing SDP Queries to find the appropriate 
 *     service and opening an RFCOMM channel.  The success of the operation 
 *     is indicated by the HFG_EVENT_SERVICE_CONNECTED event.  If the 
 *     connection fails, the application is notified by the 
 *     HFG_EVENT_SERVICE_DISCONNECTED event.  
 * 
 *     If an ACL link does not exist to the audio gateway, one will be 
 *     created first.  If desired, however, the ACL link can be established 
 *     prior to calling this function.  
 *
 * Parameters:
 *
 *     Chan - Pointer to a registered channel structure.  
 *
 *     Addr - The Bluetooth address of the remote device.  
 *
 * Returns:
 *     BT_STATUS_PENDING - The operation has started; the application will 
 *         be notified when the connection has been created (via the 
 *         callback function registered by HFG_Register).
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_BUSY - The operation failed because a connection is already 
 *         open to the remote device, or a new connection is being created.  
 *
 *     BT_STATUS_FAILED - The channel has not been registered.  
 *
 *     BT_STATUS_CONNECTION_FAILED - The connection failed.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter is invalid or not properly 
 *         initialized (XA_ERROR_CHECK only).  
 */
BtStatus HFG_CreateServiceLink(HfgChannel *Chan, BD_ADDR *Addr);

/*---------------------------------------------------------------------------
 * HFG_DisconnectServiceLink()
 *
 *     This function releases the service level connection with the 
 *     Hands-Free Unit.  This will close the RFCOMM channel and will also 
 *     close the SCO and ACL links if no other services are active, and no 
 *     other link handlers are in use (ME_CreateLink).  When the operation 
 *     is complete the application will be notified by the 
 *     HFG_EVENT_SERVICE_DISCONNECTED event.  
 *
 * Parameters:
 *     Chan - Pointer to a registered channel structure.  
 *
 * Returns:
 *     BT_STATUS_PENDING - The operation has started; the application will 
 *         be notified when the service level connection is down (via the 
 *         callback function registered by HFG_Register).  
 *
 *     BT_STATUS_IN_PROGRESS - The operation was previously started; the 
 *         application will be notified when the service level connection is 
 *         down (via the callback function registered by HFG_Register).  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NO_CONNECTION - The operation failed because a service link 
 *         does not exist to the audio gateway.
 *
 *     BT_STATUS_INVALID_PARM - A parameter is invalid or not properly 
 *         initialized (XA_ERROR_CHECK only).  
 */
BtStatus HFG_DisconnectServiceLink(HfgChannel *Chan);

/*---------------------------------------------------------------------------
 * HFG_CreateAudioLink()
 *
 *     This function creates an audio (SCO) link to the Hands-Free Unit.  
 *     The success of the operation is indicated by the 
 *     HFG_EVENT_AUDIO_CONNECTED event when the "HfgCallbackParms.p.status" 
 *     field contains BT_STATUS_SUCCESS.  If the connection fails, the 
 *     application is notified either by the HFG_EVENT_AUDIO_CONNECTED event 
 *     when the "HfgCallbackParms.p.status" field contains something other 
 *     than BT_STATUS_SUCCESS, or by the HFG_EVENT_AUDIO_DISCONNECTED 
 *     event.  
 *
 * Parameters:
 *     Chan - Pointer to a registered channel structure.  
 *
 *     Reason - This parameter must contain the reason why the (e)SCO link 
 *         is being set up.  Depending on the reason and the environment, 
 *         the audio link may be disallowed.  
 *
 * Returns:
 *     BT_STATUS_PENDING - The operation has started; the application will 
 *         be notified when the audio link has been established (via the 
 *         callback function registered by HFG_Register).  
 *
 *     BT_STATUS_SUCCESS - The audio (SCO) link already exists.  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NO_CONNECTION - The operation failed because a service 
 *         level connection does not exist to the audio gateway.  
 *
 *     BT_STATUS_FAILED - An audio connection already exists.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter is invalid or not properly 
 *         initialized (XA_ERROR_CHECK only).  
 *
 *     BT_STATUS_RESTRICTED - The remote Hands-Free device does not support 
 *         Voice Recognition.  
 */
BtStatus HFG_CreateAudioLink(HfgChannel *Chan, HfgAudioReason Reason);

/*---------------------------------------------------------------------------
 * HFG_DisconnectAudioLink()
 *
 *     This function releases the audio connection with the Hands-Free 
 *     Unit.  When the operation is complete, the application will be 
 *     notified by the HFG_EVENT_SERVICE_DISCONNECTED event.  
 *
 * Parameters:
 *     Chan - Pointer to a registered channel structure.  
 *
 *     Reason - This parameter must contain the reason why the (e)SCO link 
 *         is being disconnected.  Depending on the reason and the 
 *         environment, disconnecting the audio link may be disallowed.  
 *
 * Returns:
 *     BT_STATUS_PENDING - The operation has started; the application will 
 *         be notified when the audio connection is down (via the callback 
 *         function registered by HFG_Register).  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NO_CONNECTION - The operation failed because a service link 
 *         does not exist to the audio gateway.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter is invalid or not properly 
 *         initialized (XA_ERROR_CHECK only).  
 *
 *     BT_STATUS_RESTRICTED - The remote Hands-Free device does not support 
 *         Voice Recognition.  
 */
BtStatus HFG_DisconnectAudioLink(HfgChannel *Chan, HfgAudioReason Reason);

#if BT_SCO_HCI_DATA == XA_ENABLED

/*---------------------------------------------------------------------------
 * HFG_SendAudioData()
 *
 *     This function sends the specified audio data on the audio link.  
 *
 * Requires:
 *     BT_SCO_HCI_DATA enabled.  
 *
 * Parameters:
 *     Chan - The Channel over which to send the audio data.  
 *
 *     packet - The packet of data to send.  After this call, the Hands-free 
 *         SDK owns the packet. When the packet has been transmitted 
 *         to the host controller, HFG_EVENT_AUDIO_DATA_SENT is sent to the 
 *         application.  
 *
 * Returns:
 *     BT_STATUS_PENDING - The packet was queued successfully.  
 *
 *     BT_STATUS_NO_CONNECTION - No audio connection exists.  
 *
 *     BT_STATUS_INVALID_PARM - Invalid parameter (XA_ERROR_CHECK only).  
 */
BtStatus HFG_SendAudioData(HfgChannel *Chan, BtPacket *Packet);
#define HFG_SendAudioData(c, p) (CMGR_SendAudioData(&(c)->cmgrHandler, p))

#endif /* BT_SCO_HCI_DATA == XA_ENABLED */ 

/*---------------------------------------------------------------------------
 * HFG_SetIndicatorValue()
 *
 *     This function sets the current value for an indicator.  If a service 
 *     level connection is active and indicator reporting is currently 
 *     enabled, the state of the modified indicator is reported to the 
 *     Hands-Free Unit.  If no service level connection exists, the current 
 *     value is changed and will be reported during the establishment of the 
 *     service level connection.  If indicator reporting is disabled, the 
 *     value of the indicator will only be reported when requested by the 
 *     Hands-Free Unit via the AT+CIND message.  
 *
 *     Upon registration of an Audio Gateway (HFG_Register()), all 
 *     indicators are initialized to 0.  To properly initialize all 
 *     indicators, this function must be called once for each indicator 
 *     prior to establishing a service level connection.  
 *
 *     Indicators must be sent to the hands-free device as specified by the 
 *     hands-free v1.5 specification.  Indicators are sent in the order that 
 *     calls are made to this function.  
 *
 * Parameters:
 *     Chan - Pointer to a registered channel structure.  
 *
 *     Indicator - Indicator type.  
 *
 *     Value - The value of the indicator.  
 *
 *     Response - A response structure to be used for transmitting the 
 *         response.  
 *
 * Returns:
 *     BT_STATUS_SUCCESS - The indicator value was set and the Response 
 *         structure is available for use.  
 *
 *     BT_STATUS_PENDING - The indicator value was set and queued for 
 *         sending to the hands-free unit.  When the response has been sent, 
 *         the HFG_EVENT_RESPONSE_COMPLETE event will be received by the 
 *         application.  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NO_CONNECTION - A Service Level Connection does not exist.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter is invalid or not properly 
 *         initialized.  
 */
BtStatus HFG_SetIndicatorValue(HfgChannel  *Chan, 
                               HfgIndicator Indicator, 
                               uint8_t           value, 
                               HfgResponse *Response);

/*---------------------------------------------------------------------------
 * HFG_GetIndicatorValue()
 *
 *     This function gets the current value of the specified indicator.  
 *
 * Parameters:
 *     Chan - Pointer to a registered channel structure.  
 *
 *     Indicator - Indicator type.  
 *
 *     Value - Receives the value of the indicator.  
 *
 * Returns:
 *     BT_STATUS_SUCCESS - The indicator value was set and the Response 
 *         structure is available for use.  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter is invalid or not properly 
 *         initialized.  
 */
BtStatus HFG_GetIndicatorValue(HfgChannel  *Chan, 
                               HfgIndicator Indicator, 
                               uint8_t          *value);

#if HFG_USE_IIA == XA_ENABLED

/*---------------------------------------------------------------------------
 * HFG_GetIIAReportingState()
 *
 *     This function is only available when HFG_USE_IIA is set to 
 *     XA_ENABLED.  This function indicates which Individual Indicators are 
 *     being reported by the Audio Gateway, and which are not.  The HF Unit 
 *     may send AT+BIA messages to disable the reporting of certain 
 *     individual activators such as signal strength to extend battery 
 *     life.  
 *
 * Requires:
 *     HFG_USE_IIA must be enabled.  
 *
 * Parameters:
 *     hfgChanP - This parameter must contain the address of a registered 
 *         hands free channel structure.  
 *
 *     indicatorsP - This parameter must contain the address of an 
 *         individual indicators activation report structure.  The elements 
 *         in this structure will be updated to correspond to the current 
 *         reporting state for the various individual indicators.  
 *
 * Returns:
 *     BT_STATUS_SUCCESS - The given individual indication report structure 
 *         has been updated with the latest activation state information.  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter invalid or not properly 
 *         initialized (XA_ERROR_CHECK only).  
 *
 *     BT_STATUS_FAILED - The individual indicator activation data has 
 *         become corrupted.  
 */
BtStatus HFG_GetIIAReportingState(HfgChannel           *hfgChanP, 
                                  HfgIndicatorActivity *indicatorsP);

#endif /* HFG_USE_IIA == XA_ENABLED */ 

/*---------------------------------------------------------------------------
 * HFG_SendHfResults()
 *
 *     This function is a generic routine for sending the results of a 
 *     Hands-Free operation from the Audio Gateway to the Hands-Free Unit.  
 *
 *     This prototype is used by several public functions macro definitions.  
 *
 * Parameters:
 *     Chan - This is a pointer to a registered channel structure.  
 *
 *     Command - This can be thought of as an enumeration corresponding to 
 *         the AT Command being responded to.  However, strictly speaking, 
 *         it is a bit-mask, as in the case of the AT_NETWORK_OPERATOR | 
 *         AT_READ command.  
 *
 *     Response - A response structure to be used for transmitting the 
 *         response.  
 *
 *     Parms - The meaning of this parameter varies with the Command 
 *         parameter.  In some cases, the Parms parameter must contain a 
 *         zero, because it is not to be used as a pointer to a buffer, and 
 *         is not to be copied from.  In some cases, the Parms parameter 
 *         must contain a non-zero value, because it must point to a buffer 
 *         that is to be copied into a response buffer.  In many cases the 
 *         buffered input data is not completely copied, so the original 
 *         data that is not copied needs to be preserved until the 
 *         HFG_EVENT_RESPONSE_COMPLETE event occurs.  The decision to not 
 *         copy the all of the input parameter data was deliberately made 
 *         both to save memory, and because it is not possible to predict 
 *         the size of all of the arrays needed to store the copies in 
 *         advance.  
 *
 *     ParmLen - Depending on the Command enumeration, this either may be 
 *         interpreted as the size of a buffer pointed at by the Parms 
 *         parameter, or it may have no meaning and must be zeroed out.  
 *
 *     done - If FALSE, then either a single message is used as the response 
 *         for this type of Command, or this is an intermediate call being 
 *         made to this function for this response, and more calls to this 
 *         function will follow.  If TRUE, then this is the final call that 
 *         will be made to this function for this response, and an OK 
 *         response message will be generated.  The only exception is for 
 *         the CLCC call list message, in which case the HFG_SendHfResults() 
 *         function does not generate an OK response message when the done 
 *         flag is set.  The logic for the CLCC message must generate its 
 *         own OK response message.  
 *
 * Returns:
 *     BT_STATUS_SUCCESS - The result code has been sent to the HF.  
 *
 *     BT_STATUS_PENDING - The result code has been successfully queued up 
 *         to be sent to the HF.  In some cases, depending on the Command 
 *         parameter, the application will be notified when the response has 
 *         been sent (via the callback function registered by 
 *         HFG_Register).  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NOT_SUPPORTED - Caller ID notification is disabled.  
 *
 *     BT_STATUS_NO_CONNECTION - A Service Level Connection does not exist.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter is invalid or not properly 
 *         initialized.  (This error will only be detected if XA_ERROR_CHECK 
 *         is defined to be XA_ENABLED.)  
 */
BtStatus HFG_SendHfResults(HfgChannel  *Chan, 
                           uint16_t    Command, 
                           uint32_t      Parms, 
                           uint16_t          ParmLen, 
                           HfgResponse *Response,
                           int         done);

/*---------------------------------------------------------------------------
 * HFG_SendOK
 *     This macro sends an OK response to the hands-free device.  This 
 *     macro must be called after receiving several events (see the 
 *     description of each event).  
 *
 * Parameters:
 *     Chan - This is a pointer to a registered channel structure.  
 *
 *     Response - A response structure to be used for transmitting the
 *         response.  
 *
 * Returns:
 *     BT_STATUS_SUCCESS - The result code has been sent to the HF.  
 *
 *     BT_STATUS_PENDING - The result code has been successfully queued up 
 *         to be sent to the HF.  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NO_CONNECTION - A Service Level Connection does not exist.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter is invalid or not properly 
 *         initialized.  (This error will only be detected if XA_ERROR_CHECK 
 *         is defined to be XA_ENABLED.)  
 */
BtStatus HFG_SendOK(HfgChannel *Chan, HfgResponse *Response);
#define HFG_SendOK(c, r) HFG_SendHfResults(c, AT_OK, 0, 0, r, FALSE)

/*---------------------------------------------------------------------------
 * HFG_SendError()
 *
 *     This macro sends an ERROR result code to the Hands Free Unit.  This 
 *     macro may be called after receiving several events when an error 
 *     condition exists.  (See the description of each event.)  If extended 
 *     error codes are enabled, the value specified in the 'Error' parameter 
 *     will be sent with the extended error response.  
 *
 * Parameters:
 *     Chan - This is a pointer to a registered channel structure.  
 *
 *     Error - The Extended error to be sent (if enabled).  
 *
 *     Response - A response structure to be used for transmitting the 
 *         response.  
 *
 * Returns:
 *     BT_STATUS_SUCCESS - The result code has been sent to the HF.  
 *
 *     BT_STATUS_PENDING - The result code has been successfully queued up 
 *         to be sent to the HF.  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NO_CONNECTION - A Service Level Connection does not exist.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter is invalid or not properly 
 *         initialized.  (This error will only be detected if XA_ERROR_CHECK 
 *         is defined to be XA_ENABLED.)  
 */
BtStatus HFG_SendError(HfgChannel  *Chan, 
                       HfgCmeError  Error,
                       HfgResponse *Response);
#define HFG_SendError(c, e, r) \
    HFG_SendHfResults(c, AT_ERROR, (e), 0, r, FALSE)

/*---------------------------------------------------------------------------
 * HFG_SendRing()
 *
 *     This macro notifies the Hands Free Unit of an incoming call.  The 
 *     Audio Gateway invokes this macro periodically as long as the call is 
 *     still incoming.  If caller ID is enabled, a call to 
 *     HFG_SendCallerId() should be called after calling HFG_SendRing().  
 *
 * Parameters:
 *     Chan - This is a pointer to a registered channel structure.  
 *
 *     Response - A response structure to be used for transmitting the 
 *         response.  
 *
 * Returns:
 *     BT_STATUS_SUCCESS - The result code has been sent to the HF.  
 *
 *     BT_STATUS_PENDING - The result code has been successfully queued up 
 *         to be sent to the HF.  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NO_CONNECTION - A Service Level Connection does not exist.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter is invalid or not properly 
 *         initialized.  (This error will only be detected if XA_ERROR_CHECK 
 *         is defined to be XA_ENABLED.)  
 */
BtStatus HFG_SendRing(HfgChannel *Chan, HfgResponse *Response);
#define HFG_SendRing(c, r) HFG_SendHfResults(c, AT_RING, 0, 0, r, FALSE)

/*---------------------------------------------------------------------------
 * HFG_SendCallListRsp
 *
 *     This macro sends the call listing response to the Hands-Free Unit.  
 *     (See HFG_EVENT_LIST_CURRENT_CALLS.)  This macro should be called for 
 *     each line supported on the audio gateway with the state of any call 
 *     set appropriately.  If no call is active on the specified line, a 
 *     response (+CLCC) will not be sent.  If a call is is any state besides 
 *     HFG_CALL_STATUS_NONE, then a response will be sent.  On the final 
 *     call to this function, FinalFlag should be set.  This will send an OK 
 *     response in addition to +CLCC (if sent).  
 *
 *     If it is known that no call exists on any line, it is acceptable to 
 *     call HFG_SendOK() instead of calling this function.  
 *
 * Parameters:
 *     Chan - This is a pointer to a registered channel structure.  
 *
 *     Parms - A structure containing the call status information 
 *         for the specified line.  Although a copy of this data is made, 
 *         only the addresses of the buffers pointed at inside the data are 
 *         copied, so the buffers pointed at by this structure need to be 
 *         preserved until the HFG_EVENT_RESPONSE_COMPLETE event occurs.  
 *
 *     Response - A response structure to be used for transmitting the
 *         response.  
 *
 *     FinalFlag - Set to TRUE when the final call is make to this 
 *         function.  
 *
 * Returns:
 *     BT_STATUS_SUCCESS - The result code has been sent to the HF.  
 *
 *     BT_STATUS_PENDING - The result code has been successfully queued up 
 *         to be sent to the HF.  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NO_CONNECTION - A Service Level Connection does not exist.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter is invalid or not properly 
 *         initialized.  (This error will only be detected if XA_ERROR_CHECK 
 *         is defined to be XA_ENABLED.)  
 */
BtStatus HFG_SendCallListRsp(HfgChannel       *Chan, 
                             HfgCallListParms *Parms, 
                             HfgResponse      *Response, 
                             int              FinalFlag);
#define HFG_SendCallListRsp(c, p, r, f) \
    HFG_SendHfResults(c, \
                      AT_LIST_CURRENT_CALLS, \
                      (uint32_t)p, \
                      sizeof(HfgCallListParms), \
                      r, \
                      f)

/*---------------------------------------------------------------------------
 * HFG_SendCallerId()
 *
 *     This macro Sends a Calling Line Identification result code containing 
 *     the phone number and type of the incoming call.  This macro should be 
 *     called immediately after HFG_SendRing() if Calling Line 
 *     Identification Notification has been enabled by the Hands Free Unit.  
 *     If caller ID notification has been disabled by the remote Hands Free 
 *     Unit, no notification will be sent even if this macro is invoked.  
 *
 * Parameters:
 *     Chan - Pointer to a registered channel structure.  
 *
 *     CallId - A structure containing the number and type of the 
 *         incoming call.  Although a copy of this data is made, only the 
 *         addresses of the buffers pointed at inside the data are copied, 
 *         so the buffers pointed at by this structure need to be preserved 
 *         until the HFG_EVENT_RESPONSE_COMPLETE event occurs.  
 *
 *     Response - A response structure to be used for transmitting the
 *         response.  
 *
 * Returns:
 *     BT_STATUS_SUCCESS - The result code has been sent to the HF.  
 *
 *     BT_STATUS_PENDING - The result code has been successfully queued up 
 *         to be sent to the HF.  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NOT_SUPPORTED - Caller ID notification is disabled.  
 *
 *     BT_STATUS_NO_CONNECTION - A Service Level Connection does not exist.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter is invalid or not properly 
 *         initialized.  (This error will only be detected if XA_ERROR_CHECK 
 *         is defined to be XA_ENABLED.)  
 */
BtStatus HFG_SendCallerId(HfgChannel       *Chan, 
                          HfgCallerIdParms *CallId, 
                          HfgResponse      *Response);
#define HFG_SendCallerId(c, i, r)  \
    HFG_SendHfResults(c, \
                      AT_CALL_ID, \
                      (uint32_t)i, \
                      sizeof(HfgCallerIdParms), \
                      r, \
                      FALSE)

/*---------------------------------------------------------------------------
 * HFG_SendSubscriberNumberRsp
 *
 *     This macro is called in response to a request for the subscriber 
 *     number.  (See HFG_EVENT_QUERY_SUBSCRIBER_NUMBER.)  It is not 
 *     necessary to call HFG_SendOK() after calling this macro.  
 * 
 * Parameters:
 *     Chan - This is a pointer to a registered channel structure.  
 *
 *     SbuNum - A structure containing the subscriber number information.  
 *         Although a copy of this data is made, only the addresses of the 
 *         buffers pointed at inside the data are copied, so the buffers 
 *         pointed at by this structure need to be preserved until the 
 *         HFG_EVENT_RESPONSE_COMPLETE event occurs.  
 *
 *     Response - A response structure to be used for transmitting the 
 *         response.  
 *
 * Returns:
 *     BT_STATUS_SUCCESS - The result code has been sent to the HF.  
 *
 *     BT_STATUS_PENDING - The result code has been successfully queued up 
 *         to be sent to the HF.  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NO_CONNECTION - A Service Level Connection does not exist.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter is invalid or not properly 
 *         initialized.  (This error will only be detected if XA_ERROR_CHECK 
 *         is defined to be XA_ENABLED.)  
 */
BtStatus HFG_SendSubscriberNumberRsp(HfgChannel       *Chan, 
                                     HfgSubscriberNum *SubNum, 
                                     HfgResponse      *Response);
#define HFG_SendSubscriberNumberRsp(c, s, r)  \
    HFG_SendHfResults(c, \
                      AT_SUBSCRIBER_NUM, \
                      (uint32_t)s, \
                      sizeof(HfgSubscriberNum), \
                      r, \
                      TRUE)

/*---------------------------------------------------------------------------
 * HFG_SendNetworkOperatorRsp
 *
 *     This macro is called in response to a request for the network 
 *     operator information.  (See HFG_EVENT_QUERY_NETWORK_OPERATOR.)  It is 
 *     not necessary to call HFG_SendOK() after calling this macro.  
 *
 * Parameters:
 *     Chan - This is a pointer to a registered channel structure.  
 *
 *     Oper - A structure containing the operator information.  Although 
 *         a copy of this data is made, only the addresses of the buffers 
 *         pointed at inside the data are copied, so the buffers pointed at 
 *         by this structure need to be preserved until the 
 *         HFG_EVENT_RESPONSE_COMPLETE event occurs.  
 *
 *     Response - A response structure to be used for transmitting the 
 *         response.  
 *
 * Returns:
 *     BT_STATUS_SUCCESS - The result code has been sent to the HF.  
 *
 *     BT_STATUS_PENDING - The result code has been successfully queued up 
 *                         to be sent to the HF.  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NO_CONNECTION - A Service Level Connection does not exist.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter is invalid or not properly 
 *         initialized.  (This error will only be detected if XA_ERROR_CHECK 
 *         is defined to be XA_ENABLED.)  
 */
BtStatus HFG_SendNetworkOperatorRsp(HfgChannel         *Chan, 
                                    HfgNetworkOperator *Oper, 
                                    HfgResponse        *Response);
#define HFG_SendNetworkOperatorRsp(c, o, r)  \
    HFG_SendHfResults(c, \
                      AT_NETWORK_OPERATOR | AT_READ, \
                      (uint32_t)o, \
                      sizeof(HfgNetworkOperator), \
                      r, \
                      TRUE)

/*---------------------------------------------------------------------------
 * HFG_SendMicVolume()
 *
 *     This macro notifies the Hands Free Unit of the Audio Gateway's 
 *     current microphone gain level.  
 *
 * Parameters:
 *     Chan - This is a pointer to a registered channel structure.  
 *
 *     gain - Current gain level.  
 *
 *     Response - A response structure to be used for transmitting the
 *         response.  
 *
 * Returns:
 *     BT_STATUS_SUCCESS - The microphone volume level has been sent to the 
 *         HF.  
 *
 *     BT_STATUS_PENDING - The microphone volume level has been successfully 
 *         queued up to be sent to the HF.  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NO_CONNECTION - A Service Level Connection does not exist.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter is invalid or not properly 
 *         initialized.  (This error will only be detected if XA_ERROR_CHECK 
 *         is defined to be XA_ENABLED.)  
 */
BtStatus HFG_SendMicVolume(HfgChannel  *Chan, 
                           uint8_t           Gain, 
                           HfgResponse *Response);
#define HFG_SendMicVolume(c, g, r)  \
    HFG_SendHfResults(c, AT_MICROPHONE_GAIN, (uint32_t)(g), 0, r, FALSE)

/*---------------------------------------------------------------------------
 * HFG_SendSpeakerVolume()
 *
 *     This macro notifies the Hands Free Unit of the Audio Gateway's 
 *     current speaker volume level.  
 *
 * Parameters:
 *     Chan - This is a pointer to a registered channel structure.  
 *
 *     gain - Current volume level.  
 *
 *     Response - A response structure to be used for transmitting the
 *         response.  
 *
 * Returns:
 *     BT_STATUS_SUCCESS - The speaker volume level has been sent to the 
 *         HF.  
 *
 *     BT_STATUS_PENDING - The speaker volume level has been successfully 
 *         queued up to be sent to the HF.  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NO_CONNECTION - A Service Level Connection does not exist.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter is invalid or not properly 
 *         initialized.  (This error will only be detected if XA_ERROR_CHECK 
 *         is defined to be XA_ENABLED.)  
 */
BtStatus HFG_SendSpeakerVolume(HfgChannel  *Chan, 
                               uint8_t           Gain, 
                               HfgResponse *Response);
#define HFG_SendSpeakerVolume(c, g, r)  \
    HFG_SendHfResults(c, AT_SPEAKER_GAIN, (uint32_t)(g), 0, r, FALSE)

/*---------------------------------------------------------------------------
 * HFG_SendRingToneStatus()
 *
 *     This macro notifies the Hands Free Unit of the in-band ring tone 
 *     status.  
 *
 * Parameters:
 *     Chan - This is a pointer to a registered channel structure.  
 *
 *     Enabled - TRUE if in-band ring tone enabled, FALSE otherwise.  
 *
 *     Response - A response structure to be used for transmitting the 
 *         response.  
 *
 * Returns:
 *     BT_STATUS_SUCCESS - The result code has been sent to the HF.  
 *
 *     BT_STATUS_PENDING - The result code has been successfully queued up 
 *         to be sent to the HF.  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NO_CONNECTION - A Service Level Connection does not exist.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter is invalid or not properly 
 *         initialized.  (This error will only be detected if XA_ERROR_CHECK 
 *         is defined to be XA_ENABLED.)  
 */
BtStatus HFG_SendRingToneStatus(HfgChannel  *Chan, 
                                int         Enabled, 
                                HfgResponse *Response);
#define HFG_SendRingToneStatus(c, e, r)  \
    HFG_SendHfResults(c, AT_IN_BAND_RING_TONE, (uint32_t)(e), 0, r, FALSE)

#if HFG_USE_RESP_HOLD == XA_ENABLED

/*---------------------------------------------------------------------------
 * HFG_SendRespHoldState()
 *
 *     This macro notifies the Hands Free Unit of the Response and Hold 
 *     state.  This macro is invoked to report the Response and Hold state 
 *     in response to a request by the Hands-Free Unit, or upon an action 
 *     taken on the audio gateway.  (See HFG_RESPONSE_HOLD.)  
 *
 *     This macro is also invoked in response to a query for the 
 *     Response and Hold state from the Hands-Free unit.  (See 
 *     HFG_QUERY_RESPONSE_HOLD.)  This macro should be called with the 
 *     'State' parameter set to HFG_RESP_HOLD_HOLD if the audio gateway is 
 *     in the Response and Hold state, followed by a call to HFG_SendOK().  
 *     Otherwise, the application should simply call HFG_SendOK().  
 *
 * Parameters:
 *     Chan - This is a pointer to a registered channel structure.  
 *
 *     State - The current Response and Hold state.  
 *
 *     Response - A response structure to be used for transmitting the 
 *         response.  
 *
 * Returns:
 *     BT_STATUS_SUCCESS - The result code has been sent to the HF.  
 *
 *     BT_STATUS_PENDING - The result code has been successfully queued up 
 *         to be sent to the HF.  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NO_CONNECTION - A Service Level Connection does not exist.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter is invalid or not properly 
 *         initialized.  (This error will only be detected if XA_ERROR_CHECK 
 *         is defined to be XA_ENABLED.)  
 */
BtStatus HFG_SendRespHoldState(HfgChannel     *Chan, 
                               HfgResponseHold State, 
                               HfgResponse    *Response);
#define HFG_SendRespHoldState(c, s, r)  \
    HFG_SendHfResults(c, \
                      AT_RESPONSE_AND_HOLD, \
                      (uint32_t)s, \
                      sizeof(HfgResponseHold), \
                      r, \
                      FALSE)

#endif /* HFG_USE_RESP_HOLD == XA_ENABLED */ 

/*---------------------------------------------------------------------------
 * HFG_CallWaiting()
 *
 *     This macro notifies the Hands Free Unit of a waiting call (if the 
 *     Hands Free Unit has enabled the Call Waiting Notification feature).  
 *
 * Parameters:
 *     Chan - This is a pointer to a registered channel structure.  
 *
 *     CallWait - A structure containing the number, type, and class of the 
 *         incoming call.  Although a copy of this data is made, only the 
 *         addresses of the buffers pointed at inside the data are copied, 
 *         so the buffers pointed at by this structure need to be preserved 
 *         until the HFG_EVENT_RESPONSE_COMPLETE event occurs.  
 *
 *     Response - A response structure to be used for transmitting the 
 *         response.  
 *
 * Returns:
 *     BT_STATUS_SUCCESS - The result code has been sent to the HF.  
 *
 *     BT_STATUS_PENDING - The result code has been successfully queued up 
 *         to be sent to the HF.
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NO_CONNECTION - A Service Level Connection does not exist.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter is invalid or not properly 
 *         initialized.  (This error will only be detected if XA_ERROR_CHECK 
 *         is defined to be XA_ENABLED.)  
 */
BtStatus HFG_CallWaiting(HfgChannel       *Chan, 
                         HfgCallWaitParms *CallWait,
                         HfgResponse      *Response);
#define HFG_CallWaiting(c, w, r)  \
    HFG_SendHfResults(c, \
                      AT_CALL_WAIT_NOTIFY, \
                      (uint32_t)w, \
                      sizeof(HfgCallWaitParms), \
                      r, \
                      FALSE)

/*---------------------------------------------------------------------------
 * HFG_EnableVoiceRec()
 *
 *     This function notifies the Hands Free Unit that the Audio Gateway is 
 *     enabling or disabling voice recognition.  
 *
 * Parameters:
 *     Chan - This is a pointer to a registered channel structure.  
 *
 *     Enabled - TRUE if voice recognition is being activated, otherwise 
 *         FALSE.  
 *
 *     Response - A response structure to be used for transmitting the 
 *         response.  
 *
 * Returns:
 *     BT_STATUS_SUCCESS - The result code has been sent to the HF.  
 *
 *     BT_STATUS_PENDING - The result code has been successfully queued up 
 *         to be sent to the HF.  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NO_CONNECTION - A Service Level Connection does not exist.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter is invalid or not properly 
 *         initialized.  (This error will only be detected if XA_ERROR_CHECK 
 *         is defined to be XA_ENABLED.)  
 *
 *     BT_STATUS_RESTRICTED - The remote Hands-Free device does not support 
 *         Voice Recognition.  
 */
BtStatus HFG_EnableVoiceRec(HfgChannel  *Chan, 
                            int         Enabled, 
                            HfgResponse *Response);

/*---------------------------------------------------------------------------
 * HFG_VoiceTagRsp()
 *
 *     This macro is invoked by the application to return the phone number 
 *     associated with the VoiceTag request to the Hands Free Unit.  It is 
 *     not necessary to call HFG_SendOK() after invoking this macro.  
 *
 * Parameters:
 *     Chan - This is a pointer to a registered channel structure.  
 *
 *     voiceTag - A structure containing the phone number associated 
 *         with the last voice tag.  Although a copy of this data is made, 
 *         only the addresses of the buffers pointed at inside the data are 
 *         copied, so the buffers pointed at by this structure need to be 
 *         preserved until the HFG_EVENT_RESPONSE_COMPLETE event occurs.  
 *
 *     Response - A response structure to be used for transmitting the 
 *         response.
 *
 * Returns:
 *     BT_STATUS_SUCCESS - The result code has been sent to the HF.  
 *
 *     BT_STATUS_PENDING - The result code has been successfully queued up 
 *         to be sent to the HF.  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NO_CONNECTION - A Service Level Connection does not exist.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter is invalid or not properly 
 *         initialized.  (This error will only be detected if XA_ERROR_CHECK 
 *         is defined to be XA_ENABLED.)  
 */
BtStatus HFG_VoiceTagRsp(HfgChannel  *Chan, 
                         HfgVoiceTag *voiceTag, 
                         HfgResponse *Response);
#define HFG_VoiceTagRsp(c, v, r)  \
    HFG_SendHfResults(c, AT_VOICE_TAG, (uint32_t)v, sizeof(HfgVoiceTag), r, TRUE)

/*---------------------------------------------------------------------------
 * HFG_NoCarrierRsp()
 *
 *     This macro is invoked by the application to send the "NO CARRIER" 
 *     response to the Hands Free Unit.  This response can be sent in 
 *     addition to the "+CME ERROR:" response.  
 *
 * Parameters:
 *     Chan - This is a pointer to a registered channel structure.  
 *
 *     Response - A response structure to be used for transmitting the 
 *         response.  
 *
 * Returns:
 *     BT_STATUS_SUCCESS - The result code has been sent to the HF.  
 *
 *     BT_STATUS_PENDING - The result code has been successfully queued up 
 *         to be sent to the HF.
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NO_CONNECTION - A Service Level Connection does not exist.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter is invalid or not properly 
 *         initialized.  (This error will only be detected if XA_ERROR_CHECK 
 *         is defined to be XA_ENABLED.)  
 */
BtStatus HFG_NoCarrierRsp(HfgChannel *Chan, HfgResponse *Response);
#define HFG_NoCarrierRsp(c, r)  \
    HFG_SendHfResults(c, AT_NO_CARRIER, 0, 0, r, FALSE)

/*---------------------------------------------------------------------------
 * HFG_BusyRsp()
 *
 *     This macro is invoked by the application to send a "BUSY" response to 
 *     the Hands Free Unit.  This response can be sent in addition to the 
 *     "+CME ERROR:" response.  
 *
 * Parameters:
 *     Chan - This is a pointer to a registered channel structure.  
 *
 *     Response - A response structure to be used for transmitting the 
 *         response.  
 *
 * Returns:
 *     BT_STATUS_SUCCESS - The result code has been sent to the HF.  
 *
 *     BT_STATUS_PENDING - The result code has been successfully queued up 
 *         to be sent to the HF.  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NO_CONNECTION - A Service Level Connection does not exist.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter is invalid or not properly 
 *         initialized.  (This error will only be detected if XA_ERROR_CHECK 
 *         is defined to be XA_ENABLED.)  
 */
BtStatus HFG_BusyRsp(HfgChannel *Chan, HfgResponse *Response);
#define HFG_BusyRsp(c, r)  HFG_SendHfResults(c, AT_BUSY, 0, 0, r, FALSE)

/*---------------------------------------------------------------------------
 * HFG_NoAnswerRsp()
 *
 *     This macro is invoked by the application to send a "NO ANSER" 
 *     response to the Hands Free Unit.  This response can be sent in 
 *     addition to the "+CME ERROR:" response.  
 *
 * Parameters:
 *     Chan - This is a pointer to a registered channel structure.  
 *
 *     Response - A response structure to be used for transmitting the 
 *         response.  
 *
 * Returns:
 *     BT_STATUS_SUCCESS - The result code has been sent to the HF.  
 *
 *     BT_STATUS_PENDING - The result code has been successfully queued up 
 *         to be sent to the HF.  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NO_CONNECTION - A Service Level Connection does not exist.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter is invalid or not properly 
 *         initialized.  (This error will only be detected if XA_ERROR_CHECK 
 *         is defined to be XA_ENABLED.)  
 */
BtStatus HFG_NoAnswerRsp(HfgChannel *Chan, HfgResponse *Response);
#define HFG_NoAnswerRsp(c, r)  \
    HFG_SendHfResults(c, AT_NO_ANSWER, 0, 0, r, FALSE)

/*---------------------------------------------------------------------------
 * HFG_DelayedRsp()
 *
 *     This macro is invoked by the application to send a "DELAYED" response 
 *     to the Hands Free Unit.  This response can be sent in addition to the 
 *     "+CME ERROR:" response.  
 *
 * Parameters:
 *     Chan - This is a pointer to a registered channel structure.  
 *
 *     Response - A response structure to be used for transmitting the 
 *         response.  
 *
 * Returns:
 *     BT_STATUS_SUCCESS - The result code has been sent to the HF.  
 *
 *     BT_STATUS_PENDING - The result code has been successfully queued up 
 *         to be sent to the HF.  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NO_CONNECTION - A Service Level Connection does not exist.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter is invalid or not properly 
 *         initialized.  (This error will only be detected if XA_ERROR_CHECK 
 *         is defined to be XA_ENABLED.)  
 */
BtStatus HFG_DelayedRsp(HfgChannel *Chan, HfgResponse *Response);
#define HFG_DelayedRsp(c, r)  \
    HFG_SendHfResults(c, AT_DELAYED, 0, 0, r, FALSE)

/*---------------------------------------------------------------------------
 * HFG_BlacklistedRsp()
 *
 *     This macro is invoked by the application to send the "BLACKLISTED" 
 *     response to the Hands Free Unit.  This response can be sent in 
 *     addition to the "+CME ERROR:" response.  
 *
 * Parameters:
 *     Chan - This is a pointer to a registered channel structure.  
 *
 *     Response - A response structure to be used for transmitting the 
 *         response.  
 *
 * Returns:
 *     BT_STATUS_SUCCESS - The result code has been sent to the HF.  
 *
 *     BT_STATUS_PENDING - The result code has been successfully queued up 
 *         to be sent to the HF.  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NO_CONNECTION - A Service Level Connection does not exist.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter is invalid or not properly 
 *         initialized.  (This error will only be detected if XA_ERROR_CHECK 
 *         is defined to be XA_ENABLED.)  
 */
BtStatus HFG_BlacklistedRsp(HfgChannel *Chan, HfgResponse *Response);
#define HFG_BlacklistedRsp(c, r)  \
    HFG_SendHfResults(c, AT_BLACKLISTED, 0, 0, r, FALSE)

/*---------------------------------------------------------------------------
 * HFG_SendAtResponse()
 *
 *     This macro sends any AT response.  The 'AtString' parameter must be 
 *     initialized, and the AT response must be formatted properly.  The 
 *     "AT" characters must be included in the string when needed.  The 
 *     carriage return and line feed characters will be added 
 *     automatically.  
 *
 *     When the AT response has been sent, the HFG_EVENT_RESPONSE_COMPLETE 
 *     event will be received by the application's callback function.  
 *
 * Parameters:
 *     Chan - This is a pointer to a registered channel structure.  
 *
 *     AtString - A properly formatted AT response.  Only the 
 *         address of the buffers pointed at is copied, so the input buffer 
 *         needs to be preserved until the HFG_EVENT_RESPONSE_COMPLETE event 
 *         occurs.  
 *
 *     Response - A response structure to be used for transmitting the 
 *         response.  
 *
 *     done - If FALSE, then either a single message is used as the response 
 *         for this type of Command, or this is an intermediate call being 
 *         made to this function for this response, and more calls to this 
 *         function will follow.  If TRUE, then this is the final call that 
 *         will be made to this function for this response, and an OK 
 *         response message will be generated.  
 *
 * Returns:
 *     BT_STATUS_SUCCESS - The result code has been sent to the HF.  
 *
 *     BT_STATUS_PENDING - The operation has started; the application will 
 *         be notified when the response has been sent (via the callback 
 *         function registered by HFG_Register).  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NO_CONNECTION - The operation failed because a service link 
 *         does not exist to the audio gateway.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter is invalid or not properly 
 *         initialized.  (This error will only be detected if XA_ERROR_CHECK 
 *         is defined to be XA_ENABLED.)  
 */
BtStatus HFG_SendAtResponse(HfgChannel  *Chan, 
                            const char  *AtString,
                            HfgResponse *Response,
                            int         done);
#define HFG_SendAtResponse(c, s, r, d)  \
    HFG_SendHfResults(c, AT_RAW, (uint32_t)s, sizeof(s), r, d)

/*---------------------------------------------------------------------------
 * HFG_IsNRECEnabled()
 *
 *     This function indicates whether Noise Reduction and Echo Cancellation 
 *     is enabled in the Audio Gateway.  
 *
 * Parameters:
 *     Chan - Pointer to a registered channel structure.  
 *
 * Returns:
 *     TRUE - NREC is enabled in the AG.  
 *
 *     FALSE - NREC is disabled in the AG.  
 */
int HFG_IsNRECEnabled(HfgChannel *Chan);

/*---------------------------------------------------------------------------
 * HFG_IsInbandRingEnabled()
 *
 *     This function indicates whether In-Band Ringing is enabled in the 
 *     Audio Gateway.  
 *
 * Parameters:
 *     Chan - Pointer to a registered channel structure.  
 *
 * Returns:
 *     TRUE - In-band ringing is enabled in the AG.  
 *
 *     FALSE - In-band ringing is disabled in the AG.  
 */
int HFG_IsInbandRingEnabled(HfgChannel *Chan);

/*---------------------------------------------------------------------------
 * HFG_IsCallIdNotifyEnabled()
 *
 *     This function indicates whether Caller ID notification is enabled in 
 *     the Audio Gateway.  
 *
 * Parameters:
 *     Chan - Pointer to a registered channel structure.  
 *
 * Returns:
 *     TRUE - Caller ID notification is enabled in the AG.  
 *
 *     FALSE - Caller ID notification is disabled in the AG.  
 */
int HFG_IsCallIdNotifyEnabled(HfgChannel *Chan);

/*---------------------------------------------------------------------------
 * HFG_IsVoiceRecActive()
 *
 *     This function indicates whether Voice Recognition is active in the 
 *     Audio Gateway.  
 *
 * Parameters:
 *     Chan - Pointer to a registered channel structure.  
 *
 * Returns:
 *     TRUE - Voice Recognition is active in the AG.  
 *
 *     FALSE - Voice Recognition is inactive in the AG.  
 */
int HFG_IsVoiceRecActive(HfgChannel *Chan);

/*---------------------------------------------------------------------------
 * HFG_IsCallWaitingActive()
 *
 *     This function indicates wheter Call Waiting is active in the Audio 
 *     Gateway.  
 *
 * Parameters:
 *     Chan - Pointer to a registered channel structure.  
 *
 * Returns:
 *     TRUE - Call Waiting is active in the AG.  
 *
 *     FALSE - Call Waiting is inactive in the AG.  
 */
int HFG_IsCallWaitingActive(HfgChannel *Chan);

/*---------------------------------------------------------------------------
 * HFG_SetMasterRole()
 *
 *     This macro attempts to keep the local device in the Master role.  
 *
 * Parameters:
 *     Chan - Pointer to a registered channel structure.  
 *
 *     Flag - TRUE if this device wants to be the master, otherwise FALSE.  
 * 
 * Returns:
 *
 *     BtStatus
 */
extern BtStatus CMGR_SetMasterRole(CmgrHandler *Handler, int flag);
BtStatus HFG_SetMasterRole(HfgChannel *Chan, int Flag);
#define HFG_SetMasterRole(c, f)  CMGR_SetMasterRole(&((c)->cmgrHandler), f)

/*---------------------------------------------------------------------------
 * HFG_UnitFeatures()
 *
 *     This function provides the features of the Hands-Free Unit.  
 *
 * Parameters:
 *     Chan - Pointer to a registered channel structure.  
 *
 * Returns:
 *     HfgHandsFreeFeatures
 *
 */
HfgHandsFreeFeatures HFG_UnitFeatures(HfgChannel *Chan);

/*---------------------------------------------------------------------------
 *            HFG_GetNumInd()
 *---------------------------------------------------------------------------
 *
 *     The function provides the number of individual indicators supported 
 *     by the audio gateway.  
 *
 * Parameters:
 *     none
 *
 * Returns:  
 *     The number of individual indicators.  
 */
uint8_t HFG_GetNumInd(void);

/*---------------------------------------------------------------------------
 * HFG_CreateCodecConnection()
 *
 *     This function notifies the Hands Free Unit that the Audio Gateway is 
 *     creating codec connection.  
 *
 * Parameters:
 *     Chan - This is a pointer to a registered channel structure.  
 *
 *     Codec_id - codec id used by the subsequent sco links 
 *
 *     Response - A response structure to be used for transmitting the 
 *         response.  
 *
 * Returns:
 *     BT_STATUS_SUCCESS - The result code has been sent to the HF.  
 *
 *     BT_STATUS_PENDING - The result code has been successfully queued up 
 *         to be sent to the HF.  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NO_CONNECTION - A Service Level Connection does not exist.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter is invalid or not properly 
 *         initialized.  (This error will only be detected if XA_ERROR_CHECK 
 *         is defined to be XA_ENABLED.)  
 *
 *     BT_STATUS_RESTRICTED - The remote Hands-Free device does not support 
 *         Voice Recognition.  
 */
BtStatus HFG_CreateCodecConnection(HfgChannel  *Chan, 
                                    uint32_t Codec_id,
                                    HfgResponse *Response);
                                    
/*---------------------------------------------------------------------------
 * HFG_SetSupportedFeature()
 *
 *     This function set local ag supported features.  
 *
 * Parameters:
 * 
 *     feature - bit mask of ag feature.  
 *
 * Returns:
 *
 *     NULL
 */
void HFG_SetSupportedFeature(uint16_t feature);

/*---------------------------------------------------------------------------
 * HFG_GetSupportedFeature()
 *
 *     This function get local ag supported features.  
 *
 * Parameters:
 * 
 *     NULL.  
 *
 * Returns:
 *
 *     ag supported feature
 */
uint16_t HFG_GetSupportedFeature(void);
#endif /* __HFG_H_ */ 

