/*
  ******************************************************************************
  * @file    driver_fft.c
  * @author  FreqChip Firmware Team
  * @version V1.0.0
  * @date    2022
  * @brief   FFT module driver.
  *          This file provides firmware functions to manage the 
  *          FFT (Fast Fourier Transform) peripheral.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 FreqChip.
  * All rights reserved.
  ******************************************************************************
*/
#include "fr30xx.h"

/************************************************************************************
 * @fn      fft_IRQHandler
 *
 * @brief   fft interrupt handler
 *
 * @param   hfft: FFT_HandleTypeDef.
 */
void fft_IRQHandler(FFT_HandleTypeDef *hfft)
{
    int i;
    /* clear int status */
    __FFT_INT_STATUS_CLEAR();

    /* Data OUT */
    for (i = 0; i < hfft->BlockSize; i++)
        hfft->DataOut[hfft->DataOutIndex++] = FFT->FFT_ACCESSRAM;

    if (hfft->BlockCNT)
    {
        hfft->BlockCNT--;

        /* Data IN */
        for (i = 0; i < hfft->BlockSize; i++)
            FFT->FFT_ACCESSRAM = hfft->DataIn[hfft->DataIndex++];
    }
    else
    {
        __FFT_INT_DISALE();
        hfft->FFT_Busy = false;
    }
}

/************************************************************************************
 * @fn      FFT_init
 *
 * @brief   initialize the FFT module.
 *
 * @param   hfft: FFT_HandleTypeDef.
 */
void fft_init(FFT_HandleTypeDef *hfft)
{
    /* FFT calculate Mod */
    FFT->FFT_CTRL.Bits.MODE_SEL = hfft->FFT_Init.FFT_Cal_Mode;
    /* FFT Samples number */
    FFT->FFT_CTRL.Bits.NP_SEL = hfft->FFT_Init.FFT_Samples;

    /* default DMA enable */
    __FFT_DMA_ENABLE();
}

/************************************************************************************
 * @fn      FFT_start
 *
 * @brief   start fft calculate.
 *
 * @param   hfft: FFT_HandleTypeDef.
 * @param   fp_Data_In: the pointer of input data.
 * @param   fp_Data_Out: the pointer of output data.
 * @param   fu32_BlockCNT: Number of blocks to calculate.
 */
void fft_start(FFT_HandleTypeDef *hfft, uint32_t *fp_Data_In, uint32_t *fp_Data_Out, uint32_t fu32_BlockCNT)
{
    int i;
    uint32_t BlockSize;
    uint32_t DataIndex    = 0;
    uint32_t DataOutIndex = 0;

    /* wait fft calculate idle */
    while(!__FFT_IS_IDLE());

    switch (hfft->FFT_Init.FFT_Samples)
    {
        case FFT_128: BlockSize = 128; break;
        case FFT_256: BlockSize = 256; break;
        case FFT_512: BlockSize = 512; break;

        default:break;
    }

    while(fu32_BlockCNT--)
    {
        /* Data IN */
        for (i = 0; i < BlockSize; i++)
            FFT->FFT_ACCESSRAM = fp_Data_In[DataIndex++];
        /* Wait calculate done */
        while(!__FFT_CAL_IS_DONE());
        /* Data OUT */
        for (i = 0; i < BlockSize; i++)
            fp_Data_Out[DataOutIndex++] = FFT->FFT_ACCESSRAM;
    }
}

/************************************************************************************
 * @fn      FFT_start_IT
 *
 * @brief   start fft calculate with isr.
 *
 * @param   hfft: FFT_HandleTypeDef.
 * @param   fp_Data_In: the pointer of input data.
 * @param   fp_Data_Out: the pointer of output data.
 * @param   fu32_BlockCNT: Number of blocks to calculate.
 */
int fft_start_IT(FFT_HandleTypeDef *hfft, uint32_t *fp_Data_In, uint32_t *fp_Data_Out, uint32_t fu32_BlockCNT)
{
    int i;
    uint32_t BlockSize;

    if (hfft->FFT_Busy)
        return -1;

    /* wait fft calculate idle */
    while(!__FFT_IS_IDLE());

    switch (hfft->FFT_Init.FFT_Samples)
    {
        case FFT_128: BlockSize = 128; break;
        case FFT_256: BlockSize = 256; break;
        case FFT_512: BlockSize = 512; break;
        default:break;
    }

    __FFT_INT_ENALE();
    hfft->FFT_Busy     = true;
    hfft->BlockCNT     = fu32_BlockCNT;
    hfft->BlockSize    = BlockSize;
    hfft->DataIndex    = 0;
    hfft->DataOutIndex = 0;
    hfft->DataIn  = fp_Data_In;
    hfft->DataOut = fp_Data_Out;

    hfft->BlockCNT--;

    /* Data IN */
    for (i = 0; i < hfft->BlockSize; i++)
        FFT->FFT_ACCESSRAM = hfft->DataIn[hfft->DataIndex++];
    
    return 0;
}
