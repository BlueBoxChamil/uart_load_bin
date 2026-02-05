#include "app_config.h"
#include "app_task.h"
#include "app_btdm.h"
#include "app_ble.h"
#include "app_bt.h"

#include "controller.h"
#include "host.h"
#include "fdb_app.h"
#include "user_bt.h"

void app_btdm_start(void)
{
    app_ble_init();
#if BTDM_STACK_ENABLE_BT == 1
    app_bt_init();
    user_bt_init();
#endif
}

void host_ready_cb(void)
{
    struct app_task_event *event;
    /* notify application BTDM stack is ready. */
    event = app_task_event_alloc(APP_TASK_EVENT_HOST_INITED, 0, true);
    app_task_event_post(event, false);
}

void app_btdm_init(void)
{
    uint8_t bt_addr[] = {0x32, 0x32, 0x32, 0x32, 0x32, 0x32};
    uint8_t id_length = 5;
    
    system_get_unique_ID(bt_addr, &id_length);
    /* prepare for BTDM stack */
    controller_start(BTDM_STACK_HCI_BAUDRATE, bt_addr, bt_addr, 0);
    bt_addr[5] = 0xc0;
#if BTDM_STACK_ENABLE_BT == 1
    host_btdm_start(BTDM_STACK_HCI_BAUDRATE, HOST_TASK_STACK_SIZE, HOST_TASK_PRIORITY, bt_addr);
#else
    host_ble_start(BTDM_STACK_HCI_BAUDRATE, HOST_TASK_STACK_SIZE, HOST_TASK_PRIORITY, bt_addr);
#endif
    
    /* 
     * init MCU->BT pin, configure PMU_PIN_8 output BBG_EN signal, this pin is used to 
     * notice BT core that MCU is in working mode.
     */
    ool_write(PMU_REG_DIAG_CTRL, 0x82);
    ool_write(PMU_REG_PIN_IOMUX_H, 0x03);

    /* 
     * init BT->MCU pin, system should not enter sleep mode when this pin is low level. 
     * This pin is used by BT core to notice MCU than BT core is in working mode.
     */
    system_prevent_sleep_set(SYSTEM_PREVENT_SLEEP_TYPE_HCI_RX);
    pmu_gpio_int_init(PMU_PIN_9, PMU_GPIO_PULL_UP, 0);
    pmu_enable_isr(PMU_GPIO_PMU_INT_MSK_BIT);
    NVIC_SetPriority(PMU_IRQn, 4);
    NVIC_EnableIRQ(PMU_IRQn);
}

