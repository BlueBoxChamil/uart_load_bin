/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "fr30xx.h"

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

#include "fdb_app.h"
#include "host.h"
#include "ff.h"
#if DSP_ROM_CODE_XIP == 1
#include "ext_flash_program.h"
#endif

#include "app_task.h"
#include "app_at.h"
#include "audio_scene.h"
#include "app_rpmsg.h"

/* hardware handlers */
static UART_HandleTypeDef Uart3_handle;
SD_HandleTypeDef sdio_handle;
static CALI_HandleTypeDef cali_handle;

#if ENABLE_RTOS_MONITOR == 1
/* FreeRTOS running status monitor task */
static TaskHandle_t monitor_task_handle;
volatile unsigned int CPU_RunTime;
static uint8_t CPU_RunInfo[2048];
#endif

/* file system */
static FATFS fs;

/* APP task */
TaskHandle_t app_task_handle;

void controller_start(void);
void host_start(void);

#if defined(__ARMCC_VERSION) || defined(__CC_ARM)
int fputc(int c, FILE *fp)
{
    uart_transmit(&Uart3_handle, (void *)&c, 1);

    return c;
}
#endif

#ifdef __GNUC__
int _write(int file, char *ptr, int len)
{
    uart_transmit(&Uart3_handle, (void *)ptr, len);
    return len;
}
#endif

#ifdef __ICCARM__
int putchar(int c)
{
    uart_transmit(&Uart3_handle, (void *)&c, 1);
    while (!(Uart3_handle.UARTx->USR.TFE))
        ;
    return c;
}
#endif

#if ENABLE_RTOS_MONITOR == 1
static void monitor_task(void *arg)
{
    while (1)
    {
        vTaskDelay(10000);

        memset(CPU_RunInfo, 0, 2048);
        vTaskList((char *)&CPU_RunInfo);
        printf("---------------------------------------------\r\n");
        printf("name           	state     priority  stack     seq\r\n");
        printf("%s", CPU_RunInfo);
        printf("---------------------------------------------\r\n");

        memset(CPU_RunInfo, 0, 400);
        vTaskGetRunTimeStats((char *)&CPU_RunInfo);
        printf("name                counter             usage\r\n");
        printf("%s", CPU_RunInfo);
        printf("---------------------------------------------\r\n");
    }
}
#endif

#if DSP_ROM_CODE_XIP == 1
static uint32_t ext_flash_read_id(void)
{
    return flash_read_id(QSPI1);
}

static uint32_t ext_flash_write(uint32_t offset, uint32_t length, const uint8_t *buffer)
{
    flash_write(QSPI1, offset, length, buffer);
    return length;
}

static uint32_t ext_flash_read(uint32_t offset, uint32_t length, uint8_t *buffer)
{
    flash_read(QSPI1, offset, length, buffer);
    return length;
}

static void ext_flash_erase(uint32_t offset, uint32_t length)
{
    flash_erase(QSPI1, offset, length);
}

static const struct ext_flash_operator_t ext_flash_op = {
    .flash_init = ext_flash_read_id,
    .read = ext_flash_read,
    .write = ext_flash_write,
    .erase = ext_flash_erase,
    .chip_erase = NULL,
    .protect_disable = NULL,
    .protect_enable = NULL,
};

static void prog_uart_init(uint32_t bandrate)
{
    GPIO_InitTypeDef gpio_config;

    /* configure PB4 and PB5 to UART3 function */
    gpio_config.Pin = GPIO_PIN_4 | GPIO_PIN_5;
    gpio_config.Mode = GPIO_MODE_AF_PP;
    gpio_config.Pull = GPIO_PULLUP;
    gpio_config.Alternate = GPIO_FUNCTION_1;
    gpio_init(GPIOB, &gpio_config);

    /* UART3: used for Log and AT command */
    __SYSTEM_UART3_CLK_ENABLE();
    Uart3_handle.UARTx = UART3;
    Uart3_handle.Init.BaudRate = bandrate;
    Uart3_handle.Init.DataLength = UART_DATA_LENGTH_8BIT;
    Uart3_handle.Init.StopBits = UART_STOPBITS_1;
    Uart3_handle.Init.Parity = UART_PARITY_NONE;
    Uart3_handle.Init.FIFO_Mode = UART_FIFO_ENABLE;
    Uart3_handle.TxCpltCallback = NULL;
    Uart3_handle.RxCpltCallback = NULL;
    uart_init(&Uart3_handle);
    NVIC_SetPriority(UART3_IRQn, 4);
}

static void prog_uart_read(uint8_t *data, uint16_t length)
{
    uart_receive(&Uart3_handle, data, length);
}

static uint16_t prog_uart_read_no_block(uint8_t *data, uint16_t length)
{
    int len = 0;
    UART_HandleTypeDef *huart = &Uart3_handle;

    while (len < length)
    {
        /* Rx ready */
        if (!(huart->UARTx->LSR.LSR_BIT.DR))
            break;
        /* receive data */
        data[len++] = huart->UARTx->DATA_DLL.DATA;
    }
    return len;
}

static void prog_uart_write(uint8_t *data, uint16_t length)
{
    uart_transmit(&Uart3_handle, data, length);
}

static const struct ext_flash_prog_uart_op_t ext_flash_prog_uart_op = {
    .init = prog_uart_init,
    .read = prog_uart_read,
    .read_no_block = prog_uart_read_no_block,
    .write = prog_uart_write,
};
#endif

void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
    (void)pcTaskName;
    (void)pxTask;

    assert(0);
}

void vApplicationTickHook(void)
{
#if ENABLE_RTOS_MONITOR == 1
    CPU_RunTime++;
#endif
}

static void cali_done_handle(CALI_HandleTypeDef *hcali, uint32_t result)
{
    system_set_LPRCCLK(cali_calc_rc_freq(hcali, result));
    system_prevent_sleep_clear(SYSTEM_PREVENT_SLEEP_TYPE_CALIBRATION);
}

__RAM_CODE void hw_clock_init(void)
{
    System_ClkConfig_t sys_clk_cfg;

    sys_clk_cfg.AUPLL_CFG.PLL_N = 8;
    sys_clk_cfg.AUPLL_CFG.PLL_M = 15205;
    sys_clk_cfg.AUPLL_CFG.PowerEn = 1;
    sys_clk_cfg.SPLL_CFG.PLL_N = 8;
    sys_clk_cfg.SPLL_CFG.PLL_M = 0;
    sys_clk_cfg.SPLL_CFG.PowerEn = 1;
    sys_clk_cfg.MCU_Clock_Source = MCU_CLK_SEL_SPLL_CLK;
    sys_clk_cfg.SOC_DIV = 1;
    sys_clk_cfg.MCU_DIV = 1;
    sys_clk_cfg.APB0_DIV = 1;
    sys_clk_cfg.APB1_DIV = 1;
    sys_clk_cfg.APB2_DIV = 1;
    sys_clk_cfg.APB3_DIV = 1;
    System_AUPLL_config(&sys_clk_cfg.AUPLL_CFG, 1000);
    System_SPLL_config(&sys_clk_cfg.SPLL_CFG, 1000);
    System_MCU_clock_Config(&sys_clk_cfg);

    __SYSTEM_UART_CLK_SELECT_SPLL();
    //    __SYSTEM_UART_CLK_DIV(2);
}

__RAM_CODE void hw_xip_flash_init(bool wake_up)
{
    System_XIPConfig_t xip_config;
    xip_config.CLK_SRC_SEL = XIP_CLK_SEL_SPLL;
    xip_config.DIV_SEL = QSPI_BAUDRATE_DIV_4;
    xip_config.RD_TYPE = FLASH_RD_TYPE_QUAD;
    xip_config.WR_TYPE = FLASH_WR_TYPE_SINGLE;
    system_xip_flash_init(&xip_config, wake_up);
}

#if DSP_ROM_CODE_XIP == 1
__RAM_CODE void hw_dsp_xip_flash_init(bool wake_up)
{
    /* init QSPI1 for DSP XIP flash */
    __SYSTEM_QSPI1_CLK_SELECT_SPLL();
    __SYSTEM_QSPI1_CLK_ENABLE();

#if 0
    /* config OSPI pad to QSPI1 function, used to access DSP XIP flash */
    SYSTEM->OspiPadConfig.OSPI_FuncMux = 0x55555555;
    
    /* power on DSP flash */
    ool_write(0xfc, ool_read(0xfc) | 0x01);
#else
    {
        /* config PC12~PC15 to QSPI1 function, used to access DSP XIP flash */
        GPIO_InitTypeDef gpio_config;
        gpio_config.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
        gpio_config.Mode = GPIO_MODE_AF_PP;
        gpio_config.Pull = GPIO_PULLUP;
        gpio_config.Alternate = GPIO_FUNCTION_C;
        gpio_init(GPIOB, &gpio_config);
        gpio_config.Pin = GPIO_PIN_0 | GPIO_PIN_1;
        gpio_config.Mode = GPIO_MODE_AF_PP;
        gpio_config.Pull = GPIO_PULLUP;
        gpio_config.Alternate = GPIO_FUNCTION_C;
        gpio_init(GPIOC, &gpio_config);
    }
#endif

    /* configure QSPI1 controller, used to access DSP XIP flash */
    flash_init_controller(QSPI1, FLASH_RD_TYPE_QUAD_FAST, FLASH_WR_TYPE_SINGLE);
    __QSPI_DELAY_CS_START_SET(QSPI1, 4);
    __QSPI_DELAY_CS_END_SET(QSPI1, 4);
    flash_set_baudrate(QSPI1, QSPI_BAUDRATE_DIV_4);
    __QSPI_READ_CAPTURE_DELAY_SET(QSPI1, 0);
    flash_enable_quad(QSPI1);
}
#endif

__RAM_CODE bool user_deep_sleep_check(void)
{
    /// check gpio status first
    //    if((GPIOC->GPIO_IN_DATA & 0x04) == 0){
    //        return false;
    //    }
    return host_before_sleep_check();
}
__RAM_CODE bool user_entry_before_sleep_check(void)
{
    bool ret = true;
    /// user check if the io has been toggled just before sleep, shall return to wakeup state as soon as possible
    /// following code check whether pc2(user defined wakeup io) has been fall down before sleep
    //    if((GPIOC->GPIO_IN_DATA & 0x04) == 0){
    //        ret = false;
    //    }
    return ret;
}
__RAM_CODE void user_entry_before_sleep(void)
{
    while (!(Uart3_handle.UARTx->USR.TFE))
        ;
    system_delay_us(100);
#if defined(CHIP_SEL_FR3068E)
    ool_write16(PMU_REG_PIN_PULL_EN, 0x3fff);
#else
    ool_write16(PMU_REG_PIN_PULL_EN, 0x3ffd);
#endif
    ool_write16(PMU_REG_PIN_PULL_SEL, 0x3fff);

    /* gpio_PD Wakeup Init */
    SYSTEM->PortD_InputCutoffDisable = 0x0000ffff;

    ool_write(PMU_REG_PMU_GATE_M, ool_read(PMU_REG_PMU_GATE_M) | 0x40);
}

__RAM_CODE void user_entry_after_sleep(void)
{
    /*
     * enable pull up of all 3.3v IO, these configuration will be latched by set
     * BIT6 of PMU_REG_PMU_GATE_M regsiter. used to avoid electric leakage
     */
    SYSTEM->PortA_PullSelect = 0x0000ffff;
    SYSTEM->PortB_PullSelect = 0x00000fff;
    SYSTEM->PortC_PullSelect = 0x00000000;
    SYSTEM->PortD_PullSelect = 0x0000ffff;
#if defined(CHIP_SEL_FR3068E)
    SYSTEM->PortA_PullEN = 0x00007fff;
#else
    SYSTEM->PortA_PullEN = 0x0000ffff;
#endif
    SYSTEM->PortB_PullEN = 0x00000fff;
    SYSTEM->PortC_PullEN = 0x00000000;
    SYSTEM->PortD_PullEN = 0x0000ffff;
    SYSTEM->QspiPadConfig.QSPI_PullEN = 0x0000000;

    hw_clock_init();
    hw_xip_flash_init(true);
    host_hci_reinit();
    ool_write(PMU_REG_PMU_GATE_M, ool_read(PMU_REG_PMU_GATE_M) & (~0x40));

    GPIO_InitTypeDef gpio_config;

    /* configure all interrupt priority to 2 */
    *(volatile uint32_t *)0xE000E400 = 0x40404040;
    *(volatile uint32_t *)0xE000E404 = 0x40404040;
    *(volatile uint32_t *)0xE000E408 = 0x40404040;
    *(volatile uint32_t *)0xE000E40C = 0x40404040;
    *(volatile uint32_t *)0xE000E410 = 0x40404040;
    *(volatile uint32_t *)0xE000E414 = 0x40404040;
    *(volatile uint32_t *)0xE000E418 = 0x40404040;
    *(volatile uint32_t *)0xE000E41C = 0x40404040;
    *(volatile uint32_t *)0xE000E420 = 0x40404040;
    *(volatile uint32_t *)0xE000E424 = 0x40404040;
    *(volatile uint32_t *)0xE000E428 = 0x40404040;
    *(volatile uint32_t *)0xE000E42C = 0x40404040;
    *(volatile uint32_t *)0xE000E430 = 0x40404040;
    *(volatile uint32_t *)0xE000E434 = 0x40404040;
    *(volatile uint32_t *)0xE000E438 = 0x40404040;
    *(volatile uint32_t *)0xE000E43C = 0x40404040;
    *(volatile uint32_t *)0xE000E440 = 0x40404040;

    NVIC_SetPriority(UART0_IRQn, 2);
    NVIC_EnableIRQ(UART0_IRQn);
    NVIC_SetPriority(PMU_IRQn, 4);
    NVIC_EnableIRQ(PMU_IRQn);

    /* configure PB4 and PB5 to UART3 function */
    __SYSTEM_GPIOA_CLK_ENABLE();
    gpio_config.Pin = GPIO_PIN_4 | GPIO_PIN_5;
    gpio_config.Mode = GPIO_MODE_AF_PP;
    gpio_config.Pull = GPIO_PULLUP;
    gpio_config.Alternate = GPIO_FUNCTION_1;
    gpio_init(GPIOB, &gpio_config);

    /* UART0: used for Log and AT command */
    __SYSTEM_UART3_CLK_ENABLE();
    Uart3_handle.UARTx = UART3;
    Uart3_handle.Init.BaudRate = 921600;
    Uart3_handle.Init.DataLength = UART_DATA_LENGTH_8BIT;
    Uart3_handle.Init.StopBits = UART_STOPBITS_1;
    Uart3_handle.Init.Parity = UART_PARITY_NONE;
    Uart3_handle.Init.FIFO_Mode = UART_FIFO_ENABLE;
    Uart3_handle.TxCpltCallback = NULL;
    Uart3_handle.RxCpltCallback = app_at_rx_done;
    uart_init(&Uart3_handle);
    NVIC_SetPriority(UART3_IRQn, 4);
    NVIC_EnableIRQ(UART3_IRQn);

    {
        static bool first_wakeup = true;
        bool do_calib = false;
        static TickType_t last_tick;
        TickType_t curr_tick;
        if (first_wakeup)
        {
            first_wakeup = false;
            last_tick = xTaskGetTickCount();
            curr_tick = last_tick;
            do_calib = true;
        }
        else
        {
            curr_tick = xTaskGetTickCount();
            if ((curr_tick - last_tick) > 10000)
            {
                last_tick = curr_tick;
                do_calib = true;
            }
        }

        if (do_calib)
        {
            /* restart calibration */
            __SYSTEM_CALI_CLK_ENABLE();
            cali_handle.mode = CALI_UP_MODE_NORMAL;
            cali_handle.rc_cnt = 60;
            cali_handle.DoneCallback = cali_done_handle;
            cali_init(&cali_handle);
            cali_start_IT(&cali_handle);
            system_prevent_sleep_set(SYSTEM_PREVENT_SLEEP_TYPE_CALIBRATION);
            NVIC_SetPriority(CALI_IRQn, 2);
            NVIC_EnableIRQ(CALI_IRQn);
        }
    }
    app_rpmsg_recover();
}

__RAM_CODE void user_entry_after_sleep_user(void)
{
}

int main(void)
{
    GPIO_InitTypeDef gpio_config;
    UART_HandleTypeDef dsp_uart_handle;
    uint32_t error;

    system_delay_us(1000000);

    /* configure all interrupt priority to 2 */
    *(volatile uint32_t *)0xE000E400 = 0x40404040;
    *(volatile uint32_t *)0xE000E404 = 0x40404040;
    *(volatile uint32_t *)0xE000E408 = 0x40404040;
    *(volatile uint32_t *)0xE000E40C = 0x40404040;
    *(volatile uint32_t *)0xE000E410 = 0x40404040;
    *(volatile uint32_t *)0xE000E414 = 0x40404040;
    *(volatile uint32_t *)0xE000E418 = 0x40404040;
    *(volatile uint32_t *)0xE000E41C = 0x40404040;
    *(volatile uint32_t *)0xE000E420 = 0x40404040;
    *(volatile uint32_t *)0xE000E424 = 0x40404040;
    *(volatile uint32_t *)0xE000E428 = 0x40404040;
    *(volatile uint32_t *)0xE000E42C = 0x40404040;
    *(volatile uint32_t *)0xE000E430 = 0x40404040;
    *(volatile uint32_t *)0xE000E434 = 0x40404040;
    *(volatile uint32_t *)0xE000E438 = 0x40404040;
    *(volatile uint32_t *)0xE000E43C = 0x40404040;
    *(volatile uint32_t *)0xE000E440 = 0x40404040;

    pmu_init();
    ool_write(0xc3, 0x27);

    /* Power Keep: 32KB PRAM, 128KB SRAM */
    //    ool_write16(PMU_REG_PKSRAM_GATE, ~0x0063);
    /* IO33 always on, IO1V8 off in sleep mode */
    ool_write(PMU_REG_PMU_MASK_L, 0x08);

    hw_clock_init();
    hw_xip_flash_init(false);
#if DSP_ROM_CODE_XIP == 1
    hw_dsp_xip_flash_init(false);
    ext_flash_program(&ext_flash_op, &ext_flash_prog_uart_op);
#endif

    __SYSTEM_GPIOA_CLK_ENABLE();
    /* configure PA6 to PA_EN function */
    gpio_config.Pin = GPIO_PIN_6;
    gpio_config.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_config.Pull = GPIO_PULLUP;
    gpio_config.Alternate = GPIO_FUNCTION_0;
    gpio_init(GPIOA, &gpio_config);
    gpio_write_pin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);

    /* initialize uart for DSP UART */
    /* ========================================================== */
    /* =========         Uart LOG configuration         ========= */
    /* ========================================================== */
    /* configure PA4 and PA5 to UART1 function */
    gpio_config.Pin = GPIO_PIN_4 | GPIO_PIN_5;
    gpio_config.Mode = GPIO_MODE_AF_PP;
    gpio_config.Pull = GPIO_PULLUP;
    gpio_config.Alternate = GPIO_FUNCTION_1;
    gpio_init(GPIOA, &gpio_config);

    /* UART1: used for DSP Log  */
    __SYSTEM_UART1_CLK_ENABLE();
    dsp_uart_handle.UARTx = UART1;
    dsp_uart_handle.Init.BaudRate = 115200;
    dsp_uart_handle.Init.DataLength = UART_DATA_LENGTH_8BIT;
    dsp_uart_handle.Init.StopBits = UART_STOPBITS_1;
    dsp_uart_handle.Init.Parity = UART_PARITY_NONE;
    dsp_uart_handle.Init.FIFO_Mode = UART_FIFO_ENABLE;
    dsp_uart_handle.Init.AUTO_FLOW = UART_AUTO_FLOW_DISABLE;
    dsp_uart_handle.TxCpltCallback = NULL;
    dsp_uart_handle.RxCpltCallback = NULL;
    uart_init(&dsp_uart_handle);

    /* configure PA12~PA15 to DSP-JTAG function */
    gpio_config.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    gpio_config.Mode = GPIO_MODE_AF_PP;
    gpio_config.Pull = GPIO_PULLUP;
    gpio_config.Alternate = GPIO_FUNCTION_6;
    gpio_init(GPIOA, &gpio_config);

    /* configure PB4 and PB5 to UART3 function */
    gpio_config.Pin = GPIO_PIN_4 | GPIO_PIN_5;
    gpio_config.Mode = GPIO_MODE_AF_PP;
    gpio_config.Pull = GPIO_PULLUP;
    gpio_config.Alternate = GPIO_FUNCTION_1;
    gpio_init(GPIOB, &gpio_config);

    /* UART3: used for Log and AT command */
    __SYSTEM_UART3_CLK_ENABLE();
    Uart3_handle.UARTx = UART3;
    Uart3_handle.Init.BaudRate = 921600;
    Uart3_handle.Init.DataLength = UART_DATA_LENGTH_8BIT;
    Uart3_handle.Init.StopBits = UART_STOPBITS_1;
    Uart3_handle.Init.Parity = UART_PARITY_NONE;
    Uart3_handle.Init.FIFO_Mode = UART_FIFO_ENABLE;
    Uart3_handle.TxCpltCallback = NULL;
    Uart3_handle.RxCpltCallback = app_at_rx_done;
    uart_init(&Uart3_handle);
    NVIC_EnableIRQ(UART3_IRQn);
    NVIC_SetPriority(UART3_IRQn, 4);

    /* do calibration, get current RC frequency */
    __SYSTEM_CALI_CLK_ENABLE();
    cali_handle.mode = CALI_UP_MODE_NORMAL;
    cali_handle.rc_cnt = 200;
    cali_handle.DoneCallback = cali_done_handle;
    cali_init(&cali_handle);
    cali_start_IT(&cali_handle);
    system_prevent_sleep_set(SYSTEM_PREVENT_SLEEP_TYPE_CALIBRATION);
    NVIC_SetPriority(CALI_IRQn, 4);
    NVIC_EnableIRQ(CALI_IRQn);

    /* ========================================================== */
    /* =========       I2S interface configuration       ======== */
    /* ========================================================== */
    /* configure PB0~PB3 to I2S0 function */
    gpio_config.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3;
    gpio_config.Mode = GPIO_MODE_AF_PP;
    gpio_config.Pull = GPIO_PULLUP;
    gpio_config.Alternate = GPIO_FUNCTION_B;
    gpio_init(GPIOB, &gpio_config);

    /* ========================================================== */
    /* =========      SPDIF interface configuration      ======== */
    /* ========================================================== */
    /* configure PC10 to SPDIF function */
    gpio_config.Pin = GPIO_PIN_10;
    gpio_config.Mode = GPIO_MODE_AF_PP;
    gpio_config.Pull = GPIO_PULLUP;
    gpio_config.Alternate = GPIO_FUNCTION_C;
    gpio_init(GPIOC, &gpio_config);

    /* init flashdb to store user data */
    flashdb_init();

    /* get random seed*/
    uint32_t rand_num;
    size_t size = flashdb_get(FDB_KEY_RANDOM_SEED, (void *)&rand_num, 4);
    printf("flashdb get random seed :%d\r\n", size);
    if (size == 0)
    {
        __SYSTEM_TRNG_CLK_ENABLE();
        trng_init();
        trng_read_rand_num((uint8_t *)&rand_num, 4);
        flashdb_set(FDB_KEY_RANDOM_SEED, (uint8_t *)&rand_num, 4);
        __SYSTEM_TRNG_CLK_DISABLE();
    }
    srand(rand_num);
    printf("flash db get rand num: %x\r\n", rand_num);
    /* Create tasks */
#if ENABLE_RTOS_MONITOR == 1
    xTaskCreate(monitor_task, "monitor", MONITOR_TASK_STACK_SIZE, NULL, MONITOR_TASK_PRIORITY, &monitor_task_handle);
#endif
    py_uart_trans_init();
    app_task_init();
    audio_scene_init(AUDIO_SCENE_TASK_STACK_SIZE, AUDIO_SCENE_TASK_PRIORITY);

    /* initialize AT command */
    app_at_init(&Uart3_handle);

    /*
     * enable pull up of all 3.3v IO, these configuration will be latched by set
     * BIT6 of PMU_REG_PMU_GATE_M regsiter. used to avoid electric leakage
     */
    SYSTEM->PortA_PullSelect = 0x0000ffff;
    SYSTEM->PortB_PullSelect = 0x00000fff;
    SYSTEM->PortC_PullSelect = 0x00000000;
    SYSTEM->PortD_PullSelect = 0x0000ffff;
#if defined(CHIP_SEL_FR3068E)
    SYSTEM->PortA_PullEN = 0x00007fff;
#else
    SYSTEM->PortA_PullEN = 0x0000ffff;
#endif
    SYSTEM->PortB_PullEN = 0x00000fff;
    SYSTEM->PortC_PullEN = 0x00000000;
    SYSTEM->PortD_PullEN = 0x0000ffff;
    SYSTEM->QspiPadConfig.QSPI_PullEN = 0x0000000;

    /* enable sleep */
    // system_prevent_sleep_clear(SYSTEM_PREVENT_SLEEP_TYPE_DISABLE);

    printf("FR5090: BTDM test.\r\n");

    /* Start the scheduler itself. */
    vTaskStartScheduler();

    return 0;
}

void uart3_irq(void)
{
    uart_IRQHandler(&Uart3_handle);
}

void PMU_GPIO_PMU_IRQHandler(void)
{
    uint16_t data = ool_read16(PMU_REG_PIN_DATA);
    uint16_t result = ool_read16(PMU_REG_PIN_XOR_RESULT);

    /* update last value with latest data */
    ool_write16(PMU_REG_PIN_LAST_V, data);
    /* clear last XOR result */
    ool_write16(PMU_REG_PIN_XOR_CLR, result);

    if (data & PMU_PIN_9)
    {
        system_prevent_sleep_clear(SYSTEM_PREVENT_SLEEP_TYPE_HCI_RX);
    }
    else
    {
        system_prevent_sleep_set(SYSTEM_PREVENT_SLEEP_TYPE_HCI_RX);
    }
}

void cali_irq(void)
{
    cali_IRQHandler(&cali_handle);
}

const uint32_t dac_gaf_coef_48000[] = {
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
};

const uint32_t dac_gaf_coef_44100[] = {
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
};

const uint32_t dac_gaf_coef_16000[] = {
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
};

const uint32_t dac_gaf_coef_8000[] = {
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
};

const uint32_t adc_gaf_coef_16000[] = {
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
};

const uint32_t adc_gaf_coef_8000[] = {
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00400000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
};

void codec_GPF1_config(CODEC_HandleTypeDef *hcodec)
{
    const uint32_t *coef_ptr;
    uint32_t *reg_p0_ptr = (uint32_t *)&CODEC->GPF[1].SectionCoef[0];
    uint32_t *reg_p1_ptr = (uint32_t *)&CODEC->GPF[1].SectionCoef[6];
    if (hcodec->OutputConfig.DAC_SampleRate == CODEC_SAMPLE_RATE_48000)
    {
        coef_ptr = dac_gaf_coef_48000;
    }
    else if (hcodec->OutputConfig.DAC_SampleRate == CODEC_SAMPLE_RATE_44100)
    {
        coef_ptr = dac_gaf_coef_44100;
    }
    else if (hcodec->OutputConfig.DAC_SampleRate == CODEC_SAMPLE_RATE_16000)
    {
        coef_ptr = dac_gaf_coef_16000;
    }
    else if (hcodec->OutputConfig.DAC_SampleRate == CODEC_SAMPLE_RATE_8000)
    {
        coef_ptr = dac_gaf_coef_8000;
    }
    else
    {
        __CODEC_GPF1_BYPASS();
        return;
    }

    __CODEC_GPF1_DISABLE();
    __CODEC_GPF1_RESET();

    for (uint32_t i = 0; i < 60; i++)
    {
        *reg_p0_ptr++ = coef_ptr[i];
        *reg_p1_ptr++ = coef_ptr[i];
    }

    __CODEC_GPF_SECTION_ENABLE(1, 0xfff);
    __CODEC_GPF1_ENABLE();
}

void codec_GPF0_config(CODEC_HandleTypeDef *hcodec)
{
    const uint32_t *coef_ptr;
    uint32_t *reg_p0_ptr = (uint32_t *)&CODEC->GPF[0].SectionCoef[0];
    uint32_t *reg_p1_ptr = (uint32_t *)&CODEC->GPF[0].SectionCoef[6];

    if (hcodec->InputConfig.ADC_SampleRate == CODEC_SAMPLE_RATE_16000)
    {
        coef_ptr = adc_gaf_coef_16000;
    }
    else if (hcodec->InputConfig.ADC_SampleRate == CODEC_SAMPLE_RATE_8000)
    {
        coef_ptr = adc_gaf_coef_8000;
    }
    else
    {
        __CODEC_GPF0_BYPASS();
        return;
    }

    __CODEC_GPF0_DISABLE();
    __CODEC_GPF0_RESET();

    for (uint32_t i = 0; i < 60; i++)
    {
        *reg_p0_ptr++ = coef_ptr[i];
        *reg_p1_ptr++ = coef_ptr[i];
    }

    __CODEC_GPF_SECTION_ENABLE(0, 0xfff);
    __CODEC_GPF0_ENABLE();
}
