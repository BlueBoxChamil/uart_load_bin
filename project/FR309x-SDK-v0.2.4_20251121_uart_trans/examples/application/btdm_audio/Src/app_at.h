#ifndef _APP_AT_H
#define _APP_AT_H

#include <stdint.h>

#include "driver_uart.h"

void app_at_cmd_recv_handler(uint8_t *data, uint16_t length);

void app_at_rx_done(struct __UART_HandleTypeDef *handle);

void app_at_init(struct __UART_HandleTypeDef *handle);

#endif  // _APP_AT_H
