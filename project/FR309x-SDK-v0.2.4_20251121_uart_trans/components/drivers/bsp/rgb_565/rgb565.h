/*
  ******************************************************************************
  * @file    rgb565.h
  * @author  FreqChip Firmware Team
  * @version V1.0.0
  * @date    2023
  * @brief   rgb565 Config header file.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 FreqChip.
  * All rights reserved.
  ******************************************************************************
*/
#ifndef __RGB565_H__
#define __RGB565_H__

#include "fr30xx.h"
#include "rgb_demo.h"

extern SPI_HandleTypeDef    spi_handle;
extern DMA_HandleTypeDef    dma_handle;
extern PARALLEL_HandTypeDef hparallel;

typedef struct
{
    uint32_t VerticalSignalCount;

    unsigned char *rgb_TxData;

}struct_RGB_TypeDef_t;

#define  RGB_ROW         480
#define  RGB_COLUMN      480

#define __RGB_LCD_DENABLE_SET()                             rgb_lcd_enable_release()  
#define __RGB_LCD_DENABLE_RELEASE()                         rgb_lcd_enable_set() 

#define __RGB_LCD_VSYNC_SET()                               rgb_lcd_vsync_set() 
#define __RGB_LCD_VSYNC__RELEASE()                          rgb_lcd_vsync__release() 
                                                            
#define __RGB_LCD_HSYNC_SET()                               rgb_lcd_hsync_set() 
#define __RGB_LCD_HSYNC__RELEASE()                          rgb_lcd_hsync__release() 
                                                            
#define __RGB_LCD_RESET_SET()                               rgb_lcd_reset_set() 
#define __RGB_LCD_RESET_RELEASE()                           rgb_lcd_reset_release() 
                                                            
#define __RGB_SPI_CS_RELEASE()                              rgb_spi_cs_release() 
#define __RGB_SPI_CS_SET()                                  rgb_spi_cs_set()

#define __8080_DATA_WR_LEN(__LEVEL__)                       __PARALLEL_SET_WR_LEN(hparallel.PARALLELx,__LEVEL__)
#define __8080_TXFIFO_EMPTY()                               __PARALLEL_INT_STATUS(hparallel.PARALLELx)&INT_TXFIFO_EMPTY
#define __8080_WRITE_BLANK()                                hparallel.PARALLELx->TX_FIFO = 0

#define __TIMER_CLEAR_IQR(__TIMERx__)                       timer_int_clear(__TIMERx__)
#define __TIMER_INIT(__TIMERx__, __LoadCount__)             timer_init(__TIMERx__, (24000 * __LoadCount__))
#define __TIMER_INT_ENABLE(__TIMERx__)                      timer_int_enable(__TIMERx__)
#define __TIMER_START(__TIMERx__)                           timer_start(__TIMERx__)

#define __DMA_GET_TFR_STATUS()                              dma_get_tfr_Status(&dma_handle)
#define __DMA_CLEAR_TFR_STATUS()                            dma_clear_tfr_Status(&dma_handle)
#define __DMA_TO_8080_START_IT(__BUFFER__,__SIZE__)         dma_start_IT(&dma_handle, (uint32_t)__BUFFER__, (uint32_t)&hparallel.PARALLELx->TX_FIFO, __SIZE__)  

#define __SPI_WRITE_DATA(__BUFFER__, __SIZE__)              spi_master_transmit_X1(&spi_handle, (void *)__BUFFER__, __SIZE__)


/* Exported functions --------------------------------------------------------*/

/* rgb_display_start */
/* rgb_timer_IRQHandler */
/* rgb_dma_IRQHandler */
void rgb_init(void);
void rgb_display_start(struct_Timer_t *TIMERx, struct_RGB_TypeDef_t *hrgb, uint32_t fps, unsigned char *Imagedata);
void rgb_timer_IRQHandler(struct_Timer_t *TIMERx, struct_RGB_TypeDef_t *hrgb);
void rgb_dma_IRQHandler(struct_RGB_TypeDef_t *hrgb);

#endif