#ifndef _AUDIO_LE_SINK_H
#define _AUDIO_LE_SINK_H

#include "audio_common.h"
#include "audio_hw.h"
#include "audio_scene.h"

enum decorder_direction {
    DIRECTION_LEFT,
    DIRECTION_RIGHT,
};

typedef struct  {
    audio_type_t decoder_type;
    audio_decoder_param_t decoder_param;
    
    audio_hw_type_t hw_type;
    uint32_t hw_base_addr;
    uint32_t sample_rate;
    uint8_t channels;
} audio_le_sink_param_t;

extern audio_scene_operator_t audio_le_sink_operator;

#endif
