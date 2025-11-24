#ifndef __ATP_H
#define __ATP_H

/** This result acknowledges successful acceptance of a command.  There are 
 *  no parameters with this command.  
 */
#define AT_OK                   0x0000

/** This result indicates that the received command was not accepted.  There 
 *  are no parameters with this command.  
 */
#define AT_ERROR                0x0001

/** This unsolicited result indicates an incoming call signal from network.  
 *  There are no parameters with this command.  
 */
#define AT_RING                 0x0002

/** This result indicates that the received command was not accepted.  The 
 *  "error" parameter is used with this command.  
 */
#define AT_EXTENDED_ERROR       0x0003

/** This command is used to enable or disable extended error reporting.  The 
 * "error" parameter is used with this command.  
 */
#define AT_SET_ERROR_MODE       0x0004

/** This command informs the mobile equipment of the character set used by 
 *  the terminal equipment.  The "charSet" parameter is used with this 
 *  command.  
 */
#define AT_SELECT_CHARACTER_SET 0x0005

/** This result indicates that the call could not be established or the 
 *  connection was terminated.  There are no parameters with this command.  
 */
#define AT_NO_CARRIER           0x0006

/** This result indicates that the called party is busy.  There are no 
 *  parameters with this command.  
 */
#define AT_BUSY                 0x0007

/** This result indicates connection completion on timeout.  There are no 
 *  parameters with this command.  
 */
#define AT_NO_ANSWER            0x0008

/** This result indicates that calls to the number are temporarily 
 *  forbidden.  There are no parameters with this command.  
 */
#define AT_DELAYED              0x0009

/** This result indicates that calls to the number are forbidden until a 
 *  manual reset.  There are no parameters with this command.  
 */
#define AT_BLACKLISTED          0x000A

/** This indicates that the data will be sent as 'raw' AT data.  The 
 *  "data" parameter is used with this response.  
 */
#define AT_RAW                  0x000B


/*********************************************************************/
///for at_hf
/** Command to answer incoming call. No parameters. */ 
#define AT_ANSWER               0x0200

/** Command to dial a number from memory. The AtHandsfreeCmd.dial parameter 
 *  is valid. 
 */
#define AT_DIAL_MEMORY          0x0201

/** Command to dial a phone number. The AtHandsfreeCmd.dial parameter is 
 *  valid.  
 */
#define AT_DIAL_NUMBER          0x0202

/** This command allows control of the Call Waiting supplementary service. 
 *  An unsolicited result is used to indicate the presence of a call.  The 
 *  "wait" parameter is used with this command.  
 */
#define AT_CALL_WAIT_NOTIFY     0x0203

/** This command allows calls can be put on hold, recovered, released, added 
 *  to conversation, and transferred.  The test command can be issued to 
 *  request a list of supported operations.  The AtHandsfreeCmd.hold 
 *  parameter is valid.  
 */
#define AT_CALL_HOLD            0x0204

/** This command instructs the ME to hang-up the current GSM call.  There 
 *  are no parameters.  
 */
#define AT_HANG_UP              0x0205

/** The test command is used to retrieve the mapping between each indicator 
 *  supported by the ME and its corresponding range and order index.  The TE 
 *  can retrieve the current status of the indicators by issuing a read 
 *  command.  There are no parameters to this command.  
 */
#define AT_INDICATORS           0x0206

/** This command enables/disables presentation of the calling line identity 
 *  (CLI) to the subscriber.  An unsolicited result is used to indicate the 
 *  identity to the TE.  The "callId" parameter is used with this command.  
 */
#define AT_CALL_ID              0x0207

/** The TE may test and read the status of the ME indicators.  Received 
 *  unsolicited indicator result messages are indicated by 
 *  AT_INDICATOR_EVENT.  
 */
#define AT_EVENT_REPORTING      0x0208

/** This unsolicited result code is used to report an indicator status 
 *  change.  
 */
#define AT_INDICATOR_EVENT      0x0209

/** This command allows the transmission of DTMF and arbitrary tones.  These 
 *  tones may be used (for example) when announcing the start of a recording 
 *  period.  The "dtmf" parameter is used with this command.  
 */
#define AT_GENERATE_DTMF_TONE   0x020A

/** Command used for requesting the phone number corresponding to the last 
 *  voice tag recorded in the ME.  There are no parameters to this command.  
 */
#define AT_VOICE_TAG            0x020B

/** This command is used to call the last phone number dialed. On reception, 
 *  the ME shall set up a voice call to the last phone number dialed.  There 
 *  are no parameters to this command.  
 */
#define AT_REDIAL               0x020C

/** This command enables/disables the voice recognition function in the ME.  
 *  The unsolicited result code is used to indicate when the voice 
 *  recognition setting was changed autonomously on the ME.  The "vrec" 
 *  parameter is used with this command.  
 */
#define AT_VOICE_RECOGNITION    0x020D

/** This command and result are used for exchanging the supported features 
 *  list between the TE and ME.  The "features" parameter is used with this 
 *  command.  
 */
#define AT_SUPPORTED_FEATURES   0x020E

/** This command is issued to disable any Echo Canceling and Noise Reduction 
 *  functions embedded in the ME.  There are no parameters to this command.  
 */
#define AT_ECHO_C_AND_NOISE_R   0x020F

/** An unsolicited result code issued by the ME to indicate to the TE that 
 *  the in-band ring tone setting has been locally changed.  The TE may 
 *  react accordingly by changing its own alert method.  
 */
#define AT_IN_BAND_RING_TONE    0x0210

/** This command is used by the TE to read the current "Response and Hold" 
 *  state of the ME.  This command may also be used to set the response 
 *  and hold state on the ME of an incoming call.  The unsolicited result 
 *  code is used to indicate when an incoming call is either put on hold, 
 *  accepted or rejected.  The "btrh" parameters are used with this 
 *  command.  
 */
#define AT_RESPONSE_AND_HOLD    0x0211

/** This command is used to request the Subscriber Number information from 
 *  the ME.  The result provides "subscriberNum" parameters.  
 */
#define AT_SUBSCRIBER_NUM       0x0212

/** This command is used to request a list of the current calls.  The result 
 *  indicates each current call and its information.  The "currentCall" 
 *  parameter is used with the result of this command.  
 */
#define AT_LIST_CURRENT_CALLS   0x0213

/** This command is used for reading, selecting and registering the network 
 *  operator.  The response is used to return the current mode and the 
 *  currently selected operator.  The "networkOper" and "networkOper_read" 
 *  parameters are used with this command.  
 */
#define AT_NETWORK_OPERATOR     0x0214

/** This command is used to activate and deactivate reporting for individual 
 *  indicators.  Within the AtHandsfreeCmd structure, the iia.activationStr 
 *  parameter is valid.  
 */
#define AT_IIA                  0x0215

/** An unsolicited result code issued by the ME to setup codec connection.
 */
#define AT_BCS                  0x0216

/** This command and result are used for exchanging the supported codec between 
 *  the TE and ME.  The "bac" parameter is used with this command.
 */
#define AT_BAC                  0x0217

/** requesting the AG to start the codec connection procedure
 */
#define AT_BCC                  0x0218

/**********************************************************************************/ 


///for at_hs
/**********************************************************************************/ 

/**  */ 
#define AT_MICROPHONE_GAIN      0x0100

/**  */ 
#define AT_SPEAKER_GAIN         0x0101

/**  */ 
#define AT_KEYPAD_CONTROL       0x0102

/**********************************************************************************/ 

#endif