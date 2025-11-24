/*
  ******************************************************************************
  * @file    driver_flash.c
  * @author  FreqChip Firmware Team
  * @version V1.1.0
  * @date    2024
  * @brief   Internal XIP Flash module driver.
  *          This file provides firmware functions to manage the internal
  *          stacked xip flash
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 FreqChip.
  * All rights reserved.
  ******************************************************************************
*/
#include "fr30xx.h"

#include "driver_psram.h"

#include "co_util.h"

#define PSRAM_IO_GPIO                   0
#define PSRAM_ENABLE_Q_MODE             1
#define PSRAM_CLK_DIV_SEL               QSPI_BAUDRATE_DIV_2

#define PSRAM_OSPI_IF                   OSPI

#define PSRAM_READ_IDENTIFICATION       0x9F

#define PSRAM_READ_OPCODE               0x03
#define PSRAM_FAST_READ_OPCODE          0x0B
#define PSRAM_FAST_QUAL_READ_OPCODE     0xEB

#define PSRAM_PAGE_PROGRAM_OPCODE       0x02
#define PSRAM_PAGE_QUAL_PROGRAM_OPCODE  0x38

#define PSRAM_ENTER_QUAD_MODE_OPCODE    0x35
#define PSRAM_EXIT_QUAD_MODE_OPCODE     0xF5

#define PSRAM_RESET_ENABLE_OPCODE       0x66
#define PSRAM_RESET_OPCODE              0x99

#define PSRAM_READ_MODE_OPCODE          0xb5
#define PSRAM_WRITE_MODE_OPCODE         0xb1

static const struct qspi_stig_reg_t psram_read_ctrl_cmd = {
    .enable_bank = 0,
    .dummy_cycles = 0,
    .write_bytes = 0,
    .enable_write = 0,
    .addr_bytes = QSPI_STIG_ADDR_BYTES_3,
    .enable_mode = 0,
    .enable_cmd_addr = 1,
    .read_bytes = 2,
    .enable_read = 1,
    .opcode = 0x40,
};

static const struct qspi_stig_reg_t psram_write_ctrl_cmd = {
    .enable_bank = 0,
    .dummy_cycles = 0,
    .write_bytes = 2,
    .enable_write = 1,
    .addr_bytes = QSPI_STIG_ADDR_BYTES_4,
    .enable_mode = 0,
    .enable_cmd_addr = 1,
    .read_bytes = 0,
    .enable_read = 0,
    .opcode = 0xC0,
};

static const struct qspi_stig_reg_t psram_read_id_cmd = {
    .enable_bank = 0,
    .dummy_cycles = 0,
    .write_bytes = 0,
    .enable_write = 0,
    .addr_bytes = 0,
    .enable_mode = 0,
    .enable_cmd_addr = 0,
    .read_bytes = 3,
    .enable_read = 1,
    .opcode = PSRAM_READ_IDENTIFICATION,
};

static const struct qspi_stig_reg_t enter_quad_mode_cmd = {
    .enable_bank = 0,
    .dummy_cycles = 0,
    .write_bytes = 0,
    .enable_write = 0,
    .addr_bytes = 0,
    .enable_mode = 0,
    .enable_cmd_addr = 0,
    .read_bytes = 0,
    .enable_read = 0,
    .opcode = PSRAM_ENTER_QUAD_MODE_OPCODE,
};

static const struct qspi_stig_reg_t reset_enable_cmd = {
    .enable_bank = 0,
    .dummy_cycles = 0,
    .write_bytes = 0,
    .enable_write = 0,
    .addr_bytes = 0,
    .enable_mode = 0,
    .enable_cmd_addr = 0,
    .read_bytes = 0,
    .enable_read = 0,
    .opcode = PSRAM_RESET_ENABLE_OPCODE,
};

static const struct qspi_stig_reg_t reset_cmd = {
    .enable_bank = 0,
    .dummy_cycles = 0,
    .write_bytes = 0,
    .enable_write = 0,
    .addr_bytes = 0,
    .enable_mode = 0,
    .enable_cmd_addr = 0,
    .read_bytes = 0,
    .enable_read = 0,
    .opcode = PSRAM_RESET_OPCODE,
};

static const struct qspi_stig_reg_t read_mode_cmd = {
    .enable_bank = 0,
    .dummy_cycles = 1,
    .write_bytes = 0,
    .enable_write = 0,
    .addr_bytes = 2,
    .enable_mode = 0,
    .enable_cmd_addr = 1,
    .read_bytes = 0,
    .enable_read = 1,
    .opcode = PSRAM_READ_MODE_OPCODE,
};

static struct qspi_stig_reg_t write_single_cmd = {
    .enable_bank = 0,
    .dummy_cycles = 0,
    .write_bytes = 0,
    .enable_write = 0,
    .addr_bytes = 0,
    .enable_mode = 0,
    .enable_cmd_addr = 0,
    .read_bytes = 0,
    .enable_read = 0,
    .opcode = PSRAM_WRITE_MODE_OPCODE,
};

static uint8_t psram_read_mode(void)
{
    uint8_t mode;

    qspi_stig_cmd(PSRAM_OSPI_IF, read_mode_cmd, QSPI_STIG_CMD_READ, 1, &mode);
    
    return mode;
}

static void psram_write_mode(uint8_t mode)
{
    GPIO_InitTypeDef    GPIO_Handle;
#if PSRAM_IO_GPIO
    __SYSTEM_GPIOB_CLK_ENABLE();
    GPIO_Handle.Pin       = GPIO_PIN_13;
    GPIO_Handle.Mode      = GPIO_MODE_OUTPUT_PP;
    GPIO_Handle.Pull      = GPIO_NOPULL;
    GPIO_Handle.Alternate = GPIO_FUNCTION_0;
    gpio_init(GPIOB, &GPIO_Handle);
    gpio_write_pin(GPIOB, GPIO_PIN_13, GPIO_PIN_CLEAR);
#else
    GPIO_Handle.Pin       = GPIO_PIN_7;
    GPIO_Handle.Mode      = GPIO_MODE_OUTPUT_PP;
    GPIO_Handle.Pull      = GPIO_NOPULL;
    GPIO_Handle.Alternate = GPIO_FUNCTION_0;
    gpioex_ospi_init(&GPIO_Handle);
    gpioex_ospi_write_pin(GPIO_PIN_7, GPIO_PIN_CLEAR);
#endif
    write_single_cmd.opcode = PSRAM_WRITE_MODE_OPCODE;
    qspi_stig_cmd(PSRAM_OSPI_IF, write_single_cmd, QSPI_STIG_CMD_EXE, 0, NULL);
    write_single_cmd.opcode = 0;
    qspi_stig_cmd(PSRAM_OSPI_IF, write_single_cmd, QSPI_STIG_CMD_EXE, 0, NULL);
    write_single_cmd.opcode = 0;
    qspi_stig_cmd(PSRAM_OSPI_IF, write_single_cmd, QSPI_STIG_CMD_EXE, 0, NULL);
    write_single_cmd.opcode = 0;
    qspi_stig_cmd(PSRAM_OSPI_IF, write_single_cmd, QSPI_STIG_CMD_EXE, 0, NULL);
    write_single_cmd.opcode = mode;
    qspi_stig_cmd(PSRAM_OSPI_IF, write_single_cmd, QSPI_STIG_CMD_EXE, 0, NULL);
#if PSRAM_IO_GPIO
    GPIO_Handle.Pin       = GPIO_PIN_7;
    GPIO_Handle.Mode      = GPIO_MODE_AF_PP;
    GPIO_Handle.Pull      = GPIO_NOPULL;
    GPIO_Handle.Alternate = GPIO_FUNCTION_1;
    gpioex_ospi_init(&GPIO_Handle);
#else
    GPIO_Handle.Pin       = GPIO_PIN_13;
    GPIO_Handle.Mode      = GPIO_MODE_AF_PP;
    GPIO_Handle.Pull      = GPIO_NOPULL;
    GPIO_Handle.Alternate = GPIO_FUNCTION_9;
    gpio_init(GPIOB, &GPIO_Handle);
#endif
}

static void psram_enter_quad(void)
{
    qspi_stig_cmd(PSRAM_OSPI_IF, enter_quad_mode_cmd, QSPI_STIG_CMD_EXE, 0, NULL);
}

static void psram_controller_init(uint16_t page_boundary)
{
    while(__QSPI_IS_BUSY(PSRAM_OSPI_IF));
    
#if PSRAM_ENABLE_Q_MODE == 1
    __QSPI_READ_OPCODE_SET(PSRAM_OSPI_IF, PSRAM_FAST_QUAL_READ_OPCODE);
    __QSPI_READ_INSTRUCTION_TYPE_SET(PSRAM_OSPI_IF, QSPI_WIRE_TYPE_QIO);
    __QSPI_READ_ADDRESS_TYPE_SET(PSRAM_OSPI_IF, QSPI_WIRE_TYPE_QIO);
    __QSPI_READ_DATA_TYPE_SET(PSRAM_OSPI_IF, QSPI_WIRE_TYPE_QIO);
    __QSPI_READ_DUMMY_CYCLES_SET(PSRAM_OSPI_IF, 6);
    __QSPI_READ_MODE_ENABLE_SET(PSRAM_OSPI_IF, 0);
    __QSPI_MODE_BIT_SET(PSRAM_OSPI_IF, 0);
    
    __QSPI_WRITE_OPCODE_SET(PSRAM_OSPI_IF, PSRAM_PAGE_QUAL_PROGRAM_OPCODE);
    __QSPI_WRITE_ADDRESS_TYPE_SET(PSRAM_OSPI_IF, QSPI_WIRE_TYPE_QIO);
    __QSPI_WRITE_DATA_TYPE_SET(PSRAM_OSPI_IF, QSPI_WIRE_TYPE_QIO);
    __QSPI_WRITE_DUMMY_CYCLES_SET(PSRAM_OSPI_IF, 0);
#else
    __QSPI_READ_OPCODE_SET(PSRAM_OSPI_IF, PSRAM_FAST_QUAL_READ_OPCODE);
    __QSPI_READ_INSTRUCTION_TYPE_SET(PSRAM_OSPI_IF, QSPI_WIRE_TYPE_STAND);
    __QSPI_READ_ADDRESS_TYPE_SET(PSRAM_OSPI_IF, QSPI_WIRE_TYPE_QIO);
    __QSPI_READ_DATA_TYPE_SET(PSRAM_OSPI_IF, QSPI_WIRE_TYPE_QIO);
    __QSPI_READ_DUMMY_CYCLES_SET(PSRAM_OSPI_IF, 6);
    __QSPI_READ_MODE_ENABLE_SET(PSRAM_OSPI_IF, 0);
    __QSPI_MODE_BIT_SET(PSRAM_OSPI_IF, 0);

    __QSPI_WRITE_OPCODE_SET(PSRAM_OSPI_IF, PSRAM_PAGE_QUAL_PROGRAM_OPCODE);
    __QSPI_WRITE_ADDRESS_TYPE_SET(PSRAM_OSPI_IF, QSPI_WIRE_TYPE_QIO);
    __QSPI_WRITE_DATA_TYPE_SET(PSRAM_OSPI_IF, QSPI_WIRE_TYPE_QIO);
    __QSPI_WRITE_DUMMY_CYCLES_SET(PSRAM_OSPI_IF, 0);
#endif

    //init configuration register
    __QSPI_CFG_CPOL_SET(PSRAM_OSPI_IF, 0);
    __QSPI_CFG_CPHA_SET(PSRAM_OSPI_IF, 0);
    __QSPI_CFG_DAC_ENABLE(PSRAM_OSPI_IF);
    __QSPI_CFG_LEGACY_DISABLE(PSRAM_OSPI_IF);
    __QSPI_CFG_REMAP_ENABLE(PSRAM_OSPI_IF);
    __QSPI_CFG_BAUDRATE_SET(PSRAM_OSPI_IF, PSRAM_CLK_DIV_SEL);
    __QSPI_READ_CAPTURE_DELAY_SET(PSRAM_OSPI_IF, 3);

    __QSPI_WRITE_WEL_DISABLE(PSRAM_OSPI_IF);
    __QSPI_POLL_DISABLE(PSRAM_OSPI_IF);

    __QSPI_READ_CAPTURE_LP_CLK_EN(PSRAM_OSPI_IF);
    __QSPI_DELAY_CS_START_SET(PSRAM_OSPI_IF, 2);
    __QSPI_DELAY_CS_END_SET(PSRAM_OSPI_IF, 2);
    __QSPI_DELAY_CS_DESSERT_SET(PSRAM_OSPI_IF, 2);
    
    __QSPI_DEVICE_PAGE_SIZE_SET(PSRAM_OSPI_IF, page_boundary);
    
    __QSPI_CS_CTRL_RD_BRK_ENABLE(PSRAM_OSPI_IF);
    __QSPI_CS_CTRL_DIS_CS_AFT_FIRST_BYTE_SET(PSRAM_OSPI_IF);
    __QSPI_CS_PAGE_BOUNDARY_SET(PSRAM_OSPI_IF, page_boundary);
    __QSPI_CS_PAGE_BOUNDARY_PROTECT_ENABLE(PSRAM_OSPI_IF);

    __QSPI_REMAP_ADDRESS_SET(PSRAM_OSPI_IF, PSRAM_DAC_BASE);
    
    __QSPI_CFG_OCTAL_OPI_DISABLE(PSRAM_OSPI_IF);
    __QSPI_CFG_OCTAL_XCCELA_DISABLE(PSRAM_OSPI_IF);

    __QSPI_ENABLE(PSRAM_OSPI_IF);
}

bool psram_init(enum psram_clk_sel_t clk_sel)
{
    GPIO_InitTypeDef    GPIO_Handle;
    uint32_t ref_clk;
    
    if (clk_sel >= PSRAM_CLK_SEL_MAX) {
        return false;
    }

    switch (clk_sel) {
        case PSRAM_CLK_SEL_COREH:
            __SYSTEM_OSPI_CLK_SELECT_COREH();
            ref_clk = 24000000;
            break;
        case PSRAM_CLK_SEL_SPLL:
            __SYSTEM_OSPI_CLK_SELECT_SPLL();
            ref_clk = system_get_SPLLCLK();
            break;
        case PSRAM_CLK_SEL_AUPLL:
            __SYSTEM_OSPI_CLK_SELECT_AUPLL();
            ref_clk = system_get_AUPLLCLK();
            break;
        default:
            return false;
    }
    __SYSTEM_OSPI_CLK_ENABLE();
    /* clock of AHB cache should be enabled before access psram */
    __SYSTEM_AHBC_CLK_ENABLE();

#if PSRAM_IO_GPIO == 1
    GPIO_Handle.Pin       = GPIO_PIN_12|GPIO_PIN_13;
    GPIO_Handle.Mode      = GPIO_MODE_AF_PP;
    GPIO_Handle.Pull      = GPIO_NOPULL;
    GPIO_Handle.Alternate = GPIO_FUNCTION_9;
    gpio_init(GPIOB, &GPIO_Handle);
    GPIO_Handle.Pin       = GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_5|GPIO_PIN_6;
    GPIO_Handle.Mode      = GPIO_MODE_AF_PP;
    GPIO_Handle.Pull      = GPIO_NOPULL;
    GPIO_Handle.Alternate = GPIO_FUNCTION_9;
    gpio_init(GPIOC, &GPIO_Handle);
#else
    SYSTEM->OspiPadConfig.OSPI_FuncMux = 0xFDF55FFD;
#endif

    qspi_stig_cmd(PSRAM_OSPI_IF, reset_enable_cmd, QSPI_STIG_CMD_EXE, 0, NULL);
    qspi_stig_cmd(PSRAM_OSPI_IF, reset_cmd, QSPI_STIG_CMD_EXE, 0, NULL);
#if PSRAM_ENABLE_Q_MODE == 1
    psram_enter_quad();
#endif  // PSRAM_ENABLE_Q_MODE == 1

    /* about 100 bytes can be transmitted during 8us when QSPI clock is 24MHz */
    ref_clk /= ((PSRAM_CLK_DIV_SEL+1) * 2);
    uint32_t bytes = ref_clk / 240000;
    uint32_t boundary_cfg = 1 << (31 - __CLZ(bytes));
    if (boundary_cfg >= 0x100) {
        boundary_cfg = 0x100;
    }
    psram_controller_init(boundary_cfg);
    
    /* detect read capture delay configuration */
    uint8_t delay_lower, delay_upper, index;
    *(volatile uint32_t *)(PSRAM_DAC_BASE) = 0x5a5a5a5a;
    for (index = 0; index < 16; index++) {
        __QSPI_READ_CAPTURE_DELAY_SET(PSRAM_OSPI_IF, index);
        if (*(volatile uint32_t *)(PSRAM_DAC_BASE) == 0x5a5a5a5a) {
            break;
        }
    }
    if (index == 16) {
        __SYSTEM_OSPI_CLK_DISABLE();
        __SYSTEM_AHBC_CLK_DISABLE();
        return false;
    }
    else {
        delay_lower = index;
    }
    for (; index < 16; index++) {
        __QSPI_READ_CAPTURE_DELAY_SET(PSRAM_OSPI_IF, index);
        delay_upper = index;
        if (*(volatile uint32_t *)(PSRAM_DAC_BASE) != 0x5a5a5a5a) {
            break;
        }
    }
    __QSPI_READ_CAPTURE_DELAY_SET(PSRAM_OSPI_IF, (delay_lower + delay_upper)/2);
    
//    printf("0x%02x, %d.\r\n", 1 << (31 - __CLZ(bytes)), (delay_lower + delay_upper)/2);

    return true;
}
