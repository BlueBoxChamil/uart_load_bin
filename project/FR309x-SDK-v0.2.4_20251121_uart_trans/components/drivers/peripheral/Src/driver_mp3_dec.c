/*
  ******************************************************************************
  * @file    driver_mp3_dec.c
  * @author  FreqChip Firmware Team
  * @version V1.0.0
  * @date    2021
  * @brief   MP3 decoder module driver.
  *          This file provides firmware functions to manage the 
  *          MP3 DECODEER peripheral
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 FreqChip.
  * All rights reserved.
  ******************************************************************************
*/
#include "fr30xx.h"

/******************************************************************************
 * @fn      mp3_decoder_init
 *
 * @brief   Initialize the mp3 decoder according to the specified parameters
 *          in the MP3_DEC_HandleTypeDef 
 *
 * @param   hmp3dec : MP3_DEC_HandleTypeDef structure that contains the
 *                      configuration information for MP3DEC module.
 *          pcm_buff : mp3 decoder paddr
 *          pcm_buf_size : pcm buffer size
 */
void mp3_dec_init(MP3_DEC_HandleTypeDef *hmp3dec, uint32_t *pcm_buff, uint32_t pcm_buf_size)
{
    __MP3D_SOFTRST();//softreset
    __MP3D_PCM_FIFO_RESET();
    __MP3D_ISSUE_NEW_FILE();
    MP3D->MP3D_CTRL.Bits.DAI_EN = hmp3dec->mp3dec_ctrl_Init.Bits.DAI_EN;
    __MP3D_OPERA_ENABLE();
    MP3D->MP3D_CTRL.Bits.ISR_EN = hmp3dec->mp3dec_ctrl_Init.Bits.ISR_EN;
    MP3D->MP3D_PADDR = (uint32_t)pcm_buff;
    MP3D->MP3D_PSIZE.Word = pcm_buf_size;
    memset(pcm_buff,0,pcm_buf_size);
}

/******************************************************************************
 * @fn      mp3_decoder_start
 *
 * @brief   Start the mp3 decoder 
 *
 * @param   src_buff : src data pointer
 *          src_size : src data size
 */
static void mp3_dec_start(uint8_t *src_buff, uint32_t src_size)
{
    MP3D->MP3D_SADDR = (uint32_t)src_buff;
    MP3D->MP3D_SSIZE.Word = src_size;
    MP3D->MP3D_INTSTA.Bits.SOURCE_CMPLETION_ISR = 1;
    MP3D->MP3D_CTRL.Bits.OPERA_START = 1;
}

/******************************************************************************
 * @fn      mp3_decoder_stop
 *
 * @brief   Stop the mp3 decoder 
 *
 * @param   void
 */
static void mp3d_stop(void)
{
    __MP3D_PCM_FIFO_RESET();
    __MP3D_ISSUE_NEW_FILE();
}

/******************************************************************************
 * @fn      mp3_clear_isr
 *
 * @brief   Clear the mp3 decoder isr flag
 *
 * @param   isr : enum_mp3d_isr_t contains info
 */
static void mp3d_clear_isr(enum_mp3d_isr_t isr)
{
    MP3D->MP3D_INTSTA.Word = (1 << isr);
}

/******************************************************************************
 * @fn      mp3_dec_playdata
 *
 * @brief   Mp3 decoder play data
 *
 * @param   hmp3dec : mp3 decoder handler
 *          fp_Data : the data which is needed to decoder
 *          fu32_Size : the decoded data's size
 */
void mp3_dec_playdata(MP3_DEC_HandleTypeDef *hmp3dec, uint8_t *fp_Data, uint32_t fu32_Size)
{
    uint32_t length = 0;
    uint32_t CurrentIndex = 0;
    uint32_t DataSize = fu32_Size;
    while(DataSize != 0)
    {
        length = fu32_Size > 512 ? 512 : fu32_Size;
        mp3_dec_start((uint8_t *)&fp_Data[CurrentIndex], length);
        DataSize -= length;
        while(1)
        {
            if(__MP3D_CHECK_NEED_SEC())
            {
                mp3d_clear_isr(SOURCE_COMPLETE_ISR);
                CurrentIndex += length;
                break;
            }
                
        }
    }
    mp3d_stop();
}

/******************************************************************************
 * @fn      mp3_dec_playdata_IT
 *
 * @brief   Mp3 decoder play data with isr
 *
 * @param   hmp3dec : mp3 decoder handler
 *          fp_Data : the data which is needed to decoder
 *          fu32_Size : the decoded data's size
 */
void mp3_dec_playdata_IT(MP3_DEC_HandleTypeDef *hmp3dec, uint8_t *fp_Data, uint32_t fu32_Size)
{
    __MP3D_COMPLETE_ISR_ENABLE();
    
    hmp3dec->MP3Enced_Data     = fp_Data;
    hmp3dec->MP3Enced_Index    = 0;
    hmp3dec->LeftLen           = fu32_Size;
    hmp3dec->FrameLen = hmp3dec->LeftLen > 512 ? 512 : hmp3dec->LeftLen;
    mp3_dec_start((uint8_t *)&hmp3dec->MP3Enced_Data[hmp3dec->MP3Enced_Index], hmp3dec->FrameLen);
    hmp3dec->MP3Enced_Index += hmp3dec->FrameLen;
    hmp3dec->LeftLen -= hmp3dec->FrameLen;
}

/******************************************************************************
 * @fn      mp3d_get_stream_info
 *
 * @brief   Mp3 decoder get the decoded frame info
 *
 * @param   info : struct_MP3D_stream_info_t structure contains
 */
void mp3d_get_stream_info(struct_MP3D_stream_info_t *info)
{
    info->audio_ver = MP3D->MP3D_INFO.Bits.MP3_STREAM_AUDIO_VER;
    info->audio_emphasis = MP3D->MP3D_INFO.Bits.MP3_STREAM_AUDIO_EMPHASIS;
    info->audio_mode = MP3D->MP3D_INFO.Bits.MP3_STREAM_AUDIO_MODE;
    info->bit_rate = MP3D->MP3D_INFO.Bits.MP3_STREAM_BIT_RATE;
    info->copyright = MP3D->MP3D_INFO.Bits.MP3_STREAM_COPYRIGHT;
    info->error_protection = MP3D->MP3D_INFO.Bits.MP3_STREAM_ERROR_PROTECT;
    info->frame_locked = MP3D->MP3D_INFO.Bits.MP3_FRAME_LOCKED;
    info->main_done = MP3D->MP3D_INFO.Bits.MP3_MAIN_DONE;
    info->main_exhaust = MP3D->MP3D_INFO.Bits.MP3_MAIN_EXHAUST;
    info->mode_extension = MP3D->MP3D_INFO.Bits.MP3_STREAM_AUDIO_MODE_EXTEN;
    info->original = MP3D->MP3D_INFO.Bits.MP3_STREAM_ORIGINAL;
    info->sample_rate = MP3D->MP3D_INFO.Bits.MP3_STREAM_SAMPLE_RATE;
}

/******************************************************************************
 * @fn      mp3d_check_dec_failed
 *
 * @brief   check whether the mp3dec is failed or successful
 *
 * @param   void
 */
int mp3d_check_dec_failed(void)
{
    if((MP3D->MP3D_INTSTA.Bits.MP3_FRAME_LOCKED_ISR == 1) ||
        (MP3D->MP3D_INFO.Bits.MP3_FRAME_LOCKED == 0))
    {
        MP3D->MP3D_INTSTA.Word = (1 << MP3_FRAME_LOCKED_ISR);
        return 1;
    }
    return 0;
}

/******************************************************************************
 * @fn      mp3dec_IRQHandler
 *
 * @brief   mp3 decoder handle function in mo3 dec isr
 *
 * @param   hmp3dec : MP3 decoder handle
 */
void mp3dec_IRQHandler(MP3_DEC_HandleTypeDef *hmp3dec)
{
    if(__MP3D_SRC_COMPLETE_ISR_IS_SET())
    {
        if(hmp3dec->LeftLen != 0)
        {
            hmp3dec->FrameLen = hmp3dec->LeftLen > 512 ? 512 : hmp3dec->LeftLen;
            mp3_dec_start((uint8_t *)&hmp3dec->MP3Enced_Data[hmp3dec->MP3Enced_Index], hmp3dec->FrameLen);
            hmp3dec->MP3Enced_Index += hmp3dec->FrameLen;
            hmp3dec->LeftLen -= hmp3dec->FrameLen;
        }
        else
        {
            __MP3D_COMPLETE_ISR_DISABLE();
            mp3d_stop();
        }
    }
}