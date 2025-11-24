#ifndef __HCI_VENDOR_H__
#define __HCI_VENDOR_H__

#include <stdint.h>
#include <stdbool.h>

#include "cmsis_compiler.h"

typedef __PACKED_STRUCT {
    uint32_t address;
    uint32_t data;
} hci_vendor_cmd_apb_reg_write_t;

typedef __PACKED_STRUCT {
    uint32_t address;
} hci_vendor_cmd_apb_reg_read_t;

typedef __PACKED_STRUCT {
    uint8_t address;
    uint8_t data;
} hci_vendor_cmd_pmu_reg_write_t;

typedef __PACKED_STRUCT {
    uint8_t address;
} hci_vendor_cmd_pmu_reg_read_t;

typedef __PACKED_STRUCT {
    uint8_t settings;
    uint8_t rsv;
} hci_vendor_cmd_osc_cap_set_t;

typedef __PACKED_STRUCT {
    uint8_t port_sel;       // 0: PORTA; 1: PORTB
    uint16_t bits_sel;
    uint16_t int_en;
    uint16_t output_en;
    uint16_t pull_en;
    uint16_t pull_sel;
    uint16_t output_value;
} hci_vendor_cmd_pmu_port_cfg_t;

typedef __PACKED_STRUCT {
    uint8_t working_mode;   // 0: slave; 1: master
} hci_vendor_cmd_24G_enable_t;

typedef __PACKED_STRUCT {
    uint16_t start_freq;   // from 2372 to 2432
} hci_vendor_cmd_start_freq_t;

typedef __PACKED_STRUCT {
    uint8_t enable;   // 0: disable; 1: enable
} hci_vendor_cmd_fast_adv_cfg_t;

typedef __PACKED_STRUCT {
    uint8_t poll_interval;
} hci_vendor_cmd_bt_poll_inteval_set_t;

typedef __PACKED_STRUCT {
    uint8_t channel;
    uint8_t payload_type;
    uint8_t tx_power;
} hci_vendor_cmd_ble_rf_tx_param_t;


typedef __PACKED_STRUCT {
    uint8_t channel;
    uint8_t payload_type;
    uint8_t tx_power;
	uint8_t data_len;
} hci_vendor_cmd_ble_rf_tx_param_add_len_t;

typedef __PACKED_STRUCT {
    uint8_t channel;
} hci_vendor_cmd_ble_rf_rx_param_t;

typedef __PACKED_STRUCT {
    uint8_t channel;
    uint8_t payload_type;
    uint8_t packet_type;
    uint8_t tx_power;
} hci_vendor_cmd_bt_rf_tx_param_t;

typedef __PACKED_STRUCT {
    uint8_t channel;
    uint8_t packet_type;//0x00
    uint8_t type;//0x01
} hci_vendor_cmd_bt_rf_rx_param_t;

typedef __PACKED_STRUCT {
    uint8_t cmd;
    __PACKED_UNION {
        __PACKED_STRUCT {
            uint8_t addr;
            uint8_t value;
        } pmu_read;
        __PACKED_STRUCT {
            uint32_t addr;
            uint32_t value;
        } apb_read;
        __PACKED_STRUCT {
            uint32_t max_usage;
            uint32_t curr_avail;
            uint32_t em_end;
        } heap_usage;
        __PACKED_STRUCT {
            uint32_t curr_time;
        } curr_time_get;
        __PACKED_STRUCT {
            uint8_t osc_cap;
        } osc_cap_get;
    } p;
} hci_vendor_cmd_cmp_evt_t;

/* used to access regs of controller */
void hci_vendor_apb_reg_write(uint32_t address, uint32_t data);
void hci_vendor_apb_reg_read(uint32_t address);
void hci_vendor_pmu_reg_write(uint8_t address, uint8_t data);
void hci_vendor_pmu_reg_read(uint8_t address);

/* used to fetch heap usage of controller */
void hci_vendor_controller_heap_usage(void);

/* 
 * get how many milliseconds have passed after controller start-up,
 * and the value will loop back to 0 after reaching 83886079(0x4FFFFFF)
 */
void hci_vendor_controller_curr_time(void);

/* used to get and set load capacitance of crystal */
void hci_vendor_osc_cap_set(uint8_t setting);
void hci_vendor_osc_cap_get(void);

/* 
 * all the unused GPIO of controller side is in the charge of PMU, 
 * this function is used to configure the state of these GPIO
 */
void hci_vendor_pmu_port_cfg(uint8_t port_sel, uint16_t bits_sel, 
                                uint16_t int_en, uint16_t output_en, 
                                uint16_t pull_en, uint16_t pull_sel, 
                                uint16_t output_value);

/* used to remove 7.5ms connection interval restriction */
void hci_vendor_24G_enable(uint8_t working_mode);

/* used to set the start frequency of operation channel */
void hci_vendor_start_freq_set(uint16_t start_freq);

/* used to remove 20ms adv interval restriction */
void hci_vendor_fast_adv_cfg(bool enable);

/* 
 * Before the system enters shutdown, it must call this function to 
 * notify the controller to get ready. Once the MCU receives the 
 * controller��s response to this command, it can carry out the final 
 * shutdown sequence.
 */
void hci_vendor_power_off(void);

/* used to set poll interval when BT is working in master mode. unit: 625us */
void hci_vendor_bt_poll_interval_set(uint8_t poll_interval);

/*********************************************************************
 * @brief   ble tx test command. payload length is fixed to 37 bytes
 *
 * @param   channel : 0~39. Physical channel is 2402+channel*2 MHz
 * @param   payload_type : 0x00-PRBS9, 0x01-11110000, 0x02-10101010
 * @param   power : 0x00(-12dBm), 0x02(-8dBm), 0x03(-4dBm), 0x06(0dBm), 0x0a(4dBm), 0x0f(8dBm) 
 * @param   len: data length, only valid in 1m_mw_burst and 2m_mw_burst mode
 */
void hci_vendor_tx_1m_mw_cont(uint8_t channel, uint8_t payload_type, uint8_t power);
void hci_vendor_tx_1m_cw_burst(uint8_t channel, uint8_t payload_type, uint8_t power);
void hci_vendor_tx_2m_mw_cont(uint8_t channel, uint8_t payload_type, uint8_t power);
void hci_vendor_tx_2m_cw_cont(uint8_t channel, uint8_t payload_type, uint8_t power);
void hci_vendor_tx_1m_mw_burst(uint8_t channel, uint8_t payload_type, uint8_t power, uint8_t len);
void hci_vendor_tx_2m_mw_burst(uint8_t channel, uint8_t payload_type, uint8_t power, uint8_t len);
void hci_vendor_tx_ble_disable(void);

/*********************************************************************
 * @brief   ble rx test command. Number of received packet will be return 
 *          in command complete event of disble command.
 *
 * @param   channel : 0~39. Physical channel is 2402+channel*2 MHz
 */
void hci_vendor_rx_ble_1m(uint8_t channel);
void hci_vendor_rx_ble_2m(uint8_t channel);
void hci_vendor_rx_ble_disable(void);

/*********************************************************************
 * @brief   bt tx test command.
 *
 * @param   channel : 2402+channel*2 MHz for 0~39, 2403+(channel-40)*2 MHz for 40~78, 
 * @param   payload_type : 0x00-PRBS9, 0x01-11110000, 0x02-10101010
 * @param   packet_type : 0x00-DM1, 0x01-DH1, 0x02-DM3, 0x03-DH3, 0x04-DM5, 0x05-DH5
 *                        0x81-DH1_2, 0x82-DH1_3, 0x83-DH3_2, 0x84-DH3_3, 0x85-DH5_2, 0x86-DH5_3
 * @param   power : 0x00(-12dBm), 0x02(-8dBm), 0x03(-4dBm), 0x06(0dBm), 0x0a(4dBm), 0x0f(8dBm) 
 */
void hci_vendor_tx_bt(uint8_t channel, uint8_t payload_type, uint8_t packet_type, uint8_t power);
void hci_vendor_tx_bt_disable(void);

/*********************************************************************
 * @brief   bt rx test command.
 *
 * @param   channel : 2402+channel*2 MHz for 0~39, 2403+(channel-40)*2 MHz for 40~78,
 * @param   packet_type : 0x00-DM1, 0x01-DH1, 0x02-DM3, 0x03-DH3, 0x04-DM5, 0x05-DH5
 *                        0x81-DH1_2, 0x82-DH1_3, 0x83-DH3_2, 0x84-DH3_3, 0x85-DH5_2, 0x86-DH5_3
 * @param   type : 0x00-whiten disabled, 0x01-whiten enabled
 */
void hci_vendor_rx_bt(uint8_t channel, uint8_t packet_type, uint8_t type);
void hci_vendor_rx_bt_disable(void);

#endif  // __HCI_VENDOR_H__
