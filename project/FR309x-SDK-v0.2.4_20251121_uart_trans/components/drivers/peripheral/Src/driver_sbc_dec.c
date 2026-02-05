/*
  ******************************************************************************
  * @file    driver_sbc_codec.c
  * @author  FreqChip Firmware Team
  * @version V1.0.0
  * @date    2021
  * @brief   IIR module driver.
  *          This file provides firmware functions to manage the 
  *          SBC CODEC peripheral
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 FreqChip.
  * All rights reserved.
  ******************************************************************************
*/
#include "fr30xx.h"

#define SBC_SYNCWORD 0x9c

/******************************************************************************
 * @fn      sbc_dec_get_packed_frame_info
 *
 * @brief   sbc decoder get sbc packed frame info
 *
 * @param   data : sbc packed frame
 *          frame_info : struct_frame_info_t structure contains
 */
int sbc_dec_get_packed_frame_info(uint8_t *data, struct_frame_info_t frame_info)
{
    if (data[0] != SBC_SYNCWORD)
    {
        return -2;
    }
    frame_info.frequency = (data[1] >> 6) & 0x03;
    frame_info.block_mode = (data[1] >> 4) & 0x03;
    
    switch (frame_info.block_mode) {
    case 0:
        frame_info.block_len = 4;
        break;
    case 1:
        frame_info.block_len = 8;
        break;
    case 2:
        frame_info.block_len = 12;
        break;
    case 3:
        frame_info.block_len = 16;
        break;
    default:
        frame_info.block_len = 4;
        break;
    }
    frame_info.mode = (data[1] >> 2) & 0x03;
    switch(frame_info.mode)
    {
        case 0:
            frame_info.channels = 1;
        break;
        case 3:
            frame_info.channels = 2;
        break;
        default:
            break;
    }
    frame_info.allocation = (data[1] >> 1) & 0x01;
    frame_info.subband_mode = (data[1] & 0x01);
    frame_info.subbands = frame_info.subband_mode ? 8 : 4;
    frame_info.bit_pool = data[2];
    
    if((frame_info.mode == 0 || frame_info.mode == 1) &&
        frame_info.bit_pool > 16 * frame_info.subbands)
        return -4;
    if((frame_info.mode == 3 || frame_info.mode == 2) &&
         frame_info.bit_pool > 32 * frame_info.subbands)
        return -4;
    
    return 0;
}

/******************************************************************************
 * @fn      sbc_dec_init
 *
 * @brief   sbc decoder initialize 
 *
 * @param   hSbcDec : sbc decoder handler
 */
void sbc_dec_init(SBC_DEC_HandleTypeDef *hSbcDec)
{
    /*sbc decoder fifo init*/
    SBC_DEC->SBCD_CTRL.Bits.DEC_IN_FIFO_RESET   = 1;
    SBC_DEC->SBCD_CTRL.Bits.DEC_OUTL_FIFO_RESET = 1;
    SBC_DEC->SBCD_CTRL.Bits.DEC_OUTR_FIFO_RESET = 1;
    SBC_DEC->SBCD_CTRL.Bits.DEC_RESET           = 1;
    
    /* sbc dma config initialize*/
    SBC_DEC->SBCD_CTRL.Bits.IN_DMA_EN    = hSbcDec->sbc_init.Bits.input_dma_en;
    SBC_DEC->SBCD_CTRL.Bits.OUT_DMA_EN   = hSbcDec->sbc_init.Bits.output_dma_en;
    if(SBC_DEC->SBCD_CTRL.Bits.OUT_DMA_EN == 1)
    {
        if(hSbcDec->sbc_init.Bits.ch_mode == 1)
        {
            SBC_DEC->SBCD_CTRL.Bits.OUTR_FIFO_EN = 1;  
        }
        else
        {
            SBC_DEC->SBCD_CTRL.Bits.OUTL_FIFO_EN = 1;
        }
    }
    /*sbc fifo level intialize*/
    SBC_DEC->SBCD_FIFO_LEVEL.Bits.INFIFO_ALEMPTY_LEVEL  = hSbcDec->sbc_init.Bits.infifo_alempty_lvl;
    SBC_DEC->SBCD_FIFO_LEVEL.Bits.OUTLFIFO_ALFULL_LEVEL = hSbcDec->sbc_init.Bits.outlfifo_alfull_lvl;
    SBC_DEC->SBCD_FIFO_LEVEL.Bits.OUTRFIFO_ALFULL_LEVEL = hSbcDec->sbc_init.Bits.outrlfifo_alfull_lvl;

    SBC_DEC->SBCD_CTRL.Bits.DEC_EN = 1;
}

/******************************************************************************
 * @fn      sbc_dec_playdata_IT
 *
 * @brief   sbc decoder play data with isr
 *
 * @param   hSbcDec : sbc decoder handler
 *          fp_Data : sbc packed frame data
 *          fu32_Size : sbc packed frame data size
 *          fp_Data_Out : after sbc decoder pcm data
 */
void sbc_dec_playdata_IT(SBC_DEC_HandleTypeDef *hSbcDec, uint8_t *fp_Data, uint32_t fu32_Size, uint16_t *fp_Data_Out)
{
    hSbcDec->OrignData  = fp_Data_Out;
    hSbcDec->EncodeData = fp_Data;
    hSbcDec->DataSize   = fu32_Size;
    hSbcDec->InputIndex = 0;
    hSbcDec->OutIndex   = 0;
    if(hSbcDec->sbc_init.Bits.ch_mode == SBCD_MONO)
    {
        SBC_DEC->SBCD_INTEN.Bits.CRC_ERR_INT_EN       = 1;
        SBC_DEC->SBCD_INTEN.Bits.OUTLFF_ALFULL_INT_EN = 1;
        SBC_DEC->SBCD_INTEN.Bits.INFF_EMPTY_INT_EN    = 1;
    }
    else
    {
        SBC_DEC->SBCD_INTEN.Bits.CRC_ERR_INT_EN       = 1;
        SBC_DEC->SBCD_INTEN.Bits.OUTRFF_ALFULL_INT_EN = 1;
        SBC_DEC->SBCD_INTEN.Bits.INFF_EMPTY_INT_EN    = 1;
    }
    
}

/******************************************************************************
 * @fn      sbcdec_IRQHandler
 *
 * @brief   sbc decoder handle function in sbc decoder isr
 *
 * @param   hSbcDec : sbc decoder handler
 */
void sbcdec_IRQHandler(SBC_DEC_HandleTypeDef *hSbcDec)
{
    uint32_t status = __SBCD_GET_ISR_STS();

    if(status & OUTLF_ALFULL_INT)
    {
        while(!__SBCD_OUTLFIFO_IS_EMPTY())
        {
            hSbcDec->OrignData[hSbcDec->OutIndex++] = SBC_DEC->SBCD_OUTFIFO.Bits.PCM_LEFT_DATA;
        }
    }
    if(status & OUTRF_ALFULL_INT)
    {
        /*outfifo almost full need to do*/
        while(!__SBCD_OUTRFIFO_IS_EMPTY())
        {
            hSbcDec->OrignData[hSbcDec->OutIndex++] = SBC_DEC->SBCD_OUTFIFO.Bits.PCM_LEFT_DATA;
            hSbcDec->OrignData[hSbcDec->OutIndex++] = SBC_DEC->SBCD_OUTFIFO.Bits.PCM_RIGHT_DATA;
        }
    }
    if(status & INFF_EMPTY_INT)
    {
        while(!__SBCD_INFIFO_IS_FULL())
        {
            SBC_DEC->SBCD_INFIFO.Bits.SBC_IN = hSbcDec->EncodeData[hSbcDec->InputIndex++];
            if(hSbcDec->InputIndex >= hSbcDec->DataSize)
            {
                __SBCD_INFIFO_EMPTY_DISABLE();
                if(hSbcDec->Callback)
                {
                    hSbcDec->Callback(hSbcDec);
                }
                break;
            }
        }
    }

}

