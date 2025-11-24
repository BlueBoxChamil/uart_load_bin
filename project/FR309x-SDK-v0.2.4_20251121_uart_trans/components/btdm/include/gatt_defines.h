#ifndef _GATT_DEFINES_H
#define _GATT_DEFINES_H

/// Length of an handle
#define GATT_HANDLE_LEN      (2)
/// Length of 16-bit UUID in octets
#define GATT_UUID_16_LEN     (2)
/// Length of 32-bit UUID in octets
#define GATT_UUID_32_LEN     (4)
/// Length of 128-bit UUID in octets
#define GATT_UUID_128_LEN    (16)
/// Length of Database Hash in octets
#define GATT_DB_HASH_LEN     (16)

///  GATT UUID Type
enum gatt_uuid_type
{
    /// 16-bit UUID value
    GATT_UUID_16      = 0x00,
    /// 32-bit UUID value
    GATT_UUID_32      = 0x01,
    /// 128-bit UUID value
    GATT_UUID_128     = 0x02,
    /// Invalid UUID Type
    GATT_UUID_INVALID = 0x03,
};

/// GATT attribute security level
enum gatt_sec_lvl
{
    /// Attribute value is accessible on an unencrypted link.
    GATT_SEC_NOT_ENC    = 0x00,
    /// Attribute value is accessible on an encrypted link or modified with using write signed procedure
    /// on unencrypted link if bonded using an unauthenticated pairing.
    GATT_SEC_NO_AUTH    = 0x01,
    /// Attribute value is accessible on an encrypted link or modified with using write signed procedure
    /// on unencrypted link if bonded using an authenticated pairing.
    GATT_SEC_AUTH       = 0x02,
    /// Attribute value is accessible on an encrypted link or modified with using write signed procedure
    /// on unencrypted link if bonded using a secure connection pairing.
    GATT_SEC_SECURE_CON = 0x03,
};

#endif  // _GATT_DEFINES_H

