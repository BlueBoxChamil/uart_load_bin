/*
  ******************************************************************************
  * @file    rgb565.c
  * @author  FreqChip Firmware Team
  * @version V1.0.0
  * @date    2023
  * @brief   rgb565 IC driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the 8080 SPI Timer and DMA norflash driver for rgb565.
  *           @ Initialization and de-initialization functions
  *           @ IO operation functions
  *           @ Peripheral Control functions
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 FreqChip.
  * All rights reserved.
  ******************************************************************************
*/
#include "rgb565.h"

static void rgb_reg_writer(uint16_t *fp_data, uint32_t size)
{
    __RGB_SPI_CS_SET(); 

    /* fp_Data[0] stores command, while others store data
       Bit8 set to 1 as data, otherwise it is a command
    */
    for(int i = 1; i < size; i++)
    {
      fp_data[i] = fp_data[i] + 0x100;
    }
    
    __SPI_WRITE_DATA(fp_data,size); 
    
    __RGB_SPI_CS_RELEASE();
}
static void rgb_idle_clock(uint32_t count)
{
    /* 8080 hits idle Clocks */
    __8080_DATA_WR_LEN(count);
    for(int i = 0; i < count/2; i++)
        __8080_WRITE_BLANK();
}

static void Line_porch_set(uint32_t count)
{
    for(int i = 0; i < count; i++)
    {
        __RGB_LCD_HSYNC__RELEASE();
        __RGB_LCD_DENABLE_RELEASE();
        rgb_idle_clock(4);

        __RGB_LCD_HSYNC_SET();
        rgb_idle_clock(RGB_ROW);
    }
}

static void Vertical_back_porch_set(void)
{
    __RGB_LCD_HSYNC__RELEASE();
    __RGB_LCD_DENABLE_SET(); 
    rgb_idle_clock(4);
    
    __RGB_LCD_HSYNC_SET();
    
    rgb_idle_clock(4);   

    while(!(__8080_TXFIFO_EMPTY()));
}

static void Vertical_front_porch_set(void)
{
    __RGB_LCD_HSYNC_SET();
    __RGB_LCD_DENABLE_SET();

    rgb_idle_clock(4); 

    while(!(__8080_TXFIFO_EMPTY()));
}

void rgb_init(void)
{
    uint16_t WBuffer[20];
    
    __RGB_LCD_RESET_RELEASE();
    system_delay_us(50); 
    __RGB_LCD_RESET_SET();   
    system_delay_us(1000 * 120); 
    
    WBuffer[0] = 0xFF;
    WBuffer[1] = 0x77;
    WBuffer[2] = 0x01;
    WBuffer[3] = 0x00;
    WBuffer[4] = 0x00;
    WBuffer[5] = 0x13;
    rgb_reg_writer(WBuffer,6); 
   
    WBuffer[0] = 0xEF;
    WBuffer[1] = 0x08;    
    rgb_reg_writer(WBuffer,2);   

    WBuffer[0] = 0xFF;
    WBuffer[1] = 0x77;
    WBuffer[2] = 0x01;
    WBuffer[3] = 0x00;
    WBuffer[4] = 0x00;
    WBuffer[5] = 0x10;    
    rgb_reg_writer(WBuffer,6);  

    WBuffer[0] = 0xC0;
    WBuffer[1] = 0x3B;
    WBuffer[2] = 0x00;
    rgb_reg_writer(WBuffer,3);

    WBuffer[0] = 0xC1;
    WBuffer[1] = 0x0B;
    WBuffer[2] = 0x02;
    rgb_reg_writer(WBuffer,3); 

    WBuffer[0] = 0xC2;
    WBuffer[1] = 0x00;
    WBuffer[2] = 0x02;
    rgb_reg_writer(WBuffer,3);  
    
    WBuffer[0] = 0xCC;
    WBuffer[1] = 0x10;    
    rgb_reg_writer(WBuffer,2); 
    
    WBuffer[0] = 0xB0;
    WBuffer[1] = 0x40;
    WBuffer[2] = 0x06;
    WBuffer[3] = 0x4F;
    WBuffer[4] = 0x01;
    WBuffer[5] = 0x04;  
    WBuffer[6] = 0x00;  
    WBuffer[7] = 0x00; 
    WBuffer[8] = 0x00; 
    WBuffer[9] = 0x06; 
    WBuffer[10] = 0x1D;  
    WBuffer[11] = 0x00;  
    WBuffer[12] = 0x0C;
    WBuffer[13] = 0x0C; 
    WBuffer[14] = 0x69;  
    WBuffer[15] = 0x36; 
    WBuffer[16] = 0x24;     
    rgb_reg_writer(WBuffer,17);   
    
    WBuffer[0] = 0xB1;
    WBuffer[1] = 0x40;
    WBuffer[2] = 0x1B;
    WBuffer[3] = 0x64;
    WBuffer[4] = 0x16;
    WBuffer[5] = 0x19;  
    WBuffer[6] = 0x0A;  
    WBuffer[7] = 0x07; 
    WBuffer[8] = 0x0C; 
    WBuffer[9] = 0x0B; 
    WBuffer[10] = 0x30;  
    WBuffer[11] = 0x0A;  
    WBuffer[12] = 0x1B;
    WBuffer[13] = 0x17; 
    WBuffer[14] = 0x7E;  
    WBuffer[15] = 0x3B; 
    WBuffer[16] = 0x1A;      
    rgb_reg_writer(WBuffer,17); 

    WBuffer[0] = 0xFF;
    WBuffer[1] = 0x77;
    WBuffer[2] = 0x01;
    WBuffer[3] = 0x00;
    WBuffer[4] = 0x00; 
    WBuffer[5] = 0x11;     
    rgb_reg_writer(WBuffer,6);   

    WBuffer[0] = 0xB0;
    WBuffer[1] = 0x6D; 
    rgb_reg_writer(WBuffer,2); 

    WBuffer[0] = 0xB1;
    WBuffer[1] = 0x0D; 
    rgb_reg_writer(WBuffer,2); 

    WBuffer[0] = 0xB2;
    WBuffer[1] = 0x81; 
    rgb_reg_writer(WBuffer,2); 

    WBuffer[0] = 0xB3;
    WBuffer[1] = 0x80; 
    rgb_reg_writer(WBuffer,2); 

    WBuffer[0] = 0xB5;
    WBuffer[1] = 0x43; 
    rgb_reg_writer(WBuffer,2); 
    
    WBuffer[0] = 0xB7;
    WBuffer[1] = 0x85; 
    rgb_reg_writer(WBuffer,2);  

    WBuffer[0] = 0xB8;
    WBuffer[1] = 0x20; 
    rgb_reg_writer(WBuffer,2); 
    
    WBuffer[0] = 0xC1;
    WBuffer[1] = 0x78; 
    rgb_reg_writer(WBuffer,2);  

    WBuffer[0] = 0xC2;
    WBuffer[1] = 0x78; 
    rgb_reg_writer(WBuffer,2); 
    
    WBuffer[0] = 0xD0;
    WBuffer[1] = 0x88; 
    rgb_reg_writer(WBuffer,2); 

    WBuffer[0] = 0xE0;
    WBuffer[1] = 0x00; 
    WBuffer[2] = 0x00; 
    WBuffer[3] = 0x02;     
    rgb_reg_writer(WBuffer,4); 
    
    WBuffer[0] = 0xE1;
    WBuffer[1] = 0x03;
    WBuffer[2] = 0xA0;
    WBuffer[3] = 0x00;
    WBuffer[4] = 0x00;
    WBuffer[5] = 0x04;
    WBuffer[6] = 0xA0; 
    WBuffer[7] = 0x00;  
    WBuffer[8] = 0x00;   
    WBuffer[9] = 0x00;  
    WBuffer[10] = 0x20;    
    WBuffer[11] = 0x20;      
    rgb_reg_writer(WBuffer,12); 
    
    WBuffer[0] = 0xE2;
    WBuffer[1] = 0x00;
    WBuffer[2] = 0x00;
    WBuffer[3] = 0x00;
    WBuffer[4] = 0x00;
    WBuffer[5] = 0x00;
    WBuffer[6] = 0x00; 
    WBuffer[7] = 0x00;  
    WBuffer[8] = 0x00;   
    WBuffer[9] = 0x00;  
    WBuffer[10] = 0x00;    
    WBuffer[11] = 0x00; 
    WBuffer[12] = 0x00; 
    WBuffer[13] = 0x00;     
    rgb_reg_writer(WBuffer,14); 
    
    WBuffer[0] = 0xE3;
    WBuffer[1] = 0x00;
    WBuffer[2] = 0x00;
    WBuffer[3] = 0x11;
    WBuffer[4] = 0x00;     
    rgb_reg_writer(WBuffer,5); 
     
    WBuffer[0] = 0xE4;
    WBuffer[1] = 0x22;
    WBuffer[2] = 0x00;    
    rgb_reg_writer(WBuffer,3);
    
    WBuffer[0] = 0xE5;
    WBuffer[1] = 0x05;
    WBuffer[2] = 0xEC;
    WBuffer[3] = 0xA0;
    WBuffer[4] = 0xA0;
    WBuffer[5] = 0x07;
    WBuffer[6] = 0xEE; 
    WBuffer[7] = 0xA0;  
    WBuffer[8] = 0xA0;   
    WBuffer[9] = 0x00;  
    WBuffer[10] = 0x00;    
    WBuffer[11] = 0x00; 
    WBuffer[12] = 0x00; 
    WBuffer[13] = 0x00; 
    WBuffer[14] = 0x00;
    WBuffer[15] = 0x00;
    WBuffer[16] = 0x00;    
    rgb_reg_writer(WBuffer,17); 

    WBuffer[0] = 0xE6;
    WBuffer[1] = 0x00;
    WBuffer[2] = 0x00;
    WBuffer[3] = 0x11;
    WBuffer[4] = 0x00;   
    rgb_reg_writer(WBuffer,5); 
    
    WBuffer[0] = 0xE7;
    WBuffer[1] = 0x22;
    WBuffer[2] = 0x00;  
    rgb_reg_writer(WBuffer,3);
    
    WBuffer[0] = 0xE8;
    WBuffer[1] = 0x06;
    WBuffer[2] = 0xED;
    WBuffer[3] = 0xA0;
    WBuffer[4] = 0xA0;
    WBuffer[5] = 0x08;
    WBuffer[6] = 0xEF; 
    WBuffer[7] = 0xA0;  
    WBuffer[8] = 0xA0;   
    WBuffer[9] = 0x00;  
    WBuffer[10] = 0x00;    
    WBuffer[11] = 0x00; 
    WBuffer[12] = 0x00; 
    WBuffer[13] = 0x00; 
    WBuffer[14] = 0x00;
    WBuffer[15] = 0x00;
    WBuffer[16] = 0x00;    
    rgb_reg_writer(WBuffer,17);
    
    WBuffer[0] = 0xEB;
    WBuffer[1] = 0x00;
    WBuffer[2] = 0x00;
    WBuffer[3] = 0x40;
    WBuffer[4] = 0x40;
    WBuffer[5] = 0x00;
    WBuffer[6] = 0x00; 
    WBuffer[7] = 0x00;      
    rgb_reg_writer(WBuffer,8); 

    WBuffer[0] = 0xED;
    WBuffer[1] = 0xFF;
    WBuffer[2] = 0xFF;
    WBuffer[3] = 0xFF;
    WBuffer[4] = 0xBA;
    WBuffer[5] = 0x0A;
    WBuffer[6] = 0xBF; 
    WBuffer[7] = 0x45;  
    WBuffer[8] = 0xFF;   
    WBuffer[9] = 0xFF;  
    WBuffer[10] = 0x54;    
    WBuffer[11] = 0xFB; 
    WBuffer[12] = 0xA0; 
    WBuffer[13] = 0xAB; 
    WBuffer[14] = 0xFF;
    WBuffer[15] = 0xFF;
    WBuffer[16] = 0xFF;    
    rgb_reg_writer(WBuffer,17);

    WBuffer[0] = 0xEF;
    WBuffer[1] = 0x08;
    WBuffer[2] = 0x08;
    WBuffer[3] = 0x08;
    WBuffer[4] = 0x45;
    WBuffer[5] = 0x3F;
    WBuffer[6] = 0x54;      
    rgb_reg_writer(WBuffer,7);  

    WBuffer[0] = 0xFF;
    WBuffer[1] = 0x77;
    WBuffer[2] = 0x01;
    WBuffer[3] = 0x00;
    WBuffer[4] = 0x00;
    WBuffer[5] = 0x13; 
    rgb_reg_writer(WBuffer,6);
    
    WBuffer[0] = 0xE8;
    WBuffer[1] = 0x00;
    WBuffer[2] = 0x0E;
    rgb_reg_writer(WBuffer,3); 
   
    WBuffer[0] = 0xFF;
    WBuffer[1] = 0x77;
    WBuffer[2] = 0x01;
    WBuffer[3] = 0x00;
    WBuffer[4] = 0x00;
    WBuffer[5] = 0x00;    
    rgb_reg_writer(WBuffer,6);
    
    WBuffer[0] = 0x3A;
    WBuffer[1] = 0x66;   
    rgb_reg_writer(WBuffer,2);   

    WBuffer[0] = 0x36;
    WBuffer[1] = 0x00;   
    rgb_reg_writer(WBuffer,2); 
    
    WBuffer[0] = 0x11;  
    rgb_reg_writer(WBuffer,1); 
      
    system_delay_us(120 * 1000); 

    WBuffer[0] = 0xFF;
    WBuffer[1] = 0x77;
    WBuffer[2] = 0x01;
    WBuffer[3] = 0x00;
    WBuffer[4] = 0x00;
    WBuffer[5] = 0x13;    
    rgb_reg_writer(WBuffer,6);
    
    WBuffer[0] = 0xE8;
    WBuffer[1] = 0x00;
    WBuffer[2] = 0x0C;    
    rgb_reg_writer(WBuffer,3);
 
    system_delay_us(10 * 1000);  

    WBuffer[0] = 0xE8;
    WBuffer[1] = 0x00;
    WBuffer[2] = 0x00;    
    rgb_reg_writer(WBuffer,3);
    
    WBuffer[0] = 0xFF;
    WBuffer[1] = 0x77;
    WBuffer[2] = 0x01;
    WBuffer[3] = 0x00;
    WBuffer[4] = 0x00;
    WBuffer[5] = 0x00;    
    rgb_reg_writer(WBuffer,6);
    
    WBuffer[0] = 0x29;  
    rgb_reg_writer(WBuffer,1);

    system_delay_us(50 * 1000);   
}

void rgb_display_start(struct_Timer_t *TIMERx, struct_RGB_TypeDef_t *hrgb, uint32_t fps, unsigned char *Imagedata)
{
    __TIMER_INIT(TIMERx, fps);
    __TIMER_INT_ENABLE(TIMERx);

    hrgb->rgb_TxData = Imagedata;
    hrgb->VerticalSignalCount = 0;

    __TIMER_START(TIMERx);
}


void rgb_timer_IRQHandler(struct_Timer_t *TIMERx, struct_RGB_TypeDef_t *hrgb)
{
    __TIMER_CLEAR_IQR(TIMERx);

    /* If the previous frame is not completed ,return */
    if(hrgb->VerticalSignalCount != 0)
    {
         return;
    }

    /* Vertical signal Start*/
    __RGB_LCD_VSYNC__RELEASE(); 
    Line_porch_set(4);
    __RGB_LCD_VSYNC_SET();
    Line_porch_set(8);

    /* set Vertical back porch signal */
    Vertical_back_porch_set();

    /* send First Row */
    __RGB_LCD_DENABLE_RELEASE(); 
    __8080_DATA_WR_LEN(RGB_ROW);
     
    __DMA_TO_8080_START_IT(&hrgb->rgb_TxData[0], RGB_ROW / 2);     
}

void rgb_dma_IRQHandler(struct_RGB_TypeDef_t *hrgb)
{
    if (__DMA_GET_TFR_STATUS())
    {
        __DMA_CLEAR_TFR_STATUS();

        hrgb->VerticalSignalCount++;
    }

    while(!(__8080_TXFIFO_EMPTY()));

    /* Row *Column End of one frame transmission */
    if(hrgb->VerticalSignalCount == RGB_COLUMN)
    {
        hrgb->VerticalSignalCount = 0;

        __RGB_LCD_VSYNC_SET();                
        
        Line_porch_set(8);  

        return;
    }

    /* set Vertical back porch signal and back porch signal */
    Vertical_front_porch_set();
    Vertical_back_porch_set();

    /* send next Row */
    __RGB_LCD_DENABLE_RELEASE(); 
    __8080_DATA_WR_LEN(RGB_ROW);
    __DMA_TO_8080_START_IT(&hrgb->rgb_TxData[RGB_ROW * hrgb->VerticalSignalCount * 2], RGB_ROW / 2);   
}

