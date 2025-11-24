#ifndef __DRIVER_ICNA3310_H
#define __DRIVER_ICNA3310_H

#include <stdint.h>

void icna3310_init(void);

void icna3310_set_window(uint16_t x_s, uint16_t x_e, uint16_t y_s, uint16_t y_e);

void icna3310_display(uint32_t pixel_count, uint8_t pixel_width, void *data);

void icna3310_display_dma(uint32_t pixel_count, uint8_t pixel_width, void *data);

void icna3310_display_dma_isr(void);

#endif  // __DRIVER_ICNA3310_H
