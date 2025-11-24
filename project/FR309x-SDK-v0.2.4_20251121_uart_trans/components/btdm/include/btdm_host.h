/*
 * @file       btdm_host.h
 * @version    v1.0
 * @brief      GAP API exposed to app layer
 * @attention
 *  Copyright (c) 2022 Freqchip.   \n
 *  All rights reserved.           \n
 */

#ifndef _BTDM_HOST_H
#define _BTDM_HOST_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include <stdint.h>

/* Exported types ------------------------------------------------------------*/

enum btdm_nvds_tag {
    BTDM_NVDS_TAG_IRK,                                  //!< local device irk
    BTDM_NVDS_TAG_BOND_INFO,                            //!< BLE bonding information
    BTDM_NVDS_TAG_MAX = BTDM_NVDS_TAG_BOND_INFO+16,     //!< reserve 16 bonding information
    BTDM_NVDS_TAG_CONTROLLER_INFO,                      //!< controller information
};

enum btdm_nvds_status {
    BTDM_NVDS_STATUS_OK,
    BTDM_NVDS_STATUS_FAILED,
};

struct ble_host_param {
    /*
     * this field is to indicate identity address is PUBLIC or RANDOM PRIVATE.
     */
    uint8_t own_addr_type;  // 0: public, 1:random static, other: unacceptable

    /* local random static address, valied when own_addr_type is 1 */
    struct {
        uint8_t addr[6];
    } own_addr;
};

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/*
 * @fn      ble_host_init
 * 
 * @brief   initialize ble host and HCI interface.
 *
 * param[in] param: BLE host parameters, @ref ble_host_param, default value is used when
 *                  param is NULL
 */
void ble_host_init(struct ble_host_param *param);

/*
 * @fn      ble_gaf_init
 * 
 * @brief   initialize audio over ble module. This function should be called after host is initialized.
 */
void ble_gaf_init(void);

/*
 * @fn      bt_host_init
 * 
 * @brief   initialize bt host, ble host should be ready before call this function.
 */
void bt_host_init(void);

/*
 * @fn      bt_a2dp_init
 * 
 * @brief   initialize a2dp profile, this function should be called following bt_stack_init.
 */
void bt_a2dp_init(void);

/*
 * @fn      bt_avrcp_init
 * 
 * @brief   initialize avrcp profile, this function should be called following bt_stack_init.
 */
void bt_avrcp_init(void);

/*
 * @fn      bt_hf_init
 * 
 * @brief   initialize HF profile, this function should be called following bt_stack_init.
 */
void bt_hf_init(void);

/*
 * @fn      bt_hfg_init
 * 
 * @brief   initialize HFG profile, this function should be called following bt_stack_init.
 */
void bt_hfg_init(void);

/*
 * @fn      bt_pan_init
 * 
 * @brief   initialize BNEP/PAN profile, this function should be called following bt_stack_init.
 */
void bt_pan_init(void);

/*
 * @fn      bt_hid_init
 * 
 * @brief   initialize HID profile, this function should be called following bt_stack_init.
 */
void bt_hid_init(void);

/*
 * @fn      bt_spp_init
 * 
 * @brief   initialize SPP profile, this function should be called following bt_stack_init.
 */
void bt_spp_init(void);

/*
 * @fn      bt_pbap_init
 * 
 * @brief   initialize PBAP profile, this function should be called following bt_stack_init.
 */
void bt_pbap_init(void);

/*
 * @fn      bt_opp_init
 * 
 * @brief   initialize OPP profile, this function should be called following bt_stack_init.
 */
void bt_opp_init(void);

/*
 * @fn      bt_ftp_init
 * 
 * @brief   initialize FTP profile, this function should be called following bt_stack_init.
 */
void bt_ftp_init(void);

/*
 * @fn      bt_map_init
 * 
 * @brief   initialize MAP profile, this function should be called following bt_stack_init.
 */
void bt_map_init(void);

/*
 * @fn      ble_host_ready
 * 
 * @brief   used to check whether ble host is ready or not.
 *
 * @return  true: ble host is ready
 */
bool ble_host_ready(void);

/*
 * @fn      bt_host_ready
 * 
 * @brief   used to check whether bt host is ready or not.
 *
 * @return  true: bt host is ready
 */
bool bt_host_ready(void);

/*
 * @fn      btdm_host_schedule
 * 
 * @brief   used to schedule btdm host internal event, this function should be called
 *          in system main loop.
 *
 * @example no RTOS:
 *          while(1) {
 *              btdm_host_schedule();
 *              // other process
 *              ...
 *          }
 *
 *          RTOS:
 *          void stack_task(void)
 *          {
 *              while(1) {
 *                  pending_new_event();
 *                  btdm_host_schedule();
 *              }
 *          }
 *
 *          void btdm_host_notify_schedule(void)
 *          {
 *              post_new_event();
 *          }
 */
void btdm_host_schedule(void);

/*
 * @fn      btdm_host_check_sleep
 * 
 * @brief   used to check whether system is allowed to enter sleep.
 *
 * @return  true: btdm host can enter sleep right now
 */
bool btdm_host_check_sleep(void);

/*
 * @fn      btdm_host_schedule_ble
 * 
 * @brief   used to schedule ble host internal event, this function should be called
 *          in system main loop. This function is useful when only BLE is enabled.
 */
void btdm_host_schedule_ble(void);

/*
 * @fn      btdm_host_check_sleep_ble
 * 
 * @brief   used to check whether system is allowed to enter sleep. This function is useful
 *          when only BLE is enabled.
 *
 * @return  true: ble host can enter sleep right now
 */
bool btdm_host_check_sleep_ble(void);

/*
 * @fn      btdm_timer_trigger
 * 
 * @brief   used to notice host timer is triggered.
 */
void btdm_timer_trigger(void);

/*
 * @fn      btdm_set_seed
 * 
 * @brief   initial seed for random generate function inside btdm stack.
 *
 * @param[in]  seed: random seed.
 */
void btdm_set_seed(uint16_t seed);

/*
 * @fn      btdm_host_send_vendor_cmd
 * 
 * @brief   used to send vendor command to controller.
 *
 * @param[in]  type: command type.
 * @param[in]  length: length command parameter.
 * @param[in]  data: payload of command parameter.
 */
void btdm_host_send_vendor_cmd(uint8_t type, uint8_t length, void *data);

/* Need to be implemented functions ------------------------------------------------*/

/*
 * @fn      btdm_malloc
 * 
 * @brief   allocate request size memory.
 *
 * @param[in]  size: request size.
 *
 * @return  allocated memory pointer
 */
void *btdm_malloc(uint32_t size);

/*
 * @fn      btdm_free
 * 
 * @brief   free allocated memory by btdm_malloc.
 *
 * @param[in]  ptr: allocated memory pointer.
 */
void btdm_free(void *ptr);

/*
 * @fn      btdm_host_notify_schedule
 * 
 * @brief   used to notify btdm_host_schedule should be called to handle new btdm event,
 *          user should implement this function when RTOS is used, @ref btdm_host_schedule.
 *          Counting semaphore should be used because btdm_notify_schedule may be called more
 *          than one time before btdm_stack_schedule is called.
 */
void btdm_host_notify_schedule(void);

/*
 * @fn      btdm_host_lock
 * 
 * @brief   Called by the host to indicate the host and its APIs must not be entered by 
 *          another task. This function should be implement when RTOS is used, and binary 
 *          semaphore or mutex should be used.
 *
 * @example void btdm_stack_lock(void)
 *          {
 *              xSemaphoreTake();
 *          }
 */
void btdm_host_lock(void);

/*
 * @fn      btdm_host_unlock
 * 
 * @brief   Called by the host to allow access to the host again. This function should 
 *          be implement when RTOS is used
 *
 * @example void btdm_stack_unlock(void)
 *          {
 *              xSemaphoreGive();
 *          }
 */
void btdm_host_unlock(void);

/*
 * @fn      btdm_timer_start
 * 
 * @brief   used to start or restart timer with target timeout time.
 *
 * @param[in]  ms: timeout time(millisecond).
 */
void btdm_timer_start(uint32_t ms);

/*
 * @fn      btdm_timer_stop
 * 
 * @brief   used to stop timer.
 */
void btdm_timer_stop(void);

/*
 * @fn      btdm_get_system_time
 * 
 * @brief   used to get current time.
 *
 * @return  current time(millisecond)
 */
uint32_t btdm_get_system_time(void);

/*
 * @fn    btdm_time_diff
 *
 * @brief Clocks time difference.
 *
 * @param[in] a_time value (in ms).
 * @param[in] b_time value (in ms).
 *
 * @return    result the time difference from a_time to b_time
 *                           - result < 0  => b_time is in the past
 *                           - result == 0 => a_time is equal to b_time
 *                           - result > 0  => b_time is in the future
 */
int btdm_time_diff(uint32_t a_time, uint32_t b_time);

/*
 * @fn    btdm_nvds_put
 *
 * @brief Save generated data into NVDS.
 *
 * @param[in] tag: data type
 * @param[in] length: data length
 * @param[in] data: data pointer
 *
 * @return storage status, @ref btdm_nvds_status
 */
enum btdm_nvds_status btdm_nvds_put(uint8_t tag, uint16_t length, uint8_t *data);

/*
 * @fn    btdm_nvds_get
 *
 * @brief Get data from NVDS.
 *
 * @param[in] tag: data type
 * @param[inout] length: buffer length for input, real data length for outpu
 * @param[in] data: data pointer
 *
 * @return get data success or not, @ref btdm_nvds_status
 */
enum btdm_nvds_status btdm_nvds_get(uint8_t tag, uint16_t *length, uint8_t *buffer);

/*
 * @fn    btdm_nvds_del
 *
 * @brief delete data from NVDS with given tag.
 *
 * @param[in] tag: data type to be deleted
 *
 * @return delete success or not, @ref btdm_nvds_status
 */
enum btdm_nvds_status btdm_nvds_del(uint8_t tag);

/*
 * @fn    btdm_ble_tx_buf_nb_avail_get
 *
 * @brief get number of avaliable ble controller tx buffer.
 *
 * @return number
 */
uint16_t btdm_ble_tx_buf_nb_avail_get(void);

/*
 * @fn    btdm_sec_enable_h6
 *
 * @brief enable generating bt linkkey from ble ltk.
 *
 * @return NULL
 */
void btdm_sec_enable_h6(void);

#ifdef __cplusplus
}
#endif

#endif  // _BTDM_HOST_H


