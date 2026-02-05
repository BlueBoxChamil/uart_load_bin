/*
  ******************************************************************************
  * @file    driver_sbc_enc.c
  * @author  FreqChip Firmware Team
  * @version V1.0.0
  * @date    2021
  * @brief   SBC ENCODER module driver.
  *          This file provides firmware functions to manage the 
  *          SBC ENCODER peripheral
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 FreqChip.
  * All rights reserved.
  ******************************************************************************
*/
#include "fr30xx.h"

/******************************************************************************
 * @fn      sbc_enc_init
 *
 * @brief   Initialize the SBC encoder according to the specified parameters
 *          in the SBC_ENC_HandleTypeDef 
 *
 * @param   hSbcEnc : SBC_ENC_HandleTypeDef structure that contains the
 *                      configuration information for SBC encoder module.
 */
void sbc_enc_init(SBC_ENC_HandleTypeDef *hSbcEnc)
{
    /*sbc encoder init*/
    SBC_ENC->SBCE_CTRL.Bits.ENC_INL_FIFO_RESET = 1;
    SBC_ENC->SBCE_CTRL.Bits.ENC_INR_FIFO_RESET = 1;
    SBC_ENC->SBCE_CTRL.Bits.ENC_OUT_FIFO_RESET = 1;
    SBC_ENC->SBCE_CTRL.Bits.ENC_RESET          = 1;
    
    /*encode config*/
    SBC_ENC->SBCE_CFG.Bits.CH_MODE = hSbcEnc->SbcEncInit.Bits.ch_mode;
    if(hSbcEnc->SbcEncInit.Bits.ch_mode == SBCE_MONO)
    {
        SBC_ENC->SBCE_CTRL.Bits.ENC_INR_FLOW_CTRL = 1;
    }
    SBC_ENC->SBCE_CFG.Bits.BLK_LEN = hSbcEnc->SbcEncInit.Bits.blk_len;
    SBC_ENC->SBCE_CFG.Bits.SMP_FRQ = hSbcEnc->SbcEncInit.Bits.sample_freq;
    
    /*encode fifo level config*/
    SBC_ENC->SBCE_INFIFO_LVL.Bits.INLFIFO_ALEMPTY_LEVEL = hSbcEnc->SbcEncInit.Bits.inlfifo_alempty_lvl;
    SBC_ENC->SBCE_INFIFO_LVL.Bits.INRFIFO_ALEMPTY_LEVEL = hSbcEnc->SbcEncInit.Bits.inrfifo_alempty_lvl;
    SBC_ENC->SBCE_OUTFF_LVL.Bits.OUTFIFO_ALFULL_LEVEL   = hSbcEnc->SbcEncInit.Bits.outlfifo_alfull_lvl;
    
    /*dma config*/
    if(hSbcEnc->SbcEncInit.Bits.input_dma_en == 1)
    {
        SBC_ENC->SBCE_CTRL.Bits.IN_DMA_EN   = 1;
        SBC_ENC->SBCE_CTRL.Bits.INL_FIFO_EN = 1;
        SBC_ENC->SBCE_CTRL.Bits.INR_FIFO_EN = 1;
    }
    if(hSbcEnc->SbcEncInit.Bits.output_dma_en == 1)
    {
        SBC_ENC->SBCE_CTRL.Bits.OUT_DMA_EN = 1;
    }
    
    /*sbc encoder start*/
    SBC_ENC->SBCE_CTRL.Bits.ENC_EN = 1;
}

/******************************************************************************
 * @fn      sbc_encoder_enc_IT
 *
 * @brief   start sbc encoder with isr
 *
 * @param   hSbcEnc : sbc encoder handle.
 *          fp_Data_In : pcm data in
 *          fu32_size  : pcm data size
 *          fp_Data_Out : sbc encoder packed frame out
 */
void sbc_encoder_enc_IT(SBC_ENC_HandleTypeDef *hSbcEnc, uint16_t *fp_Data_In, uint32_t fu32_size, uint8_t *fp_Data_Out)
{

    hSbcEnc->OrignlData     = fp_Data_In;
    hSbcEnc->EncodedData    = fp_Data_Out;
    hSbcEnc->OrignlDataSize = fu32_size;
    hSbcEnc->gDataInIndex   = 0;
    hSbcEnc->gDataOutIndex  = 0;
    SBC_ENC->SBCE_INTEN.Word |= 0x24;
    return;
}

/******************************************************************************
 * @fn      sbcenc_IRQHandler
 *
 * @brief   sbc encoder handle function in sbc encoder isr
 *
 * @param   hSbcEnc : sbc encoder handle.
 *          
 */
void sbcenc_IRQHandler(SBC_ENC_HandleTypeDef *hSbcEnc)
{
    uint32_t isr = __SBCE_GET_ISR_STS();
    if(isr & OUTFF_ALFULL_INT)
    {
        while(!__SBCE_OUTFIFO_IS_EMPTY())
        {
            hSbcEnc->EncodedData[hSbcEnc->gDataOutIndex++] = (uint8_t)__SBCE_GET_OUT_RESULT_WORD();
        }
    }
    if(isr & INLF_EMPTY_INT)
    {
        uint32_t read_size = 0;
        if(SBC_ENC->SBCE_CFG.Bits.CH_MODE == SBCE_STEREO)/*2 channels*/
        {
            read_size = (hSbcEnc->OrignlDataSize - hSbcEnc->gDataInIndex) >= 256 ? 256 : (hSbcEnc->OrignlDataSize - hSbcEnc->gDataInIndex);
            for(uint32_t i = 0; i< read_size/2; i++)
            {
                SBC_ENC->SBCE_INFIFO.Word = *((int32_t *)&hSbcEnc->OrignlData[hSbcEnc->gDataInIndex]);
                hSbcEnc->gDataInIndex += 2;
            }
        }
        else/*1 channel*/
        {
            read_size = (hSbcEnc->OrignlDataSize - hSbcEnc->gDataInIndex) >= 128 ? 128 : (hSbcEnc->OrignlDataSize - hSbcEnc->gDataInIndex);
            for(uint32_t i = 0; i< read_size; i++)
            {
                SBC_ENC->SBCE_INFIFO.Bits.ENC_FFL = hSbcEnc->OrignlData[hSbcEnc->gDataInIndex++];
            }
        }
        if(hSbcEnc->gDataInIndex >= hSbcEnc->OrignlDataSize)
        {
            __SBCE_INLFF_EMPTY_INT_DISABLE();
        }
        
//        while(!__SBCE_INLFIFO_IS_FULL())
//        {
//            if(SBC_ENC->SBCE_CFG.Bits.CH_MODE == SBCE_STEREO)/*2 channels*/
//            {
//                SBC_ENC->SBCE_INFIFO.Word = *((int32_t *)&hSbcEnc->OrignlData[hSbcEnc->gDataInIndex]);
//                hSbcEnc->gDataInIndex += 2;
//            }
//            else/*1 channel*/
//            {
//                SBC_ENC->SBCE_INFIFO.Bits.ENC_FFL = hSbcEnc->OrignlData[hSbcEnc->gDataInIndex++];
//            }
//            if(hSbcEnc->gDataInIndex >= hSbcEnc->OrignlDataSize)
//            {
//                __SBCE_INLFF_EMPTY_INT_DISABLE();
//                break;
//            }
//        }
    }

}