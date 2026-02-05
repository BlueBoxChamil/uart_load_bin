/*
  ******************************************************************************
  * @file    driver_yuv2rgb.c
  * @author  FreqChip Firmware Team
  * @version V1.0.0
  * @date    2022
  * @brief   YUV2RGB module driver.
  *          This file provides firmware functions to manage the YUV2RGB.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 FreqChip.
  * All rights reserved.
  ******************************************************************************
*/
#include "fr30xx.h"

/************************************************************************************
 * @fn      yuv2rgb_IRQHandler
 *
 * @brief   yuv2rgb interrupt handler.
 *
 * @param   hyuv2rgb: YUV2RGB handle.
 */
__WEAK void yuv2rgb_IRQHandler(YUV2RGB_HandleTypeDef *hyuv2rgb)
{
    if (__YUV2RGB_GET_INT_STATUS(hyuv2rgb->YUV2RGBx) & YUV_FIFO_ALMOST_EMPTY)
    {
        while(!(__YUV2RGB_GET_INT_RAW_STATUS(hyuv2rgb->YUV2RGBx) & YUV_FIFO_FULL))
        {
            switch (hyuv2rgb->Init.YUV_Format)
            {
                case YUV_FORMAT_444: hyuv2rgb->YUV2RGBx->YUV_DATA = *hyuv2rgb->u_YUVData.p_u32++; break;
                case YUV_FORMAT_422: hyuv2rgb->YUV2RGBx->YUV_DATA = *hyuv2rgb->u_YUVData.p_u16++; break;
                default:break;
            }

            hyuv2rgb->u32_YUVCount++;
            if (hyuv2rgb->u32_YUVCount >= hyuv2rgb->u32_Pixels)
            {
                __YUV2RGB_INT_DISABLE(hyuv2rgb->YUV2RGBx, YUV_FIFO_ALMOST_EMPTY);
                break;
            }
        }
    }

    if (__YUV2RGB_GET_INT_STATUS(hyuv2rgb->YUV2RGBx) & RGB_FIFO_ALMOST_FULL)
    {
        while (!(__YUV2RGB_GET_INT_RAW_STATUS(hyuv2rgb->YUV2RGBx) & RGB_FIFO_EMPTY))
        {
            switch (hyuv2rgb->Init.RGB_Format)
            {
                case RGB_FORMAT_888: *hyuv2rgb->u_RGBData.p_u32++ = hyuv2rgb->YUV2RGBx->RGB_DATA; break;
                case RGB_FORMAT_565: *hyuv2rgb->u_RGBData.p_u16++ = hyuv2rgb->YUV2RGBx->RGB_DATA; break;
                case RGB_FORMAT_332: *hyuv2rgb->u_RGBData.p_u8++  = hyuv2rgb->YUV2RGBx->RGB_DATA; break;
                default:break;
            }

            hyuv2rgb->u32_RGBCount++;
            if (hyuv2rgb->u32_RGBCount >= hyuv2rgb->u32_Pixels)
            {
                __YUV2RGB_INT_DISABLE(hyuv2rgb->YUV2RGBx, RGB_FIFO_ALMOST_FULL);
                hyuv2rgb->b_CovertBusy = false;
                break;
            }
        }
    }
}

/************************************************************************************
 * @fn      yuv2rgb_init
 *
 * @brief   Initialize YUV2RGB UART according to the specified parameters 
 *          in the struct_YUV2RGBInit_t.
 *
 * @param   hyuv2rgb: YUV2RGB handle.
 */
void yuv2rgb_init(YUV2RGB_HandleTypeDef *hyuv2rgb)
{
    /* YUV2RGB enable */
    __YUV2RGB_ENABLE(hyuv2rgb->YUV2RGBx);
    /* DMA default enable */
    __YUV2RGB_DMA_ENABLE(hyuv2rgb->YUV2RGBx);

    /* Set YUV2RGB format, calculate Mode */
    hyuv2rgb->YUV2RGBx->YUV2RGB_CFG.RGB_FORMAT = hyuv2rgb->Init.RGB_Format;
    hyuv2rgb->YUV2RGBx->YUV2RGB_CFG.YUV_FORMAT = hyuv2rgb->Init.YUV_Format;
    hyuv2rgb->YUV2RGBx->YUV2RGB_CFG.YUV_MODE   = hyuv2rgb->Init.YUV_CalculateMode;

    if (hyuv2rgb->Init.YUV_Format == YUV_FORMAT_444)
        hyuv2rgb->YUV2RGBx->FLOW_CTRL.YUV_FLOW_LEVEL = 1;
    else
        hyuv2rgb->YUV2RGBx->FLOW_CTRL.YUV_FLOW_LEVEL = 2;

    hyuv2rgb->YUV2RGBx->FLOW_CTRL.RGB_FLOW_LEVEL = 30;
}

/************************************************************************************
 * @fn      yuv2rgb_convert
 *
 * @brief   YUV to RGB convert in blocking mode.
 *
 * @param   hyuv2rgb: YUV2RGB handle.
 *          YUV_Buffer:  YUV data buffer.
 *          RGB_BUffer:  RGB data buffer.
 *          fu32_Pixels: 
 */
void yuv2rgb_convert(YUV2RGB_HandleTypeDef *hyuv2rgb, void *YUV_Buffer, void *RGB_Buffer, uint32_t fu32_Pixels)
{
    uint32_t lu32_RGBCount = 0;
    uint32_t lu32_YUVCount = 0;

    hyuv2rgb->u_YUVData.p_data = YUV_Buffer;
    hyuv2rgb->u_RGBData.p_data = RGB_Buffer;

    while(true)
    {
        if (!(__YUV2RGB_GET_INT_RAW_STATUS(hyuv2rgb->YUV2RGBx) & RGB_FIFO_EMPTY))
        {
            if (lu32_RGBCount < fu32_Pixels)
            {
                switch (hyuv2rgb->Init.RGB_Format)
                {
                    case RGB_FORMAT_888: *hyuv2rgb->u_RGBData.p_u32++ = hyuv2rgb->YUV2RGBx->RGB_DATA; break;
                    case RGB_FORMAT_565: *hyuv2rgb->u_RGBData.p_u16++ = hyuv2rgb->YUV2RGBx->RGB_DATA; break;
                    case RGB_FORMAT_332: *hyuv2rgb->u_RGBData.p_u8++  = hyuv2rgb->YUV2RGBx->RGB_DATA; break;
                    default:break;
                }
                lu32_RGBCount++;
                if (lu32_RGBCount >= fu32_Pixels)
                    break;
            }
        }

        if (!(__YUV2RGB_GET_INT_RAW_STATUS(hyuv2rgb->YUV2RGBx) & YUV_FIFO_FULL))
        {
            if (lu32_YUVCount < fu32_Pixels)
            {
                switch (hyuv2rgb->Init.YUV_Format)
                {
                    case YUV_FORMAT_444: hyuv2rgb->YUV2RGBx->YUV_DATA = *hyuv2rgb->u_YUVData.p_u32++; break;
                    case YUV_FORMAT_422: hyuv2rgb->YUV2RGBx->YUV_DATA = *hyuv2rgb->u_YUVData.p_u16++; break;
                    default: break;
                }

                lu32_YUVCount++;
            }
        }
    }
}

/************************************************************************************
 * @fn      yuv2rgb_convert_IT
 *
 * @brief   YUV to RGB convert in interrupt mode.
 *
 * @param   hyuv2rgb: YUV2RGB handle.
 */
void yuv2rgb_convert_IT(YUV2RGB_HandleTypeDef *hyuv2rgb, void *YUV_Buffer, void *RGB_Buffer, uint32_t fu32_Pixels)
{
    hyuv2rgb->u_YUVData.p_data = YUV_Buffer;
    hyuv2rgb->u_RGBData.p_data = RGB_Buffer;
    hyuv2rgb->u32_RGBCount = 0;
    hyuv2rgb->u32_YUVCount = 0;
    hyuv2rgb->u32_Pixels   = fu32_Pixels;
    hyuv2rgb->b_CovertBusy = true;

    __YUV2RGB_YUV_FIFO_ALMOST_EMPTY_LEVEL(hyuv2rgb->YUV2RGBx, 6);
    __YUV2RGB_RGB_FIFO_ALMOST_FULL_LEVEL(hyuv2rgb->YUV2RGBx,  1);
    __YUV2RGB_INT_ENABLE(hyuv2rgb->YUV2RGBx, YUV_FIFO_ALMOST_EMPTY);
    __YUV2RGB_INT_ENABLE(hyuv2rgb->YUV2RGBx, RGB_FIFO_ALMOST_FULL);
}
