/*
  ******************************************************************************
  * @file    driver_display.c
  * @author  FreqChip Firmware Team
  * @version V1.0.0
  * @date    2022
  * @brief   display abstract interfase.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 FreqChip.
  * All rights reserved.
  ******************************************************************************
*/

#include <stdint.h>

//#define DISPLAY_TYPE_GC9C01
//#define DISPLAY_TYPE_JD9854
//#define DISPLAY_TYPE_SH8601A
//#define DISPLAY_TYPE_ICNA3310
#define DISPLAY_TYPE_SH8601Z

#include "driver_display.h"

#ifdef DISPLAY_TYPE_GC9C01
#include "driver_gc9c01.h"
#endif
#ifdef DISPLAY_TYPE_JD9854
#include "driver_jd9854.h"
#endif
#ifdef DISPLAY_TYPE_SH8601A
#include "driver_sh8601a.h"
#endif
#ifdef DISPLAY_TYPE_ICNA3310
#include "driver_icna3310.h"
#endif
#ifdef DISPLAY_TYPE_SH8601Z
#include "driver_sh8601z.h"
#endif

void display_init(void)
{
#ifdef DISPLAY_TYPE_GC9C01
    gc9c01_init();
#endif
#ifdef DISPLAY_TYPE_JD9854
    jd9854_init();
#endif
#ifdef DISPLAY_TYPE_SH8601A
    sh8601a_init();
#endif
#ifdef DISPLAY_TYPE_ICNA3310
    icna3310_init();
#endif
#ifdef DISPLAY_TYPE_SH8601Z
    sh8601z_init();
#endif
}

void display_set_window(uint16_t x_s, uint16_t x_e, uint16_t y_s, uint16_t y_e)
{
#ifdef DISPLAY_TYPE_GC9C01
    gc9c01_set_window(x_s, x_e, y_s, y_e);
#endif
#ifdef DISPLAY_TYPE_JD9854
    jd9854_set_window(x_s, x_e, y_s, y_e);
#endif
#ifdef DISPLAY_TYPE_SH8601A
    sh8601a_set_window(x_s, x_e, y_s, y_e);
#endif
#ifdef DISPLAY_TYPE_ICNA3310
    icna3310_set_window(x_s, x_e, y_s, y_e);
#endif
#ifdef DISPLAY_TYPE_SH8601Z
    sh8601z_set_window(x_s, x_e, y_s, y_e);
#endif
}

void display_update(uint32_t pixel_count, uint8_t pixel_width, void *data)
{
#ifdef DISPLAY_TYPE_GC9C01
    gc9c01_display(pixel_count, pixel_width, data);
#endif
#ifdef DISPLAY_TYPE_JD9854
    jd9854_display(pixel_count, pixel_width, data);
#endif
#ifdef DISPLAY_TYPE_SH8601A
    sh8601a_display(pixel_count, pixel_width, data);
#endif
#ifdef DISPLAY_TYPE_ICNA3310
    icna3310_display(pixel_count, pixel_width, data);
#endif
#ifdef DISPLAY_TYPE_SH8601Z
    sh8601z_display(pixel_count, pixel_width, data);
#endif
}

void display_update_dma(uint32_t pixel_count, uint8_t pixel_width, void *data)
{
#ifdef DISPLAY_TYPE_GC9C01
    gc9c01_display_dma(pixel_count, pixel_width, data);
#endif
#ifdef DISPLAY_TYPE_JD9854
    jd9854_display_dma(pixel_count, pixel_width, data);
#endif
#ifdef DISPLAY_TYPE_SH8601A
    sh8601a_display_dma(pixel_count, pixel_width, data);
#endif
#ifdef DISPLAY_TYPE_ICNA3310
    icna3310_display_dma(pixel_count, pixel_width, data);
#endif
#ifdef DISPLAY_TYPE_SH8601Z
    sh8601z_display_dma(pixel_count, pixel_width, data);
#endif
}

void display_power_off(void)
{
#ifdef DISPLAY_TYPE_SH8601Z
    sh8601z_power_off();
#endif
}

void display_power_on(void)
{
#ifdef DISPLAY_TYPE_SH8601Z
    sh8601z_power_on();
#endif
}

void display_brightness_adj(void)
{
#ifdef DISPLAY_TYPE_SH8601Z
    sh8601z_brightness_adj();
#endif
}

void display_update_dma_isr(void)
{
#ifdef DISPLAY_TYPE_GC9C01
    gc9c01_display_dma_isr();
#endif
#ifdef DISPLAY_TYPE_JD9854
    jd9854_display_dma_isr();
#endif
#ifdef DISPLAY_TYPE_SH8601A
    sh8601a_display_dma_isr();
#endif
#ifdef DISPLAY_TYPE_ICNA3310
    icna3310_display_dma_isr();
#endif
#ifdef DISPLAY_TYPE_SH8601Z
    sh8601z_display_dma_isr();
#endif
}
