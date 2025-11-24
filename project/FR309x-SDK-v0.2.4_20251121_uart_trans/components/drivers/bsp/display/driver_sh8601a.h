#ifndef __DRIVER_SH8601A_H
#define __DRIVER_SH8601A_H

#include <stdint.h>

void sh8601a_init(void);

void sh8601a_set_window(uint16_t x_s, uint16_t x_e, uint16_t y_s, uint16_t y_e);

void sh8601a_display(uint32_t pixel_count, uint8_t pixel_width, void *data);

void sh8601a_display_dma(uint32_t pixel_count, uint8_t pixel_width, void *data);

void sh8601a_display_dma_isr(void);

#endif  // __DRIVER_SH8601A_H
