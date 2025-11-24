#ifndef __HF_API_H_
#define __HF_API_H_

#include "btconfig.h"

#include "bt_types.h"
#include "rfcomm_api.h"
#include "sec_api.h"
/*---------------------------------------------------------------------------
 * Hands-free SDK API layer
 *
 *     The Hands-free SDK is designed to create a hands-free application 
 *     for Bluetooth.  It includes a complete implementation of the 
 *     Hands-Free Profiles v1.5.  It also includes some features beyond the 
 *     basic profiles that take advantage of features exported by many 
 *     shipping phones.  
 *
 *     The Hands-free SDK provides service discovery, connection management, 
 *     call management, and basic phone book management.  It also allows for 
 *     the transmission and reception of AT commands not specifically 
 *     supported by Bluetooth profiles.  
 *
 *     This SDK also includes an optional Call Manager component that tracks 
 *     the state of calls in the audio gateway.  Many different events are 
 *     generated to provide information about calls on the audio gateway.  
 *     The Call Manager interprets these various events and translates them 
 *     into a simplified event form.  The states of all calls are maintained 
 *     within the Call Manager and can be queried by the application.  The 
 *     Hands-Free code informs the Call Manager when events occur that could 
 *     affect the call state.  The Call Manager makes intelligent decisions 
 *     on which AT commands to send based on this information. The Call 
 *     Manager will also use a polling method to keep the call state 
 *     information up to date.  
 */

/****************************************************************************
 *
 * Constants
 *
 ****************************************************************************/
#ifndef HF_WIDE_BAND_SPEECH
#define HF_WIDE_BAND_SPEECH XA_DISABLED
#endif
#define HF_CODEC_NEG HF_WIDE_BAND_SPEECH

/*---------------------------------------------------------------------------
 * HF_VREC constant
 *     This define indicates whether the voice recognition features are 
 *     supported or not.  Normally, voice recognition is enabled.  If your  
 *     application does not support voice recognition, then you can define 
 *     HF_VREC as XA_DISABLED in the overide.h file.  
 */
#ifndef HF_VREC
#define HF_VREC XA_ENABLED
#endif /* HF_VREC */ 

/*---------------------------------------------------------------------------
 * HF_FEATURE_ECHO_NOISE constant
 *     This define indicates whether this device supports the echo canceling 
 *     and/or noise reduction feature.  This value is used when defining the 
 *     capabilities of the Hands-Free application.  See HF_SDK_FEATURES.  
 */
#define HF_FEATURE_ECHO_NOISE           0x00000001

/*---------------------------------------------------------------------------
 * HF_FEATURE_CALL_WAITING constant
 *     This define indicates whether this device supports the call-waiting 
 *     and 3-way calling feature.  This value can be used when defining the 
 *     capabilities of the Hands-Free application.  See HF_SDK_FEATURES.  
 */
#define HF_FEATURE_CALL_WAITING         0x00000002

/*---------------------------------------------------------------------------
 * HF_FEATURE_CLI_PRESENTATION constant
 *     This define indicates whether this device supports the Calling Line 
 *     Identification (CLI) presentation capability feature.  This value can 
 *     be used when defining the capabilities of the Hands-Free 
 *     application.  See HF_SDK_FEATURES.  
 */
#define HF_FEATURE_CLI_PRESENTATION     0x00000004

/*---------------------------------------------------------------------------
 * HF_FEATURE_VOICE_RECOGNITION constant
 *     This define indicates whether this device supports the voice 
 *     recognition function.  This value can be used when defining the 
 *     capabilities of the Hands-Free application.  See HF_SDK_FEATURES.  
 */
#define HF_FEATURE_VOICE_RECOGNITION    0x00000008

/*---------------------------------------------------------------------------
 * HF_FEATURE_VOLUME_CONTROL constant
 *     This define indicates whether this device supports the remote volume 
 *     control feature.  This value can be used when defining the 
 *     capabilities of the Hands-Free application.  See HF_SDK_FEATURES.  
 */
#define HF_FEATURE_VOLUME_CONTROL       0x00000010

/*---------------------------------------------------------------------------
 * HF_FEATURE_ENHANCED_CALL_STATUS constant
 *     This define indicates whether this device supports enhanced call 
 *     status features like call listing and call held indications.  See 
 *     HF_SDK_FEATURES.  
 */
#define HF_FEATURE_ENHANCED_CALL_STATUS 0x00000020

/*---------------------------------------------------------------------------
 * HF_FEATURE_ENHANCED_CALL_CTRL constant
 *     This define indicates whether this device supports enhanced call 
 *     control features like call specifying specific lines to put on hold.  
 *     See HF_SDK_FEATURES.  
 */
#define HF_FEATURE_ENHANCED_CALL_CTRL   0x00000040

/*---------------------------------------------------------------------------
 * HF_FEATURE_CODEC_NEGOTIATION constant
 *     This define indicates whether this device supports codec negotiation 
 *     features used for Wide Band Speech.  
 *     See HF_SDK_FEATURES.  
 */
#define HF_FEATURE_CODEC_NEGOTIATION    0x00000080

/*---------------------------------------------------------------------------
 * HF_SDK_FEATURES constant
 *     This define indicates which Hands Free Unit features are supported by 
 *     the application and device.  Features that are supported by the 
 *     application must be advertised to the Audio Gateway, so that it knows 
 *     the capabilities of the Hands-Free Unit.  If this value needs to be 
 *     changed, a modified definition can be placed in overide.h with the 
 *     features supported by the Hands-Free application.  
 */
#ifndef HF_SDK_FEATURES
#define HF_FEATURE_VOICE_RECOGNITION_CFG    HF_FEATURE_VOICE_RECOGNITION

#define HF_FEATURE_CODEC_NEGOTIATION_CFG    HF_FEATURE_CODEC_NEGOTIATION

#define HF_SDK_FEATURES  (HF_FEATURE_ECHO_NOISE           | \
                          HF_FEATURE_CALL_WAITING         | \
                          HF_FEATURE_CLI_PRESENTATION     | \
                          HF_FEATURE_VOICE_RECOGNITION_CFG| \
                          HF_FEATURE_CODEC_NEGOTIATION_CFG| \
                          HF_FEATURE_VOLUME_CONTROL       | \
                          HF_FEATURE_ENHANCED_CALL_STATUS | \
                          HF_FEATURE_ENHANCED_CALL_CTRL)
#endif /* HF_SDK_FEATURES */ 

/*---------------------------------------------------------------------------
 * HF_TX_BUFFER_SIZE constant
 *     HF_TX_BUFFER_SIZE defines the maximum size of AT command data that 
 *     can be transmitted.  The default is large enough to handle all AT 
 *     commands supported by the Hands-free SDK.  If raw AT commands are 
 *     sent that are larger than the default value, then this number must be 
 *     increased.  
 */
#ifndef HF_TX_BUFFER_SIZE
#define HF_TX_BUFFER_SIZE       32
#endif /* HF_TX_BUFFER_SIZE */ 

/*---------------------------------------------------------------------------
 * HF_RECV_BUFFER_SIZE constant
 *     HF_RECV_BUFFER_SIZE defines the maximum size of AT response data that 
 *     can be received.  The default is large enough to handle all AT 
 *     responses supported by the Hands-Free profile.  
 */
#ifndef HF_RECV_BUFFER_SIZE
#define HF_RECV_BUFFER_SIZE     512
#endif /* HF_RECV_BUFFER_SIZE */ 

/****************************************************************************
 *
 * Types
 *
 ****************************************************************************/

/*--------------------------------------------------------------------------
 * HfChannelStates type
 *
 *     This type enumerates the possible Hands-Free channel connection 
 *     states.  
 */
typedef uint8_t HfChannelStates;

/** The channel is currently closed.  
 */ 
#define HF_STATE_CLOSED        0

/** An outgoing ACL data link is currently being setup.  
 */ 
#define HF_STATE_CONN_PENDING  1

/** An incoming ACL data link is currently being setup.  
 */ 
#define HF_STATE_CONN_INCOMING 2

/** An ACL data link has been established, and the Hands-Free Service Level 
 *  Link parameters are currently being negotiated.  
 */ 
#define HF_STATE_NEGOTIATE     3

/** A Hands-Free channel is currently open.  
 */ 
#define HF_STATE_OPEN          4

/** Closing the link but waiting for the audio link to go down first.  
 */ 
#define HF_STATE_DISC          5

/* End of HfChannelStates */ 

/*---------------------------------------------------------------------------
 * HfEvent type
 *
 *     All indications and confirmations are sent through a callback 
 *     function.  Depending on the event, different elements of 
 *     the HfCallbackParms "HfCallbackParms.p" union will be valid.  
 */
typedef uint16_t HfEvent;

/** An incoming service level connection is being established.  This happens 
 *  when the audio gateway establishes the service connection.  
 *  The data connection is not available yet for issuing commands to the 
 *  audio gateway.  When the HF_EVENT_SERVICE_CONNECTED event is received, 
 *  the channel is available for issuing commands.  
 *
 *  When this callback is received, the "HfCallbackParms.p.remDev" field 
 *  contains a pointer to the remote device context.  
 */
#define HF_EVENT_SERVICE_CONNECT_REQ   0

/** A service level connection has been established.  This can happen as the 
 *  result of a call to HF_CreateServiceLink, or if the audio gateway 
 *  establishes the service connection.  When this event has been received, 
 *  a data connection is available for issuing commands to the audio 
 *  gateway.  
 *
 *  When this callback is received, the "HfCallbackParms.p.remDev" field 
 *  contains a pointer to the remote device context.  
 */
#define HF_EVENT_SERVICE_CONNECTED      1

/** The service level connection has been released.  This can happen as the 
 *  result of a call to HF_DisconnectServiceLink, or if the audio gateway 
 *  releases the service connection.  Communication with the audio gateways 
 *  is no longer possible.  In order to communicate with the audio gateway, 
 *  a new service level connection must be established.  
 *
 *  This event can also occur when an attempt to create a service level 
 *  connection (HF_CreateServiceLink) fails.  
 *
 *  When this callback is received, the "HfCallbackParms.p.remDev" 
 *  field contains a pointer to the remote device context.  If the 
 *  "HfCallbackParms.p.remDev" field contains a NULL, then the ACL 
 *  connection attempt failed and a remote device was never established.  
 *  In addition, the "HfCallbackParms.errCode" fields contains the reason 
 *  for disconnect.  
 */
#define HF_EVENT_SERVICE_DISCONNECTED   2

/** An audio connection has been established.  This event occurs whenever 
 *  the audio channel (SCO) comes up, whether it is initiated by the audio 
 *  gateway or the hands-free unit.  
 *
 *  When this callback is received, the "HfCallbackParms.p.remDev" field 
 *  contains a pointer to the remote device context.  
 */
#define HF_EVENT_AUDIO_CONNECTED        3

/** An audio connection has been released.  This event occurs whenever the 
 *  audio channel (SCO) goes down, whether it is terminated by the audio 
 *  gateway or the hands-free unit.  
 *
 *  When this callback is received, the "HfCallbackParms.p.remDev" field 
 *  contains a pointer to the remote device context.  In addition, the 
 *  "HfCallbackParms.errCode" fields contains the reason for disconnect.  
 */
#define HF_EVENT_AUDIO_DISCONNECTED     4
 
/** Only valid if BT_SCO_HCI_DATA is set to XA_ENABLED.  Audio data has been 
 *  received from the remote device.  The data is only valid during the 
 *  callback.  
 *
 *  When this callback is received, the "HfCallbackParms.p.audioData" field 
 *  contains the audio data.  
 */
#define HF_EVENT_AUDIO_DATA             5

/** Only valid if BT_SCO_HCI_DATA is set to XA_ENABLED.  Audio data has been 
 *  sent to the remote device.  This event is received by the application 
 *  when the data sent by HF_SendAudioData has been successfully sent.  
 *
 *  When this callback is received, the "HfCallbackParms.p.audioPacket" 
 *  field contains the result.  
 */
#define HF_EVENT_AUDIO_DATA_SENT        6

/** After the service level connection has been established, this event will 
 *  indicate the features supported on the audio gateway.  
 *
 *  When this callback is received, the "HfCallbackParms.p.features" field 
 *  contains the features (see HfFeatures).  
 */
#define HF_EVENT_GATEWAY_FEATURES       7

/** After the service level connection has been established, this event will 
 *  indicate the hold features (3-Way calling) supported on the audio 
 *  gateway.  
 *
 *  When this callback is received, the "HfCallbackParms.p.holdFeatures" 
 *  field contains the features (see HfGwHoldFeatures).  
 */
#define HF_EVENT_GW_HOLD_FEATURES       8

/** This event is generated only if HF_USE_CALL_MANAGER is defined as 
 *  XA_ENABLED.  After the service level connection has been established, 
 *  this event will indicate the call state of the current connection.  
 *  Whenever the call state changes or the multiparty status changes, this 
 *  event is generated.  
 *
 *  When this callback is received, the "HfCallbackParms.p.callStateParms" 
 *  field contains the current call state (see HfCallStateParms).  
 */ 
#define HF_EVENT_CALL_STATE             9

/** This event is generated only if HF_USE_CALL_MANAGER is defined as 
 *  XA_ENABLED.  The identification of the call has been received from the 
 *  audio gateway.  
 * 
 *  When this callback is received, the "HfCallbackParms.p.callerId" 
 *  field contains a pointer to the ASCII string representation of the 
 *  number (NULL terminated).  
 */ 
#define HF_EVENT_CALLER_ID             10

/** This event is generated only if HF_USE_CALL_MANAGER is defined as 
 *  XA_ENABLED.  The Call Manager has determined the CLCC command is 
 *  supported by the device.  If this event is received, all call state 
 *  information can be considered reliable.  
 */
#define HF_EVENT_CALL_LISTING_ENABLED  11


/** This event is generated only if both HF_USE_CALL_MANAGER and 
 *  HF_USE_RESP_HOLD are defined as XA_ENABLED.  The Call Manager has 
 *  determined that a response and hold feature has changed its 
 *  applicability.  
 *
 *  When this callback is received, the "HfCallbackParms.p.respHoldAppl" 
 *  field contains the current response-hold relevancies.  (See 
 *  HfRespHoldAppl).  
 */
#define HF_EVENT_RESPONSE_HOLD_APPL    12

/** This event is generated only if HF_USE_CALL_MANAGER is defined as 
 *  XA_DISABLED.  After the service level connection has been established, 
 *  this event will indicate whether at least one call is active on the 
 *  gateway device.  Whenever all calls are terminated, or when there were 
 *  no calls and a new call is created, this event is generated.  
 *
 *  When this callback is received, the "HfCallbackParms.p.call" field 
 *  contains the current call state (see HfCallActiveState).  
 */ 
#define HF_EVENT_CALL_IND              13

/** This event is generated only if HF_USE_CALL_MANAGER is defined as 
 *  XA_DISABLED.  After the service level connection has been established, 
 *  this event will indicate the current call setup state.  Whenever the 
 *  call setup state changes, this event is generated.  
 *
 *  When this callback is received, the "HfCallbackParms.p.callSetup" field 
 *  contains the current call setup state (see HfCallSetupState).
 */ 
#define HF_EVENT_CALLSETUP_IND         14

/** This event is generated only if HF_USE_CALL_MANAGER is defined as 
 *  XA_DISABLED.  After the service level connection has been established, 
 *  this event will indicate the current call held state.  Whenever the held 
 *  state changes, this event is generated.  
 *
 *  When this callback is received, the "HfCallbackParms.p.callHeld" field 
 *  contains the current call held state (see HfCallHeldState).  
 */ 
#define HF_EVENT_CALLHELD_IND          15

/** This event is generated only if HF_USE_CALL_MANAGER is defined as 
 *  XA_DISABLED.  When an incoming call is received on the audio gateway, 
 *  this event is generated to indicate the incoming ring.  
 */
#define HF_EVENT_RING_IND              16

/** This event is generated only if HF_USE_CALL_MANAGER is defined as 
 *  XA_DISABLED.  When call waiting is supported on the audio gateway and an 
 *  incoming call is received while another call is active, this event is 
 *  received.  
 *
 *  When this callback is received, the "HfCallbackParms.p.callWaitParms" 
 *  field contains information about the waiting call (see 
 *  HfCallWaitParms).  
 */ 
#define HF_EVENT_WAIT_NOTIFY           17

/** This event is generated only if HF_USE_CALL_MANAGER is defined as 
 *  XA_DISABLED.  If caller ID notification is active, this event is 
 *  received when an incoming call is detected on the audio gateway.  
 *
 *  When this callback is received, the "HfCallbackParms.p.callerIdParms" 
 *  field contains the current caller ID information (see HfCallerIdParms).  
 */ 
#define HF_EVENT_CALLER_ID_NOTIFY      18

/** This event is generated only if HF_USE_CALL_MANAGER is defined as 
 *  XA_DISABLED.  This event is received once for each call which exists on 
 *  the audio gateway.  This event is received after calling 
 *  HF_ListCurrentCalls().  
 *
 *  When this callback is received, the "HfCallbackParms.p.callListParms" 
 *  field contains the current caller ID information (see HfCallListParms).  
 */                                     
#define HF_EVENT_CURRENT_CALL_STATE    19

/** This event is only available when HF_USE_RESP_HOLD is set to XA_ENABLED 
 *  and HF_USE_CALL_MANAGER is set to XA_DISABLED.  The Response and Hold 
 *  state has been received from the audio gateway.  This event is generated 
 *  in response to a call to HF_QueryResponseHold() or HF_ResponseHold().  
 *
 *  When this callback is received, the "HfCallbackParms.p.respHold" field 
 *  contains the result.  
 */
#define HF_EVENT_RESPONSE_HOLD         20
/** The service indicator has been received from the audio gateway.  
 *
 *  When this callback is received, the "HfCallbackParms.p.service" 
 *  field contains a pointer to the service state.  
 */ 
#define HF_EVENT_SERVICE_IND           21

/** The battery indicator has been received from the audio gateway.  
 *
 *  When this callback is received, the "HfCallbackParms.p.battery" 
 *  field contains a pointer to the battery level.  
 */ 
#define HF_EVENT_BATTERY_IND           22

/** The signal strength indicator has been received from the audio gateway.  
 *
 *  When this callback is received, the "HfCallbackParms.p.signal" 
 *  field contains a pointer to the signal strength.  
 */ 
#define HF_EVENT_SIGNAL_IND            23

/** The roam indicator has been received from the audio gateway.  
 *
 *  When this callback is received, the "HfCallbackParms.p.roam" 
 *  field contains a pointer to the roam state.  
 */ 
#define HF_EVENT_ROAM_IND              24

/** The voice recognition state has changed.  This event occurs if the 
 *  audio gateway changes the state of voice recognition.  
 *
 *  When this callback is received, the "HfCallbackParms.p.voiceRecognition" 
 *  field contains state of voice recognition.  
 */
#define HF_EVENT_VOICE_REC_STATE       26

/** A number was returned in response to the HF_GetLastVoiceTag function.  
 *
 *  When this callback is received, the "HfCallbackParms.p.voiceTag" field 
 *  contains a pointer to the ASCII string representation of the number 
 *  (NULL terminated).  
 */
#define HF_EVENT_VOICE_TAG_NUMBER      27

/** The speaker volume has been received from the audio gateway.  
 */
#define HF_EVENT_SPEAKER_VOLUME        28

/** The microphone volume has been received from the audio gateway.  
 */
#define HF_EVENT_MIC_VOLUME            29

/** The in-band ring tone setting has been received from the audio gateway.  
 *
 *  When this callback is received, the "HfCallbackParms.p.inBandRing" 
 *  field contains a pointer to the In-Band ring state.  
 */ 
#define HF_EVENT_IN_BAND_RING          30

/** The network operator string has been received from the remote device.  
 *
 *  When this callback is received, the "HfCallbackParms.p.networkOper" 
 *  field contains a pointer to the operator string state.  
 */ 
#define HF_EVENT_NETWORK_OPERATOR      31

/** The subscriber number has been received from the audio gateway.  
 *
 *  When this callback is received, the "HfCallbackParms.p.subscriberNum" 
 *  field contains a pointer to the subscriber number.  
 */ 
#define HF_EVENT_SUBSCRIBER_NUMBER     32

/** The NO CARRIER event has been received from the audio gateway.  If 
 *  HF_USE_CALL_MANAGER is defined as XA_ENABLED, then the call manager will 
 *  have already modified the call states accordingly, so this event is for 
 *  informational purposes only and the application need take no further 
 *  action.  If HF_USE_CALL_MANAGER is defined as XA_DISABLED, then the 
 *  application will need to make the appropriate call state changes 
 *  itself.  
 */
#define HF_EVENT_NO_CARRIER            33

/** The BUSY event has been received from the audio gateway.  If 
 *  HF_USE_CALL_MANAGER is defined as XA_ENABLED, then the call manager will 
 *  have already modified the call states accordingly, so this event is for 
 *  informational purposes only and the application need take no further 
 *  action.  If HF_USE_CALL_MANAGER is defined as XA_DISABLED, then the 
 *  application will need to make the appropriate call state changes 
 *  itself.  
 */
#define HF_EVENT_BUSY                  34

/** The NO ANSWER event has been received from the audio gateway.  If 
 *  HF_USE_CALL_MANAGER is defined as XA_ENABLED, then the call manager will 
 *  have already modified the call states accordingly, so this event is for 
 *  informational purposes only and the application need take no further 
 *  action.  If HF_USE_CALL_MANAGER is defined as XA_DISABLED, then the 
 *  application will need to make the appropriate call state changes 
 *  itself.  
 */
#define HF_EVENT_NO_ANSWER             35

/** The DELAYED event has been received from the audio gateway.  
 */
#define HF_EVENT_DELAYED               36

/** The BLACKLISTED event has been received from the audio gateway.  
 */
#define HF_EVENT_BLACKLISTED           37

/** The supported phonebook storage types have been received from the audio 
 *  gateway.  This event occurs in response to a call to 
 *  HF_QueryPhonebooks.  
 *
 *  When this callback is received, the "HfCallbackParms.p.phonebooks" 
 *  field contains a bitmask of phonebook type supported on the gateway 
 *  (see HfPhonebooks).  
 */ 
#define HF_EVENT_PHONEBOOK_STORAGE     38

/** The phonebook storage information has been received from the audio 
 *  gateway.  This event occurs in response to a call to 
 *  HF_GetCurrentPhonebooksInfo.  
 *
 *  When this callback is received, the "HfCallbackParms.p.phonebookInfo" 
 *  field contains a pointer to a structure containing the phonebook 
 *  information.  
 */ 
#define HF_EVENT_PHONEBOOK_INFO        39

/** The number of phonebook entries has been received from the audio 
 *  gateway.  This event occurs in response to a call to 
 *  HF_GetPhonebookSize.  
 *
 *  When this callback is received, the "HfCallbackParms.p.phonebookSize" 
 *  field contains a pointer to a structure containing the phonebook 
 *  information.  
 */ 
#define HF_EVENT_PHONEBOOK_SIZE        40

/** A phonebook entry has been received from the audio gateway.  This event 
 *  occurs in response to a call to HF_ReadPhonebookEntries or 
 *  HF_FindPhonebookEntries.  
 *
 *  When this callback is received, the "HfCallbackParms.p.phonebookEntry" 
 *  field contains a pointer to a structure containing the phonebook 
 *  information.  
 */ 
#define HF_EVENT_PHONEBOOK_ENTRY       41

/** A result code has been received from the audio gateway.  This event is 
 *  received for unsolicited result codes not handled by the internal 
 *  Hands-free AT parser.  
 *
 *  When this callback is received, the "HfCallbackParms.p.data" field 
 *  contains the AT result code.  
 */
#define HF_EVENT_AT_RESULT_DATA        42

/** A command to the audio gateway has completed.  This event is received 
 *  when the processing a command is complete.  
 *
 *  When this callback is received, the "HfCallbackParms.p.command" field 
 *  contains the command that was sent.  If "HfCallbackParms.status is set 
 *  to BT_STATUS_FAILED, then "HfCallbackParms.p.command->cmeError" contains 
 *  the command error (if known).  
 */
#define HF_EVENT_COMMAND_COMPLETE      43

/** The codec negotiation has been received from the audio gateway.  
 */
#define HF_EVENT_CODEC_NEGOTIATION      44

/* End of HfEvent */ 

/*--------------------------------------------------------------------------
 * HfCommandType type
 *
 *     HfCommandType corresponds to a logical command.  This value is 
 *     returned in the "HfCallbackParms.p.command" structure with the 
 *     HF_EVENT_COMMAND_COMPLETE event.  It indicates the API call 
 *     associated with this command structure.  Any API call that takes an 
 *     HfCommand parameter receives the HF_EVENT_COMMAND_COMPLETE event when 
 *     the command has completed.  The "HfCallbackParms.p.command->type" 
 *     field identifies the API call that was made.  
 */
typedef uint8_t HfCommandType;

/* HF_AnswerCall */ 
#define HF_COMMAND_ANSWER_CALL             0

/* HF_DialNumber */ 
#define HF_COMMAND_DIAL_NUMBER             1

/* HF_MemoryDial */ 
#define HF_COMMAND_DIAL_MEMORY             2

/* HF_Redial */ 
#define HF_COMMAND_REDIAL                  3

/* HF_CallHold */ 
#define HF_COMMAND_CALL_HOLD               4

/* HF_QueryResponseHold */ 
#define HF_COMMAND_QUERY_RESPONSE_HOLD     5

/* HF_ResponseHold */ 
#define HF_COMMAND_RESPONSE_HOLD           6

/* HF_Hangup */ 
#define HF_COMMAND_HANGUP_CALL             7

/* HF_ListCurrentCalls */ 
#define HF_COMMAND_LIST_CURRENT_CALLS      8

/* HF_EnableCallerIdNotify */ 
#define HF_COMMAND_ENABLE_CID_NOTIFY       9

/* HF_EnableCallWaitNotify */ 
#define HF_COMMAND_ENABLE_WAIT_NOTIFY      10

/* HF_GenerateDtmf() */ 
#define HF_COMMAND_GENERATE_DTMF           11

/* HF_GetLastVoiceTag */ 
#define HF_COMMAND_GET_LAST_VOICE_TAG      12

/* HF_EnableVoiceRecognition */ 
#define HF_COMMAND_VOICE_RECOGNITION       13

/* HF_DisableNREC */ 
#define HF_COMMAND_DISABLE_NREC            14

/* HF_ReportMicVolume */ 
#define HF_COMMAND_REPORT_MIC_VOLUME       15

/* HF_ReportSpeakerVolume */ 
#define HF_COMMAND_REPORT_SPEAKER_VOLUME   16

/* HF_QueryNetworkOperator */ 
#define HF_COMMAND_QUERY_NETWORK_OPER      17

/* HF_QuerySubscriberNumber */ 
#define HF_COMMAND_QUERY_SUBSCRIBER_NUM    18

/* HF_EnableExtendedErrors */ 
#define HF_COMMAND_ENABLE_EXTENDED_ERR     19

/* HF_SendAtCommand */ 
#define HF_COMMAND_SEND_AT_COMMAND         20

/* HF_QueryPhonebooks */ 
#define HF_COMMAND_QUERY_PB                21

/* HF_SelectPhonebook */ 
#define HF_COMMAND_SELECT_PB               22

/* HF_GetCurrentPhonebookInfo */ 
#define HF_COMMAND_GET_CURRENT_PB_INFO     23

/* HF_GetPhonebookSize */ 
#define HF_COMMAND_GET_PB_SIZE             24

/* HF_ReadPhonebookEntries */ 
#define HF_COMMAND_READ_PB_ENTRIES         25

/* HF_FindPhonebookEntries */ 
#define HF_COMMAND_FIND_PB_ENTRIES         26

/* HF_WritePhonebookEntry */ 
#define HF_COMMAND_WRITE_PB_ENTRY          27

#define HF_COMMAND_IIA                     28

/* HF_CodecConnectionSetup */
#define HF_COMMAND_BCS                     29

/* End of HfCommandType */ 

/*--------------------------------------------------------------------------
 * HfCmeError type
 *
 *     HfCmeError corresponds to an AT error indication.  
 */
typedef uint8_t HfCmeError;

#define AT_CME_NO_CONNECTION                 ATCME_NO_CONNECTION
#define AT_CME_OP_NOT_ALLOWED                ATCME_OP_NOT_ALLOWED
#define AT_CME_OP_NOT_SUPPORTED              ATCME_OP_NOT_SUPPORTED
#define AT_CME_PH_SIM_PIN_REQUIRED           ATCME_PH_SIM_PIN_REQUIRED
#define AT_CME_SIM_NOT_INSERTED              ATCME_SIM_NOT_INSERTED
#define AT_CME_SIM_PIN_REQUIRED              ATCME_SIM_PIN_REQUIRED
#define AT_CME_SIM_PUK_REQUIRED              ATCME_SIM_PUK_REQUIRED
#define AT_CME_SIM_FAILURE                   ATCME_SIM_FAILURE
#define AT_CME_SIM_BUSY                      ATCME_SIM_BUSY
#define AT_CME_INCORRECT_PASSWORD            ATCME_INCORRECT_PASSWORD
#define AT_CME_SIM_PIN2_REQUIRED             ATCME_SIM_PIN2_REQUIRED
#define AT_CME_SIM_PUK2_REQUIRED             ATCME_SIM_PUK2_REQUIRED
#define AT_CME_MEMORY_FULL                   ATCME_MEMORY_FULL
#define AT_CME_INVALID_INDEX                 ATCME_INVALID_INDEX
#define AT_CME_MEMORY_FAILURE                ATCME_MEMORY_FAILURE
#define AT_CME_TEXT_STRING_TOO_LONG          ATCME_TEXT_STRING_TOO_LONG
#define AT_CME_INVALID_CHARS_IN_TEXT_STRING  ATCME_INVALID_CHARS_IN_TEXT_STRING
#define AT_CME_DIAL_STRING_TOO_LONG          ATCME_DIAL_STRING_TOO_LONG
#define AT_CME_INVALID_CHARS_IN_DIAL_STRING  ATCME_INVALID_CHARS_IN_DIAL_STRING
#define AT_CME_NO_NETWORK_SERVICE            ATCME_NO_NETWORK_SERVICE
#define AT_CME_NETWORK_NOT_ALLOWED           ATCME_NETWORK_NOT_ALLOWED
#define AT_CME_UNKNOWN                       ATCME_UNKNOWN

/* End of HfCmeError */ 

/*--------------------------------------------------------------------------
 * HfGatewayVersion type
 *
 *     HfGatewayVersion corresponds to the Audio Gateway Profile version 
 *     discovered during the SDP query.  The service connection features 
 *     will be limited to the capabilities of this profile version.  
 */
typedef uint16_t  HfGatewayVersion;

/* Unable to determine the Hands Free Profile version that is supported */ 
#define HF_GW_VERSION_UNKNOWN 0x0000

/* Supports Version 0.96 of the Hands Free Profile */ 
#define HF_GW_VERSION_0_96    0x0100

/* Supports Version 1.0 of the Hands Free Profile */ 
#define HF_GW_VERSION_1_0     0x0101

/* Supports Version 1.5 of the Hands Free Profile */ 
#define HF_GW_VERSION_1_5     0x0105

/* Supports Version 1.5 of the Hands Free Profile */ 
#define HF_GW_VERSION_1_6     0x0106

/* Supports Version 1.5 of the Hands Free Profile */ 
#define HF_GW_VERSION_1_8     0x0108

/* End of HfGatewayVersion */ 

/*---------------------------------------------------------------------------
 * HfGatewayFeatures type
 *      
 *  HfGatewayFeatures is a bit mask specifying the gateway feature set.  The 
 *  service connection capabilities will be limited to the features 
 *  advertised by the profile.  
 */
typedef uint32_t HfGatewayFeatures;

/** 3-way calling
 */
#define HF_GW_FEATURE_3_WAY              0x00000001

/** Echo canceling and/or noise reduction function
 */
#define HF_GW_FEATURE_ECHO_NOISE         0x00000002

/** Voice recognition function
 */
#define HF_GW_FEATURE_VOICE_RECOGNITION  0x00000004

/** In-band ring tone
 */
#define HF_GW_FEATURE_IN_BAND_RING       0x00000008

/** Voice tag
 */
#define HF_GW_FEATURE_VOICE_TAG          0x00000010

/** Reject a call
 */
#define HF_GW_FEATURE_CALL_REJECT        0x00000020

/** Enhanced Call Status
 */
#define HF_GW_FEATURE_ENH_CALL_STATUS    0x00000040

/** Enhanced Call Control
 */
#define HF_GW_FEATURE_ENH_CALL_CTRL      0x00000080

/* End of HfGatewayFeatures */ 

/*---------------------------------------------------------------------------
 * HfGwHoldFeatures type
 *
 *  This type is used as a bit mask specifying the gateway's 3-Way calling 
 *  (hold) feature set.  The service connection capabilities will be limited 
 *  to the features advertised by the profile.  
 */
typedef uint8_t HfGwHoldFeatures;

/** Releases all held calls or sets User Determined User Busy 
 * (UDUB) for a waiting call.  
 */
#define HF_GW_HOLD_RELEASE_HELD_CALLS      0x01

/** Releases all active calls (if any exist) and accepts the other 
 * (held or waiting) call.  
 */
#define HF_GW_HOLD_RELEASE_ACTIVE_CALLS    0x02

/** Releases a specific call. */ 
#define HF_GW_HOLD_RELEASE_SPECIFIC_CALL   0x04

/** Places all active calls (if any exist) on hold and accepts the 
 * other (held or waiting) call.  
 */
#define HF_GW_HOLD_HOLD_ACTIVE_CALLS       0x08

/** Places a specific call on hold. */ 
#define HF_GW_HOLD_HOLD_SPECIFIC_CALL      0x10

/** Adds a held call to the conversation.  
 */
#define HF_GW_HOLD_ADD_HELD_CALL           0x20

/** Connects the two calls and disconnects the AG from 
 *  both calls (Explicit Call Transfer).  
 */
#define HF_GW_HOLD_CALL_TRANSFER           0x40

/* End of HfGwHoldFeatures */ 

/*---------------------------------------------------------------------------
 * HfSignalStrength type
 *
 *  HfSignalStrength contains the last signal strength reading from the 
 *  Audio Gateway.  
 */
typedef uint8_t HfSignalStrength;

/* End of HfSignalStrength */ 

/* Forward references */ 
typedef struct _HfCallbackParms HfCallbackParms;
typedef struct _HfChannel HfChannel;
typedef struct _HfCommand HfCommand;

/*---------------------------------------------------------------------------
 * HfCallback type
 *
 * A function of this type is called to indicate events to the application.  
 */
typedef void (*HfCallback)(HfChannel *Chan, HfCallbackParms *Info);

/* End of HfCallback */ 

/*---------------------------------------------------------------------------
 * HfCmdOverride type
 *
 * A function of this type is called when a function override is 
 * registered.  
 */
typedef BtStatus (*HfCmdOverride)(HfChannel *Chan, HfCommand *Command);

/* End of HfCmdOverride */ 

/*---------------------------------------------------------------------------
 * HfCallStatus type
 *
 * HfCallStatus defines the current state of a call.  Not all states are 
 * supported by all Audio Gateways.  At the very minimum, 
 * HF_CALL_STATUS_NONE, HF_CALL_STATUS_DIALING, HF_CALL_STATUS_INCOMING, and 
 * HF_CALL_STATUS_ACTIVE will be supported.  
 */
typedef uint8_t HfCallStatus;

/** An active call exists.  
 */
#define HF_CALL_STATUS_ACTIVE     0

/** The call is held.  
 */
#define HF_CALL_STATUS_HELD       1

/** A call is outgoing.  This state occurs when attempting a call using any 
 *  of the dialing functions.  
 */
#define HF_CALL_STATUS_DIALING    2

/** The remote party is being alerted.  
 */
#define HF_CALL_STATUS_ALERTING   3

/** A call is incoming.  It can be answered by invoking HFG_AnswerCall() or 
 *  rejected by invoking HFG_Hangup().  This state occurs when a call is 
 *  being set up by a remote party, and there is currently no 
 *  established call.  
 */
#define HF_CALL_STATUS_INCOMING   4

/** A call is waiting.  It can be answered by invoking HFG_AnswerCall() or 
 *  rejected by invoking HFG_Hangup().  This state occurs when a call is 
 *  being set up by a remote party, and there is currently an 
 *  established call.  
 */
#define HF_CALL_STATUS_WAITING    5

/** A call is in the response and hold state.  It can be accepted or 
 *  rejected by invoking HF_ResponseHold().  This can be thought of as a 
 *  special muted "active call" state.  This state is not used by most 
 *  networks, but it can occur in Japanese networks.  See the Hands Free 
 *  Specification Errata 2716, which adds this new call state to the CLCC 
 *  messages.  
 */ 
#define HF_CALL_STATUS_RESPHOLD   6

/** No active call
 */
#define HF_CALL_STATUS_NONE       0xFF

/** Unknown call state
 */
#define HF_CALL_STATUS_UNKNOWN    0xFE

/* End of HfCallStatus */ 

/*---------------------------------------------------------------------------
 * HfCallMode type
 *
 * HfCallMode defines the current mode of a call.  It is only meaningful if 
 * HF_USE_CALL_MANAGER is defined as XA_DISABLED.  
 */
typedef uint8_t HfCallMode;

/** Voice Call 
  */ 
#define HF_CALL_MODE_VOICE     0

/** Data Call 
 */ 
#define HF_CALL_MODE_DATA      1

/** FAX Call 
 */ 
#define HF_CALL_MODE_FAX       2

/* End of HfCallMode */ 

/*---------------------------------------------------------------------------
 * HfCallActiveState type
 *
 * HfCallActiveState enumerates the possible current call states that can be 
 * indicated by the Audio Gateway.  
 */
typedef uint8_t HfCallActiveState;

/** No call exists on the Audio Gateway 
 */ 
#define HF_CALL_NONE           0

/** A call is active on the Audio Gateway 
  */ 
#define HF_CALL_ACTIVE         1

/* End of HfCallActiveState */ 

/*---------------------------------------------------------------------------
 * HfCallHeldState type
 *
 * HfCallHeldState enumerates the possible current held state that can be 
 * indicated by the audio gateway.  
 */
typedef uint8_t HfCallHeldState;

/** No calls are held on the audio gateway 
 */ 
#define HF_CALL_HELD_NONE      0

/** A call is held and another call is active on the audio gateway.  This 
 *  indication can be sent for several reasons, including when an active and 
 *  held call are swapped.  
 */ 
#define HF_CALL_HELD_ACTIVE    1

/** A call is held and no active call exists on the audio gateway 
 */ 
#define HF_CALL_HELD_NO_ACTIVE 2

/* End of HfCallHeldState */ 

/*---------------------------------------------------------------------------
 * HfCallSetupState type
 *
 * HfCallSetupState enumerates the possible current call setup state that 
 * can be indicated by the Audio Gateway.  
 */
typedef uint8_t HfCallSetupState;

/** No outgoing or incoming call is present on the Audio Gateway 
 */ 
#define HF_CALL_SETUP_NONE     0

/** An incoming call is present on the Audio Gateway 
 */ 
#define HF_CALL_SETUP_IN       1

/** An outgoing call is present on the Audio Gateway 
 */ 
#define HF_CALL_SETUP_OUT      2

/** An outgoing call is being alerted on the Audio Gateway 
 */ 
#define HF_CALL_SETUP_ALERT    3

/* End of HfCallSetupState */ 

/*---------------------------------------------------------------------------
 * HfHoldAction type
 *
 * HfHoldAction enumerates the possible actions that can be taken when 
 * calling the HF_CallHold() function.
 */
typedef uint8_t HfHoldAction;

/** Indicates that the code should release all held calls, or set the User 
 *  Determined User Busy (UDUB) indication for a waiting call.  
 */
#define HF_HOLD_RELEASE_HELD_CALLS   0

/** Indicates that the code should release all active calls (if any exist) 
 *  and accepts the other (held or waiting) call.  
 *
 *  If a call index is specified, the code should release the specific call.  
 */
#define HF_HOLD_RELEASE_ACTIVE_CALLS 1

/** Indicates that the code should place all active calls (if any exist) on 
 *  hold and accepts the other (held or waiting) call.  
 *
 *  If a call index is specified, the code should put all calls on hold 
 *  except the specified call.  
 */
#define HF_HOLD_HOLD_ACTIVE_CALLS    2

/** Indicates that the code should add a held call to the conversation.  
 */
#define HF_HOLD_ADD_HELD_CALL        3

/** Indicates that the code should connects the two calls and disconnect the 
 *  Audio Gateway from both calls.  In other words, the code should perform 
 *  an Explicit Call Transfer.  
 */
#define HF_HOLD_CALL_TRANSFER        4

/* End of HfHoldAction */ 

/*---------------------------------------------------------------------------
 * HfRespHoldAction type
 *
 * HfRespHoldAction enumerates the possible actions that can be taken when 
 * calling the HF_ResponseHold() function.
 */
typedef uint8_t HfRespHoldAction;

/** This action is used to request that an incoming call be put on hold.  
 */
#define HF_RH_HOLD   0

/** This action is used to request that a call that was previously put on 
 *  hold by a response and hold command now be activated.  
 */
#define HF_RH_ACCEPT 1

/** This action is used to request that a call that was previously put on 
 *  hold by a response and hold command now be dropped.  
 */
#define HF_RH_REJECT 2

/* End of HfRespHoldAction */ 

/*---------------------------------------------------------------------------
 * HfPhonebooks type
 *
 * This type is only available when HF_USE_PHONEBOOK_COMMANDS is set to 
 * XA_ENABLED.  It enumerates the possible types of phone books supported in 
 * an Audio Gateway.  
 */
typedef uint16_t HfPhonebooks;

/** GW dialed calls list */ 
#define HF_PB_DIALED_CALLS          AT_PBS_DIALED_CALLS

/** SIM fixed-dialing-phonebook list */ 
#define HF_PB_FIXED_DIAL            AT_PBS_FIXED_DIAL

/** SIM last-dialing-phonebook list */ 
#define HF_PB_LAST_DIAL             AT_PBS_LAST_DIAL

/** GW missed calls list */ 
#define HF_PB_MISSED_CALLS          AT_PBS_MISSED_CALLS

/** GW phonebook list */ 
#define HF_PB_PHONE                 AT_PBS_ME_PHONEBOOK

/** Combined GW and SIM phonebook list */ 
#define HF_PB_COMBINED              AT_PBS_ME_SIM_COMBINED

/** GW received calls list */ 
#define HF_PB_RECEIVED_CALLS        AT_PBS_RECEIVED_CALLS

/** SIM phonebook list */ 
#define HF_PB_SIM                   AT_PBS_SIM_PHONEBOOK

/* End of HfPhonebooks */ 

/*---------------------------------------------------------------------------
 * Poll Interval type
 *
 *     Indicates the interval between CLCC polls.  
 */
typedef uint8_t PollInterval;

/** The polling interval has not been defined.  
 */
#define PL_UNKNOWN 0x00

/** Used to indicate that low rate polling is sufficient because there are 
 *  no calls on either line.  
 */
#define PL_SLOW    0x0F

/** Used to indicate that high rate polling is wanted because there is a 
 *  call on at least one of the lines.  
 */
#define PL_FAST    0xF0

/* End of PollInterval */ 
#if 0
#if BT_SCO_HCI_DATA == XA_ENABLED

/*---------------------------------------------------------------------------
 * HfAudioData type
 *
 * This type is only available when BT_SCO_HCI_DATA is set to XA_ENABLED.  
 * It can be used to store audio data received from the remote device.  
 */
typedef CmgrAudioData HfAudioData;

/* End of HfAudioData */ 

#endif /* BT_SCO_HCI_DATA == XA_ENABLED */ 
#endif

/*---------------------------------------------------------------------------
 * HfCallListState type
 *
 * HfCallListState is used to indicate whether received call list messages 
 * have been formatted correctly or have been mangled.  
 */
typedef uint8_t HfCallListState;

/** No mangled call list messages have been received.
 */
#define HF_CALL_LIST_OK      0x00

/** At least one mangled call list message has been received.
 */
#define HF_CALL_LIST_MANGLED 0x01

/* End of HfCallListState */ 

#if HF_USE_IIA == XA_ENABLED

/*---------------------------------------------------------------------------
 * HfIIReportingState type
 *
 * This type indicates the reporting state for an individual indicator.  
*/
typedef U8 HfIIReportingState;

/** Actively reporting the individual indicator.  */ 
#define HF_IIA_ACTIVE   0

/** Reporting for the individual indicator has been deactivated.  */ 
#define HF_IIA_INACTIVE 1

/** Used for indices that are not applicable because they exceed 
 *  the number of indicators reported by the CIND message.  
 */ 
#define HF_IIA_NA       2

/* End of HfIIReportingState */ 

/*---------------------------------------------------------------------------
 * HfIIRepStatCmd type
 *
 * This type indicates whether the state of an individual indicator should be 
 * actively reported, left unreported, or whether the current reporting state 
 * should not be modified.  
*/
typedef U8 HfIIRepStatCmd;

/** This define is used to request that reporting for the individual 
 *  indicator be activated.  
 */ 
#define HF_IIA_CMD_ACTIVATE    0

/** This define is used to request that reporting for the individual 
 *  indicator be deactivated.  
 */ 
#define HF_IIA_CMD_DEACTIVE    1

/** This define is used to request that reporting for the individual 
 *  indicator be left unchanged.  It is also used for individual activators 
 *  not defined in the CIND message.  
 */ 
#define HF_IIA_CMD_DONT_CHANGE 2

/* End of HfIIRepStatCmd */ 

#endif /* HF_USE_IIA == XA_ENABLED */ 

/****************************************************************************
 *
 * Data Structures
 *
 ****************************************************************************/

#if HF_USE_IIA == XA_ENABLED

/*---------------------------------------------------------------------------
 * HfIIARepElem structure
 *
 * Structures of type HfIIARepElem can be used to store the reporting state 
 * for an individual indicator.  
 */
typedef struct _HfIIARepElem {
    char              *desc;
    HfIIReportingState state;
} HfIIARepElem;

/*---------------------------------------------------------------------------
 * HfIIAReporting structure
 *
 * Structures of type HfIIAReporting can be used to store the reporting 
 * state for the various individual indicators.  
 */
typedef struct _HfIIAReporting {

    HfIIARepElem ii[AT_MAX_INDICATORS];

    /* The number of indicators that were defined in the CIND message.  */ 
    U8           numInd;

} HfIIAReporting;

/*---------------------------------------------------------------------------
 * HfIIACmd structure
 *
 * Structures of type HfIIACmd can be used to store commands for changing 
 * the reporting state of the various individual indicators.  
 */
typedef struct _HfIIACmd {
    HfIIRepStatCmd state[AT_MAX_INDICATORS];
} HfIIACmd;

#endif /* HF_USE_IIA == XA_ENABLED */ 

/*---------------------------------------------------------------------------
 * HfCommand structure
 *
 * Structures of type HfCommand can be used to store the command type and 
 * parameters for sending Hands-Free SDK commands.  
 */
struct _HfCommand {

    /* Used Internally by the Hands-free SDK */ 
    ListEntry   node;

    /* The type of command */ 
    uint8_t          type;

    /* The command parameters */ 
    uint32_t         parms[6];

    /* The command status */ 
    BtStatus    status;

    /* CME Error when command fails */ 
    HfCmeError  cmeError;

    /* Application context */ 
    void       *context;

    /* === Internal use only === */ 

    uint8_t     state;

#if HF_USE_CALL_MANAGER == XA_ENABLED
    
    U16         callMgrFlags;
    
#endif /* HF_USE_CALL_MANAGER == XA_ENABLED */ 

};

/*---------------------------------------------------------------------------
 * HfAtData structure
 *
 *     Structures of type HfAtData can be used to store raw AT data.  
 */
typedef struct _HfAtData {
    uint8_t         *data;
    uint16_t         dataLen;
} HfAtData;

/*---------------------------------------------------------------------------
 * HfCallWaitParms structure
 *
 *     This type is defined only if HF_USE_CALL_MANAGER is defined as 
 *     XA_DISABLED.  It is used to identify the waiting call.  
 */
typedef struct _HfCallWaitParms {

    /* Phone number of the waiting call */ 
    const char     *number;

    /* Voice parameters */ 
    uint8_t         classmap;

    /* Type of address */ 
    uint8_t         type;

} HfCallWaitParms;

/*---------------------------------------------------------------------------
 * HfCallerIdParms structure
 *
 *     This type is defined only if HF_USE_CALL_MANAGER is defined as 
 *     XA_DISABLED.  It is used to identify the calling number.  
 */
typedef struct _HfCallerIdParms {

    /* Phone number of the caller */ 
    const char     *number;

    /* Type of address */ 
    uint8_t         type;

} HfCallerIdParms;

/*---------------------------------------------------------------------------
 * HfCallListParms structure
 *
 *     This type is defined only if HF_USE_CALL_MANAGER is defined as 
 *     XA_DISABLED.  It is used to identify the listed calls on the Audio 
 *     Gateway.  
 */
typedef struct _HfCallListParms {

    /* Index of the call on the audio gateway (1 based). */ 
    uint8_t        index;

    /* 0 - Mobile Originated, 1 = Mobile Terminated */ 
    uint8_t        dir;

    /* Call state (see HfCallState). */ 
    HfCallStatus   state;

    /* Call mode (see HfCallMode). */ 
    HfCallMode      mode;

    /* 0 - Not Multiparty, 1 - Multiparty */ 
    uint8_t         multiParty;
    
    /* Phone number of the call */ 
    const char     *number;

    /* Type of address */ 
    uint8_t         type;

} HfCallListParms;

/*---------------------------------------------------------------------------
 * HfSubscriberNum structure
 *
 *     Structures of type HfSubscriberNum can be used to identify the 
 *     subscriber number.
 */
typedef struct _HfSubscriberNum {

    /* String phone number of format specified by "type". */ 
    const char *number;

    /* Phone number format */ 
    uint8_t     type;

    /* Service related to the phone number. */ 
    uint8_t     service;

} HfSubscriberNum;

/*---------------------------------------------------------------------------
 * AtRaw structure
 *
 *     Defines the structure containing raw AT data.  
 */
typedef struct _AtRaw {
    uint8_t *str;
    uint8_t  len;
} AtRaw;

#if AT_HEADSET == XA_ENABLED

/*---------------------------------------------------------------------------
 * AtHeadsetCmd
 * 
 *     This structure is used to specify the parameters associated with 
 *     Headset commands. Headset commands are sent from the Headset device 
 *     to the Audio Gateway.  As such, this structure is used by the Headset 
 *     device encoder, and the Audio Gateway decoder functions.  
 */
typedef union _AtHeadsetCmd {

    /* AT_MICROPHONE_GAIN */ 
    struct {
        uint8_t     gain;
    } mic;

    /* AT_SPEAKER_GAIN */ 
    struct {
        uint8_t     gain;
    } speaker;

    /* AT_KEYPAD_CONTROL */ 
    struct {
        uint8_t     button;
    } keypad;

} AtHeadsetCmd;

#else /* AT_HEADSET == XA_ENABLED */ 

/* Stub structures to keep #defines out 
 * of AtResults/AtCommands structures. 
 */ 
typedef uint8_t AtHeadsetCmd;

#endif /* AT_HEADSET == XA_ENABLED */ 

#if AT_HANDSFREE == XA_ENABLED

/*---------------------------------------------------------------------------
 * AtHandsfreeCmd
 * 
 *     This structure is used to specify the parameters associated with 
 *     Hands-Free commands.  Hands-Free commands are sent from the 
 *     Hands-Free unit to the Audio Gateway.  As such, this structure is 
 *     used by the Hands-Free unit encoder, and the Audio Gateway decoder 
 *     functions.  
 */
typedef union _AtHandsfreeCmd {

    /* AT_DIAL_NUMBER, AT_DIAL_MEMORY */ 
    struct {
        const char     *number;
    } dial;

    /* AT_CALL_WAIT_NOTIFY */ 
    struct {
        /* Enable/Disable the presentation of the AT_CALL_WAIT_NOTIFY
         * unsolicited result code.  
         */
        uint8_t         notify;
    } wait;

    /* AT_CALL_HOLD */ 
    struct {

        /* Call hold procedure to perform. */ 
        uint8_t         action;

        /* If "action" is AT_HOLD_RELEASE_ACTIVE_CALLS or 
         * AT_HOLD_HOLD_ACTIVE_CALLS, this value can be used to specify 
         * the index (1 - 9) of a specific call to address.  
         */
        uint8_t         call;

    } hold;

    /* AT_CALL_ID */ 
    struct {
        int32_t         enabled;
    } callId;

    /* AT_EVENT_REPORTING */ 
    struct {
        uint8_t         mode;
        uint8_t         ind;
    } report;

    /* AT_GENERATE_DTMF_TONE */ 
    struct {
        uint8_t         tone;
    } dtmf;

    /* AT_VOICE_RECOGNITION */ 
    struct {
        int             enabled;
    } vrec;

    struct {
        uint32_t        bitmap;
    } features;

    /* AT_RESPONSE_AND_HOLD */ 
    struct {
        uint8_t         setting;
    } btrh;

    /* AT_NETWORK_OPERATOR */ 
    struct {

        /* 0 = automatic, 1 = manual, 2 = deregister, 3 = set format only, 
         * 4 = manual/automatic.  
         */
        uint8_t         mode;

        /* Format of "oper" parameter (0-3) */ 
        uint8_t         format;

    } networkOper;

    /* AT_IIA */ 
    struct {

        /* A string of up to 20 zeroes and ones separated by commas.  One 
         * means activate, and zero means deactivate.  The zeroes and ones 
         * can be left out for individual indicators whose reporting state 
         * should not change.  
         */ 
        const char     *activationStr;

    } iia;

    /* AT_BCS */ 
    struct {

        /* A string of supported codec.  
         */ 
        const char     *supportedCodec;

    } bcs;

    /* AT_BAC */ 
    struct {

        /* A string of supported codec.  
         */ 
        const char     *supportedCodec;

    } bac;

} AtHandsfreeCmd;

#else /* AT_HANDSFREE == XA_ENABLED */ 

/* Stub structures to keep #defines out 
 * of AtResults/AtCommands structures. 
 */ 
typedef uint8_t AtHandsfreeCmd;

#endif /* AT_HANDSFREE == XA_ENABLED */

#if AT_PHONEBOOK == XA_ENABLED

/*---------------------------------------------------------------------------
 * AtPhonebookCmd
 *
 */
typedef union _AtPhonebookCmd {

    /* AT_SELECT_PHONEBOOK_STORAGE */ 
    struct {

        /* Phonebook storage type to select. */ 
        uint16_t            select;

    } storage;

    /* AT_READ_PHONEBOOK_ENTRY */ 
    struct {

        /* First entry to return. */ 
        uint16_t            first;

        /* Last entry to return. To return only one entry, 
         * set last = first.  
         */
        uint16_t            last;

    } read;

    /* AT_FIND_PHONEBOOK_ENTRY */ 
    struct {

        /* Start text to search for. */ 
        const char         *text;

    } find;

    /* AT_WRITE_PHONEBOOK_ENTRY */ 
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

    } write;

} AtPhonebookCmd;

#else /* AT_PHONEBOOK == XA_ENABLED */ 

/* Stub structures to keep #defines out 
 * of AtResults/AtCommands structures.  
 */
typedef uint8_t AtPhonebookCmd;

#endif /* AT_PHONEBOOK == XA_ENABLED */

#if AT_SMS == XA_ENABLED

/*---------------------------------------------------------------------------
 * 
 *
 */
typedef union _AtSmsCmd {
    struct {

        /* Service type: 0=GSM 7.05 Phase 2, 1=GSM 7.05 Phase 2+ */ 
        uint8_t        type;

    } service;

    struct {

        /* Memory from which messages are read and deleted. */ 
        uint8_t         read;

        /* Memory to which writing and sending operations are made. */ 
        uint8_t         write;
        
        /* Memory to which received SMs are preferred to be stored. */ 
        uint8_t         recv;

    } preferred;

    struct {

        /* Message format 0=PDU mode, 1=text mode. */ 
        uint8_t         mode;

    } format;

} AtSmsCmd;

#else /* AT_SMS != XA_ENABLED */ 

/* Stub structures to keep #defines out 
 * of AtResults/AtCommands structures.  
 */ 
typedef uint8_t AtSmsCmd;

#endif /* AT_SMS != XA_ENABLED */

#if AT_DUN == XA_ENABLED

/*---------------------------------------------------------------------------
 * 
 *
 */
typedef union _AtDunCmd {
    uint8_t     junk;
} AtDunCmd;

#else /* AT_DUN == XA_ENABLED */ 

/* Stub structures to keep #defines out of AtResults/AtCommands structures. */
typedef uint8_t AtDunCmd;

#endif /* AT_DUN == XA_ENABLED */

/*---------------------------------------------------------------------------
 * AtCommands structure
 *
 *     This structure is used to specify the parameters associated with 
 *     all supported AT commands.  Commands are sent by the Terminal Entity 
 *     (e.g. Hands-Free unit) and received on the Mobile Entity (e.g. 
 *     Gateway).  
 */
typedef struct _AtCommands {

    /* This field is intended for use by the structure owner. */ 
    ListEntry           node;

    /* AT_??? type from any specified command group. The type may include
     * the AT_READ or AT_TEST flags. No parameters are specified when
     * reading or testing a command.
     */ 
    uint16_t           type;

    union {

        /* Headset AT command group parameters. */ 
        AtHeadsetCmd        hs;

        /* Hands-Free AT command group parameters. */ 
        AtHandsfreeCmd      hf;

        /* Phonebook AT command group parameters. */ 
        AtPhonebookCmd      pb;

        /* Short Message Service AT command group parameters. */ 
        AtSmsCmd            sms;

        /* Dial Up Networking AT command group parameters. */ 
        AtDunCmd            dun;

        /* AT_SET_ERROR_MODE */ 
        struct {

            /* Error mode: 0=disable, 1=enable, 2=verbose */ 
            uint8_t              mode;

        } error;

#if (AT_HANDSFREE == XA_ENABLED) || (AT_PHONEBOOK == XA_ENABLED)

        /* AT_SELECT_CHARACTER_SET */ 
        struct {

            /* Character set type (e.g. "GSM", "HEX", "UCS2"). */ 
            const char     *type;

        } charSet;

#endif /* (AT_HANDSFREE == XA_ENABLED) || (AT_PHONEBOOK == XA_ENABLED) */ 

        AtRaw               raw;
    } p;

} AtCommands;

/*---------------------------------------------------------------------------
 * HfChannel structure
 *
 *     Structures of type HfChannel can be used to store information about a 
 *     Hands-Free channel.  
 */
struct _HfChannel {

    /* === Internal use only === */ 

    /* Registration variables */ 
    ListEntry           node;  
    HfCallback          callback;         /* Application callback 
                                           * function */ 
    RfChannel           rfChannel;        /* RFCOMM channel used for 
                                           * hands-freeclient or server 
                                           * connection.  
                                           */ 

    /* Transmit Queue */
    int                 processingCmdQ;   /* Used to prevent recursive calls 
                                           * to HfExecuteNextCommand().  
                                           */ 
    ListEntry           cmdQueue;         /* List of logical commands     */ 
    ListEntry           txQueue;          /* List of AT commands          */ 
    HfCommand          *nextCommand;      /* The next command to execute  */ 

    /* Connection State Variables */ 
    HfChannelStates     state;           /* Current connection state      */ 
    uint16_t            flags;           /* Current connection flags      */ 
    uint8_t             linkFlags;       /* Levels of service connected   */ 

    /* Gateway State Information */ 
    HfGatewayVersion    version;         /* Profile version parsed from 
                                          * SDP 
                                          */ 
    HfGatewayFeatures   gwFeatures;      /* Profile features parsed from 
                                          * SDP 
                                          */ 
    HfGwHoldFeatures    gwHoldFeatures;  /* 3-Way calling hold features   */ 
    int                 nrec;            /* State of noise reduction and 
                                          * echo cancellation 
                                          */ 
    int                 ibRing;          /* State of in-band ringing      */ 

#if HF_VREC == XA_ENABLED
    int                 voiceRec;        /* State of voice recognition    */ 
#endif /* HF_VREC == XA_ENABLED */ 

#if HF_CODEC_NEG == XA_ENABLED
    uint8_t             codecID;         /* current selected codec ID */
#endif /* HF_CODEC_NEG == XA_ENABLED */
    int                 callId;          /* State of caller id 
                                          * nofitication 
                                          */ 
    int                 callWaiting;     /* State of call waiting         */ 
    uint8_t             micGain;         /* Current microphone gain       */ 
    uint8_t             speakerGain;     /* Current speaker gain          */ 

    /* SDP variables for client */ 
    SdpQueryToken       sdpQueryToken;   /* Used to query the service     */ 
    uint8_t             queryFlags;      /* Defines which SDP entries were 
                                          * parsed from the gateway.  
                                          */ 
    uint8_t             rfServerChannel; /* When connecting hf client     */ 

    /* Channel Resources */ 
    CmgrHandler         cmgrHandler;
    AtCommands          atCommand;
    AtCommands         *currentAtCommand;
    AtCommands         *lastAtCommand;
    XaBufferDesc        atBuffer;
    BtPacket            atTxPacket;
    uint8_t             atTxData[HF_TX_BUFFER_SIZE];
    uint16_t            bytesToSend;
    EvmTimer            atTimer;
    EvmTimer            ringTimer;
    uint8_t             atRxBuffer[HF_RECV_BUFFER_SIZE];
    uint16_t            atRxLen;

#if HF_USE_CALL_MANAGER == XA_ENABLED
    
    /* Polling resources */ 
    EvmTimer            pollTimer;
    AtCommands          pollCommand;
    BtPacket            pollPacket;
    U8                  pollData[RF_MAX_FRAME_SIZE];
    U8                  pollFlags;
    PollInterval        pollInterval;
    BOOL                pollQuickPending;
    HfCallListState     pollCallListState;

    /* Call State */ 
    HfCallStatus        oldState[2];     /* Old     Call States           */ 
    HfCallStatus        recentState[2];  /* Recent  Call States           */ 
    HfCallStatus        callState[2];    /* Current Call States           */ 
    BOOL                multiParty;
    BOOL                multiRecent;
    BOOL                multiOld;
    U8                  context;         /* Context for the current op    */ 
    HfOptCallIDStruct   cidOld;          /* The caller ID for old calls   */ 
    HfOptCallIDStruct   cidRecent;       /* The caller ID for recent 
                                          * calls.  
                                          */ 
    HfOptCallIDStruct   cid;             /* The caller ID for current 
                                          * calls.  
                                          */ 
    BOOL                heldActiveSwap;  /* If a held-active event is 
                                          * received, and there is a held 
                                          * and an active call, then they 
                                          * should be swapped.  
                                          */ 

#if HF_USE_RESP_HOLD == XA_ENABLED

    HfRespHoldAppl      respHoldAppl;
    HfRespHoldAppl      respHoldRecent;
    HfRespHoldAppl      respHoldOld;
    HfRespHoldStatus    respHoldStatus[2];
    HfRespHoldQState    respHoldQState;
    BOOL                rhPending;       /* TRUE when a response and hold 
                                          * command or query is pending.  
                                          * Additional response and hold 
                                          * commands or queries must not be 
                                          * issued until this is FALSE 
                                          * again.  Otherwise, the call 
                                          * manager will be confused by the 
                                          * interleaved OK responses.  
                                          */ 

#endif /* HF_USE_RESP_HOLD == XA_ENABLED */ 

#endif /* HF_USE_CALL_MANAGER == XA_ENABLED */ 

    uint8_t             indMap[20];
#if HF_USE_IIA == XA_ENABLED

#if HF_USE_CALL_MANAGER == XA_ENABLED
    
    /* This contains the current individual 
     * indicator active or inactive reporting state.  
     */ 
    HfIIAReporting      cur;

    /* This contains the individual indicator active or inactive reporting 
     * state that the audio gateway has been commanded to adopt.  The 
     * individual indicator order in this array is the same the CIND 
     * message.  
     */ 
    HfIIReportingState  iiaCmd[AT_MAX_INDICATORS];

    char                cindBuf[0x400];

#endif /* HF_USE_CALL_MANAGER == XA_ENABLED */ 

    char                biaBuff[HF_MAX_BIA_STRING];

#else /* HF_USE_IIA != XA_ENABLED */ 
    uint8_t             numInd;
#endif
};

/*---------------------------------------------------------------------------
 * HfPhonebookInfo structure
 *
 * This type is only available when HF_USE_PHONEBOOK_COMMANDS is set to 
 * XA_ENABLED.  A pointer to this structure is sent to the application's 
 * callback function notifying the application when phonebook storage 
 * information is received (see HF_GetPhonebookInfo).  
 */
typedef struct _HfPhonebookInfo {
    HfPhonebooks   selected;  /* Selected phonebook type */ 
    uint16_t       used;      /* Number of entries used  */ 
    uint16_t       total;     /* Total number of entries */ 
} HfPhonebookInfo;

/*---------------------------------------------------------------------------
 * HfPhonebookSize structure
 *
 * This type is only available when HF_USE_PHONEBOOK_COMMANDS is set to 
 * XA_ENABLED.  A pointer to this structure is sent to the application's 
 * callback function notifying the application when phonebook size 
 * information is received (see HF_GetPhonebookSize).  
 */
typedef struct _HfPhonebookSize {
    uint16_t         index1;     /* First Entry           */ 
    uint16_t         index2;     /* Last Entry            */ 
    uint16_t         numberLen;  /* Maximum number length */ 
    uint16_t         textLen;    /* Maximum text length   */ 
} HfPhonebookSize;

/*---------------------------------------------------------------------------
 * HfPhonebookEntry structure
 *
 * This type is only available when HF_USE_PHONEBOOK_COMMANDS is set to 
 * XA_ENABLED.  A pointer to this structure is sent to the application's 
 * callback function notifying the application when a phonebook entry is 
 * received (see HF_ReadPhonebookEntries and HF_FindPhonebookEntries).  
 */
typedef struct _HfPhonebookEntry {
    uint16_t        index;     /* Location in the phone book     */ 
    uint8_t         type;
    const char     *number;    /* The phone number               */ 
    const char     *text;      /* The text part                  */ 
} HfPhonebookEntry;

/*---------------------------------------------------------------------------
 * HfCallbackParms structure
 *
 * A pointer to this structure is sent to the application's callback 
 * function notifying the application of any state changes or important 
 * events.  
 */
struct _HfCallbackParms {
    HfEvent    event;    /* Event associated with the callback       */ 

    BtStatus    status;  /* Status of the callback event             */ 
    BtErrorCode errCode; /* Error code (reason) on disconnect events */ 

    /* For certain events, a single member of this union will be valid.  
     * See HfEvent documentation for more information.  
     */
    union {
        void             *ptr;
        BtRemoteDevice   *remDev;
        HfGatewayFeatures features;
        HfGwHoldFeatures  holdFeatures;
        int               service;
        int               roam;
        int               inBandRing;
        int               voiceRecognition;
        uint8_t           battery;
        uint8_t           signal;
        uint8_t           gain;
#if HF_CODEC_NEG == XA_ENABLED
        uint8_t           codecID;
#endif        
        HfCommand        *command;
        uint8_t          *networkOper;
        uint8_t          *voiceTag;
        HfSubscriberNum  *subscriberNum;

#if HF_USE_MESSAGING_COMMANDS == XA_ENABLED

        BOOL              sms;           /* Only valid if 
                                          * HF_USE_MESSAGING_COMMANDS 
                                          * is set to XA_ENABLED.  
                                          */

#endif /* HF_USE_MESSAGING_COMMANDS == XA_ENABLED */ 

#if HF_USE_RESP_HOLD == XA_ENABLED
        
#if HF_USE_CALL_MANAGER == XA_ENABLED

        HfRespHoldAppl    respHoldAppl;  /* Indicates which response and 
                                          * hold features are currently 
                                          * applicable.  
                                          */ 

#else /* HF_USE_CALL_MANAGER != XA_ENABLED */ 

        HfRespHoldAction  respHold;      /* Only valid if HF_USE_RESP_HOLD 
                                          * is set to XA_ENABLED.  
                                          */

#endif /* else HF_USE_CALL_MANAGER != XA_ENABLED */ 

#endif /* HF_USE_RESP_HOLD == XA_ENABLED */ 

#if HF_USE_CALL_MANAGER == XA_ENABLED
        
        HfCallStateParms *callStateParms;/* Only valid if 
                                          * HF_USE_CALL_MANAGER is set to 
                                          * XA_ENABLED.  
                                          */

        HfCallIdParms    *callIdParms;   /* Only valid if HF_
                                          * USE_CALL_MANAGER is set to 
                                          * XA_ENABLED.  
                                          */

#else /* HF_USE_CALL_MANAGER != XA_ENABLED */ 

        HfCallActiveState call;          /* Only valid if 
                                          * HF_USE_CALL_MANAGER is set to 
                                          * XA_DISABLED.  
                                          */

        HfCallSetupState  callSetup;     /* Only valid if 
                                          * HF_USE_CALL_MANAGER is set to 
                                          * XA_DISABLED.  
                                          */

        HfCallHeldState   callHeld;      /* Only valid if 
                                          * HF_USE_CALL_MANAGER is set to 
                                          * XA_DISABLED.  
                                          */

        HfCallWaitParms  *callWaitParms; /* Only valid if 
                                          * HF_USE_CALL_MANAGER is set to 
                                          * XA_DISABLED.  
                                          */

        HfCallerIdParms  *callerIdParms; /* Only valid if 
                                          * HF_USE_CALL_MANAGER is set to 
                                          * XA_DISABLED.  
                                          */

        HfCallListParms  *callListParms; /* Only valid if 
                                          * HF_USE_CALL_MANAGER is set to 
                                          * XA_DISABLED.  
                                          */

#endif /* else HF_USE_CALL_MANAGER != XA_ENABLED */ 

#if HF_USE_PHONEBOOK_COMMANDS == XA_ENABLED

        HfPhonebooks      phonebooks;    /* Only valid if 
                                          * HF_USE_PHONEBOOK_COMMANDS 
                                          * is set to XA_ENABLED.  
                                          */

        HfPhonebookInfo  *phonebookInfo; /* Only valid if 
                                          * HF_USE_PHONEBOOK_COMMANDS 
                                          * is set to XA_ENABLED.  
                                          */

        HfPhonebookSize  *phonebookSize; /* Only valid if 
                                          * HF_USE_PHONEBOOK_COMMANDS 
                                          * is set to XA_ENABLED.  
                                          */

        HfPhonebookEntry *phonebookEntry;/* Only valid if 
                                          * HF_USE_PHONEBOOK_COMMANDS 
                                          * is set to XA_ENABLED.  
                                          */

#endif /* HF_USE_PHONEBOOK_COMMANDS == XA_ENABLED */

#if BT_SCO_HCI_DATA == XA_ENABLED
        
        HfAudioData      *audioData;     /* Only valid if BT_SCO_HCI_DATA is 
                                          * set to XA_ENABLED.  
                                          */
        BtPacket         *audioPacket;   /* Only valid if BT_SCO_HCI_DATA is 
                                          * set to XA_ENABLED.  
                                          */

#endif /* BT_SCO_HCI_DATA == XA_ENABLED */ 

        HfAtData         *data;
    } p;
};

/****************************************************************************
 *
 * Function Reference
 *
 ****************************************************************************/

/*---------------------------------------------------------------------------
 * HF_Init()
 *
 *     This function initializes the Hands-free SDK.  This function should 
 *     only be called once, normally at system initialization time.  The 
 *     calling of this function can be specified in overide.h using the 
 *     XA_LOAD_LIST macro.  
 *
 * Returns:
 *     TRUE - Initialization was successful
 *
 *     FALSE - Initialization failed.
 */
int HF_Init(void);

/*---------------------------------------------------------------------------
 * HF_Register()
 *
 *     HF_Register() has been deprecated.  The HF_RegisterSec() function 
 *     should be used instead.  For the time being, if HF_Register() is 
 *     used, it is remapped as a macro to the HF_RegisterSec().  However, 
 *     the HF_Register() macro will be deleted from future releases of the 
 *     Hands Free profile.  This macro registers and initializes a channel 
 *     for use in creating or receiving service level connections.  It 
 *     registers the Hands-Free profile with the connection manager, RFCOMM, 
 *     and SDP.  The application callback function is also bound to the 
 *     channel.  This API will register a security record with default 
 *     values using HF_SECURITY_SETTINGS.  To set the values in the security 
 *     settings for the security record, it is better to use 
 *     HF_RegisterSec() instead of HF_Register().  
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
 *     BT_STATUS_INVALID_PARM - A parameter invalid or not properly 
 *         initialized (XA_ERROR_CHECK only). 
 */
BtStatus HF_Register(HfChannel *Chan, HfCallback Callback);
#define HF_Register(ch, cb) HF_RegisterSec(ch, cb, 0)

/*---------------------------------------------------------------------------
 * HF_RegisterSec()
 *
 *     This function registers and initializes a channel for use in creating 
 *     or receiving service level connections.  Registers the Hands-Free 
 *     profile with the connection manager, RFCOMM, and SDP.  The 
 *     application callback function is also bound to the channel.  
 *     Registers a security record based on the values passed to the secParm 
 *     parameter.  If null, a record with default values set with 
 *     HF_SECURITY_SETTINGS is used.  
 *
 * Parameters:
 *     Chan - Contains a pointer to the channel structure that will be 
 *         initialized and registered.  
 *
 *     Callback - The application callback function that will receive 
 *         events.  
 * 
 *     secParms - The BtSecurityParms values passed in for registering
 *         a BtSecurityRecord.
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
 *     BT_STATUS_INVALID_PARM - A parameter invalid or not properly 
 *         initialized (XA_ERROR_CHECK only). 
 */
BtStatus HF_RegisterSec(HfChannel       *Chan, 
                        HfCallback       Callback, 
                        BtSecurityParms *secParms);

/*---------------------------------------------------------------------------
 * HF_Deregister()
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
 *     BT_STATUS_INVALID_PARM - A parameter invalid or not properly 
 *         initialized (XA_ERROR_CHECK only).  
 */
BtStatus HF_Deregister(HfChannel *Chan);

/*---------------------------------------------------------------------------
 * HF_CreateServiceLink()
 *
 *     This function creates a service level connection with the Audio 
 *     Gateway.  This includes performing SDP Queries to find the 
 *     appropriate service and opening an RFCOMM channel.  The success of 
 *     the operation is indicated by the HF_EVENT_SERVICE_CONNECTED event.  
 *     If the connection fails, the application is notified by the 
 *     HF_EVENT_SERVICE_DISCONNECTED event.  
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
 *         callback function registered by HF_Register).  
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
 *     BT_STATUS_INVALID_PARM - A parameter invalid or not properly 
 *         initialized (XA_ERROR_CHECK only).  
 */
BtStatus HF_CreateServiceLink(HfChannel *Chan, BD_ADDR *Addr);

/*---------------------------------------------------------------------------
 * HF_DisconnectServiceLink()
 *
 *     This function releases the service level connection with the Audio 
 *     Gateway.  This will close the RFCOMM channel and will also close the 
 *     SCO and ACL links if no other services are active, and no other link 
 *     handlers are in use (ME_CreateLink).  When the operation is complete 
 *     the application will be notified by the HF_EVENT_SERVICE_DISCONNECTED 
 *     event.  
 *
 * Parameters:
 *     Channel - Pointer to a registered channel structure.  
 *
 * Returns:
 *     BT_STATUS_PENDING - The operation has started; the application will 
 *         be notified when the service level connection is down (via the 
 *         callback function registered by HF_Register).
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NO_CONNECTION - The operation failed because a service link 
 *         does not exist to the audio gateway.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter invalid or not properly 
 *         initialized (XA_ERROR_CHECK only).  
 */
BtStatus HF_DisconnectServiceLink(HfChannel *Chan);

/*---------------------------------------------------------------------------
 * HF_CreateAudioLink()
 *
 *     This function creates an audio (SCO) link to the Audio Gateway.  The 
 *     success of the operation is indicated by the HF_EVENT_AUDIO_CONNECTED 
 *     event.  If the connection fails, the application is notified by the 
 *     HF_EVENT_AUDIO_DISCONNECTED event.  
 *
 * Parameters:
 *     Chan - Pointer to a registered channel structure.  
 *
 * Returns:
 *     BT_STATUS_PENDING - The operation has started; the application will 
 *         be notified when the audio link has been established (via the 
 *         callback function registered by HF_Register).  
 *
 *     BT_STATUS_SUCCESS - The audio (SCO) link already exists.  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NO_CONNECTION - The operation failed because a service level 
 *         connection does not exist to the audio gateway.  
 *
 *     BT_STATUS_FAILED - An audio connection already exists.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter invalid or not properly 
 *         initialized (XA_ERROR_CHECK only).  
 */
BtStatus HF_CreateAudioLink(HfChannel *Chan);

/*---------------------------------------------------------------------------
 * HF_DisconnectAudioLink()
 *
 *     This function releases the audio connection with the Audio Gateway.  
 *     When the operation is complete the application will be notified by 
 *     the HF_EVENT_AUDIO_DISCONNECTED event.  
 * 
 * Parameters:
 *     Chan - Pointer to a registered channel structure.  
 *
 * Returns:
 *     BT_STATUS_PENDING - The operation has started; the application will 
 *         be notified when the audio connection is down (via the callback 
 *         function registered by HF_Register).  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NO_CONNECTION - The operation failed because a service link 
 *         does not exist to the audio gateway.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter invalid or not properly 
 *         initialized (XA_ERROR_CHECK only).  
 */
BtStatus HF_DisconnectAudioLink(HfChannel *Chan);

#if BT_SCO_HCI_DATA == XA_ENABLED

/*---------------------------------------------------------------------------
 * Sends the given data on the SCO link
 */
BtStatus SCO_SendData(BtScoConnect *scocon, BtPacket *packet);

/*---------------------------------------------------------------------------
 * CMGR_SendAudioData()
 *
 *     Sends the specified audio data on the SCO link.
 *
 * Requires:
 *     BT_SCO_HCI_DATA enabled
 *
 * Parameters:
 *     Handler - Handler structure to use.
 *
 *     Packet - The packet of data to send. After this call, the SCO 
 *         manager owns the packet.  When the packet has been transmitted 
 *         to the host controller, CMEVENT_AUDIO_DATA_SENT is sent to the 
 *         handler(s).  
 *
 * Returns:  
 *     BT_STATUS_PENDING - The packet was queued successfully.
 *
 *     BT_STATUS_NO_CONNECTION - No SCO connection exists.
 *
 *     BT_STATUS_INVALID_PARM - Invalid parameter (XA_ERROR_CHECK only).
 */
BtStatus CMGR_SendAudioData(CmgrHandler *Handler, BtPacket *Packet);
#define CMGR_SendAudioData(h, p) (SCO_SendData((h)->scoConnect, p))

/*---------------------------------------------------------------------------
 * HF_SendAudioData()
 *
 *     This macro sends the specified audio data on the audio link.  
 *
 * Requires:
 *     BT_SCO_HCI_DATA enabled.  
 *
 * Parameters:
 *     Chan - The Channel over which to send the audio data.  
 *
 *     Packet - The packet of data to send. After this call, the Hands-free 
 *         SDK owns the packet. When the packet has been transmitted 
 *         to the host controller, HF_EVENT_AUDIO_DATA_SENT is sent to the 
 *         application.  
 *
 * Returns:
 *     BT_STATUS_PENDING - The packet was queued successfully.  
 *
 *     BT_STATUS_NO_CONNECTION - No audio connection exists.  
 *
 *     BT_STATUS_INVALID_PARM - Invalid parameter (XA_ERROR_CHECK only).  
 */
BtStatus HF_SendAudioData(HfChannel *Chan, BtPacket *Packet);
#define HF_SendAudioData(c, p) (CMGR_SendAudioData((&(c)->cmgrHandler), p))

#endif /* BT_SCO_HCI_DATA == XA_ENABLED */ 

/*---------------------------------------------------------------------------
 * HF_AnswerCall()
 *
 *     This function answers an incoming call.  This function is called 
 *     after receiving a HF_EVENT_CALL_STATE event that indicates an 
 *     incoming call.  To reject the incoming call, use the HF_Hangup 
 *     function.  When the call is accepted or rejected by the gateway, the 
 *     application will be notified of the call state change by the 
 *     HF_EVENT_CALL_STATE event.  
 *
 *     In addition, the HF_EVENT_COMMAND_COMPLETE event will be received.  
 *
 *     When an active call exists and a second incoming call is indicated, 
 *     the HF_AnswerCall function will perform the equivalent of HF_CallHold 
 *     with an "HfHoldAction" of HF_HOLD_HOLD_ACTIVE_CALLS.  If call waiting 
 *     is disabled, notification of a second incoming call will not occur 
 *     (see HF_EnableCallWaitNotify).  
 *
 * Parameters:
 *     Chan - Pointer to a registered channel structure.  
 *
 *     Command - A command structure to be used for transmitting the 
 *         command.  Until the Bluetooth stack sends the 
 *         HF_EVENT_COMMAND_COMPLETE event for this command back to the 
 *         application, the application must not reuse the memory space in 
 *         this structure, but instead must use different command structure 
 *         instances for sending additional commands.  
 *
 * Returns:
 *     BT_STATUS_PENDING - The operation has started, the application will 
 *         be notified when the command has completed (via the callback 
 *         function registered by HF_Register).
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NO_CONNECTION - The operation failed because a service link 
 *         does not exist to the audio gateway.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter invalid or not properly 
 *         initialized (XA_ERROR_CHECK only).  
 *
 *     BT_STATUS_FAILED - No incoming call exists.  
 */
BtStatus HF_AnswerCall(HfChannel *Chan, HfCommand *Command);

/*---------------------------------------------------------------------------
 * HF_DialNumber()
 *
 *     This function initiates an outgoing call using a phone number.  
 *
 *     During the process of calling, the HF_EVENT_CALL_STATE event will be 
 *     generated to show the progress of the call.  Not all states are 
 *     applicable to all services.  At a minimum, the application will be 
 *     notified of the following states:  HF_CALL_STATUS_DIALING and 
 *     HF_CALL_STATUS_ACTIVE.  
 *
 *     In addition, the HF_EVENT_COMMAND_COMPLETE event will be received.  
 *
 *     If a call is already active, it must be put on hold before calling 
 *     this function.  
 *
 * Parameters:
 *     Chan - Pointer to a registered channel structure.  
 *
 *     Number - An ASCII string containing the number to be dialed.  Until 
 *         the Bluetooth stack sends the HF_EVENT_COMMAND_COMPLETE event for 
 *         this command back to the application, the application must not 
 *         reuse the memory space in this string, but instead must use 
 *         different strings for sending additional commands.  
 *
 *     Command - A command structure to be used for transmitting the 
 *         command.  Until the Bluetooth stack sends the 
 *         HF_EVENT_COMMAND_COMPLETE event for this command back to the 
 *         application, the application must not reuse the memory space in 
 *         this structure, but instead must use different command structure 
 *         instances for sending additional commands.  
 *
 * Returns:
 *     BT_STATUS_PENDING - The operation has started, the application will 
 *         be notified when the command has completed (via the callback 
 *         function registered by HF_Register).
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NO_CONNECTION - The operation failed because a service link 
 *         does not exist to the audio gateway.  
 *
 *     BT_STATUS_FAILED - A call cannot be made.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter invalid or not properly 
 *         initialized (XA_ERROR_CHECK only).  
 */
BtStatus HF_DialNumber(HfChannel *Chan, 
                       uint8_t        *Number, 
                       uint16_t        Len, 
                       HfCommand *Command);

/*---------------------------------------------------------------------------
 * HF_MemoryDial()
 *
 *     This function initiates an outgoing call using a memory location on 
 *     the phone.  
 *
 *     During the process of calling, the HF_EVENT_CALL_STATE event will be 
 *     generated to show the progress of the call.  Not all states are 
 *     applicable to all services.  At a minimum, the application will be 
 *     notified of the following states:  HF_CALL_STATUS_DIALING and 
 *     HF_CALL_STATUS_ACTIVE.  
 *
 *     In addition, the HF_EVENT_COMMAND_COMPLETE event will be received.  
 *
 *     If a call is already active, it must be put on hold before calling 
 *     this function.  
 *
 * Parameters:
 *     Chan - Pointer to a registered channel structure.  
 *
 *     Location - An ASCII string containing the memory location 
 *         to be dialed.  Until the Bluetooth stack sends the 
 *         HF_EVENT_COMMAND_COMPLETE event for this command back to the 
 *         application, the application must not reuse the memory space in 
 *         this string, but instead must use different strings for sending 
 *         additional commands.  
 *
 *     Len - This parameter must contain the number of characters in the 
 *         Location parameter string, not counting any NULL termination 
 *         characters.  
 *
 *     Command - A command structure to be used for transmitting the 
 *         command.  Until the Bluetooth stack sends the 
 *         HF_EVENT_COMMAND_COMPLETE event for this command back to the 
 *         application, the application must not reuse the memory space in 
 *         this structure, but instead must use different command structure 
 *         instances for sending additional commands.  
 *
 * Returns:
 *     BT_STATUS_PENDING - The operation has started, the application will 
 *         be notified when the command has completed (via the callback 
 *         function registered by HF_Register).
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NO_CONNECTION - The operation failed because a service link 
 *         does not exist to the audio gateway.  
 *
 *     BT_STATUS_FAILED - A call cannot be made.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter invalid or not properly 
 *         initialized (XA_ERROR_CHECK only).  
 */
BtStatus HF_MemoryDial(HfChannel *Chan, 
                       uint8_t        *Location, 
                       uint16_t        Len, 
                       HfCommand *Command);

/*---------------------------------------------------------------------------
 * HF_Redial()
 *
 *     This function initiates an outgoing call based on the last number 
 *     dialed in the audio gateway.  
 *
 *     During the process of calling, the HF_EVENT_CALL_STATE event will be 
 *     generated to show the progress of the call.  Not all states are 
 *     applicable to all services.  At a minimum, the application will be 
 *     notified of the following states:  HF_CALL_STATUS_DIALING and 
 *     HF_CALL_STATUS_ACTIVE.  
 *
 *     In addition, the HF_EVENT_COMMAND_COMPLETE event will be received.  
 *
 *     If a call is already active, it must be put on hold before calling 
 *     this function.  
 *
 * Parameters:
 *     Chan - Pointer to a registered channel structure.  
 *
 *     Command - A command structure to be used for transmitting the 
 *         command.  Until the Bluetooth stack sends the 
 *         HF_EVENT_COMMAND_COMPLETE event for this command back to the 
 *         application, the application must not reuse the memory space in 
 *         this structure, but instead must use different command structure 
 *         instances for sending additional commands.  
 *
 * Returns:
 *     BT_STATUS_PENDING - The operation has started, the application will 
 *         be notified when the command has completed (via the callback 
 *         function registered by HF_Register).  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NO_CONNECTION - The operation failed because a service link 
 *         does not exist to the audio gateway.  
 *
 *     BT_STATUS_FAILED - A call cannot be made.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter invalid or not properly 
 *         initialized (XA_ERROR_CHECK only).  
 */
BtStatus HF_Redial(HfChannel *Chan, HfCommand *Command);

/*---------------------------------------------------------------------------
 * HF_CallHold()
 *
 *     This function issues a command to the Audio Gateway to manage 
 *     multi-party calling.  This function allows the application to perform 
 *     explicit handling of 3-Way calls (see HfHoldAction).  During the 
 *     process of this command, the HF_EVENT_CALL_STATE event will be 
 *     generated to show the state change of the call.  
 *
 *     In addition, the HF_EVENT_COMMAND_COMPLETE event will be received.  
 *
 * Parameters:
 *     Chan - Pointer to a registered channel structure.  
 *
 *     HoldAction - Describes the type of hold function.  
 *
 *     Index - Call to which the action applies.  Ignored if HoldAction is 
 *            not 1 or 2.  
 *
 *     Command - A command structure to be used for transmitting the 
 *         command.  Until the Bluetooth stack sends the 
 *         HF_EVENT_COMMAND_COMPLETE event for this command back to the 
 *         application, the application must not reuse the memory space in 
 *         this structure, but instead must use different command structure 
 *         instances for sending additional commands.  
 *
 * Returns:
 *     BT_STATUS_PENDING - The operation has started, the application will 
 *         be notified when the command has completed (via the callback 
 *         function registered by HF_Register).  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NO_CONNECTION - The operation failed because a service link 
 *         does not exist to the audio gateway.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter invalid or not properly 
 *         initialized (XA_ERROR_CHECK only).  
 */
BtStatus HF_CallHold(HfChannel   *Chan, 
                     HfHoldAction HoldAction, 
                     uint8_t           Index, 
                     HfCommand   *Command);

#if HF_USE_RESP_HOLD == XA_ENABLED

/*---------------------------------------------------------------------------
 * HF_QueryResponseHold()
 *
 *     This function is only available if HF_USE_RESP_HOLD is set to 
 *     XA_ENABLED.  It issues a Query for the state of Response and Hold on 
 *     the Audio Gateway.  This feature is typically supported in Japanese 
 *     markets (see HfResponseHoldState).  If the audio gateway is in a 
 *     response and hold state, then it will send back a +BTRH message.  If 
 *     HF_USE_CALL_MANAGER is set to XA_ENABLED, if polling is not reliable, 
 *     and if the previous call state does not correspond to the reported 
 *     response and hold condition, then an HF_EVENT_CALL_STATE event is 
 *     received; but if HF_USE_CALL_MANAGER is not set to XA_ENABLED, and if 
 *     the audio gateway is in a response and hold state, then a 
 *     HF_EVENT_RESPONSE_HOLD will be received.  
 *
 *     In addition, the HF_EVENT_COMMAND_COMPLETE event will be received.  
 *
 * Requires:
 *     HF_USE_RESP_HOLD enabled.  
 *
 * Parameters:
 *     Chan - Pointer to a registered channel structure.  
 *
 *     Command - A command structure to be used for transmitting the 
 *         command.  Until the Bluetooth stack sends the 
 *         HF_EVENT_COMMAND_COMPLETE event for this command back to the 
 *         application, the application must not reuse the memory space in 
 *         this structure, but instead must use different command structure 
 *         instances for sending additional commands.  
 *
 * Returns:
 *     BT_STATUS_PENDING - The operation has started, the application will 
 *         be notified when the command has completed (via the callback 
 *         function registered by HF_Register).
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NO_CONNECTION - The operation failed because a service link 
 *         does not exist to the audio gateway.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter invalid or not properly 
 *         initialized (XA_ERROR_CHECK only).  
 *
 *     BT_STATUS_BUSY - A response and hold message is already in progress, 
 *         so a new one cannot be started until the other completes.  
 */
BtStatus HF_QueryResponseHold(HfChannel *Chan, HfCommand *Command);

/*---------------------------------------------------------------------------
 * HF_ResponseHold()
 *
 *     This function is only available if HF_USE_RESP_HOLD is set to 
 *     XA_ENABLED.  It issues a Response and Hold command to the Audio 
 *     Gateway to manage incoming calls.  This feature is typically 
 *     supported in Japanese markets.  (See HfResponseHoldState).  When the 
 *     specified action is taken, the HF_EVENT_RESPONSE_HOLD event will be 
 *     received, which should indicate the action that was taken.  
 *
 *     In addition, the HF_EVENT_COMMAND_COMPLETE event will be received.  
 *
 * Requires:
 *     HF_USE_RESP_HOLD enabled.  
 *
 * Parameters:
 *     Chan - Pointer to a registered channel structure.  
 *
 *     RespHoldAction - Describes the type of action to take.  
 *
 *     Command - A command structure to be used for transmitting the 
 *         command.  Until the Bluetooth stack sends the 
 *         HF_EVENT_COMMAND_COMPLETE event for this command back to the 
 *         application, the application must not reuse the memory space in 
 *         this structure, but instead must use different command structure 
 *         instances for sending additional commands.  
 *
 * Returns:
 *     BT_STATUS_PENDING - The operation has started, the application will 
 *         be notified when the command has completed (via the callback 
 *         function registered by HF_Register).  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NO_CONNECTION - The operation failed because a service link 
 *         does not exist to the audio gateway.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter invalid or not properly 
 *         initialized (XA_ERROR_CHECK only).  
 *
 *     BT_STATUS_BUSY - A response and hold message is already in progress, 
 *         so a new one cannot be started until the other completes.  
 */
BtStatus HF_ResponseHold(HfChannel       *Chan, 
                         HfRespHoldAction RespHoldAction, 
                         HfCommand       *Command);

#endif /* HF_USE_RESP_HOLD == XA_ENABLED */ 

/*---------------------------------------------------------------------------
 * HF_Hangup()
 *
 *     This function terminates an existing (active) call, rejects an 
 *     incoming call, or cancels an outgoing call.  This function can be 
 *     called whenever an active call exists or after receiving a 
 *     HF_EVENT_CALL_STATE event that indicates an incoming or outgoing 
 *     call.  When the call is terminated, the application will be notified 
 *     of the call state change by the HF_EVENT_CALL_STATE event.
 *
 *     In addition, the HF_EVENT_COMMAND_COMPLETE event will be received.  
 *
 * Parameters:
 *     Chan - Pointer to a registered channel structure.
 *
 *     Command - A command structure to be used for transmitting the 
 *         command.  Until the Bluetooth stack sends the 
 *         HF_EVENT_COMMAND_COMPLETE event for this command back to the 
 *         application, the application must not reuse the memory space in 
 *         this structure, but instead must use different command structure 
 *         instances for sending additional commands.  
 *
 * Returns:
 *     BT_STATUS_PENDING - The operation has started, the application will 
 *         be notified when the command has completed (via the callback 
 *         function registered by HF_Register).
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NO_CONNECTION - The operation failed because a service link 
 *         does not exist to the audio gateway.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter invalid or not properly 
 *         initialized (XA_ERROR_CHECK only).  
 *
 *     BT_STATUS_FAILED - No call exists.  
 */
BtStatus HF_Hangup(HfChannel *Chan, HfCommand *Command);

/*---------------------------------------------------------------------------
 * HF_ListCurrentCalls()
 *
 *     This function queries the Audio Gateway for call state information.  
 *
 *     After making this call, the HF_EVENT_CURRENT_CALL_STATE event will be 
 *     generated for each call on the audio gateway.  
 *
 *     If HF_USE_CALL_MANAGER is set to XA_ENABLED, an HF_EVENT_CALL_STATE 
 *     event is received instead of the HF_EVENT_CURRENT_CALL_STATE event.  
 *
 *     The HF_EVENT_COMMAND_COMPLETE event will be received when the 
 *     gateway signals that all calls have been listed.  
 *
 * Parameters:
 *     Chan - Pointer to a registered channel structure.  
 *
 *     Command - A command structure to be used for transmitting the 
 *         command.  Until the Bluetooth stack sends the 
 *         HF_EVENT_COMMAND_COMPLETE event for this command back to the 
 *         application, the application must not reuse the memory space in 
 *         this structure, but instead must use different command structure 
 *         instances for sending additional commands.  
 *
 * Returns:
 *     BT_STATUS_PENDING - The operation has started, the application will 
 *         be notified when the command has completed (via the callback 
 *         function registered by HF_Register).
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NO_CONNECTION - The operation failed because a service link 
 *         does not exist to the audio gateway.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter invalid or not properly 
 *         initialized (XA_ERROR_CHECK only).  
 */
BtStatus HF_ListCurrentCalls(HfChannel *Chan, HfCommand *Command);

/*---------------------------------------------------------------------------
 * HF_EnableCallerIdNotify()
 *
 *     This function enables notification of the calling line 
 *     identification.  When this command is complete, the 
 *     HF_EVENT_COMMAND_COMPLETE event will be received.  
 *
 * Parameters:
 *     Chan - Pointer to a registered channel structure.  
 *
 *     Enabled - TRUE or FALSE.  
 *
 *     Command - A command structure to be used for transmitting the 
 *         command.  Until the Bluetooth stack sends the 
 *         HF_EVENT_COMMAND_COMPLETE event for this command back to the 
 *         application, the application must not reuse the memory space in 
 *         this structure, but instead must use different command structure 
 *         instances for sending additional commands.  
 *
 * Returns:
 *     BT_STATUS_PENDING - The operation has started, the application will 
 *         be notified when the command has completed (via the callback 
 *         function registered by HF_Register).
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NO_CONNECTION - The operation failed because a service link 
 *         does not exist to the audio gateway.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter invalid or not properly 
 *         initialized (XA_ERROR_CHECK only).  
 */
BtStatus HF_EnableCallerIdNotify(HfChannel *Chan, 
                                 int       Enabled, 
                                 HfCommand *Command);

/*---------------------------------------------------------------------------
 * HF_EnableCallWaitNotify()
 *
 *     This function enables notification of call waiting.  When this 
 *     command is complete, the HF_EVENT_COMMAND_COMPLETE event will be 
 *     received.  
 *
 * Parameters:
 *     Chan - Pointer to a registered channel structure.  
 *
 *     Enabled - TRUE or FALSE.  
 *
 *     Command - A command structure to be used for transmitting the 
 *         command.  Until the Bluetooth stack sends the 
 *         HF_EVENT_COMMAND_COMPLETE event for this command back to the 
 *         application, the application must not reuse the memory space in 
 *         this structure, but instead must use different command structure 
 *         instances for sending additional commands.  
 *
 * Returns:
 *     BT_STATUS_PENDING - The operation has started, the application will 
 *         be notified when the command has completed (via the callback 
 *         function registered by HF_Register).  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NO_CONNECTION - The operation failed because a service link 
 *         does not exist to the audio gateway.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter invalid or not properly 
 *         initialized (XA_ERROR_CHECK only).  
 *
 *     BT_STATUS_NOT_SUPPORTED - Call waiting was not included in the list 
 *         of supported features (HF_SDK_FEATURES).  
 */
BtStatus HF_EnableCallWaitNotify(HfChannel *Chan, 
                                 int       Enabled, 
                                 HfCommand *Command);

/*---------------------------------------------------------------------------
 * HF_GenerateDtmf()
 *
 *     This function commands the Audio Gateway to send a DTMF code to the 
 *     network.  A call MUST be ongoing in order for the Audio Gateway to 
 *     generate a DTMF code.  The HF_EVENT_COMMAND_COMPLETE event will be 
 *     received when the DTMF code is sent.  
 *
 *
 * Parameters:
 *     Chan - Pointer to a registered channel structure.  
 *
 *     dtmfTone - A single ASCII character in the set 0-9, #, *, A-D.  
 *
 *     Command - A command structure to be used for transmitting the 
 *         command.  Until the Bluetooth stack sends the 
 *         HF_EVENT_COMMAND_COMPLETE event for this command back to the 
 *         application, the application must not reuse the memory space in 
 *         this structure, but instead must use different command structure 
 *         instances for sending additional commands.  
 *
 * Returns:
 *     BT_STATUS_PENDING - The operation has started, the application will 
 *         be notified when the command has completed (via the callback 
 *         function registered by HF_Register).
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_BUSY - No available RFCOMM packets.  
 *
 *     BT_STATUS_FAILED - A service link is not active.  
 *
 *     BT_STATUS_INVALID_PARM - Invalid ASCII character.  
 */
BtStatus HF_GenerateDtmf(HfChannel *Chan, uint8_t dtmfTone, HfCommand *Command);

/*---------------------------------------------------------------------------
 * HF_GetLastVoiceTag()
 *
 *     This function retrieves the number associated with the last voice tag 
 *     recorded in the Hands-Free Unit.  When the number is received, the 
 *     application's callback function will receive the 
 *     HF_EVENT_COMMAND_COMPLETE event.  
 *
 * Parameters:
 *     Chan - Pointer to a registered channel structure.  
 *
 *     Command - A command structure to be used for transmitting the 
 *         command.  Until the Bluetooth stack sends the 
 *         HF_EVENT_COMMAND_COMPLETE event for this command back to the 
 *         application, the application must not reuse the memory space in 
 *         this structure, but instead must use different command structure 
 *         instances for sending additional commands.  
 *
 * Returns:
 *     BT_STATUS_PENDING - The operation has started, the application will 
 *         be notified when the command has completed (via the callback 
 *         function registered by HF_Register).
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NO_CONNECTION - The operation failed because a service link 
 *         does not exist to the audio gateway.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter invalid or not properly 
 *         initialized (XA_ERROR_CHECK only).  
 */
BtStatus HF_GetLastVoiceTag(HfChannel *Chan, HfCommand *Command);

/*---------------------------------------------------------------------------
 * HF_EnableVoiceRecognition()
 *
 *     This function enables or disables voice recognition on the Audio 
 *     Gateway.  When this command is complete, the 
 *     HF_EVENT_COMMAND_COMPLETE event will be received.  
 *
 * Parameters:
 *     Chan - Pointer to a registered channel structure.  
 *
 *     Enabled - Set to TRUE if voice recognition is enabled, and FALSE 
 *         if it is disabled.  
 *
 *     Command - A command structure to be used for transmitting the 
 *         command.  Until the Bluetooth stack sends the 
 *         HF_EVENT_COMMAND_COMPLETE event for this command back to the 
 *         application, the application must not reuse the memory space in 
 *         this structure, but instead must use different command structure 
 *         instances for sending additional commands.  
 *
 * Returns:
 *     BT_STATUS_SUCCESS - The specified mode is already set.  
 *
 *     BT_STATUS_PENDING - The operation has started, the application will 
 *         be notified when the command has completed (via the callback 
 *         function registered by HF_Register).  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NO_CONNECTION - The operation failed because a service link 
 *         does not exist to the audio gateway.  
 *
 *     BT_STATUS_FAILED - Could not initiate voice recognition.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter invalid or not properly 
 *         initialized (XA_ERROR_CHECK only).  
 */
BtStatus HF_EnableVoiceRecognition(HfChannel *Chan, 
                                   int       Enabled, 
                                   HfCommand *Command);

/*---------------------------------------------------------------------------
 * HF_DisableNREC()
 *
 *     This function disables noise reduction and echo canceling.  When this 
 *     command is complete, the HF_EVENT_COMMAND_COMPLETE event will be 
 *     received.  
 *
 * Parameters:
 *     Chan - Pointer to a registered channel structure.  
 *
 *     Command - A command structure to be used for transmitting the 
 *         command.  Until the Bluetooth stack sends the 
 *         HF_EVENT_COMMAND_COMPLETE event for this command back to the 
 *         application, the application must not reuse the memory space in 
 *         this structure, but instead must use different command structure 
 *         instances for sending additional commands.  
 *
 * Returns:
 *     BT_STATUS_PENDING - The operation has started, the application will 
 *         be notified when the command has completed (via the callback 
 *         function registered by HF_Register).
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NO_CONNECTION - The operation failed because a service link 
 *         does not exist to the audio gateway.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter invalid or not properly 
 *         initialized (XA_ERROR_CHECK only).  
 */
BtStatus HF_DisableNREC(HfChannel *Chan, HfCommand *Command);

/*---------------------------------------------------------------------------
 * HF_ReportMicVolume()
 *
 *     This function reports the current microphone gain of the Hands-Free 
 *     device.  
 *
 *     When the command issued as a result of this call is completed, 
 *     the HF_EVENT_COMMAND_COMPLETE event will be received by the 
 *     application's callback function.  
 *
 * Parameters:
 *     Chan - Pointer to a registered channel structure.  
 *
 *     Gain - The current gain level (0-15).  
 *
 *     Command - A command structure to be used for transmitting the 
 *         command.  Until the Bluetooth stack sends the 
 *         HF_EVENT_COMMAND_COMPLETE event for this command back to the 
 *         application, the application must not reuse the memory space in 
 *         this structure, but instead must use different command structure 
 *         instances for sending additional commands.  
 *
 * Returns:
 *     BT_STATUS_PENDING - The operation has started, the application will 
 *         be notified when the command has completed (via the callback 
 *         function registered by HF_Register).
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NO_CONNECTION - The operation failed because a service link 
 *         does not exist to the audio gateway.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter invalid or not properly 
 *         initialized (XA_ERROR_CHECK only).  
 */
BtStatus HF_ReportMicVolume(HfChannel *Chan, uint8_t Gain, HfCommand *Command);

/*---------------------------------------------------------------------------
 * HF_ReportSpeakerVolume()
 *
 *     This function reports the current speaker volume of the Hands-Free 
 *     Unit.  
 *
 *     When the command issued as a result of this call is completed, 
 *     the HF_EVENT_COMMAND_COMPLETE event will be received by the 
 *     application's callback function.  
 *
 * Parameters:
 *     Chan - Pointer to a registered channel structure.  
 *
 *     Gain - The current gain level (0-15).  
 *
 *     Command - A command structure to be used for transmitting the 
 *         command.  Until the Bluetooth stack sends the 
 *         HF_EVENT_COMMAND_COMPLETE event for this command back to the 
 *         application, the application must not reuse the memory space in 
 *         this structure, but instead must use different command structure 
 *         instances for sending additional commands.  
 *
 * Returns:
 *     BT_STATUS_PENDING - The operation has started, the application will 
 *         be notified when the command has completed (via the callback 
 *         function registered by HF_Register).
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NO_CONNECTION - The operation failed because a service link 
 *         does not exist to the audio gateway.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter invalid or not properly 
 *         initialized (XA_ERROR_CHECK only).  
 */
BtStatus HF_ReportSpeakerVolume(HfChannel *Chan, 
                                uint8_t         Gain, 
                                HfCommand *Command);

/*---------------------------------------------------------------------------
 * HF_QueryNetworkOperator()
 *
 *     This function queries the Audio Gateway for the Network Operator.  
 *     The HF_EVENT_NETWORK_OPERATOR event will be received from the audio 
 *     gateway.  
 *
 *     When the command issued as a result of this call is completed, 
 *     the HF_EVENT_COMMAND_COMPLETE event will be received by the 
 *     application's callback function.  
 *
 * Parameters:
 *     Chan - Pointer to a registered channel structure.  
 *
 *     Command - A command structure to be used for transmitting the 
 *         command.  Until the Bluetooth stack sends the 
 *         HF_EVENT_COMMAND_COMPLETE event for this command back to the 
 *         application, the application must not reuse the memory space in 
 *         this structure, but instead must use different command structure 
 *         instances for sending additional commands.  
 *
 * Returns:
 *     BT_STATUS_PENDING - The operation has started, the application will 
 *         be notified when the command has completed (via the callback 
 *         function registered by HF_Register).  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NO_CONNECTION - The operation failed because a service link 
 *         does not exist to the audio gateway.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter invalid or not properly 
 *         initialized (XA_ERROR_CHECK only).  
 */
BtStatus HF_QueryNetworkOperator(HfChannel *Chan, HfCommand *Command);

/*---------------------------------------------------------------------------
 * HF_QuerySubscriberNumber()
 *
 *     This function queries the Audio Gateway for the Subscriber Number.  
 *     The HF_EVENT_SUBSCRIBER_NUMBER event will be received from the audio 
 *     gateway.  
 *
 *     When the command issued as a result of this call is completed, 
 *     the HF_EVENT_COMMAND_COMPLETE event will be received by the 
 *     application's callback function.  
 *
 * Parameters:
 *     Chan - Pointer to a registered channel structure.  
 *
 *     Command - A command structure to be used for transmitting the 
 *         command.  Until the Bluetooth stack sends the 
 *         HF_EVENT_COMMAND_COMPLETE event for this command back to the 
 *         application, the application must not reuse the memory space in 
 *         this structure, but instead must use different command structure 
 *         instances for sending additional commands.  
 *
 * Returns:
 *     BT_STATUS_PENDING - The operation has started, the application will 
 *         be notified when the command has completed (via the callback 
 *         function registered by HF_Register).  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NO_CONNECTION - The operation failed because a service link 
 *         does not exist to the audio gateway.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter invalid or not properly 
 *         initialized (XA_ERROR_CHECK only).  
 */
BtStatus HF_QuerySubscriberNumber(HfChannel *Chan, HfCommand *Command);

/*---------------------------------------------------------------------------
 * HF_EnableExtendedErrors()
 *
 *     This function enables extended error codes on the Audio Gateway.  
 *     (See HfCmeError.)  
 *
 *     When the command issued as a result of this call is completed, 
 *     the HF_EVENT_COMMAND_COMPLETE event will be received by the 
 *     application's callback function.  
 *
 * Parameters:
 *     Chan - Pointer to a registered channel structure.  
 *
 *     Command - A command structure to be used for transmitting the 
 *         command.  Until the Bluetooth stack sends the 
 *         HF_EVENT_COMMAND_COMPLETE event for this command back to the 
 *         application, the application must not reuse the memory space in 
 *         this structure, but instead must use different command structure 
 *         instances for sending additional commands.  
 *
 * Returns:
 *     BT_STATUS_PENDING - The operation has started, the application will 
 *         be notified when the command has completed (via the callback 
 *         function registered by HF_Register).  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NO_CONNECTION - The operation failed because a service link 
 *         does not exist to the audio gateway.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter invalid or not properly 
 *         initialized (XA_ERROR_CHECK only).  
 */
BtStatus HF_EnableExtendedErrors(HfChannel *Chan, HfCommand *Command);

/*---------------------------------------------------------------------------
 * HF_SendAtCommand()
 *
 *     This function sends any AT command.  The 'AtString' parameter must be 
 *     initialized and the AT command must be a properly formatted AT 
 *     Command.  The "AT" characters must be included in the string when 
 *     needed.  A carriage return character will be appended to the end of 
 *     the string.  
 *
 *     When the AT command is completed, the HF_EVENT_COMMAND_COMPLETE 
 *     event will be received by the application's callback function.  In 
 *     addition, any unsolicited result code not recognized by the 
 *     Hands-free SDK will generate an HF_EVENT_AT_RESULT_DATA event.  
 *
 *     When HF_USE_CALL_MANAGER, sending AT commands that affect the call 
 *     state should be avoided, because they may compromise the internal 
 *     call states of the Call Manager.  Use the API calls provided for 
 *     initiating, answering, or changing the state of ongoing calls.  
 *
 * Parameters:
 *     Chan - Pointer to a registered channel structure.  
 *
 *     AtString - A pointer to an initialized AT Command string.  Until the 
 *         Bluetooth stack sends the HF_EVENT_COMMAND_COMPLETE event for 
 *         this command back to the application, the application must not 
 *         reuse the memory space in this string, but instead must use 
 *         different strings for sending additional commands.  
 *   
 *     Command - A command structure to be used for transmitting the 
 *         command.  Until the Bluetooth stack sends the 
 *         HF_EVENT_COMMAND_COMPLETE event for this command back to the 
 *         application, the application must not reuse the memory space in 
 *         this structure, but instead must use different command structure 
 *         instances for sending additional commands.  
 *
 * Returns:
 *     BT_STATUS_PENDING - The operation has started, the application will 
 *         be notified when the command has completed (via the callback 
 *         function registered by HF_Register).  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NO_CONNECTION - The operation failed because a service link 
 *         does not exist to the audio gateway.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter invalid or not properly 
 *         initialized (XA_ERROR_CHECK only).  
 */
BtStatus HF_SendAtCommand(HfChannel  *Chan, 
                          const char *AtString,
                          HfCommand  *Command);

/*---------------------------------------------------------------------------
 * HF_QueryPhonebooks()
 *
 *     This function is only available when HF_USE_PHONEBOOK_COMMANDS is set 
 *     to XA_ENABLED.  It queries the Audio Gateway for its supported 
 *     phonebooks.  When this command is issued successfully, the 
 *     HF_EVENT_PHONEBOOK_STORAGE event will be received, followed by the 
 *     HF_EVENT_COMMAND_COMPLETE event.  
 *
 * Requires:
 *     HF_USE_PHONEBOOK_COMMANDS must be enabled.  
 *
 * Parameters:
 *     Chan - Pointer to a registered channel structure.  
 *
 *     Command - A command structure to be used for transmitting the 
 *         command.  Until the Bluetooth stack sends the 
 *         HF_EVENT_COMMAND_COMPLETE event for this command back to the 
 *         application, the application must not reuse the memory space in 
 *         this structure, but instead must use different command structure 
 *         instances for sending additional commands.  
 *
 * Returns:
 *     BT_STATUS_PENDING - The operation has started, the application will 
 *         be notified when the command has completed (via the callback 
 *         function registered by HF_Register).  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NO_CONNECTION - The operation failed because a service link 
 *         does not exist to the audio gateway.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter invalid or not properly 
 *         initialized (XA_ERROR_CHECK only).  
 */
BtStatus HF_QueryPhonebooks(HfChannel *Chan, HfCommand *Command);

/*---------------------------------------------------------------------------
 * HF_SelectPhonebook()
 *
 *     This function is only available when HF_USE_PHONEBOOK_COMMANDS is set 
 *     to XA_ENABLED.  It makes the specific phonebook the active one for 
 *     subsequent calls.  When this occurs, the HF_EVENT_COMMAND_COMPLETE 
 *     event will be received.  
 *
 * Requires:
 *     HF_USE_PHONEBOOK_COMMANDS must be enabled.  
 *
 * Parameters:
 *     Chan - Pointer to a registered channel structure.  
 *
 *     Phonebook - This parameter must contain an enumeration value 
 *         corresponding to the phonebook that is to be activated.  
 *
 *     Command - A command structure to be used for transmitting the 
 *         command.  Until the Bluetooth stack sends the 
 *         HF_EVENT_COMMAND_COMPLETE event for this command back to the 
 *         application, the application must not reuse the memory space in 
 *         this structure, but instead must use different command structure 
 *         instances for sending additional commands.  
 *
 * Returns: 
 *     BT_STATUS_PENDING - The operation has started, the application will 
 *         be notified when the command has completed (via the callback 
 *         function registered by HF_Register).
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NO_CONNECTION - The operation failed because a service link 
 *         does not exist to the audio gateway.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter invalid or not properly 
 *         initialized (XA_ERROR_CHECK only).  
 */
BtStatus HF_SelectPhonebook(HfChannel   *Chan, 
                            HfPhonebooks Phonebook, 
                            HfCommand   *Command);

/*---------------------------------------------------------------------------
 * HF_GetCurrentPhonebookInfo()
 *
 *     This function is only available when HF_USE_PHONEBOOK_COMMANDS is set 
 *     to XA_ENABLED.  It queries for the active phonebook.  When this 
 *     command is issued successfully, the HF_EVENT_PHONEBOOK_INFO event 
 *     will be received, followed by the HF_EVENT_COMMAND_COMPLETE event.  
 *
 * Requires:
 *     HF_USE_PHONEBOOK_COMMANDS must be enabled.  
 *
 * Parameters:
 *     Chan - Pointer to a registered channel structure.  
 *
 *     Command - A command structure to be used for transmitting the 
 *         command.  Until the Bluetooth stack sends the 
 *         HF_EVENT_COMMAND_COMPLETE event for this command back to the 
 *         application, the application must not reuse the memory space in 
 *         this structure, but instead must use different command structure 
 *         instances for sending additional commands.  
 *
 * Returns:
 *     BT_STATUS_PENDING - The operation has started, the application will 
 *         be notified when the command has completed (via the callback 
 *         function registered by HF_Register).
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NO_CONNECTION - The operation failed because a service link 
 *         does not exist to the audio gateway.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter invalid or not properly 
 *         initialized (XA_ERROR_CHECK only).  
 */
BtStatus HF_GetCurrentPhonebookInfo(HfChannel *Chan, HfCommand *Command);

/*---------------------------------------------------------------------------
 * HF_GetPhonebookSize()
 *
 *     This function is only available when HF_USE_PHONEBOOK_COMMANDS is set 
 *     to XA_ENABLED.  It gets the number of entries and the size of the 
 *     elements of the active phonebook.  When this command is issued 
 *     successfully, the HF_EVENT_PHONEBOOK_INFO event will be received, 
 *     followed by the HF_EVENT_COMMAND_COMPLETE event.  
 *
 * Requires:
 *     HF_USE_PHONEBOOK_COMMANDS must be enabled.  
 *
 * Parameters:
 *     Chan - Pointer to a registered channel structure.  
 *
 *     Command - A command structure to be used for transmitting the 
 *         command.  Until the Bluetooth stack sends the 
 *         HF_EVENT_COMMAND_COMPLETE event for this command back to the 
 *         application, the application must not reuse the memory space in 
 *         this structure, but instead must use different command structure 
 *         instances for sending additional commands.  
 *
 * Returns:
 *     BT_STATUS_PENDING - The operation has started, the application will 
 *         be notified when the command has completed (via the callback 
 *         function registered by HF_Register).  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NO_CONNECTION - The operation failed because a service link 
 *         does not exist to the audio gateway.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter invalid or not properly 
 *         initialized (XA_ERROR_CHECK only).  
 */
BtStatus HF_GetPhonebookSize(HfChannel *Chan, HfCommand *Command);

/*---------------------------------------------------------------------------
 * HF_ReadPhonebookEntries()
 *
 *     This function is only available when HF_USE_PHONEBOOK_COMMANDS is set 
 *     to XA_ENABLED.  It reads entries from the specified phonebook in the 
 *     specified range.  When this command is issued successfully, the 
 *     HF_EVENT_PHONEBOOK_ENTRY event will be received, followed by the 
 *     HF_EVENT_COMMAND_COMPLETE event.  
 *
 * Requires:
 *     HF_USE_PHONEBOOK_COMMANDS must be enabled.  
 *
 * Parameters:
 *     Chan - Pointer to a registered channel structure.  
 *
 *     From - Starting index
 *
 *     To - Ending index
 *
 *     Command - A command structure to be used for transmitting the 
 *         command.  Until the Bluetooth stack sends the 
 *         HF_EVENT_COMMAND_COMPLETE event for this command back to the 
 *         application, the application must not reuse the memory space in 
 *         this structure, but instead must use different command structure 
 *         instances for sending additional commands.  
 *
 * Returns:
 *     BT_STATUS_PENDING - The operation has started, the application will 
 *         be notified when the command has completed (via the callback 
 *         function registered by HF_Register).  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NO_CONNECTION - The operation failed because a service link 
 *         does not exist to the audio gateway.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter invalid or not properly 
 *         initialized (XA_ERROR_CHECK only).  
 */
BtStatus HF_ReadPhonebookEntries(HfChannel *Chan, 
                                 uint16_t        From, 
                                 uint16_t        To, 
                                 HfCommand *Command);

/*---------------------------------------------------------------------------
 * HF_FindPhonebookEntries()
 *
 *     This function is only available when HF_USE_PHONEBOOK_COMMANDS is set 
 *     to XA_ENABLED.  It finds all entries beginning with the specified 
 *     string.  Queries the Audio Gateway for its supported phonebooks.  
 *     When this command is issued successfully, the 
 *     HF_EVENT_PHONEBOOK_ENTRY event will be received, followed by the 
 *     HF_EVENT_COMMAND_COMPLETE event.  
 *
 * Requires:
 *     HF_USE_PHONEBOOK_COMMANDS must be enabled.  
 *
 * Parameters:
 *     Chan - Pointer to a registered channel structure.  
 *
 *     Text - The search string.  Until the Bluetooth stack sends the 
 *         HF_EVENT_COMMAND_COMPLETE event for this command back to the 
 *         application, the application must not reuse the memory space in 
 *         this string, but instead must use different strings for sending 
 *         additional commands.  
 *
 *     Command - A command structure to be used for transmitting the 
 *         command.  Until the Bluetooth stack sends the 
 *         HF_EVENT_COMMAND_COMPLETE event for this command back to the 
 *         application, the application must not reuse the memory space in 
 *         this structure, but instead must use different command structure 
 *         instances for sending additional commands.  
 *
 * Returns:
 *     BT_STATUS_PENDING - The operation has started, the application will 
 *         be notified when the command has completed (via the callback 
 *         function registered by HF_Register).  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NO_CONNECTION - The operation failed because a service link 
 *         does not exist to the audio gateway.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter invalid or not properly 
 *         initialized (XA_ERROR_CHECK only).  
 */
BtStatus HF_FindPhonebookEntries(HfChannel  *Chan, 
                                 const char *Text, 
                                 HfCommand  *Command);

/*---------------------------------------------------------------------------
 * HF_WritePhonebookEntry()
 *
 *     This function is only available when HF_USE_PHONEBOOK_COMMANDS is set 
 *     to XA_ENABLED.  It writes the phonebook entry.  When the request is 
 *     done HF_EVENT_COMMAND_COMPLETE event will be received.  
 *
 * Requires:
 *     HF_USE_PHONEBOOK_COMMANDS must be enabled.  
 *
 * Parameters:
 *     Channel - Pointer to a registered channel structure.  
 *
 *     Index - The phone book index
 *
 *     Number - The telephone number string.  Until the Bluetooth stack 
 *         sends the HF_EVENT_COMMAND_COMPLETE event for this command back 
 *         to the application, the application must not reuse the memory 
 *         space in this string, but instead must use different strings for 
 *         sending additional commands.  
 *
 *     Text - The phone book text string.  Until the Bluetooth stack sends 
 *         the HF_EVENT_COMMAND_COMPLETE event for this command back to the 
 *         application, the application must not reuse the memory space in 
 *         this string, but instead must use different strings for sending 
 *         additional commands.  
 *
 *     Type - The phone book type
 *
 *     Command - A command structure to be used for transmitting the 
 *         command.  Until the Bluetooth stack sends the 
 *         HF_EVENT_COMMAND_COMPLETE event for this command back to the 
 *         application, the application must not reuse the memory space in 
 *         this structure, but instead must use different command structure 
 *         instances for sending additional commands.  
 *
 * Returns:
 *     BT_STATUS_PENDING - The operation has started, the application will 
 *         be notified when the command has completed (via the callback 
 *         function registered by HF_Register).
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NO_CONNECTION - The operation failed because a service link 
 *         does not exist to the audio gateway.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter invalid or not properly 
 *         initialized (XA_ERROR_CHECK only).  
 */
BtStatus HF_WritePhonebookEntry(HfChannel  *Chan, 
                                uint16_t         Index, 
                                const char *Number, 
                                const char *Text, 
                                uint8_t          Type,
                                HfCommand  *Command);


#if HF_USE_IIA == XA_ENABLED

/*---------------------------------------------------------------------------
 * HF_IIAlwaysActive()
 *
 *     This function is only available when HF_USE_IIA is set to 
 *     XA_ENABLED.  This function indicates whether the reporting for an 
 *     individual indicator corresponding to the given index can be 
 *     deactivated or not.  The call, call setup, and call hold individual 
 *     indicators cannot be deactivated.  
 *
 * Requires:
 *     HF_USE_IIA must be enabled.  
 *
 * Parameters:
 *     hfChanP - This parameter must contain the address of a registered 
 *         hands free channel structure.  
 *
 *     iiNdx - An index corresponding to an individual indicator as defined 
 *         by the CIND message.  
 *
 * Returns:
 *     TRUE if the reporting for an individual indicator corresponding 
 *     to the given index must always be activated, or FALSE 
 *     otherwise.  
 */
BOOL HF_IIAlwaysActive(HfChannel *hfChanP, U8 iiNdx);

/*---------------------------------------------------------------------------
 * HF_SetIIAReportingState()
 *
 *     This function is only available when HF_USE_IIA is set to 
 *     XA_ENABLED.  This function defines which Individual Indicators should 
 *     be reported by the Audio Gateway, and which should not.  This allows 
 *     the reporting of certain individual activators such as signal 
 *     strength to be disabled to extend battery life.  When the request is 
 *     done HF_EVENT_COMMAND_COMPLETE event will be received.  
 *
 * Requires:
 *     HF_USE_IIA must be enabled.  
 *
 * Parameters:
 *     hfChanP - This parameter must contain the address of a registered 
 *         hands free channel structure.  
 *
 *     iiaCmdP - This parameter must contain the address of a individual 
 *         indicators activation command structure.  The elements in this 
 *         structure correspond to the desired reporting state for the 
 *         various individual indicators.  
 *
 *     hfCommandP - This parameter must contain the address of a hands free 
 *         command structure to be used for transmitting the command.  Until 
 *         the HF_EVENT_COMMAND_COMPLETE event for this command is sent back 
 *         to the application, the application must not reuse the memory 
 *         space in this structure, but instead must use different command 
 *         structure instances for sending additional commands.  
 *
 * Returns:
 *     BT_STATUS_PENDING - The operation has started, the application will 
 *         be notified when the command has completed (via the callback 
 *         function registered by HF_Register).
 *
 *     BT_STATUS_CANCELLED - The reporting state is already as requested, so 
 *         no BIA command message was sent.  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NO_CONNECTION - The operation failed because a service link 
 *         does not exist to the audio gateway.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter invalid or not properly 
 *         initialized (XA_ERROR_CHECK only).  
 */
BtStatus HF_SetIIAReportingState(HfChannel *hfChanP, 
                                 HfIIACmd  *iiaCmdP,
                                 HfCommand *hfCommandP);

#if HF_USE_CALL_MANAGER == XA_ENABLED

/*---------------------------------------------------------------------------
 * HF_GetIIAReportingState()
 *
 *     This function is only available when HF_USE_IIA and 
 *     HF_USE_CALL_MANAGER are both set to XA_ENABLED.  This function 
 *     indicates which Individual Indicators are being reported by the Audio 
 *     Gateway, and which are not.  The application can invoke 
 *     HF_SetIIAReportingState() to disable the reporting of certain 
 *     individual activators such as signal strength to extend battery 
 *     life.  
 *
 * Requires:
 *     HF_USE_IIA and HF_USE_CALL_MANAGER both must be enabled.  
 *
 * Parameters:
 *     hfChanP - This parameter must contain the address of a registered 
 *         hands free channel structure.  
 *
 *     rsP - This parameter must contain the address of an individual 
 *         indicators activation report structure.  The elements in this 
 *         structure will be updated to correspond to the current reporting 
 *         state for the various individual indicators.  
 *
 * Returns:
 *     BT_STATUS_SUCCESS - The given individual indication report structure 
 *         has been updated with the latest activation state information.  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NO_CONNECTION - The operation failed because a service link 
 *         does not exist to the audio gateway.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter invalid or not properly 
 *         initialized (XA_ERROR_CHECK only).  
 *
 *     BT_STATUS_FAILED - The call manager data has become corrupted.  
 */
BtStatus HF_GetIIAReportingState(HfChannel      *hfChanP, 
                                 HfIIAReporting *rsP);

#endif /* HF_USE_CALL_MANAGER == XA_ENABLED */ 

#endif /* HF_USE_IIA == XA_ENABLED */ 

#if HF_CODEC_NEG == XA_ENABLED
/*---------------------------------------------------------------------------
 *            HF_CodecConnectionSetup()
 *---------------------------------------------------------------------------
 *
 * Synopsis:  Send available codec to AG when doing connection codec setup.  
 *
 * Return:    (See header file)
 *
 */
BtStatus HF_CodecConnectionSetup(HfChannel *Chan, const char *codecID, HfCommand *Command);

/*---------------------------------------------------------------------------
 * HF_CodecConnectionReq()
 *
 *     HF requesting the AG to start the codec connection procedure.  
 *
 * Parameters:
 *     Chan - Pointer to a registered channel structure.  
 *
 *     Command - A command structure to be used for transmitting the 
 *         command.  Until the Bluetooth stack sends the 
 *         HF_EVENT_COMMAND_COMPLETE event for this command back to the 
 *         application, the application must not reuse the memory space in 
 *         this structure, but instead must use different command structure 
 *         instances for sending additional commands.  
 *
 * Returns:
 *     BT_STATUS_PENDING - The operation has started, the application will 
 *         be notified when the command has completed (via the callback 
 *         function registered by HF_Register).  
 *
 *     BT_STATUS_NOT_FOUND - The specified channel has not been registered.  
 *
 *     BT_STATUS_NO_CONNECTION - The operation failed because a service link 
 *         does not exist to the audio gateway.  
 *
 *     BT_STATUS_INVALID_PARM - A parameter invalid or not properly 
 *         initialized (XA_ERROR_CHECK only).  
 */
BtStatus HF_CodecConnectionReq(HfChannel *Chan, HfCommand *Command);

#endif /* HF_CODEC_NEG == XA_ENABLED */

/*---------------------------------------------------------------------------
 * HF_IsChannelOpen()
 *
 *     This function indicates whether a channel is open.  
 *
 * Parameters:
 *     Chan - Pointer to a registered channel structure.  
 *
 * Returns:
 *
 *     TRUE - The specified channel is open.  
 *
 *     FALSE - The channel is closed.  
 */
int HF_IsChannelOpen(HfChannel *Chan);

/*---------------------------------------------------------------------------
 * HF_IsACLConnected()
 *
 *     This function indicates whether the ACL link is up.  
 *
 * Parameters:
 *     Chan - Pointer to a registered channel structure.  
 *
 * Returns:
 *     TRUE - The ACL link is connected.  
 *
 *     FALSE - The ACL link is disconnected.  
 */
int HF_IsACLConnected(HfChannel *Chan);

/*---------------------------------------------------------------------------
 * HF_IsHandsfreeConnected()
 *
 *     This function indicates whether the Hands-Free profile is connected.  
 *
 * Parameters:
 *     Chan - Pointer to a registered channel structure.  
 *
 * Returns:
 *     TRUE - The Hands-Free profile is connected.  
 *
 *     FALSE - The Hands-Free profile is not connected.  
 */
int HF_IsHandsfreeConnected(HfChannel *Chan);

/*---------------------------------------------------------------------------
 * HF_IsAudioConnected()
 *
 *     This function indicates whether an audio connection is up.  
 *
 * Parameters:
 *     Chan - Pointer to a registered channel structure.  
 *
 * Returns:
 *     TRUE - An audio link is up.  
 *     
 *     FALSE - No audio link is up.  
 */
int HF_IsAudioConnected(HfChannel *Chan);

/*---------------------------------------------------------------------------
 * HF_GetRemoteBDAddr()
 *
 *     This function provides the Bluetooth Device Address associated with 
 *     the given channel.  
 *
 * Parameters:
 *     Chan - Pointer to a registered channel structure.  
 *
 *     bdAddrP - Must contain the address of a structure of type BD_ADDR 
 *         where the Bluetooth Device Address can be written.  
 *
 * Returns:
 *     TRUE if the address was obtained, or FALSE otherwise.  
 */
int HF_GetRemoteBDAddr(HfChannel *Chan, BD_ADDR *bdAddrP);

/*---------------------------------------------------------------------------
 * HF_ProfileVersion()
 *
 *     This function provides the profile version retrieved from the SDP 
 *     database of the remote device for this service connection.  
 *
 * Parameters:
 *     Chan - Pointer to a registered channel structure.  
 *
 * Returns:
 *     HfGatewayVersion
 *
 */
HfGatewayVersion HF_ProfileVersion(HfChannel *Chan);

/*---------------------------------------------------------------------------
 * HF_GatewayFeatures()
 *
 *     This function provides the features of the Audio Gateway.  
 *
 * Parameters:
 *     Chan - Pointer to a registered channel structure.  
 *
 * Returns:
 *     HfGatewayFeatures
 *
 */
HfGatewayFeatures HF_GatewayFeatures(HfChannel *Chan);

/*---------------------------------------------------------------------------
 * HF_GatewayHoldFeatures()
 *
 *     This function provides the hold features of the audio gateway.  
 *
 * Parameters:
 *     Chan - Pointer to a registered channel structure.  
 *
 * Returns:
 *     HfGwHoldFeatures
 *
 */
HfGwHoldFeatures HF_GatewayHoldFeatures(HfChannel *Chan);

/*---------------------------------------------------------------------------
 * HF_SpeakerGain()
 *
 *     This function provides the current value of speaker volume.  
 *
 * Parameters:
 *     Chan - Pointer to a registered channel structure.  
 *
 * Returns:
 *     The current Speaker Gain setting.  
 *
 */
uint8_t HF_SpeakerGain(HfChannel *Chan);

/*---------------------------------------------------------------------------
 * HF_MicGain()
 *
 *     This function the current value of microphone gain.  
 *
 * Parameters:
 *     Chan - Pointer to a registered channel structure.  
 *
 * Returns:
 *     The current Microphone Gain setting.  
 */
uint8_t HF_MicGain(HfChannel *Chan);

/*---------------------------------------------------------------------------
 * HF_IsNRECEnabled()
 *
 *     This function indicates whether Noise Reduction and Echo Cancelling 
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
int HF_IsNRECEnabled(HfChannel *Chan);

/*---------------------------------------------------------------------------
 * HF_IsInbandRingEnabled()
 *
 *     This function indicates whether In-band Ringing is enabled in the 
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
int HF_IsInbandRingEnabled(HfChannel *Chan);

/*---------------------------------------------------------------------------
 * HF_IsCallIdNotifyEnabled()
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
int HF_IsCallIdNotifyEnabled(HfChannel *Chan);

/*---------------------------------------------------------------------------
 * HF_IsVoiceRecActive()
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
int HF_IsVoiceRecActive(HfChannel *Chan);

/*---------------------------------------------------------------------------
 * HF_IsCallWaitingActive()
 *
 *     This function indicates whether Call Waiting is active in the Audio 
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
int HF_IsCallWaitingActive(HfChannel *Chan);

/*---------------------------------------------------------------------------
 * HF_RegisterCmdOverride()
 *
 *     This function registers a callback function to be called any time the 
 *     command state machine is called.  
 *
 *     When an API function is called in the Hands-free SDK, the request is 
 *     queued up to be executed when the channel becomes available.  When 
 *     the channel is available for the next command on the queue, the 
 *     command state machine is called repeatedly to allow the work of the 
 *     logical command to be performed.  Each call the command state machine 
 *     results in either an AT command being sent, or completion of a 
 *     command.  Each time an AT command is sent and a response is received 
 *     (OK, ERROR, CME ERROR, or a command timeout), the state machine is 
 *     called again.  When the work of the logical command is completed, the 
 *     command state machine completes the command and notifies the 
 *     application.  
 *
 *     This function allows the registration of a callback function that 
 *     will be called each time the command state machine is called.  This 
 *     allows an application to override the default behavior of an API 
 *     call.  The callback function can determine which function call is 
 *     being executed and set up its own AT commands for transmission.  
 *
 *     When the override function sets up an AT command for transmission, it 
 *     must return BT_STATUS_PENDING.  This indicates to the command state 
 *     machine that it must send the AT command.  When the override function 
 *     has completed its work, it must return BT_STATUS_SUCCESS.  This 
 *     indicates to the command state machine that it must complete the 
 *     command and notify the application (no AT command will be sent).  If 
 *     the override function does not perform any work, it must return 
 *     BT_STATUS_FAILED to indicate to the command state machine that it 
 *     must take the default action.  
 *
 *     In order to use this feature properly, AT commands must be set up 
 *     using the internal HfChannel structure field "atCommand."  This field 
 *     is not normally a public field, but it can be accessed directly and 
 *     is of the type AtCommandValues (see HF_SendAtCommand).  The override 
 *     function must not send the AT command itself, but must rely on the 
 *     command state machine to transmit the command.  
 *
 *     In addition, the override function can look at the "state" field of 
 *     the HfCommand structure to determine how many times the command state 
 *     machine has been called for this particular command.  Each time the 
 *     state machine is called, the "state" field is incremented.  Initially 
 *     it will have the value of 0.  If multiple AT commands must be sent 
 *     for a particular logical command, the "state" field will help to 
 *     identify the command to send, or if the command is complete.  
 *
 *     This feature adds a lot of power, but must be used properly as 
 *     defined above in order for the internal states of the SDK to be 
 *     maintained properly.  
 *
 *     To unregister the callback, this function should be called with a 
 *     NULL parameter.  
 *
 * Parameters:
 *     Callback - A pointer to the function that will be called.  
 *
 * Returns:
 *     void
 */
void HF_RegisterCmdOverride(HfCmdOverride Callback);

/*---------------------------------------------------------------------------
 * HF_SetMasterRole()
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
BtStatus HF_SetMasterRole(HfChannel *Chan, int Flag);
//#define HF_SetMasterRole(c, f) CMGR_SetMasterRole(&((c)->cmgrHandler), f)

/*---------------------------------------------------------------------------
 * HF_SetSupportedFeature()
 *
 *     This function set local hf supported features.  
 *
 * Parameters:
 * 
 *     feature - bit mask of hf feature.  
 *
 * Returns:
 *
 *     NULL
 */
void HF_SetSupportedFeature(uint16_t feature);

/*---------------------------------------------------------------------------
 * HF_GetSupportedFeature()
 *
 *     This function get local hf supported features.  
 *
 * Parameters:
 * 
 *     NULL.  
 *
 * Returns:
 *
 *     hf supported feature
 */
uint16_t HF_GetSupportedFeature(void);

#endif /* __HF_H_ */ 

