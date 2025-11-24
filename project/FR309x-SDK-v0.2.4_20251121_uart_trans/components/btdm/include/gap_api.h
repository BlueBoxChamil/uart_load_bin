/*
 ****************************************************************************************
 *
 * @file       gap_api.h
 * @version    v1.0
 * @brief      GAP API exposed to app layer
 * @attention
 *  Copyright (C) 2022 Freqchip.   \n
 *  All rights reserved.           \n
 * 
 * 
 ****************************************************************************************
 */

#ifndef _GAP_API_H
#define _GAP_API_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include <stdint.h>

#include "gap_defines.h"
#include "ble_api_error.h"

/* Exported types ------------------------------------------------------------*/
enum gap_event_type {
    GAP_EVT_ADV_SET_PARAM,          //!< Set advertising parameter ended
    GAP_EVT_ADV_SET_ADV_DATA,       //!< Set advertising data ended
    GAP_EVT_ADV_SET_SCAN_RSP,       //!< Set advertising scan response data ended
    GAP_EVT_ADV_SET_PER_DATA,       //!< Set periodic advertising data ended
    GAP_EVT_ADV_START,
    GAP_EVT_ADV_END,                //!< Advertising ended
    GAP_EVT_SCAN_START,
    GAP_EVT_SCAN_END,               //!< Scanning ended
    GAP_EVT_ADV_REPORT,             //!< Scan result report
    GAP_EVT_CONN_START,
    GAP_EVT_CONN_END,               //!< Connecion action is ended
    GAP_EVT_PER_SYNC_START,         //!< Periodic synchronization started
    GAP_EVT_PER_SYNC_END,           //!< Periodic synchronization ended
    GAP_EVT_PER_SYNC_ESTABLISHED,   //!< Periodic synchronization established
    GAP_EVT_PER_SYNC_CTRL_CMP,      //!< Control periodic synchronization completed
    GAP_EVT_PER_ADV_SYNC_TRANS_CMP, //!< Transfer periodic advertising sync information to peer device completed

    GAP_EVT_NAME_REQ,               //!< Name request from peer device through GAP, need to call gap_name_req_rsp()
    GAP_EVT_SLAVE_PREF_PARAM,       //!< Slave prefer parameter from peer device through GAP, need to call gap_slave_pref_param_req_rsp()
    GAP_EVT_APPEARANCE_REQ,         //!< Appearance request from peer device through GAP, need to call gap_appearance_req_rsp()

    GAP_EVT_SLAVE_CONNECT,          //!< Connected as slave role
    GAP_EVT_MASTER_CONNECT,         //!< Connected as master role
    GAP_EVT_SLAVE_RAW_CONNECT,      //!< Connected as slave role, early than adv end and slave connect
    GAP_EVT_MASTER_RAW_CONNECT,     //!< Connected as master role, early than adv end and master connect
    GAP_EVT_DISCONNECT,             //!< Connection is removed

    GAP_EVT_LINK_PARAM_REQ,         //!< Parameter update request, need to call gap_param_update_rsp()
    GAP_EVT_LINK_PARAM_REJECT,      //!< Parameter update rejected
    GAP_EVT_LINK_PARAM_UPDATE,      //!< Parameter update successful

    GAP_EVT_SMP_TK_REQ,             //!< Reqeust Temporary Key in pairing procedure, need to call gap_security_tk_rsp()
    GAP_EVT_SMP_NC_REQ,             //!< Request confirmation of the value of Numbric Comparision, need to call gap_security_nc_rsp()
    GAP_EVT_SMP_OOB_EXC_REQ,        //!< Reqeust parameters required for OOB pairing in pairing procedure, need to call gap_security_oob_exchange()
    GAP_EVT_SMP_OOB_DATA_GEN_RESULT,//!< Result of gap_security_generate_oob_data() 
    GAP_EVT_SMP_BOND_SUCCESS,       //!< Link bond is successful
    GAP_EVT_SMP_BOND_FAILED,        //!< Link bond is failed
    GAP_EVT_SMP_ENCRYPT_SUCCESS,    //!< Link encrypt successful
    GAP_EVT_SMP_ENCRYPT_FAILED,     //!< Link encrypt failed
    GAP_EVT_SMP_SEC_REQ,            //!< Recvive security request from slave
    GAP_EVT_SMP_PAIR_REQ,           //!< Recvive pair request from master, need to call gap_pairing_rsp()
    GAP_EVT_SMP_ENCRYPT_REQ,        //!< Recvive encrypt request from master, need to call gap_encrypt_rsp()   

    GATT_EVT_MTU,                   //!< Mtu updated

    GAP_EVT_PHY_REJECT,             //!< PHY update rejected
    GAP_EVT_PHY_IND,                //!< PHY update indication or read phy success
    GAP_EVT_LL_PKT_SIZE_IND,        //!< Packet size of link layer changed event
    GAP_EVT_LINK_FEATURE,           //!< Got peer device supported features
    GAP_EVT_LINK_RSSI,              //!< Got RSSI value of link peer device
    GAP_EVT_LINK_VER,               //!< Got version value of link peer device

    GAP_EVT_ADD_DEVICE_TO_WL,       //!< Result of add device to white list
    GAP_EVT_RMV_DEVICE_FROM_WL,     //!< Result pf remove device from white list
    GAP_EVT_ADD_DEVICE_TO_RAL,      //!< Result of add device to resolve list

    GATT_EVT_PROFILE_ADDED,         //!< GATT profile have been added

    GAP_EVT_CH_MAP_IND = 50,        //!< Channel map updated
    GAP_EVT_LINK_ACTIVE_INFO,       //!< Current connection info
    GAP_EVT_SCAN_FILTER_ADD_CMP,    //!< The filtering conditions for scanning have been added
    GAP_EVT_SCAN_FILTER_REMOVE_CMP, //!< The filtering conditions for scanning have been removed
};

enum gap_adv_state {
    GAP_ADV_STATE_NONE,               //!< Advertising not created.
    GAP_ADV_STATE_IDLE,               //!< Successfully setting advertising parameters or stopping advertising will be in this state.
    GAP_ADV_STATE_CREATING,           //!< After calling gap_adv_set_param() and before reporting the GAP_EVT_ADV_SET_PARAM event, will be in this state.
    GAP_ADV_STATE_STARTING,           //!< After calling gap_adv_start() and before reporting the GAP_EVT_ADV_START event, will be in this state.
    GAP_ADV_STATE_WORKING,            //!< After reporting the GAP_EVT_ADV_START event and status is 0x00, will be in this state.
    GAP_ADV_STATE_STOPPING,           //!< After calling gap_adv_stop() and before reporting the GAP_EVT_ADV_END event, will be in this state.
    GAP_ADV_STATE_DELETING,           //!< After recalling gap_adv_set_param() and before reporting the GAP_EVT_ADV_SET_PARAM event, will be in this state.
};

enum gap_own_addr_type {
    GAP_ADDR_TYPE_STATIC,                //!< Public or Private Static Address according to device address configuration
    GAP_ADDR_TYPE_GEN_RSLV,              //!< Generated resolvable private random address
    GAP_ADDR_TYPE_GEN_NON_RSLV,          //!< Generated non-resolvable private random address
};

enum gap_adv_mode {
    GAP_ADV_MODE_UNDIRECT = 0x01,                       //!< Scannable and connectable
    GAP_ADV_MODE_DIRECT = 0x02,                         //!< Connectable, gap_adv_disc_mode should be GAP_ADV_DISC_MODE_NON_DISC, cannot set advertising data
    GAP_ADV_MODE_NON_CONN_NON_SCAN = 0x03,              //!< Non connectable non scannable
    GAP_ADV_MODE_NON_CONN_SCAN = 0x04,                  //!< Non connectable scannable
    GAP_ADV_MODE_HDC_DIRECT = 0x05,                     //!< Connectable, gap_adv_disc_mode should be GAP_ADV_DISC_MODE_NON_DISC, cannot set advertising data
    GAP_ADV_MODE_BEACON = 0x06,                         //!< Non connectable non scannable

    GAP_ADV_MODE_EXTEND_CONN_UNDIRECT = 0x11,           //!< Connectable
    GAP_ADV_MODE_EXTEND_CONN_DIRECT = 0x12,             //!< Connectable
    GAP_ADV_MODE_EXTEND_NON_CONN_SCAN = 0x13,           //!< Non connectable scannable
    GAP_ADV_MODE_EXTEND_NON_CONN_SCAN_DIRECT = 0x14,    //!< Non connectable scannable
    GAP_ADV_MODE_EXTEND_NON_CONN_NON_SCAN = 0x15,       //!< Non connectable non scannable
    GAP_ADV_MODE_EXTEND_NON_CONN_NON_SCAN_DIRECT = 0x16,//!< Non connectable non scannable

    GAP_ADV_MODE_PER_ADV_UNDIRECT = 0x21,
    GAP_ADV_MODE_PER_ADV_DIRECT = 0x22,
};

enum gap_adv_chan {
    GAP_ADV_CHAN_37 = 0x01,
    GAP_ADV_CHAN_38 = 0x02,
    GAP_ADV_CHAN_39 = 0x04,
    GAP_ADV_CHAN_ALL = 0x07,
};

enum gap_adv_filter {
    GAP_ADV_FILTER_SCAN_ANY_CON_ANY = 0x00,     //!< The Link Layer shall process scan and connection requests from all devices.
    
    GAP_ADV_FILTER_SCAN_WLST_CON_ANY = 0x01,    //!< The Link Layer shall process connection requests from all devices,
                                                //!< and shall only process scan requests from devices that are in the White List.

    GAP_ADV_FILTER_SCAN_ANY_CON_WLST = 0x02,    //!< The Link Layer shall process scan requests from all devices,
                                                //!< and shall only process connection requests from devices that are in the White List.

    GAP_ADV_FILTER_SCAN_WLST_CON_WLST = 0x03,   //!< The Link Layer shall process scan and connection requests only from 
                                                //!< devices in the White List
};

enum gap_adv_disc_mode {
    GAP_ADV_DISC_MODE_NON_DISC = 0,     //!< Non-Discoverable mode
                                        //!< This mode of package will be filtered out in the host layer.

    GAP_ADV_DISC_MODE_GEN_DISC = 1,     //!< General Discoverable mopde
                                        //!< Except for scan mode GAP_SCAN_TYPE_GEN_DISC, 
                                        //!< other scan modes will filter out this mode of package at the host layer

    GAP_ADV_DISC_MODE_LIM_DISC = 2,     //!< Limited Discoverable mopde
                                        //!< This mode of package will automatically stop within a certain period of time
                                        //!< Can be received by devices using GAP_SCAN_TYPE_GEN_DISC and GAP_SCAN_TYPE_LIM_DISC scanning modes
    GAP_ADV_DISC_MODE_BEACON   = 3,
};

enum gap_scan_type {
    GAP_SCAN_TYPE_GEN_DISC = 0,        //!< General discovery, active scanning
    GAP_SCAN_TYPE_LIM_DISC = 1,        //!< Limited discovery, active scanning
    GAP_SCAN_TYPE_OBSERVER = 2,        //!< Observer, passive scanning
    GAP_SCAN_TYPE_SEL_OBSERVER = 3,    //!< Selective observer, passive scanning, only receive advertising data from whitelist
    GAP_SCAN_TYPE_CONN_DISC = 4,       //!< Connectable discovery
    GAP_SCAN_TYPE_SEL_CONN_DISC = 5,   //!< Selective connectable discovery
};

enum gap_per_sync_type {               
    GAP_PER_SYNC_TYPE_GEN,             //!< General SYNC. Use advertiser information provided
    GAP_PER_SYNC_TYPE_SEL,             //!< Selective SYnc. Use periodic advertiser list for synchronization
    GAP_PER_SYNC_TYPE_PAST,            //!< Use Periodic advertising sync transfer information send through connection for synchronization
};

enum smp_tk_type {
    GAP_TK_TYPE_OOB,
    GAP_TK_TYPE_DISPLAY,    //!< Display TK(pin code)
    GAP_TK_TYPE_ENTRY,      //!< Input TK(pin code)
};

enum gap_sec_mode {
    ENABLE_AUTO_RSP,        //!< Automatically reply to pairing or encryption requests

    DISABLE_AUTO_RSP,       //!< Pairing or encryption requests will be reported to the GAP callback function
                            //!< The user will decide whether to reply or not
                            //!< Callback event is either GAP_EVT_SMP_PAIR_REQ or GAP_EVT_SMP_ENCRYPT_REQ
};

enum local_device_con_role {
    LOCAL_ROLE_MASTER = 0,
    LOCAL_ROLE_SLAVE,
    LOCAL_ROLE_NONE,
};

/**@brief GAP advertising data flag field select.
 * Decide whether the flag field of advertising data is automatically generated by the host 
 * according to advertising parameters or set by the upper layer
 */
enum adv_type_flag_src_t {
    ADV_TYPE_FLAG_SRC_HOST,     //!< Host automatically set adv type flag in adv data
    ADV_TYPE_FLAG_SRC_USER,     //!< User set adv type flag in adv data
};

/**@brief GAP scan accept select.
 * Decide whether the host will report the Non-Discoverable-mode advertising during scanning
 */
enum scan_accept_t {
    SCAN_NON_DISC_MODE_NOT_ACCEPT,  //!< Non-Discoverable mode Advertising will not report to the upper layer
    SCAN_NON_DISC_MODE_ACCEPT,      //!< Non-Discoverable mode Advertising will report to the upper layer
};

enum gap_per_adv_sync_trans_type {
    GAP_PER_ADV_SYNC_TRANS_TYPE_ADV,    //!< Periodic advertising is local
    GAP_PER_ADV_SYNC_TRANS_TYPE_SYNC,   //!< Periodic advertising is from another device
};

enum gap_scan_filter_bf {
    GAP_SCAN_FILTER_BF_BY_ADV_DATA_TOTAL_BIT    = 0x01,
    GAP_SCAN_FILTER_BF_BY_ADV_DATA_KEYWORD_BIT  = 0x02,
    GAP_SCAN_FILTER_BF_BY_ADDR_BIT              = 0x04,
    GAP_SCAN_FILTER_BF_BY_EVT_TYPE_BIT          = 0x08,
    GAP_SCAN_FILTER_BF_BY_RSSI_BIT              = 0x10,
};

/**@brief GAP Security Manage Protocol key Distribution Flags select.
 * Decide whether to distribute the following keys when pairing
 */
enum gap_sec_key_distribution_sel {
    GAP_SEC_KEY_DIST_IRK                        = 0x02, //!< IRK (ID key)in distribution
    GAP_SEC_KEY_DIST_CSRK                       = 0x04, //!< CSRK(Signature key) in distribution
};

/** @defgroup GAP_ADVTYPE_DEFINES GAP Advertisement Data Types
 * These are the data type identifiers for the data tokens in the advertisement data field.
 * @{
 */
#define GAP_ADTVYPE_FLAGS                       0x01 //!< Discovery Mode: @ref GAP_ADTYPE_FLAGS_MODES   
#define GAP_ADVTYPE_16BIT_MORE                  0x02 //!< Service: More 16-bit UUIDs available
#define GAP_ADVTYPE_16BIT_COMPLETE              0x03 //!< Service: Complete list of 16-bit UUIDs
#define GAP_ADVTYPE_32BIT_MORE                  0x04 //!< Service: More 32-bit UUIDs available
#define GAP_ADVTYPE_32BIT_COMPLETE              0x05 //!< Service: Complete list of 32-bit UUIDs
#define GAP_ADVTYPE_128BIT_MORE                 0x06 //!< Service: More 128-bit UUIDs available
#define GAP_ADVTYPE_128BIT_COMPLETE             0x07 //!< Service: Complete list of 128-bit UUIDs
#define GAP_ADVTYPE_LOCAL_NAME_SHORT            0x08 //!< Shortened local name
#define GAP_ADVTYPE_LOCAL_NAME_COMPLETE         0x09 //!< Complete local name
#define GAP_ADVTYPE_POWER_LEVEL                 0x0A //!< TX Power Level: 0xXX: -127 to +127 dBm
#define GAP_ADVTYPE_OOB_CLASS_OF_DEVICE         0x0D //!< Simple Pairing OOB Tag: Class of device (3 octets)
#define GAP_ADVTYPE_OOB_SIMPLE_PAIRING_HASHC    0x0E //!< Simple Pairing OOB Tag: Simple Pairing Hash C (16 octets)
#define GAP_ADVTYPE_OOB_SIMPLE_PAIRING_RANDR    0x0F //!< Simple Pairing OOB Tag: Simple Pairing Randomizer R (16 octets)
#define GAP_ADVTYPE_SM_TK                       0x10 //!< Security Manager TK Value
#define GAP_ADVTYPE_SM_OOB_FLAG                 0x11 //!< Secutiry Manager OOB Flags
#define GAP_ADVTYPE_SLAVE_CONN_INTERVAL_RANGE   0x12 //!< Min and Max values of the connection interval (2 octets Min, 2 octets Max) (0xFFFF indicates no conn interval min or max)
#define GAP_ADVTYPE_SIGNED_DATA                 0x13 //!< Signed Data field
#define GAP_ADVTYPE_SERVICES_LIST_16BIT         0x14 //!< Service Solicitation: list of 16-bit Service UUIDs
#define GAP_ADVTYPE_SERVICES_LIST_128BIT        0x15 //!< Service Solicitation: list of 128-bit Service UUIDs
#define GAP_ADVTYPE_SERVICE_DATA                0x16 //!< Service Data - 16-bit UUID
#define GAP_ADVTYPE_PUBLIC_TARGET_ADDR          0x17 //!< Public Target Address
#define GAP_ADVTYPE_RANDOM_TARGET_ADDR          0x18 //!< Random Target Address
#define GAP_ADVTYPE_APPEARANCE                  0x19 //!< Appearance
#define GAP_ADVTYPE_ADV_INTERVAL                0x1A //!< Advertising Interval
#define GAP_ADVTYPE_LE_BD_ADDR                  0x1B //!< LE Bluetooth Device Address
#define GAP_ADVTYPE_LE_ROLE                     0x1C //!< LE Role
#define GAP_ADVTYPE_SIMPLE_PAIRING_HASHC_256    0x1D //!< Simple Pairing Hash C-256
#define GAP_ADVTYPE_SIMPLE_PAIRING_RANDR_256    0x1E //!< Simple Pairing Randomizer R-256
#define GAP_ADVTYPE_SERVICE_DATA_32BIT          0x20 //!< Service Data - 32-bit UUID
#define GAP_ADVTYPE_SERVICE_DATA_128BIT         0x21 //!< Service Data - 128-bit UUID
#define GAP_ADVTYPE_3D_INFO_DATA                0x3D //!< 3D Information Data
#define GAP_ADVTYPE_MANUFACTURER_SPECIFIC       0xFF //!< Manufacturer Specific Data: first 2 octets contain the Company Identifier Code followed by the additional manufacturer specific data
/** @} End GAP_ADVTYPE_DEFINES */

/** @defgroup GAP_SCAN_EVT_TYPE_DEFINES GAP Event Types
 * @{
 */
#define GAP_SCAN_EVT_CONN_UNDIR        0x00  //!< Undirected advertising
#define GAP_SCAN_EVT_CONN_DIR          0x01  //!< Directed advertising
#define GAP_SCAN_EVT_NONCONN_UNDIR     0x02  //!< Non Connectable advertising
#define GAP_SCAN_EVT_SCAN_RSP          0x04  //!< Scan Response
#define GAP_SCAN_EVT_NONCONN_NONSCAN   0x08  //!< Non Connectable Non Scan advertising
#define GAP_SCAN_EVT_EXT_ADV           0x10  //!< Scan Response of extended adv
#define GAP_SCAN_EVT_EXT_SCAN_RSP      0x14  //!< Scan Response of extended scan response
#define GAP_SCAN_EVT_PER_ADV           0x20  //!< Periodic adv
/** @} EndGAP_SCAN_EVT_TYPE_DEFINES */

/** @defgroup GAP_APPEARANCE_VALUES GAP Appearance Values
 * @{
 */
#define GAP_APPEARE_UNKNOWN                     0x0000 //!< Unknown
#define GAP_APPEARE_GENERIC_PHONE               0x0040 //!< Generic Phone
#define GAP_APPEARE_GENERIC_COMPUTER            0x0080 //!< Generic Computer
#define GAP_APPEARE_GENERIC_WATCH               0x00C0 //!< Generic Watch
#define GAP_APPEARE_WATCH_SPORTS                0x00C1 //!< Watch: Sports Watch
#define GAP_APPEARE_GENERIC_CLOCK               0x0100 //!< Generic Clock
#define GAP_APPEARE_GENERIC_DISPLAY             0x0140 //!< Generic Display
#define GAP_APPEARE_GENERIC_RC                  0x0180 //!< Generic Remote Control
#define GAP_APPEARE_GENERIC_EYE_GALSSES         0x01C0 //!< Generic Eye-glasses
#define GAP_APPEARE_GENERIC_TAG                 0x0200 //!< Generic Tag
#define GAP_APPEARE_GENERIC_KEYRING             0x0240 //!< Generic Keyring
#define GAP_APPEARE_GENERIC_MEDIA_PLAYER        0x0280 //!< Generic Media Player
#define GAP_APPEARE_GENERIC_BARCODE_SCANNER     0x02C0 //!< Generic Barcode Scanner
#define GAP_APPEARE_GENERIC_THERMOMETER         0x0300 //!< Generic Thermometer
#define GAP_APPEARE_GENERIC_THERMO_EAR          0x0301 //!< Thermometer: Ear
#define GAP_APPEARE_GENERIC_HR_SENSOR           0x0340 //!< Generic Heart rate Sensor
#define GAP_APPEARE_GENERIC_HRS_BELT            0x0341 //!< Heart Rate Sensor: Heart Rate Belt
#define GAP_APPEARE_GENERIC_BLOOD_PRESSURE      0x0380 //!< Generic Blood Pressure
#define GAP_APPEARE_GENERIC_BP_ARM              0x0381 //!< Blood Pressure: Arm
#define GAP_APPEARE_GENERIC_BP_WRIST            0x0382 //!< Blood Pressure: Wrist
#define GAP_APPEARE_GENERIC_HID                 0x03C0 //!< Generic Human Interface Device (HID)
#define GAP_APPEARE_HID_KEYBOARD                0x03C1 //!< HID Keyboard
#define GAP_APPEARE_HID_MOUSE                   0x03C2 //!< HID Mouse
#define GAP_APPEARE_HID_JOYSTIC                 0x03C3 //!< HID Joystick
#define GAP_APPEARE_HID_GAMEPAD                 0x03C4 //!< HID Gamepad
#define GAP_APPEARE_HID_DIGITIZER_TYABLET       0x03C5 //!< HID Digitizer Tablet
#define GAP_APPEARE_HID_DIGITAL_CARDREADER      0x03C6 //!< HID Card Reader
#define GAP_APPEARE_HID_DIGITAL_PEN             0x03C7 //!< HID Digital Pen
#define GAP_APPEARE_HID_BARCODE_SCANNER         0x03C8 //!< HID Barcode Scanner
/** @} End GAP_APPEARANCE_VALUES */

typedef void *adv_handle;

struct gap_ble_addr {
    uint8_t addr_type;      //!< Address type: 0=public/1=random
    struct {
        uint8_t addr[6];
    } addr;                 //!< Address value
};

typedef struct gap_adv_param {
    enum gap_adv_mode       adv_mode;         //!< Advertising mode, ref @ gap_adv_mode
    enum gap_adv_disc_mode  disc_mode;        //!< Advertising discovery mode, ref @gap_adv_disc_mode
                                              //!< When adv_mode is in directional mode, disc_mode should be set to GAP_ADV_DISC_MODE_NON_DISC
                                              //!< When adv_mode is GAP_ADV_MODE_BEACON, disc_mode should be set to GAP_ADV_DISC_MODE_BEACON

    enum gap_adv_filter     filt_policy;      //!< Advertising filter policy, ref @ gap_adv_filter
                                              //!< When disc_mode is GAP_ADV_DISC_MODE_GEN_DISC or GAP_ADV_DISC_MODE_LIM_DISC,
                                              //!< filt_policy should be set to GAP_ADV_FILTER_SCAN_ANY_CON_ANY

    enum gap_adv_chan       adv_chnl_map;     //!< Advertising channal map, 37, 38, 39, ref @ gap_adv_chan
    enum gapm_phy_type      phy_mode;         //!< ref @gapm_phy_type
    enum gap_own_addr_type  own_addr_type;    //!< Own address type, ref @gap_own_addr_type
    
    struct gap_ble_addr peer_addr;            //!< Peer addr used for connect, used for direction adv
    uint16_t    adv_intv_min;                 //!< Minimum advertising interval, (in unit of 625us). Must >=  32
    uint16_t    adv_intv_max;                 //!< Maximum advertising interval, (in unit of 625us). Must >=  32

    /* the following is for secondary advertising configuration */
    uint8_t     max_skip;                     //!< Maximum number of advertising events the controller can skip before sending the
                                              //!< AUX_ADV_IND packets. 0 means that AUX_ADV_IND PDUs shall be sent prior each
                                              //!< advertising events
    enum gapm_phy_type  second_phy_mode;      //!< PHY mode for secondary advertising ref @gapm_phy_type
    uint8_t     adv_sid;                      //!< Advertising set idx, for EXTENDED adv and PERIODIC ADV only, range:0~0xF

    /* the following is for periodic advertising */
    uint16_t    per_adv_intv_min;             //!< Minimum periodic advertising interval (in unit of 1.25ms).. Must be greater than 20ms,for PERIODIC ADV only
    uint16_t    per_adv_intv_max;             //!< Maximum periodic advertising interval (in unit of 1.25ms). Must be greater than 20ms,for PERIODIC ADV only
}gap_adv_param_t;

typedef struct gap_scan_param {
    enum gap_scan_type      scan_type;        //!< scan type, ref @ gap_scan_type
    enum gap_own_addr_type  own_addr_type;    //!< Own address type, ref @gap_own_addr_type
    enum gapm_phy_type      phy_mode;         //!< ref @gapm_phy_type. Can not set GAPM_PHY_TYPE_LE_2M

    uint8_t     dup_filt_pol;           //!< scan duplicated pkt filter enbale, 0, donot filter; 1 filter duplicated pkt
    uint16_t    scan_intv;              //!< scan interval, (in unit of 625us). range [4,16384]
    uint16_t    scan_window;            //!< scan window, (in unit of 625us). must <= scan_intv, range [4,16384]
    uint16_t    duration;               //!< Scan duration (in unit of 10ms). 
                                        //!< if duration > 0, when scan_type == GAP_SCAN_TYPE_GEN_DISC or GAP_SCAN_TYPE_LIM_DISC, duration must >= 1024(phy_mode == 1M) or 3072(phy_mode == CODED)
                                        //!< 0 means that the controller will scan continuously until host stop it.
}gap_scan_param_t;

typedef struct gap_conn_param {
    struct gap_ble_addr     peer_addr;        //!< Peer addr used for connect
    enum gap_own_addr_type  own_addr_type;    //!< Own address type, ref @gap_own_addr_type
    enum gapm_phy_type      phy_mode;         //!< ref @gapm_phy_type

    uint16_t    scan_intv;              //!< scan interval, (in unit of 625us). range [4,16384]
    uint16_t    scan_window;            //!< scan window, (in unit of 625us). must <= scan_intv, range [4,16384]

    uint16_t    conn_intv_min;          //!< connection minimum interval (in unit of 1.25ms), should less than conn_intv_max
    uint16_t    conn_intv_max;          //!< connection maximum interval (in unit of 1.25ms), range is 7.5ms to 4s
    uint16_t    supervision_to;         //!< link supervision timeout (in unit of 10ms)
    uint16_t    slave_latency;          //!< number of events than can be missed by a connected slave device
    uint16_t    ce_len_min;             //!< maximum duration of connection event (in unit of 625us)
    uint16_t    ce_len_max;             //!< minumum duration of connection event (in unit of 625us)
}gap_conn_param_t;

typedef struct gap_per_sync_param {
    enum gap_per_sync_type    type;     //!< Periodic synchronization type (@see enum gap_per_sync_type)
    struct gap_ble_addr  adv_addr;      //!< Address of advertiser with which synchronization has to be established (used only if use_pal is false)
    uint16_t             skip;          //!< Number of periodic advertising that can be skipped after a successful receive. Maximum authorized value is 499
    uint16_t             sync_to;       //!< Synchronization timeout for the periodic advertising (in unit of 10ms between 100ms and 163.84s)
    uint8_t              conidx;        //!< Connection index used for periodic sync info reception (only valid for GAP_PER_SYNC_TYPE_PAST)
    uint8_t              adv_sid;       //!< Advertising set idx, for EXTENDED adv and PERIODIC ADV only, range:0~0xF
}gap_per_sync_param_t;

typedef struct gap_per_sync_ctrl {
    bool    adv_report_en;              //!< Periodic advertising reports reception enabled or not
}gap_per_sync_ctrl_t;

typedef struct gap_per_adv_sync_trans {
    enum gap_per_adv_sync_trans_type type;  //!< Indicate if the periodic advertising is local or from another device
    uint8_t     conidx;                     //!< Connection index
    adv_handle  adv;                        //!< Valid if type is GAP_PER_ADV_SYNC_TRANS_TYPE_ADV. Other cases, equal to NULL
    uint16_t    serv_data;                  //!< Service data
}gap_per_adv_sync_trans_t; 

/**@brief Event structure for event GAP_EVT_ADV_REPORT
 */
typedef struct gap_evt_adv_report {
    uint8_t     evt_type;               //!< Bit field providing information about the received report (@see GAP_SCAN_EVT_TYPE_DEFINES)
    struct gap_ble_addr addr;           //!< Target address (in case of a directed advertising report)
    int8_t      tx_pwr;                 //!< TX power (in dBm)
    int8_t      rssi;                   //!< RSSI (between -127 and +20 dBm)
    uint8_t     phy_prim;               //!< Primary PHY on which advertising report has been received
    uint8_t     phy_second;             //!< Secondary PHY on which advertising report has been received
    uint8_t     adv_sid;                //!< Advertising SID. Valid only for periodic advertising report
    uint16_t    period_adv_intv;        //!< Periodic advertising interval (in unit of 1.25ms, min is 7.5ms).Valid only for periodic advertising report
    uint16_t    length;                 //!< Report length
    const uint8_t *data;                //!< Report data
}gap_evt_adv_report_t;

/**@brief Event structure for event GAP_EVT_PER_SYNC_ESTABLISHED
 */
typedef struct gap_evt_sync_established_ind
{
    struct gap_ble_addr addr;           //!< Advertiser address
    uint8_t           phy;              //!< PHY on which synchronization has been established (@see gap_phy_type)
    uint16_t          intv;             //!< Periodic advertising interval (in unit of 1.25ms, min is 7.5ms)
    uint8_t           adv_sid;          //!< Advertising SID
    uint8_t           clk_acc;          //!< Advertiser clock accuracy (@see enum gapm_clk_acc)
    uint16_t          serv_data;        //!< Only valid for a Periodic Advertising Sync Transfer, else ignore
}gap_evt_sync_established_ind_t;

/**@brief Event structure for event GAP_EVT_MASTER_CONNECT or GAP_EVT_SLAVE_CONNECT
 */
typedef struct gap_evt_conn_param {
    uint8_t     conidx;                 //!< Connection index
    struct gap_ble_addr peer_addr;      //!< Peer addr used for connect
    uint16_t    con_interval;           //!< Connection interval
    uint16_t    con_latency;            //!< Slave latency
    uint16_t    sup_to;                 //!< Supervision timeout
}gap_evt_conn_param_t;

/**@brief Event structure for event GAP_EVT_DISCONNECT
 */
typedef struct gap_evt_disconnect {
    uint8_t     conidx;                     //!< Connection index
    uint8_t     reason;                     //!< Disconnect reason
    enum local_device_con_role local_role;   //!< Get the local device role when disconnected
}gap_evt_disconnect_t;

/**@brief Event structure for event GAP_EVT_LINK_PARAM_REJECT
 */
typedef struct gap_evt_link_param_reject {
    uint8_t     conidx;                 //!< Connection index
    uint8_t     status;                 //!< Parameter reject status
}gap_evt_link_param_reject_t;

/**@brief Event structure for event GAP_EVT_LINK_PARAM_UPDATE
 */
typedef struct gap_evt_link_param_update {
    uint8_t     conidx;                 //!< Connection index
    uint16_t    con_interval;           //!< Connection interval (in unit of 1.25ms)
    uint16_t    con_latency;            //!< Connection latency value (in unit of 1.25ms)
    uint16_t    sup_to;                 //!< Supervision timeout
}gap_evt_link_param_update_t;

/**@brief Event structure for event GAP_EVT_LINK_PARAM_REQ
 */
typedef struct gap_evt_link_param_update_req {
    uint8_t     conidx;                 //!< Connection index
    uint16_t    intv_min;               //!< Connection minimum interval (in unit of 1.25ms)
    uint16_t    intv_max;               //!< Connection maximum interval (in unit of 1.25ms)
    uint16_t    latency;                //!< Latency
    uint16_t    time_out;               //!< Supervision timeout (in unit of 10ms)
}gap_evt_link_param_update_req_t;

// Link parameter update rsponse event
typedef struct gap_evt_link_param_update_rsp {
    bool        accept;
    uint8_t     conidx;                 //!< Connection index
    uint16_t    ce_len_min;             //!< Mimimum Connection Event Duration (in unit of 625us)
    uint16_t    ce_len_max;             //!< Maximum Connection Event Duration (in unit of 625us)
}gap_evt_link_param_update_rsp_t;

typedef struct gap_link_param_update_req {
    uint8_t     conidx;                 //!< Connection index
    uint16_t    intv_min;               //!< Connection minimum interval (in unit of 1.25ms), should less than intv_max
    uint16_t    intv_max;               //!< Connection maximum interval (in unit of 1.25ms), range is 7.5ms to 4s
    uint16_t    latency;                //!< Latency
    uint16_t    time_out;               //!< Supervision timeout
}gap_link_param_update_req_t;

typedef struct gap_link_param_update_req_v2 {
    uint8_t     conidx;                 //!< Connection index
    uint16_t    intv_min;               //!< Connection minimum interval (in unit of 1.25ms), should less than intv_max
    uint16_t    intv_max;               //!< Connection maximum interval (in unit of 1.25ms), range is 7.5ms to 4s
    uint16_t    latency;                //!< Latency
    uint16_t    time_out;               //!< Supervision timeout
    uint16_t    ce_len_min;             //!< Mimimum Connection Event Duration (in unit of 625us)
    uint16_t    ce_len_max;             //!< Maximum Connection Event Duration (in unit of 625us)
}gap_link_param_update_req_v2_t;

typedef struct gap_link_param_update_rsp {
    bool        accept;
    uint8_t     conidx;                 //!< Connection index
    uint16_t    ce_len_min;             //!< Mimimum Connection Event Duration (in unit of 625us)
    uint16_t    ce_len_max;             //!< Maximum Connection Event Duration (in unit of 625us)
}gap_link_param_update_rsp_t;

typedef struct gap_ll_pkt_size_exchange_req {
    uint8_t     conidx;                 //!< Connection index
    uint16_t    tx_octets;              //!< Maximum tx packet length on link layer
    uint16_t    tx_time;                //!< Maximum tx packet duration (unit: us) on link layer
}gap_ll_pkt_size_exchange_req_t;

typedef struct gap_security_param {
    bool        mitm;                   //!< Man In The Middle mode enalbe/disable
    bool        secure_connection;      //!< BLE Secure Simple Pairing, also called Secure Connection mode.
    bool        bond;                   //!< Bond or not, if true, then will distribute encryption key, and will check this key_req when bonding.
    enum gap_sec_mode   rsp_mode;       //!< Security rsp mode, ref @enum gap_sec_mode
    enum gap_oob        oob_used;       //!< Oob data exists or not, ref @enum gap_oob
    enum gap_io_cap     io_cap;         //!< IO capbilities, ref @enum gap_io_cap
}gap_security_param_t;

typedef struct gap_security_pair_param {
    enum gap_sec_key_distribution_sel key_dist; //!< Which key will be distributed, ref @enum gap_sec_key_distribution_sel
} gap_security_pair_param_t;

typedef struct gap_bond_ltk {
    uint8_t       ltk[16];              //!< Long Term Key
    uint16_t      ediv;                 //!< Encryption Diversifier
    uint8_t       randnb[8];            //!< Random Number
    uint8_t       key_size;             //!< Encryption key size (7 to 16)
}gap_bond_ltk_t; 

typedef struct gap_bond_info {
    uint8_t    conidx;                  //!< Connection index
    uint8_t    status;                  //!< Pairing results, 0x00 means success
                                        //!< Meaningful only for GAP_EVT_SMP_BOND_SUCCESS and GAP_EVT_SMP_BOND_FAILED events
    struct gap_ble_addr peer_addr;      //!< Peer addr used for connect
    struct gap_ble_addr peer_id_addr;   //!< Peer identity address
    uint8_t    peer_irk[16];            //!< Peer irk
    struct gap_bond_ltk peer_ltk;       //!< Peer ltk
}gap_bond_info_t;

typedef struct gap_channel_map {
    uint8_t     map[5];                 //!< 5-byte channel map array
}gap_channel_map_t;

typedef struct gap_channel_map_ind {
    uint8_t     conidx;                 //!< Connection index
    uint8_t     map[5];                 //!< 5-byte channel map array
}gap_channel_map_ind_t;

typedef struct gap_link_active_info {
    uint8_t     conidx;             /// Connect index
    uint8_t     csa_sel;            /// Frequency hopping algorithm(0: CSA #1, 1: CSA #2) 
    uint8_t     hop_increase;       /// Hop increase
    uint8_t     tx_phy;             /// PHY (0: 1Mbps | 1: 2 Mbps)
    uint8_t     rx_phy;             /// PHY (0: 1Mbps | 1: 2 Mbps)
    uint8_t     ch_map[5];
    uint8_t     peer_device_role;   /// 0: master, 1: slave
    uint8_t     last_unmapped_ch;   /// Used for CSA #1
    uint16_t    latency;
    uint16_t    con_interval;
    uint32_t    access_code;
    uint32_t    crc;                /// CRC init Low 3 bytes vaild
    uint16_t    master_sca;         /// Master Sleep clock accuracy (in ppm)
    uint16_t    event_counter;      /// Used for CSA #2
}gap_link_active_info_t;

typedef struct gap_scan_filter_param {
    uint8_t                         filter_id;
    union {
        struct {
            enum gap_scan_filter_bf filter_bf;              //!< Filter bit field ,ref @enum gap_scan_filter_bf
            uint8_t                 filter_evt_type;        //!< Ref @defgroup GAP_SCAN_EVT_TYPE_DEFINES.
                                                            //!< Except for GAP_SCAN_EVT_EXT_ADV, GAP_SCAN_EVT_EXT_SCAN_RSP and GAP_SCAN_EVT_PER_ADV 
            int8_t                  filter_rssi;            //!< Advertising reports with RSSI less than this value will be filtered
            struct gap_ble_addr     filter_addr;            //!< Advertising reports other than this address will be filtered
            uint8_t                 filter_adv_data_length; //!< The data length of the specified filtered Advertising
            uint8_t                 *filter_adv_data;       //!< Advertising reports containing this Advertising data will not be filtered
        } add;
    } p;
} gap_scan_filter_param_t;

struct gap_event {
    enum gap_event_type type;           //!< GAP event type
    union {
        uint8_t conidx;

        struct {
            uint8_t act_id;
            uint8_t status;
        } adv_set_param;                    //!< Value of GAP_EVT_ADV_SET_PARAM event

        struct {
            uint8_t act_id;
            uint8_t status;
        } adv_set_adv_data;                 //!< Value of GAP_EVT_ADV_SET_ADV_DATA event

        struct {
            uint8_t act_id;
            uint8_t status;
        } adv_set_scan_rsp;                 //!< Value of GAP_EVT_ADV_SET_SCAN_RSP event

        struct {
            uint8_t act_id;
            uint8_t status;
        } adv_set_period_data;              //!< Value of GAP_EVT_ADV_SET_PER_DATA event

        struct {
            uint8_t act_id;
            uint8_t status;
        } adv_start;                        //!< Value of GAP_EVT_ADV_START event
        
        struct {
            uint8_t act_id;
            uint8_t status;
        } adv_end;                          //!< Advertising end status

        uint8_t scan_start_status;          //!< Scanning start status
        uint8_t scan_end_status;            //!< Scanning end status
        struct gap_evt_adv_report *adv_rpt; //!< Value of GAP_EVT_ADV_REPORT event, scanning results
        uint8_t conn_start_status;          //!< Connection start reason
        uint8_t conn_end_status;            //!< Connection end reason
        uint8_t per_sync_start_status;      //!< Periodic synchronization start status
        uint8_t per_sync_end_status;        //!< Periodic synchronization end status
        uint8_t per_sync_ctrl_status;       //!< Periodic synchronization control status
        uint8_t per_adv_sync_trans_status;  //!< Transfer periodic advertising sync information status

        struct gap_evt_sync_established_ind per_sync_established;   //!< Value of GAP_EVT_PER_SYNC_ESTABLISHED event

        struct {
            uint8_t conidx;
            uint16_t token;
        } name_req;                         //!< Value of GAP_EVT_NAME_REQ event

        struct {
            uint8_t conidx;
            uint16_t token;
        } slv_pref_param_req;               //!< Value of GAP_EVT_SLAVE_PREF_PARAM event

        struct {
            uint8_t conidx;
            uint16_t token;
        } appearance_req;                   //!< Value of GAP_EVT_APPEARANCE_REQ event

        struct gap_evt_conn_param connect;    //!< Value of GAP_EVT_MASTER_CONNECT or GAP_EVT_SLAVE_CONNECT event
        struct gap_evt_disconnect disconnect; //!< Value of GAP_EVT_DISCONNECT event

        struct {
            uint8_t conidx;
            uint32_t number;
        } nc_req;                             //!< Value of GAP_EVT_SMP_NC_REQ

        struct {
            uint8_t conidx;
            enum smp_tk_type tk_type;
        } tk_req;                             //!< Value of GAP_EVT_SMP_TK_REQ event

        struct {
            uint8_t conidx;
        } oob_exc_req;                       //!< Value of GAP_EVT_SMP_OOB_EXC_REQ event

        struct {
            uint8_t conidx;
            uint8_t status;
            uint8_t loc_rand[16];
            uint8_t loc_conf_value[16];
        } oob_data_gen_result;                //!< Value of GAP_EVT_SMP_OOB_DATA_GEN_RESULT event

        struct {
            uint8_t conidx;
        } pair_req;                           //!< Value of GAP_EVT_SMP_PAIR_REQ event

        struct {
            uint8_t conidx;                   
        } security_req;                       //!< Value of GAP_EVT_SMP_SEC_REQ event

        struct gap_bond_info bond;            //!< Value of GAP_EVT_SMP_BOND_SUCCESS and GAP_EVT_SMP_BOND_FAILED event

        struct {
            uint8_t conidx;    
            uint8_t ltk[16];                  //!< Can be used for gap_encrypt_rsp()         
        } encrypt_req;                        //!< Value of GAP_EVT_SMP_ENCRYPT_REQ event

        struct {
            uint8_t conidx;
            uint8_t status;                   //!< Encrypt status, 0x00 means success
        } encrypt;                            //!< Value of GAP_EVT_SMP_ENCRYPT_SUCCESS or GAP_EVT_SMP_ENCRYPT_FAILED event

        struct gap_evt_link_param_reject link_param_reject;             //!< Value of GAP_EVT_LINK_PARAM_REJECT event, Connect param reject
        struct gap_evt_link_param_update link_param_update;             //!< Value of GAP_EVT_LINK_PARAM_UPDATE event, Connect param update
        struct gap_evt_link_param_update_req link_param_update_req;     //!< Value of GAP_EVT_LINK_PARAM_REQ event
        struct gap_evt_link_param_update_rsp link_param_update_rsp;

        struct {
            uint8_t conidx;
            uint16_t mtu;
        } mtu_ind;                          //!< Value of GATT_EVT_MTU event, mtu update indication

        struct {
            uint8_t conidx;
            uint16_t tx_octets;
            uint16_t tx_time;
            uint16_t rx_octets;
            uint16_t rx_time;
        } ll_pkt_size;                      //!< Value of GAP_EVT_LL_PKT_SIZE_IND event

        struct {
            uint8_t conidx;
            int8_t  link_rssi;              //!< Peer device RSSI value
        } gap_link_rssi;                    //!< Value of GAP_EVT_LINK_RSSI event

        struct {
            uint8_t  conidx;                //!< Connection index
            uint16_t compid;                //!< Manufacturer name
            uint16_t lmp_subvers;           //!< LMP subversion
            uint8_t  lmp_vers;              //!< LMP version
        } gap_link_ver;                     //!< Value of GAP_EVT_LINK_VER event

        struct {
            uint8_t conidx;                 //!< Connection index
            uint8_t features[8];            //!< Features bitmask
        } gap_link_feature;                 //!< Value of GAP_EVT_LINK_FEATURE event

        struct {
            uint8_t conidx;
            uint8_t tx_phy;                 //!< ref @gapm_phy_type
            uint8_t rx_phy;                 //!< ref @gapm_phy_type
        } gap_phy_ind;                      //!< Value of GAP_EVT_PHY_IND event, PHY update success event or get phy success

        struct {
            uint8_t conidx;
            uint8_t status;                 //!< Reason of reject
        } gap_phy_update_reject;            //!< Value of GAP_EVT_PHY_REJECT event, PHY update rejected event

        uint8_t add_device_to_wl_status;    //!< Value of GAP_EVT_ADD_DEVICE_TO_WL event
        uint8_t rmv_device_from_wl_status;  //!< Value of GAP_EVT_RMV_DEVICE_FROM_WL event
        uint8_t add_device_to_ral_status;   //!< Value of GAP_EVT_ADD_DEVICE_TO_TAL event
        uint8_t profile_added_status;       //!< Value of GATT_EVT_PROFILE_ADDED event, profile added status

        struct gap_channel_map_ind ch_map_ind; //!< Value of GAP_EVT_CH_MAP_IND event
        struct gap_link_active_info link_info; //!< Value of GAP_EVT_LINK_ACTIVE_INFO event
        struct {
            uint8_t filter_id;
            uint8_t status;                 //!< 0x00 mean success, others error
        } scan_filter_add;                  //!< Value of GAP_EVT_SCAN_FILTER_ADD_CMP event
        struct {
            uint8_t filter_id;
            uint8_t status;                 //!< 0x00 mean success, others error
        } scan_filter_remove;               //!< Value of GAP_EVT_SCAN_FILTER_REMOVE_CMP event
    } param;
};

typedef uint16_t(*gap_callback_func_t)(struct gap_event *event);   //!< Gap callback function define

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/
extern gap_callback_func_t gap_evt_callback;

/* Exported functions --------------------------------------------------------*/

/*
 * @fn          gap_reset_local_address
 * 
 * @brief       reset local static address(public or random static).
 *              Must stop advertising before configure.
 *              After address change, the advertising parameters and data must be reset
 *
 * @param[in]   new_addr : pointer to new address info
 *
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gap_reset_local_address(struct gap_ble_addr *new_addr);

/*
 * @fn          gap_get_local_address
 * 
 * @brief       get local static address(public or random static).
 *
 * @param[in]   local_addr : pointer to local address info
 *
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gap_get_local_address(struct gap_ble_addr *local_addr);

/*
 * @fn          gap_set_fast_adv
 * 
 * @brief       set fast adv
 *
 * @return      None
 */
void gap_set_fast_adv(bool enable);

/*
 * @fn          gap_set_conn_req_rssi_threshold
 * 
 * @brief       set conn req rssi threshold
 *
 * @param[in]   rssi : rssi threshold
 *
 * @return      None
 */
void gap_set_conn_req_rssi_threshold(int8_t rssi);

/*
 * @fn          gap_adv_create
 * 
 * @brief       Create an advertising activity before any advertising operation. This
 *              function will return a handle used by following operations.
 *
 * @return      operation handle
 */
adv_handle gap_adv_create(void);

/*
 * @fn          gap_adv_handle_act_id_get
 * 
 * @brief       Get the act_id corresponding to adv_handle for distinguishing different adv_handles during multi advertising.
 *              For example, @struct gap_event.param.adv_set_param.act_id
 *              Act_id is only valid after calling function gap_adv_set_param()
 *
 * @return      act_id
 */
uint8_t gap_adv_handle_act_id_get(adv_handle handle);

/*
 * @fn          gap_get_adv_state
 * 
 * @brief       Obtain the state of the specified advertising.
 * 
 * @param[in]   adv : advertising operation handle
 *
 * @return      enum gap_adv_state
 */
enum gap_adv_state gap_get_adv_state(adv_handle adv);

/*
 * @fn          gap_adv_set_param
 * 
 * @brief       used to create advertising entity with parameters defined by user
 *
 * @param[in]   adv : advertising operation handle
 * @param[in]   param : advertising parameters
 *
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gap_adv_set_param(adv_handle adv, struct gap_adv_param *param);

/*
 * @fn          gap_adv_type_flag_src_set
 * 
 * @brief       Generally, there is a field (such as 02 01 06) in advertising data called
 *              flags, this function is used to set who takes charge of generating this field.
 *
 * @param[in]   src: @ref adv_type_flag_src_t
 *
 * @return      None
 */
void gap_adv_type_flag_src_set(enum adv_type_flag_src_t src);
/*
 * @fn          gap_adv_set_adv_data
 * 
 * @brief       used to set advertising data
 *
 * @param[in]   adv : advertising operation handle
 * @param[in]   data : advertising data buffer pointer
 * @param[in]   length : advertising data length
 *
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gap_adv_set_adv_data(adv_handle adv, const uint8_t *data, uint16_t length);

/*
 * @fn          gap_adv_set_scan_rsp
 * 
 * @brief       used to set scan response data
 *
 * @param[in]   adv : advertising operation handle
 * @param[in]   data : scan response data buffer pointer
 * @param[in]   length : scan response data length
 *
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gap_adv_set_scan_rsp(adv_handle adv, const uint8_t *data, uint16_t length);

/*
 * @fn          gap_adv_set_period_data
 * 
 * @brief       used to set periodic advertising data
 *
 * @param[in]   adv : advertising operation handle
 * @param[in]   data : periodic advertising data buffer pointer
 * @param[in]   length : periodic advertising data length
 *
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gap_adv_set_period_data(adv_handle adv, const uint8_t *data, uint16_t length);

/*
 * @fn          gap_adv_start
 * 
 * @brief       start advertising
 *
 * @param[in]   adv : advertising operation handle
 * @param[in]   duration : advertising duration, unit: 10ms. 0 means that advertising will not terminate
 *                         automaticly.
 * @param[in]   max_adv_evt : Maximum number of extended advertising events the controller shall attempt to send prior to
 *                            terminating the extending advertising. Valid only if extended advertising.
 *                            0 means taht extended advertising events will always be sent
 *
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gap_adv_start(adv_handle adv, uint16_t duration, uint8_t max_adv_evt);

/*
 * @fn          gap_adv_stop
 * 
 * @brief       stop on-going advertising
 *
 * @param[in]   adv : advertising operation handle
 *
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gap_adv_stop(adv_handle adv);

/*
 * @fn          gap_scan_start
 * 
 * @brief       start scan advertising devices
 *
 * @param[in]   param : scan parameters
 *
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gap_scan_start(struct gap_scan_param *param);

/*
 * @fn          gap_scan_stop
 * 
 * @brief       stop on-going scan
 *
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gap_scan_stop(void);

/*
 * @fn          gap_conn_start
 * 
 * @brief       start connect request to remote device, this function should not be called when
 *              there is an on-going connection request.
 *
 * @param[in]   param : connection parameters
 *
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gap_conn_start(struct gap_conn_param *param);

/*
 * @fn          gap_conn_stop
 * 
 * @brief       stop out-going connection request
 *
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gap_conn_stop(void);

/*
 * @fn          gap_per_sync_start
 * 
 * @brief       Start a periodic synchronization activity
 *
 * @param[in]   param : periodic synchronization parameters
 *
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gap_per_sync_start(struct gap_per_sync_param *param);

/*
 * @fn          gap_per_sync_stop
 * 
 * @brief       Stop a periodic synchronization activity
 *
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gap_per_sync_stop(void);

/*
 * @fn          gap_per_sync_ctrl
 * 
 * @brief       Enable/Disable reception of periodic advertising report
 *
 * @param[in]   param : periodic synchronization control parameters
 *
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gap_per_sync_ctrl(struct gap_per_sync_ctrl *param);

/*
 * @fn          gap_per_adv_sync_trans
 * 
 * @brief       Transfer periodic advertising sync information to peer device
 *
 * @param[in]   param : periodic advertising sync information
 *
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gap_per_adv_sync_trans(struct gap_per_adv_sync_trans *param);

/*
 * @fn          gap_disconnect
 * 
 * @brief       remove an existing connection
 *
 * @param[in]   conidx : connection index
 * 
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gap_disconnect(uint8_t conidx);

/*
 * @fn          gap_disconnect_v2
 * 
 * @brief       Disconnect with reason
 *
 * @param[in]   conidx : connection index
 * @param[in]   reason : see Core5.3 | Vol 1, Part F(Controller error codes)
 * 
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gap_disconnect_v2(uint8_t conidx, uint8_t reason);

/*
 * @fn          gap_get_connect_num
 * 
 * @brief       get the current active connections number
 *
 * @return      number of active connections
 */
uint8_t gap_get_connect_num(void);

/*
 * @fn          gap_get_connect_num_through_role
 * 
 * @brief       Get the number of devices currently connected when the local device as the master or slave role
 * 
 * @param[in]   role : local device role
 *
 * @return      number of active connections
 */
uint8_t gap_get_connect_num_through_role(enum local_device_con_role role);

/*
 * @fn          gap_get_local_role
 * 
 * @brief       Get the local device role
 *
 * @param[in]   conidx : connection index
 * 
 * @return      the connection role
 */
enum local_device_con_role gap_get_local_dev_role(uint8_t conidx);

/*
 * @fn          gap_get_connect_status
 * 
 * @brief       Get the connection status with a peer device
 *
 * @param[in]   conidx : connection index
 * 
 * @return      the connection status
 */
bool gap_get_connect_status(uint8_t conidx);

/*
 * @fn          gap_act_done
 * 
 * @brief       this function will be called after action has been handled by host layer
 *
 * @param[in]   operation : last requested action
 * @param[in]   status : handle result
 * @param[in]   act_id : action id
 */
void gap_act_done(uint8_t operation, uint8_t status, uint8_t act_id);

/*
 * @fn          gap_param_update_rsp
 * 
 * @brief       stop out-going connection request
 *
 * @param[in]   rsp : link param update response parameters, @ref gap_link_param_update_rsp_t
 * 
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gap_param_update_rsp(struct gap_link_param_update_rsp *rsp);

/*
 * @fn          gap_param_update_req
 * 
 * @brief       send connection parameters update request from ble peripheral device.
 *
 * @param[in]   req : link param update request parameters, @ref gap_link_param_update_req_t
 * 
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gap_param_update_req(struct gap_link_param_update_req *req);

/*
 * @fn          gap_param_update_req_v2
 * 
 * @brief       send connection parameters update request from ble peripheral device.
 *
 * @param[in]   req : link param update request parameters, @ref gap_link_param_update_req_v2_t
 * 
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gap_param_update_req_v2(struct gap_link_param_update_req_v2 *req);

/*
 * @fn          gap_ll_pkt_size_exchange_req
 * 
 * @brief       used to exchange maximum packet length on link layer.
 *
 * @param[in]   req : request parameters, @ref gap_evt_link_param_update_rsp
 * 
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gap_ll_pkt_size_exchange_req(struct gap_ll_pkt_size_exchange_req *req);

/*
 * @fn          gap_name_req_rsp
 * 
 * @brief       send response to name request from GAP
 *
 * @param[in]   conidx: connection index
 * @param[in]   token: received from GAP_EVT_NAME_REQ
 * @param[in]   length: name length
 * @param[in]   name: name string
 * 
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gap_name_req_rsp(uint8_t conidx, uint16_t token, uint16_t length, uint8_t *name);

/*
 * @fn          gap_appearance_req_rsp
 * 
 * @brief       send response to appearance request from GAP
 *
 * @param[in]   conidx: connection index
 * @param[in]   token: received from GAP_EVT_NAME_REQ
 * @param[in]   appearance: appearance, see @defgroup GAP_APPEARANCE_VALUES
 * 
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gap_appearance_req_rsp(uint8_t conidx, uint16_t token, uint16_t appearance);

/*
 * @fn          gap_slave_pref_param_req_rsp
 * 
 * @brief       send response to slave prefer parameter request from GAP
 *
 * @param[in]   conidx: connection index
 * @param[in]   token: received from GAP_EVT_NAME_REQ
 * @param[in]   con_intv_min: minimum connection interval
 * @param[in]   con_intv_max: maximum connection interval
 * @param[in]   slave_latency: slave latency
 * @param[in]   conn_timeout: connection timeout
 * 
 * @return      @ref app_err_t
 */
ble_api_err_t gap_slave_pref_param_req_rsp(uint8_t conidx, uint16_t token,
                                                uint16_t con_intv_min, uint16_t con_intv_max,
                                                uint16_t slave_latency, uint16_t conn_timeout);

/*
 * @fn          gap_set_cb_func
 *
 * @brief       GAP callback functions set. Links application layer callbacks to GAP layer.
 *
 * @param[in]   gap_evt_cb : Application layer callback function, defined in application layer.
 * 
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gap_set_cb_func(gap_callback_func_t gap_evt_cb);

/*
 * @fn          gap_security_param_init
 *
 * @brief       Used to set SMP parameters.
 *
 * @param[in]   sec_param : SMP parameters. @ref gap_security_param.
 * 
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gap_security_param_init(struct gap_security_param *sec_param);

/*
 * @fn          gap_security_set_bond_list_size
 *
 * @brief       Used to set the maximum size of the list for storing bond information.
 *              The default value is 12.
 *              Can only be set once.
 *
 * @param[in]   max_size :  size of the bond info list.
 *                          range: 1 - 16.
 * 
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gap_security_set_bond_list_size(uint8_t max_size);

/*
 * @fn          gap_security_tk_rsp
 *
 * @brief       Used to response Temporary Key request from GAP in pairing procedure. When local io-cap is
 *              GAP_IO_CAP_DISPLAY_ONLY, this function is used to send local generated TK, also this value
 *              should be showed in display. When local io-cap is GAP_IO_CAP_KB_ONLY, this function is used
 *              to send local TK input by keyboard.
 *
 * @param[in]   conidx : connection index
 * @param[in]   tk : temporary key, the value should be between 000000 and 999999
 * @param[in]   accept: true  - Agree to pairing .
 *                      false - Refuse pairing.
 * 
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gap_security_tk_rsp(uint8_t conidx, uint32_t tk, bool accept);

/*
 * @fn          gap_security_nc_rsp
 *
 * @brief       Used to response Numbric Comparision request from GAP in pairing procedure.
 *
 * @param[in]   conidx : connection index
 * @param[in]   accept : true  - Agree to pairing. 
 *                       false - Refuse pairing.
 * 
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gap_security_nc_rsp(uint8_t conidx, bool accept);

/*
 * @fn          gap_security_oob_exchange
 *
 * @brief       Used to verify validation of received confirm data from peer device through OOB.
 *
 * @param[in]   conidx  : connection index
 * @param[in]   accept  : true  - Agree to pairing req. 
 *                        false - Refuse pairing req.
 * @param[in]   rem_conf : Remote confirm value obtained from OOB. If there is no peer device's OOB data, set it to 0
 * @param[in]   rem_rand  : Remote random value obtained from OOB. If there is no peer device's OOB data, set it to 0
 * @param[in]   loc_rand  : Local random value, the return value in event GAP_EVT_SMP_OOB_DATA_GEN_RESULT.
 *  
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gap_security_oob_exchange(uint8_t conidx, bool accept, uint8_t *rem_conf, uint8_t *rem_rand, uint8_t *loc_rand);

/*
 * @fn          gap_security_generate_oob_data
 *
 * @brief       Used to generatr the data required for OOB pairing.
 *
 * @param[in]   conidx  : connection index
 * 
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gap_security_generate_oob_data(uint8_t conidx);

/*
 * @fn          gap_security_req
 *
 * @brief       Used to send security requests to the master device when the local device is a slave device,
 *              The master device will send a pairing request if it agrees to the security request
 *              
 *
 * @param[in]   conidx : connection index
 * 
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gap_security_req(uint8_t conidx);

/*
 * @fn          gap_pairing_req
 *
 * @brief       Used to send pairing request when the local device is the master
 *
 * @param[in]   conidx : connection index
 * 
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gap_pairing_req(uint8_t conidx);

/*
 * @fn          gap_pairing_req_v2
 *
 * @brief       Used to send pairing request when the local device is the master
 *
 * @param[in]   conidx : connection index
 * @param[in]   add_param: additional params, detailed reference struct gap_security_pair_param
 * 
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gap_pairing_req_v2(uint8_t conidx, struct gap_security_pair_param add_param);

/*
 * @fn          gap_pairing_rsp
 *
 * @brief       Used to send pairing response when event GAP_EVT_SMP_PAIR_REQ occurs
 *
 * @param[in]   conidx : connection index
 * @param[in]   accept : true  - Agree to pairing req. 
 *                       false - Refuse pairing req.
 * 
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gap_pairing_rsp(uint8_t conidx, bool accept);

/*
 * @fn          gap_pairing_rsp_v2
 *
 * @brief       Used to send pairing response when event GAP_EVT_SMP_PAIR_REQ occurs
 *
 * @param[in]   conidx : connection index
 * @param[in]   accept : true  - Agree to pairing req. 
 *                       false - Refuse pairing req.
 * @param[in]   add_param: additional params, detailed reference struct gap_security_pair_param
 * 
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gap_pairing_rsp_v2(uint8_t conidx, bool accept, struct gap_security_pair_param add_param);

/*
 * @fn          gap_encrypt_req
 *
 * @brief       Used to send encrypt request when the local device is the master
 *
 * @param[in]   conidx : connection index
 * 
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gap_encrypt_req(uint8_t conidx);

/*
 * @fn          gap_encrypt_rsp
 *
 * @brief       Used to send encrypt response when event GAP_EVT_SMP_ENCRYPT_REQ occurs
 *
 * @param[in]   conidx : connection index
 * @param[in]   ltk : long Term Key information 
 * @param[in]   accept : true  - Agree to encryption req. 
 *                       false - Refuse encryption req.
 * 
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gap_encrypt_rsp(uint8_t conidx, uint8_t *ltk, bool accept);

/*
 * @fn          gap_get_bond_status
 *
 * @brief       Get bond info status of peer device.
 *
 * @param[in]   conidx : connection index
 * 
 * @return      @ref true: bond info existed, false: bond info not existed
 */
bool gap_get_bond_status(uint8_t conidx);

/*
 * @fn          gap_get_encrypted_status
 *
 * @brief       Get encrypted status of peer device.
 *
 * @param[in]   conidx : connection index
 * 
 * @return      @ref encrypted status
 */
bool gap_get_encrypted_status(uint8_t conidx);

/*
 * @fn          gap_get_bond_info
 *
 * @brief       Get a bond information.
 *
 * @param[in]   bond_info_list_index - index of the bond information list in the ble stack.
 *                                     range:0 ~ (value set by gap_security_set_bond_list_size() - 1).
 * @param[in]   out_bond_info - pointer to bond info
 * 
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gap_get_bond_info(uint8_t bond_info_list_index, gap_bond_info_t *out_bond_info);

/*
 * @fn          gap_get_bond_info_by_conidx
 *
 * @brief       Get a bond information by conidx.
 *              The device of this conidx needs to be in a connected state.
 *
 * @param[in]   conidx : connection index
 * @param[in]   out_bond_info - pointer to bond info
 * 
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gap_get_bond_info_by_conidx(uint8_t conidx, gap_bond_info_t *out_bond_info);

/*
 * @fn          gap_delete_bond_info
 *
 * @brief       Erase a bond information.
 *
 * @param[in]   peer_id_addr - pointer to peer device identity address
 * 
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gap_delete_bond_info(struct gap_ble_addr *peer_id_addr);

/*
 * @fn          gap_deletet_all_bond_info
 *
 * @brief       Erase all bond information.
 *
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gap_deletet_all_bond_info(void);

/*
 * @fn          gap_get_conn_phy
 *
 * @brief       Read active link PHY mode.
 *              The phy report will be uploaded to user laye as GAP_EVENT:GAP_EVT_PHY_IND.
 *              This function sample is:
 *              case GAP_EVT_PHY_IND:
 *                  co_printf("link tx phy: %d\r\n",event->param.gap_phy_ind.tx_phy);
 *                  ...
 *              break;
 *
 * @param[in]   conidx : connection index
 * 
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gap_get_conn_phy(uint8_t conidx);

/*
 * @fn          gap_conn_phy_update
 *
 * @brief       Set active link PHY mode.
 *
 * @param[in]   conidx : connection handle/index of the connection.
 * @param[in]   tx_phy : TX PHY mode.   //!< see @enum gapm_phy_mode
 * @param[in]   rx_phy : RX PHY mode.   //!< see @enum gapm_phy_mode
 * 
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gap_conn_phy_update(uint8_t conidx, enum gapm_phy_type tx_phy, enum gapm_phy_type rx_phy);

/*
 * @fn          gap_get_link_rssi
 *
 * @brief       Used to send a request to lower layer to get current rssi of link peer device.
 *              The rssi report will be uploaded to user laye as GAP_EVENT:GAP_EVT_LINK_RSSI.
 *              This function sample is:
 *              case GAP_EVT_LINK_RSSI:
 *                  co_printf("link rssi: %d\r\n",event->param.gap_link_rssi.link_rssi);
 *                  ...
 *              break;
 *
 * @param[in]   conidx : connection index
 * 
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gap_get_link_rssi(uint8_t conidx);

/*
 * @fn          gap_get_link_features
 *
 * @brief       Used to send a request to lower layer to get features of link peer device.
 *              The feature report will be uploaded to user laye as GAP_EVENT:GAP_EVT_LINK_FEATURE.
 *
 * @param[in]   conidx : connection index
 * 
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gap_get_link_features(uint8_t conidx);

/*
 * @fn          gap_get_link_version
 *
 * @brief       Used to send a request to lower layer to get version info of link peer device.
 *              The version report will be uploaded to user laye as GAP_EVENT:GAP_EVT_LINK_VER.
 *
 * @param[in]   conidx : connection index
 * 
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gap_get_link_version(uint8_t conidx);

/*
 * @fn          gap_add_device_to_white_list
 *
 * @brief       Add device to white list.
 *
 * @param[in]   peer_id_addr - pointer to peer device identity address, which will be added to white list
 * @param[in]   size         - device number,rang:1~22. how many devices will be added to white list
 * 
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gap_add_device_to_white_list(struct gap_ble_addr *peer_id_addr, uint8_t size);

/*
 * @fn          gap_remove_device_from_white_list
 *
 * @brief       Remove device from white list.
 *
 * @param[in]   peer_id_addr - pointer to peer device identity address, which will be removed from white list
 * @param[in]   size         - device number,range:0~22. how many devices will be removed from white list.
 *                             when size is 0, All devices in the white list will be removed
 * 
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gap_remove_device_from_white_list(struct gap_ble_addr *peer_id_addr, uint8_t size);

/*
 * @fn          gap_add_device_to_resolve_list
 *
 * @brief       Add device to resolve list.
 *
 * @param[in]   peer_id_addr - pointer to peer device identity address, which will be added to resolve list
 * @param[in]   irk_array    - peer device irk array.Each irk has a length of 16.
 * @param[in]   size         - device number,range:1~3. how many devices will be added to resolve list.
 *                        
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gap_add_device_to_resolve_list(struct gap_ble_addr *peer_id_addr, uint8_t *irk_array, uint8_t size);

/*
 * @fn          gap_get_link_active_info
 *
 * @brief       Get current connection information.
 *              The info report will be uploaded to user laye as GAP_EVENT:GAP_EVT_LINK_ACTIVE_INFO
 *
 * @param[in]   conidx : connection index
 *                        
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gap_get_link_active_info(uint8_t conidx);

/*
 * @fn          gap_scan_filter_add
 *
 * @brief       Set filtering condition for advertising reports reported by scanning operations.
 *              Advertising reports that do not meet the condition will be filtered out by the controller.
 *              Note: 1.Extended advertising will not be filtered   
 *                    2.Cannot be called after scanning starts.
 *                    3.When adding a scan response package to the list, its corresponding advertising package also needs to be added to the list
 *
 * @param[in]   param : pointer to gap scan filter param
 *                        
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gap_scan_filter_add(struct gap_scan_filter_param *param);

/*
 * @fn          gap_scan_filter_remove
 *
 * @brief       Remove filtering condition
 *              Note: 1.Cannot be called after scanning starts.
 *
 * @param[in]   param : pointer to gap scan filter param
 *                        
 * @return      @ref ble_api_err_t
 */
ble_api_err_t gap_scan_filter_remove(struct gap_scan_filter_param *param);

/*
 * @fn          gap_enable_ch_map_ind_event
 *
 * @brief       Control the reporting of event GAP_EVT_CH_MAP_IND.
 *
 * @param[in]   enable : true  - Event GAP_EVT_CH_MAP_IND will be reported
 *                       false - Event GAP_EVT_CH_MAP_IND will not be reported
 *                        
 * @return      @ref None
 */
void gap_enable_ch_map_ind_event(bool enable);

/*
 * @fn          gap_scan_accept_set
 *
 * @brief       Control the reporting of Non-Discoverable mode Advertising.
 *
 * @param[in]   flag(ref @enum scan_accept_t).
 *                        
 * @return      @ref None
 */
void gap_scan_accept_set(enum scan_accept_t flag);

#ifdef __cplusplus
}
#endif

#endif  // _GAP_API_H

