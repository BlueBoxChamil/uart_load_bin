#include "fr30xx.h"

#include "FreeRTOS.h"

#include "SWD.h"
#include "fdb_app.h"

#include "controller.h"

#define CONTROLLER_CODE_SPLIT

#define HCI_UART                UART0
#define HCI_UART_IRQn           UART0_IRQn

///default 921600, set uart ref clk to pll if baudrate set to 3M
#define HCI_UART_BAUDRATE       921600  //3000000

#define CONTROLLER_PARAM_DUMMY          0x00
#define CONTROLLER_PARAM_BAUDRATE       0x01
#define CONTROLLER_PARAM_KEY            0x02
#define CONTROLLER_PARAM_BT_ADDR        0x03
#define CONTROLLER_PARAM_BLE_ADDR       0x04
#define CONTROLLER_PARAM_TX_POWER       0x05
#define CONTROLLER_PARAM_IO_CFG         0x06
#define CONTROLLER_PARAM_FT_POWER       0x07
#define CONTROLLER_PARAM_CAPABILITIES   0x08
#define CONTROLLER_PARAM_PMU_CFG        0x09
#define CONTROLLER_PARAM_HCI_TX_DELAY   0x0a
#define CONTROLLER_PARAM_LL_PKT_SIZE    0x0b
#define CONTROLLER_PARAM_DOUBLE_RATE_EN 0x0c
#define CONTROLLER_PARAM_WKUP_PULL_DOWN 0x0d

#if !(defined(CHIP_SEL_FR3068E) || defined(CHIP_SEL_FR3092E))
#define RESET_CONTROLLER()      do {    \
                                    /* reset controller first */    \
                                    pmu_set_pin_dir(PMU_PIN_1, PMU_GPIO_MODE_OUTPUT);   \
                                    pmu_set_pin_value(PMU_PIN_1, 0);    \
                                    system_delay_us(1000);  \
                                    pmu_set_pin_dir(PMU_PIN_1, PMU_GPIO_MODE_INPUT);    \
                                }while(0)
#endif

enum rf_tx_power_t {
    RF_TX_POWER_NEG_12dBm,
    RF_TX_POWER_NEG_8dBm,
    RF_TX_POWER_NEG_4dBm,
    RF_TX_POWER_0dBm,
    RF_TX_POWER_POS_4dBm,
    RF_TX_POWER_POS_8dBm,
    RF_TX_POWER_POS_10dBm,
    RF_TX_POWER_MAX,
};

enum controller_version_t {
    CONTROLLER_VERSION_B,
    CONTROLLER_VERSION_C,
    CONTROLLER_VERSION_MAX,
};

typedef __PACKED_STRUCT {
    uint8_t port_sel;
    uint16_t bit_mask;
    uint16_t int_en;
    uint16_t inout_sel;
    uint16_t pull_en;
    uint16_t pull_sel;
    uint16_t out_val;
} bt_io_pmu_cfg_t;

typedef __PACKED_STRUCT {
    uint16_t tx_len;
    uint16_t tx_time;
    uint16_t rx_len;
    uint16_t rx_time;
} bt_ll_pkt_size_t;

static const uint8_t app_boot_conn_req[] = {'f','r','e','q','c','h','i','p'};//from embedded to pc, request
static const uint8_t app_boot_conn_ack[] = {'F','R','1','0','1','0','O','K'};//from pc to embedded,ack
static const uint8_t app_boot_conn_success[] = {'o','k'};

static const uint8_t controller_param_header[] = {'f', 'r', 'e', 'q'};
static const uint8_t controller_param_tail[] = {'c', 'h', 'i', 'p'};

static const uint8_t tx_pwr_table[RF_TX_POWER_MAX][CONTROLLER_VERSION_MAX] = {
    {0, 0},
    {0, 1},
    {0, 2},
    {1, 3},
    {2, 4},
    {3, 5},
    {3, 6},
};

///default feature
//static uint8_t bt_feature_param[] = {
//    0x41,0x08,
//    0xaf,0x2a,0x4d,0xde,0xc3,0x2f,0x5b,0x87,
//};
const static uint8_t btdm_internal_param[] = {
    ///default value: 0xaf,0x2a,0x4d,0xde,0xc3,0x2f,0x5b,0x87
    ///disable 3M(B3-2),disable sniff subrating(B5-1),enable afh capable salve(B4-3)
    0x41,0x08,
    0xaf,0x2a,0x4d,0xda,0xcb,0x2d,0x5b,0x87,
    ///to add
};

#if defined(CHIP_SEL_FR3066D) || defined(CHIP_SEL_FR3068E_V2) || defined(CHIP_SEL_FR5098E) 
/* 
 * BT_PA7->MCU_PB3
 * BT_PA6->MCU_PB2
 * BT_PA5->MCU_PB1
 * BT_PA4->MCU_PB0
 */
static const bt_io_pmu_cfg_t port_cfg = {
    .port_sel = 0,
    .bit_mask = 0xffff,
    .int_en = 0x0000,
    .inout_sel = 0xffff,
    .pull_en = 0xff0f,
    .pull_sel = 0xffff,
    .out_val = 0x0000,
};
#elif defined(CHIP_SEL_FR3068E)
/* 
 * BT_PA6->MCU_PB1
 * BT_PA5->MCU_PB0
 */
 static const bt_io_pmu_cfg_t port_cfg = {
    .port_sel = 0,
    .bit_mask = 0xffff,
    .int_en = 0x0000,
    .inout_sel = 0xffff,
    .pull_en = 0xff9f,
    .pull_sel = 0xffff,
    .out_val = 0x0000,
};
#elif defined(CHIP_SEL_FR3092E) || defined(CHIP_SEL_FR3092E_V4) || defined(CHIP_SEL_FR5092E)
/* 
 * BT_PA11->MCU_PB3
 * BT_PA10->MCU_PB2
 * BT_PA9->MCU_PB1
 * BT_PA8->MCU_PB0
 */
static const bt_io_pmu_cfg_t port_cfg = {
    .port_sel = 0,
    .bit_mask = 0xffff,
    .int_en = 0x0000,
    .inout_sel = 0xffff,
    .pull_en = 0xf0ff,
    .pull_sel = 0xffff,
    .out_val = 0x0000,
};
#elif defined(CHIP_SEL_FR5099E)
/* 
 * BT_PA5->MCU_PA8
 */
static const bt_io_pmu_cfg_t port_cfg = {
    .port_sel = 0,
    .bit_mask = 0xffff,
    .int_en = 0x0000,
    .inout_sel = 0xffff,
    .pull_en = 0xffdf,
    .pull_sel = 0xffff,
    .out_val = 0x0000,
};
#else
#error "unsupported chip!"
#endif

static const bt_ll_pkt_size_t ll_pkt_size = {
    .tx_len = 251,
    .tx_time = 2120,
    .rx_len = 251,
    .rx_time = 2120,
};

__RAM_CODE static void clear_uart_fifo(UART_HandleTypeDef *uart_handle)
{
    while(!__UART_IS_RxFIFO_EMPTY(uart_handle->UARTx)) {
        volatile uint8_t c = uart_handle->UARTx->DATA_DLL.DATA;
    }
}

__RAM_CODE static void write_reg(UART_HandleTypeDef *uart_handle, uint32_t address, uint32_t data)
{
    uint8_t opcode;
    uint8_t buffer[6];
    opcode = 0x27;  // write reg
    buffer[0] = 0;   
    buffer[1] = 0;
    buffer[2] = 0;
    buffer[3] = 0;
    buffer[4] = 8;
    buffer[5] = 0;
    uart_transmit(uart_handle, (void *)&opcode, 1);
    uart_transmit(uart_handle, (void *)&buffer, 6);
    uart_transmit(uart_handle, (void *)&address, 4);
    uart_transmit(uart_handle, (void *)&data, 4);
}

#if !(defined(CHIP_SEL_FR3068E) || defined(CHIP_SEL_FR3092E))
__RAM_CODE static void set_bypass(UART_HandleTypeDef *uart_handle)
{
    RESET_CONTROLLER();
    
    system_delay_us(15000);
    clear_uart_fifo(uart_handle);   // clear handshake data
    uart_transmit(uart_handle, (void *)app_boot_conn_ack, 8);
    system_delay_us(5000);
    clear_uart_fifo(uart_handle);   // clear handshake data
    
    /* set IOLDO of controller to BYPASS mode */
    write_reg(uart_handle, 0x500f0000,0x00a83112);
    system_delay_us(3000);
    clear_uart_fifo(uart_handle);   // clear response data
    write_reg(uart_handle, 0x500f0004,0x00000076);
    system_delay_us(3000);
    clear_uart_fifo(uart_handle);   // clear response data
    write_reg(uart_handle, 0x500f0000,0x00a83113);
    system_delay_us(3000);
    clear_uart_fifo(uart_handle);   // clear response data
    
    /* make sure PB0~PB7 is in charge of GPIO */
    write_reg(uart_handle, 0x500f0000,0x00c63112);
    system_delay_us(3000);
    clear_uart_fifo(uart_handle);   // clear response data
    write_reg(uart_handle, 0x500f0004,0x000000ff);
    system_delay_us(3000);
    clear_uart_fifo(uart_handle);   // clear response data
    write_reg(uart_handle, 0x500f0000,0x00c63113);
    system_delay_us(3000);
    clear_uart_fifo(uart_handle);   // clear response data
    
    RESET_CONTROLLER();
}
#endif

bool controller_start(uint32_t baudrate, const uint8_t *ble_addr, const uint8_t *bt_addr, uint32_t src_addr)
{
    /* hardware handlers */
    UART_HandleTypeDef HCI_handle;
    GPIO_InitTypeDef gpio_config;
    uint8_t buffer[12];
    uint32_t length;
    uint8_t *src, *dst;
    uint16_t tx_length;
    uint8_t opcode, param_type;
    struct {
        uint32_t dst;
        uint32_t len;
    } header;
    enum controller_version_t version;

#if !(defined(CHIP_SEL_FR3068E) || defined(CHIP_SEL_FR3092E))
    RESET_CONTROLLER();

    enum_PMU_3V3_ioldo_voltage_t vol_setting = pmu_get_3V3_ioldo_voltage();
    pmu_set_3V3_ioldo_voltage(PMU_3V3_IOLDO_1_9V);
#endif

    /* configure PA0, PA1, PA2, PA3 to UART0 function */
    gpio_config.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3;
    gpio_config.Mode = GPIO_MODE_AF_PP;
    gpio_config.Pull = GPIO_PULLUP;
    gpio_config.Alternate = GPIO_FUNCTION_1;
    gpio_init(GPIOA, &gpio_config);
    
    /* UART0: used for Log and AT command */
    __SYSTEM_UART0_CLK_ENABLE();
    HCI_handle.UARTx = HCI_UART;
    HCI_handle.Init.BaudRate   = 115200;
    HCI_handle.Init.DataLength = UART_DATA_LENGTH_8BIT;
    HCI_handle.Init.StopBits   = UART_STOPBITS_1;
    HCI_handle.Init.Parity     = UART_PARITY_NONE;
    HCI_handle.Init.FIFO_Mode  = UART_FIFO_ENABLE;
    HCI_handle.TxCpltCallback  = NULL;
    HCI_handle.RxCpltCallback  = NULL;
    uart_init(&HCI_handle);
    /* keep RTS is inactive before HCI is ready */
    __UART_AUTO_FLOW_CONTROL_DISABLE(HCI_handle.UARTx);
    __UART_RTS_INACTIVE(HCI_handle.UARTx);

#if !(defined(CHIP_SEL_FR3068E) || defined(CHIP_SEL_FR3092E))
    set_bypass(&HCI_handle);
#endif
    
    /* hand shake with controller */
    uart_receive(&HCI_handle, buffer, 8);
    while (memcmp(buffer, app_boot_conn_req, 8)) {
        for (uint8_t i=0; i< 7; i++) {
            buffer[i] = buffer[i+1];
        }
        uart_receive(&HCI_handle, &buffer[7], 1);
    }

    uart_transmit(&HCI_handle, (void *)app_boot_conn_ack, 8);
    uart_receive(&HCI_handle, buffer, 2);
    if (memcmp(buffer, app_boot_conn_success, 2)) {
        return false;
    }
    
    /* SWD Enable RAM */
    SWD_W_SystemReg(HCI_UART_BAUDRATE);
    
    HCI_handle.Init.BaudRate   = HCI_UART_BAUDRATE;
    uart_config_baudRate(&HCI_handle);
    system_delay_us(5000);

#if !(defined(CHIP_SEL_FR3068E) || defined(CHIP_SEL_FR3092E))
    pmu_set_3V3_ioldo_voltage(vol_setting);
#endif
    
    if (src_addr == 0) {
        /* read ROM code */
        opcode = 0x0a;
        buffer[0] = 4;   
        buffer[1] = 0;
        buffer[2] = 0;
        buffer[3] = 0;
        buffer[4] = 4;
        buffer[5] = 0;
        uart_transmit(&HCI_handle, (void *)&opcode, 1);
        uart_transmit(&HCI_handle, (void *)&buffer, 6);
        uart_receive(&HCI_handle, buffer, 11);
#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
        if (buffer[7] == 0x79) {
            extern uint8_t CONTROLLER_CODE_REVC_BASE;
            src_addr = (uint32_t)&CONTROLLER_CODE_REVC_BASE;
            version = CONTROLLER_VERSION_C;
        }
        else if (buffer[7] == 0x75) {
            extern uint8_t CONTROLLER_CODE_REVB_BASE;
            src_addr = (uint32_t)&CONTROLLER_CODE_REVB_BASE;
            version = CONTROLLER_VERSION_B;
        }
        else {
            while(1);
        }
#elif defined(__GNUC__) || defined(__ICCARM__)
        if (buffer[7] == 0x79) {
            extern const uint8_t controller_code_revc_buffer[];
            src_addr = (uint32_t)&controller_code_revc_buffer[0];
            version = CONTROLLER_VERSION_C;
        }
        else if (buffer[7] == 0x75) {
            extern const uint8_t controller_code_buffer[];
            src_addr = (uint32_t)&controller_code_buffer[0];
            version = CONTROLLER_VERSION_B;
        }
        else {
            while(1);
        }
#else
#error "not supported platform"
#endif
        
    }
    src_addr += 0x10;
    
    /* change uart baudrate */
//    opcode = 0x12;
//    buffer[0] = 11;   // 921600
//    buffer[1] = 0;
//    buffer[2] = 0;
//    buffer[3] = 0;
//    buffer[4] = 0;
//    buffer[5] = 0;
//    uart_transmit(&HCI_handle, (void *)&opcode, 1);
//    uart_transmit(&HCI_handle, (void *)&buffer, 6);
//    uart_receive(&HCI_handle, buffer, 7);
//    if (buffer[0] != 0x13) {
//        return false;
//    }
//    HCI_handle.Init.BaudRate   = HCI_UART_BAUDRATE;
//    uart_config_baudRate(&HCI_handle);
//    system_delay_us(5000);
    
    memcpy((void *)&header, (void *)src_addr, sizeof(header));
    src_addr += sizeof(header);
    
    /* write code into RAM */
    opcode = 0x04;
    src = (void *)src_addr;
    dst = (void *)header.dst;
    length = header.len;
    while (length) {
        tx_length = length > 256 ? 256 : length;
        uart_transmit(&HCI_handle, (void *)&opcode, 1);
        uart_transmit(&HCI_handle, (void *)&dst, 4);
        uart_transmit(&HCI_handle, (void *)&tx_length, 2);
        uart_transmit(&HCI_handle, src, tx_length);
        
        uart_receive(&HCI_handle, buffer, 7);
        if (buffer[0] != 0x05) {
            return false;
        }
        
        length -= tx_length;
        src += tx_length;
        dst += tx_length;
    }
    src_addr += header.len;

    /* write parameters to exchange memory */
    opcode = 0x04;
    dst = (void *)0x40014000;
    /* write header to remote device */
    uart_transmit(&HCI_handle, (void *)&opcode, 1);
    uart_transmit(&HCI_handle, (void *)&dst, 4);
    length = sizeof(controller_param_header);
    dst += length;
    uart_transmit(&HCI_handle, (void *)&length, 2);
    uart_transmit(&HCI_handle, (void *)&controller_param_header[0], length);
    uart_receive(&HCI_handle, buffer, 7);
    if (buffer[0] != 0x05) {
        return false;
    }
    /* write baudrate to controller */
    uart_transmit(&HCI_handle, (void *)&opcode, 1);
    uart_transmit(&HCI_handle, (void *)&dst, 4);
    length = sizeof(baudrate) + 1 + 2;
    dst += length;
    uart_transmit(&HCI_handle, (void *)&length, 2);
    param_type = CONTROLLER_PARAM_BAUDRATE;
    uart_transmit(&HCI_handle, (void *)&param_type, 1);
    length = sizeof(baudrate);
    uart_transmit(&HCI_handle, (void *)&length, 2);
    uart_transmit(&HCI_handle, (void *)&baudrate, length);
    uart_receive(&HCI_handle, buffer, 7);
    if (buffer[0] != 0x05) {
        return false;
    }
    /* write keys to controller */
    length = flashdb_get_length(FDB_KEY_CONTROLLER_INFO) + sizeof(btdm_internal_param);
    if (length) {
        uint8_t *tmp = pvPortMalloc(length);
        uint16_t sub_length = length;
        uint16_t key_len = length - sizeof(btdm_internal_param);
        if(key_len){
            flashdb_get(FDB_KEY_CONTROLLER_INFO, tmp, key_len);
            memcpy(&tmp[key_len],btdm_internal_param,sizeof(btdm_internal_param));
        }
        else{
            memcpy(&tmp[0],btdm_internal_param,sizeof(btdm_internal_param));
        }
        uart_transmit(&HCI_handle, (void *)&opcode, 1);
        uart_transmit(&HCI_handle, (void *)&dst, 4);
        
        length = sub_length + 1 + 2;
        dst += length;
        uart_transmit(&HCI_handle, (void *)&length, 2);
        param_type = CONTROLLER_PARAM_KEY;
        uart_transmit(&HCI_handle, (void *)&param_type, 1);
        length = sub_length;
        uart_transmit(&HCI_handle, (void *)&length, 2);
        uart_transmit(&HCI_handle, (void *)&tmp[0], length);
        uart_receive(&HCI_handle, buffer, 7);
        if (buffer[0] != 0x05) {
            vPortFree(tmp);
            return false;
        }
        vPortFree(tmp);
    }

    /* write bt address to controller */
    uart_transmit(&HCI_handle, (void *)&opcode, 1);
    uart_transmit(&HCI_handle, (void *)&dst, 4);
    length = 6 + 1 + 2;
    dst += length;
    uart_transmit(&HCI_handle, (void *)&length, 2);
    param_type = CONTROLLER_PARAM_BT_ADDR;
    uart_transmit(&HCI_handle, (void *)&param_type, 1);
    length = 6;
    uart_transmit(&HCI_handle, (void *)&length, 2);
    uart_transmit(&HCI_handle, (void *)&bt_addr[0], length);
    uart_receive(&HCI_handle, buffer, 7);
    if (buffer[0] != 0x05) {
        return false;
    }

    /* write ble address to controller */
    uart_transmit(&HCI_handle, (void *)&opcode, 1);
    uart_transmit(&HCI_handle, (void *)&dst, 4);
    length = 6 + 1 + 2;
    dst += length;
    uart_transmit(&HCI_handle, (void *)&length, 2);
    param_type = CONTROLLER_PARAM_BLE_ADDR;
    uart_transmit(&HCI_handle, (void *)&param_type, 1);
    length = 6;
    uart_transmit(&HCI_handle, (void *)&length, 2);
    uart_transmit(&HCI_handle, (void *)&ble_addr[0], length);
    uart_receive(&HCI_handle, buffer, 7);
    if (buffer[0] != 0x05) {
        return false;
    }

    if(trim_get_txpower_param() != 0x7fff){
        /* write FT paramer base tx power to controller */
        uart_transmit(&HCI_handle, (void *)&opcode, 1);
        uart_transmit(&HCI_handle, (void *)&dst, 4);
        length = 2 + 1 + 2;
        dst += length;
        uart_transmit(&HCI_handle, (void *)&length, 2);
        param_type = CONTROLLER_PARAM_FT_POWER;
        uart_transmit(&HCI_handle, (void *)&param_type, 1);
        length = 2;
        uint8_t tx_power[2] = {0};
        tx_power[0] =  (trim_get_txpower_param()-200)&0xff;
        tx_power[1] = ((trim_get_txpower_param()-200)&0xff00) >> 8;
        uart_transmit(&HCI_handle, (void *)&length, 2);
        uart_transmit(&HCI_handle, tx_power, length);
        uart_receive(&HCI_handle, buffer, 7);
        if (buffer[0] != 0x05) {
            return false;
        }    
    }

    /* write tx power to controller */
    uart_transmit(&HCI_handle, (void *)&opcode, 1);
    uart_transmit(&HCI_handle, (void *)&dst, 4);
    length = 1 + 1 + 2;
    dst += length;
    uart_transmit(&HCI_handle, (void *)&length, 2);
    param_type = CONTROLLER_PARAM_TX_POWER;
    uart_transmit(&HCI_handle, (void *)&param_type, 1);
    length = 1;
    uint8_t tx_power_cfg = tx_pwr_table[RF_TX_POWER_0dBm][version];
    uart_transmit(&HCI_handle, (void *)&length, 2);
    uart_transmit(&HCI_handle, &tx_power_cfg, length);
    uart_receive(&HCI_handle, buffer, 7);
    if (buffer[0] != 0x05) {
        return false;
    }
    
//    /* write pmu cfg to controller */
//    const uint8_t pmu_cfg[] = {0xa5, 0xe4};
//    uart_transmit(&HCI_handle, (void *)&opcode, 1);
//    uart_transmit(&HCI_handle, (void *)&dst, 4);
//    length = sizeof(pmu_cfg) + 1 + 2;
//    dst += length;
//    uart_transmit(&HCI_handle, (void *)&length, 2);
//    param_type = CONTROLLER_PARAM_PMU_CFG;
//    uart_transmit(&HCI_handle, (void *)&param_type, 1);
//    length = sizeof(pmu_cfg);
//    uart_transmit(&HCI_handle, (void *)&length, 2);
//    uart_transmit(&HCI_handle, (uint8_t *)&pmu_cfg[0], length);
//    uart_receive(&HCI_handle, buffer, 7);
//    if (buffer[0] != 0x05) {
//        return false;
//    }    

    /* write IO configuration to controller */
    uart_transmit(&HCI_handle, (void *)&opcode, 1);
    uart_transmit(&HCI_handle, (void *)&dst, 4);
    length = sizeof(port_cfg) + 1 + 2;
    dst += length;
    uart_transmit(&HCI_handle, (void *)&length, 2);
    param_type = CONTROLLER_PARAM_IO_CFG;
    uart_transmit(&HCI_handle, (void *)&param_type, 1);
    length = sizeof(port_cfg);
    uart_transmit(&HCI_handle, (void *)&length, 2);
    uart_transmit(&HCI_handle, (uint8_t *)&port_cfg, length);
    uart_receive(&HCI_handle, buffer, 7);
    if (buffer[0] != 0x05) {
        return false;
    }
    
    /* write baseband capabilities to controller */
    const uint8_t bb_cap[] = {0x00/*coded_phy_sel*/, 10/*con_max*/, 2/*adv_max*/, 37, 0x00, 8/*ble_tx*/, 8/*ble_rx*/, 5/*bt_tx*/, 6/*bt_rx*/};
    uart_transmit(&HCI_handle, (void *)&opcode, 1);
    uart_transmit(&HCI_handle, (void *)&dst, 4);
    length = sizeof(bb_cap) + 1 + 2;
    dst += length;
    uart_transmit(&HCI_handle, (void *)&length, 2);
    param_type = CONTROLLER_PARAM_CAPABILITIES;
    uart_transmit(&HCI_handle, (void *)&param_type, 1);
    length = sizeof(bb_cap);
    uart_transmit(&HCI_handle, (void *)&length, 2);
    uart_transmit(&HCI_handle, (uint8_t *)&bb_cap[0], length);
    uart_receive(&HCI_handle, buffer, 7);
    if (buffer[0] != 0x05) {
        return false;
    }
    
    /* write custom link layer packet size to controller */
    uart_transmit(&HCI_handle, (void *)&opcode, 1);
    uart_transmit(&HCI_handle, (void *)&dst, 4);
    length = sizeof(ll_pkt_size) + 1 + 2;
    dst += length;
    uart_transmit(&HCI_handle, (void *)&length, 2);
    param_type = CONTROLLER_PARAM_LL_PKT_SIZE;
    uart_transmit(&HCI_handle, (void *)&param_type, 1);
    length = sizeof(ll_pkt_size);
    uart_transmit(&HCI_handle, (void *)&length, 2);
    uart_transmit(&HCI_handle, (uint8_t *)&ll_pkt_size, length);
    uart_receive(&HCI_handle, buffer, 7);
    if (buffer[0] != 0x05) {
        return false;
    }
    
//    /* enable double rate mode */
//    uart_transmit(&HCI_handle, (void *)&opcode, 1);
//    uart_transmit(&HCI_handle, (void *)&dst, 4);
//    length = 2 + 1 + 2;
//    dst += length;
//    uart_transmit(&HCI_handle, (void *)&length, 2);
//    param_type = CONTROLLER_PARAM_DOUBLE_RATE_EN;
//    uart_transmit(&HCI_handle, (void *)&param_type, 1);
//    length = 2;
//    uart_transmit(&HCI_handle, (void *)&length, 2);
//    buffer[0] = 1;
//    buffer[1] = 32;
//    uart_transmit(&HCI_handle, (uint8_t *)&buffer[0], length);
//    uart_receive(&HCI_handle, buffer, 7);
//    if (buffer[0] != 0x05) {
//        return false;
//    }

//    /* set wake up pin to pull down mode */
//    uart_transmit(&HCI_handle, (void *)&opcode, 1);
//    uart_transmit(&HCI_handle, (void *)&dst, 4);
//    length = 1 + 1 + 2;
//    dst += length;
//    uart_transmit(&HCI_handle, (void *)&length, 2);
//    param_type = CONTROLLER_PARAM_WKUP_PULL_DOWN;
//    uart_transmit(&HCI_handle, (void *)&param_type, 1);
//    length = 1;
//    uint8_t dummy = 1;
//    uart_transmit(&HCI_handle, (void *)&length, 2);
//    uart_transmit(&HCI_handle, &dummy, length);
//    uart_receive(&HCI_handle, buffer, 7);
//    if (buffer[0] != 0x05) {
//        return false;
//    }

    /* write tail to controller */
    uart_transmit(&HCI_handle, (void *)&opcode, 1);
    uart_transmit(&HCI_handle, (void *)&dst, 4);
    length = sizeof(controller_param_tail);
    dst += length;
    uart_transmit(&HCI_handle, (void *)&length, 2);
    uart_transmit(&HCI_handle, (void *)&controller_param_tail[0], length);
    uart_receive(&HCI_handle, buffer, 7);
    if (buffer[0] != 0x05) {
        return false;
    }
    
    /* boot from RAM */
    opcode = 0x2b;
    dst = (void *)header.dst;
    tx_length = 0;
    uart_transmit(&HCI_handle, (void *)&opcode, 1);
    uart_transmit(&HCI_handle, (void *)&dst, 4);
    uart_transmit(&HCI_handle, (void *)&tx_length, 2);
    uart_receive(&HCI_handle, buffer, 7);
    if (buffer[0] != 0x2c) {
        return false;
    }

    memcpy((void *)&header, (void *)src_addr, sizeof(header));
    src_addr += sizeof(header);
    if (header.len) {
        system_delay_us(50000);
        opcode = 0x04;
        src = (void *)src_addr;
        dst = (void *)header.dst;
        length = header.len;
        while (length) {
            tx_length = length > 256 ? 256 : length;
            uart_transmit(&HCI_handle, (void *)&opcode, 1);
            uart_transmit(&HCI_handle, (void *)&dst, 4);
            uart_transmit(&HCI_handle, (void *)&tx_length, 2);
            uart_transmit(&HCI_handle, src, tx_length);
            
            uart_receive(&HCI_handle, buffer, 7);
            if (buffer[0] != 0x05) {
                return false;
            }
            
            length -= tx_length;
            src += tx_length;
            dst += tx_length;
        }
        src_addr += header.len;
    
        /* disconnect */
        opcode = 0x10;
        dst = (void *)1;    /* normal disconnect */
        tx_length = 0;
        uart_transmit(&HCI_handle, (void *)&opcode, 1);
        uart_transmit(&HCI_handle, (void *)&dst, 4);
        uart_transmit(&HCI_handle, (void *)&tx_length, 2);
        uart_receive(&HCI_handle, buffer, 7);
        if (buffer[0] != 0x11) {
            return false;
        }
    }
    
//#if !(defined(CHIP_SEL_FR3068E) || defined(CHIP_SEL_FR3092E))
//    system_delay_us(10000);
//    pmu_set_3V3_ioldo_voltage(vol_setting);
//#endif
    return true;
}
