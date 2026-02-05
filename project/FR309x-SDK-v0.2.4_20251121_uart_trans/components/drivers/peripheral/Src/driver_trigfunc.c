/*
  ******************************************************************************
  * @file    driver_trigfunc.c
  * @author  FreqChip Firmware Team
  * @version V1.0.0
  * @date    2023
  * @brief   trigfunc module driver.
  *          This file provides firmware functions to manage the 
  *          trigfunc(trigonometric function) peripheral
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 FreqChip.
  * All rights reserved.
  ******************************************************************************
*/
#include "fr30xx.h"

double trigfunc_sin(double fe_argin)
{
    double    f_value = 0;
    uint32_t  int_value = 0;
    
    fe_argin =  QUANTIZATION_PARAMETER * fe_argin;
    
    int_value = (int)fe_argin;
    f_value   = fe_argin - int_value;

    if((f_value) >= 0.5)
    {
        int_value++;
    } 
    
    TRIGFUNC->TRIGFUNC_INTR &= ~TRIGFUNC_INTRCR;
    TRIGFUNC->TRIGFUNC_CTRL =   TRIG_CAL_SIN_AND_COS | BYTE_2;

    TRIGFUNC->TRIGFUNC_ARG1_IN = int_value;
    while(!__TRIGFUNC_CAL_IS_DONE_STATUS());
    
    return (int16_t)(TRIGFUNC->TRIGFUNC_RESULT2_OUT) / QUANTIZATION_PARAMETER;   
}

double trigfunc_cos(double fe_argin)
{ 
    double   f_value = 0;    
    uint32_t int_value = 0;
    
    fe_argin =  QUANTIZATION_PARAMETER * fe_argin;
    
    int_value = (int)fe_argin;
    f_value   = fe_argin - int_value;

    if((f_value) >= 0.5)
    {
        int_value++;
    }
    
    TRIGFUNC->TRIGFUNC_INTR &= ~TRIGFUNC_INTRCR;
    TRIGFUNC->TRIGFUNC_CTRL =   TRIG_CAL_SIN_AND_COS | BYTE_2;

    TRIGFUNC->TRIGFUNC_ARG1_IN = int_value;
    while(!__TRIGFUNC_CAL_IS_DONE_STATUS());
 
    return (int16_t)(TRIGFUNC->TRIGFUNC_RESULT1_OUT) / QUANTIZATION_PARAMETER;     
}

double trigfunc_tan(double fe_argin)
{
    double f_value;
    uint32_t int_value = 0;
    
    fe_argin =  QUANTIZATION_PARAMETER * fe_argin;
    
    int_value = (int)fe_argin;
    f_value   = fe_argin - int_value;

    if((f_value) >= 0.5)
    {
        int_value++;
    }    
    
    TRIGFUNC->TRIGFUNC_INTR &= ~TRIGFUNC_INTRCR;
    TRIGFUNC->TRIGFUNC_CTRL =   TRIG_CAL_TAN | BYTE_2;

    TRIGFUNC->TRIGFUNC_ARG1_IN = int_value;
    while(!__TRIGFUNC_CAL_IS_DONE_STATUS());
       
    return (int16_t)(TRIGFUNC->TRIGFUNC_RESULT1_OUT) / (double)0x100 ;       
}

double trigfunc_atan(double fe_argin)
{
    int16_t f_value;   
       
    TRIGFUNC->TRIGFUNC_INTR &= ~TRIGFUNC_INTRCR;                                                                                              
    TRIGFUNC->TRIGFUNC_CTRL =   TRIG_CAL_ARCTAN_X | BYTE_2;
    
    f_value = fe_argin * 0x100;
    
    TRIGFUNC->TRIGFUNC_ARG1_IN = f_value;
    
    while(!__TRIGFUNC_CAL_IS_DONE_STATUS());
    
    return (int16_t)(TRIGFUNC->TRIGFUNC_RESULT1_OUT) * 180 / QUANTIZATION_PARAMETER; 
}
