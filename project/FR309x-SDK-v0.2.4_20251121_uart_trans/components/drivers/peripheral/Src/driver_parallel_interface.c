/*
  ******************************************************************************
  * @file    driver_parallel_interface.c
  * @author  FreqChip Firmware Team
  * @version V1.0.0
  * @date    2021
  * @brief   parallel module driver.
  *          This file provides firmware functions to manage the 
  *          parallel interface 8080/6800 peripheral.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 FreqChip.
  * All rights reserved.
  ******************************************************************************
*/
#include "fr30xx.h"

/*********************************************************************
 * @fn      parallel_init
 *
 * @brief   Initialize the parallel_nterface according to the specified parameters
 *          in the str_ParallelParam_t 
 *
 * @param   ParalleInit : pointer to a str_ParallelParam_t structure that contains
 *                        the configuration information for LCD module
 *          
 * @return  None.
 */
void parallel_init(PARALLEL_HandTypeDef *hparallel)
{
    /* FIFO_RST */
    __PARALLEL_TX_FIFO_RESET(hparallel->PARALLELx);    
    /* DMA Config */
    hparallel->PARALLELx->DMA.DMA_TX_LEVEL = 16;
    hparallel->PARALLELx->DMA.DMA_ENABLE   = 1;
    /* select 8080 or 6800 */
    hparallel->PARALLELx->INTF_CFG.MODE = hparallel->Init.ParallelMode;
    /* select 8bit or 16bit */
    hparallel->PARALLELx->INTF_CFG.PARA_WIDTH = hparallel->Init.DataBusSelect;
    /* Write Clock DIV */
    hparallel->PARALLELx->CRM.WRITE_CLK_CFG = hparallel->Init.WriteClock;
    /* Read Clock DIV */
    hparallel->PARALLELx->CRM.READ_CLK_CFG = hparallel->Init.ReadClock;
    /* FIFO_RELEASE */
    __PARALLEL_TX_FIFO_RELEASE(hparallel->PARALLELx);
}

/*********************************************************************
 * @fn      Parallel_write_cmd
 *
 * @brief   Sending command
 *
 * @param   fp8_CMD : command data
 *  
 * @return  None.
 */
void Parallel_write_cmd(PARALLEL_HandTypeDef *hparallel, uint8_t fp8_CMD)
{
    /* Write, CMD */
    __PARALLEL_WR_CMD(hparallel->PARALLELx, fp8_CMD);
    /* wait bus idle */
    while(__PARALLEL_IS_BUS_BUSY(hparallel->PARALLELx)); 
}

/*********************************************************************
 * @fn      Parallel_write_param
 *
 * @brief   Sending parameter.
 *
 * @param   fu16_Data : parameter. Can be 8 bit or 16 bit, depend on BUS bit.
 *
 * @return  None.
 */
void Parallel_write_param(PARALLEL_HandTypeDef *hparallel, uint16_t fu16_Data)
{   
    /* Write, param */
    __PARALLEL_WR_PARAM(hparallel->PARALLELx, fu16_Data);
    /* wait bus idle */
    while(__PARALLEL_IS_BUS_BUSY(hparallel->PARALLELx));   
}

/*********************************************************************
 * @fn      Parallel_write_data
 *
 * @brief   Sending data or parameters
 *
 * @param   fp32_WriteBuffer : Write data buffer
 *          fu32_WriteNum    : transmit number.
 *                             1. select DATA_BUS_8_BIT,  1 count sent 1 byte
 *                             2. select DATA_BUS_16_BIT, 1 count sent 2 byte
 * @return  None.
 */
void Parallel_write_data(PARALLEL_HandTypeDef *hparallel, uint32_t *fp32_WriteBuffer, uint32_t fu32_WriteNum)
{
    uint32_t i;
    uint32_t lu32_Num;

    hparallel->PARALLELx->DATA_WR_LEN = fu32_WriteNum;
    
    /* 8 bit bus */
    if (hparallel->PARALLELx->INTF_CFG.PARA_WIDTH == DATA_BUS_8_BIT) 
    {
        lu32_Num = fu32_WriteNum / 4;
        
        if (fu32_WriteNum % 4) 
        {
            lu32_Num++;
        }
    }
    /* 16 bit bus */
    else 
    {
        lu32_Num = fu32_WriteNum / 2;
        
        if (fu32_WriteNum % 2) 
        {
            lu32_Num++;
        }
    }
    
    while(lu32_Num >= PARALLEL_FIFO_DEPTH)
    {
        uint32_t u32_l = PARALLEL_FIFO_DEPTH;
            
        while(u32_l--)
        {
            hparallel->PARALLELx->TX_FIFO = *fp32_WriteBuffer++;
        }
        
        lu32_Num -= PARALLEL_FIFO_DEPTH;  
            
        while(!(__PARALLEL_INT_STATUS(hparallel->PARALLELx)&INT_TXFIFO_EMPTY));   
    }
    while(lu32_Num--)
    {
        hparallel->PARALLELx->TX_FIFO = *fp32_WriteBuffer++;        
    }
    /* wait bus idle */
    while(__PARALLEL_IS_BUS_BUSY(hparallel->PARALLELx)); 
}

/*********************************************************************
 * @fn      Parallel_read_data_8bit
 *
 * @brief   read data. select DATA_BUS_8_BIT, 1 count receive 1 byte.
 *
 * @param   fu8_Param      : read Param
 *          fp8_ReadBuffer : read data buffer.
 *          fu32_ReadNum   : receive number.
 * 
 * @return  None.
 */
void Parallel_read_data_8bit(PARALLEL_HandTypeDef *hparallel, uint8_t fu8_Param, uint8_t *fp8_ReadBuffer, uint32_t fu32_ReadNum)
{
    uint32_t i;   

    __PARALLEL_WR_PARAM(hparallel->PARALLELx, fu8_Param);    
    /* wait bus idle */
    while(__PARALLEL_IS_BUS_BUSY(hparallel->PARALLELx)); 
    
    for (i = 0; i < fu32_ReadNum; i++)
    {
        /* Read  REQ*/
        __PARALLEL_RD_REQ(hparallel->PARALLELx);
        /* wait bus idle */
        while(__PARALLEL_IS_BUS_BUSY(hparallel->PARALLELx)); 

        fp8_ReadBuffer[i] = hparallel->PARALLELx->DAT_RD;
    }
}

/*********************************************************************
 * @fn      Parallel_read_data_16bit
 *
 * @brief   read data. select DATA_BUS_16_BIT, 1 count receive 2 byte.
 *
 * @param   fu8_Param       : read Param
            fp16_ReadBuffer : read data buffer.
 *          fu32_ReadNum    : receive number.
 * 
 * @return  None.
 */
void Parallel_read_data_16bit(PARALLEL_HandTypeDef *hparallel, uint8_t fu8_Param, uint16_t *fp16_ReadBuffer, uint32_t fu32_ReadNum)
{
    uint32_t i;
    
    __PARALLEL_WR_PARAM(hparallel->PARALLELx, fu8_Param);
    
    /* wait bus idle */
    while(__PARALLEL_IS_BUS_BUSY(hparallel->PARALLELx)); 
    
    for (i = 0; i < fu32_ReadNum; i++)
    {
        /* Read  REQ*/
        __PARALLEL_RD_REQ(hparallel->PARALLELx);
        /* wait bus idle */
        while(__PARALLEL_IS_BUS_BUSY(hparallel->PARALLELx)); 

        fp16_ReadBuffer[i] = hparallel->PARALLELx->DAT_RD;
    }
}
