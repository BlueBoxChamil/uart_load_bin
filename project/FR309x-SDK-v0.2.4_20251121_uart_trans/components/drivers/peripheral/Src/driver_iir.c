/*
  ******************************************************************************
  * @file    driver_iir.c
  * @author  FreqChip Firmware Team
  * @version V1.0.0
  * @date    2023
  * @brief   IIR module driver.
  *          This file provides firmware functions to manage the 
  *          IIR filter peripheral
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 FreqChip.
  * All rights reserved.
  ******************************************************************************
*/
#include "fr30xx.h"

/******************************************************************************
 * @fn      iir_init
 *
 * @brief   Initialize the IIR according to the specified parameters
 *          in the IIR_HandleTypeDef 
 *
 * @param   hiir : IIR Handle.
 */
void iir_init(IIR_InitTypeDef *hiir)
{
    __IIR_SOFTRST_SET();
    __IIR_SOFTRST_CLEAR();
    
    for(int i = 0; i < 20; i++)
    {
      IIR_FILTER->IIR_COEF[i] = hiir->IIRCoef[i];  
    }
    
    IIR_FILTER->IIR_CTRL.N_DIV     = hiir->N_Div;
    IIR_FILTER->IIR_CTRL.ORDER_SEL = hiir->Order_Sel;
    IIR_FILTER->IIR_CTRL.NODE_SEL  = hiir->Node_Sel;
    IIR_FILTER->IIR_CTRL.NBYTE_SEL = hiir->Nbytes_Sel;
    
    __IIR_RxFIFO_THRESHOLD_LEVEL(0x20);
    __IIR_TxFIFO_THRESHOLD_LEVEL(0x20);
}

/******************************************************************************
 * @fn      iir_filter_start
 *
 * @brief   Start iir filter
 *
 * @param   fp_Data_In : the data need to handle
 *          fp_Data_Out : the output data
 *          fu32_Size : the size of data which is need to handle
 */
void iir_filter_start(uint32_t *fp_Data_In, uint32_t *fp_Data_Out, uint32_t fu32_Size)
{  
    while(fu32_Size)
    {       
        if(!__IIR_GET_FIFO_STATUS(TX_FIFO_FULL))
        {
            IIR_FILTER->IIR_FIFO = *fp_Data_In++;
            fu32_Size--;
        }
        
        while(!__IIR_GET_FIFO_STATUS(RX_FIFO_EMPTY))
        {
            *fp_Data_Out++ =  IIR_FILTER->IIR_FIFO;
        }
    }  
    
    __IIR_SOFTRST_SET();
    __IIR_SOFTRST_CLEAR();
}

