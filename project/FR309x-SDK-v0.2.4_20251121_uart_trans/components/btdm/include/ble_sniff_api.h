#ifndef __BLE_SNIFF_API_H
#define __BLE_SNIFF_API_H

#include <stdio.h>
#include <stdint.h>

/*user for 306x*/

#define HCI_CMD_VENDOR_SNIFF                    0x10

#define SNIFF_CMD_INIT                          0x01
#define SNIFF_CMD_UPDATE_PARAM                  0x02
#define SNIFF_CMD_GET_RSSI                      0x03
#define SNIFF_CMD_STOP                          0x04
#define SNIFF_CMD_GET_RATE                      0x05
#define SNIFF_CMD_SET_PERIOD_OF_RATE            0x06
#define SNIFF_CMD_RESTART                       0x07
#define SNIFF_CMD_SET_TIMEOUT                   0x08
#define SNIFF_CMD_MAX                           0x0A

enum ble_sniff_event {
    BLE_SNIFF_START,            //!< Sniffing started
    BLE_SNIFF_RSSI_REPORT,      //!< Proactively reporting RSSI
    BLE_SNIFF_RATE_REPORT,
    BLE_SNIFF_GET_RSSI_IND,     //!< Callback event of ble_sniff_get_rssi()
    BLE_SNIFF_GET_RATE_IND,     //!< Sniffing rate reporting

    BLE_SNIFF_LINK_LOST = 10,   //!< Sniffing failed due to not receiving the packet for a long time,
                                //!< Can call ble_sniff_restart() to restart sniffing of ble_sniff_stop() to stop.
    BLE_SNIFF_END,              //!< Sniffing stopped
    BLE_SNIFF_CMD_CMP,          
};

struct ble_sniff_event_t {
    uint8_t conidx;
    enum ble_sniff_event evt_type;
    union {
        struct {
            int8_t rssi;
        } get_rssi_ind;

        struct {
            int8_t rssi;
        } rssi_report;

        struct {
            uint16_t sch_failed_rate;
            uint16_t success_rate;
        } rate;

        struct {
            uint8_t code;
        } cmd_cmp;
    } param;
};

typedef struct sniff_param {
    uint8_t     conidx;             /// Connect index
    uint8_t     csa_sel;            /// frequency hopping algorithm(0: CSA #1, 1: CSA #2) 
    uint8_t     hop_increase;       /// hop increase
    uint8_t     tx_phy;             /// PHY (0: 1Mbps | 1: 2 Mbps)
    uint8_t     rx_phy;             /// PHY (0: 1Mbps | 1: 2 Mbps)
    uint8_t     ch_map[5];
    uint8_t     peer_role;          /// 0: master, 1: slave
    uint8_t     last_unmapped_ch;   /// Used for CSA #1
    uint16_t    latency;
    uint16_t    con_interval;       /// Connect interval(in unit of 1.25ms)
    uint32_t    access_code;        /// 
    uint32_t    crc;                /// Low 3 bytes vaild
    uint16_t    master_sca;         /// Master Sleep clock accuracy (in ppm)
    uint16_t    event_counter;      /// used for CSA #2
} sniff_param_t;

typedef void (*ble_sniff_callback_func_t)(struct ble_sniff_event_t *event);
extern ble_sniff_callback_func_t ble_sniff_evt_callback;
void ble_sniff_set_callback(ble_sniff_callback_func_t callback);

/*******************************************************
 * @fn          ble_sniff_set_period_of_rate
 * 
 * @brief       Set the statistical interval for the rate of successful sniffing.
 *
 * @param[in]   conidx         : connect index.
 * @param[in]   period         : The period of success rate statistics.
 * 
 * @return      None
 */
void ble_sniff_set_period_of_rate(uint16_t period);

/*******************************************************
 * @fn          ble_sniff_get_rate
 * 
 * @brief       Get the success rate and schdule failed rate of sniffing. 
 *              This value will be updated at intervals set by ble_sniff_set_period_of_rate(), the default value is 100.
 *              For example, if the return value is 955, the success rate is 95.5%.
 *
 * @param[in]   conidx      : connect index.
 * 
 * @return      success rate
 */
void ble_sniff_get_rate(uint8_t conidx);

/*******************************************************
 * @fn          ble_sniff_get_rssi
 * 
 * @brief       Get the Last sniffed RSSI.
 *
 * @param[in]   conidx      : connect index.
 * @param[in]   real_rssi   : pointer to real rssi.
 * @param[in]   filter_rssi : pointer to filter rssi, can be set to NULL.
 * 
 * @return      None
 */
void ble_sniff_get_rssi(uint8_t conidx);

/*******************************************************
 * @fn          ble_sniff_set_timeout
 * 
 * @brief       Set the maximum timeout for the sniffing.
 *              For example, if the link cannot sniffing to the device in timeout connection event windows, 
 *                  event BLE_SNIFF_LINK_LOST will be reported.
 *              Can only be called in BLE_SNIFF_START event
 *
 * @param[in]   conidx   : connect index.
 * @param[in]   timeout  : maximum timeout, Num of connect interval.
 *                                        
 *                                        
 * 
 * @return      None
 */
void ble_sniff_set_timeout(uint8_t conidx, uint16_t timeout);

/*******************************************************
 * @fn          ble_sniff_update_info
 * 
 * @brief       Update sniff information for a link.
 *              If this is the first update, the link will start sniffing and teh BLE_SNIFF_START event will be reported.
 *
 * @param[in]   param : BLE param of link.
 * 
 * @return      None
 */
void ble_sniff_update_info(struct sniff_param *param);

/*******************************************************
 * @fn          ble_sniff_stop
 * 
 * @brief       Stop sniffing.
 *
 * @param[in]   conidx      : connect index.
 * 
 * @return      None
 */
void ble_sniff_stop(uint8_t conidx);

/*******************************************************
 * @fn          ble_sniff_init
 * 
 * @brief       Restart sniffing of a link.
 *              If no packet is received within 38 connection intervals, the BLE_SNIFF_LINK_LOST event will be reported.
 *
 * @param[in]   conidx : connect index.
 * 
 * @return      None
 */
void ble_sniff_restart(uint8_t conidx);

/*******************************************************
 * @fn          ble_sniff_init
 * 
 * @brief       Initialize the parameters required for sniff.
 *
 * @param[in]   enable : 0: deinit sniff mode, 1: init sniff mode.
 * 
 * @return      None
 */
void ble_sniff_init(uint8_t enable);

#endif
