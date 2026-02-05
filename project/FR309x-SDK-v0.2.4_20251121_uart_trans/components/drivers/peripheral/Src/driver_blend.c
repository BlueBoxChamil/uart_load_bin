/*
  ******************************************************************************
  * @file    driver_blend.c
  * @author  FreqChip Firmware Team
  * @version V1.0.0
  * @date    2022
  * @brief   BLEND module driver.
  *          This file provides firmware functions to manage the 
  *          BLEND peripheral
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 FreqChip.
  * All rights reserved.
  ******************************************************************************
*/
#include "fr30xx.h"


/************************************************************************************
 * @fn      blend_IRQHandler
 *
 * @brief   Handle BLEND interrupt request.
 *
 * @param   hblend: BLEND handle.
 */
void blend_IRQHandler(BLEND_HandleTypeDef *hblend)
{
    uint32_t isr_status;
    isr_status = BLEND_AHB0->ISR_STATUS.Word;
    /*Out image interrupt handle*/
    if(BLEND_AHB0->ISR_STATUS.Bits.ORGB_ALFULL_INT_STS == 1)
    {
        while(BLEND_AHB0->FIFO_STATUS.Bits.ORGB_EMPTY == 0)
        {
            if(hblend->DataOutIndex >= hblend->OutSize)
            {
                __BLEND_INT_ORGB_FIFO_ALFULL_DISABLE();
                if(hblend->Callback)
                {
                    hblend->Callback(hblend);
                }
                return;
            }
            hblend->OutRgb[hblend->DataOutIndex++] = BLEND_AHB1->ORGB_DATA;
        }
    }
    /*Src image interrupt handle*/
    if(isr_status & BLEND_RGB0_EMPTY_INT_STS)
    {
        while(!__RGB0_FIFO_IS_ALFULL())
        {
            BLEND_AHB0->RGB0_DATA = hblend->SrcRgb[hblend->SrcInIndex++];
            if(hblend->SrcInIndex >= hblend->SrcSize)
            {
                __BLEND_INT_RGB0_FIFO_EMPTY_DISABLE();
                break;
            }
        }
    }
    
    /*Back image interrupt handle*/
    if(isr_status & BLEND_RGB1_EMPTY_INT_STS)
    {
        while(!__RGB1_FIFO_IS_ALFULL())
        {
            BLEND_AHB0->RGB1_DATA = hblend->BackRgb[hblend->BackInIndex++];
            if(hblend->BackInIndex >= hblend->BackSize)
            {
                __BLEND_INT_RGB1_FIFO_EMPTY_DISABLE();
                break;
            }
        }
    }
    
    /*Mask image interrupt handle*/
    if(isr_status & BLEND_MASK_EMPTY_INT_STS)
    {
        while(!__MASK_FIFO_IS_ALFULL())
        {
            BLEND_AHB1->MASK_DATA = hblend->MaskRgb[hblend->MaskInIndex++];
            if(hblend->MaskInIndex >= hblend->MaskSize)
            {
                __BLEND_INT_MASK_FIFO_EMPTY_DISABLE();
                break;
            }
        }
    }
}

/******************************************************************************
 * @fn      blend_init
 *
 * @brief   Initialize the BLEND according to the specified parameters
 *          in the BLEND_HandleTypeDef 
 *
 * @param   hblend : BLEND_HandleTypeDef structure that contains the
 *                      configuration information for blend module.
 */
void blend_init(BLEND_HandleTypeDef *hblend)
{
    /*general config*/
    BLEND_AHB0->CONFIG.Bits.BLEND_MODE   = hblend->BlendInit.Bits.BLEND_MODE;
    BLEND_AHB0->CONFIG.Bits.ORGB_FORMAT  = hblend->BlendInit.Bits.OUTRGB_FORMAT;
    BLEND_AHB0->CONFIG.Bits.RGB0_FORMAT  = hblend->BlendInit.Bits.INRGB0_FORMAT;
    BLEND_AHB0->CONFIG.Bits.RGB1_FORMAT  = hblend->BlendInit.Bits.INRGB1_FORMAT;
    BLEND_AHB0->SRC_ALPHA.Bits.SRC_ALPHA = hblend->BlendInit.Bits.SRC_ALPHA;
    BLEND_AHB0->CONFIG.Bits.MASK_CAL_EN = hblend->BlendInit.Bits.MASK_CAL_EN;
    BLEND_AHB0->CONFIG.Bits.RGB0_CAL_EN = 1;
    BLEND_AHB0->CONFIG.Bits.RGB1_CAL_EN = 1;
    
    BLEND_AHB0->CTRL.Bits.MASK_FF_RST = 1;
    BLEND_AHB0->CTRL.Bits.ORGB_FF_RST = 1;
    BLEND_AHB0->CTRL.Bits.RGB0_FF_RST = 1;
    BLEND_AHB0->CTRL.Bits.RGB1_FF_RST = 1;
    
    BLEND_AHB0->CONFIG.Bits.R_MUL_TC_EN      = 1;
    BLEND_AHB0->CONFIG.Bits.G_MUL_TC_EN      = 1;
    BLEND_AHB0->CONFIG.Bits.B_MUL_TC_EN      = 1;
    BLEND_AHB0->CONFIG.Bits.A_MUL_TC_EN      = 1;
    BLEND_AHB0->CONFIG.Bits.ORGB_565_R_TC_EN = 1;
    BLEND_AHB0->CONFIG.Bits.ORGB_565_G_TC_EN = 1;
    BLEND_AHB0->CONFIG.Bits.ORGB_565_B_TC_EN = 1;
    BLEND_AHB0->CONFIG.Bits.ORGB_332_R_TC_EN = 1;
    BLEND_AHB0->CONFIG.Bits.ORGB_332_G_TC_EN = 1;
    BLEND_AHB0->CONFIG.Bits.ORGB_332_B_TC_EN = 1;
    BLEND_AHB0->PIXEL_THR.Bits.O565_B_THR    = 4;
    BLEND_AHB0->PIXEL_THR.Bits.O565_G_THR    = 2;
    BLEND_AHB0->PIXEL_THR.Bits.O565_R_THR    = 4;
    BLEND_AHB0->PIXEL_THR.Bits.O332_B_THR    = 32;
    BLEND_AHB0->PIXEL_THR.Bits.O332_G_THR    = 16;
    BLEND_AHB0->PIXEL_THR.Bits.O332_R_THR    = 16; 

    /*some option select to config*/
    if(hblend->BlendInit.Bits.SRC_IS_PURE == 1)
    {
        BLEND_AHB0->PIXEL.Bits.PIXEL_ALPHA = hblend->BlendInit.Bits.SRC_ALPHA;
        BLEND_AHB0->PIXEL.Bits.PIXEL_DATA  = hblend->Pure.Bits.PIXEL;
        BLEND_AHB0->CONFIG.Bits.ORGB_MODE  = 0;
    }
    else
    {
        BLEND_AHB0->CONFIG.Bits.ORGB_MODE  = 1;
    }

    if(hblend->BlendInit.Bits.DMA_IN_EN == 1)
    {
        BLEND_AHB0->DMAC.Bits.RGB0_DMA_EN = 1;
        BLEND_AHB0->DMAC.Bits.RGB1_DMA_EN = 1;
        BLEND_AHB0->DMAC.Bits.MASK_DMA_EN = 1;
    }
    if(hblend->BlendInit.Bits.DMA_OUT_EN == 1)
    {
        BLEND_AHB0->DMAC.Bits.ORGB_DMA_EN = 1;
    }
    
    /*fifo threshold config*/
    BLEND_AHB0->INFIFO0_THR_CTL.Bits.RGB0_ALEMPTY_THR     = hblend->Infifo0Thr.Bits.ALEMPTY_THR;
    BLEND_AHB0->INFIFO0_THR_CTL.Bits.RGB0_ALFULL_THR      = 30;
    BLEND_AHB0->INFIFO0_THR_CTL.Bits.RGB0_DMA_THR         = hblend->Infifo0Thr.Bits.DMA_THR;
    
    BLEND_AHB0->INFIFO1_THR_CTL.Bits.RGB1_ALEMPTY_THR     = hblend->Infifo1Thr.Bits.ALEMPTY_THR;
    BLEND_AHB0->INFIFO1_THR_CTL.Bits.RGB1_ALFULL_THR      = 30;
    BLEND_AHB0->INFIFO1_THR_CTL.Bits.RGB1_DMA_THR         = hblend->Infifo1Thr.Bits.DMA_THR;

    BLEND_AHB0->INFIFOMASK_THR_CTL.Bits.MASK_ALEMPTY_THR  = hblend->InfifoMaskThr.Bits.ALEMPTY_THR;
    BLEND_AHB0->INFIFOMASK_THR_CTL.Bits.MASK_ALFULL_THR   = 30;
    BLEND_AHB0->INFIFOMASK_THR_CTL.Bits.MASK_DMA_THR      = hblend->InfifoMaskThr.Bits.DMA_THR;

    BLEND_AHB0->OUTFIFO_THR_CTL.Bits.ORGB_ALFULL_THR      = hblend->OutfifoThr.Bits.ALFULL_THR;
    BLEND_AHB0->OUTFIFO_THR_CTL.Bits.ORGB_DMA_THR         = hblend->OutfifoThr.Bits.DMA_THR;
    
    BLEND_AHB0->CTRL.Bits.BLEND_EN = 1;
    
}

/******************************************************************************
 * @fn      blend_start
 *
 * @brief   Blend start without interrupt.
 * 
 * @param   hblend : BLEND HANDLE
 *          DataParam : BLEND_DataParam_t structure contain pixel need to handle 
 */
void blend_start(BLEND_HandleTypeDef *hblend, BLEND_DataParam_t DataParam)
{
    uint32_t *SrcRgb;
    uint32_t *BackRgb;
    uint32_t *Mask;
    uint32_t *OutRgb;
    uint32_t SrcSize;
    uint32_t BackSize;
    uint32_t MaskSize;
    uint32_t OutSize;
    
    SrcRgb = (uint32_t *)DataParam.SrcRgb;
    BackRgb = (uint32_t *)DataParam.BackRgb;
    Mask = (uint32_t *)DataParam.MaskRgb;
    OutRgb = (uint32_t *)DataParam.OutRgb;
    
    SrcSize = DataParam.PixelCount;
    BackSize = DataParam.PixelCount;
    MaskSize = DataParam.PixelCount;
    OutSize = DataParam.PixelCount;

    
    uint32_t MaskAddValue = 0;
    

    while(hblend->DataOutIndex < OutSize)
    {
        /*whether txfifo 0 is almost full or not*/
        if((!__RGB0_FIFO_IS_ALFULL()) && (hblend->SrcInIndex < SrcSize))
        {
            BLEND_AHB0->RGB0_DATA = SrcRgb[hblend->SrcInIndex++];
        }
        /*whether txfifo 1 is full or not*/
        if((!__RGB1_FIFO_IS_ALFULL()) && (hblend->BackInIndex < BackSize))
        {
            BLEND_AHB0->RGB1_DATA = BackRgb[hblend->BackInIndex++];
        }
        /*whether txfifo mask is full or not*/
        if((!__MASK_FIFO_IS_ALFULL()) && (hblend->MaskInIndex < MaskSize) &&
            (hblend->BlendInit.Bits.MASK_CAL_EN))
        {
            BLEND_AHB1->MASK_DATA = Mask[hblend->MaskInIndex++];
        }
        /*whether rx fifo is empty or not*/
        if(BLEND_AHB0->FIFO_STATUS.Bits.ORGB_EMPTY != 1)
        {
            DataParam.OutRgb[hblend->DataOutIndex++] = BLEND_AHB1->ORGB_DATA;
        }
    }
}

/******************************************************************************
 * @fn      blend_start_IT
 *
 * @brief   Blend start with interrupt.
 * 
 * @param   hblend : BLEND HANDLE
 *          DataParam : BLEND_DataParam_t structure contain pixel need to handle 
 */
void blend_start_IT(BLEND_HandleTypeDef *hblend, BLEND_DataParam_t DataParam)
{
    hblend->SrcRgb   = (uint32_t *)DataParam.SrcRgb;
    hblend->BackRgb  = (uint32_t *)DataParam.BackRgb;
    hblend->OutRgb   = (uint32_t *)DataParam.OutRgb;
    hblend->SrcSize  = DataParam.PixelCount;
    hblend->BackSize = DataParam.PixelCount;
    hblend->MaskSize = DataParam.PixelCount;
    hblend->OutSize  = DataParam.PixelCount;
    hblend->PixelCount = DataParam.PixelCount;
            
    if(hblend->BlendInit.Bits.MASK_CAL_EN == 1)
    {
        hblend->MaskRgb = (uint32_t *)DataParam.MaskRgb;
        __BLEND_INT_MASK_FIFO_EMPTY_ENABLE();
    }
    __BLEND_INT_RGB0_FIFO_EMPTY_ENABLE();
    __BLEND_INT_RGB1_FIFO_EMPTY_ENABLE();
    __BLEND_INT_ORGB_FIFO_ALFULL_ENABLE();  
    
}