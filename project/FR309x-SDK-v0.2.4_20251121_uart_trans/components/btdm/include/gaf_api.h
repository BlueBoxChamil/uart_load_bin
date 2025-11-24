#ifndef _GAF_API_H
#define _GAF_API_H

#include <stdint.h>

#include "gap_api.h"
#include "gaf_sig_defines.h"

/// Length of Codec ID value
#define GAF_CODEC_ID_LEN                (5)

#define GAF_KEY_LEN                     (16)

/// Event ID for callback
enum gaf_cb_event_id
{
    GAF_EVT_INIT_EVENT_START,
    /// Used for GAF Init
    GAF_EVT_BAP_CONFIGURED,                     //!< GAF stack inited
    GAF_EVT_INIT_EVENT_MAX,

    GAF_EVT_BAP_BC_EVENT_START = GAF_EVT_INIT_EVENT_MAX + 1,
    /// Used for BC

    GAF_EVT_BAP_BC_SRC_CMD_CMP,                 //!< Complete event for the enumeration table(@see enum gaf_bap_bc_src_cmd_code)
    GAF_EVT_BAP_BC_SRC_SEND_ISO_CMP,

    GAF_EVT_BAP_BC_SINK_STATUS,                 //!< Indicates the status of synchronization with a Broadcast Group
    GAF_EVT_BAP_BC_SINK_ENABLE_REQ,             //!<
    GAF_EVT_BAP_BC_SINK_DISABLE_REQ,            //!< 
    GAF_EVT_BAP_BC_SINK_ISO_DATA,               //!< Audio data received
    GAF_EVT_BAP_BC_SINK_CMD_CMP,                //!< Complete event for the enumeration table(@see enum gaf_bap_bc_sink_cmd_code)

    GAF_EVT_BAP_BC_SCAN_TIMEOUT,                //!< Indicates that the scanning operation has timed out and ended(for GAF_CMD_BAP_BC_SCAN_START)
    GAF_EVT_BAP_BC_SCAN_REPORT,                 //!< Scan result report
    GAF_EVT_BAP_BC_SCAN_PA_ESTABLISHED,         //!< Indicates that a periodic advertising sync has been established
    GAF_EVT_BAP_BC_SCAN_PA_TERMINATED,          //!< Indicates that a periodic advertising sync has been terminated, lost or establishment has been aborted
    GAF_EVT_BAP_BC_SCAN_PA_REPORT,              //!< Indicates that a periodic advertising report has been received
    GAF_EVT_BAP_BC_SCAN_BIG_INFO_REPORT,        //!< Indicates that a BIG Info advertising report has been received
    GAF_EVT_BAP_BC_SCAN_GROUP_REPORT,           //!< Indicates that a Broadcast group info in basic audio announcement advertising data
    GAF_EVT_BAP_BC_SCAN_SUBGROUP_REPORT,        //!< Indicates that a Broadcast sub-group info in basic audio announcement advertising data
    GAF_EVT_BAP_BC_SCAN_STREAM_REPORT,          //!< Indicates that a Broadcast stream info in basic audio announcement advertising data
    GAF_EVT_BAP_BC_SCAN_PA_SYNCHRONIZE_REQ,     //!< 
    GAF_EVT_BAP_BC_SCAN_PA_TERMINATE_REQ,       //!< 
    GAF_EVT_BAP_BC_SCAN_CMD_CMP,                //!< Complete event for the enumeration table(@see enum gaf_bap_bc_scan_cmd_code)

    GAF_EVT_BAP_BC_EVENT_MAX,

    GAF_EVT_BAP_UC_EVENT_START = GAF_EVT_BAP_BC_EVENT_MAX + 1,
    /// Used for PACS Client
    GAF_EVT_BAP_UC_CAPA_CLI_BOND_DATA ,         //!< Result of PACS(Published Audio Capabilities Service) service discovery
    GAF_EVT_BAP_UC_CAPA_CLI_RECORD,             //!< PAC(Published Audio Capabilities) characteristic value of PACS Server
    GAF_EVT_BAP_UC_CAPA_CLI_LOCATION,           //!< Sink/Soure Audio Location characteristic value of PACS Server
    GAF_EVT_BAP_UC_CAPA_CLI_CONTEXT,            //!< Available/Supported Audio Contexts characteristic value of PACS Server
    GAF_EVT_BAP_UC_CAPA_CLI_SVC_CHANGED,        //!< Received notification of PACS service changed
    GAF_EVT_BAP_UC_CAPA_CLI_CMD_CMP,            //!< Complete event for the enumeration table(@see enum gaf_bap_uc_capa_cli_cmd_code)

    /// Used for PACS Server
    GAF_EVT_BAP_UC_CAPA_SRV_BOND_DATA,          //!< Indicate if sending of notifications(CCC) is enabled
    GAF_EVT_BAP_UC_CAPA_SRV_LOCATION,           //!< Sink/Soure Audio Location characteristic has been written
    GAF_EVT_BAP_UC_CAPA_SRV_CMD_CMP,            //!< Complete event for the enumeration table(@see enum gaf_bap_uc_capa_srv_cmd_code)
    
    /// Used for ASCS Client
    GAF_EVT_BAP_UC_ASCS_CLI_BOND_DATA,          //!< Result of ASCS(Audio Stream Control Service) service discovery
    GAF_EVT_BAP_UC_ASCS_CLI_CIS_STATE,          //!< Indicate the state of a CIS(Connected Isochronous Stream)
    GAF_EVT_BAP_UC_ASCS_CLI_STATE_EMPTY,        //!< Indicate that the state of an ASE(Audio Stream Endpoint) is in Empty
    GAF_EVT_BAP_UC_ASCS_CLI_STATE_CODEC,        //!< Indicate that the state of an ASE is in Codec Configured
    GAF_EVT_BAP_UC_ASCS_CLI_STATE_QOS,          //!< Indicate that the state of an ASE is in Qos(Quality of Service) Configured
    GAF_EVT_BAP_UC_ASCS_CLI_STATE_METADATA,     //!< Indicate that the metadata of an ASE has been updated
    GAF_EVT_BAP_UC_ASCS_CLI_ERROR,              //!< Indicate Aa error occurred during an ASE operation
    GAF_EVT_BAP_UC_ASCS_CLI_SVC_CHANGED,        //!< Received notification of ASCS service changed
    GAF_EVT_BAP_UC_ASCS_CLI_ISO_DATA,           //!< Audio data received
    GAF_EVT_BAP_UC_ASCS_CLI_CMD_CMP,            //!< Complete event for the enumeration table(@see enum gaf_bap_uc_ascs_cli_cmd_code)
    GAF_EVT_BAP_UC_ASCS_CLI_SEND_ISO_CMP,       //!< 

    /// Used for ASCS Server
    GAF_EVT_BAP_UC_ASCS_SRV_BOND_DATA,          //!< Indicate if sending of notifications(CCC) is enabled
    GAF_EVT_BAP_UC_ASCS_SRV_ASE_STATE,          //!< Indicate the state of an ASE
    GAF_EVT_BAP_UC_ASCS_SRV_CIS_STATE,          //!< Indicate the state of a CIS
    GAF_EVT_BAP_UC_ASCS_SRV_ISO_DATA,           //!< Audio data received
    GAF_EVT_BAP_UC_ASCS_SRV_CFG_CODEC_REQ,      //!< Indicate that a codec configuration request has been received
    GAF_EVT_BAP_UC_ASCS_SRV_ASE_ENABLE_REQ,     //!< Indicate that a request to enable an ASE has been received
    GAF_EVT_BAP_UC_ASCS_SRV_UPDATE_METADATA_REQ,//!< Indicate that a request to update metadata for an ASE has been received
    GAF_EVT_BAP_UC_ASCS_SRV_ASE_RELEASE_REQ,    //!< Indicate that a request to release an ASE has been received
    GAF_EVT_BAP_UC_ASCS_SRV_CMD_CMP,            //!< Complete event for the enumeration table(@see enum gaf_bap_uc_ascs_srv_cmd_code)
    GAF_EVT_BAP_UC_ASCS_SRV_SEND_ISO_CMP,
    GAF_EVT_BAP_UC_EVENT_MAX,

    GAF_EVT_UNHANDLED_MSG,
};

/// List of GAF_CMD command codes for Basic Audio Profile - Broadcast Source
enum gaf_bap_bc_src_cmd_code
{
    /// Add a Broadcast Group.
    GAF_CMD_BAP_BC_SRC_ADD_GROUP = 0,
    /// Configure or reconfigure a Broadcast Group.
    GAF_CMD_BAP_BC_SRC_SET_GROUP,
    /// Configure or reconfigure a Broadcast Subgroup.
    GAF_CMD_BAP_BC_SRC_SET_SUBGROUP,
    /// Configure or reconfigure a Broadcast Stream and set the Broadcast Subgroup it belongs to.
    GAF_CMD_BAP_BC_SRC_SET_STREAM,
    /// Remove a Broadcast Group.
    GAF_CMD_BAP_BC_SRC_REMOVE_GROUP,
    /// Enable Periodic Advertising for a Broadcast Group.
    GAF_CMD_BAP_BC_SRC_ENABLE_PA,
    /// Enable a Broadcast Group.
    GAF_CMD_BAP_BC_SRC_ENABLE,
    /// Start transmission of audio data on either all streams or on a subset of streams within a Broadcast Group.
    GAF_CMD_BAP_BC_SRC_START_STREAMING,
    /// Stop transmission of audio data for either all streams or for a subset of streams within a Broadcast Group.
    GAF_CMD_BAP_BC_SRC_STOP_STREAMING,
    /// Disable Periodic Advertising for a Broadcast Group.
    GAF_CMD_BAP_BC_SRC_DISABLE_PA,
    /// Disable a Broadcast Group.
    GAF_CMD_BAP_BC_SRC_DISABLE,
    /// Get the tx sync information of the specified stream
    GAF_CMD_BAP_BC_SRC_GET_TX_SYNC,
};

/// List of GAF_CMD command codes for Basic Audio Profile - Broadcast Sink
enum gaf_bap_bc_sink_cmd_code
{
    /// Enable a Broadcast Group Sink - it creates a corresponding Broadcast group.
    GAF_CMD_BAP_BC_SINK_ENABLE          = 0,
    /// Disable a Broadcast Group Sink - it removes Broadcast group and included streams.
    GAF_CMD_BAP_BC_SINK_DISABLE,
    /// Start reception of audio data onto a Broadcast stream.
    GAF_CMD_BAP_BC_SINK_START_STREAMING,
    /// Stop reception of audio data onto a Broadcast stream.
    GAF_CMD_BAP_BC_SINK_STOP_STREAMING,
};

/// List of GAF_CMD command codes for Basic Audio Profile - Broadcast Scan
enum gaf_bap_bc_scan_cmd_code
{
    /// Set scan parameters to be used for all scan procedures
    GAF_CMD_BAP_BC_SCAN_PARAMS_SET     = 0,
    /// Terminate or cancel establishment of a periodic advertising sync activity
    GAF_CMD_BAP_BC_SCAN_PA_TERMINATE,
    /// Control reception or not of periodic advertising reports
    GAF_CMD_BAP_BC_SCAN_PA_REPORT_CTRL,
    /// Start seeking for Broadcast Audio Announcements sent by Broadcast Source devices.
    GAF_CMD_BAP_BC_SCAN_START,
    /// Stop all Scan procedures currently in progress.
    GAF_CMD_BAP_BC_SCAN_STOP,
    /// Establish a periodic sync using Scanning
    GAF_CMD_BAP_BC_SCAN_PA_SYNCHRONIZE,
    /// Establish a periodic sync using connection
    GAF_CMD_BAP_BC_SCAN_PA_SYNCHRONIZE_CON,
};

/// List of GAF_CFM request indication codes for Basic Audio Profile - Broadcast Sink
enum gaf_bap_bc_sink_cfm_code
{
    /// Request to enable a Broadcast Group Sink - it creates a corresponding Broadcast group.
    GAF_CFM_BAP_BC_SINK_CFM_ENABLE       = 0,
    /// Request to disable a Broadcast Group Sink - it removes Broadcast group and included streams.
    GAF_CFM_BAP_BC_SINK_CFM_DISABLE,
};

/// List of GAF_CFM request indication codes for Basic Audio Profile - Broadcast Scan
enum gaf_bap_bc_scan_cfm_code
{
    /// Scan delegator receives request to establish a periodic advertising sync
    GAF_CFM_BAP_BC_SCAN_CFM_PA_SYNCHRONIZE      = 0,
    /// Scan delegator receives request to terminate a periodic advertising sync
    GAF_CFM_BAP_BC_SCAN_CFM_PA_TERMINATE,
};

/// List of GAF_CMD command codes for BAP PACS Client
enum gaf_bap_uc_capa_cli_cmd_code
{
    /// Used to resotre the PACS service configuration of the UC server during reconnection without rediscovering the service
    GAF_CMD_BAP_UC_CAPA_CLI_RESTORE_BOND_DATA = 0,
    /// Discovery of PACS service on UC server
    GAF_CMD_BAP_UC_CAPA_CLI_DISCOVER,
    /// Get the characteristic value of specified characteristic
    GAF_CMD_BAP_UC_CAPA_CLI_GET_CHAR,
    /// Change the characteristic value of Audio Locations for the peer service
    GAF_CMD_BAP_UC_CAPA_CLI_SET_LOCATION,
};

/// List of GAF_CMD command codes for BAP PACS Server
enum gaf_bap_uc_capa_srv_cmd_code
{
    /// Change the characteristic value of specified PAC 
    GAF_CMD_BAP_UC_CAPA_SRV_SET_RECORD = 0,
    /// Remove the characteristic value of specified PAC 
    GAF_CMD_BAP_UC_CAPA_SRV_REMOVE_RECORD,
    /// Reset Local PACS service vaule
    /// Used to resotre the PACS configuration during reconnection without rediscovering the service
    GAF_CMD_BAP_UC_CAPA_SRV_RESTORE_BOND_DATA,
    /// Change the characteristic value of Audio Locations
    GAF_CMD_BAP_UC_CAPA_SRV_SET_LOCATION,
    /// Change the characteristic value of Available Audio Contexts
    GAF_CMD_BAP_UC_CAPA_SRV_SET_CONTEXT,
};

/// List of GAF_CMD command codes for BAP ASCS Client
enum gaf_bap_uc_ascs_cli_cmd_code
{
    /// Used to resotre the ASCS configuration of the UC server during reconnection without rediscovering the service
    GAF_CMD_BAP_UC_ASCS_CLI_RESTORE_BOND_DATA = 0,
    /// Used to resotre the configuration of the codec without reconfiguring the codec
    GAF_CMD_BAP_UC_ASCS_CLI_RESTORE_BOND_DATA_CODEC,
    /// Used to initialize CIG parameters
    GAF_CMD_BAP_UC_ASCS_CLI_CREATE_GROUP,
    /// Used to remove a specified CIG
    GAF_CMD_BAP_UC_ASCS_CLI_REMOVE_GROUP,
    /// Discovery of ASCS service on UC server
    GAF_CMD_BAP_UC_ASCS_CLI_DISCOVER,
    /// Used to request a codec configuration with the server
    GAF_CMD_BAP_UC_ASCS_CLI_CONFIGURE_CODEC,
    /// Used to request a CIS configuration preference with the server and to assign identifiers to the CIS. 
    GAF_CMD_BAP_UC_ASCS_CLI_CONFIGURE_QOS,
    /// Used to request the server to enable an ASE and to provide any Metadata applicable for that ASE. 
    GAF_CMD_BAP_UC_ASCS_CLI_ASE_ENABLE,
    /// Used to provide the server with Metadata to be applied to an ASE
    GAF_CMD_BAP_UC_ASCS_CLI_UPDATE_METADATA,
    /// Used to request the server to disable an ASE
    GAF_CMD_BAP_UC_ASCS_CLI_ASE_DISABLE,
    /// Used to request the server to release an ASE and all resources associated with that ASE.  
    GAF_CMD_BAP_UC_ASCS_CLI_ASE_RELEASE,
    /// Get the quality of the specified ASE
    GAF_CMD_BAP_UC_ASCS_CLI_GET_QUALITY,
    /// Get the tx sync information of the specified ASE
    GAF_CMD_BAP_UC_ASCS_CLI_GET_TX_SYNC,
};

/// List of GAF_CMD command codes for BAP ASCS Server
enum gaf_bap_uc_ascs_srv_cmd_code
{
    /// Used to resotre the ASCS configuration during reconnection without rediscovering the service
    GAF_CMD_BAP_UC_ASCS_SRV_RESTORE_BOND_DATA = 0,
    /// Used to resotre the configuration of the codec without reconfiguring the codec
    GAF_CMD_BAP_UC_ASCS_SRV_RESTORE_BOND_DATA_CODEC,
    /// Used to request a codec configuration and send a notification to the client
    GAF_CMD_BAP_UC_ASCS_SRV_CONFIGURE_CODEC,
    /// Used to provide the client with Metadata to be applied to an ASE
    GAF_CMD_BAP_UC_ASCS_SRV_UPDATE_METADATA,
    /// Used for server to disable an ASE
    GAF_CMD_BAP_UC_ASCS_SRV_ASE_DISABLE,
    /// Used for server to release an ASE and all resources associated with that ASE
    GAF_CMD_BAP_UC_ASCS_SRV_ASE_RELEASE,
    /// Get the quality of the specified ASE
    GAF_CMD_BAP_UC_ASCS_SRV_GET_QUALITY,
    /// Get the tx sync information of the specified ASE
    GAF_CMD_BAP_UC_ASCS_SRV_GET_TX_SYNC,
};

/// List of GAF_CFM command codes for BAP ASCS Server
enum gaf_bap_uc_ascs_srv_cfm_code
{
    /// Used to confirm the request for a codec configuration sent by the client
    GAF_CFM_BAP_UC_ASCS_SRV_CFM_CONFIGURE_CODEC = 0,
    /// Used to confirm the request to enable an ASE sent by the client
    GAF_CFM_BAP_UC_ASCS_SRV_CFM_ASE_ENABLE,
    /// Used to confirm the metadata request sent by the client
    GAF_CFM_BAP_UC_ASCS_SRV_CFM_UPDATE_METADATA,
    /// Used to confirm the request to release an ASE sent by the client
    GAF_CFM_BAP_UC_ASCS_SRV_CFM_ASE_RELEASE,
};

/// Supported Roles bit field meaning
enum gaf_bap_role_bf
{
    /// Capabilities Server supported
    GAF_BAP_ROLE_SUPP_UC_CAPA_SRV_BIT = 0x0001,
    GAF_BAP_ROLE_SUPP_UC_CAPA_SRV_POS = 0,
    /// Capabilities Client supported
    GAF_BAP_ROLE_SUPP_UC_CAPA_CLI_BIT = 0x0002,
    GAF_BAP_ROLE_SUPP_UC_CAPA_CLI_POS = 1,
    /// Unicast Server supported
    GAF_BAP_ROLE_SUPP_UC_ASCS_SRV_BIT = 0x0004,
    GAF_BAP_ROLE_SUPP_UC_ASCS_SRV_POS = 2,
    /// Unicast Client supported
    GAF_BAP_ROLE_SUPP_UC_ASCS_CLI_BIT = 0x0008,
    GAF_BAP_ROLE_SUPP_UC_ASCS_CLI_POS = 3,
    /// Broadcast Source supported
    GAF_BAP_ROLE_SUPP_BC_SRC_BIT = 0x0010,
    GAF_BAP_ROLE_SUPP_BC_SRC_POS = 4,
    /// Broadcast Sink supported
    GAF_BAP_ROLE_SUPP_BC_SINK_BIT = 0x0020,
    GAF_BAP_ROLE_SUPP_BC_SINK_POS = 5,
    /// Broadcast Scan supported
    GAF_BAP_ROLE_SUPP_BC_SCAN_BIT = 0x0040,
    GAF_BAP_ROLE_SUPP_BC_SCAN_POS = 6,
    /// Broadcast Scan Assistant supported
    GAF_BAP_ROLE_SUPP_BC_ASSIST_BIT = 0x0080,
    GAF_BAP_ROLE_SUPP_BC_ASSIST_POS = 7,
    /// Broadcast Scan Delegator supported
    GAF_BAP_ROLE_SUPP_BC_DELEG_BIT  = 0x0100,
    GAF_BAP_ROLE_SUPP_BC_DELEG_POS  = 8,
};

/// Bit Field of periodic advertising information that is supposed to be report by scan module
/// Filtering level to report information about basic audio announcement present in periodic advertising report
enum gaf_bap_bc_scan_report_filter_bf
{
    /// Report Periodic advertising data (in raw format)
    GAF_BAP_BC_SCAN_REPORT_PER_ADV_DATA_BIT     = 0x01,
    GAF_BAP_BC_SCAN_REPORT_PER_ADV_DATA_POS     = 0,
    /// Report BIG Info present in ACAD data of a periodic advertising report
    GAF_BAP_BC_SCAN_REPORT_BIG_INFO_BIT         = 0x02,
    GAF_BAP_BC_SCAN_REPORT_BIG_INFO_POS         = 1,
    /// Report Broadcast Group information of basic audio announcement present in periodic advertising report
    GAF_BAP_BC_SCAN_REPORT_ANNOUNCE_LVL_1_BIT   = 0x04,
    GAF_BAP_BC_SCAN_REPORT_ANNOUNCE_LVL_1_POS   = 2,
    /// Report Broadcast SubGroups information of basic audio announcement present in periodic advertising report
    GAF_BAP_BC_SCAN_REPORT_ANNOUNCE_LVL_2_BIT   = 0x08,
    GAF_BAP_BC_SCAN_REPORT_ANNOUNCE_LVL_2_POS   = 3,
    /// Report Broadcast Stream information of basic audio announcement present in periodic advertising report
    GAF_BAP_BC_SCAN_REPORT_ANNOUNCE_LVL_3_BIT   = 0x10,
    GAF_BAP_BC_SCAN_REPORT_ANNOUNCE_LVL_3_POS   = 4,
};

/// Broadcast group sink state
enum gaf_bap_bc_sink_state
{
    /// Broadcast Sink establishment succeed
    GAF_BAP_BC_SINK_STATE_ESTABLISHED     = 0x00,
    /// Broadcast Sink establishment failed due to LL error
    GAF_BAP_BC_SINK_STATE_FAILED,
    /// Broadcast Sink establishment canceled
    GAF_BAP_BC_SINK_STATE_CANCELLED,
    /// Broadcast Sink connection lost
    GAF_BAP_BC_SINK_STATE_LOST,
    /// Broadcast Sink connection stop due to peer termination
    GAF_BAP_BC_SINK_STATE_PEER_TERMINATE,
    /// Broadcast Sink connection stop due to upper layer termination
    GAF_BAP_BC_SINK_STATE_UPPER_TERMINATE,
    /// Broadcast Sink connection stop due to an encryption error
    GAF_BAP_BC_SINK_STATE_MIC_FAILURE,
};

/// ASE Direction
enum gaf_hl_direction
{
    /// Sink direction
    GAF_HL_DIRECTION_SINK = 0,
    /// Source direction
    GAF_HL_DIRECTION_SRC,

    GAF_HL_DIRECTION_MAX,
};

/// Context type values
enum gaf_bap_uc_capa_context_type
{
    /// Supported Audio Contexts
    GAF_BAP_UC_CAPA_CONTEXT_TYPE_SUPP = 0,
    /// Available Audio Contexts
    GAF_BAP_UC_CAPA_CONTEXT_TYPE_AVA,

    GAF_BAP_UC_CAPA_CONTEXT_TYPE_MAX
};

/// Characteristic type values for Published Audio Capabilities Service
enum gaf_bap_uc_capa_char_type
{
    /// Available Audio Contexts characteristic
    GAF_BAP_UC_CAPA_CHAR_TYPE_CONTEXT_AVA = 0,
    /// Supported Audio Contexts characteristic
    GAF_BAP_UC_CAPA_CHAR_TYPE_CONTEXT_SUPP,
    /// Sink Audio Locations characteristic
    GAF_BAP_UC_CAPA_CHAR_TYPE_LOC_SINK,
    /// Source Audio Locations characteristic
    GAF_BAP_UC_CAPA_CHAR_TYPE_LOC_SRC,
    /// PAC characteristic
    GAF_BAP_UC_CAPA_CHAR_TYPE_PAC,

    GAF_BAP_UC_CAPA_CHAR_TYPE_MAX,
};

/// List of Audio Stream Control Service characteristics
enum gaf_bap_uc_char_type
{
    /// ASE Control Point characteristic
    GAF_BAP_UC_CHAR_TYPE_CP = 0,
    /// ASE characteristic
    GAF_BAP_UC_CHAR_TYPE_ASE,

    GAF_BAP_UC_CHAR_TYPE_MAX,
};

/// ASE State values
enum gaf_bap_uc_ase_state
{
    /// Idle
    GAF_BAP_UC_ASE_STATE_IDLE = 0,
    /// Codec configured
    GAF_BAP_UC_ASE_STATE_CODEC_CONFIGURED,
    /// QoS configured
    GAF_BAP_UC_ASE_STATE_QOS_CONFIGURED,
    /// Enabling
    GAF_BAP_UC_ASE_STATE_ENABLING,
    /// Streaming
    GAF_BAP_UC_ASE_STATE_STREAMING,
    /// Disabling
    GAF_BAP_UC_ASE_STATE_DISABLING,
    /// Releasing
    GAF_BAP_UC_ASE_STATE_RELEASING,

    GAF_BAP_UC_ASE_STATE_MAX,
};

/// Target Latency values
enum gaf_bap_uc_tgt_latency
{
    GAF_BAP_UC_TGT_LATENCY_MIN = 1,

    /// Target lower latency
    GAF_BAP_UC_TGT_LATENCY_LOWER = GAF_BAP_UC_TGT_LATENCY_MIN,
    /// Target balanced latency and reliability
    GAF_BAP_UC_TGT_LATENCY_BALENCED,
    /// Target higher reliability
    GAF_BAP_UC_TGT_LATENCY_RELIABLE,

    GAF_BAP_UC_TGT_LATENCY_MAX,
};

/// Target PHY values
enum gaf_bap_uc_tgt_phy
{
    GAF_BAP_UC_TGT_PHY_MIN = 1,

    /// 1M PHY
    GAF_BAP_UC_TGT_PHY_1M = GAF_BAP_UC_TGT_PHY_MIN,
    /// 2M PHY
    GAF_BAP_UC_TGT_PHY_2M,
    /// Coded PHY
    GAF_BAP_UC_TGT_PHY_CODED,

    GAF_BAP_UC_TGT_PHY_MAX,
};


enum gaf_bap_uc_cis_state 
{
    /// CIS disconnect
    GAF_BAP_UC_CIS_STATE_LOST,
    /// CIS establishment
    GAF_BAP_UC_CIS_STATE_CON,
}; 

/// Codec Identifier
typedef struct gaf_codec_id
{
    /// Codec ID value
    uint8_t codec_id[GAF_CODEC_ID_LEN];
} gaf_codec_id_t;

/// LC3 Parameters structure
typedef struct gaf_bap_lc3_param
{
    /// Audio Locations of audio channels being configured for the codec
    uint32_t location_bf;
    /// Number of octets for a codec frame
    uint16_t frame_octet;
    /// Sampling frequency
    uint8_t sampling_freq;
    /// Frame duration
    uint8_t frame_dur;
    /// Number of blocks of LC3 codec frames per SDU
    uint8_t nb_lc3_frames;
} gaf_bap_lc3_param_t;

/// LC3 Configuration structure
typedef struct gaf_bap_lc3_cfg
{
    /// LC3 parameters
    gaf_bap_lc3_param_t params;
    /// Length of Codec Configuration value
    uint8_t cfg_len;
    /// Codec Configuration value (in LTV format)
    uint8_t *cfg;
} gaf_bap_lc3_cfg_t;

/// QoS Requirement structure
typedef struct bap_qos_req
{
    /// Preferred Presentation delay minimum in microseconds
    uint32_t pref_pres_delay_min_us;
    /// Preferred Presentation delay maximum in microseconds
    uint32_t pref_pres_delay_max_us;
    /// Presentation delay minimum in microseconds
    uint32_t pres_delay_min_us;
    /// Presentation delay maximum in microseconds
    uint32_t pres_delay_max_us;
    /// Preferred Transport latency maximum in milliseconds
    /// From 5ms (0x5) to 4000ms (0xFA0)
    uint16_t trans_latency_max_ms;
    /// Preferred PDU framing arrangement
    uint8_t framing;
    /// Preferred PHY bit field
    uint8_t phy_bf;
    /// Preferred maximum number of retransmissions for each CIS Data PDU
    /// From 0 to 15
    uint8_t retx_nb;
} bap_qos_req_t;

/// Broadcast code used for stream encryption
typedef struct gaf_bcast_code_t
{
    /// Broadcast Code value
    uint8_t bcast_code[GAF_KEY_LEN];
} gaf_bcast_code_t;

typedef struct gaf_iso_data {
    /// Packet status, meaningful only for receiving iso data
    bool     valid;
    /// SDU sequence number
    uint16_t sdu_seq;
    /// Data length
    uint16_t length;
    /// ts is valid or not
    bool ts_exist;
    /// Time stamp
    uint32_t ts;
    /// Data buffer
    uint8_t *buffer;
} gaf_iso_data_t;

typedef struct gaf_bap_uc_capa_srv_cfg
{
    /// Number of PAC Groups for Sink direction
    uint8_t nb_pacs_sink;
    /// Number of PAC Groups for Source direction
    uint8_t nb_pacs_src;
    /// Required start handle
    /// If set to GATT_INVALID_HDL, the start handle will be automatically chosen
    uint16_t shdl;
    /// Supported Audio Locations bit field for Sink direction
    /// Meaningful only if nb_pac_sink is different than 0
    /// See @defgroup Audio Location Definitions
    uint32_t location_bf_sink;
    /// Supported Audio Locations bit field for Source direction
    /// Meaningful only if nb_pac_src is different than 0
    /// See @defgroup Audio Location Definitions
    uint32_t location_bf_src;
    /// Supported Audio Contexts bit field for Sink direction
    /// Meaningful only if nb_pac_sink is different than 0
    /// See @defgroup Context Type Definitions
    uint16_t supp_context_bf_sink;
    /// Supported Audio Contexts bit field for Source direction
    /// Meaningful only if nb_pac_src is different than 0
    /// See @defgroup Context Type Definitions
    uint16_t supp_context_bf_src;
} gaf_bap_uc_capa_srv_cfg_t;

typedef struct gaf_bap_uc_capa_cli_cfg
{
    /// Reserved
    /// fix IAR compile error
    uint32_t dummpy;
} gaf_bap_uc_capa_cli_cfg_t;

typedef struct gaf_bap_uc_ascs_srv_cfg
{
    /// Number of instances of the ASE characteristic
    /// Shall be in the range [0, 15]
    uint8_t nb_ase_chars;
    /// Number of ASE configurations that can be maintained
    /// Shall be at least equal to nb_ase_chars
    /// Should be a multiple of nb_ase_chars
    /// Shall not be larger than nb_ase_chars * BLE_CONNECTION_MAX
    uint8_t nb_ases_cfg;
    /// Required start handle
    /// If set to GATT_INVALID_HDL, the start handle will automatically chosen
    uint16_t shdl;
    /// Direction bit field
    /// Each bit represents an ASE
    /// 0 mean sink, 1 mean source
    uint16_t direction_bf;
} gaf_bap_uc_ascs_srv_cfg_t;

typedef struct gaf_bap_uc_ascs_cli_cfg
{
    /// Number of ASE configurations that can be maintained
    /// Shall be at least equal to nb_ase_chars
    /// Should be a multiple of nb_ase_chars
    /// Shall not be larger than nb_ase_chars * BLE_CONNECTION_MAX
    uint8_t nb_ases_cfg;
} gaf_bap_uc_ascs_cli_cfg_t;

typedef struct gaf_bap_bc_scan_cfg
{
    /// Number of Broadcast Source information that may be stored in the cache
    uint8_t cache_size;
    /// Number of Periodic Synchronizations that may be established in parallel
    uint8_t nb_sync;
} gaf_bap_bc_scan_cfg_t;

typedef struct gaf_configure_param
{
    uint32_t role_bf;
    /// Capabilities Server configuration
    gaf_bap_uc_capa_srv_cfg_t capa_srv_cfg;
    /// Capabilities Client configuration
    gaf_bap_uc_capa_cli_cfg_t capa_cli_cfg;
    /// Unicast Server configuration
    gaf_bap_uc_ascs_srv_cfg_t uc_ascs_srv_cfg;
    /// Unicast Client configuration
    gaf_bap_uc_ascs_cli_cfg_t uc_ascs_cli_cfg;
    /// Broadcast Scan configuration
    gaf_bap_bc_scan_cfg_t bc_scan_cfg;
    /// Broadcast Delegator configuration
    // gaf_bap_bc_deleg_cfg_t bc_deleg_cfg;
    /// Broadcast Assistant configuration
    // gaf_bap_bc_assist_cfg_t bc_assist_cfg;
} gaf_configure_param_t;

// *******************************************************************************************
// -------------------------------------------------------------------------------------------
// ------------------------ BAP Broadcast Audio API Parameter Define -------------------------
// -------------------------------------------------------------------------------------------
// *******************************************************************************************

/// BIG Info Report
typedef struct gaf_big_info
{
    /// Value of the SDU interval in microseconds (Range 0x0000FF-0x0FFFFF)
    uint32_t  sdu_interval;
    /// Value of the ISO Interval (1.25 ms unit)
    uint16_t  iso_interval;
    /// Value of the maximum PDU size (Range 0x0000-0x00FB)
    uint16_t  max_pdu;
    /// VValue of the maximum SDU size (Range 0x0000-0x0FFF)
    uint16_t  max_sdu;
    /// Number of BIS present in the group (Range 0x01-0x1F)
    uint8_t   num_bis;
    /// Number of sub-events (Range 0x01-0x1F)
    uint8_t   nse;
    /// Burst number (Range 0x01-0x07)
    uint8_t   bn;
    /// Pre-transmit offset (Range 0x00-0x0F)
    uint8_t   pto;
    /// Initial retransmission count (Range 0x01-0x0F)
    uint8_t   irc;
    /// PHY used for transmission (0x01: 1M, 0x02: 2M, 0x03: Coded, All other values: RFU)
    uint8_t   phy;
    /// Framing mode (0x00: Unframed, 0x01: Framed, All other values: RFU)
    uint8_t   framing;
    /// True if broadcast isochronous group is encrypted, False otherwise
    bool      encrypted;
} gaf_big_info_t;

typedef struct gaf_big_config
{
    /// Transmission delay time in microseconds of all BISs in the BIG (in us range 0x0000EA-0x7FFFFF)
    uint32_t sync_delay_us;
    /// The maximum delay time, in microseconds, for transmission of SDUs of all BISes
    /// (in us range 0x0000EA-0x7FFFFF)
    uint32_t tlatency_us;
    /// PHY used, bit 0: 1Mbps, bit 1: 2Mbps, bit 2: LE-Coded
    uint8_t phy;
    /// The number of subevents in each BIS event in the BIG, range 0x01-0x1E
    uint8_t nse;
    /// The number of new payloads in each BIS event, range 0x01-0x07
    uint8_t bn;
    /// Offset used for pre-transmissions, range 0x00-0x0F
    uint8_t pto;
    /// The number of times a payload is transmitted in a BIS event, range 0x01-0x0F
    uint8_t irc;
    /// Maximum size of the payload in octets, range 0x00-0xFB
    uint8_t max_pdu;
} gaf_big_config_t;

/// Broadcast Group Parameters structure
typedef struct bap_bc_grp_param
{
    /// SDU interval in microseconds
    uint32_t sdu_intv_us;
    /// Maximum size of an SDU
    uint16_t max_sdu;
    /// Maximum time (in milliseconds) between the first transmission of an SDU to the end of the last transmission
    /// of the same SDU
    uint16_t max_tlatency;
    /// Sequential or Interleaved scheduling
    uint8_t packing;
    /// Unframed or framed mode
    uint8_t framing;
    /// Bitfield indicating PHYs that can be used by the controller for transmission of SDUs
    uint8_t phy_bf;
    /// Number of times every PDU should be transmitted
    uint8_t rtn;
} bap_bc_grp_param_t;

/// Advertising Parameters structure
typedef struct bap_bc_adv_param
{
    /// Minimum advertising interval in multiple of 0.625ms. Must be higher than 20ms.
    uint32_t adv_intv_min_slot;
    /// Maximum advertising interval in multiple of 0.625ms. Must be higher than 20ms.
    uint32_t adv_intv_max_slot;
    /// Channel Map. see(@enum gap_adv_chan)
    uint8_t chnl_map;
    /// PHY for primary advertising. Only LE 1M and LE Codec PHYs are allowed
    /// see(@enum gapm_phy_type)
    uint8_t phy_prim;
    /// PHY for secondary advertising. see(@enum gapm_phy_type)
    uint8_t phy_second;
    /// Advertising SID
    uint8_t adv_sid;
} bap_bc_adv_param_t;

/// Periodic Advertising Parameters structure
typedef struct bap_bc_per_adv_param
{
    /// Minimum periodic advertising interval in multiple of 1.25ms. Must be higher than 7.5ms
    uint32_t adv_intv_min_frame;
    /// Maximum periodic advertising interval in multiple of 1.25ms. Must be higher than 7.5ms
    uint32_t adv_intv_max_frame;
} bap_bc_per_adv_param_t;

typedef struct gaf_bap_bc_src_grpup_add_cmd {
    /// Number of Streams in the Broadcast Group. Cannot be 0
    uint8_t                nb_streams;
    /// Number of Subgroups in the Broadcast Group. Cannot be 0
    uint8_t                nb_subgroups;
    /// Broadcast Group parameters
    bap_bc_grp_param_t     grp_param;
    /// Advertising parameters
    bap_bc_adv_param_t     adv_param;
    /// Periodic Advertising parameters
    bap_bc_per_adv_param_t per_adv_param;
} gaf_bap_bc_src_grpup_add_cmd_t;

typedef struct gaf_bap_bc_src_group_set_cmd
{
    /// Group local index
    uint8_t        grp_lid;
    /// Audio output presentation delay in microseconds
    uint32_t       pres_delay_us;
    /// Codec ID value
    gaf_codec_id_t codec_id;
    /// Length of Codec Configuration value
    uint8_t        cfg_len;
    /// Codec Configuration value
    /// Codec Configuration part to be casted as bap_lc3_cfg_t for LC3 codec
    uint8_t        *cfg;
    /// Length of Metadata value
    uint8_t        metadata_len;
    /// Metadata value
    uint8_t       *metadata;
} gaf_bap_bc_src_group_set_cmd_t;

typedef struct gaf_bap_bc_src_subgroup_set_cmd
{
    /// Group local index
    uint8_t        grp_lid;
    /// Subgroup local index
    uint8_t        sgrp_lid;
    /// Length of Metadata value for the subgroup
    uint8_t        metadata_len;
    /// Metadata value for the subgroup (in LTV format)
    uint8_t        *metadata;
} gaf_bap_bc_src_subgroup_set_cmd_t;

typedef struct gaf_bap_bc_src_stream_set_cmd
{
    /// Group local index
    uint8_t        grp_lid;
    /// Subgroup local index the Stream belongs to
    uint8_t        sgrp_lid;
    /// Stream local index
    uint8_t        stream_lid;
    /// Controller delay in microseconds
    uint32_t       ctl_delay_us;
    /// Length of Codec Configuration value for the Stream
    uint8_t        cfg_len;
    /// Codec Configuration value for the Stream (in LTV format)
    uint8_t        *cfg;
} gaf_bap_bc_src_stream_set_cmd_t;

typedef struct gaf_bap_bc_src_grpup_remove_cmd {
    /// Group local index
    uint8_t        grp_lid;
} gaf_bap_bc_src_grpup_remove_cmd_t;

typedef struct gaf_bap_bc_src_enable_pa_cmd
{
    /// Group local index
    uint8_t        grp_lid;
    /// Length of additional advertising data
    uint8_t        adv_data_len;
    /// Advertising data
    uint8_t        *data;
    /// Length of additional periodic advertising data
    uint8_t        per_adv_data_len;
    /// Periodic advertising data
    uint8_t        *per_data;
} gaf_bap_bc_src_enable_pa_cmd_t;

typedef struct gaf_bap_bc_src_enable_cmd
{
    /// Group local index
    uint8_t             grp_lid;
    /// Indicate if streams are encrypted (!= 0) or not
    uint8_t             encrypted;
    /// Broadcast code. Meaningful only if encrypted parameter indicates that streams are encrypted
    gaf_bcast_code_t    bcast_code;
} gaf_bap_bc_src_enable_cmd_t;

typedef struct gaf_bap_bc_src_streaming_start_cmd
{
    /// Group local index
    uint8_t             grp_lid;
    /// Stream local index bit field indicating for which stream streaming must be started
    /// 0xFFFFFFFF means that streaming must be started for all BISes
    uint32_t            stream_lid_bf;
} gaf_bap_bc_src_streaming_start_cmd_t;

typedef struct gaf_bap_bc_src_streaming_stop_cmd
{
    /// Group local index
    uint8_t             grp_lid;
    /// Stream local index bit field indicating for which stream streaming must be stopped
    /// 0xFFFFFFFF means that streaming must be stopped for all BISes
    uint32_t            stream_lid_bf;
} gaf_bap_bc_src_streaming_stop_cmd_t;

typedef struct gaf_bap_bc_src_disable_pa_cmd
{
    /// Group local index
    uint8_t             grp_lid;
} gaf_bap_bc_src_disable_pa_cmd_t;

typedef struct gaf_bap_bc_src_disable_cmd
{
    /// Group local index
    uint8_t             grp_lid;
} gaf_bap_bc_src_disable_cmd_t;

typedef struct gaf_bap_bc_src_get_tx_sync_cmd
{
    /// Group local index
    uint8_t             grp_lid;
    /// Stream local index
    uint8_t             stream_lid;
} gaf_bap_bc_src_get_tx_sync_cmd_t;

typedef struct gaf_bap_bc_src_cmd
{
    /// Confirm code (@see enum gaf_bap_bc_src_cmd_code)
    enum gaf_bap_bc_src_cmd_code cmd_code;
    union {
        gaf_bap_bc_src_grpup_add_cmd_t group_add;
        gaf_bap_bc_src_group_set_cmd_t group_set;
        gaf_bap_bc_src_subgroup_set_cmd_t subgroup_set;
        gaf_bap_bc_src_stream_set_cmd_t stream_set;
        gaf_bap_bc_src_grpup_remove_cmd_t group_remove;

        gaf_bap_bc_src_enable_pa_cmd_t pa_enable;
        gaf_bap_bc_src_enable_cmd_t enable;
        gaf_bap_bc_src_streaming_start_cmd_t streaming_start;
        gaf_bap_bc_src_streaming_stop_cmd_t streaming_stop;
        gaf_bap_bc_src_disable_pa_cmd_t pa_disable;
        gaf_bap_bc_src_disable_cmd_t disable;
        gaf_bap_bc_src_get_tx_sync_cmd_t get_tx_sync;
    } p;
} gaf_bap_bc_src_cmd_t;

typedef struct gaf_bap_bc_sink_enable_cmd
{
    /// Periodic Advertising local index
    uint8_t             pa_lid;
    /// Maximum number of subevents the controller should use to receive data payloads in each interval
    uint8_t             mse;
    /// Stream position bit field indicating streams to synchronize with.
    uint32_t            stream_pos_bf;
    /// Timeout duration (10ms unit) before considering synchronization lost (Range 100 ms to 163.84 s).
    uint16_t            timeout_ms;
    /// Indicate if streams are encrypted (!= 0) or not
    uint8_t             encrypted;
    /// Broadcast code. Meaningful only if encrypted parameter indicates that streams are encrypted
    gaf_bcast_code_t    bcast_code;
} gaf_bap_bc_sink_enable_cmd_t;

typedef struct gaf_bap_bc_sink_disable_cmd
{
    /// Group local index
    uint8_t             grp_lid;
} gaf_bap_bc_sink_disable_cmd_t;

typedef struct gaf_bap_bc_sink_start_streaming_cmd
{
    /// Group local index
    uint8_t             grp_lid;
    /// Position of the stream in the group (range 1 to 32)
    uint8_t             stream_pos;
    /// Controller delay in microseconds
    uint32_t            ctl_delay_us;
    /// Codec ID value
    gaf_codec_id_t      codec_id;
    /// Length of Codec Configuration value - in bytes
    uint8_t             cfg_len;
    /// Codec Configuration - to be casted as bap_lc3_cfg_t for LC3 codec.
    /// Array of bytes - 32-bit aligned to be casted as a SW structure
    uint32_t            *cfg;
} gaf_bap_bc_sink_start_streaming_cmd_t;

typedef struct gaf_bap_bc_sink_stop_streaming_cmd
{
    /// Group local index
    uint8_t             grp_lid;
    /// Position of the stream in the group (range 1 to 32)
    uint8_t             stream_pos;
} gaf_bap_bc_sink_stop_streaming_cmd_t;

typedef struct gaf_bap_bc_sink_cmd
{
    /// Confirm code (@see enum gaf_bap_bc_sink_cmd_code)
    enum gaf_bap_bc_sink_cmd_code cmd_code;
    union {
        gaf_bap_bc_sink_enable_cmd_t enable;
        gaf_bap_bc_sink_disable_cmd_t disable;
        gaf_bap_bc_sink_start_streaming_cmd_t streaming_start;
        gaf_bap_bc_sink_stop_streaming_cmd_t streaming_stop;
    } p;
} gaf_bap_bc_sink_cmd_t;

typedef struct gaf_bap_bc_sink_enable_cfm
{
    /// Group local index
    uint8_t             grp_lid;
    /// True to accept broadcast sink establishment, False otherwise
    bool                accept;
    /// Timeout duration (10ms unit) before considering synchronization lost (Range 100 ms to 163.84 s).
    uint16_t            timeout_s;
    /// Maximum number of subevents the controller should use to received data payloads in each interval
    uint8_t             mse;
} gaf_bap_bc_sink_enable_cfm_t;

typedef struct gaf_bap_bc_sink_disable_cfm
{
    /// Group local index
    uint8_t             grp_lid;
    /// True to accept broadcast sink termination, False otherwise
    bool                accept;
} gaf_bap_bc_sink_disable_cfm_t;

typedef struct gaf_bap_bc_sink_cfm
{
    /// Confirm code (@see enum gaf_bap_bc_sink_cfm_code)
    enum gaf_bap_bc_sink_cfm_code cfm_code;
    union {
        gaf_bap_bc_sink_enable_cfm_t enable_cfm;
        gaf_bap_bc_sink_disable_cfm_t disable_cfm;
    } p;
} gaf_bap_bc_sink_cfm_t;

typedef struct gaf_bap_bc_scan_param
{
    /// Scan interval for LE 1M PHY in multiple of 0.625ms - Must be higher than 2.5ms
    uint16_t intv_1m_slot;
    /// Scan interval for LE Codec PHY in multiple of 0.625ms - Must be higher than 2.5ms
    uint16_t intv_coded_slot;
    /// Scan window for LE 1M PHY in multiple of 0.625ms - Must be higher than 2.5ms
    uint16_t wd_1m_slot;
    /// Scan window for LE Codec PHY in multiple of 0.625ms - Must be higher than 2.5ms
    uint16_t wd_coded_slot;
} gaf_bap_bc_scan_param_t;

typedef struct gaf_bap_bc_scan_params_set_cmd
{
    /// Scan parameters
    gaf_bap_bc_scan_param_t scan_param;
} gaf_bap_bc_scan_params_set_cmd_t;

typedef struct gaf_bap_bc_scan_pa_terminate_cmd
{
    /// Periodic Advertising local index
    uint8_t              pa_lid;
} gaf_bap_bc_scan_pa_terminate_cmd_t;

typedef struct gaf_bap_bc_scan_pa_report_ctrl_cmd
{
    /// Periodic Advertising local index
    uint8_t              pa_lid;
    /// Bit field of filtered report types (@see enum gaf_bap_bc_scan_report_filter_bf)
    uint8_t              report_filter_bf;
} gaf_bap_bc_scan_pa_report_ctrl_cmd_t;


typedef struct gaf_bap_bc_scan_start_cmd
{
    /// Timeout duration in seconds
    uint16_t       timeout_s;
} gaf_bap_bc_scan_start_cmd_t;

typedef struct gaf_bap_bc_scan_pa_synchronize_cmd
{
    /// Device Address
    struct gap_ble_addr addr;
    /// Advertising SID
    uint8_t             adv_sid;
    /// Number of periodic advertising that can be skipped after a successful receive.
    /// Maximum authorized value is 499
    uint16_t             skip;
    /// Bit field of filtered report types (@see enum gaf_bap_bc_scan_report_filter_bf)
    uint8_t              report_filter_bf;
    /// Synchronization timeout for the periodic advertising (in unit of 10ms between 100ms and 163.84s)
    uint16_t             sync_to;
    /// Scan Timeout in seconds
    uint16_t             timeout_s;
} gaf_bap_bc_scan_pa_synchronize_cmd_t;

typedef struct gaf_bap_bc_scan_pa_synchronize_con_cmd
{
    /// Connection local identifier
    uint8_t              conidx;
    /// Number of periodic advertising that can be skipped after a successful receive.
    /// Maximum authorized value is 499
    uint16_t             skip;
    /// Bit field of filtered report types (@see enum gaf_bap_bc_scan_report_filter_bf)
    uint8_t              report_filter_bf;
    /// Synchronization timeout for the periodic advertising (in unit of 10ms between 100ms and 163.84s)
    uint16_t             sync_to;
} gaf_bap_bc_scan_pa_synchronize_con_cmd_t;

typedef struct gaf_bap_bc_scan_cmd
{
    /// Confirm code (@see enum gaf_bap_bc_scan_cmd_code)
    enum gaf_bap_bc_scan_cmd_code cmd_code;
    union {
        gaf_bap_bc_scan_params_set_cmd_t scan_param_set;
        gaf_bap_bc_scan_start_cmd_t scan_start;
        gaf_bap_bc_scan_pa_synchronize_cmd_t pa_sync;
        gaf_bap_bc_scan_pa_synchronize_con_cmd_t pa_sync_by_con;
        gaf_bap_bc_scan_pa_terminate_cmd_t pa_terminate;
        gaf_bap_bc_scan_pa_report_ctrl_cmd_t pa_report_ctrl;
    } p;
} gaf_bap_bc_scan_cmd_t;

typedef struct gaf_bap_bc_scan_pa_synchronize_cfm
{
    /// Periodic Advertising local index
    uint8_t              pa_lid;
    /// True to accept request, False to reject it
    bool                 accept;
    /// Bit field of filtered report types (@see enum gaf_bap_bc_scan_report_filter_bf)
    uint8_t              report_filter_bf;
    /// Number of periodic advertising that can be skipped after a successful receive.
    /// Maximum authorized value is 499
    uint16_t             skip;
    /// Synchronization timeout for the periodic advertising (in unit of 10ms between 100ms and 163.84s)
    uint16_t             sync_to;
    /// Sync establishment timeout in seconds
    uint16_t             timeout_s;
} gaf_bap_bc_scan_pa_synchronize_cfm_t;

typedef struct gaf_bap_bc_scan_pa_terminate_cfm
{
    /// Periodic Advertising local index
    uint8_t              pa_lid;
    /// True to accept request, False to reject it
    bool                 accept;
} gaf_bap_bc_scan_pa_terminate_cfm_t;

typedef struct gaf_bap_bc_scan_cfm
{
    /// Confirm code (@see enum gaf_bap_bc_scan_cfm_code)
    enum gaf_bap_bc_scan_cfm_code cfm_code;
    union {
        gaf_bap_bc_scan_pa_synchronize_cfm_t pa_sync_cfm;
        gaf_bap_bc_scan_pa_terminate_cfm_t pa_terminate_cfm;
    } p;
} gaf_bap_bc_scan_cfm_t;


typedef struct gaf_bap_bc_iso_data
{
    /// Group local index
    uint8_t  grp_lid;
    /// Stream local index
    uint8_t  stream_lid;
    /// Data
    gaf_iso_data_t p;
} gaf_bap_bc_iso_data_t;

// *******************************************************************************************
// -------------------------------------------------------------------------------------------
// --------------------- BAP Broadcast Audio API Parameter Defines END -----------------------
// -------------------------------------------------------------------------------------------
// *******************************************************************************************

// *******************************************************************************************
// ---------------------------------------------------------------------------------------
// ---------------------- BAP Unicast Audio API Parameter Defines ------------------------
// ---------------------------------------------------------------------------------------
// *******************************************************************************************

typedef struct gaf_bap_uc_ascs_cli_disc_cmd
{
    /// Connection local index
    uint8_t conidx;
} gaf_bap_uc_ascs_cli_disc_cmd_t;

typedef struct gaf_bap_uc_cis_quality
{
    /// ASE local index
    uint8_t ase_lid;
    /// Number of packets transmitted and unacked
    uint32_t tx_unacked_packets;
    /// Number of flushed transmitted packets
    uint32_t tx_flushed_packets;
    /// Number of packets transmitted during last subevent
    uint32_t tx_last_subevent_packets;
    /// Number of retransmitted packets
    uint32_t retx_packets;
    /// Number of packets received with a CRC error
    uint32_t crc_error_packets;
    /// Number of unreceived packets
    uint32_t rx_unrx_packets;
    /// Number of duplicate packets received
    uint32_t duplicate_packets;
} gaf_bap_uc_cis_quality_t;

typedef struct gaf_bap_uc_cis_tx_sync
{
    /// ASE local index
    uint8_t ase_lid;
    /// The packet sequence number of an SDU (Range 0x0000-0xFFFF)
    uint16_t pkt_seq_nb;
    /// The time stamp, in microseconds, of the reference anchor point of a transmitted SDU (Range 0x00000000-0xFFFFFFFF)
    uint32_t time_stamp;
    /// The time offset, in microseconds, that is associated with a transmitted SDU (Range 0x000000-0xFFFFFF)
    uint32_t time_offset;
} gaf_bap_uc_cis_tx_sync_t;

typedef struct gaf_bap_uc_ascs_cli_cfg_codec_cmd
{
    /// Connection local index
    uint8_t conidx;
    /// ASE local index
    uint8_t ase_lid;
    /// ASE instance index
    uint8_t ase_instance_idx;
    /// Direction (@see enum gaf_hl_direction)
    uint8_t direction;
    /// Target Latency (@see enum gaf_bap_uc_tgt_latency)
    uint8_t tgt_latency;
    /// Target PHY (@see enum gaf_bap_uc_tgt_phy)
    uint8_t tgt_phy;
    /// Codec ID
    gaf_codec_id_t codec_id;
    /// Length of Codec Configuration value
    uint16_t cfg_len;
    /// Codec Configuration value, length is cfg_len
    uint8_t *cfg;
} gaf_bap_uc_ascs_cli_cfg_codec_cmd_t;

/// QoS Configuration structure (short)
typedef struct gaf_bap_uc_qos_cfg
{
    /// PHY
    uint8_t phy;
    /// Maximum number of retransmissions for each CIS Data PDU
    /// From 0 to 15
    uint8_t retx_nb;
    /// Maximum SDU size
    /// From 0 to 4095 bytes (0xFFF)
    uint16_t max_sdu_size;
    /// Presentation delay in microseconds
    uint32_t pres_delay_us;
} gaf_bap_uc_qos_cfg_t;

typedef struct gaf_bap_uc_ascs_cli_cfg_qos_cmd
{
    /// ASE local index
    uint8_t ase_lid;
    /// Group local index
    uint8_t grp_lid;
    /// CIS ID
    uint8_t cis_id;
    /// QoS Configuration
    gaf_bap_uc_qos_cfg_t qos_cfg;
} gaf_bap_uc_ascs_cli_cfg_qos_cmd_t;

/// Metadata for Codec Configuration structure
typedef struct gaf_bap_metadata_cfg
{
    /// Streaming audio context bit field
    uint16_t context_bf;
    /// Length of Metadata value
    uint8_t metadata_len;
    /// Metadata value
    uint8_t *metadata;
} gaf_bap_metadata_cfg_t;

typedef struct gaf_bap_uc_ascs_cli_ase_enable_cmd
{
    /// ASE local index
    uint8_t ase_lid;
    /// Metadata
    gaf_bap_metadata_cfg_t metadata;
} gaf_bap_uc_ascs_cli_ase_enable_cmd_t;

typedef struct gaf_bap_uc_ascs_cli_update_meta_cmd
{
    /// ASE local index
    uint8_t ase_lid;
    /// Metadata
    gaf_bap_metadata_cfg_t metadata;
} gaf_bap_uc_ascs_cli_update_meta_cmd_t;

typedef struct gaf_bap_uc_ascs_cli_ase_disable_cmd
{
    /// ASE local index
    uint8_t ase_lid;
} gaf_bap_uc_ascs_cli_ase_disable_cmd_t;

typedef struct gaf_bap_uc_ascs_cli_ase_release_cmd
{
    /// ASE local index
    uint8_t ase_lid;
} gaf_bap_uc_ascs_cli_ase_release_cmd_t;

typedef struct gaf_bap_uc_ascs_cli_get_quality_cmd
{
    /// ASE local index
    uint8_t ase_lid;
} gaf_bap_uc_ascs_cli_get_quality_cmd_t;

typedef struct gaf_bap_uc_ascs_cli_get_tx_sync_cmd
{
    /// ASE local index
    uint8_t ase_lid;
} gaf_bap_uc_ascs_cli_get_tx_sync_cmd_t;

typedef struct gaf_bap_uc_char
{
    /// Characteristic value handle
    uint16_t val_hdl;
    /// Client Characteristic Configuration descriptor handle
    uint16_t desc_hdl;
} gaf_bap_uc_char_t;

/// Audio Stream Control Service characteristic description structure
typedef struct gaf_bap_uc_ase_char
{
    /// Characteristic value handle
    uint16_t val_hdl;
    /// Client Characteristic Configuration descriptor handle
    uint16_t desc_hdl;
    /// ASE ID
    uint8_t ase_id;
    /// Direction (@see enum gaf_hl_direction)
    uint8_t direction;
    /// ASE instance index
    uint8_t ase_instance_idx;
} gaf_bap_uc_ase_char_t;

/// Audio Stream Control Service content description structure
typedef struct gaf_bap_uc_ascs_info
{
    /// start handle
    uint16_t svc_shdl;
    /// end handle
    uint16_t svc_ehdl;
    /// Number of discovered ASE characteristics
    uint8_t nb_ases;
    /// ASE Control Point characteristic information structures
    gaf_bap_uc_char_t ase_cp_char_info;
    /// ASE Characteristic information structures
    /// The element length is nb_ases
    gaf_bap_uc_ase_char_t *ase_char_info;
} gaf_bap_uc_ascs_info_t;

typedef struct gaf_bap_uc_ascs_cli_restore_bond_data_cmd
{
    /// Connection local index
    uint8_t conidx;
    /// Audio Stream Control Service content description
    gaf_bap_uc_ascs_info_t ascs_info;
} gaf_bap_uc_ascs_cli_restore_bond_data_cmd_t;

typedef struct gaf_bap_uc_ascs_cli_restore_bond_data_codec_cmd
{
    /// Connection local index
    uint8_t conidx;
    /// ASE local index
    uint8_t ase_lid;
    /// ASE instance index
    uint8_t ase_instance_idx;
    /// Direction (@see enum gaf_hl_direction)
    uint8_t direction;
    /// Codec ID
    gaf_codec_id_t codec_id;
    /// Controller Delay in microseconds
    uint32_t ctl_delay_us;
    /// Length of Codec Configuration value
    uint16_t cfg_len;
    /// Codec Configuration value, length is cfg_len
    uint8_t *cfg;
} gaf_bap_uc_ascs_cli_restore_bond_data_codec_cmd_t;

typedef struct gaf_bap_uc_ascs_cli_grp
{
    /// SDU interval from Master to Slave in microseconds
    /// From 0xFF (255us) to 0xFFFF (1.048575s)
    uint32_t sdu_intv_m2s_us;
    /// SDU interval from Slave to Master in microseconds
    /// From 0xFF (255us) to 0xFFFF (1.048575s)
    uint32_t sdu_intv_s2m_us;
    /// Maximum time (in milliseconds) for an SDU to be transported from master controller to slave
    /// controller. From 0x5 (5ms) to 0xFA0 (4s)
    uint16_t tlatency_m2s_ms;
    /// Maximum time (in milliseconds) for an SDU to be transported from slave controller to master
    /// controller. From 0x5 (5ms) to 0xFA0 (4s)
    uint16_t tlatency_s2m_ms;
    /// Sequential or interleaved scheduling
    /// 0: Sequential, 1: interleaved
    uint8_t packing;
    /// Unframed or framed mode
    /// 0: Unframed, 1: framed
    uint8_t framing;
    /// Worst slow clock accuracy of slaves
    uint8_t sca;
} gaf_bap_uc_ascs_cli_grp_t;

typedef struct gaf_bap_uc_ascs_cli_create_group_cmd
{
    /// Group local index
    uint8_t grp_lid;
    /// Group parameters
    gaf_bap_uc_ascs_cli_grp_t params;
} gaf_bap_uc_ascs_cli_create_group_cmd_t;

typedef struct gaf_bap_uc_ascs_cli_remove_group_cmd
{
    /// Group local index
    uint8_t grp_lid;
} gaf_bap_uc_ascs_cli_remove_group_cmd_t;

typedef struct gaf_bap_uc_ascs_cli_cmd
{
    /// Command code (@see enum gaf_bap_uc_ascs_cli_cmd_code)
    enum gaf_bap_uc_ascs_cli_cmd_code cmd_code;
    union
    {
        gaf_bap_uc_ascs_cli_restore_bond_data_cmd_t restore_bd;
        gaf_bap_uc_ascs_cli_restore_bond_data_codec_cmd_t restore_bd_codec;
        gaf_bap_uc_ascs_cli_create_group_cmd_t create_group;
        gaf_bap_uc_ascs_cli_remove_group_cmd_t remove_group;

        gaf_bap_uc_ascs_cli_disc_cmd_t disc;
        gaf_bap_uc_ascs_cli_cfg_codec_cmd_t cfg_codec;
        gaf_bap_uc_ascs_cli_cfg_qos_cmd_t cfg_qos;
        gaf_bap_uc_ascs_cli_ase_enable_cmd_t ase_enable;
        gaf_bap_uc_ascs_cli_update_meta_cmd_t update_meta;
        gaf_bap_uc_ascs_cli_ase_disable_cmd_t ase_disable;
        gaf_bap_uc_ascs_cli_ase_release_cmd_t ase_release;
        gaf_bap_uc_ascs_cli_get_quality_cmd_t get_quality;
        gaf_bap_uc_ascs_cli_get_tx_sync_cmd_t get_tx_sync;
    } p;
} gaf_bap_uc_ascs_cli_cmd_t;

typedef struct gaf_bap_uc_ascs_srv_ase_disable_cmd
{
    /// ASE local index
    uint8_t ase_lid;
} gaf_bap_uc_ascs_srv_ase_disable_cmd_t;

typedef struct gaf_bap_uc_ascs_srv_ase_release_cmd
{
    /// ASE local index
    uint8_t ase_lid;
    /// Indicate if ASE next state will be Idle or Codec Configured
    uint8_t idle;
} gaf_bap_uc_ascs_srv_ase_release_cmd_t;

typedef struct gaf_bap_uc_ascs_srv_get_quality_cmd
{
    /// ASE local index
    uint8_t ase_lid;
} gaf_bap_uc_ascs_srv_get_quality_cmd_t;

typedef struct gaf_bap_uc_ascs_srv_get_tx_sync_cmd
{
    /// ASE local index
    uint8_t ase_lid;
} gaf_bap_uc_ascs_srv_get_tx_sync_cmd_t;

typedef struct gaf_bap_uc_ascs_srv_restore_bond_data_cmd
{
    /// Connection local index
    uint8_t conidx;
    /// ASE Control Point Client configuration for Audio Stream Control Service
    uint8_t cp_cli_cfg;
    /// Client configuration bit field for instances of the ASE characteristics
    /// Each bit correspond to an instance of the ASE characteristic
    uint16_t ase_cli_cfg_bf;
} gaf_bap_uc_ascs_srv_restore_bond_data_cmd_t;

typedef struct gaf_bap_uc_ascs_srv_restore_bond_data_codec_cmd
{
    /// Connection local index
    uint8_t conidx;
    /// ASE local index
    uint8_t ase_lid;
    /// ASE instance index
    uint8_t ase_instance_idx;
    /// Direction (@see enum gaf_hl_direction)
    uint8_t direction;
    /// Codec ID
    gaf_codec_id_t codec_id;
    /// QoS Requirements
    bap_qos_req_t qos_req;
    /// Controller delay in microseconds
    uint32_t ctl_delay_us;
    /// Length of Codec Configuration structure value
    uint16_t cfg_len;
    /// Codec Configuration structure value, length is cfg_len
    uint8_t *cfg;
} gaf_bap_uc_ascs_srv_restore_bond_data_codec_cmd_t;

/// Structure for BAP_uc_ascs_srv_CONFIGURE_CODEC request message
typedef struct gaf_bap_uc_ascs_srv_cfg_codec_cmd
{
    /// Connection local index
    uint8_t conidx;
    /// ASE local index
    uint8_t ase_lid;
    /// ASE instance index
    uint8_t ase_instance_idx;
    /// Direction
    uint8_t direction;
    /// Codec ID
    gaf_codec_id_t codec_id;
    /// QoS Requirements
    bap_qos_req_t qos_req;
    /// Controller delay in microseconds
    uint32_t ctl_delay_us;
    /// Length of Codec Configuration structure value
    uint16_t cfg_len;
    /// Codec Configuration structure value, length is cfg_len
    uint8_t *cfg;
} gaf_bap_uc_ascs_srv_cfg_codec_cmd_t;

typedef struct gaf_bap_uc_ascs_srv_update_metadata_cmd
{
    /// ASE local index
    uint8_t ase_lid;
    /// Metadata structure
    gaf_bap_metadata_cfg_t metadata;
} gaf_bap_uc_ascs_srv_update_metadata_cmd_t;

typedef struct gaf_bap_uc_ascs_srv_cmd
{
    /// Command code (@see enum gaf_bap_uc_ascs_srv_cmd_code)
    enum gaf_bap_uc_ascs_srv_cmd_code cmd_code;
    union
    {
        gaf_bap_uc_ascs_srv_restore_bond_data_cmd_t restore_bd;
        gaf_bap_uc_ascs_srv_restore_bond_data_codec_cmd_t restore_bd_codec;
        gaf_bap_uc_ascs_srv_cfg_codec_cmd_t cfg_codec;
        gaf_bap_uc_ascs_srv_update_metadata_cmd_t update_meta;

        gaf_bap_uc_ascs_srv_ase_disable_cmd_t ase_disable;
        gaf_bap_uc_ascs_srv_ase_release_cmd_t ase_release;
        gaf_bap_uc_ascs_srv_get_quality_cmd_t get_quality;
        gaf_bap_uc_ascs_srv_get_tx_sync_cmd_t get_tx_sync;
    } p;
} gaf_bap_uc_ascs_srv_cmd_t;

typedef struct gaf_bap_uc_ascs_srv_cfg_codec_cfm
{
    /// Response code (@see enum bap_uc_cp_rsp_code)
    /// BAP_UC_CP_RSP_CODE_SUCCESS means accepting the request
    enum bap_uc_cp_rsp_code rsp_code;
    /// Reason (@see enum bap_uc_cp_reason)
    enum bap_uc_cp_reason reason;
    /// ASE local index
    uint8_t ase_lid;
    /// Codec ID
    gaf_codec_id_t codec_id;
    /// QoS Requirements
    bap_qos_req_t qos_req;
    /// Length of Codec Configuration value
    uint16_t cfg_len;
    /// Codec Configuration value, length is cfg_len
    /// To be casted as @see bap_lc3_cfg_t for LC3
    /// To be casted as @see bap_vendor_codec_cfg_t for Vendor Codec
    uint8_t *cfg;
} gaf_bap_uc_ascs_srv_cfg_codec_cfm_t;

typedef struct gaf_bap_uc_ascs_srv_ase_enable_cfm
{
    /// Response code (@see enum bap_uc_cp_rsp_code)
    /// BAP_UC_CP_RSP_CODE_SUCCESS means accepting the request
    enum bap_uc_cp_rsp_code rsp_code;
    /// Reason (@see enum bap_uc_cp_reason)
    enum bap_uc_cp_reason reason;
    /// Metadata structure
    gaf_bap_metadata_cfg_t metadata;
} gaf_bap_uc_ascs_srv_ase_enable_cfm_t;

typedef struct gaf_bap_uc_ascs_srv_update_meta_cfm
{
    /// Response code (@see enum bap_uc_cp_rsp_code)
    /// BAP_UC_CP_RSP_CODE_SUCCESS means accepting the request
    enum bap_uc_cp_rsp_code rsp_code;
    /// Reason (@see enum bap_uc_cp_reason)
    enum bap_uc_cp_reason reason;
    /// Metadata structure
    gaf_bap_metadata_cfg_t metadata;
} gaf_bap_uc_ascs_srv_update_meta_cfm_t;

typedef struct gaf_bap_uc_ascs_srv_ase_release_cfm
{
    /// Response code (@see enum bap_uc_cp_rsp_code)
    /// BAP_UC_CP_RSP_CODE_SUCCESS means accepting the request
    enum bap_uc_cp_rsp_code rsp_code;
    /// Reason (@see enum bap_uc_cp_reason)
    enum bap_uc_cp_reason reason;
    /// Indicate if ASE next state will be Idle or Codec Configured
    uint8_t idle;
} gaf_bap_uc_ascs_srv_ase_release_cfm_t;

typedef struct gaf_bap_uc_ascs_srv_cfm
{
    /// Confirm code (@see enum gaf_bap_uc_ascs_srv_cfm_code)
    enum gaf_bap_uc_ascs_srv_cfm_code cfm_code;
    union
    {
        gaf_bap_uc_ascs_srv_cfg_codec_cfm_t cfg_codec;
        gaf_bap_uc_ascs_srv_ase_enable_cfm_t ase_enable;
        gaf_bap_uc_ascs_srv_update_meta_cfm_t update_meta;
        gaf_bap_uc_ascs_srv_ase_release_cfm_t ase_release;
    } p;
} gaf_bap_uc_ascs_srv_cfm_t;

typedef struct gaf_bap_uc_capa_cli_discover_cmd
{
    /// Connection local index
    uint8_t conidx;
} gaf_bap_uc_capa_cli_discover_cmd_t;

typedef struct gaf_bap_uc_capa_cli_get_cmd
{
    /// Connection local index
    uint8_t conidx;
    /// Characteristic type (@enum enum gaf_bap_uc_capa_char_type)
    uint8_t char_type;
    /// PAC local index
    uint8_t pac_lid;
} gaf_bap_uc_capa_cli_get_cmd_t;

typedef struct gaf_bap_uc_capa_cli_set_location_cmd
{
    /// Connection local index
    uint8_t conidx;
    /// Direction (@see enum gaf_hl_direction)
    uint8_t direction;
    /// Location bit field
    /// See @defgroup Audio Location Definitions
    uint32_t location_bf;
} gaf_bap_uc_capa_cli_set_location_cmd_t;

typedef struct gaf_bap_uc_pac_char
{
    /// Direction (@see enum gaf_hl_direction)
    uint8_t direction;
    /// Characteristic value handle
    uint16_t val_hdl;
    /// Client Characteristic Configuration descriptor handle
    uint16_t desc_hdl;
} gaf_bap_uc_pac_char_t;

typedef struct gaf_bap_uc_pacs_info
{
    /// start handle
    uint16_t svc_shdl;
    /// end handle
    uint16_t svc_ehdl;
    /// Number of PAC Groups for Sink direction
    uint8_t nb_pacs_sink;
    /// Number of PAC Group for Source direction
    uint8_t nb_pacs_src;
    /// Optional features bit field
    uint8_t opt_feat_bf;
    /// Available Audio Contexts characteristic information structures
    gaf_bap_uc_char_t aac_char_info;
    /// Supported Audio Contexts characteristic information structures
    gaf_bap_uc_char_t sac_char_info;
    /// Sink/Soure characteristic information structures
    /// If nb_pacs is 0, then the hdl(val_hdl and desc_hdl) corresponding to the direction is 0xFFFF
    gaf_bap_uc_char_t location_char_info[GAF_HL_DIRECTION_MAX];
    /// PAC characteristic information structures
    /// The element length is (nb_pacs_sink + nb_pacs_src)
    gaf_bap_uc_pac_char_t *pac_char_info;
} gaf_bap_uc_pacs_info_t;

typedef struct gaf_bap_uc_capa_cli_restore_bond_data_cmd
{
    /// Connection local index
    uint8_t conidx;
    /// Published Audio Capabilities Service content description
    gaf_bap_uc_pacs_info_t pacs_info;
} gaf_bap_uc_capa_cli_restore_bond_data_cmd_t;

typedef struct gaf_bap_uc_capa_cli_cmd
{
    /// Command code (@see enum gaf_bap_uc_capa_cli_cmd_code)
    enum gaf_bap_uc_capa_cli_cmd_code cmd_code;
    union 
    {
        gaf_bap_uc_capa_cli_restore_bond_data_cmd_t restore_bd;
        gaf_bap_uc_capa_cli_discover_cmd_t disc;
        gaf_bap_uc_capa_cli_get_cmd_t get_value;
        gaf_bap_uc_capa_cli_set_location_cmd_t set_location;
    } p;
} gaf_bap_uc_capa_cli_cmd_t;

typedef struct gaf_bap_uc_capa_srv_restore_bond_data_cmd
{
    /// Connection local index
    uint8_t conidx;
    /// Client configuration bit field
    /// Each bit corresponds to a characteristic in the range [0, GAF_BAP_UC_CAPA_CHAR_TYPE_PAC]
    uint8_t cli_cfg_bf;
    /// Client configuration bit field for Sink/Source PAC characteristic
    /// Each bit corresponds to an instance of the Sink/Source PAC characteristic (Sink placed first)
    uint16_t pac_cli_cfg_bf;
    /// Event bit field
    /// Each bit corresponds to a characteristic in the range [0, GAF_BAP_UC_CAPA_CHAR_TYPE_PAC]
    /// If ((evt_bf & cli_cfg_bf) != 0), a notification will be sent to the Capa client(Available Audio Contexts Characteristic)
    uint8_t evt_bf;
    /// Event configuration bit field for Sink/Source PAC characteristics
    /// Each bit corresponds to an instance of the Sink/Source PAC characteristic (Sink placed first)
    /// If ((pac_evt_bf & pac_cli_cfg_bf) != 0), a notification will be sent to the Capa client(PAC Characteristic)
    uint16_t pac_evt_bf;
    /// Available Audio Contexts for Sink direction
    uint16_t context_bf_sink;
    /// Available Audio Contexts for Source direction
    uint16_t context_bf_src;
} gaf_bap_uc_capa_srv_restore_bond_data_cmd_t;

/// LC3 Capabilities structure
typedef struct gaf_bap_lc3_capa
{
    /// Supported sampling frequencies bit field
    uint16_t sampling_freq_bf;
    /// Minimum number of octets supported per codec frame
    uint16_t frame_octet_min;
    /// Maximum number of octets supported per codec frame
    uint16_t frame_octet_max;
    /// Preferred Audio Context bit field
    uint16_t context_bf;
    /// Supported frame durations bit field
    uint8_t frame_dur_bf;
    /// Channel count supported
    uint8_t chan_cnt;
    /// Maximum supported LC3 frames per SDU
    /// Shall be either equal to 0 or >= to chan_cnt
    /// Part of Codec Specific Capabilities only if different than 0
    uint8_t max_lc3_frame_sdu;
    /// Length of additional Codec Capabilities value
    uint8_t add_capa_len;
    /// Length of additional Metadata value
    uint8_t add_metadata_len;
    /// Additional Codec Capabilities value (in LTV format) followed by additional Metadata value
    /// (in LTV format)
    uint8_t *val;
} gaf_bap_lc3_capa_t;

typedef struct gaf_bap_uc_capa_srv_set_record_cmd
{
    /// PAC local index
    uint8_t pac_lid;
    /// Record identifier
    uint8_t record_id;
    /// Codec ID
    gaf_codec_id_t codec_id;
    /// Length of Codec Capabilities structure value
    uint16_t capa_len;
    /// Codec Capabilities structure value
    /// To be casted as bap_lc3_capa_t for LC3.
    /// To be casted as bap_vendor_codec_capa_t for Vendor Codec
    uint8_t *capa;
} gaf_bap_uc_capa_srv_set_record_cmd_t;

typedef struct gaf_bap_uc_capa_srv_remove_record_cmd
{
    /// Record identifier
    uint8_t record_id;
} gaf_bap_uc_capa_srv_remove_record_cmd_t;

typedef struct gaf_bap_uc_capa_srv_set_location_cmd
{
    /// Direction (@see enum gaf_hl_direction)
    uint8_t direction;
    /// Location bit field
    uint32_t location_bf;
} gaf_bap_uc_capa_srv_set_location_cmd_t;

typedef struct gaf_bap_uc_capa_srv_set_context_cmd
{
    /// Context type (@see enum gaf_bap_uc_capa_context_type)
    uint8_t context_type;
    /// Connection local index
    /// Meaningful only for Available Audio Contexts value
    /// Must be set to GAP_INVALID_CONIDX if indicated audio contexts are available for all connections
    uint8_t conidx;
    /// Context bit field value for Sink direction
    uint16_t context_bf_sink;
    /// Context bit field value for Source direction
    uint16_t context_bf_src;
} gaf_bap_uc_capa_srv_set_context_cmd_t;

typedef struct gaf_bap_uc_capa_srv_cmd
{
    /// Request code (@see enum gaf_bap_uc_capa_srv_cmd_code)
    enum gaf_bap_uc_capa_srv_cmd_code cmd_code;
    union 
    {
        gaf_bap_uc_capa_srv_restore_bond_data_cmd_t restore_bd;
        gaf_bap_uc_capa_srv_set_record_cmd_t set_record;
        gaf_bap_uc_capa_srv_remove_record_cmd_t remove_record;
        gaf_bap_uc_capa_srv_set_location_cmd_t set_location;
        gaf_bap_uc_capa_srv_set_context_cmd_t set_context;
    } p;
} gaf_bap_uc_capa_srv_cmd_t;

typedef struct gaf_bap_uc_iso_data
{
    /// ASE local index
    uint8_t ase_lid;
    /// Data
    gaf_iso_data_t p;
} gaf_bap_uc_iso_data_t;

// *******************************************************************************************
// -------------------------------------------------------------------------------------------
// ---------------------- BAP Unicast Audio API Parameter Defines END ------------------------
// -------------------------------------------------------------------------------------------
// *******************************************************************************************

// *******************************************************************************************
// ------------------------------------------------------------------------------
// ---------------------- GAF Callback Parameter Defines ------------------------
// ------------------------------------------------------------------------------
// *******************************************************************************************

typedef struct gaf_bap_bc_sink_status_ind
{
    /// Group local index
    uint8_t             grp_lid;
    /// Broadcast Sink state (@see enum gaf_bap_bc_sink_state)
    uint8_t             state;
    /// Stream position bit field in broadcast group to receive
    uint32_t            stream_pos_bf;
    /// When sync established, provides information about broadcast group else meaningless
    gaf_big_config_t    bg_cfg;
} gaf_bap_bc_sink_status_ind_t;

typedef struct gaf_bap_bc_sink_enable_req_ind
{
    /// Group local index
    uint8_t             grp_lid;
    /// Source local index
    uint8_t             src_lid;
    /// Connection Local Identifier of requester
    uint8_t             conidx;
    /// Stream position bit field indicating streams to synchronize with.
    uint32_t            stream_pos_bf;
} gaf_bap_bc_sink_enable_req_ind_t;

typedef struct gaf_bap_bc_sink_disable_req_ind
{
    /// Group local index
    uint8_t             grp_lid;
    /// Source local index
    uint8_t             src_lid;
    /// Connection Local Identifier of requester
    uint8_t             conidx;
} gaf_bap_bc_sink_disable_req_ind_t;

typedef struct gaf_bap_bc_scan_report_ind
{
    /// Device Address
    struct gap_ble_addr addr;
    /// Advertising SID
    uint8_t             adv_sid;
    /// Length of complete advertising data
    uint8_t             length;
    /// Complete advertising data containing the complete Broadcast Audio Announcement
    uint8_t             *data;
} gaf_bap_bc_scan_report_ind_t;

typedef struct gaf_bap_bc_scan_pa_established_ind
{
    /// Periodic Advertising local index
    uint8_t              pa_lid;
    /// Periodic advertising address information
    struct gap_ble_addr  adv_addr;
    /// Advertising SID
    uint8_t              adv_sid;
    /// Only valid for a Periodic Advertising Sync Transfer, else ignore
    uint16_t             serv_data;
} gaf_bap_bc_scan_pa_established_ind_t;

typedef struct gaf_bap_bc_scan_pa_terminated_ind
{
    /// Periodic Advertising local index
    uint8_t              pa_lid;
    /// Stop reason (@see enum gaf_err)
    uint16_t             reason;
} gaf_bap_bc_scan_pa_terminated_ind_t;

typedef struct gaf_bap_bc_scan_pa_report_ind
{
    /// Periodic Advertising local index
    uint8_t              pa_lid;
    /// Length of advertising data
    uint8_t              length;
    /// Complete periodic advertising report
    uint8_t              *data;
} gaf_bap_bc_scan_pa_report_ind_t;

typedef struct gaf_bap_bc_scan_big_info_report_ind
{
    /// Periodic Advertising local index
    uint8_t              pa_lid;
    /// BIG Info Report
    gaf_big_info_t       info;
} gaf_bap_bc_scan_big_info_report_ind_t;

typedef struct gaf_bap_bc_scan_group_report_ind
{
    /// Periodic Advertising local index
    uint8_t              pa_lid;
    /// Number of subgroups in the Broadcast Group
    uint8_t              nb_subgroups;
    /// Total number of streams in the Broadcast Group
    uint8_t              nb_streams;
    /// Audio output presentation delay in microseconds
    uint32_t             pres_delay_us;
} gaf_bap_bc_scan_group_report_ind_t;

typedef struct gaf_bap_bc_scan_subgroup_report_ind
{
    /// Periodic Advertising local index
    uint8_t              pa_lid;
    /// Subgroup identifier
    uint8_t              sgrp_id;
    /// Codec ID value
    /// (Octet  = 0xFE if equals to group codec, surcharged otherwise)
    gaf_codec_id_t       codec_id;
    /// Stream position index bit field indicating for which streams are part of the subgroup
    uint32_t             stream_pos_bf;
    /// Length of Codec Configuration value
    uint8_t              cfg_len;
    /// Codec configuration data in LTV format
    uint8_t              *cfg;
    /// Length of Metadata value
    uint8_t              metadata_len;
    /// Metadata value
    uint8_t              *metadata;
} gaf_bap_bc_scan_subgroup_report_ind_t;

typedef struct gaf_bap_bc_scan_stream_report_ind
{
    /// Periodic Advertising local index
    uint8_t              pa_lid;
    /// Stream position in group
    uint8_t              stream_pos;
    /// Codec configuration length
    uint8_t              cfg_len;
    /// Codec configuration data in LTV format
    uint8_t              *cfg;
} gaf_bap_bc_scan_stream_report_ind_t;

typedef struct gaf_bap_bc_scan_pa_synchronize_req_ind
{
    /// Periodic Advertising local index
    uint8_t              pa_lid;
    /// Source local index
    uint8_t              src_lid;
    /// Connection Local Identifier of requester
    uint8_t              conidx;
    /// Targeted periodic advertiser
    struct gap_ble_addr  addr;
    /// Advertising SID
    uint8_t              adv_sid;
} gaf_bap_bc_scan_pa_synchronize_req_ind_t;

typedef struct gaf_bap_bc_scan_pa_terminate_req_ind
{
    /// Periodic Advertising local index
    uint8_t              pa_lid;
    /// Source local index
    uint8_t              src_lid;
    /// Connection Local Identifier of requester
    uint8_t              conidx;
} gaf_bap_bc_scan_pa_terminate_req_ind_t;

typedef struct gaf_bap_bc_bis_tx_sync
{
    /// Group local index
    uint8_t grp_lid;
    /// Stream local index
    uint8_t stream_lid;
    /// The packet sequence number of an SDU (Range 0x0000-0xFFFF)
    uint16_t pkt_seq_nb;
    /// The time stamp, in microseconds, of the reference anchor point of a transmitted SDU (Range 0x00000000-0xFFFFFFFF)
    uint32_t time_stamp;
    /// The time offset, in microseconds, that is associated with a transmitted SDU (Range 0x000000-0xFFFFFF)
    uint32_t time_offset;
} gaf_bc_bis_tx_sync_t;

typedef struct gaf_bap_bc_src_cmd_cmp
{
    /// Command code (@see enum gaf_bap_bc_src_cmd_code)
    enum gaf_bap_bc_src_cmd_code cmd_code;
    uint16_t status;

    union {
        /// Valid only for GAF_CMD_BAP_BC_SRC_SET_SUBGROUP
        struct {
            /// Group local index
            uint8_t grp_lid;
            /// Subgroup local index
            uint8_t sgrp_lid;
        } subgroup_set;

        /// Valid only for GAF_CMD_BAP_BC_SRC_SET_STREAM
        struct {
            /// Group local index
            uint8_t grp_lid;
            /// Subgroup local index
            uint8_t sgrp_lid;
            /// Stream local index
            uint8_t stream_lid;
        } stream_set;

        /// others
        struct {
            /// Group local index
            uint8_t  grp_lid;
            /// Stream local index bit field. 
            /// Meaningful only for GAF_CMD_BAP_BC_SRC_START_STREAMING and GAF_CMD_BAP_BC_SRC_STOP_STREAMING commands
            uint32_t stream_lid_bf;
        } other;

        gaf_bc_bis_tx_sync_t get_tx_sync;
    } p;
} gaf_bap_bc_src_cmd_cmp_t;

typedef struct gaf_uc_bap_bc_send_iso_cmp
{
    /// Status
    uint16_t status;
    /// Group local index
    uint8_t grp_lid;
    /// Stream local index bit field. 
    uint32_t stream_lid_bf;
} gaf_uc_bap_bc_send_iso_cmp_t;

typedef struct gaf_bap_bc_sink_cmd_cmp
{
    /// Command code (@see enum gaf_bap_bc_sink_cmd_code)
    enum gaf_bap_bc_sink_cmd_code cmd_code;
    uint16_t status;

    union {
        struct {
            /// Group local index
            uint8_t grp_lid;
            /// Stream position. 
            /// Meaningful only for GAF_CMD_BAP_BC_SINK_START_STREAMING and GAF_CMD_BAP_BC_SINK_STOP_STREAMING commands
            uint8_t stream_pos;
        } global;
    } p;
} gaf_bap_bc_sink_cmd_cmp_t;

typedef struct gaf_bap_bc_scan_cmd_cmp
{
    /// Command code (@see enum gaf_bap_bc_scan_cmd_code)
    enum gaf_bap_bc_scan_cmd_code cmd_code;
    uint16_t status;

    union {
        struct {
            /// Periodic Advertising local index
            /// Meaningful only for GAF_CMD_BAP_BC_SCAN_PA_SYNCHRONIZE and GAF_CMD_BAP_BC_SCAN_PA_SYNCHRONIZE_CON commands
            uint8_t pa_lid;
        } global;
    } p;
} gaf_bap_bc_scan_cmd_cmp_t;

typedef struct gaf_bap_uc_capa_cli_bond_data_ind
{
    /// Connection local index
    uint8_t conidx;
    /// Published Audio Capabilities Service content description
    gaf_bap_uc_pacs_info_t pacs_info;
} gaf_bap_uc_capa_cli_bond_data_ind_t;

typedef struct gaf_bap_uc_capa_cli_record_ind
{
    /// Connection local index
    uint8_t conidx;
    /// PAC local index
    uint8_t pac_lid;
    /// Record local index
    uint8_t record_lid;
    /// Number of records
    uint8_t nb_records;
    /// Codec ID
    gaf_codec_id_t codec_id;
    /// Length of Codec Capabilities value
    uint16_t capa_len;
    /// Codec Capabilities value
    uint8_t *capa;
} gaf_bap_uc_capa_cli_record_ind_t;

typedef struct gaf_bap_uc_capa_cli_context_ind
{
    /// Connection local index
    uint8_t conidx;
    /// Context type (@see enum enum gaf_bap_uc_capa_context_type)
    uint8_t context_type;
    /// Context type bit field for Sink direction
    uint16_t context_bf_sink;
    /// Context type bit field for Source direction
    uint16_t context_bf_src;
} gaf_bap_uc_capa_cli_context_ind_t;

typedef struct gaf_bap_uc_capa_cli_location_ind
{
    /// Connection local index
    uint8_t conidx;
    /// Direction (@see enum gaf_hl_direction)
    uint8_t direction;
    /// Location bit field
    uint32_t location_bf;
} gaf_bap_uc_capa_cli_location_ind_t;

typedef struct gaf_bap_uc_capa_cli_svc_changed_ind
{
    /// Connection local index
    uint8_t conidx;
} gaf_bap_uc_capa_cli_svc_changed_ind_t;

typedef struct gaf_bap_uc_capa_srv_bond_data_ind
{
    /// Connection local index
    uint8_t conidx;
    /// Client configuration bit field
    /// Each bit corresponds to a characteristic in the range [0, GAF_BAP_UC_CAPA_CHAR_TYPE_PAC]
    uint8_t cli_cfg_bf;
    /// Client configuration bit field for Sink/Source PAC characteristic
    /// Each bit corresponds to an instance of the Sink/Source PAC characteristic (Sink placed first)
    uint16_t pac_cli_cfg_bf;
} gaf_bap_uc_capa_srv_bond_data_ind_t;

typedef struct gaf_bap_uc_capa_srv_location_ind
{
    /// Connection local index
    uint8_t conidx;
    /// Direction (@see enum gaf_hl_direction)
    uint8_t direction;
    /// Location bit field
    uint32_t location_bf;
} gaf_bap_uc_capa_srv_location_ind_t;

typedef struct gaf_bap_uc_ascs_cli_bond_data_ind
{
    /// Connection local index
    uint8_t conidx;
    /// Audio Stream Control Service content description
    gaf_bap_uc_ascs_info_t ascs_info;
} gaf_bap_uc_ascs_cli_bond_data_ind_t;

typedef struct gaf_bap_uc_ascs_cli_state_empty_ind
{
    /// Connection local index
    uint8_t conidx;
    /// ASE instance index
    uint8_t ase_instance_idx;
    /// ASE local index
    uint8_t ase_lid;
    /// ASE State
    uint8_t state;
} gaf_bap_uc_ascs_cli_state_empty_ind_t;

typedef struct gaf_bap_uc_ascs_cli_state_codec_ind
{
    /// Connection local index
    uint8_t conidx;
    /// ASE instance index
    uint8_t ase_instance_idx;
    /// ASE local index
    uint8_t ase_lid;
    /// Codec ID
    gaf_codec_id_t codec_id;
    /// QoS Requirements
    bap_qos_req_t qos_req;
    /// Length of Codec Configuration value
    uint16_t cfg_len;
    /// Codec Configuration value, length is cfg_len
    /// To be casted as @see bap_lc3_cfg_t for LC3
    /// To be casted as @see bap_vendor_codec_cfg_t for Vendor Codec
    uint8_t *cfg;
} gaf_bap_uc_ascs_cli_state_codec_ind_t;

typedef struct gaf_bap_qos_cfg
{
    /// PDU framing arrangement
    uint8_t framing;
    /// PHY
    uint8_t phy;
    /// Maximum number of retransmissions for each CIS Data PDU
    /// From 0 to 15
    uint8_t retx_nb;
    /// Maximum SDU size
    /// From 0 to 4095 bytes (0xFFF)
    uint16_t max_sdu_size;
    /// Transport latency in milliseconds
    /// From 5ms (0x5) to 4000ms (0xFA0)
    uint16_t trans_latency_ms;
    /// Presentation delay in microseconds
    uint32_t pres_delay_us;
    /// SDU interval in microseconds
    /// From 255us (0xFF) to 16777215us (0xFFFFFF)
    uint32_t sdu_intv_us;
} gaf_bap_qos_cfg_t;

typedef struct gaf_bap_uc_ascs_cli_state_qos_ind
{
    /// ASE local index
    uint8_t ase_lid;
    /// QoS Configuration
    gaf_bap_qos_cfg_t qos_cfg;
} gaf_bap_uc_ascs_cli_state_qos_ind_t;

typedef struct gaf_bap_uc_ascs_cli_state_metadata_ind
{
    /// ASE local index
    uint8_t ase_lid;
    /// State
    uint8_t state;
    /// Metadata
    gaf_bap_metadata_cfg_t metadata;
} gaf_bap_uc_ascs_cli_state_metadata_ind_t;

typedef struct gaf_bap_uc_ascs_cli_svc_changed_ind
{
    /// Connection local index
    uint8_t conidx;
} gaf_bap_uc_ascs_cli_svc_changed_ind_t;

typedef struct gaf_bap_uc_ascs_cli_error_ind
{
    /// ASE local index
    uint8_t ase_lid;
    /// Operation code
    uint8_t opcode;
    /// Response code
    uint8_t rsp_code;
    /// Reason
    uint8_t reason;
} gaf_bap_uc_ascs_cli_error_ind_t;

typedef struct gaf_bap_uc_ascs_srv_bond_data_ind
{
    /// Connection local index
    uint8_t conidx;
    /// ASE Control Point Client configuration for Audio Stream Control Service
    uint8_t cp_cli_cfg;
    /// Client configuration bit field for instances of the ASE characteristics
    /// Each bit correspond to an instance of the ASE characteristic
    uint16_t ase_cli_cfg_bf;
} gaf_bap_uc_ascs_srv_bond_data_ind_t;

typedef struct gaf_bap_uc_ascs_srv_ase_state_ind
{
    /// ASE local index
    uint8_t ase_lid;
    /// Connection local index
    uint8_t conidx;
    /// State (@see enum enum gaf_bap_uc_ase_state)
    uint8_t state;
    /// Direction (@see enum gaf_hl_direction)
    uint8_t direction;
} gaf_bap_uc_ascs_srv_ase_state_ind_t;

typedef struct gaf_bap_uc_ascs_srv_configure_codec_req_ind
{
    /// Connection local index
    uint8_t conidx;
    /// ASE instance index
    uint8_t ase_instance_idx;
    /// ASE local index
    uint8_t ase_lid;
    /// Direction (@see enum gaf_hl_direction)
    uint8_t direction;
    /// Target Latency (@see enum gaf_bap_uc_tgt_latency)
    uint8_t tgt_latency;
    /// Target PHY (@see enum gaf_bap_uc_tgt_phy)
    uint8_t tgt_phy;
    /// Codec ID
    gaf_codec_id_t codec_id;
    /// Length of Codec Configuration value
    uint16_t cfg_len;
    /// Codec Configuration value, length is cfg_len
    /// To be casted as @see bap_lc3_cfg_t for LC3
    /// To be casted as @see bap_vendor_codec_cfg_t for Vendor Codec
    uint8_t *cfg;
} gaf_bap_uc_ascs_srv_configure_codec_req_ind_t;

typedef struct gaf_bap_uc_ascs_srv_enable_req_ind
{
    /// ASE local index
    uint8_t ase_lid;
    /// Metadata structure
    gaf_bap_metadata_cfg_t metadata;
} gaf_bap_uc_ascs_srv_enable_req_ind_t;

typedef struct gaf_bap_uc_ascs_srv_update_metadata_req_ind
{
    /// ASE local index
    uint8_t ase_lid;
    /// Metadata structure
    gaf_bap_metadata_cfg_t metadata;
} gaf_bap_uc_ascs_srv_update_metadata_req_ind_t;

typedef struct gaf_bap_uc_ascs_srv_release_req_ind
{
    /// ASE local index
    uint8_t ase_lid;
} gaf_bap_uc_ascs_srv_release_req_ind_t;

typedef struct gaf_bap_uc_ascs_cis_param
{
    /// The CIG synchronization delay time in microseconds
    uint32_t cig_sync_delay;
    /// The CIS synchronization delay time in microseconds
    uint32_t cis_sync_delay;
    /// The maximum time, in microseconds, for transmission of SDUs of all CISes from master to slave
    /// (range 0x0000EA to 0x7FFFFF)
    uint32_t trans_latency_m2s;
    /// The maximum time, in microseconds, for transmission of SDUs of all CISes from slave to master
    /// (range 0x0000EA to 0x7FFFFF)
    uint32_t trans_latency_s2m;
    /// Master to slave PHY, 0x01: 1Mbps, 0x02: 2Mbps, 0x03: LE-Coded
    uint8_t  phy_m2s;
    /// Slave to master PHY, 0x01: 1Mbps, 0x02: 2Mbps, 0x03: LE-Coded
    uint8_t  phy_s2m;
    /// Maximum number of subevents in each isochronous event (Range: 0x01-0x1E)
    uint8_t  nse;
    /// The burst number for master to slave transmission (0x00: no isochronous data from the master to the slave, range 0x01-0x0F)
    uint8_t  bn_m2s;
    /// The burst number for slave to master transmission (0x00: no isochronous data from the slave to the master, range 0x01-0x0F)
    uint8_t  bn_s2m;
    /// The flush timeout, in multiples of the ISO_Interval, for each payload sent from the master to the slave (Range: 0x01-0x1F)
    uint8_t  ft_m2s;
    /// The flush timeout, in multiples of the ISO_Interval, for each payload sent from the slave to the master (Range: 0x01-0x1F)
    uint8_t  ft_s2m;
    /// ISO interval (1.25ms unit, range: 5ms to 4s)
    uint16_t iso_interval;
} gaf_bap_uc_ascs_cis_param_t;

typedef struct gaf_bap_uc_ascs_cis_state_ind
{
    /// ASE local index for Sink direction
    uint8_t ase_lid_sink;
    /// ASE local index for Source direction
    uint8_t ase_lid_src;
    /// Group local index
    uint8_t grp_lid;
    /// CIS ID
    uint8_t cis_id;
    /// CIS state (@see enum gaf_bap_cis_state)
    uint8_t cis_state;
    /// Connection handle allocated by the controller
    /// Not equal to conidx
    uint16_t conhdl;
    /// CIS param, valid if cis_state == GAF_BAP_UC_CIS_STATE_CON
    gaf_bap_uc_ascs_cis_param_t cis_param;
} gaf_bap_uc_ascs_cis_state_ind_t;

typedef struct gaf_bap_uc_capa_cli_cmd_cmp
{
    /// Command code (@see enum gaf_bap_uc_capa_cli_cmd_code)
    enum gaf_bap_uc_capa_cli_cmd_code cmd_code;
    /// Status
    uint16_t status;

    union {
        struct {
            /// Connection local index
            uint8_t conidx;
        } restore_bd;
        
        struct {
            /// Connection local index
            uint8_t conidx;
        } discovery;

        struct {
            /// Connection local index
            uint8_t conidx;
            /// Characteristic type (@enum enum gaf_bap_uc_capa_char_type)
            uint8_t char_type;
            /// PAC local index
            uint8_t pac_lid;
        } get_char;

        struct {
            /// Connection local index
            uint8_t conidx;
            /// Direction (@see enum gaf_hl_direction)
            uint8_t direction;
        } set_location;
    } p;
} gaf_bap_uc_capa_cli_cmd_cmp_t;

typedef struct gaf_bap_uc_capa_srv_cmd_cmp
{
    /// Request code (@see enum gaf_bap_uc_capa_srv_cmd_code)
    enum gaf_bap_uc_capa_srv_cmd_code cmd_code;
    /// Status
    uint16_t status;

    union {
        struct {
            /// PAC local index
            uint8_t pac_lid;
            /// Record identifier
            uint8_t record_id;
        } set_record;

        struct {
            /// Record identifier
            uint8_t record_id;
        } remove_record;

        struct {
            /// Connection local index
            uint8_t conidx;
        } restore_bd;

        struct {
            /// Connection local index
            uint8_t conidx;
            /// Context type (@see enum enum gaf_bap_capa_context_type)
            uint8_t context_type;
        } set_context;

        struct {
            /// Direction (@see enum gaf_hl_direction)
            uint8_t direction;
        } set_location;
    } p;
} gaf_bap_uc_capa_srv_cmd_cmp_t;

typedef struct gaf_bap_uc_ascs_cli_cmd_cmp
{
    /// Command code (@see enum gaf_bap_uc_ascs_cli_cmd_code)
    enum gaf_bap_uc_ascs_cli_cmd_code cmd_code;
    /// Status
    uint16_t status;

    union {
        struct {
            /// Connection local index
            uint8_t conidx;
        } restore_bd;

        struct {
            /// Connection local index
            uint8_t conidx;
        } restore_bd_codec;

        struct {
            // Group local index
            uint8_t grp_lid;
        } create_group;

        struct {
            // Group local index
            uint8_t grp_lid;
        } remove_group;

        struct {
            /// Connection local index
            uint8_t conidx;
        } discovery;

        struct {
            /// ASE local index
            uint8_t ase_lid;
        } ase_op;

        gaf_bap_uc_cis_quality_t quality;
        gaf_bap_uc_cis_tx_sync_t tx_sync;
    } p;
} gaf_bap_uc_ascs_cli_cmd_cmp_t;

typedef struct gaf_bap_uc_ascs_srv_cmd_cmp
{
    /// Command code (@see enum gaf_bap_uc_ascs_srv_cmd_code)
    enum gaf_bap_uc_ascs_srv_cmd_code cmd_code;
    /// Status
    uint16_t status;

    union {
        struct {
            /// Connection local index
            uint8_t conidx;
        } restore_bd;

        struct {
            /// Connection local index
            uint8_t conidx;
            /// ASE local index
            uint8_t ase_lid;
        } restore_bd_codec;

        struct {
            /// Connection local index
            uint8_t conidx;
            /// ASE local index
            uint8_t ase_lid;
        } cfg_codec;

        struct {
            /// ASE local index
            uint8_t ase_lid;
        } update_meta;

        struct {
            /// ASE local index
            uint8_t ase_lid;
        } ase_disable;

        struct {
            /// ASE local index
            uint8_t ase_lid;
        } ase_release;

        gaf_bap_uc_cis_quality_t quality;
        gaf_bap_uc_cis_tx_sync_t tx_sync;
    } p;
} gaf_bap_uc_ascs_srv_cmd_cmp_t;

typedef struct gaf_uc_bap_ascs_send_iso_cmp
{
    /// Status
    uint16_t status;
    /// ASE local index
    uint8_t ase_lid;
} gaf_bap_uc_ascs_send_iso_cmp_t;

typedef union gaf_bap_callback_param
{
    /// Param of event GAF_EVT_BAP_CONFIGURED
    uint16_t bap_cfg_status;

    /// Param of event GAF_BAP_BC_SINK_STATUS
    gaf_bap_bc_sink_status_ind_t bc_sink_status;
    /// Param of event GAF_BAP_BC_SINK_ISO_DATA
    gaf_bap_bc_iso_data_t bc_sink_iso_data;

    /// Param of event GAF_BAP_BC_SINK_ENABLE_REQ
    gaf_bap_bc_sink_enable_req_ind_t bc_sink_enable_req;
    /// Param of event GAF_BAP_BC_SINK_DISABLE_REQ
    gaf_bap_bc_sink_disable_req_ind_t bc_sink_disable_req;

    /// Param of event GAF_BAP_BC_SCAN_REPORT
    gaf_bap_bc_scan_report_ind_t bc_scan_report;
    /// Param of event GAF_BAP_BC_SCAN_PA_ESTABLISHED
    gaf_bap_bc_scan_pa_established_ind_t bc_scan_pa_established;
    /// Param of event GAF_BAP_BC_SCAN_PA_TERMINATED
    gaf_bap_bc_scan_pa_terminated_ind_t bc_scan_pa_terminated;
    /// Param of event GAF_BAP_BC_SCAN_PA_REPORT
    gaf_bap_bc_scan_pa_report_ind_t bc_scan_pa_report;
    /// Param of event GAF_BAP_BC_SCAN_BIG_INFO_REPORT
    gaf_bap_bc_scan_big_info_report_ind_t bc_scan_big_info_report;
    /// Param of event GAF_BAP_BC_SCAN_GROUP_REPORT
    gaf_bap_bc_scan_group_report_ind_t bc_scan_group_report;
    /// Param of event GAF_BAP_BC_SCAN_SUBGROUP_REPORT
    gaf_bap_bc_scan_subgroup_report_ind_t bc_scan_subgroup_report;
    /// Param of event GAF_BAP_BC_SCAN_STREAM_REPORT
    gaf_bap_bc_scan_stream_report_ind_t bc_scan_stream_report;
    
    /// Param of event GAF_BAP_BC_SCAN_PA_SYNCHRONIZE_REQ
    gaf_bap_bc_scan_pa_synchronize_req_ind_t bc_scan_pa_synchronize_req;
    /// Param of event GAF_BAP_BC_SCAN_PA_TERMINATE_REQ
    gaf_bap_bc_scan_pa_terminate_req_ind_t bc_scan_pa_terminate_req;

    /// Param of event GAF_BAP_BC_SRC_CMD_CMP
    gaf_bap_bc_src_cmd_cmp_t bc_src_cmd_cmp;
    /// Param of event GAF_BAP_BC_SINK_CMD_CMP
    gaf_bap_bc_sink_cmd_cmp_t bc_sink_cmd_cmp;
    /// Param of event GAF_BAP_BC_SCAN_CMD_CMP
    gaf_bap_bc_scan_cmd_cmp_t bc_scan_cmd_cmp;
    /// Param of event GAF_EVT_BAP_BC_SRC_SEND_ISO_CMP
    gaf_uc_bap_bc_send_iso_cmp_t bc_src_send_iso_cmp;
    
    /// Param of event GAF_BAP_UC_CAPA_CLI_BOND_DATA
    gaf_bap_uc_capa_cli_bond_data_ind_t capa_cli_bond_data;
    /// Param of event GAF_BAP_UC_CAPA_CLI_RECORD
    gaf_bap_uc_capa_cli_record_ind_t capa_cli_record;
    /// Param of event GAF_BAP_UC_CAPA_CLI_CONTEXT
    gaf_bap_uc_capa_cli_context_ind_t capa_cli_context;
    /// Param of event GAF_BAP_UC_CAPA_CLI_LOCATION
    gaf_bap_uc_capa_cli_location_ind_t capa_cli_location;
    /// Param of event GAF_BAP_UC_CAPA_CLI_SVC_CHANGED
    gaf_bap_uc_capa_cli_svc_changed_ind_t capa_cli_svc_changed;
    
    /// Param of event GAF_BAP_UC_CAPA_SRV_BOND_DATA
    gaf_bap_uc_capa_srv_bond_data_ind_t capa_srv_bond_data;
    /// Param of event GAF_BAP_UC_CAPA_SRV_LOCATION
    gaf_bap_uc_capa_srv_location_ind_t capa_srv_location;

    /// Param of event GAF_BAP_UC_ASCS_CLI_BOND_DATA
    gaf_bap_uc_ascs_cli_bond_data_ind_t uc_ascs_cli_bond_data;
    /// Param of event GAF_BAP_UC_ASCS_CLI_CIS_STATE
    gaf_bap_uc_ascs_cis_state_ind_t uc_ascs_cli_cis_state;
    /// Param of event GAF_BAP_UC_ASCS_CLI_STATE_EMPTY
    gaf_bap_uc_ascs_cli_state_empty_ind_t uc_ascs_cli_state_empty;
    /// Param of event GAF_BAP_UC_ASCS_CLI_STATE_CODEC
    gaf_bap_uc_ascs_cli_state_codec_ind_t uc_ascs_cli_state_codec;
    /// Param of event GAF_BAP_UC_ASCS_CLI_STATE_QOS
    gaf_bap_uc_ascs_cli_state_qos_ind_t uc_ascs_cli_state_qos;
    /// Param of event GAF_BAP_UC_ASCS_CLI_STATE_METADATA
    gaf_bap_uc_ascs_cli_state_metadata_ind_t uc_ascs_cli_state_metadata;
    /// Param of event GAF_BAP_UC_ASCS_CLI_ERROR
    gaf_bap_uc_ascs_cli_error_ind_t uc_ascs_cli_error;
    /// Param of event GAF_BAP_UC_ASCS_CLI_SVC_CHANGED
    gaf_bap_uc_ascs_cli_svc_changed_ind_t uc_ascs_cli_svc_changed;
    /// Param of event GAF_BAP_UC_ASCS_CLI_ISO_DATA
    gaf_bap_uc_iso_data_t uc_ascs_cli_iso_data;
    /// Param of event GAF_EVT_BAP_UC_ASCS_CLI_SEND_ISO_CMP
    gaf_bap_uc_ascs_send_iso_cmp_t uc_ascs_cli_send_iso_cmp;

    /// Param of event GAF_BAP_UC_ASCS_SRV_BOND_DATA
    gaf_bap_uc_ascs_srv_bond_data_ind_t uc_ascs_srv_bond_data;
    /// Param of event GAF_BAP_UC_ASCS_SRV_ASE_STATE
    gaf_bap_uc_ascs_srv_ase_state_ind_t uc_ascs_srv_ase_state;
    /// Param of event GAF_BAP_UC_ASCS_SRV_CIS_STATE
    gaf_bap_uc_ascs_cis_state_ind_t uc_ascs_srv_cis_state;
    /// Param of event GAF_BAP_UC_ASCS_SRV_ISO_DATA
    gaf_bap_uc_iso_data_t uc_ascs_srv_iso_data;
    /// Param of event GAF_EVT_BAP_UC_ASCS_SRV_SEND_ISO_CMP
    gaf_bap_uc_ascs_send_iso_cmp_t uc_ascs_srv_send_iso_cmp;

    /// Param of event GAF_BAP_UC_ASCS_SRV_CFG_CODEC_REQ
    gaf_bap_uc_ascs_srv_configure_codec_req_ind_t uc_ascs_srv_cfg_codec_req;
    /// Param of event GAF_BAP_UC_ASCS_SRV_ASE_ENABLE_REQ
    gaf_bap_uc_ascs_srv_enable_req_ind_t uc_ascs_srv_aes_enable_req;
    /// Param of event GAF_BAP_UC_ASCS_SRV_UPDATE_METADATA_REQ
    gaf_bap_uc_ascs_srv_update_metadata_req_ind_t uc_ascs_srv_update_metadata_req;
    /// Param of event GAF_BAP_UC_ASCS_SRV_ASE_RELEASE_REQ
    gaf_bap_uc_ascs_srv_release_req_ind_t uc_ascs_srv_ase_release_req;

    /// Param of event GAF_BAP_UC_CAPA_CLI_CMD_CMP
    gaf_bap_uc_capa_cli_cmd_cmp_t capa_cli_cmd_cmp;
    /// Param of event GAF_BAP_UC_CAPA_SRV_cmd_CMP
    gaf_bap_uc_capa_srv_cmd_cmp_t capa_srv_cmd_cmp;
    /// Param of event GAF_BAP_UC_ASCS_CLI_CMD_CMP
    gaf_bap_uc_ascs_cli_cmd_cmp_t uc_ascs_cli_cmd_cmp;
    /// Param of event GAF_BAP_UC_ASCS_SRV_CMD_CMP
    gaf_bap_uc_ascs_srv_cmd_cmp_t uc_ascs_srv_cmd_cmp;
} gaf_bap_callback_param_t;

typedef union _gaf_callback_param {
    uint16_t status;

    /// Param of BAP profile
    gaf_bap_callback_param_t bap;

    struct {
        uint16_t msg_type;
        uint16_t cmd_code;
        uint16_t status;
    } unhandled_msg;
} gaf_callback_param_t;

typedef void (*gaf_callback_t)(uint8_t event, gaf_callback_param_t *param);

// *******************************************************************************************
// ------------------------------------------------------------------------------
// -------------------- GAF Callback Parameter Defines END ----------------------
// ------------------------------------------------------------------------------
// *******************************************************************************************

/*
 * @fn          gaf_cfg
 *
 * @brief       Used to initialize the configuration of the GAF layer
 *
 * @param[in]   cb : Application layer callback function, defined in application layer.
 * @param[in]   param : Configuration of the GAF layer
 * 
 * @return      @ref ble_api_err_t
 */
void gaf_cfg(gaf_callback_t cb, gaf_configure_param_t *param);

/*
 * @fn          gaf_bap_uc_capa_cli_cmd
 *
 * @brief       Used to discover the PACS(Published Audio Capabilities Service) and control the Location of the UC server
 *
 * @param[in]   p_cmd : param of command
 * 
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gaf_bap_uc_capa_cli_cmd(gaf_bap_uc_capa_cli_cmd_t const* p_cmd);

/*
 * @fn          gaf_bap_uc_capa_srv_cmd
 *
 * @brief       Used to control local PACS(Published Audio Capabilities Service)
 *
 * @param[in]   p_cmd : param of request
 * 
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gaf_bap_uc_capa_srv_cmd(gaf_bap_uc_capa_srv_cmd_t const* p_cmd);


/*
 * @fn          gaf_bap_uc_ascs_cli_cmd
 *
 * @brief       Used to execute requests in the enumeration table(@see enum gaf_bap_uc_ascs_cli_cmd_code)
 *
 * @param[in]   p_cmd : param of request
 * 
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gaf_bap_uc_ascs_cli_cmd(gaf_bap_uc_ascs_cli_cmd_t const* p_cmd);

/*
 * @fn          gaf_bap_uc_ascs_srv_cmd
 *
 * @brief       Used to execute requests in the enumeration table(@see enum gaf_bap_uc_ascs_srv_cmd_code)
 *
 * @param[in]   p_cmd : param of request
 * 
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gaf_bap_uc_ascs_srv_cmd(gaf_bap_uc_ascs_srv_cmd_t const* p_cmd);

/*
 * @fn          gaf_bap_uc_ascs_srv_cfm
 *
 * @brief       Used to confirm the operation of UC client on local ASEs
 *
 * @param[in]   p_cmd : param of confirm
 * 
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gaf_bap_uc_ascs_srv_cfm(gaf_bap_uc_ascs_srv_cfm_t const* p_cfm);

void gaf_bap_uc_srv_send_iso_data(gaf_bap_uc_iso_data_t const* p_cmd);
void gaf_bap_uc_cli_send_iso_data(gaf_bap_uc_iso_data_t const* p_cmd);

/*
 * @fn          gaf_bap_bc_src_cmd
 *
 * @brief       Used to execute requests in the enumeration table(@see enum gaf_bap_bc_src_cmd_code)
 *
 * @param[in]   p_cmd : param of command
 * 
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gaf_bap_bc_src_cmd(gaf_bap_bc_src_cmd_t const *p_cmd);
/*
 * @fn          gaf_bap_bc_sink_cmd
 *
 * @brief       Used to execute requests in the enumeration table(@see enum gaf_bap_bc_sink_cmd_code)
 *
 * @param[in]   p_cmd : param of command
 * 
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gaf_bap_bc_sink_cmd(gaf_bap_bc_sink_cmd_t const *p_cmd);
/*
 * @fn          gaf_bap_bc_sink_cfm
 *
 * @brief       Used to execute requests in the enumeration table(@see enum gaf_bap_bc_sink_cfm_code)
 *
 * @param[in]   p_cmd : param of command
 * 
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gaf_bap_bc_sink_cfm(gaf_bap_bc_sink_cfm_t const *p_cfm);
/*
 * @fn          gaf_bap_bc_scan_cmd
 *
 * @brief       Used to execute requests in the enumeration table(@see enum gaf_bap_bc_scan_cmd_code)
 *
 * @param[in]   p_cmd : param of command
 * 
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gaf_bap_bc_scan_cmd(gaf_bap_bc_scan_cmd_t const *p_cmd);
/*
 * @fn          gaf_bap_bc_scan_cfm
 *
 * @brief       Used to execute requests in the enumeration table(@see enum gaf_bap_bc_scan_cfm_code)
 *
 * @param[in]   p_cmd : param of confirm
 * 
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gaf_bap_bc_scan_cfm(gaf_bap_bc_scan_cfm_t const *p_cfm);

void gaf_bap_bc_src_send_data(gaf_bap_bc_iso_data_t const *p_cmd);

#endif  // _GAF_API_H

