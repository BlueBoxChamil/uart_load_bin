#ifndef __DRIVER_SH8601A_H
#define __DRIVER_SH8601A_H

#include <stdint.h>

void sh8601z_init(void);

void sh8601z_set_window(uint16_t x_s, uint16_t x_e, uint16_t y_s, uint16_t y_e);

void sh8601z_display(uint32_t pixel_count, uint8_t pixel_width, void *data);

void sh8601z_display_dma(uint32_t pixel_count, uint8_t pixel_width, void *data);

void sh8601z_power_off(void);

void sh8601z_power_on(void);

void sh8601z_display_dma_isr(void);

void sh8601z_brightness_adj(void);

#endif  // __DRIVER_SH8601A_H
