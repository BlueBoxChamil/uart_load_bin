#include "audio_hw.h"
#include "audio_common.h"

#include "fr30xx.h"
#include "FreeRTOS.h"

#define AUDIO_HW_SPDIF_RX_INT_LEVEL         (SPDIF_FIFO_DEPTH/4)
#define AUDIO_HW_SPDIF_TX_INT_LEVEL         (SPDIF_FIFO_DEPTH/4)
#define AUDIO_HW_PDM_RX_INT_LEVEL           16
#define AUDIO_HW_STORE_FRAME_COUNT          40

struct audio_hw_env_t {
    struct co_list hw_list;
};

struct audio_hw_handle_t {
    audio_hw_t *audio_hw;
    void *hw_handle;
};

static struct audio_hw_env_t audio_hw_env = {0};
static struct audio_hw_handle_t i2s_hw_table[3];
static struct audio_hw_handle_t pdm_hw_table[3];
static struct audio_hw_handle_t spdif_hw_table[1];

static struct audio_hw_handle_t codec_hw;

static void i2s_rx_callback(I2S_HandleTypeDef *i2s_handle)
{
    uint8_t index;
    audio_hw_t *hw;

    for (index=0; index<3; index++) {
        if (i2s_hw_table[index].hw_handle == i2s_handle) {
            break;
        }
    }
    
    if (index < 3) {
        hw = i2s_hw_table[index].audio_hw;

        /* store data into internal buffer */        
        if (hw->channels == AUDIO_CHANNELS_MONO) {
            int16_t *pcm = (void *)&hw->pcm[hw->channels * sizeof(int16_t) * hw->wr_ptr];
            for (uint32_t i=0; i<I2S_FIFO_HALF_DEPTH;) {
                pcm[i++] = i2s_handle->I2Sx->DATA_L;
            }
        }
        else {
            uint32_t *pcm = (void *)&hw->pcm[hw->channels * sizeof(int16_t) * hw->wr_ptr];
            for (uint32_t i=0; i<I2S_FIFO_HALF_DEPTH;) {
                pcm[i++] = i2s_handle->I2Sx->DATA_L;
            }
        }
        
        hw->wr_ptr += I2S_FIFO_HALF_DEPTH;
        if (hw->wr_ptr >= hw->pcm_samples) {
            hw->wr_ptr = 0;
        }

        /* notify receivers new data are available */
        audio_hw_output_t *output;
        output = (void *)co_list_pick(&hw->output_list);
        while (output) {
            if (output->handler) {
                output->handler(I2S_FIFO_HALF_DEPTH);
            }
            
            output = (void *)output->hdr.next;
        }
    }
}

static void i2s_tx_callback(I2S_HandleTypeDef *i2s_handle)
{
    uint8_t index;
    audio_hw_t *hw;

    for (index=0; index<3; index++) {
        if (i2s_hw_table[index].hw_handle == i2s_handle) {
            break;
        }
    }
    
    if (index < 3) {
        hw = i2s_hw_table[index].audio_hw;
        
        /* request new data to send through I2S */
        if (hw->request_handler) {
            hw->request_handler(hw->pcm_out, I2S_FIFO_HALF_DEPTH, hw->channels);
        }
        else {
            memset(hw->pcm_out, 0, hw->channels * sizeof(int16_t) * I2S_FIFO_HALF_DEPTH);
        }
        if (hw->channels == AUDIO_CHANNELS_MONO) {
            for (uint32_t i=0; i<I2S_FIFO_HALF_DEPTH;) {
                i2s_handle->I2Sx->DATA_L = hw->pcm_out[i++];
            }
        }
        else {
            uint32_t *pcm = (void *)&hw->pcm_out[0];
            if(i2s_handle->Init.DataFormat == I2S_DATA_FORMAT_16BIT) {
                for (uint32_t i=0; i<I2S_FIFO_HALF_DEPTH;i++) {
                    i2s_handle->I2Sx->DATA_L = pcm[i];
                }
            }
            else if(i2s_handle->Init.DataFormat == I2S_DATA_FORMAT_24BIT) {
                for (uint32_t i=0; i<I2S_FIFO_HALF_DEPTH * AUDIO_CHANNELS_STEREO;) {
                    i2s_handle->I2Sx->DATA_L = hw->pcm_out[i++];
                    i2s_handle->I2Sx->DATA_R = hw->pcm_out[i++];
                }
            }
        }
    }
}

#ifndef LE_AUDIO_LOW_LATENCY
#define CODEC_DATA_SIZE     CODEC_FIFO_HALF_DEPTH
#else
#define CODEC_DATA_SIZE     30
#endif

void codec_irq(void)
{
    uint32_t status;
    audio_hw_t *hw;
    
    status = __CODEC_GET_INT_STATUS();
    
    if (status & (CODEC_INT_ADCFF_L_AFULL | CODEC_INT_ADCFF_L_FULL)) {
//        fputc('R', &__stdout);
        hw = codec_hw.audio_hw;

#ifndef LE_AUDIO_LOW_LATENCY
        /* store data into internal buffer */      
        uint32_t data_size = CODEC_DATA_SIZE;
#else
        /* store data into internal buffer */      
        uint32_t data_size = CODEC->FifoADCThd.LEFT_F;
#endif

        if (hw->channels == AUDIO_CHANNELS_MONO) {
            int16_t *pcm = (void *)&hw->pcm[hw->channels * sizeof(int16_t) * hw->wr_ptr];

            uint32_t copy_length = data_size;

            if ((hw->wr_ptr + data_size) > hw->pcm_samples)
            {
                copy_length = hw->pcm_samples - hw->wr_ptr;

                for (uint32_t i=0; i<copy_length;) {
                    pcm[i++] = CODEC->ADCDataL;
                }

                copy_length = data_size - copy_length;
                pcm = (int16_t *)&hw->pcm[0];
            }

            for (uint32_t i=0; i<copy_length;) {
                pcm[i++] = CODEC->ADCDataL;
            }
        }
        else {
            uint16_t *pcm = (void *)&hw->pcm[hw->channels * sizeof(int16_t) * hw->wr_ptr];

            uint32_t copy_length = data_size;

            if ((hw->wr_ptr + copy_length) > hw->pcm_samples)
            {
                copy_length = hw->pcm_samples - hw->wr_ptr;

                for (uint32_t i=0; i<copy_length;) {
                    uint16_t data = CODEC->ADCDataL;
                    *pcm++ = data;
                    *pcm++ = data;
                    i++;
                }

                copy_length = data_size - copy_length;
                pcm = (uint16_t *)&hw->pcm[0];
            }
            
            for (uint32_t i=0; i<copy_length;) {
                uint16_t data = CODEC->ADCDataL;
                *pcm++ = data;
                *pcm++ = data;
                i++;
            }
        }
        
        hw->wr_ptr += data_size;
        if (hw->wr_ptr >= hw->pcm_samples) {
            hw->wr_ptr = hw->wr_ptr - hw->pcm_samples;
        }

        /* notify receivers new data are available */
        audio_hw_output_t *output;
        output = (void *)co_list_pick(&hw->output_list);
        while (output) {
            if (output->handler) {
                output->handler(data_size);
            }
            
            output = (void *)output->hdr.next;
        }
    }
    
    if (status & (CODEC_INT_DACFF_L_AEMPTY | CODEC_INT_DACFF_L_EMPTY)) {
//        fputc('T', &__stdout);
        hw = codec_hw.audio_hw;
        
        /* request new data to send through Codec */
        if (hw->request_handler) {
            hw->request_handler(hw->pcm_out, CODEC_DATA_SIZE, hw->channels);
        }
        else {
            memset(hw->pcm_out, 0, hw->channels * sizeof(int16_t) * CODEC_DATA_SIZE);
        }
        
        #if 1
        if (hw->channels == AUDIO_CHANNELS_MONO) {
            for (uint32_t i=0; i<CODEC_DATA_SIZE;) {
                CODEC->DACLRDataL = hw->pcm_out[i++];
            }
        }
        else {
            uint32_t *pcm = (void *)&hw->pcm_out[0];
            int16_t left, right;
            for (uint32_t i=0; i<CODEC_FIFO_HALF_DEPTH; i++) {
                left = pcm[i] & 0xffff;
                right = (pcm[i]  >> 16) & 0xffff;
                CODEC->DACLRDataL = (left >> 1) + (right >> 1);
            }
        }
        #else
        const int16_t sin_pcm[] = {0, 4000, 8000, 12000, 16000, 20000, 24000, 28000, 32000, 28000, 24000, 20000, 16000, 12000, 8000, 4000,
                                    0, -4000, -8000, -12000, -16000, -20000, -24000, -28000, -32000, -28000, -24000, -20000, -16000, -12000, -8000, -4000};
        for (uint32_t i=0; i<CODEC_FIFO_HALF_DEPTH;) {
            CODEC->DACLRDataL = sin_pcm[i];
            CODEC->DACLRDataR = sin_pcm[i++];
        }
        #endif
    }
    
}

static void pdm_rx_callback(PDM_HandleTypeDef *pdm_handle)
{
    uint8_t index;
    audio_hw_t *hw;

    for (index=0; index<3; index++) {
        if (pdm_hw_table[index].hw_handle == pdm_handle) {
            break;
        }
    }
    
    if (index < 3) {
        hw = pdm_hw_table[index].audio_hw;

        /* store data into internal buffer */
        pdm_read_data(pdm_handle, (void *)&hw->pcm[hw->channels * sizeof(int16_t) * hw->wr_ptr]);
        hw->wr_ptr += AUDIO_HW_PDM_RX_INT_LEVEL;
        if (hw->wr_ptr >= hw->pcm_samples) {
            hw->wr_ptr = 0;
        }

        /* notify receivers new data are available */
        audio_hw_output_t *output;
        output = (void *)co_list_pick(&hw->output_list);
        while (output) {
            if (output->handler) {
                output->handler(AUDIO_HW_PDM_RX_INT_LEVEL);
            }
            
            output = (void *)output->hdr.next;
        }
    }
}

void spdif_rx_callback(SPDIF_HandleTypeDef *hspdif)
{
    audio_hw_t *hw = spdif_hw_table[0].audio_hw;
    
    spdif_read_data(hspdif, (void *)&hw->pcm[hw->channels * sizeof(int16_t) * hw->wr_ptr], AUDIO_HW_SPDIF_RX_INT_LEVEL * hw->channels);
    hw->wr_ptr += AUDIO_HW_SPDIF_RX_INT_LEVEL * hw->channels;
    if (hw->wr_ptr >= hw->pcm_samples) {
        hw->wr_ptr = 0;
    }
    
    /* notify receivers new data are available */
    audio_hw_output_t *output;
    output = (void *)co_list_pick(&hw->output_list);
    while (output) {
        if (output->handler) {
            output->handler(AUDIO_HW_SPDIF_RX_INT_LEVEL);
        }
        
        output = (void *)output->hdr.next;
    }
}

void spdif_tx_callback(SPDIF_HandleTypeDef *hspdif)
{
    audio_hw_t *hw = spdif_hw_table[0].audio_hw;
    
    /* request new data to send through Codec */
    if (hw->request_handler) {
        hw->request_handler(hw->pcm_out, AUDIO_HW_SPDIF_TX_INT_LEVEL, hw->channels);
    }
    else {
        memset(hw->pcm_out, 0, hw->channels * sizeof(int16_t) * AUDIO_HW_SPDIF_TX_INT_LEVEL);
    }

    spdif_send_data(hspdif, hw->pcm_out, hw->channels * AUDIO_HW_SPDIF_TX_INT_LEVEL);
}

void i2s0_irq(void)
{
//    fputc('I', NULL);
    i2s_IRQHandler(i2s_hw_table[0].hw_handle);
}

void i2s1_irq(void)
{
    i2s_IRQHandler(i2s_hw_table[1].hw_handle);
}

void i2s2_irq(void)
{
    i2s_IRQHandler(i2s_hw_table[2].hw_handle);
}

void pdm0_irq(void)
{
    pdm_IRQHandler(pdm_hw_table[0].hw_handle);
}

void pdm1_irq(void)
{
    pdm_IRQHandler(pdm_hw_table[1].hw_handle);
}

void pdm2_irq(void)
{
    pdm_IRQHandler(pdm_hw_table[2].hw_handle);
}

void spdif_irq(void)
{
    SPDIF_IRQHandler(spdif_hw_table[0].hw_handle);
}

static __attribute__((noinline)) void config_aupll(uint32_t freq)
{
    System_PLLConfig_t AUPLL_CFG;
    uint64_t tmp_u64;

    AUPLL_CFG.PLL_N = freq / 24000000;
    tmp_u64 = (uint64_t)(freq % 24000000) * 65535;
    AUPLL_CFG.PLL_M = tmp_u64 / 24000000;
    AUPLL_CFG.PowerEn = 1;

    System_AUPLL_config(&AUPLL_CFG, 1000);
}

void codec_ASRC0_config(CODEC_HandleTypeDef *hcodec)
{
    __CODEC_ASRC0_DISABLE();
    __CODEC_ASRC0_RESET();
    CODEC->ASRC[0].ClkCfg.DIV = 5;
    CODEC->ASRC[0].ClkCfg.CLK_SRC = 1;
    CODEC->ASRC[0].InputSR.DIV = (24000000 / 32000) - 1;
    CODEC->ASRC[0].OutputSR.DIV = (24000000 / 8000) - 1;
    CODEC->ASRC[0].RatioMode.MANUAL_RATIO = 0x100000;
    __CODEC_ASRC0_ENABLE();
    
    __CODEC_ADC_DISABLE();
    __CODEC_ADC_RESET();
    CODEC->AdcCfg.SAMPLE_RATE = CODEC_SAMPLE_RATE_32000;
    __CODEC_ADC_ENABLE();
}

void codec_ASRC1_config(CODEC_HandleTypeDef *hcodec)
{
    __CODEC_ASRC1_DISABLE();
    __CODEC_ASRC1_RESET();
    CODEC->ASRC[1].ClkCfg.DIV = 5;
    CODEC->ASRC[1].ClkCfg.CLK_SRC = 1;
    CODEC->ASRC[1].InputSR.DIV = (24000000 / 8000) - 1;
    CODEC->ASRC[1].OutputSR.DIV = (24000000 / 32000) - 1;
    CODEC->ASRC[1].RatioMode.MANUAL_RATIO = 0x1000000;
    __CODEC_ASRC1_ENABLE();
    
    __CODEC_DAC_DISABLE();
    __CODEC_DAC_RESET();
    CODEC->DacCfg.SAMPLE_RATE = CODEC_SAMPLE_RATE_32000;
    __CODEC_DAC_ENABLE();
}

audio_hw_t *audio_hw_create(audio_hw_type_t type, audio_hw_request_pcm_t handler, uint32_t base_addr, audio_hw_dir_t dir, uint32_t sample_rate, uint8_t channels)
{
    audio_hw_t *tmp;
    bool reject = false;
    
    /* check duplication */
    tmp = (void *)co_list_pick(&audio_hw_env.hw_list);
    while (tmp) {
        if (type == tmp->type) {
            reject = true;
        }
        tmp = (void *)tmp->hdr.next;
    }
    if (reject) {
        return NULL;
    }
    
    tmp = pvPortMalloc(sizeof(audio_hw_t));
    if (tmp == NULL) {
        goto __err;
    }
    tmp->type = type;
    tmp->dir = dir;
    tmp->channels = channels;
    tmp->sample_rate = sample_rate;
    tmp->base_addr = base_addr;
    tmp->hw_handle = NULL;
    tmp->request_handler = handler;
    tmp->pcm_out = NULL;
    tmp->wr_ptr = 0;
    co_list_init(&tmp->output_list);
    tmp->pcm = NULL;

    switch (type) {
        case AUDIO_HW_TYPE_I2S:
            {
                I2S_HandleTypeDef *i2s_handle = pvPortMalloc(sizeof(I2S_HandleTypeDef));
                if (i2s_handle == NULL) {
                    goto __err;
                }
                tmp->hw_handle = i2s_handle;
                
                switch (base_addr) {
                    case I2S0_BASE:
                        __SYSTEM_I2S0_CLK_ENABLE();
                        i2s_handle->I2Sx = I2S0;
                        i2s_hw_table[0].hw_handle = i2s_handle;
                        i2s_hw_table[0].audio_hw = tmp;
                        break;
                    case I2S1_BASE:
                        __SYSTEM_I2S1_CLK_ENABLE();
                        i2s_handle->I2Sx = I2S1;
                        i2s_hw_table[1].hw_handle = i2s_handle;
                        i2s_hw_table[1].audio_hw = tmp;
                        break;
                    case I2S2_BASE:
                        __SYSTEM_I2S2_CLK_ENABLE();
                        i2s_handle->I2Sx = I2S2;
                        i2s_hw_table[2].hw_handle = i2s_handle;
                        i2s_hw_table[2].audio_hw = tmp;
                        break;
                    default:
                        goto __err;
                }

                i2s_handle->Init.Mode = I2S_MODE_MASTER;
                i2s_handle->Init.Standard = I2S_STANDARD_PHILIPS;
                i2s_handle->Init.DataFormat = I2S_DATA_FORMAT_16BIT;
                if (sample_rate == 48000) {
                    config_aupll(147456000);
                    __SYSTEM_I2S_CLK_SELECT_AUPLL();
                    __SYSTEM_I2S_CLK_DIV(6);
                    
                    i2s_handle->Init.BCLKDIV = 8;
                    i2s_handle->Init.ChannelLength = 32;
                    i2s_handle->Init.AudioFreq = I2S_AUDIOFREQ_48000;
                }
                else if (sample_rate == 44100) {
                    config_aupll(158054400);
                    __SYSTEM_I2S_CLK_SELECT_AUPLL();
                    __SYSTEM_I2S_CLK_DIV(7);
                    
                    i2s_handle->Init.BCLKDIV = 8;
                    i2s_handle->Init.ChannelLength = 32;
                    i2s_handle->Init.AudioFreq = I2S_AUDIOFREQ_44100;
                }
                else if (sample_rate == 16000) {
                    config_aupll(147456000);
                    __SYSTEM_I2S_CLK_SELECT_AUPLL();
                    __SYSTEM_I2S_CLK_DIV(6);
                    
                    i2s_handle->Init.BCLKDIV = 24;
                    i2s_handle->Init.ChannelLength = 32;
                    i2s_handle->Init.AudioFreq = I2S_AUDIOFREQ_16000;
                }
                else if (sample_rate == 8000) {
                    config_aupll(147456000);
                    __SYSTEM_I2S_CLK_SELECT_AUPLL();
                    __SYSTEM_I2S_CLK_DIV(6);
                    
                    i2s_handle->Init.BCLKDIV = 48;
                    i2s_handle->Init.ChannelLength = 32;
                    i2s_handle->Init.AudioFreq = I2S_AUDIOFREQ_8000;
                }
                else {
                    goto __err;
                }
                
                /* insert new audio hw to list before enable interrupt */
                co_list_push_back(&audio_hw_env.hw_list, &tmp->hdr);

                /* Initialize and start I2S */
                i2s_init(i2s_handle);
                if (dir & AUDIO_HW_DIR_IN) {
                    tmp->pcm_samples = I2S_FIFO_HALF_DEPTH * AUDIO_HW_STORE_FRAME_COUNT;
                    tmp->pcm = (void *)pvPortMalloc(sizeof(int16_t) * channels * tmp->pcm_samples);
                    if (tmp->pcm == NULL) {
                        goto __err;
                    }

                    i2s_handle->RxIntCallback = i2s_rx_callback;
                    i2s_receive_IT(i2s_handle);
                }
                if (dir & AUDIO_HW_DIR_OUT) {
                    tmp->pcm_out = (void *)pvPortMalloc(sizeof(int16_t) * channels * I2S_FIFO_HALF_DEPTH);
                    if (tmp->pcm_out == NULL) {
                        goto __err;
                    }

                    i2s_handle->TxIntCallback = i2s_tx_callback;
                    i2s_transmit_IT(i2s_handle);
                }

                switch (base_addr) {
                    case I2S0_BASE:
                        NVIC_EnableIRQ(I2S0_IRQn);
                        break;
                    case I2S1_BASE:
                        NVIC_EnableIRQ(I2S1_IRQn);
                        break;
                    case I2S2_BASE:
                        NVIC_EnableIRQ(I2S2_IRQn);
                        break;
                    default:
                        goto __err;
                }
            }
            break;
        case AUDIO_HW_TYPE_CODEC:
            {
                codec_hw.hw_handle = pvPortMalloc(sizeof(CODEC_HandleTypeDef));
                CODEC_HandleTypeDef *CODEC_h = codec_hw.hw_handle;
                codec_hw.audio_hw = tmp;
                tmp->hw_handle = CODEC_h;
                if (dir & AUDIO_HW_DIR_IN) {
                    tmp->pcm_samples = CODEC_DATA_SIZE * AUDIO_HW_STORE_FRAME_COUNT;
                    tmp->pcm = (void *)pvPortMalloc(sizeof(int16_t) * channels * tmp->pcm_samples);
                    if (tmp->pcm == NULL) {
                        goto __err;
                    }
                }
                if (dir & AUDIO_HW_DIR_OUT) {
                    tmp->pcm_out = (void *)pvPortMalloc(sizeof(int16_t) * channels * CODEC_DATA_SIZE);
                    if (tmp->pcm_out == NULL) {
                        goto __err;
                    }
                }

                __SYSTEM_CODEC_CLK_ENABLE();

                CODEC_h->Init.Codec_Input_sel = INPUT_SELECT_ADC;
                CODEC_h->Init.Codec_Input_Routing_sel = INPUT_ROUTING_GPF0_ALC_ADCFF;
                
                CODEC_h->Init.Codec_Output_sel = OUTPUT_SELECT_DAC;
                CODEC_h->Init.Codec_Output_Routing_sel = OUTPUT_ROUTING_DACFFLR_GPF1_DRC;
                CODEC_h->Init.ADC_DataFormat = CODEC_FIFO_FORMAT_16BIT;
                CODEC_h->Init.DAC_LR_DataFormat = CODEC_FIFO_FORMAT_16BIT;
                CODEC_h->Init.DAC_01_DataFormat = CODEC_FIFO_FORMAT_16BIT;

                if ((sample_rate == 48000)
                    || (sample_rate == 32000)
                    || (sample_rate == 16000)
                    || (sample_rate == 8000)) {
                    config_aupll(24576000*7);
                    __CODEC_SET_AUPLL_DIV_SAMPLE(7);
                }
                else {
                    config_aupll(22579200*7);
                    __CODEC_SET_AUPLL_DIV_SAMPLE(7);
                }
                
                if (dir & AUDIO_HW_DIR_IN) {
                    CODEC_h->Init.Codec_Input_sel = INPUT_SELECT_ADC;
                    CODEC_h->InputConfig.ADC_Oversampling_Level = CODEC_OVERSAMPLING_HIGH;
                    if (sample_rate == 48000) {
                        CODEC_h->InputConfig.ADC_SampleRate = CODEC_SAMPLE_RATE_48000;
                        #ifndef LE_AUDIO_LOW_LATENCY
                        CODEC_h->InputConfig.ADC_ClockSource = CODEC_CLOCK_SOURCE_AUPLL;
                        #else
                        CODEC_h->InputConfig.ADC_ClockSource = CODEC_CLOCK_SOURCE_24M_MODE;
                        #endif
                    }
                    else if (sample_rate == 44100) {
                        CODEC_h->InputConfig.ADC_SampleRate = CODEC_SAMPLE_RATE_44100;
                        CODEC_h->InputConfig.ADC_ClockSource = CODEC_CLOCK_SOURCE_AUPLL;
                    }
                    else if (sample_rate == 16000) {
                        CODEC_h->InputConfig.ADC_SampleRate = CODEC_SAMPLE_RATE_16000;
                        CODEC_h->InputConfig.ADC_ClockSource = CODEC_CLOCK_SOURCE_AUPLL;
                    }
                    else if (sample_rate == 8000) {
                        CODEC_h->InputConfig.ADC_SampleRate = CODEC_SAMPLE_RATE_8000;
                        CODEC_h->InputConfig.ADC_ClockSource = CODEC_CLOCK_SOURCE_AUPLL;
                        
                        CODEC_h->Init.Codec_Input_Routing_sel = INPUT_ROUTING_ASRC0_GPF0_ALC_ADCFF;
                    }
                    else {
                        goto __err;
                    }
                }
                else {
                    CODEC_h->Init.Codec_Input_sel = INPUT_SELECT_BYPASS;
                }

                if (dir & AUDIO_HW_DIR_OUT) {
                    CODEC_h->Init.Codec_Output_sel = OUTPUT_SELECT_DAC;
                    CODEC_h->OutputConfig.DAC_Oversampling_Level = CODEC_OVERSAMPLING_HIGH;
                    if (sample_rate == 48000) {
                        CODEC_h->OutputConfig.DAC_SampleRate = CODEC_SAMPLE_RATE_48000;
                        #ifndef LE_AUDIO_LOW_LATENCY
                        CODEC_h->OutputConfig.DAC_ClockSource = CODEC_CLOCK_SOURCE_AUPLL;
                        #else
                        CODEC_h->OutputConfig.DAC_ClockSource = CODEC_CLOCK_SOURCE_24M_MODE;
                        #endif
                    }
                    else if (sample_rate == 44100) {
                        CODEC_h->OutputConfig.DAC_SampleRate = CODEC_SAMPLE_RATE_44100;
                        CODEC_h->OutputConfig.DAC_ClockSource = CODEC_CLOCK_SOURCE_AUPLL;
                    }
                    else if (sample_rate == 16000) {
                        CODEC_h->OutputConfig.DAC_SampleRate = CODEC_SAMPLE_RATE_16000;
                        CODEC_h->OutputConfig.DAC_ClockSource = CODEC_CLOCK_SOURCE_AUPLL;
                    }
                    else if (sample_rate == 8000) {
                        CODEC_h->OutputConfig.DAC_SampleRate = CODEC_SAMPLE_RATE_8000;
                        CODEC_h->OutputConfig.DAC_ClockSource = CODEC_CLOCK_SOURCE_AUPLL;
                        
                        CODEC_h->Init.Codec_Output_Routing_sel = OUTPUT_ROUTING_DACFFLR_DACFF01_GPF1_DRC_ASRC1;
                    }
                    else {
                        goto __err;
                    }
                }
                else {
                    CODEC_h->Init.Codec_Output_sel = OUTPUT_SELECT_BYPASS;
                }

                codec_init(CODEC_h);
                __CODEC_ADCFF_L_FULL_THRESHOLD(CODEC_DATA_SIZE);
                __CODEC_ADCFF_R_FULL_THRESHOLD(CODEC_DATA_SIZE);
                if (dir & AUDIO_HW_DIR_IN) {
                    codec_int_enable(CODEC_INT_ADCFF_L_FULL | CODEC_INT_ADCFF_L_AFULL);
                }
                if (dir & AUDIO_HW_DIR_OUT) {
                    codec_int_enable(CODEC_INT_DACFF_L_EMPTY | CODEC_INT_DACFF_L_AEMPTY);
                }
                codec_Set_ADC_Volume(0x1000);
                codec_Set_DAC_Volume(0x1000);
                codec_mute_output(1);
                
                /* insert new audio hw to list before enable interrupt */
                co_list_push_back(&audio_hw_env.hw_list, &tmp->hdr);

                NVIC_EnableIRQ(CODEC_IRQn);
            }
            break;
        case AUDIO_HW_TYPE_PDM:
            {
                if (dir != AUDIO_HW_DIR_IN) {
                    goto __err;
                }

                PDM_HandleTypeDef *pdm_handle = pvPortMalloc(sizeof(PDM_HandleTypeDef));
                if (pdm_handle == NULL) {
                    goto __err;
                }
                tmp->hw_handle = pdm_handle;
                tmp->pcm_samples = AUDIO_HW_PDM_RX_INT_LEVEL * AUDIO_HW_STORE_FRAME_COUNT;
                tmp->pcm = (void *)pvPortMalloc(sizeof(int16_t) * channels * tmp->pcm_samples);
                if (tmp->pcm == NULL) {
                    goto __err;
                }
                
                switch (base_addr) {
                    case PDM0_BASE:
                        __SYSTEM_PDM0_CLK_ENABLE();
                        pdm_handle->PDMx = PDM0;
                        pdm_hw_table[0].hw_handle = pdm_handle;
                        pdm_hw_table[0].audio_hw = tmp;
                        break;
                    case PDM1_BASE:
                        __SYSTEM_PDM1_CLK_ENABLE();
                        pdm_handle->PDMx = PDM1;
                        pdm_hw_table[1].hw_handle = pdm_handle;
                        pdm_hw_table[1].audio_hw = tmp;
                        break;
                    case PDM2_BASE:
                        __SYSTEM_PDM2_CLK_ENABLE();
                        pdm_handle->PDMx = PDM2;
                        pdm_hw_table[2].hw_handle = pdm_handle;
                        pdm_hw_table[2].audio_hw = tmp;
                        break;
                    default:
                        goto __err;
                }

                if (sample_rate == 48000) {
                    pdm_handle->Init.SampleRate = PDM_SAMPLE_RATE_48000;
                }
                else if (sample_rate == 44100) {
                    pdm_handle->Init.SampleRate = PDM_SAMPLE_RATE_44100;
                }
                else if (sample_rate == 16000) {
                    pdm_handle->Init.SampleRate = PDM_SAMPLE_RATE_16000;
                }
                else {
                    goto __err;
                }
                pdm_handle->Init.OverSampleMode = PDM_OSM_2;
                if (channels == AUDIO_CHANNELS_MONO) {
                    pdm_handle->Init.ChannelMode = PDM_MONO_LEFT;
                }
                else if (channels == AUDIO_CHANNELS_STEREO) {
                    pdm_handle->Init.ChannelMode = PDM_STEREO;
                }
                else {
                    goto __err;
                }
                pdm_handle->Init.Volume = 24;
                pdm_handle->Init.FIFO_FullThreshold = AUDIO_HW_PDM_RX_INT_LEVEL;
                
                pdm_handle->p_RxData = NULL;
                pdm_handle->RxCallback = pdm_rx_callback;
                
                /* insert new audio hw to list before enable interrupt */
                co_list_push_back(&audio_hw_env.hw_list, &tmp->hdr);

                pdm_init(pdm_handle);
                pdm_start_IT(pdm_handle, NULL);
                switch (base_addr) {
                    case PDM0_BASE:
                        NVIC_EnableIRQ(PDM0_IRQn);
                        break;
                    case PDM1_BASE:
                        NVIC_EnableIRQ(PDM1_IRQn);
                        break;
                    case PDM2_BASE:
                        NVIC_EnableIRQ(PDM2_IRQn);
                        break;
                    default:
                        goto __err;
                }
            }
            break;
        case AUDIO_HW_TYPE_SPDIF:
            {
                SPDIF_HandleTypeDef *spdif_handle = pvPortMalloc(sizeof(SPDIF_HandleTypeDef));
                if (spdif_handle == NULL) {
                    goto __err;
                }
                tmp->hw_handle = spdif_handle;
                if (dir == AUDIO_HW_DIR_IN) {
                    tmp->pcm_samples = AUDIO_HW_SPDIF_RX_INT_LEVEL * AUDIO_HW_STORE_FRAME_COUNT;
                    tmp->pcm = (void *)pvPortMalloc(sizeof(int16_t) * channels * tmp->pcm_samples);
                    if (tmp->pcm == NULL) {
                        goto __err;
                    }
                }
                else {
                    tmp->pcm_out = (void *)pvPortMalloc(sizeof(int16_t) * channels * AUDIO_HW_SPDIF_TX_INT_LEVEL);
                    if (tmp->pcm_out == NULL) {
                        goto __err;
                    }
                }
                
                spdif_hw_table[0].hw_handle = spdif_handle;
                spdif_hw_table[0].audio_hw = tmp;

                /* init SPDIF */
                __SYSTEM_SPDIF_RESET();
                if (sample_rate == 44100) {
                    config_aupll(169344000);
                    __SYSTEM_SPDIF_CLK_SELECT_AUPLL();
                    __SYSTEM_SPDIF_CLK_DIV(3);
                }
                else if (sample_rate == 48000) {
                    config_aupll(184320000);
                    __SYSTEM_SPDIF_CLK_SELECT_AUPLL();
                    __SYSTEM_SPDIF_CLK_DIV(3);
                }
                else if (sample_rate == 16000) {
                    config_aupll(169344000);
                    __SYSTEM_SPDIF_CLK_SELECT_AUPLL();
                    __SYSTEM_SPDIF_CLK_DIV(9);
                }
                else {
                    goto __err;
                }
                __SYSTEM_SPDIF_CLK_ENABLE();
                
                if (channels == AUDIO_CHANNELS_MONO) {
                    spdif_handle->Init.MonoEn = SPDIF_FUNC_ENABLE;
                }
                else {
                    spdif_handle->Init.MonoEn = SPDIF_FUNC_DISABLE;
                }
                spdif_handle->Init.DataWidth = SPDIF_DATA_WIDTH_16BIT;
                if (sample_rate == 48000) {
                    spdif_handle->Init.TxSampleRate = SPDIF_SAMPLE_RATE_48000;
                }
                else if (sample_rate == 44100) {
                    spdif_handle->Init.TxSampleRate = SPDIF_SAMPLE_RATE_44100;
                }
                else if (sample_rate == 16000) {
                    spdif_handle->Init.TxSampleRate = SPDIF_SAMPLE_RATE_16000;
                }
                else {
                    goto __err;
                }
                if (dir == AUDIO_HW_DIR_OUT) {
                    spdif_handle->Init.WorkMode = SPDIF_TX_MODE;
                    spdif_handle->Init.Tx_ALEmpty_Threshold = AUDIO_HW_SPDIF_TX_INT_LEVEL * channels;
                }
                else if (dir == AUDIO_HW_DIR_IN) {
                    spdif_handle->Init.WorkMode = SPDIF_RX_MODE;
                    spdif_handle->Init.Rx_ALFull_Threshold = AUDIO_HW_SPDIF_RX_INT_LEVEL * channels;
                }
                else {
                    goto __err;
                }
                spdif_handle->RxALFullCallback = spdif_rx_callback;
                spdif_handle->TxALEmptyCallback = spdif_tx_callback;
                spdif_init(spdif_handle);
                
                /* insert new audio hw to list before enable interrupt */
                co_list_push_back(&audio_hw_env.hw_list, &tmp->hdr);

                if (dir == AUDIO_HW_DIR_OUT) {
                    /* enable almost empty interrupt */
                    __SPDIF_INT_ENABLE(SPDIF_INT_ALEMPTY);
                }
                else if (dir == AUDIO_HW_DIR_IN) {
                    /* enable almost full interrupt */
                    __SPDIF_INT_ENABLE(SPDIF_INT_ALFULL);
                }
                NVIC_EnableIRQ(SPDIF_IRQn);
            }
            break;
        default:
            goto __err;
    }
    
    return tmp;
    
__err:
    if (tmp->hw_handle) {
        vPortFree(tmp->hw_handle);
    }
    if (tmp->pcm) {
        vPortFree(tmp->pcm);
    }
    if (tmp->pcm_out) {
        vPortFree(tmp->pcm_out);
    }
    co_list_extract(&audio_hw_env.hw_list, &tmp->hdr);
    vPortFree(tmp);

    return NULL;
}

void audio_hw_destroy(audio_hw_t *hw)
{
    if (hw == NULL) {
        return;
    }
    
    GLOBAL_INT_DISABLE();
    if (co_list_extract(&audio_hw_env.hw_list, &hw->hdr)) {
        switch (hw->type) {
            case AUDIO_HW_TYPE_I2S:
                i2s_deinit(hw->hw_handle);
                switch (hw->base_addr) {
                    case I2S0_BASE:
                        __SYSTEM_I2S0_CLK_DISABLE();
                        NVIC_DisableIRQ(I2S0_IRQn);
                        break;
                    case I2S1_BASE:
                        __SYSTEM_I2S1_CLK_DISABLE();
                        NVIC_DisableIRQ(I2S1_IRQn);
                        break;
                    case I2S2_BASE:
                        __SYSTEM_I2S2_CLK_DISABLE();
                        NVIC_DisableIRQ(I2S2_IRQn);
                        break;
                    default:
                        break;
                }
                break;
            case AUDIO_HW_TYPE_CODEC:
                codec_deinit(hw->hw_handle);
                __SYSTEM_CODEC_CLK_DISABLE();
                NVIC_DisableIRQ(CODEC_IRQn);
                break;
            case AUDIO_HW_TYPE_PDM:
                pdm_stop(hw->hw_handle);
                switch (hw->base_addr) {
                    case PDM0_BASE:
                        __SYSTEM_PDM0_CLK_DISABLE();
                        NVIC_DisableIRQ(PDM0_IRQn);
                        break;
                    case PDM1_BASE:
                        __SYSTEM_PDM1_CLK_DISABLE();
                        NVIC_DisableIRQ(PDM1_IRQn);
                        break;
                    case PDM2_BASE:
                        __SYSTEM_PDM2_CLK_DISABLE();
                        NVIC_DisableIRQ(PDM2_IRQn);
                        break;
                    default:
                        break;
                }
                break;
            case AUDIO_HW_TYPE_SPDIF:
                /* dsiable interrupt */
                __SPDIF_INT_DISABLE(SPDIF_INT_ALFULL | SPDIF_INT_ALEMPTY);
                __SYSTEM_SPDIF_CLK_DISABLE();
                NVIC_DisableIRQ(SPDIF_IRQn);
                break;
            default:
                break;
        }
    }
    GLOBAL_INT_RESTORE();
    
    audio_hw_output_t *output;
    output = (void *)co_list_pop_front(&hw->output_list);
    while (output) {
        vPortFree(output);
        output = (void *)co_list_pop_front(&hw->output_list);
    }
    
    if (hw->hw_handle) {
        vPortFree(hw->hw_handle);
    }
    if (hw->pcm) {
        vPortFree(hw->pcm);
    }
    if (hw->pcm_out) {
        vPortFree(hw->pcm_out);
    }
    vPortFree(hw);
}

audio_hw_output_t *audio_hw_output_add(audio_hw_t *hw, audio_hw_receive_pcm_ntf_t handler)
{
    audio_hw_output_t *output;

    if (hw == NULL) {
        return NULL;
    }
    
    output = pvPortMalloc(sizeof(audio_hw_output_t));
    if (output) {
        GLOBAL_INT_DISABLE();
        output->audio_hw = hw;
        output->handler = handler;
        output->rd_ptr = hw->wr_ptr;
        co_list_push_back(&hw->output_list, &output->hdr);
        GLOBAL_INT_RESTORE();
    }
    
    return output;
}

void audio_hw_output_remove(audio_hw_t *hw, audio_hw_output_t *output)
{
    if (hw == NULL) {
        return;
    }

    GLOBAL_INT_DISABLE();
    co_list_extract(&hw->output_list, &output->hdr);
    GLOBAL_INT_RESTORE();
    
    vPortFree(output);
}

static void *copy_pcm(audio_hw_output_t *output, void *pcm, uint32_t samples, uint8_t channels)
{
    audio_hw_t *hw = output->audio_hw;

    if (channels == AUDIO_CHANNELS_MONO) {
        if (hw->channels == AUDIO_CHANNELS_MONO) {
            int16_t *src = (int16_t *)hw->pcm + output->rd_ptr;
            int16_t *dst = pcm;
            for (uint32_t i=0; i<samples; i++) {
                *dst++ = *src++;
            }
            pcm = dst;
        }
        else {
            int16_t *src = (int16_t *)hw->pcm + (output->rd_ptr << 1);
            int16_t *dst = pcm;
            for (uint32_t i=0; i<samples; i++) {
                *dst++ = *src;
                src += 2;
            }
            pcm = dst;
        }
    }
    else {
        if (hw->channels == AUDIO_CHANNELS_MONO) {
            int16_t *src = (int16_t *)hw->pcm + output->rd_ptr;
            int16_t *dst = pcm;
            for (uint32_t i=0; i<samples; i++) {
                *dst++ = *src;
                *dst++ = *src++;
            }
            pcm = dst;
        }
        else {
            uint32_t *src = (uint32_t *)hw->pcm + output->rd_ptr;
            uint32_t *dst = pcm;
            for (uint32_t i=0; i<samples; i++) {
                *dst++ = *src++;
            }
            pcm = dst;
        }
    }
    
    return pcm;
}

uint32_t audio_hw_read_pcm(audio_hw_output_t *output, void *pcm, uint32_t samples, uint8_t channels)
{
    audio_hw_t *hw = output->audio_hw;
    uint32_t tail_samples;
    uint32_t read_samples = 0;

    tail_samples = hw->pcm_samples - output->rd_ptr;
    if (tail_samples <= samples) {
        pcm = copy_pcm(output, pcm, tail_samples, channels);
        output->rd_ptr = 0;
        samples -= tail_samples;
        read_samples = tail_samples;
    }
    if (samples) {
        pcm = copy_pcm(output, pcm, samples, channels);
        output->rd_ptr += samples;
        read_samples += samples;
    }

    return read_samples;
}

