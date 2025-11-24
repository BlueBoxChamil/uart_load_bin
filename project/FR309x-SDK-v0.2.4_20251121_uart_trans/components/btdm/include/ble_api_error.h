/*
 ****************************************************************************************
 *
 * @file       app_error.h
 * @version    v1.0
 * @brief      error code of app layer
 * @attention
 *  Copyright (C) 2023 Freqchip.   \n
 *  All rights reserved.           \n
 * 
 * 
 ****************************************************************************************
 */
#ifndef __APP_ERROR_H_
#define __APP_ERROR_H_

typedef enum ble_api_error ble_api_err_t;

enum ble_api_error
{
    /// no error
    BLE_API_ERR_NO_ERR                                  = 0x00,
    /// the input parameter is incorrect
    BLE_API_ERR_INVAILD_PARAM                           = 0x01,
    /// the specified conidx has no device connection
    BLE_API_ERR_DEVICE_NOT_CONNECTED                    = 0x02,
    /// unknown error
    BLE_API_ERR_UNKNOWN                                 = 0x03,
    /// no available 
    BLE_API_ERR_NO_AVAILABLE_RESOURCE                   = 0x04,

    // ----------------------------------------------------
    // ----------------- GATT API Error -------------------
    // ----------------------------------------------------
    /// invaild client_id or service_id
    BLE_API_ERR_INVAILD_ID                              = 0x10,
    /// GAP and GATT service is deleted
    BLE_API_ERR_SVC_IS_DELETED                          = 0x11,
    /// client handles group is empty
    BLE_API_ERR_CLIENT_HDL_IS_EMPTY                     = 0x12,

    // ----------------------------------------------------
    // ----------------- GAP API Error --------------------
    // ----------------------------------------------------
    ///has not been paired with the target device
    BLE_API_ERR_NOT_PAIRED                              = 0x30,
    ///activity state error
    BLE_ACT_STATE_IS_ERR                                = 0x31,
    ///Bond information has been lost
    BLE_API_ERR_BOND_INFO_LOST                          = 0x32,
};

#endif
