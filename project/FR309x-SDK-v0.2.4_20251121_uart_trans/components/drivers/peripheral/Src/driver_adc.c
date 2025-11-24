/*
  ******************************************************************************
  * @file    driver_adc.c
  * @author  FreqChip Firmware Team
  * @version V1.0.0
  * @date    2021
  * @brief   ADC module driver.
  *          This file provides firmware functions to manage the 
  *          Analog to Digital Converter (ADC) peripheral
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 FreqChip.
  * All rights reserved.
  ******************************************************************************
*/
#include "fr30xx.h"

/******************************************************************************
 * @fn      adc_init
 *
 * @brief   Initialize the ADC according to the specified parameters
 *          in the adc_InitConfig_t 
 *
 * @param   InitParam : adc_InitConfig_t structure that contains the
 *                      configuration information for ADC module.
 */
void adc_init(adc_InitConfig_t InitConfig)
{
    /* ADC reset */
    __ADC_RESET();

    /* analog config */
    if (InitConfig.ADC_Reference == ADC_REF_1P2V)
        ADC->ACT0 = ADC_ACT0_REF_1P2V;
    else
        ADC->ACT0 = ADC_ACT0_REF_IOLDO;

    ADC->ACT1 = ADC_ACT1_VBE_EN | ADC_ACT1_VBAT_EN | ADC_ACT1_IS;

    /* Timing config */
    __ADC_SET_CLK_DIV(2);             /*!< ADC clock = 24Mhz / ((ADC_CLK_DIV + 1) * 2). Fixed 2 */
    __ADC_SET_SETUP_CYCLE(60);        /*!< Convert setup    cycle. Fixed 60 */
    __ADC_SET_SAMPLE_CYCLE(30);       /*!< Convert sample   cycle. Fixed 30 */
    __ADC_SET_TIMEOUT_CYCLE(0x1F);    /*!< Convert timeoout cycle. Fixed 31 */

    /* Init default channel maping */
    for (int i = 0; i < ADC_MAX_IO_INPUT_MAP; i++)
    {
        __ADC_SET_CHANNEL_MAP(i, i);
    }

    /* Hrad trigger mode */
    if (InitConfig.ADC_TriggerMode == ADC_HARDWARE_TRIGGER)
    {
        __ADC_SET_TRIGGER_MODE(ADC_HARDWARE_TRIGGER);

        /* set Max channel */
        __ADC_SET_MAX_CHANNEL(InitConfig.HardTriggerConfig.ADC_Channel_Max - 1);
        /* set convert mode */
        __ADC_SET_CONVERT_MODE(InitConfig.HardTriggerConfig.ADC_Convert_Mode);
    }
    /* Soft trigger mode */
    else
    {
        __ADC_SET_TRIGGER_MODE(ADC_SOFTWARE_TRIGGER);
        /* Soft trigger mode default use single convert */
        __ADC_SET_CONVERT_MODE(ADC_SINGLE_MODE);
    }
}

/******************************************************************************
 * @fn      adc_soft_trigger_convert
 *
 * @brief   Software triggers the specified channel convert.
 * 
 * @param   fu8_Channel : channel number(0 ~ 7)
 */
void adc_soft_trigger_convert(uint8_t fu8_Channel)
{
    /* select soft trigger channel */
    __ADC_SET_SOFT_TRIGGER_CHANNEL(fu8_Channel);
    /* soft trigger */
    __ADC_SOFT_TRIGGER();
}

/******************************************************************************
 * @fn      adc_convert_start/stop
 *
 * @brief   hardware triggers channel convert start/stop.
 */
void adc_convert_start(void)
{
    __ADC_CONVERT_ENABLE();
}
void adc_convert_start_IT(void)
{
    __ADC_CONVERT_ENABLE();
    __ADC_INT_ENABLE(ADC_INT_CHANNEL_VALID);
}
void adc_convert_stop(void)
{
    __ADC_CONVERT_DISABLE();
    __ADC_INT_DISABLE(ADC_INT_CHANNEL_VALID);
}

/******************************************************************************
 * @fn      adc_channel_valid_int_enable/disable
 *
 * @brief   channel valid interrupt enable/disable.
 */
void adc_channel_valid_int_enable(void)
{
    __ADC_INT_ENABLE(ADC_INT_CHANNEL_VALID);
}
void adc_channel_valid_int_disable(void)
{
    __ADC_INT_DISABLE(ADC_INT_CHANNEL_VALID);
}

/******************************************************************************
 * @fn      adc_get_channel_valid_status
 *
 * @brief   get Channel valid status.
 * 
 * @param   fu8_Channel : channel number(0 ~ 7)
 */
bool adc_get_channel_valid_status(uint8_t fu8_Channel)
{
    return (__ADC_GET_CHANNEL_STATUS() & (1 << fu8_Channel)) ? true : false;
}

/******************************************************************************
 * @fn      adc_get_channel_data
 *
 * @brief   get Channel convert Data.
 * @param   fu8_Channel : channel number(0 ~ 7)
 */
uint32_t adc_get_channel_data(uint8_t fu8_Channel)
{
    return __ADC_GET_CHANNEL_DATA(fu8_Channel);
}

/******************************************************************************
 * @fn      adc_set_channel_maping
 *
 * @brief   set Channel maping.
 * 
 * @param   fu8_Channel : channel number(0 ~ 7)
 * @param   fe_Map      : channel map select.
 */
void adc_set_channel_maping(uint8_t fu8_Channel, enum_ADC_Channel_Map_t fe_Map)
{
    __ADC_SET_CHANNEL_MAP(fu8_Channel, fe_Map);
}
/*********************************************************************
 * @fn      adc_get_channel_data_FT
 *
 * @brief   get Channel convert Data(Not use FIFO).
 *          Calibrate ADC results using FT test param.
 * 
 * @param   fu8_Channel : channel number(0 ~ 3)
 */
uint32_t adc_get_channel_data_FT(uint8_t fu8_Channel)
{
    uint32_t lu32_ADC;
    uint32_t lu32_Result;
    
    struct_ADC_Cal_Param_t *ADC_Cal = trim_get_adc_cal_param();

    lu32_ADC = __ADC_GET_CHANNEL_DATA(fu8_Channel);
    
    lu32_ADC *= 1000;

    switch(ADC->ACT0)
    {
        /* reference select LDOIO */
        case ADC_ACT0_REF_IOLDO:
        {
            lu32_Result = (lu32_ADC - ADC_Cal->s32_constantB) / ADC_Cal->u16_slopeB;
            lu32_Result = (3103 * lu32_Result) / 1000;
         
        }break;
        
        /* reference select 1.2V */
        case ADC_ACT0_REF_1P2V:
        {
            lu32_Result = (lu32_ADC - ADC_Cal->s32_constantA) / ADC_Cal->u16_slopeA;
            lu32_Result = (8533 * lu32_Result) / 1000;
           
        }break;
    }

    return lu32_Result;
}
