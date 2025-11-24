#include <stdio.h>

#include "fr30xx.h"

#include "btdm_host.h"
#include "hci_vendor.h"

#define HCI_VENDOR_CMD_TYPE_PMU_REG_READ        0x00
#define HCI_VENDOR_CMD_TYPE_PMU_REG_WRITE       0x01
#define HCI_VENDOR_CMD_TYPE_APB_REG_READ        0x02
#define HCI_VENDOR_CMD_TYPE_APB_REG_WRITE       0x03
#define HCI_VENDOR_CMD_TYPE_HEAP_USAGE          0x11
#define HCI_VENDOR_CMD_TYPE_RF_1MTX_MW_CONT     0x17
#define HCI_VENDOR_CMD_TYPE_RF_1MTX_CW_BURST    0x18
#define HCI_VENDOR_CMD_TYPE_RF_TX_DISABLE       0x19
#define HCI_VENDOR_CMD_TYPE_RF_2MTX_MW_CONT     0x1b
#define HCI_VENDOR_CMD_TYPE_RF_2MTX_CW_CONT     0x1c
#define HCI_VENDOR_CMD_TYPE_RF_1MRX_ENABLE      0x1f
#define HCI_VENDOR_CMD_TYPE_RF_1MTX_MW_BURST    0x21
#define HCI_VENDOR_CMD_TYPE_RF_2MTX_MW_BURST    0x22
#define HCI_VENDOR_CMD_TYPE_RF_RX_DISABLE       0x23
#define HCI_VENDOR_CMD_TYPE_RF_2MRX_ENABLE      0x24
#define HCI_VENDOR_CMD_TYPE_RF_BT_TX_ENABLE     0x25
#define HCI_VENDOR_CMD_TYPE_RF_BT_TX_DISABLE    0x26
#define HCI_VENDOR_CMD_TYPE_RF_BT_RX_ENABLE     0x2f
#define HCI_VENDOR_CMD_TYPE_RF_BT_RX_DISABLE    0x30
#define HCI_VENDOR_CMD_TYPE_CURR_TIME           0x28
#define HCI_VENDOR_CMD_TYPE_OSC_CAP_GET         0x29
#define HCI_VENDOR_CMD_TYPE_OSC_CAP_SET         0x2a
#define HCI_VENDOR_CMD_TYPE_PMU_PORT_CFG        0x32
#define HCI_VENDOR_CMD_TYPE_24G_ENABLE          0x33
#define HCI_VENDOR_CMD_TYPE_START_FREQ_SET      0x34
#define HCI_VENDOR_CMD_TYPE_FAST_ADV_CFG        0x35
#define HCI_VENDOR_CMD_TYPE_POWER_OFF           0x36
#define HCI_VENDOR_CMD_TYPE_BT_POLL_INTV_SET    0x37

void hci_vendor_apb_reg_write(uint32_t address, uint32_t data)
{
    hci_vendor_cmd_apb_reg_write_t param;

    param.address = address;
    param.data = data;
    btdm_host_send_vendor_cmd(HCI_VENDOR_CMD_TYPE_APB_REG_WRITE, sizeof(param), (void *)&param);
}

void hci_vendor_apb_reg_read(uint32_t address)
{
    hci_vendor_cmd_apb_reg_read_t param;

    param.address = address;
    btdm_host_send_vendor_cmd(HCI_VENDOR_CMD_TYPE_APB_REG_READ, sizeof(param), (void *)&param);
}

void hci_vendor_pmu_reg_write(uint8_t address, uint8_t data)
{
    hci_vendor_cmd_pmu_reg_write_t param;

    param.address = address;
    param.data = data;
    btdm_host_send_vendor_cmd(HCI_VENDOR_CMD_TYPE_PMU_REG_WRITE, sizeof(param), (void *)&param);
}

void hci_vendor_pmu_reg_read(uint8_t address)
{
    hci_vendor_cmd_pmu_reg_read_t param;

    param.address = address;
    btdm_host_send_vendor_cmd(HCI_VENDOR_CMD_TYPE_PMU_REG_READ, sizeof(param), (void *)&param);
}

void hci_vendor_controller_heap_usage(void)
{
    btdm_host_send_vendor_cmd(HCI_VENDOR_CMD_TYPE_HEAP_USAGE, 0, NULL);
}

void hci_vendor_controller_curr_time(void)
{
    btdm_host_send_vendor_cmd(HCI_VENDOR_CMD_TYPE_CURR_TIME, 0, NULL);
}

void hci_vendor_osc_cap_set(uint8_t setting)
{
    hci_vendor_cmd_osc_cap_set_t param;

    param.settings = setting;
    btdm_host_send_vendor_cmd(HCI_VENDOR_CMD_TYPE_OSC_CAP_SET, sizeof(param), (void *)&param);
}

void hci_vendor_osc_cap_get(void)
{
    btdm_host_send_vendor_cmd(HCI_VENDOR_CMD_TYPE_OSC_CAP_GET, 0, NULL);
}

void hci_vendor_pmu_port_cfg(uint8_t port_sel, uint16_t bits_sel, 
                                uint16_t int_en, uint16_t output_en, 
                                uint16_t pull_en, uint16_t pull_sel, 
                                uint16_t output_value)
{
    hci_vendor_cmd_pmu_port_cfg_t param;

    param.port_sel = port_sel;
    param.bits_sel = bits_sel;
    param.int_en = int_en;
    param.output_en = output_en;
    param.pull_en = pull_en;
    param.pull_sel = pull_sel;
    param.output_value = output_value;
    btdm_host_send_vendor_cmd(HCI_VENDOR_CMD_TYPE_PMU_PORT_CFG, sizeof(param), (void *)&param);
}

void hci_vendor_24G_enable(uint8_t working_mode)
{
    hci_vendor_cmd_24G_enable_t param;

    param.working_mode = working_mode;
    btdm_host_send_vendor_cmd(HCI_VENDOR_CMD_TYPE_24G_ENABLE, sizeof(param), (void *)&param);
}

void hci_vendor_start_freq_set(uint16_t start_freq)
{
    hci_vendor_cmd_start_freq_t param;

    param.start_freq = start_freq;
    btdm_host_send_vendor_cmd(HCI_VENDOR_CMD_TYPE_START_FREQ_SET, sizeof(param), (void *)&param);
}

void hci_vendor_fast_adv_cfg(bool enable)
{
    hci_vendor_cmd_fast_adv_cfg_t param;
    param.enable = enable;
    btdm_host_send_vendor_cmd(HCI_VENDOR_CMD_TYPE_FAST_ADV_CFG, sizeof(param), (void *)&param);
}

void hci_vendor_power_off(void)
{
    btdm_host_send_vendor_cmd(HCI_VENDOR_CMD_TYPE_POWER_OFF, 0, NULL);
}

void hci_vendor_bt_poll_interval_set(uint8_t poll_interval)
{
    hci_vendor_cmd_bt_poll_inteval_set_t param;
    param.poll_interval = poll_interval;
    btdm_host_send_vendor_cmd(HCI_VENDOR_CMD_TYPE_BT_POLL_INTV_SET, sizeof(param), (void *)&param);
}

void hci_vendor_tx_1m_mw_cont(uint8_t channel, uint8_t payload_type, uint8_t power)
{
    hci_vendor_cmd_ble_rf_tx_param_t param;

    param.channel = channel;
    param.payload_type = payload_type;
    param.tx_power = power;
    btdm_host_send_vendor_cmd(HCI_VENDOR_CMD_TYPE_RF_1MTX_MW_CONT, sizeof(param), (void *)&param);
}

void hci_vendor_tx_1m_cw_burst(uint8_t channel, uint8_t payload_type, uint8_t power)
{
    hci_vendor_cmd_ble_rf_tx_param_t param;

    param.channel = channel;
    param.payload_type = payload_type;
    param.tx_power = power;
    btdm_host_send_vendor_cmd(HCI_VENDOR_CMD_TYPE_RF_1MTX_CW_BURST, sizeof(param), (void *)&param);
}

void hci_vendor_tx_2m_mw_cont(uint8_t channel, uint8_t payload_type, uint8_t power)
{
    hci_vendor_cmd_ble_rf_tx_param_t param;

    param.channel = channel;
    param.payload_type = payload_type;
    param.tx_power = power;
    btdm_host_send_vendor_cmd(HCI_VENDOR_CMD_TYPE_RF_2MTX_MW_CONT, sizeof(param), (void *)&param);
}

void hci_vendor_tx_2m_cw_cont(uint8_t channel, uint8_t payload_type, uint8_t power)
{
    hci_vendor_cmd_ble_rf_tx_param_t param;

    param.channel = channel;
    param.payload_type = payload_type;
    param.tx_power = power;
    btdm_host_send_vendor_cmd(HCI_VENDOR_CMD_TYPE_RF_2MTX_CW_CONT, sizeof(param), (void *)&param);
}

void hci_vendor_tx_1m_mw_burst(uint8_t channel, uint8_t payload_type, uint8_t power, uint8_t len)
{
    hci_vendor_cmd_ble_rf_tx_param_add_len_t param;

    param.channel = channel;
    param.payload_type = payload_type;
    param.tx_power = power;
	param.data_len = len;
    btdm_host_send_vendor_cmd(HCI_VENDOR_CMD_TYPE_RF_1MTX_MW_BURST, sizeof(param), (void *)&param);
}

void hci_vendor_tx_2m_mw_burst(uint8_t channel, uint8_t payload_type, uint8_t power, uint8_t len)
{
    hci_vendor_cmd_ble_rf_tx_param_add_len_t param;

    param.channel = channel;
    param.payload_type = payload_type;
    param.tx_power = power;
	param.data_len = len;
    btdm_host_send_vendor_cmd(HCI_VENDOR_CMD_TYPE_RF_2MTX_MW_BURST, sizeof(param), (void *)&param);
}

void hci_vendor_tx_ble_disable(void)
{
    btdm_host_send_vendor_cmd(HCI_VENDOR_CMD_TYPE_RF_TX_DISABLE, 0, NULL);
}

void hci_vendor_rx_ble_1m(uint8_t channel)
{
    hci_vendor_cmd_ble_rf_rx_param_t param;

    param.channel = channel;
    btdm_host_send_vendor_cmd(HCI_VENDOR_CMD_TYPE_RF_1MRX_ENABLE, sizeof(param), (void *)&param);
}

void hci_vendor_rx_ble_2m(uint8_t channel)
{
    hci_vendor_cmd_ble_rf_rx_param_t param;

    param.channel = channel;
    btdm_host_send_vendor_cmd(HCI_VENDOR_CMD_TYPE_RF_2MRX_ENABLE, sizeof(param), (void *)&param);
}

void hci_vendor_rx_ble_disable(void)
{
    btdm_host_send_vendor_cmd(HCI_VENDOR_CMD_TYPE_RF_RX_DISABLE, 0, NULL);
}

void hci_vendor_tx_bt(uint8_t channel, uint8_t payload_type, uint8_t packet_type, uint8_t power)
{
    hci_vendor_cmd_bt_rf_tx_param_t param;

    param.channel = channel;
    param.payload_type = payload_type;
    param.packet_type = packet_type;
    param.tx_power = power;
    btdm_host_send_vendor_cmd(HCI_VENDOR_CMD_TYPE_RF_BT_TX_ENABLE, sizeof(param), (void *)&param);
}

void hci_vendor_tx_bt_disable(void)
{
    btdm_host_send_vendor_cmd(HCI_VENDOR_CMD_TYPE_RF_BT_TX_DISABLE, 0, NULL);
}

void hci_vendor_rx_bt(uint8_t channel, uint8_t packet_type, uint8_t type)
{
    hci_vendor_cmd_bt_rf_rx_param_t param;

    param.channel = channel;
    param.packet_type = packet_type;
    param.type = type;
    btdm_host_send_vendor_cmd(HCI_VENDOR_CMD_TYPE_RF_BT_RX_ENABLE, sizeof(param), (void *)&param);
}

void hci_vendor_rx_bt_disable(void)
{
    btdm_host_send_vendor_cmd(HCI_VENDOR_CMD_TYPE_RF_BT_RX_DISABLE, 0, NULL);
}


void btdm_host_vendor_cmd_cmp_evt(uint8_t status, uint8_t len, uint8_t const *__param)
{    
    hci_vendor_cmd_cmp_evt_t *param = (void *)__param;
    
    if (status != 0x00) {
        printf("btdm_host_vendor_cmd_cmp_evt: ERROR, cmd-0x%02x\r\n", param->cmd);
        return;
    }
    
    switch(param->cmd) {
        case HCI_VENDOR_CMD_TYPE_PMU_REG_READ:
            printf("CONTROLLER: pmu_addr-0x%02x, value-0x%02x.\r\n", param->p.pmu_read.addr, param->p.pmu_read.value);
            break;
        case HCI_VENDOR_CMD_TYPE_APB_REG_READ:
            printf("CONTROLLER: addr-0x%08x, value-0x%08x.\r\n", param->p.apb_read.addr, param->p.apb_read.value);
            break;
        case HCI_VENDOR_CMD_TYPE_HEAP_USAGE:
            printf("CONTROLLER HEAP: max_usage-0x%08x, curr_avail-0x%08x, em_end-0x%04x.\r\n", 
                            param->p.heap_usage.max_usage, param->p.heap_usage.curr_avail, param->p.heap_usage.em_end);
            break;
        case HCI_VENDOR_CMD_TYPE_CURR_TIME:
            printf("CONTROLLER CURRENT TIME: %d.\r\n", param->p.curr_time_get.curr_time);
            break;
        case HCI_VENDOR_CMD_TYPE_OSC_CAP_GET:
            printf("CONTROLLER OSC CAP: 0x%02x.\r\n", param->p.osc_cap_get.osc_cap);
            break;
        case HCI_VENDOR_CMD_TYPE_POWER_OFF:
            system_power_off_post_process();
            break;
        case HCI_VENDOR_CMD_TYPE_RF_1MTX_MW_CONT:           
        case HCI_VENDOR_CMD_TYPE_RF_1MTX_CW_BURST:       
        case HCI_VENDOR_CMD_TYPE_RF_TX_DISABLE:        
        case HCI_VENDOR_CMD_TYPE_RF_2MTX_MW_CONT:          
        case HCI_VENDOR_CMD_TYPE_RF_2MTX_CW_CONT:          
        case HCI_VENDOR_CMD_TYPE_RF_1MRX_ENABLE:         
        case HCI_VENDOR_CMD_TYPE_RF_1MTX_MW_BURST:        
        case HCI_VENDOR_CMD_TYPE_RF_2MTX_MW_BURST:        
        case HCI_VENDOR_CMD_TYPE_RF_2MRX_ENABLE:        
        case HCI_VENDOR_CMD_TYPE_RF_BT_TX_ENABLE:         
        case HCI_VENDOR_CMD_TYPE_RF_BT_TX_DISABLE:        
        case HCI_VENDOR_CMD_TYPE_RF_BT_RX_ENABLE:
            printf("cmd-0x%02x excuted success\r\n",param->cmd);
            break;
        case HCI_VENDOR_CMD_TYPE_RF_RX_DISABLE:
            printf("ble rx : %02x %02x %02x %02x\r\n",__param[1],__param[2],__param[3],__param[4]);
            break;
        case HCI_VENDOR_CMD_TYPE_RF_BT_RX_DISABLE:
            printf("bt rx : %02x %02x %02x %02x\r\n",__param[1],__param[2],__param[3],__param[4]);
            break;
        default:
            printf("status: 0x%02x.\r\n", status);
            for (uint32_t i=0; i<len; i++) {
                printf("%02x ", __param[i]);
            }
            printf("\r\n");
            break;
    }
}

void btdm_host_recv_vendor_evt(uint8_t len, uint8_t *param)
{
    printf("EVENT: ");
    for (uint32_t i=0; i<len; i++) {
        printf("%02x ", param[i]);
    }
    printf("\r\n");
}
