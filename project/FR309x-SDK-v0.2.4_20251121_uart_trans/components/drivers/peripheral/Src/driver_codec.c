/*
  ******************************************************************************
  * @file    driver_codec.c
  * @author  FreqChip Firmware Team
  * @version V1.0.0
  * @date    2022
  * @brief   codec HAL module driver.
  *          This file provides firmware functions to manage the 
  *          codec, include ADC, DAC, DRC, GPF, etc.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 FreqChip.
  * All rights reserved.
  ******************************************************************************
*/
#include "fr30xx.h"

/************************************************************************************
 * @fn      codec_ASRC0_config
 *
 * @brief   Asynchronous Sampling Rate converter Config.
 */
__attribute__((weak)) void codec_ASRC0_config(CODEC_HandleTypeDef *hcodec)
{
    __CODEC_ASRC0_BYPASS();
}

/************************************************************************************
 * @fn      codec_GPF0_config
 *
 * @brief   general purpose filter Config.
 */
__attribute__((weak)) void codec_GPF0_config(CODEC_HandleTypeDef *hcodec)
{
    __CODEC_GPF0_BYPASS();
}

/************************************************************************************
 * @fn      codec_ALC_config
 *
 * @brief   automatic gain control Config.
 */
__attribute__((weak)) void codec_ALC_config(CODEC_HandleTypeDef *hcodec)
{
    __CODEC_ALC_BYPASS();
}

/************************************************************************************
 * @fn      codec_ASRC1_config
 *
 * @brief   Asynchronous Sampling Rate converter Config.
 */
__attribute__((weak)) void codec_ASRC1_config(CODEC_HandleTypeDef *hcodec)
{
    __CODEC_ASRC1_BYPASS();
}

/************************************************************************************
 * @fn      codec_GPF1_config
 *
 * @brief   general purpose filter Config.
 */
__attribute__((weak)) void codec_GPF1_config(CODEC_HandleTypeDef *hcodec)
{
    __CODEC_GPF1_BYPASS();
}

/************************************************************************************
 * @fn      codec_DRC_config
 *
 * @brief   Dynamic Range Control Config.
 */
__attribute__((weak)) void codec_DRC_config(CODEC_HandleTypeDef *hcodec)
{
    __CODEC_DRC_BYPASS();
}

/************************************************************************************
 * @fn      codec_analog_config
 *
 * @brief   analog Config.
 */
__attribute__((weak)) void codec_analog_config(CODEC_HandleTypeDef *hcodec)
{
    CODEC->CodecAna0 = CODEC_ANALOG0_CFG;
    CODEC->CodecAna1 = CODEC_ANALOG1_CFG;
    CODEC->CodecAna2 = CODEC_ANALOG2_CFG;
    CODEC->CodecAna3 = CODEC_ANALOG3_CFG;
}

/************************************************************************************
 * @fn      codec_input_config
 *
 * @brief   Input routing config.
 */
void codec_input_config(CODEC_HandleTypeDef *hcodec)
{
    /* Input ADC config */
    if (hcodec->Init.Codec_Input_sel == INPUT_SELECT_ADC)
    {
        /* ADC clock cource */
        CODEC->AdcCfg.CLK_SOURCE = hcodec->InputConfig.ADC_ClockSource;    /* Default USB 24M */

        if (CODEC->AdcCfg.CLK_SOURCE == CODEC_CLOCK_SOURCE_24M_MODE)
            CODEC->AdcCfg.USB_MODE = 1;
        else
            CODEC->AdcCfg.USB_MODE = 0;

        /* ADC Over sampling Level */
        CODEC->AdcCfg.OV_SAMPLE_MODE = hcodec->InputConfig.ADC_Oversampling_Level;
        /* ADC Sample Rate */
        CODEC->AdcCfg.SAMPLE_RATE = hcodec->InputConfig.ADC_SampleRate;

        /* Clock/Module control */
        __CODEC_ADC_CLK_ENABLE();
        __CODEC_I2S_RX_CLK_DISABLE();

        __CODEC_ADC_RESET();
        while(__CODEC_GET_RESET_STATUS() & CODEC_RESET_ADC);

        __CODEC_ADC_ENABLE();
        __CODEC_I2S_RX_DISABLE();
    }
    /* Input I2S config */
    else
    {
        /* Master/Slave */
        CODEC->I2SRxCtrl.I2S_MODE   = hcodec->InputConfig.I2S_RX_Mode;
        /* communication standard */
        CODEC->I2SRxCtrl.FORMAT     = hcodec->InputConfig.I2S_RX_Standard;
        /* data format */
        CODEC->I2SRxCtrl.DCI_WL     = hcodec->InputConfig.I2S_RX_DataFormat;
        /* Channel Copy Enable */
        CODEC->I2SRxCtrl.CHANNEL_CP = hcodec->InputConfig.I2S_RX_ChannelCopy;

        if (hcodec->InputConfig.I2S_RX_Mode == CODEC_I2S_MODE_MASTER)
        {
            /* frequency */
            //hcodec->InputConfig.I2S_TX_AudioFreq;
        }

        /* Clock/Module control */
        __CODEC_ADC_CLK_DISABLE();
        __CODEC_I2S_RX_CLK_ENABLE();

        __CODEC_ADC_DISABLE();
        __CODEC_I2S_RX_ENABLE();
    }

    /* Input routing config */
    switch (hcodec->Init.Codec_Input_Routing_sel)
    {
        /* IN -> ASRC0 -> GPF0 -> ALC -> ADC_FIFO */
        case INPUT_ROUTING_ASRC0_GPF0_ALC_ADCFF:
        {
            /* Codec rxin */
            __CODEC_ASRC0_SOURCE_SELECT(0);
            /* ASRC0 out */
            __CODEC_GPF0_CH0_SOURCE_SELECT(2);
            __CODEC_GPF0_CH1_SOURCE_SELECT(2);

            /* Clock control */
            __CODEC_ASRC0_CLK_ENABLE();
            __CODEC_ASRC0_INPUT_SAMPLE_CLK_ENABLE();
            __CODEC_ASRC0_OUTPUT_SAMPLE_CLK_ENABLE();
            __CODEC_ASRC0_OUT_LVD_CLK_ENABLE();
            __CODEC_GPF0_CLK_ENABLE();
            __CODEC_ALC_CLK_ENABLE();
            __CODEC_ADCFIFO_CLK_ENABLE();

            /* Module control */
            __CODEC_ASRC0_ENABLE();
            __CODEC_GPF0_ENABLE();
            __CODEC_ALC_ENABLE();

            codec_ASRC0_config(hcodec);
            codec_GPF0_config(hcodec);
            codec_ALC_config(hcodec);
        }break;

        /* IN -> GPF0 -> ALC -> ADC_FIFO */
        case INPUT_ROUTING_GPF0_ALC_ADCFF:
        {
            /* ASRC0 bypass */
            __CODEC_ASRC0_BYPASS();
            
            /* Codec rxin */
            __CODEC_GPF0_CH0_SOURCE_SELECT(3);
            __CODEC_GPF0_CH1_SOURCE_SELECT(3);

            /* Clock control */
            __CODEC_ASRC0_CLK_DISABLE();
            __CODEC_ASRC0_INPUT_SAMPLE_CLK_DISABLE();
            __CODEC_ASRC0_OUTPUT_SAMPLE_CLK_DISABLE();
            __CODEC_ASRC0_OUT_LVD_CLK_DISABLE();
            __CODEC_GPF0_CLK_ENABLE();
            __CODEC_ALC_CLK_ENABLE();
            __CODEC_ADCFIFO_CLK_ENABLE();

            /* Module control */
            __CODEC_ASRC0_DISABLE();
            __CODEC_GPF0_ENABLE();
            __CODEC_ALC_ENABLE();

            codec_GPF0_config(hcodec);
            codec_ALC_config(hcodec);
        }break;

        default:break;
    }
}

/************************************************************************************
 * @fn      codec_input_config
 *
 * @brief   output routing config.
 */
void codec_output_config(CODEC_HandleTypeDef *hcodec)
{
    /* Output DAC config */
    if (hcodec->Init.Codec_Output_sel == OUTPUT_SELECT_DAC)
    {
        /* ADC clock cource */
        CODEC->DacCfg.CLK_SOURCE = hcodec->OutputConfig.DAC_ClockSource;    /* Default USB 24M */

        if (CODEC->DacCfg.CLK_SOURCE == CODEC_CLOCK_SOURCE_24M_MODE)
            CODEC->DacCfg.USB_MODE = 1;
        else
            CODEC->DacCfg.USB_MODE = 0;

        /* DAC Over sampling Level */
        CODEC->DacCfg.OV_SAMPLE_MODE = hcodec->OutputConfig.DAC_Oversampling_Level;
        /* DAC Sample Rate */
        CODEC->DacCfg.SAMPLE_RATE    = hcodec->OutputConfig.DAC_SampleRate;

        /* Clock control */
        __CODEC_DAC_CLK_ENABLE();
        __CODEC_I2S_TX_CLK_DISABLE();

        __CODEC_DAC_RESET();
        while(__CODEC_GET_RESET_STATUS() & CODEC_RESET_DAC);

        __CODEC_DAC_ENABLE();
        __CODEC_I2S_TX_DISABLE();
    }   
    /* Output I2S config */
    else
    {
        /* Master/Slave */
        CODEC->I2STxCtrl.I2S_MODE   = hcodec->OutputConfig.I2S_TX_Mode;
        /* communication standard */
        CODEC->I2STxCtrl.FORMAT     = hcodec->OutputConfig.I2S_TX_Standard;
        /* data format */
        CODEC->I2STxCtrl.DCI_WL     = hcodec->OutputConfig.I2S_TX_DataFormat;
        /* Channel Copy Enable */
        CODEC->I2STxCtrl.CHANNEL_CP = hcodec->OutputConfig.I2S_TX_ChannelCopy;

        if (hcodec->OutputConfig.I2S_TX_Mode == CODEC_I2S_MODE_MASTER)
        {
            /* frequency */
            //hcodec->InputConfig.I2S_TX_AudioFreq;
        }

        /* Clock control */
        __CODEC_DAC_CLK_DISABLE();
        __CODEC_I2S_TX_CLK_ENABLE();
    }

   /* Output routing config */
    switch (hcodec->Init.Codec_Output_Routing_sel)
    {
        /* DACFIFO_LR -> ASRC1 -> GPF1 -> DRC -> OUT */
        case OUTPUT_ROUTING_DACFFLR_ASRC1_GPF1_DRC:
        {
            /* TxFIFO_LR */
            __CODEC_ASRC1_SOURCE_SELECT(1);

            /* ASRC1 out */
            __CODEC_GPF1_CH0_SOURCE_SELECT(3);
            __CODEC_GPF1_CH1_SOURCE_SELECT(3);

            /* Clock control */
            __CODEC_ASRC1_CLK_ENABLE();
            __CODEC_ASRC1_INPUT_SAMPLE_CLK_ENABLE();
            __CODEC_ASRC1_OUTPUT_SAMPLE_CLK_ENABLE();
            __CODEC_ASRC1_OUT_LVD_CLK_ENABLE();
            __CODEC_GPF1_CLK_ENABLE();
            __CODEC_DRC_CLK_ENABLE();

            __CODEC_DACFIFO_LR_CLK_ENABLE();
            __CODEC_DACFIFO_01_CLK_DISABLE();

            /* Module control */
            __CODEC_DAC_FIFO_LR_ENABLE();
            __CODEC_DAC_FIFO_01_DISABLE();
            __CODEC_ASRC1_ENABLE();
            __CODEC_GPF1_ENABLE();
            __CODEC_DRC_ENABLE();

            codec_ASRC1_config(hcodec);
            codec_GPF1_config(hcodec);
            codec_DRC_config(hcodec);
        }break;

        /* DACFIFO_LR -> GPF1 -> DRC -> OUT */
        case OUTPUT_ROUTING_DACFFLR_GPF1_DRC:
        {
            /* ASRC1 bypass */
            __CODEC_ASRC1_BYPASS();
            
            /* TxFIFO_LR*/
            __CODEC_GPF1_CH0_SOURCE_SELECT(1);
            __CODEC_GPF1_CH1_SOURCE_SELECT(1);

            /* Clock control */
            __CODEC_ASRC1_CLK_DISABLE();
            __CODEC_ASRC1_INPUT_SAMPLE_CLK_DISABLE();
            __CODEC_ASRC1_OUTPUT_SAMPLE_CLK_DISABLE();
            __CODEC_ASRC1_OUT_LVD_CLK_DISABLE();
            __CODEC_GPF1_CLK_ENABLE();
            __CODEC_DRC_CLK_ENABLE();

            __CODEC_DACFIFO_LR_CLK_ENABLE();
            __CODEC_DACFIFO_01_CLK_DISABLE();

            /* Module control */
            __CODEC_DAC_FIFO_LR_ENABLE();
            __CODEC_DAC_FIFO_01_DISABLE();
            __CODEC_ASRC1_DISABLE();
            __CODEC_GPF1_ENABLE();
            __CODEC_DRC_ENABLE();

            codec_GPF1_config(hcodec);
            codec_DRC_config(hcodec);
        }break;

        /* DACFIFO_LR & DACFIFO_01 -> ASRC1 -> GPF1 -> DRC -> OUT */
        case OUTPUT_ROUTING_DACFFLR_DACFF01_ASRC1_GPF1_DRC:
        {
            /* TxFIFO_01 */
           __CODEC_ASRC1_SOURCE_SELECT(1);

            /* TxFIFO_LR*/
            __CODEC_GPF1_CH0_SOURCE_SELECT(3);
            /* ASRC1 out */
            __CODEC_GPF1_CH1_SOURCE_SELECT(3);

            /* Clock control */
            __CODEC_ASRC1_CLK_ENABLE();
            __CODEC_ASRC1_INPUT_SAMPLE_CLK_ENABLE();
            __CODEC_ASRC1_OUTPUT_SAMPLE_CLK_ENABLE();
            __CODEC_ASRC1_OUT_LVD_CLK_ENABLE();
            __CODEC_GPF1_CLK_ENABLE();
            __CODEC_DRC_CLK_ENABLE();

            __CODEC_DACFIFO_LR_CLK_ENABLE();
            __CODEC_DACFIFO_01_CLK_ENABLE();

            /* Module control */
            __CODEC_DAC_FIFO_LR_ENABLE();
            __CODEC_DAC_FIFO_01_ENABLE();
            __CODEC_ASRC1_ENABLE();
            __CODEC_GPF1_ENABLE();
            __CODEC_DRC_ENABLE();

            codec_ASRC1_config(hcodec);
            codec_GPF1_config(hcodec);
            codec_DRC_config(hcodec);
        }break;

        /* DACFIFO_LR & DACFIFO_01 -> GPF1 -> DRC -> OUT */
        case OUTPUT_ROUTING_DACFFLR_DACFF01_GPF1_DRC:
        {
            /* ASRC1 bypass */
            __CODEC_ASRC1_BYPASS();
            
            /* TxFIFO_LR*/
            __CODEC_GPF1_CH0_SOURCE_SELECT(1);
            /* TxFIFO_01 */
            __CODEC_GPF1_CH1_SOURCE_SELECT(0);

            /* Clock control */
            __CODEC_ASRC1_CLK_DISABLE();
            __CODEC_ASRC1_INPUT_SAMPLE_CLK_DISABLE();
            __CODEC_ASRC1_OUTPUT_SAMPLE_CLK_DISABLE();
            __CODEC_ASRC1_OUT_LVD_CLK_DISABLE();
            __CODEC_GPF1_CLK_ENABLE();
            __CODEC_DRC_CLK_ENABLE();

            __CODEC_DACFIFO_LR_CLK_ENABLE();
            __CODEC_DACFIFO_01_CLK_ENABLE();

            /* Module control */
            __CODEC_DAC_FIFO_LR_ENABLE();
            __CODEC_DAC_FIFO_01_ENABLE();
            __CODEC_ASRC1_DISABLE();
            __CODEC_GPF1_ENABLE();
            __CODEC_DRC_ENABLE();


            codec_GPF1_config(hcodec);
            codec_DRC_config(hcodec);
        }break;
        
        /* DACFIFO_LR & DACFIFO_01 -> GPF1 -> DRC -> OUT -> ASRC1 */
        case OUTPUT_ROUTING_DACFFLR_DACFF01_GPF1_DRC_ASRC1:
        {
            /* TxFIFO_01 */
           __CODEC_ASRC1_SOURCE_SELECT(0);

            /* TxFIFO_LR*/
            __CODEC_GPF1_CH0_SOURCE_SELECT(1);
            /* ASRC1 out */
            __CODEC_GPF1_CH1_SOURCE_SELECT(1);

            /* Clock control */
            __CODEC_ASRC1_CLK_ENABLE();
            __CODEC_ASRC1_INPUT_SAMPLE_CLK_ENABLE();
            __CODEC_ASRC1_OUTPUT_SAMPLE_CLK_ENABLE();
            __CODEC_ASRC1_OUT_LVD_CLK_ENABLE();
            __CODEC_GPF1_CLK_ENABLE();
            __CODEC_DRC_CLK_ENABLE();

            __CODEC_DACFIFO_LR_CLK_ENABLE();
            __CODEC_DACFIFO_01_CLK_ENABLE();

            /* Module control */
            __CODEC_DAC_FIFO_LR_ENABLE();
            __CODEC_DAC_FIFO_01_ENABLE();
            __CODEC_ASRC1_ENABLE();
            __CODEC_GPF1_ENABLE();
            __CODEC_DRC_ENABLE();

            codec_ASRC1_config(hcodec);
            codec_GPF1_config(hcodec);
            codec_DRC_config(hcodec);
        }break;

        default:break;
    }
}

/************************************************************************************
 * @fn      codec_init
 *
 * @brief   Initialize the Codec according to the specified parameters.
 *
 * @param   hcodec: Codec handle.
 */
void codec_init(CODEC_HandleTypeDef *hcodec)
{
    /* FIFO Data Format */
    CODEC->FFRoute.ADCFF_L_SRC = 0;
    CODEC->FFRoute.ADCFF_R_SRC = 0;
    CODEC->FFRoute.ADCFF_BITWD    = hcodec->Init.ADC_DataFormat;
    CODEC->FFRoute.DACFF_LR_BITWD = hcodec->Init.DAC_LR_DataFormat;
    CODEC->FFRoute.DACFF_01_BITWD = hcodec->Init.DAC_01_DataFormat;

    /* Analog config */
    codec_analog_config(hcodec);

    /* Input routing config */
    if (hcodec->Init.Codec_Input_sel < INPUT_SELECT_BYPASS)
    {
        /* Input select */
        CODEC->CodecRoute.ADC_SEL = hcodec->Init.Codec_Input_sel;
        
        codec_input_config(hcodec);
    }

    /* Output routing config */
    if (hcodec->Init.Codec_Output_sel < OUTPUT_SELECT_BYPASS)
    {
        /* Output select */
        CODEC->CodecRoute.DAC_SEL = hcodec->Init.Codec_Output_sel;
        
        codec_output_config(hcodec);
    }

    /* MCLK Out config */
    if (hcodec->Init.Codec_Input_sel  == INPUT_SELECT_I2S_RX ||  \
        hcodec->Init.Codec_Output_sel == OUTPUT_SELECT_I2S_TX)
    {
        if (hcodec->MCLKConfig.MCLK_Out_Enable == MCLK_OUT_ENABLE)
        {
            CODEC->MCLKCfg.EN       = MCLK_OUT_ENABLE;
            CODEC->MCLKCfg.MCLK_SRC = hcodec->MCLKConfig.MCLK_Source;
            CODEC->MCLKCfg.DIV_TYPE = hcodec->MCLKConfig.MCLK_DIV_Mode;
            CODEC->MCLKCfg.DIV      = hcodec->MCLKConfig.MCLK_DIV;
        }
    }
}

/************************************************************************************
 * @fn      codec_deinit
 *
 * @brief   Close the Codec.
 *
 * @param   hcodec: Codec handle.
 */
void codec_deinit(CODEC_HandleTypeDef *hcodec)
{
    __CODEC_ALL_MODULE_RESET();
    __CODEC_ALL_DISABLE();
    __SYSTEM_CODEC_RESET();
}

/************************************************************************************
 * @fn      codec_Set_ADC_Volume
 *
 * @brief   Set ADC Volume.
 *
 * @param   fu32_Volume: volume = 20*lg(fu32_Volume/2^13)
 */
void codec_Set_ADC_Volume(uint32_t fu32_Volume)
{
    CODEC->AdcVolume.VOLUME = fu32_Volume;
}

/************************************************************************************
 * @fn      codec_Set_DAC_Volume
 *
 * @brief   Set DAC Volume.
 *
 * @param   fu32_Volume: volume = 20*lg(fu32_Volume/2^12)
 */
void codec_Set_DAC_Volume(uint32_t fu32_Volume)
{
    CODEC->DacVolume.VOLUME = fu32_Volume;
}

/************************************************************************************
 * @fn      codec_set_spk_vol
 *
 * @brief   Set anlog spk volume.
 *
 * @param   volume: volume [0,0x3f]
 */
void codec_set_spk_vol(uint8_t volume)
{
    if (volume == 0)
    {
        CODEC->CodecAna2 |= 0x40000000;//mute
        return;
    }
    else if (volume > 0x3f)
    {
        volume = 0x3f;
    }
    CODEC->CodecAna2 = (CODEC->CodecAna2 & 0xbfffc0ff) | (volume << 8);
}

/************************************************************************************
 * @fn      codec_set_mic_gain
 *
 * @brief   Set anlog  mic gain.
 *
 * @param   gain: mic gain, bit0-3 for gain1(-15db --- 30db), bit4-6 for gain2(0db -- 24db) [000b --- 100b]
 *          max value is 0x4f, 0x47 as default
 */
void codec_set_mic_gain(uint8_t gain)
{
    if (gain == 0)
    {
        CODEC->CodecAna1 |= 0x80000000;//mute
        return;
    }
    else if (gain > 0x4f)
    {
        gain = 0x4f;
    }
    CODEC->CodecAna1 = (CODEC->CodecAna1 & 0x00ffffff) | (gain << 24);
}
/************************************************************************************
 * @fn      codec_int_enable
 *
 * @brief   Codec interrupt enable.
 *
 * @param   fe_Int_Status: interrupt source select
 */
void codec_int_enable(enum_Codec_INT_Status_t fe_Int_Status)
{
    CODEC->CodecIntEn.STATUS |= fe_Int_Status;
}

/************************************************************************************
 * @fn      codec_int_enable
 *
 * @brief   Codec interrupt enable.
 *
 * @param   fe_Int_Status: interrupt source select
 */
void codec_int_disable(enum_Codec_INT_Status_t fe_Int_Status)
{
    CODEC->CodecIntEn.STATUS &= ~fe_Int_Status;
}

/************************************************************************************
 * @fn      codec_init_clear
 *
 * @brief   Codec interrupt enable.
 *
 * @param   fe_Int_Status: interrupt source select
 */
void codec_init_clear(enum_Codec_INT_Status_t fe_Int_Status)
{
    CODEC->CodecIntClear.STATUS |= fe_Int_Status;
}

/************************************************************************************
 * @fn      codec_read_adc_fifo_left/
 *          codec_read_adc_fifo_right/
 *          codec_read_adc_fifo_left_right/
 *
 * @brief   Codec read ADC fifo.
 *
 * @param   fp_buffer: codec ADC data buffer.
 * @param   fu32_size: read data length.
 */
void codec_read_adc_fifo_left(void *fp_buffer, uint32_t fu32_length)
{
    union_DataFormat_t Data;

    switch (CODEC->FFRoute.ADCFF_BITWD)
    {
        case CODEC_FIFO_FORMAT_8BIT:
        {
            Data.p_u8 = fp_buffer;
            while(fu32_length--)
                *Data.p_u8++ = (uint8_t)CODEC->ADCDataL;
        }break;

        case CODEC_FIFO_FORMAT_16BIT:
        {
            Data.p_u16 = fp_buffer;
            while(fu32_length--)
                *Data.p_u16++ = (uint16_t)CODEC->ADCDataL;
        }break; 

        case CODEC_FIFO_FORMAT_24BIT:
        {
            Data.p_u32 = fp_buffer;
            while(fu32_length--)
                *Data.p_u32++ = CODEC->ADCDataL;
        }break; 

        default:break;
    }
}
void codec_read_adc_fifo_right(void *fp_buffer, uint32_t fu32_length)
{
    union_DataFormat_t Data;

    switch (CODEC->FFRoute.ADCFF_BITWD)
    {
        case CODEC_FIFO_FORMAT_8BIT:
        {
            Data.p_u8 = fp_buffer;
            while(fu32_length--)
                *Data.p_u8++ = (uint8_t)CODEC->ADCDataR;
        }break;

        case CODEC_FIFO_FORMAT_16BIT:
        {
            Data.p_u16 = fp_buffer;
            while(fu32_length--)
                *Data.p_u16++ = (uint16_t)CODEC->ADCDataR;
        }break; 

        case CODEC_FIFO_FORMAT_24BIT:
        {
            Data.p_u32 = fp_buffer;
            while(fu32_length--)
                *Data.p_u32++ = CODEC->ADCDataR;
        }break; 

        default:break;
    }
}
void codec_read_adc_fifo_left_right(void *fp_buffer, uint32_t fu32_length)
{
    union_DataFormat_t Data;

    switch (CODEC->FFRoute.ADCFF_BITWD)
    {
        case CODEC_FIFO_FORMAT_8BIT:
        {
            Data.p_u8 = fp_buffer;
            while(fu32_length--){
                *Data.p_u8++ = (uint8_t)CODEC->ADCDataL;
                *Data.p_u8++ = (uint8_t)CODEC->ADCDataR;
            }
        }break;

        case CODEC_FIFO_FORMAT_16BIT:
        {
            Data.p_u16 = fp_buffer;
            while(fu32_length--){
                *Data.p_u16++ = (uint16_t)CODEC->ADCDataL;
                *Data.p_u16++ = (uint16_t)CODEC->ADCDataR;
            }
        }break; 

        case CODEC_FIFO_FORMAT_24BIT:
        {
            Data.p_u32 = fp_buffer;
            while(fu32_length--){
                *Data.p_u32++ = CODEC->ADCDataL;
                *Data.p_u32++ = CODEC->ADCDataR;
            }
        }break; 

        default:break;
    }
}

/************************************************************************************
 * @fn      codec_write_dac_fifo_left/
 *          codec_write_dac_fifo_right/
 *          codec_write_dac_fifo_left_right/
 *
 * @brief   Codec write dac fifo LR.
 *
 * @param   fp_buffer: codec dac data buffer.
 * @param   fu32_size: read data length.
 */
void codec_write_dac_fifo_left(void *fp_buffer, uint32_t fu32_length)
{
    union_DataFormat_t Data;

    switch (CODEC->FFRoute.DACFF_LR_BITWD)
    {
        case CODEC_FIFO_FORMAT_8BIT:
        {
            Data.p_u8 = fp_buffer;
            while(fu32_length--)
                CODEC->DACLRDataL = *Data.p_u8++;
        }break;

        case CODEC_FIFO_FORMAT_16BIT:
        {
            Data.p_u16 = fp_buffer;
            while(fu32_length--)
                CODEC->DACLRDataL = *Data.p_u16++;
        }break; 

        case CODEC_FIFO_FORMAT_24BIT:
        {
            Data.p_u32 = fp_buffer;
            while(fu32_length--)
                CODEC->DACLRDataL = *Data.p_u32++;
        }break; 

        default:break;
    }
}
void codec_write_dac_fifo_right(void *fp_buffer, uint32_t fu32_length)
{
    union_DataFormat_t Data;

    switch (CODEC->FFRoute.DACFF_LR_BITWD)
    {
        case CODEC_FIFO_FORMAT_8BIT:
        {
            Data.p_u8 = fp_buffer;
            while(fu32_length--)
                CODEC->DACLRDataR = *Data.p_u8++;
        }break;

        case CODEC_FIFO_FORMAT_16BIT:
        {
            Data.p_u16 = fp_buffer;
            while(fu32_length--)
                CODEC->DACLRDataR = *Data.p_u16++;
        }break; 

        case CODEC_FIFO_FORMAT_24BIT:
        {
            Data.p_u32 = fp_buffer;
            while(fu32_length--)
                CODEC->DACLRDataR = *Data.p_u32++;
        }break; 

        default:break;
    }
}
void codec_write_dac_fifo_left_right(void *fp_buffer, uint32_t fu32_length)
{
    union_DataFormat_t Data;

    switch (CODEC->FFRoute.DACFF_LR_BITWD)
    {
        case CODEC_FIFO_FORMAT_8BIT:
        {
            Data.p_u8 = fp_buffer;
            while(fu32_length--){
                CODEC->DACLRDataL = *Data.p_u8++;
                CODEC->DACLRDataR = *Data.p_u8++;
            }
        }break;

        case CODEC_FIFO_FORMAT_16BIT:
        {
            Data.p_u16 = fp_buffer;
            while(fu32_length--){
                CODEC->DACLRDataL = *Data.p_u16++;
                CODEC->DACLRDataR = *Data.p_u16++;
            }
        }break; 

        case CODEC_FIFO_FORMAT_24BIT:
        {
            Data.p_u32 = fp_buffer;
            while(fu32_length--){
                CODEC->DACLRDataL = *Data.p_u32++;
                CODEC->DACLRDataR = *Data.p_u32++;
            }
        }break; 

        default:break;
    }
}

/************************************************************************************
 * @fn      codec_write_dac_fifo_0/
 *          codec_write_dac_fifo_1/
 *          codec_write_dac_fifo_01/
 *
 * @brief   Codec write dac fifo 01.
 *
 * @param   fp_buffer: codec DAC data buffer.
 * @param   fu32_size: read data length.
 */
void codec_write_dac_fifo_0(void *fp_buffer, uint32_t fu32_length)
{
    union_DataFormat_t Data;

    switch (CODEC->FFRoute.DACFF_01_BITWD)
    {
        case CODEC_FIFO_FORMAT_8BIT:
        {
            Data.p_u8 = fp_buffer;
            while(fu32_length--)
                CODEC->DAC01DataL = *Data.p_u8++;
        }break;

        case CODEC_FIFO_FORMAT_16BIT:
        {
            Data.p_u16 = fp_buffer;
            while(fu32_length--)
                CODEC->DAC01DataL = *Data.p_u16++;
        }break; 

        case CODEC_FIFO_FORMAT_24BIT:
        {
            Data.p_u32 = fp_buffer;
            while(fu32_length--)
                CODEC->DAC01DataL = *Data.p_u32++;
        }break; 

        default:break;
    }
}
void codec_write_dac_fifo_1(void *fp_buffer, uint32_t fu32_length)
{
    union_DataFormat_t Data;

    switch (CODEC->FFRoute.DACFF_01_BITWD)
    {
        case CODEC_FIFO_FORMAT_8BIT:
        {
            Data.p_u8 = fp_buffer;
            while(fu32_length--)
                CODEC->DAC01DataR = *Data.p_u8++;
        }break;

        case CODEC_FIFO_FORMAT_16BIT:
        {
            Data.p_u16 = fp_buffer;
            while(fu32_length--)
                CODEC->DAC01DataR = *Data.p_u16++;
        }break; 

        case CODEC_FIFO_FORMAT_24BIT:
        {
            Data.p_u32 = fp_buffer;
            while(fu32_length--)
                CODEC->DAC01DataR = *Data.p_u32++;
        }break; 

        default:break;
    }
}
void codec_write_dac_fifo_01(void *fp_buffer, uint32_t fu32_length)
{
    union_DataFormat_t Data;

    switch (CODEC->FFRoute.DACFF_01_BITWD)
    {
        case CODEC_FIFO_FORMAT_8BIT:
        {
            Data.p_u8 = fp_buffer;
            while(fu32_length--){
                CODEC->DAC01DataL = *Data.p_u8++;
                CODEC->DAC01DataR = *Data.p_u8++;
            }
        }break;

        case CODEC_FIFO_FORMAT_16BIT:
        {
            Data.p_u16 = fp_buffer;
            while(fu32_length--){
                CODEC->DAC01DataL = *Data.p_u16++;
                CODEC->DAC01DataR = *Data.p_u16++;
            }
        }break; 

        case CODEC_FIFO_FORMAT_24BIT:
        {
            Data.p_u32 = fp_buffer;
            while(fu32_length--){
                CODEC->DAC01DataL = *Data.p_u32++;
                CODEC->DAC01DataR = *Data.p_u32++;
            }
        }break; 

        default:break;
    }
}

/************************************************************************************
 * @fn      codec_get_int_status
 *
 * @brief   get Codec interrupt status.
 */
enum_Codec_INT_Status_t codec_get_int_status(void)
{
    return CODEC->CodecIntRawStatus.STATUS;
}

/************************************************************************************
 * @fn      codec_mute_output
 *
 * @brief   codec mute output enable/disable.
 * 
 * @param   true: output enable.
 *          false: output disable. 
 */
void codec_mute_output(bool fb_mute)
{
    CODEC->Enable.CDC_TXOUT = fb_mute;
}