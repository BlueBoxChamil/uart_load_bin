#ifndef _GAP_DEFINES_H
#define _GAP_DEFINES_H

/// Own BD address source of the device
enum gapm_own_addr
{
   /// Public or Private Static Address according to device address configuration
   GAPM_STATIC_ADDR,
   /// Generated resolvable private random address
   GAPM_GEN_RSLV_ADDR,
   /// Generated non-resolvable private random address
   GAPM_GEN_NON_RSLV_ADDR,
};

/// PHY Type
enum gapm_phy_type
{
    /// LE 1M
    GAPM_PHY_TYPE_LE_1M = 1,
    /// LE 2M
    GAPM_PHY_TYPE_LE_2M,
    /// LE Coded
    GAPM_PHY_TYPE_LE_CODED,
};

/// PHY Type
enum gapm_phy_mask
{
    /// LE 1M
    GAPM_PHY_MASK_LE_1M = (1 << 0),
    /// LE 2M
    GAPM_PHY_MASK_LE_2M = (1 << 1),
    /// LE Coded
    GAPM_PHY_MASK_LE_CODED = (1 << 2),
    /// ALL
    GAPM_PHY_MASK_ALL = (GAPM_PHY_MASK_LE_1M | GAPM_PHY_MASK_LE_2M | GAPM_PHY_MASK_LE_CODED),
};

/// Type of advertising that can be created
enum gapm_adv_type
{
    /// Legacy advertising
    GAPM_ADV_TYPE_LEGACY = 0,
    /// Extended advertising
    GAPM_ADV_TYPE_EXTENDED,
    /// Periodic advertising
    GAPM_ADV_TYPE_PERIODIC,
};

/// Advertising discovery mode
enum gapm_adv_disc_mode
{
    /// Mode in non-discoverable
    GAPM_ADV_MODE_NON_DISC = 0,
    /// Mode in general discoverable
    GAPM_ADV_MODE_GEN_DISC,
    /// Mode in limited discoverable
    GAPM_ADV_MODE_LIM_DISC,
    /// Broadcast mode without presence of AD_TYPE_FLAG in advertising data
    GAPM_ADV_MODE_BEACON,
    GAPM_ADV_MODE_MAX,
};

/// Advertising properties bit field bit positions
enum gapm_adv_prop_bf
{
    /// Indicate that advertising is connectable, reception of CONNECT_REQ or AUX_CONNECT_REQ
    /// PDUs is accepted. Not applicable for periodic advertising.
    GAPM_ADV_PROP_CONNECTABLE_POS     = 0,
    GAPM_ADV_PROP_CONNECTABLE_BIT     = 0x0001,

    /// Indicate that advertising is scannable, reception of SCAN_REQ or AUX_SCAN_REQ PDUs is
    /// accepted
    GAPM_ADV_PROP_SCANNABLE_POS       = 1,
    GAPM_ADV_PROP_SCANNABLE_BIT       = 0x0002,

    /// Indicate that advertising targets a specific device. Only apply in following cases:
    ///   - Legacy advertising: if connectable
    ///   - Extended advertising: connectable or (non connectable and non discoverable)
    GAPM_ADV_PROP_DIRECTED_POS        = 2,
    GAPM_ADV_PROP_DIRECTED_BIT        = 0x0004,

    /// Indicate that High Duty Cycle has to be used for advertising on primary channel
    /// Apply only if created advertising is not an extended advertising
    GAPM_ADV_PROP_HDC_POS             = 3,
    GAPM_ADV_PROP_HDC_BIT             = 0x0008,

    /// Bit 4 is reserved
    GAPM_ADV_PROP_RESERVED_4_POS      = 4,
    GAPM_ADV_PROP_RESERVED_4_BIT      = 0x0010,

    /// Enable anonymous mode. Device address won't appear in send PDUs
    /// Valid only if created advertising is an extended advertising
    GAPM_ADV_PROP_ANONYMOUS_POS       = 5,
    GAPM_ADV_PROP_ANONYMOUS_BIT       = 0x0020,

    /// Include TX Power in the extended header of the advertising PDU.
    /// Valid only if created advertising is not a legacy advertising
    GAPM_ADV_PROP_TX_PWR_POS          = 6,
    GAPM_ADV_PROP_TX_PWR_BIT          = 0x0040,

    /// Include TX Power in the periodic advertising PDU.
    /// Valid only if created advertising is a periodic advertising
    GAPM_ADV_PROP_PER_TX_PWR_POS      = 7,
    GAPM_ADV_PROP_PER_TX_PWR_BIT      = 0x0080,

    /// Indicate if application must be informed about received scan requests PDUs
    GAPM_ADV_PROP_SCAN_REQ_NTF_EN_POS = 8,
    GAPM_ADV_PROP_SCAN_REQ_NTF_EN_BIT = 0x0100,
};

/// Scanning Types
enum gapm_scan_type
{
    /// General discovery
    GAPM_SCAN_TYPE_GEN_DISC = 0,
    /// Limited discovery
    GAPM_SCAN_TYPE_LIM_DISC,
    /// Observer
    GAPM_SCAN_TYPE_OBSERVER,
    /// Selective observer
    GAPM_SCAN_TYPE_SEL_OBSERVER,
    /// Connectable discovery
    GAPM_SCAN_TYPE_CONN_DISC,
    /// Selective connectable discovery
    GAPM_SCAN_TYPE_SEL_CONN_DISC,
};

/// IO Capability Values
enum gap_io_cap
{
    /// Display Only
    GAP_IO_CAP_DISPLAY_ONLY = 0x00,
    /// Display Yes No
    GAP_IO_CAP_DISPLAY_YES_NO,
    /// Keyboard Only
    GAP_IO_CAP_KB_ONLY,
    /// No Input No Output
    GAP_IO_CAP_NO_INPUT_NO_OUTPUT,
    /// Keyboard Display
    GAP_IO_CAP_KB_DISPLAY,
    GAP_IO_CAP_LAST
};

/// OOB Data Present Flag Values
enum gap_oob
{
    /// OOB Data not present
    GAP_OOB_AUTH_DATA_NOT_PRESENT = 0x00,
    /// OOB data present
    GAP_OOB_AUTH_DATA_PRESENT,
    GAP_OOB_AUTH_DATA_LAST
};

#endif  // _GAP_DEFINES_H

