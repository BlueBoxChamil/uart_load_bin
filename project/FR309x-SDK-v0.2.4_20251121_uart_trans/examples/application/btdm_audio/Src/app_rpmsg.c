#include "fr30xx.h"

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

#include "rpmsg.h"
#include "rpmsg_lite.h"
#include "rpmsg_queue.h"
#include "rpmsg_ns.h"

#include "dsp.h"

#include "app_config.h"
#include "app_task.h"
#include "app_rpmsg.h"

/* Remote rpmsg instance */
static struct rpmsg_lite_instance *remote_rpmsg;
static TaskHandle_t rpmsg_task_handle;

static void rpmsg_recv(struct rpmsg_lite_instance *rpmsg, struct rpmsg_msg_t *msg)
{
//    printf("rpmsg_recv: msg_type = %d.\r\n", msg->msg_type);

    switch (msg->msg_type) {
        case RPMSG_MSG_TYPE_SYNC_INVOKE:
            if (msg->p.sync_func.func_id == RPMSG_SYNC_FUNC_SUM) {
                uint32_t address = (uint32_t)msg->p.sync_func.param;
                struct rpmsg_sync_msg_sum_t *param = msg->p.sync_func.param;
                if (address >= DSP_DRAM_BASE_ADDR) {
                    address = DSP_DRAM_2_MCU_SRAM(address);
                }
                param = (void *)address;
                rpmsg_send_sync_ret(rpmsg, 0, param->x + param->y);
            }
            else {
                rpmsg_send_sync_ret(rpmsg, 0xffffffff, 0);
            }
            break;
        case RPMSG_MSG_TYPE_ASYNC_MSG:
            break;
        default:
            break;
    }
}

static void rpmsg_task(void *arg)
{
    struct app_task_event *event;
    struct rpmsg_msg_t *msg;
    uint32_t msg_len;
    uint32_t src_addr;
    uint32_t length;
    
    dsp_prepare();

#if DSP_CODE_LOAD_MODE == DSP_CODE_LOAD_MODE_XIP_RO
#if DSP_ROM_CODE_XIP == 1
#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
    extern uint8_t DSP_CODE_FLASH_BASE, DSP_CODE_FLASH_END;
    src_addr = (uint32_t)&DSP_CODE_FLASH_BASE;
    length = (uint32_t)&DSP_CODE_FLASH_END - (uint32_t)&DSP_CODE_FLASH_BASE;
#elif defined(__GNUC__) || defined(__ICCARM__)
    extern const uint8_t dsp_code_flash_buffer[];
    src_addr = (uint32_t)&dsp_code_flash_buffer[0];
    length = 0;
#endif
#else
#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
    extern uint8_t DSP_CODE_ROM_BASE, DSP_CODE_ROM_END;
    src_addr = (uint32_t)&DSP_CODE_ROM_BASE;
    length = (uint32_t)&DSP_CODE_ROM_END - (uint32_t)&DSP_CODE_ROM_BASE;
#elif defined(__GNUC__) || defined(__ICCARM__)
    extern const uint8_t dsp_code_rom_buffer[];
    src_addr = (uint32_t)&dsp_code_rom_buffer[0];
    length = 0;
#endif
#endif
#elif DSP_CODE_LOAD_MODE == DSP_CODE_LOAD_MODE_FIX_ADDRESS
    src_addr = (uint32_t)((DSP_CODE_FIX_ADDRESS + 0x10) | FLASH_DAC_BASE);
    length = *(volatile uint32_t *)((DSP_CODE_FIX_ADDRESS + 0x08) | FLASH_DAC_BASE);
#else
#error "choose correct DSP code load mode"
#endif
    if (dsp_load_code_from_internal_flash(src_addr, length) == false) {

        printf("DSP image load failed.\r\n");
        while(1);
    }

    printf("DSP is released.\r\n");
#if DSP_ROM_CODE_XIP == 0
    __SYSTEM_LIBRARY_ROM_CLK_ENABLE();
#endif
    remote_rpmsg = rpmsg_remote_init(rpmsg_recv);
    dsp_run(0x78400660);
    system_prevent_sleep_set(SYSTEM_PREVENT_SLEEP_TYPE_DSP);
    rpmsg_wait_master_ready(remote_rpmsg);
    system_prevent_sleep_clear(SYSTEM_PREVENT_SLEEP_TYPE_DSP);

    printf("DSP is ready.\r\n");

    /* notice APP that rpmsg is ready. */
	event = app_task_event_alloc(APP_TASK_EVENT_RPMSG_INITED, 0, true);
    app_task_event_post(event, false);

    while (1) {
        src_addr = rpmsg_recv_msg(remote_rpmsg, &msg, &msg_len);
        rpmsg_recv(remote_rpmsg, msg);
        rpmsg_lite_release_rx_buffer(remote_rpmsg, msg);
    }
}

void app_rpmsg_init(void)
{
    /* create rpmsg task. */
    xTaskCreate(rpmsg_task, "RPMSG_TASK", RPMSG_TASK_STACK_SIZE, NULL, RPMSG_TASK_PRIORITY, &rpmsg_task_handle);
}

void app_rpmsg_recover(void)
{
    dsp_prepare();
    
    uint32_t src_addr;
    uint32_t length;
#if DSP_CODE_LOAD_MODE == DSP_CODE_LOAD_MODE_XIP_RO
#if DSP_ROM_CODE_XIP == 1
#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
    extern uint8_t DSP_CODE_FLASH_BASE, DSP_CODE_FLASH_END;
    src_addr = (uint32_t)&DSP_CODE_FLASH_BASE;
    length = (uint32_t)&DSP_CODE_FLASH_END - (uint32_t)&DSP_CODE_FLASH_BASE;
#elif defined(__GNUC__) || defined(__ICCARM__)
    extern const uint8_t dsp_code_flash_buffer[];
    src_addr = (uint32_t)&dsp_code_flash_buffer[0];
    length = 0;
#endif
#else
#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
    extern uint8_t DSP_CODE_ROM_BASE, DSP_CODE_ROM_END;
    src_addr = (uint32_t)&DSP_CODE_ROM_BASE;
    length = (uint32_t)&DSP_CODE_ROM_END - (uint32_t)&DSP_CODE_ROM_BASE;
#elif defined(__GNUC__) || defined(__ICCARM__)
    extern const uint8_t dsp_code_rom_buffer[];
    src_addr = (uint32_t)&dsp_code_rom_buffer[0];
    length = 0;
#endif
#endif
#elif DSP_CODE_LOAD_MODE == DSP_CODE_LOAD_MODE_FIX_ADDRESS
    src_addr = (uint32_t)((DSP_CODE_FIX_ADDRESS + 0x10) | FLASH_DAC_BASE);
    length = *(volatile uint32_t *)((DSP_CODE_FIX_ADDRESS + 0x08) | FLASH_DAC_BASE);
#else
#error "choose correct DSP code load mode"
#endif

    dsp_load_rw_from_internal_flash(src_addr, length);

#if DSP_ROM_CODE_XIP == 0
    __SYSTEM_LIBRARY_ROM_CLK_ENABLE();
#endif
    dsp_run(0x78400660);

    rpmsg_remote_recover();
}
