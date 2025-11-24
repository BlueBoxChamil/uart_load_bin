/*
  ******************************************************************************
  * @file    IC_W25Qxx.c
  * @author  FreqChip Firmware Team
  * @version V1.0.0
  * @date    2020
  * @brief   W25Qxx IC driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the spi norflash driver for W25Qxx.
  *           @ Initialization and de-initialization functions
  *           @ IO operation functions
  *           @ Peripheral Control functions
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020 FreqChip.
  * All rights reserved.
  ******************************************************************************
*/

#include "IC_W25Qxx.h"

static void (*read_callback)(void) = NULL;

/*********************************************************************************
* function    :  Read_IT_callback
* Description :  callback function used in read data with interrupt mode
* Input       :  
* Output      :  
* Author      :  Owen                  Data : 2022
**********************************************************************************/
static void Read_IT_callback(SPI_HandleTypeDef *hspi)
{
    __SPI_CS_Release();

    if (read_callback)
    {
        read_callback();
    }
}

/*********************************************************************************
* function    :  IC_W25Qxx_WriteEnable
* Description :  Write Enable
* Input       :  
* Output      :  
* Author      :  Chris_Kyle                  Data : 2020
**********************************************************************************/
void IC_W25Qxx_WriteEnable(void)
{
    uint8_t lu8_DataBuffer[1];

    lu8_DataBuffer[0] = WRITE_ENABLE;

    /* CS Select */
    __SPI_CS_Select();
    /* Send command */
    __SPI_Write_Data(lu8_DataBuffer, 1);
    /* CS Realse */
    __SPI_CS_Release();
}

/*********************************************************************************
* function   :  IC_W25Qxx_WriteDisable
* Description :  Write Disable
* Input       :  
* Output      :  
* Author      :  Chris_Kyle                  Data : 2020
**********************************************************************************/
void IC_W25Qxx_WriteDisable(void)
{
    uint8_t lu8_DataBuffer[1];

    lu8_DataBuffer[0] = WRITE_DISABLE;

    /* CS Select */
    __SPI_CS_Select();
    /* Send command */
    __SPI_Write_Data(lu8_DataBuffer, 1);
    /* CS Realse */
    __SPI_CS_Release();
}

/*********************************************************************************
* function   :  IC_W25Qxx_WriteRegister
* Description :  Write status register
* Input       :  
* Output      :  
* Author      :  Chris_Kyle                  Data : 2020
**********************************************************************************/
void IC_W25Qxx_WriteRegister(uint8_t fu8_Register_S7_S0, uint8_t fu8_Register_S15_S08)
{
    uint8_t lu8_DataBuffer[3];

    lu8_DataBuffer[0] = WRITE_STATUS_REGISTER;
    lu8_DataBuffer[1] = fu8_Register_S7_S0;
    lu8_DataBuffer[2] = fu8_Register_S15_S08;

    /* Write Enable */
    IC_W25Qxx_WriteEnable();

    /* CS Select */
    __SPI_CS_Select();
    /* Send command */
    __SPI_Write_Data(lu8_DataBuffer, 3);
    /* CS Realse */
    __SPI_CS_Release();

    /* Wait Write register End */
    IC_W25Qxx_WaitBusy();
}

/*********************************************************************************
* function   :  IC_W25Qxx_WriteHRegister
* Description :  Write high status register separately
* Input       :  
* Output      :  
* Author      :  Chris_Kyle                  Data : 2020
**********************************************************************************/
void IC_W25Qxx_WriteHRegister(uint8_t fu8_Register_S15_S08)
{
    uint8_t lu8_DataBuffer[2];

    lu8_DataBuffer[0] = WRITE_STATUS_H_REGISTER;
    lu8_DataBuffer[1] = fu8_Register_S15_S08;

    /* Write Enable */
    IC_W25Qxx_WriteEnable();

    /* CS Select */
    __SPI_CS_Select();
    /* Send command */
    __SPI_Write_Data(lu8_DataBuffer, 2);
    /* CS Realse */
    __SPI_CS_Release();

    /* Wait Write register End */
    IC_W25Qxx_WaitBusy();
}

/*********************************************************************************
* function   :  IC_W25Qxx_Read_ID
* Description :  Read Manufacture ID and Device ID
* Input       :  
* Output      :  Manufacture ID and Device ID
* Author      :  Chris_Kyle                  Data : 2020
**********************************************************************************/
uint32_t IC_W25Qxx_Read_ID(void)
{
    uint8_t lu8_DataBuffer[4];

    lu8_DataBuffer[0] = READ_ID;
    lu8_DataBuffer[1] = 0;
    lu8_DataBuffer[2] = 0;
    lu8_DataBuffer[3] = 0;

    /* CS Select */
    __SPI_CS_Select();
    /* Send command */
    __SPI_Write_Data(lu8_DataBuffer, 1);
    /* Recieve Manufacture ID and Device ID */
    __SPI_Read_Data(lu8_DataBuffer, 3);
    /* CS Realse */
    __SPI_CS_Release();

    return ((uint32_t)lu8_DataBuffer[0] << 16
            | (uint32_t)lu8_DataBuffer[1] << 8
            | (uint32_t)lu8_DataBuffer[2]);
}

/*********************************************************************************
* function   :  IC_W25Qxx_Read_RegisterS07_S00
* Description :  Read Status Register S07 ~ S00
* Input       :  
* Output      :  Status Register S07 ~ S00
* Author      :  Chris_Kyle                  Data : 2020
**********************************************************************************/
uint8_t IC_W25Qxx_Read_RegisterS07_S00(void)
{
    uint8_t lu8_DataBuffer[1];
    
    lu8_DataBuffer[0] = READ_STATUS_REGISTER_S07_S00;

    /* CS Select */
    __SPI_CS_Select();
    /* Send command */
    __SPI_Write_Data(lu8_DataBuffer, 1);
    /* Recieve Status Register S07 ~ S00 */
    __SPI_Read_Data(lu8_DataBuffer, 1);
    /* CS Realse */
    __SPI_CS_Release();
    
    return lu8_DataBuffer[0];
}

/*********************************************************************************
* function   :  IC_W25Qxx_Read_RegisterS15_S08
* Description :  Read Status Register S15 ~ S08
* Input       :  
* Output      :  Status Register S15 ~ S08
* Author      :  Chris_Kyle                  Data : 2020
**********************************************************************************/
uint8_t IC_W25Qxx_Read_RegisterS15_S08(void)
{
    uint8_t lu8_DataBuffer[1];
    
    lu8_DataBuffer[0] = READ_STATUS_REGISTER_S15_S08;

    /* CS Select */
    __SPI_CS_Select();
    /* Send command */
    __SPI_Write_Data(lu8_DataBuffer, 1);
    /* Recieve Status Register S15 ~ S08 */
    __SPI_Read_Data(lu8_DataBuffer, 1);
    /* CS Realse */
    __SPI_CS_Release();
    
    return lu8_DataBuffer[0];
}

/*********************************************************************************
* function   :  IC_W25Qxx_Read_Data
* Description :  Read Data
* Input       :  
* Output      :  
* Author      :  Chris_Kyle                  Data : 2020
**********************************************************************************/
void IC_W25Qxx_Read_Data(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length)
{
    uint8_t lu8_DataBuffer[4];

    lu8_DataBuffer[0] = READ_DATA;
    lu8_DataBuffer[1] = (uint8_t)(fu32_DataAddress >> 16 & 0xFF);
    lu8_DataBuffer[2] = (uint8_t)(fu32_DataAddress >> 8  & 0xFF);
    lu8_DataBuffer[3] = (uint8_t)(fu32_DataAddress >> 0  & 0xFF);

    /* CS Select */
    __SPI_CS_Select();
    /* Send command and Recieve Data */
    __SPI_Read_flash_X1(lu8_DataBuffer, 4, pu8_Buffer, fu32_Length);
    /* CS Realse */
    __SPI_CS_Release();
}

/*********************************************************************************
* function   :  IC_W25Qxx_Read_Data_IT
* Description :  Read Data
* Input       :  
* Output      :  
* Author      :  Chris_Kyle                  Data : 2020
**********************************************************************************/
void IC_W25Qxx_Read_Data_IT(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length)
{
    uint8_t lu8_DataBuffer[4];

    lu8_DataBuffer[0] = READ_DATA;
    lu8_DataBuffer[1] = (uint8_t)(fu32_DataAddress >> 16 & 0xFF);
    lu8_DataBuffer[2] = (uint8_t)(fu32_DataAddress >> 8  & 0xFF);
    lu8_DataBuffer[3] = (uint8_t)(fu32_DataAddress >> 0  & 0xFF);

    spi_flash_handle.RxCpltCallback = Read_IT_callback;

    /* CS Select */
    __SPI_CS_Select();
    /* Send command and Recieve Data */
    __SPI_Read_flash_X1_IT(lu8_DataBuffer, 4, pu8_Buffer, fu32_Length);
//    /* CS Realse */
//    __SPI_CS_Release();
}

/*********************************************************************************
* function   :  IC_W25Qxx_Read_Data_DMA
* Description :  Read Data
* Input       :  
* Output      :  
* Author      :  Chris_Kyle                  Data : 2020
**********************************************************************************/
void IC_W25Qxx_Read_Data_DMA(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length)
{
    uint8_t lu8_DataBuffer[4];

    lu8_DataBuffer[0] = READ_DATA;
    lu8_DataBuffer[1] = (uint8_t)(fu32_DataAddress >> 16 & 0xFF);
    lu8_DataBuffer[2] = (uint8_t)(fu32_DataAddress >> 8  & 0xFF);
    lu8_DataBuffer[3] = (uint8_t)(fu32_DataAddress >> 0  & 0xFF);

    /* CS Select */
    __SPI_CS_Select();
    /* Send command and Recieve Data */
    __SPI_Read_flash_X1_DMA(lu8_DataBuffer, 4, fu32_Length);
    dma_start_IT(&dma_flash_handle, (uint32_t)&spi_flash_handle.SPIx->DR, (uint32_t)pu8_Buffer, fu32_Length);
//    /* CS Realse */
//    __SPI_CS_Release();
}

/*********************************************************************************
* function   :  IC_W25Qxx_Read_Dual_Output
* Description :  Dual Output Fast Read
* Input       :  
* Output      :  
* Author      :  Chris_Kyle                  Data : 2020
**********************************************************************************/
void IC_W25Qxx_Read_Dual_Output(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length)
{
    spi_flash_handle.MultWireParam.Wire_X2X4X8       = Wire_X2;
    spi_flash_handle.MultWireParam.ReceiveWaitCycles = 8;
    spi_flash_handle.MultWireParam.InstructLength    = INST_8BIT;
    spi_flash_handle.MultWireParam.Instruct          = DUAL_OUTPUT_FAST_READ;
    spi_flash_handle.MultWireParam.AddressLength     = ADDR_24BIT;
    spi_flash_handle.MultWireParam.Address           = fu32_DataAddress;

    /* CS Select */
    __SPI_CS_Select();
    /* Send Data */
    __SPI_Read_Data_X2X4X8(pu8_Buffer, fu32_Length);
    /* CS Realse */
    __SPI_CS_Release();
}

/*********************************************************************************
* function   :  IC_W25Qxx_Read_Dual_Output_IT
* Description :  Dual Output Fast Read with interrupt mode
* Input       :  
* Output      :  
* Author      :  Chris_Kyle                  Data : 2020
**********************************************************************************/
void IC_W25Qxx_Read_Dual_Output_IT(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length)
{
    spi_flash_handle.MultWireParam.Wire_X2X4X8       = Wire_X2;
    spi_flash_handle.MultWireParam.ReceiveWaitCycles = 8;
    spi_flash_handle.MultWireParam.InstructLength    = INST_8BIT;
    spi_flash_handle.MultWireParam.Instruct          = DUAL_OUTPUT_FAST_READ;
    spi_flash_handle.MultWireParam.AddressLength     = ADDR_24BIT;
    spi_flash_handle.MultWireParam.Address           = fu32_DataAddress;

    spi_flash_handle.RxCpltCallback = Read_IT_callback;
    
    /* CS Select */
    __SPI_CS_Select();
    /* Send Data */
    __SPI_Read_Data_X2X4X8_IT(pu8_Buffer, fu32_Length);
//    /* CS Realse */
//    __SPI_CS_Release();
}

/*********************************************************************************
* function   :  IC_W25Qxx_Read_Dual_Output_DMA
* Description :  Dual Output Fast Read with DMA mode
* Input       :  
* Output      :  
* Author      :  Chris_Kyle                  Data : 2020
**********************************************************************************/
void IC_W25Qxx_Read_Dual_Output_DMA(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length)
{
    spi_flash_handle.MultWireParam.Wire_X2X4X8       = Wire_X2;
    spi_flash_handle.MultWireParam.ReceiveWaitCycles = 8;
    spi_flash_handle.MultWireParam.InstructLength    = INST_8BIT;
    spi_flash_handle.MultWireParam.Instruct          = DUAL_OUTPUT_FAST_READ;
    spi_flash_handle.MultWireParam.AddressLength     = ADDR_24BIT;
    spi_flash_handle.MultWireParam.Address           = fu32_DataAddress;

    /* CS Select */
    __SPI_CS_Select();
    /* Send Data */
    __SPI_Read_Data_X2X4X8_DMA(fu32_Length);
    dma_start_IT(&dma_flash_handle, (uint32_t)&spi_flash_handle.SPIx->DR, (uint32_t)pu8_Buffer, fu32_Length);
//    /* CS Realse */
//    __SPI_CS_Release();
}

/*********************************************************************************
* function   :  IC_W25Qxx_Read_Quad_Output
* Description :  Quad Output Fast Read
* Input       :  
* Output      :  
* Author      :  Chris_Kyle                  Data : 2020
**********************************************************************************/
void IC_W25Qxx_Read_Quad_Output(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length)
{
    spi_flash_handle.MultWireParam.Wire_X2X4X8       = Wire_X4;
    spi_flash_handle.MultWireParam.ReceiveWaitCycles = 8;
    spi_flash_handle.MultWireParam.InstructLength    = INST_8BIT;
    spi_flash_handle.MultWireParam.Instruct          = QUAD_OUTPUT_FAST_READ;
    spi_flash_handle.MultWireParam.AddressLength     = ADDR_24BIT;
    spi_flash_handle.MultWireParam.Address           = fu32_DataAddress;

    /* CS Select */
    __SPI_CS_Select();
    /* Send Data */
    __SPI_Read_Data_X2X4X8(pu8_Buffer, fu32_Length);
    /* CS Realse */
    __SPI_CS_Release();
}

/*********************************************************************************
* function   :  IC_W25Qxx_Read_Quad_Output_IT
* Description :  Quad Output Fast Read
* Input       :  
* Output      :  
* Author      :  Chris_Kyle                  Data : 2020
**********************************************************************************/
void IC_W25Qxx_Read_Quad_Output_IT(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length)
{
    spi_flash_handle.MultWireParam.Wire_X2X4X8       = Wire_X4;
    spi_flash_handle.MultWireParam.ReceiveWaitCycles = 8;
    spi_flash_handle.MultWireParam.InstructLength    = INST_8BIT;
    spi_flash_handle.MultWireParam.Instruct          = QUAD_OUTPUT_FAST_READ;
    spi_flash_handle.MultWireParam.AddressLength     = ADDR_24BIT;
    spi_flash_handle.MultWireParam.Address           = fu32_DataAddress;

    spi_flash_handle.RxCpltCallback = Read_IT_callback;

    /* CS Select */
    __SPI_CS_Select();
    /* Send Data */
    __SPI_Read_Data_X2X4X8_IT(pu8_Buffer, fu32_Length);
//    /* CS Realse */
//    __SPI_CS_Release();
}

/*********************************************************************************
* function   :  IC_W25Qxx_Read_Quad_Output_DMA
* Description :  Quad Output Fast Read
* Input       :  
* Output      :  
* Author      :  Chris_Kyle                  Data : 2020
**********************************************************************************/
void IC_W25Qxx_Read_Quad_Output_DMA(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length)
{
    spi_flash_handle.MultWireParam.Wire_X2X4X8       = Wire_X4;
    spi_flash_handle.MultWireParam.ReceiveWaitCycles = 8;
    spi_flash_handle.MultWireParam.InstructLength    = INST_8BIT;
    spi_flash_handle.MultWireParam.Instruct          = QUAD_OUTPUT_FAST_READ;
    spi_flash_handle.MultWireParam.AddressLength     = ADDR_24BIT;
    spi_flash_handle.MultWireParam.Address           = fu32_DataAddress;

    /* CS Select */
    __SPI_CS_Select();
    /* Send Data */
    __SPI_Read_Data_X2X4X8_DMA(fu32_Length);
    dma_start_IT(&dma_flash_handle, (uint32_t)&spi_flash_handle.SPIx->DR, (uint32_t)pu8_Buffer, fu32_Length);
//    /* CS Realse */
//    __SPI_CS_Release();
}

/*********************************************************************************
* function   :  IC_W25Qxx_PageProgram
* Description :  Page Program
* Input       :  
* Output      :  
* Author      :  Chris_Kyle                  Data : 2020
**********************************************************************************/
void IC_W25Qxx_PageProgram(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length)
{
    uint8_t lu8_DataBuffer[4];

    lu8_DataBuffer[0] = PAGE_PROGARM;
    lu8_DataBuffer[1] = (uint8_t)(fu32_DataAddress >> 16 & 0xFF);
    lu8_DataBuffer[2] = (uint8_t)(fu32_DataAddress >> 8  & 0xFF);
    lu8_DataBuffer[3] = (uint8_t)(fu32_DataAddress >> 0  & 0xFF);

    /* Write Enable */
    IC_W25Qxx_WriteEnable();

    /* CS Select */
    __SPI_CS_Select();
    /* Send command */
    __SPI_Write_Data(lu8_DataBuffer, 4);
    /* Send Data */
    __SPI_Write_Data(pu8_Buffer, fu32_Length);
    /* CS Realse */
    __SPI_CS_Release();

    /* Wait Erase End */
    IC_W25Qxx_WaitBusy();
}

/*********************************************************************************
* function   :  IC_W25Qxx_PageProgram_Quad
* Description :  Quad Page Program 
* Input       :  
* Output      :  
* Author      :  Chris_Kyle                  Data : 2020
**********************************************************************************/
void IC_W25Qxx_PageProgram_Quad(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length)
{
    spi_flash_handle.MultWireParam.Wire_X2X4X8    = Wire_X4;
    spi_flash_handle.MultWireParam.InstructLength = INST_8BIT;
    spi_flash_handle.MultWireParam.Instruct       = QUAD_PAGE_PROGRAM;
    spi_flash_handle.MultWireParam.AddressLength  = ADDR_24BIT;
    spi_flash_handle.MultWireParam.Address        = fu32_DataAddress;

    /* Write Enable */
    IC_W25Qxx_WriteEnable();

    /* CS Select */
    __SPI_CS_Select();
    /* Send Data */
    __SPI_Write_Data_X2X4X8(pu8_Buffer, fu32_Length);
    /* CS Realse */
    __SPI_CS_Release();

    /* Wait Erase End */
    IC_W25Qxx_WaitBusy();
}

/*********************************************************************************
* function   :  IC_W25Qxx_EraseSector
* Description :  Erease The specific Sector
* Input       :  
* Output      :  
* Author      :  Chris_Kyle                  Data : 2020
**********************************************************************************/
void IC_W25Qxx_EraseSector(uint32_t fu32_DataAddress)
{
    uint8_t lu8_DataBuffer[4];
    
    lu8_DataBuffer[0] = SECTOR_ERASE;
    lu8_DataBuffer[1] = (uint8_t)(fu32_DataAddress >> 16 & 0xFF);
    lu8_DataBuffer[2] = (uint8_t)(fu32_DataAddress >> 8  & 0xFF);
    lu8_DataBuffer[3] = (uint8_t)(fu32_DataAddress >> 0  & 0xFF);

    /* Write Enable */
    IC_W25Qxx_WriteEnable();
    
    /* CS Select */
    __SPI_CS_Select();
    /* Send command */
    __SPI_Write_Data(lu8_DataBuffer, 4);
    /* CS Realse */
    __SPI_CS_Release();

    /* Wait Erase End */
    IC_W25Qxx_WaitBusy();
}

/*********************************************************************************
* function   :  IC_W25Qxx_EraseChip
* Description :  Erease The Whole Chip
* Input       :  
* Output      :  
* Author      :  Chris_Kyle                  Data : 2020
**********************************************************************************/
void IC_W25Qxx_EraseChip(void)
{
    uint8_t lu8_DataBuffer[1];

    lu8_DataBuffer[0] = CHIP_ERASE;

    /* Write Enable */
    IC_W25Qxx_WriteEnable();

    /* CS Select */
    __SPI_CS_Select();
    /* Send command */
    __SPI_Write_Data(lu8_DataBuffer, 1);
    /* CS Realse */
    __SPI_CS_Release();

    /* Wait Erase End */
    IC_W25Qxx_WaitBusy();
}

/*********************************************************************************
* function   :  IC_W25Qxx_QuadConfig
* Description :  Quad Function Config
* Input       :  
* Output      :  
* Author      :  Chris_Kyle                  Data : 2020
**********************************************************************************/
void IC_W25Qxx_QuadConfig(bool fb_Config)
{
    uint8_t lu8_CurrentState;
    
    lu8_CurrentState = IC_W25Qxx_Read_RegisterS15_S08();

    if (fb_Config == true) 
    {
        /* Set W25Qxx Quad Enable */
        if ((lu8_CurrentState & REGISTER_S15_S08_QE) == 0)
        {
//            IC_W25Qxx_WriteRegister(REGISTER_NULL, REGISTER_S15_S08_QE);
            IC_W25Qxx_WriteHRegister(REGISTER_S15_S08_QE);
        }
    }
    else 
    {
        /* Set W25Qxx Quad Disable */
        if (lu8_CurrentState & REGISTER_S15_S08_QE)
        {
            IC_W25Qxx_WriteRegister(REGISTER_NULL, REGISTER_NULL);
        }
    }
}

/*********************************************************************************
* function   :  IC_W25Qxx_WaitBusy
* Description :  Wait IC Not Busy
* Input       :  
* Output      :  
* Author      :  Chris_Kyle                  Data : 2020
**********************************************************************************/
void IC_W25Qxx_WaitBusy(void)
{   
    /* Wait IC Not Busy */
    while(IC_W25Qxx_Read_RegisterS07_S00() & REGISTER_S07_S00_WIP);
}

/*********************************************************************************
* function   :  IC_W25Qxx_PowerDown
* Description :  
* Input       :  
* Output      :  
* Author      :  Chris_Kyle                  Data : 2020
**********************************************************************************/
void IC_W25Qxx_PowerDown(void)
{ 
    uint8_t lu8_DataBuffer[1];

    lu8_DataBuffer[0] = DEEP_POWER_DOWN;

    /* CS Select */
    __SPI_CS_Select();
    /* Send command */
    __SPI_Write_Data(lu8_DataBuffer, 1);
    /* CS Realse */
    __SPI_CS_Release();
}

/*********************************************************************************
* function   :  IC_W25Qxx_Wakeup
* Description :  
* Input       :  
* Output      :  
* Author      :  Chris_Kyle                  Data : 2020
**********************************************************************************/
void IC_W25Qxx_Wakeup(void)
{  
    uint8_t lu8_DataBuffer[1];
    
    lu8_DataBuffer[0] = RELEASE_FORM_DEEP_POWER_DOWN;

    /* CS Select */
    __SPI_CS_Select();
    /* Send command */
    __SPI_Write_Data(lu8_DataBuffer, 1);
    /* CS Realse */
    __SPI_CS_Release();
}

/*********************************************************************************
* function   :  IC_W25Qxx_Reset
* Description :  W25Qxx Reset
* Input       :  
* Output      :  
* Author      :  Chris_Kyle                  Data : 2020
**********************************************************************************/
void IC_W25Qxx_Reset(void)
{
    uint8_t lu8_DataBuffer[1];
    
    lu8_DataBuffer[0] = ENABLE_RESET;

    /* CS Select */
    __SPI_CS_Select();
    /* Send command */
    __SPI_Write_Data(lu8_DataBuffer, 1);
    /* CS Realse */
    __SPI_CS_Release();

    lu8_DataBuffer[0] = RESET;
    
    /* CS Select */
    __SPI_CS_Select();
    /* Send command */
    __SPI_Write_Data(lu8_DataBuffer, 1);
    /* CS Realse */
    __SPI_CS_Release();
}

/*********************************************************************************
* function   :  IC_W25Qxx_Set_Read_Callback
* Description :  used to set read callback when unblock mode (IT, DMA) is used
* Input       :  
* Output      :  
* Author      :  owen                  Data : 2022
**********************************************************************************/
void IC_W25Qxx_Set_Read_Callback(void (*cb)(void))
{
    read_callback = cb;
}

/*********************************************************************************
* function   :  IC_W25Qxx_Spi_Interrupt
* Description :  SPI interrupt handler
* Input       :  
* Output      :  
* Author      :  Chris_Kyle                  Data : 2020
**********************************************************************************/
__RAM_CODE void IC_W25Qxx_Spi_Interrupt(void)
{
    spi_master_IRQHandler(&spi_flash_handle);
}

/*********************************************************************************
* function   :  IC_W25Qxx_DMA_Interrupt
* Description :  DMA interrupt handler
* Input       :  
* Output      :  
* Author      :  Chris_Kyle                  Data : 2020
**********************************************************************************/
__RAM_CODE void IC_W25Qxx_DMA_Interrupt(void)
{
    if (dma_get_tfr_Status(&dma_flash_handle)) {
        dma_clear_tfr_Status(&dma_flash_handle);
        
        Read_IT_callback(NULL);
    }
    
    if (dma_get_error_Status(&dma_flash_handle)) {
        dma_clear_error_Status(&dma_flash_handle);
    }
}

void IC_W25Qxx_PageProgram_Dual(uint8_t *pu8_Buffer, uint32_t fu32_DataAddress, uint32_t fu32_Length)
{
    spi_flash_handle.MultWireParam.Wire_X2X4X8      = Wire_X2;
    spi_flash_handle.MultWireParam.InstructLength = INST_8BIT;
    spi_flash_handle.MultWireParam.Instruct       = 0xA2;
    spi_flash_handle.MultWireParam.AddressLength  = ADDR_24BIT;
    spi_flash_handle.MultWireParam.Address        = fu32_DataAddress;

    /* Write Enable */
    IC_W25Qxx_WriteEnable();

    /* CS Select */
    __SPI_CS_Select();
    
    /* Send Data */
    __SPI_Write_Data_X2X4X8(pu8_Buffer, fu32_Length);
    /* CS Realse */
    __SPI_CS_Release();

    /* Wait Erase End */
    IC_W25Qxx_WaitBusy();
}
