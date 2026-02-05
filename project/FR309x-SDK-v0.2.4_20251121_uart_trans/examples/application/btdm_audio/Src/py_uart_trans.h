/*
 * @Author: BlueboxChamil
 * @Date: 2026-02-05 13:18:44
 * @LastEditTime: 2026-02-05 15:54:18
 * @FilePath: \examples\application\btdm_audio\Src\py_uart_trans.h
 * @Description:
 * Copyright (c) 2026 by BlueboxChamil, All Rights Reserved.
 */
#ifndef PY_UART_TRANS_H
#define PY_UART_TRANS_H

/*==============================================================================
 * Includes
 *============================================================================*/
#include "fr30xx.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include <string.h>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "driver_gpio.h"
#include "driver_uart.h"
#include "app_task.h"

#include "FreeRTOS.h"
#include "timers.h"
#include "co_util.h"

/*
 * =================================================================================
 * MACROS AND CONSTANTS
 * =================================================================================
 */
#define MAX_PY_UART_BUF_SIZE 1024
#define MAX_ERROR_TIMES 3
#define PY_FLASH_PAGE 256
#define PY_FLASH_SECTOR 4096

/*==============================================================================
 * Typedefs
 *============================================================================*/
typedef struct
{
    uint8_t type;
    uint8_t pack_id;
    uint16_t len;
    uint8_t *data;
    uint32_t crc;

    uint8_t file_name[64];
    uint32_t file_size;
    uint8_t error_times;
} py_uart_t;


/*==============================================================================
 * Public API Declarations
 *============================================================================*/
void py_uart_trans_init(void);
void py_uart_receive();

#endif