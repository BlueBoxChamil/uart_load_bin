#ifndef __L2CAP_API_H
#define __L2CAP_API_H

#include <stdint.h>

#include "ble_api_error.h"

#define L2CAP_COC_EVT_SPSM_ADDED            0x00    //!< new SPSM is added
#define L2CAP_COC_EVT_SPSM_REMOVED          0x01    //!< an added SPSM is removed
#define L2CAP_COC_EVT_CONNECTED             0x02    //!< Connection Oriented Channel is created.
#define L2CAP_COC_EVT_MTU_CHANGED           0x03    //!< MTU of created channel is changed
#define L2CAP_COC_EVT_DISCONNECTED          0x04    //!< Created channel is disconnected
#define L2CAP_COC_EVT_DATA_SENT             0x05    //!< data is sent to peer device
#define L2CAP_COC_EVT_CONNECT_REQ           0x06    //!< receive connect request from peer device
#define L2CAP_COC_EVT_DATA_RECV             0x07    //!< receive data from peer device

#define L2CAP_FIX_CHANNEL_EVT_DATA_SENT     0x00
#define L2CAP_FIX_CHANNEL_EVT_DATA_RX       0x01

typedef struct _l2cap_coc_evt {
    uint8_t event;
    uint8_t status;
    union {
        /// parameter for L2CAP_COC_EVT_CONNECTED event
        struct {
            uint8_t conidx;         //!< BLE connection index
            uint8_t chan_lid;       //!< channel link ID of created COC, this field will be useld in 
                                    //!< reconfig, disconnect, send data, etc. operation
            uint16_t local_rx_mtu;
            uint16_t peer_rx_mtu;
        } connected;

        /// parameter for L2CAP_COC_EVT_MTU_CHANGED event
        struct {
            uint8_t conidx;
            uint8_t chan_lid;
            uint16_t local_rx_mtu;
            uint16_t peer_rx_mtu;
        } mtu_changed;

        /// parameter for L2CAP_COC_EVT_DISCONNECTED event
        struct {
            uint8_t conidx;
            uint8_t chan_lid;
            uint16_t reason;
        } disconnected;

        /// parameter for L2CAP_COC_EVT_DATA_SENT event
        struct {
            uint8_t conidx;
            uint8_t chan_lid;
        } sent;

        /// parameter for L2CAP_COC_EVT_CONNECT_REQ event
        struct {
            uint16_t token;         //!< used in l2cap_coc_conn_cfm to response connection request
            uint8_t conidx;
            uint8_t nb_chan;
            uint16_t spsm;
            uint16_t peer_rx_mtu;
        } conn_req;

        /// parameter for L2CAP_COC_EVT_DATA_RECV event
        struct {
            uint8_t conidx;
            uint8_t chan_lid;
            uint16_t status;        //!< Error is detected if status is not 0.
            uint16_t length;
            uint8_t *data;
        } data_recv;
    } p;
} l2cap_coc_evt_t;

typedef struct _l2cap_fix_channel_evt {
    uint8_t conidx;
    uint8_t chan_lid;
    uint8_t event;
    union {
        /// parameter for L2CAP_FIX_CHANNEL_EVT_DATA_SENT event
        struct {
            uint8_t status;
        } data_sent;
        /// parameter for L2CAP_FIX_CHANNEL_EVT_DATA_RX event
        struct {
            uint8_t *data;
            uint16_t length;
        } data_rx;
    } p;
} l2cap_fix_channel_evt_t;

typedef void (*l2cap_coc_evt_cb_t)(l2cap_coc_evt_t *evt);
typedef void (*l2cap_fix_channel_evt_cb_t)(l2cap_fix_channel_evt_t *evt);

/*
 * @fn          l2cap_coc_spsm_new
 * 
 * @brief       add new SPSM to l2cap.
 *
 * @param[in]   spsm : new SPSM
 * @param[in]   sec_lvl : reserved, keep this field to 0
 */
void l2cap_coc_spsm_new(uint16_t spsm, uint8_t sec_lvl);

/*
 * @fn          l2cap_coc_spsm_delete
 * 
 * @brief       remove added SPSM from l2cap.
 *
 * @param[in]   spsm : SPSM to be removed
 */
void l2cap_coc_spsm_delete(uint16_t spsm);

/*
 * @fn          l2cap_coc_connect
 * 
 * @brief       remove added SPSM from l2cap.
 *
 * @param[in]   conidx : BLE connection index
 * @param[in]   nb_ch : How many channel to create.
 * @param[in]   spsm : which spsm to connect.
 * @param[in]   rx_mtu : local RX MTU.
 */
void l2cap_coc_connect(uint8_t conidx, uint8_t nb_ch, uint16_t spsm, uint16_t rx_mtu);

/*
 * @fn          l2cap_coc_reconfig
 * 
 * @brief       used to reset local rx mtu.
 *
 * @param[in]   conidx : BLE connection index
 * @param[in]   rx_mtu_new : new local RX MTU
 * @param[in]   nb_ch : how many channel to change MTU
 * @param[in]   chan_lids : link id of channels to change MTU
 */
void l2cap_coc_reconfig(uint8_t conidx, uint16_t rx_mtu_new, uint8_t nb_ch, uint8_t *chan_lids);

/*
 * @fn          l2cap_coc_disconnect
 * 
 * @brief       disconnect created channel.
 *
 * @param[in]   conidx : BLE connection index
 * @param[in]   chan_lid : link id of channel to disconnect
 */
void l2cap_coc_disconnect(uint8_t conidx, uint8_t chan_lid);

/*
 * @fn          l2cap_coc_send_data
 * 
 * @brief       used to send data to peer device.
 *
 * @param[in]   conidx : BLE connection index
 * @param[in]   chan_lid : link id of channel to send data
 * @param[in]   len : how many data to send, should not be more than peer_rx_mtu
 * @param[in]   data : storage pointer of sending data
 */
void l2cap_coc_send_data(uint8_t conidx, uint8_t chan_lid, uint16_t len, uint8_t *data);

/*
 * @fn          l2cap_coc_conn_cfm
 * 
 * @brief       used to confirm connect request from peer device.
 *
 * @param[in]   token : used to identify connect request by internal implementation of l2cap, this filed should be
 *                      same with "token" field in L2CAP_COC_EVT_CONNECT_REQ event.
 * @param[in]   conidx : BLE connection index
 * @param[in]   nb_chan : how many channels to create
 * @param[in]   local_rx_mtu : local RX MTU
 */
void l2cap_coc_conn_cfm(uint16_t token, uint8_t conidx, uint8_t nb_chan, uint16_t local_rx_mtu);

/*
 * @fn          l2cap_coc_init
 * 
 * @brief       used to set callback function used for dealing l2cap COC events.
 *
 * @param[in]   cb : ballback function to handle events
 */
void l2cap_coc_init(l2cap_coc_evt_cb_t cb);

/*
 * @fn          l2cap_fix_channel_register
 * 
 * @brief       User should call this funcion to resiger a l2cap fix channel after connection is established. 
 *              User could transfer data direct through this channel rather than through ATT. The registered 
 *              channel will be removed automatically after connection is disconnected.
 *
 * @param[in]   conidx : connection index
 * @param[in]   mtu : MTU of this channel
 * @param[in]   chan_lid: channel link ID. This variable will be assigned a value afet successful registration.
 *
 * @return      @ref ble_api_err_t
 */
ble_api_err_t l2cap_fix_channel_register(uint8_t conidx, uint16_t mtu, uint8_t *chan_lid);

/*
 * @fn          l2cap_fix_channel_unregister
 * 
 * @brief       The registered channel will be removed automatically after connection is disconnected. User 
 *              also unregister this channel manually after this channel is useless. 
 *
 * @param[in]   conidx : connection index
 * @param[in]   chan_lid : channel link ID
 *
 * @return      @ref ble_api_err_t
 */
ble_api_err_t l2cap_fix_channel_unregister(uint8_t conidx, uint8_t chan_lid);

/*
 * @fn          l2cap_fix_channel_send
 * 
 * @brief       Used to send data with registered l2cap fix channel
 *
 * @param[in]   conidx : connection index
 * @param[in]   chan_lid : channel link ID
 * @param[in]   data : data pointer
 * @param[in]   length : data length
 *
 * @return      @ref ble_api_err_t
 */
ble_api_err_t l2cap_fix_channel_send(uint8_t conidx, uint8_t chan_lid, uint8_t *data, uint16_t length);

/*
 * @fn          l2cap_fix_channel_init
 * 
 * @brief       Used to setup l2cap fix channel callback function, user should call this function duaring
 *              BLE setup procedure.
 *
 * @param[in]   cb : l2cap fix channel callback function, @ref l2cap_fix_channel_evt_cb_t
 */
void l2cap_fix_channel_init(l2cap_fix_channel_evt_cb_t cb);

#endif  // __L2CAP_API_H

