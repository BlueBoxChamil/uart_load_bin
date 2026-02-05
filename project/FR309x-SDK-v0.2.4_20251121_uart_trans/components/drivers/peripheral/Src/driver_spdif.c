/*
  ******************************************************************************
  * @file    driver_spdif.c
  * @author  FreqChip Firmware Team
  * @version V1.0.0
  * @date    2024
  * @brief   SPDIF module driver.
  *          This file provides firmware functions to manage the 
  *          SPDIF peripheral
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 FreqChip.
  * All rights reserved.
  ******************************************************************************
*/
#include "fr30xx.h"

/******************************************************************************
 * @fn      SPDIF_IRQHandler
 *
 * @brief   spdif handle function in spdif isr
 *
 * @param   hspdif : spdif handle
 */
__WEAK void SPDIF_IRQHandler(SPDIF_HandleTypeDef *hspdif)
{
    if (__SPDIF_GET_INT_STATUS() & SPDIF_INT_ALEMPTY)
    {
        if (hspdif->TxALEmptyCallback)
        {
            hspdif->TxALEmptyCallback(hspdif);
        }
    }

    if (__SPDIF_GET_INT_STATUS() & SPDIF_INT_ALFULL)
    {
        if (hspdif->RxALFullCallback)
        {
            hspdif->RxALFullCallback(hspdif);
        }
    }
}

/******************************************************************************
 * @fn      spdif_init
 *
 * @brief   Initialize the SPDIF according to the specified parameters
 *          in the SPDIF_HandleTypeDef.
 *
 * @param   hspdif : spdif handle
 */
void spdif_init(SPDIF_HandleTypeDef *hspdif)
{
    /* Clock enable */
    __SPDIF_CLOCK_ENABLE();
    /* Reset */
    __SPDIF_SFR_RESET();
    __SPDIF_FIFO_RESET();
    /* Block enable */
    __SPDIF_ENABLE();
    /* Work mode */
    __SPDIF_WORK_MODE(hspdif->Init.WorkMode);

    __SPDIF_PARITY_CHECK_ENABLE();
    __SPDIF_PARITY_AUTO_MODE();
    __SPDIF_VALID_BIT_CHECK_ENABLE();

    /* mono/stereo */
    __SPDIF_CHANNEL_MODE(hspdif->Init.MonoEn);
    /* right channel copy enable */
    __SPDIF_MONO_RIGHT_COPY_ENABLE();

    __SPDIF_SETPREAMBLE_OPERATION_MODE(0);
    /* data width */
    __SPDIF_DATA_WIDTH_CONFIG(hspdif->Init.DataWidth);
    /* SODIF INT enable */
    __SPDIF_INT_ENABLE(SPDIF_INT_MASK);

    /* threshold */
    if (hspdif->Init.WorkMode)
    {
        __SPDIF_TXALMOST_EMPTY_THRESHOLD_CONFIG(hspdif->Init.Tx_ALEmpty_Threshold);
        __SPDIF_RXALMOST_FULL_THRESHOLD_CONFIG(SPDIF_FIFO_DEPTH/2);
    }
    else
    {
        __SPDIF_TXALMOST_EMPTY_THRESHOLD_CONFIG(SPDIF_FIFO_DEPTH/2);
        __SPDIF_RXALMOST_FULL_THRESHOLD_CONFIG(hspdif->Init.Rx_ALFull_Threshold);
    }
}

/******************************************************************************
 * @fn      spdif_transmit_IT
 *
 * @brief   IN tx work mode, spdif send data in interrupt mode.
 *
 * @param   hspdif : spdif handle
 * @param   fp_Data : send data pointer
 */
void spdif_transmit_IT(SPDIF_HandleTypeDef *hspdif, void *fp_Data)
{
    hspdif->u_Data.p_data = fp_Data;

    /* enable almost empty interrupt */
    __SPDIF_INT_ENABLE(SPDIF_INT_ALEMPTY);
}

/******************************************************************************
 * @fn      spdif_receive_IT
 *
 * @brief   IN rx work mode, spdif receive data in interrupt mode.
 *
 * @param   hspdif : spdif handle
 * @param   fp_Data : receive data pointer
 */
void spdif_receive_IT(SPDIF_HandleTypeDef *hspdif, void *fp_Data)
{
    hspdif->u_Data.p_data = fp_Data;

    /* enable almost empty interrupt */
    __SPDIF_INT_ENABLE(SPDIF_INT_ALFULL);
}

/******************************************************************************
 * @fn      spdif_send_data
 *
 * @brief   IN tx work mode, spdif send data.
 *
 * @param   hspdif : spdif handle
 * @param   fp_Data : send data pointer
 * @param   fu32_length : send data length
 */
void spdif_send_data(SPDIF_HandleTypeDef *hspdif, void *fp_Data, uint32_t fu32_length)
{
    hspdif->u_Data.p_data = fp_Data;

    if (hspdif->Init.DataWidth == SPDIF_DATA_WIDTH_16BIT)
    {
        while(fu32_length)
        {
            while(__SPDIF_GET_RAW_STS() & SPDIF_INT_FULL);
            __SPDIF_WRITE_FIFO(*hspdif->u_Data.p_u16 << 8);
            hspdif->u_Data.p_u16++;
            fu32_length--;
        }
    }
    else if (hspdif->Init.DataWidth == SPDIF_DATA_WIDTH_20BIT)
    {
        while(fu32_length)
        {
            while(__SPDIF_GET_RAW_STS() & SPDIF_INT_FULL);
            __SPDIF_WRITE_FIFO(*hspdif->u_Data.p_u32 << 4);
            hspdif->u_Data.p_u32++;
            fu32_length--;
        }
    }
    else
    {
        while(fu32_length)
        {
            while(__SPDIF_GET_RAW_STS() & SPDIF_INT_FULL);
            __SPDIF_WRITE_FIFO(*hspdif->u_Data.p_u32++);
            fu32_length--;
        }
    }
}

/******************************************************************************
 * @fn      spdif_read_data
 *
 * @brief   IN rx work mode, spdif read data.
 *
 * @param   hspdif : spdif handle
 * @param   fp_Data : send data pointer
 * @param   fu32_length : send data length
 */
void spdif_read_data(SPDIF_HandleTypeDef *hspdif, void *fp_Data, uint32_t fu32_length)
{
    hspdif->u_Data.p_data = fp_Data;

    if (hspdif->Init.DataWidth == SPDIF_DATA_WIDTH_16BIT)
    {
        while(fu32_length)
        {
            if (!(__SPDIF_GET_RAW_STS() & SPDIF_INT_EMPTY))
            {
                *hspdif->u_Data.p_u16++ = __SPDIF_READ_FIFO() >> 8;
                fu32_length--;
            }
        }
    }
    else if (hspdif->Init.DataWidth == SPDIF_DATA_WIDTH_20BIT)
    {
        while(fu32_length)
        {
            if (!(__SPDIF_GET_RAW_STS() & SPDIF_INT_EMPTY))
            {
                *hspdif->u_Data.p_u32++ = __SPDIF_READ_FIFO() >> 4;
                fu32_length--;
            }
        }
    }
    else
    {
        while(fu32_length)
        {
            if (!(__SPDIF_GET_RAW_STS() & SPDIF_INT_EMPTY))
            {
                *hspdif->u_Data.p_u32++ = __SPDIF_READ_FIFO();
                fu32_length--;
            }
        }
    }
}

