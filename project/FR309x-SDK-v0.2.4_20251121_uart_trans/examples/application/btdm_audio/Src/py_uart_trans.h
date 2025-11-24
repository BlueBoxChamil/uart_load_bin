#ifndef PY_UART_TRANS_H
#define PY_UART_TRANS_H

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

#define MAX_PY_UART_BUF_SIZE 1024
#define MAX_ERROR_TIMES 3
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

void py_uart_trans_init(void);
void py_uart_receive(py_uart_t *ptr);
static void py_uart_send(char *str);

static void py_uart_flash_write(uint8_t *data, uint16_t len, uint32_t offset);
static void py_uart_flash_read(uint8_t *data, uint16_t len, uint32_t offset);
static uint32_t py_uart_crc32(py_uart_t *ptr);
static uint32_t crc32_compute(const uint8_t *buf, size_t len, uint32_t crc);
static void app_py_recv_c(uint8_t c);
static void app_py_rx_done(struct __UART_HandleTypeDef *handle);
static void app_py_init(struct __UART_HandleTypeDef *handle);
static void py_timeout_timer_func(); 
UART_HandleTypeDef *uart2Handler();

#endif