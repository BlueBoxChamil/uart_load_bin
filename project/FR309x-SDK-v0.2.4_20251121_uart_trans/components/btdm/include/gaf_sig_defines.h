/*
 ****************************************************************************************
 *
 * @file       gaf_sig_defines.h
 * @version    v1.0
 * @brief      
 * @attention
 *  Copyright (C) 2024 Freqchip.   \n
 *  All rights reserved.           \n
 * 
 * 
 ****************************************************************************************
 */

#ifndef _GAF_SIG_DEFINES_H
#define _GAF_SIG_DEFINES_H

#include <stdint.h>

/// Response Code values for ASE Control Point characteristic
enum bap_uc_cp_rsp_code
{
    /// Success
    BAP_UC_CP_RSP_CODE_SUCCESS = 0,
    /// Unsupported Opcode
    BAP_UC_CP_RSP_CODE_UNSUPPORTED_OPCODE,
    /// Invalid Length
    BAP_UC_CP_RSP_CODE_INVALID_LENGTH,
    /// Invalid ASE ID
    BAP_UC_CP_RSP_CODE_INVALID_ASE_ID,
    /// Invalid ASE State Machine Transition
    BAP_UC_CP_RSP_CODE_INVALID_TRANSITION,
    /// Invalid ASE direction
    BAP_UC_CP_RSP_CODE_INVAILD_ASE_DIRECTION,
    /// Unsupported Audio Capabilities
    BAP_UC_CP_RSP_CODE_UNSUPPORTED_AUDIO_CAPA,
    /// Unsupported Configuration Parameter value
    BAP_UC_CP_RSP_CODE_UNSUPPORTED_CFG_PARAM,
    /// Rejected Configuration Parameter value
    BAP_UC_CP_RSP_CODE_REJECTED_CFG_PARAM,
    /// Invalid Configuration Parameter value
    BAP_UC_CP_RSP_CODE_INVALID_CFG_PARAM,
    /// Unsupported Metadata
    BAP_UC_CP_RSP_CODE_UNSUPPORTED_METADATA,
    /// Rejected Metadata
    BAP_UC_CP_RSP_CODE_REJECTED_METADATA,
    /// Invalid Metadata
    BAP_UC_CP_RSP_CODE_INVALID_METADATA,
    /// Insufficient Resources
    BAP_UC_CP_RSP_CODE_INSUFFICIENT_RESOURCES,
    /// Unspecified Error
    BAP_UC_CP_RSP_CODE_UNSPECIFIED_ERROR,

    BAP_UC_CP_RSP_CODE_MAX,
};

/// Reason values for ASE Control Point characteristic
/// Valid when rsp_code is within the range of [BAP_UC_CP_RSP_CODE_UNSUPPORTED_CFG_PARAM, BAP_UC_CP_RSP_CODE_INVALID_CFG_PARAM]
enum bap_uc_cp_reason
{
    BAP_UC_CP_REASON_NO_ERROR,
    /// Codec ID
    BAP_UC_CP_REASON_CODEC_ID,
    /// Codec Specific Configuration
    BAP_UC_CP_REASON_CODEC_CFG,
    /// SDU Interval
    BAP_UC_CP_REASON_SDU_INTERVAL,
    /// Framing
    BAP_UC_CP_REASON_FRAMING,
    /// PHY
    BAP_UC_CP_REASON_PHY,
    /// Maximum SDU Size
    BAP_UC_CP_REASON_MAX_SDU_SIZE,
    /// Retransmission Number
    BAP_UC_CP_REASON_RETX_NB,
    /// Transport Latency
    BAP_UC_CP_REASON_TRANS_LATENCY,
    /// Presentation Delay
    BAP_UC_CP_REASON_PRES_DELAY,
    /// Invalid ASE CIS Mapping
    BAP_UC_CP_REASON_INVAILD_ASE_CIS_MAPPING,

    BAP_UC_CP_REASON_MAX,
};

#define BIT(x)      (1<<x)

/** @defgroup Codec Coding_Format Definitions
 * @{
 */
#define U_LAW_LOG                   0x00
#define A_LAW_LOG                   0x01
#define CVSD                        0x02
#define TRANSPARENT                 0x03
#define LINEAR_PCM                  0x04
#define MSBC                        0x05
#define LC3                         0x06
#define G_729A                      0x07
#define VENDOR_SPECIFIC             0xff
/** @} End Codec Coding_Format DEFINES */

/** @defgroup Audio Location Definitions
 * @{
 */
#define AUDIO_LOCA_MONO_AUDIO_BIT                  0x00
#define AUDIO_LOCA_FRONT_LEFT_BIT                  BIT(0)
#define AUDIO_LOCA_FRONT_RIGHT_BIT                 BIT(1)
#define AUDIO_LOCA_FRONT_CENTER_BIT                BIT(2)
#define AUDIO_LOCA_LOW_FREQ_EFFECTS_1_BIT          BIT(3)
#define AUDIO_LOCA_BACK_LEFT_BIT                   BIT(4)
#define AUDIO_LOCA_BACK_RIGHT_BIT                  BIT(5)
#define AUDIO_LOCA_FRONT_LEFT_OF_CENTER_BIT        BIT(6)
#define AUDIO_LOCA_FRONT_RIGHT_OF_CENTER_BIT       BIT(7)
#define AUDIO_LOCA_BACK_CENTER_BIT                 BIT(8)
#define AUDIO_LOCA_LOW_FREQ_EFFECTS_2_BIT          BIT(9)
#define AUDIO_LOCA_SIDE_LEFT_BIT                   BIT(10)
#define AUDIO_LOCA_SIDE_RIGHER_BIT                 BIT(11)
#define AUDIO_LOCA_TOP_FRONT_LEFT_BIT              BIT(12)
#define AUDIO_LOCA_TOP_FRONT_RIGHT_BIT             BIT(13)
#define AUDIO_LOCA_TOP_FRONT_CENTER_BIT            BIT(14)
#define AUDIO_LOCA_TOP_CENTER_BIT                  BIT(15)
#define AUDIO_LOCA_TOP_BACK_LEFT_BIT               BIT(16)
#define AUDIO_LOCA_TOP_BACK_RIGHT_BIT              BIT(17)
#define AUDIO_LOCA_TOP_SIDE_LEFT_BIT               BIT(18)
#define AUDIO_LOCA_TOP_SIDE_RIGHT_BIT              BIT(19)
#define AUDIO_LOCA_TOP_BACK_CENTER_BIT             BIT(20)
#define AUDIO_LOCA_BOTTOM_FRONT_CENTER_BIT         BIT(21)
#define AUDIO_LOCA_BOTTOM_FRONT_LEFT_BIT           BIT(22)
#define AUDIO_LOCA_BOTTOM_FRONT_RIGHT_BIT          BIT(23)
#define AUDIO_LOCA_FRONT_LEFT_WIDE_BIT             BIT(24)
#define AUDIO_LOCA_FRONT_RIGHT_WIDE_BIT            BIT(25)
#define AUDIO_LOCA_LEFT_SURROUND_BIT               BIT(26)
#define AUDIO_LOCA_RIGHT_SURROUND_BIT              BIT(27)
/** @} End Audio Location Definitions DEFINES */

/** @defgroup Context Type Definitions
 * @{
 */
#define CONTEXT_PROHIBITED                      0x00 
#define CONTEXT_UNSPECIFIED_BIT                 BIT(0)
#define CONTEXT_CONVERSATIONAL_BIT              BIT(1)
#define CONTEXT_MEDIA_BIT                       BIT(2)
#define CONTEXT_GAME_BIT                        BIT(3)
#define CONTEXT_INSTRUCTIONAL_BIT               BIT(4)
#define CONTEXT_VOICE_ASSISTANTS_BIT            BIT(5)
#define CONTEXT_LIVE_BIT                        BIT(6)
#define CONTEXT_SOUND_EFFECTS_BIT               BIT(7)
#define CONTEXT_NOTIFICATIONS_BIT               BIT(8)
#define CONTEXT_RINGTONE_BIT                    BIT(9)
#define CONTEXT_ALERTS_BIT                      BIT(10)
#define CONTEXT_EMERGENCY_ALARM_BIT             BIT(11)
/** @} End Context Tyoe DEFINES */

// -----------------------------------------------------------------------------
// ----------------- Codec_Specific_Capabilities LTV Defines -------------------
// -----------------------------------------------------------------------------

/** @defgroup Supported_Sampling_Frequencies Definitions.
 * @{
 */
//  | Length |  Type  |        Value        |
//  -----------------------------------------
//  |1 Octets|1 Octets|       2 Octets      |
//  -----------------------------------------
//  |  0x03  |  0x01  |                     | 
//  -----------------------------------------
#define SUPPROT_SAMPLING_FREQUENCEIS_LENGTH     0x03
#define SUPPROT_SAMPLING_FREQUENCEIS            0x01

#define SUPPROT_FREQ_8000_Hz_BIT                BIT(0)  
#define SUPPROT_FREQ_11025_Hz_BIT               BIT(1)  
#define SUPPROT_FREQ_16000_Hz_BIT               BIT(2)  
#define SUPPROT_FREQ_22050_Hz_BIT               BIT(3)
#define SUPPROT_FREQ_24000_Hz_BIT               BIT(4)
#define SUPPROT_FREQ_32000_Hz_BIT               BIT(5)
#define SUPPROT_FREQ_44100_Hz_BIT               BIT(6)
#define SUPPROT_FREQ_48000_Hz_BIT               BIT(7)
#define SUPPROT_FREQ_88200_Hz_BIT               BIT(8)
#define SUPPROT_FREQ_96000_Hz_BIT               BIT(9)
#define SUPPROT_FREQ_176400_Hz_BIT              BIT(10)
#define SUPPROT_FREQ_192000_Hz_BIT              BIT(11)
#define SUPPROT_FREQ_384000_Hz_BIT              BIT(12)
/** @} End Supported_Sampling_Frequencies DEFINES */

/** @defgroup Supported_Frame_Durations Definitions.
 * @{
 */
//  | Length |  Type  |        Value        |
//  -----------------------------------------
//  |1 Octets|1 Octets|       1 Octets      |
//  -----------------------------------------
//  |  0x02  |  0x02  |                     | 
//  -----------------------------------------
#define SUPPORT_FRAME_DURATIONS_LENGTH      0x02
#define SUPPORT_FRAME_DURATIONS             0x02

#define SUPPROT_FRAME_DURATION_7_5_MS_BIT       BIT(0)  
#define SUPPROT_FRAME_DURATION_10_MS_BIT        BIT(1)  
#define PREFERRED_7_5_MS_BIT                    BIT(4)  
#define PREFERRED_10_MS_BIT                     BIT(5)
/** @} End Supported_Frame_Durations DEFINES */

/** @defgroup Supported_Audio_Channel_Counts Definitions.
 * @{
 */
//  | Length |  Type  |        Value        |
//  -----------------------------------------
//  |1 Octets|1 Octets|       1 Octets      |
//  -----------------------------------------
//  |  0x02  |  0x03  |                     | 
//  -----------------------------------------
#define SUPPORT_AUDIO_CHANNEL_COUNTS_LENGTH     0x02
#define SUPPORT_AUDIO_CHANNEL_COUNTS            0x03

#define CHANNEL_COUNT_1_BIT             BIT(0)
#define CHANNEL_COUNT_2_BIT             BIT(1)
#define CHANNEL_COUNT_3_BIT             BIT(2)
#define CHANNEL_COUNT_4_BIT             BIT(3)
#define CHANNEL_COUNT_5_BIT             BIT(4)
#define CHANNEL_COUNT_6_BIT             BIT(5)
#define CHANNEL_COUNT_7_BIT             BIT(6)
#define CHANNEL_COUNT_8_BIT             BIT(7)
/** @} End Supported_Audio_Channel_Counts DEFINES */

/** @defgroup Supported_Octets_Per_Codec_Frame Definitions.
 * @{
 */
//  | Length |  Type  | Min num value | Max num value |
//  --------------------------------------------------
//  |1 Octets|1 Octets|    2 Octets   |    2 Octets   |
//  ---------------------------------------------------
//  |  0x05  |  0x04  |               |               | 
//  ---------------------------------------------------
#define SUPPORT_OCTETS_PER_CODEC_FRAME_LENGTH   0x05
#define SUPPORT_OCTETS_PER_CODEC_FRAME          0x04
/** @} End Supported_Octets_Per_Codec_Frame DEFINES */

/** @defgroup Supported_Max_Codec_Frames_Per_SDU Definitions.
 * @{
 */
//  | Length |  Type  |        Value        |
//  -----------------------------------------
//  |1 Octets|1 Octets|       4 Octets      |
//  -----------------------------------------
//  |  0x02  |  0x05  |                     | 
//  -----------------------------------------
#define SUPPORT_MAX_CODEC_FRAME_PER_SDU_LENGTH  0x02
#define SUPPORT_MAX_CODEC_FRAME_PER_SDU         0x05
/** @} End Supported_Max_Codec_Frames_Per_SDU DEFINES */

// ------------------------------------------------------------------------------
// ----------------- Codec_Specific_Configuration LTV Defines -------------------
// ------------------------------------------------------------------------------

/** @defgroup Sampling_Frequencies Definitions.
 * @{
 */
//  | Length |  Type  |        Value        |
//  -----------------------------------------
//  |1 Octets|1 Octets|       1 Octets      |
//  -----------------------------------------
//  |  0x02  |  0x01  |                     | 
//  -----------------------------------------
#define SAMPLING_FREQUENCEIS_LENGTH     0x02
#define SAMPLING_FREQUENCEIS            0x01

#define FREQ_8000_Hz                0x01
#define FREQ_11025_Hz               0x02
#define FREQ_16000_Hz               0x03  
#define FREQ_22050_Hz               0x04
#define FREQ_24000_Hz               0x05
#define FREQ_32000_Hz               0x06
#define FREQ_44100_Hz               0x07
#define FREQ_48000_Hz               0x08
#define FREQ_88200_Hz               0x09
#define FREQ_96000_Hz               0x0A
#define FREQ_176400_Hz              0x0B
#define FREQ_192000_Hz              0x0C
#define FREQ_384000_Hz              0x0D
/** @} End Sampling_Frequencies DEFINES */

/** @defgroup Frame_Durations Definitions.
 * @{
 */
//  | Length |  Type  |        Value        |
//  -----------------------------------------
//  |1 Octets|1 Octets|       1 Octets      |
//  -----------------------------------------
//  |  0x02  |  0x02  |                     | 
//  -----------------------------------------
#define FRAME_DURATIONS_LENGTH      0x02
#define FRAME_DURATIONS             0x02

#define FRAME_DURATION_7_5_ms       0x00
#define FRAME_DURATION_10_ms        0x01  
/** @} End Frame_Durations DEFINES */

/** @defgroup Audio_Channel_Allocation Definitions.
 * @{
 */
//  | Length |  Type  |    Value      |
//  -----------------------------------
//  |1 Octets|1 Octets|    4 Octets   |
//  -----------------------------------
//  |  0x05  |  0x03  |               |
//  -----------------------------------
//  Value: 4-octet bitfield of Audio Location values
#define AUDIO_CHANNEL_ALLOCATION_LENGTH   0x05
#define AUDIO_CHANNEL_ALLOCATION          0x03
/** @} End Audio_Channel_Allocation DEFINES */

/** @defgroup Octets_Per_Codec_Frame Definitions.
 * @{
 */
//  | Length |  Type  |    Value      |
//  -----------------------------------
//  |1 Octets|1 Octets|    2 Octets   |
//  -----------------------------------
//  |  0x03  |  0x04  |               |
//  -----------------------------------
//  Value: Number of octets used per codec frame
#define OCTETS_PER_CODEC_FRAME_LENGTH   0x03
#define OCTETS_PER_CODEC_FRAME          0x04
/** @} End Octets_Per_Codec_Frame DEFINES */

/** @defgroup Codec_Frame_Blocks_Per_SDU Definitions.
 * @{
 */
//  | Length |  Type  |    Value      |
//  -----------------------------------
//  |1 Octets|1 Octets|    1 Octets   |
//  -----------------------------------
//  |  0x02  |  0x05  |               |
//  -----------------------------------
//  Value: Number of blocks of codec frames per SDU
#define CODEC_FRAME_BLOCKS_PER_SDU_LENGTH   0x02
#define CODEC_FRAME_BLOCKS_PER_SDU          0x05
/** @} End Codec_Frame_Blocks_Per_SDU DEFINES */

#endif//_GAF_SIG_DEFINES_H