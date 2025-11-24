//#define ENABLE_NS
//#define ENABLE_LOOP

#include "audio_scene.h"
#include "audio_hw.h"
#include "audio_encoder.h"
#include "dsp_mem.h"
#ifdef ENABLE_NS
#include "algorithm.h"
#endif

#include "audio_le_recorder.h"

#ifndef ENABLE_NS
#define STORE_MIC_PCM_DATA_SAMPLES      128
#endif

#ifndef LE_AUDIO_LOW_LATENCY
#define CODEC_DATA_SIZE     CODEC_FIFO_HALF_DEPTH
#else
#define CODEC_DATA_SIZE     30
#endif

#define ENABLE_COMPENSATE

typedef struct {
    audio_hw_t *hw;
    audio_hw_output_t *hw_output;
    audio_encoder_t *encoder;
    
#ifdef ENABLE_NS
    void *ns_handle;
#endif
    
    /* used to store mic PCM data */
    uint16_t *buffer;
    
#ifdef ENABLE_NS
    uint32_t frame_samples;
    uint32_t store_offset;
#endif
    
#ifdef ENABLE_LOOP
    int16_t *ns_result_buffer;
    uint32_t ns_result_buffer_total_sample;
    uint32_t ns_result_buffer_read_offset;
    uint32_t ns_result_buffer_write_offset;
#endif
} recorder_env_t;

typedef struct compensate_info
{
    co_list_hdr_t hdr;
    uint32_t compensate_samples_num;
    uint8_t  compensate_state;
} compensate_info_t;

static audio_scene_t *recoder_scene = NULL;

static co_list_t compensate_list;
static bool compensate_ongoing = false;
static bool compensate_wait_encorder = false;

static void hw_receive_pcm(uint32_t samples)
{
#ifdef ENABLE_COMPENSATE
    if ((compensate_ongoing == false) && (compensate_wait_encorder == false))
    {
        compensate_info_t *temp_compensate = (compensate_info_t *)co_list_pick(&compensate_list);

        if (temp_compensate)
        {
            uint8_t codec_fifo_thd = CODEC_DATA_SIZE;

            if (temp_compensate->compensate_state == 0)
            {
                codec_fifo_thd -= temp_compensate->compensate_samples_num;
            }
            else
            {
                codec_fifo_thd += temp_compensate->compensate_samples_num;
            }

            __CODEC_ADCFF_L_FULL_THRESHOLD(codec_fifo_thd);
            __CODEC_ADCFF_R_FULL_THRESHOLD(codec_fifo_thd);

            compensate_ongoing = true;
        }
    }
    else if (compensate_ongoing == true)
    {
        compensate_info_t *temp_compensate = (compensate_info_t *)co_list_pop_front(&compensate_list);

        if (temp_compensate)
        {
            __CODEC_ADCFF_L_FULL_THRESHOLD(CODEC_DATA_SIZE);
            __CODEC_ADCFF_R_FULL_THRESHOLD(CODEC_DATA_SIZE);

            vPortFree(temp_compensate);
        }

        compensate_ongoing = false;
        compensate_wait_encorder = true;
    }
#endif

    audio_scene_evt_hw_in_new_samples_t *evt = (void *)pvPortMalloc(sizeof(audio_scene_evt_hw_in_new_samples_t));
    
    if (evt) {
        evt->evt.type = AUDIO_SCENE_EVT_TYPE_HW_IN_NEW_SAMPLES;
        evt->evt.scene = recoder_scene;
        evt->adc_new_samples = samples;
        audio_scene_send_event(&evt->evt);
    }
}

void print_int16(uint16_t value);
#ifdef ENABLE_LOOP
static void hw_request_pcm(void *pcm, uint32_t samples, uint8_t channels)
{
    recorder_env_t *env = recoder_scene->env;
    uint32_t last_samples = env->ns_result_buffer_total_sample - env->ns_result_buffer_read_offset;
    if (last_samples > samples) {
        memcpy(pcm, (void *)&env->ns_result_buffer[env->ns_result_buffer_read_offset], samples * sizeof(int16_t));
        env->ns_result_buffer_read_offset += samples;
        samples = 0;
    }
    else {
        memcpy(pcm, (void *)&env->ns_result_buffer[env->ns_result_buffer_read_offset], last_samples * sizeof(int16_t));
        env->ns_result_buffer_read_offset = 0;
        samples -= last_samples;
        if (samples) {
            memcpy((void *)((int16_t *)pcm + last_samples), (void *)&env->ns_result_buffer[0], samples * sizeof(int16_t));
            env->ns_result_buffer_read_offset = samples;
        }
    }
}
#endif

static audio_scene_t *allocate(void *_param)
{
    audio_scene_t *scene = pvPortMalloc(sizeof(audio_scene_t));

    if (scene) {
        recorder_env_t *env = pvPortMalloc(sizeof(recorder_env_t));
        if (env == NULL) {
            vPortFree(scene);
            return NULL;
        }
        
        recorder_param_t *param = pvPortMalloc(sizeof(recorder_param_t));
        if (param == NULL) {
            vPortFree(env);
            vPortFree(scene);
            return NULL;
        }
        
        scene->env = env;
        memcpy((void *)param, _param, sizeof(recorder_param_t));
        scene->param = param;
        scene->op = &audio_le_recoder_operator;
    }
    
    return scene;
}

static void init(audio_scene_t *scene)
{
    recorder_env_t *env = scene->env;
    recorder_param_t *param = scene->param;
    uint32_t frame_size;
    
    recoder_scene = scene;

    co_list_init(&compensate_list);

#ifdef ENABLE_NS
    env->store_offset = 0;
    env->ns_handle = ns_init(param->sample_rate, 3, &env->frame_samples);
    env->buffer = pvPortMalloc(env->frame_samples * sizeof(int16_t) * param->channels);
#else
    env->buffer = pvPortMalloc(STORE_MIC_PCM_DATA_SAMPLES * sizeof(int16_t) * param->channels);
#endif
    
#ifdef ENABLE_LOOP
#ifdef ENABLE_NS
    env->ns_result_buffer = pvPortMalloc(env->frame_samples * sizeof(int16_t) * param->channels * 3);
    env->ns_result_buffer_total_sample = env->frame_samples * 3;
    env->ns_result_buffer_read_offset = env->frame_samples * 1;
    env->ns_result_buffer_write_offset = 0;
#else
    env->ns_result_buffer = pvPortMalloc(STORE_MIC_PCM_DATA_SAMPLES * 2 * sizeof(int16_t) * param->channels);
    env->ns_result_buffer_total_sample = STORE_MIC_PCM_DATA_SAMPLES * 2;
    env->ns_result_buffer_read_offset = STORE_MIC_PCM_DATA_SAMPLES;
    env->ns_result_buffer_write_offset = 0;
#endif
#endif
    
#ifdef ENABLE_LOOP
    env->hw = audio_hw_create(param->hw_type, hw_request_pcm, param->base_addr, AUDIO_HW_DIR_INOUT, param->sample_rate, param->channels);
#else
    env->hw = audio_hw_create(param->hw_type, NULL, param->base_addr, AUDIO_HW_DIR_IN, param->sample_rate, param->channels);
#endif

    __SYSTEM_CODEC_CLK_DISABLE();
    NVIC_DisableIRQ(CODEC_IRQn);

    env->hw_output = audio_hw_output_add(env->hw, hw_receive_pcm);
    /* create encoder */
    env->encoder = audio_encoder_init(param->encoder_type, param->channels, param->sample_rate, FRAME_MAX_LENGTH_FIT_SINGLE, &param->encoder_param);
}

static void destroy(audio_scene_t *scene)
{
    recorder_env_t *env = scene->env;
    
    audio_hw_destroy(env->hw);
    audio_encoder_destroy(env->encoder);
    
#ifdef ENABLE_NS
    ns_destroy(env->ns_handle);
#endif
#ifdef ENABLE_LOOP
    vPortFree(env->ns_result_buffer);
#endif
    
    vPortFree(env->buffer);
    vPortFree(scene->env);
    vPortFree(scene->param);
    vPortFree(scene);
    recoder_scene = NULL;

    compensate_info_t *curr_compensate = (compensate_info_t *)co_list_pick(&compensate_list);
    compensate_info_t *next_compensate;
    while (curr_compensate)
    {
        next_compensate = (compensate_info_t *)co_list_next(&curr_compensate->hdr);

        co_list_extract(&compensate_list, &curr_compensate->hdr);
        vPortFree(curr_compensate);

        curr_compensate = next_compensate;
    }

    compensate_ongoing = false;
    compensate_wait_encorder = false;
}

static void event_handler(audio_scene_t *scene, audio_scene_evt_t *evt)
{
    recorder_env_t *env = scene->env;
    recorder_param_t *param = scene->param;

    switch(evt->type) {
        case AUDIO_SCENE_EVT_TYPE_HW_IN_NEW_SAMPLES:
            {
                int encoded_frame_count;
                audio_scene_evt_hw_in_new_samples_t *_evt = (void *)evt;
                uint32_t adc_new_samples = _evt->adc_new_samples;
#ifdef ENABLE_NS
                while (adc_new_samples) {
                    uint32_t last_space = env->frame_samples - env->store_offset;
                    if (adc_new_samples >= last_space) {
                        int16_t *out = NULL;
                        audio_hw_read_pcm(env->hw_output, &env->buffer[env->store_offset*param->channels], last_space, param->channels);
                        ns_exec(env->ns_handle, (const int16_t *)&env->buffer[0], &out);
#ifdef ENABLE_LOOP
                        {
                            uint32_t write_last_space = env->ns_result_buffer_total_sample - env->ns_result_buffer_write_offset;
                            if (write_last_space > env->frame_samples) {
                                memcpy(&env->ns_result_buffer[env->ns_result_buffer_write_offset], out, sizeof(int16_t) * env->frame_samples);
                                env->ns_result_buffer_write_offset += env->frame_samples;
                            }
                            else {
                                uint32_t last_saved_samples = write_last_space;
                                memcpy(&env->ns_result_buffer[env->ns_result_buffer_write_offset], out, sizeof(int16_t) * write_last_space);
                                write_last_space = env->frame_samples - write_last_space;
                                if (write_last_space) {
                                    memcpy(&env->ns_result_buffer[0], (void *)&out[last_saved_samples], sizeof(int16_t) * write_last_space);
                                    env->ns_result_buffer_write_offset = write_last_space;
                                }
                                else {
                                    env->ns_result_buffer_write_offset = 0;
                                }
                            }
                        }
#endif
                        audio_encoder_encode(env->encoder, (void *)out, env->frame_samples*sizeof(uint16_t)*param->channels, param->channels, param->sample_rate);
                        env->store_offset = 0;
                        adc_new_samples -= last_space;
                    }
                    else {
                        audio_hw_read_pcm(env->hw_output, &env->buffer[env->store_offset*param->channels], adc_new_samples, param->channels);
                        env->store_offset += adc_new_samples;
                        adc_new_samples = 0;
                    }
                }
#else
                while(adc_new_samples) {
                    uint32_t samples = adc_new_samples > STORE_MIC_PCM_DATA_SAMPLES ? STORE_MIC_PCM_DATA_SAMPLES : adc_new_samples;
                    samples = audio_hw_read_pcm(env->hw_output, env->buffer, samples, param->channels);
#ifdef ENABLE_LOOP
                    memcpy(&env->ns_result_buffer[env->ns_result_buffer_write_offset], env->buffer, sizeof(int16_t) * samples);
                    env->ns_result_buffer_write_offset += samples;
                    if (env->ns_result_buffer_write_offset >= env->ns_result_buffer_total_sample) {
                        env->ns_result_buffer_write_offset = 0;
                    }
#endif              

#ifdef ENABLE_COMPENSATE

                    if (samples < CODEC_DATA_SIZE)
                    {
                        uint8_t samples_diff = CODEC_DATA_SIZE - samples;
                        for (uint8_t i = 0; i < samples_diff; i ++)
                        {
                            env->buffer[samples + i] = env->buffer[samples - samples_diff + i];
                        }
                    }
                    else if (samples > CODEC_DATA_SIZE)
                    {
                        uint8_t samples_diff = samples - CODEC_DATA_SIZE;
                    }
                    else
                    {

                    }

                    samples = CODEC_DATA_SIZE;
                    adc_new_samples = CODEC_DATA_SIZE;
#endif

                    audio_encoder_encode(env->encoder, (void *)env->buffer, samples*sizeof(uint16_t)*param->channels, param->channels, param->sample_rate);

                    adc_new_samples -= samples;
                }
#endif
                encoded_frame_count = audio_encoder_get_frame_count(env->encoder);
                while(encoded_frame_count--) {
                    audio_encoder_frame_t *frame;
                    frame = audio_encoder_frame_pop(env->encoder);
                    if (param->report_cb) {
                        param->report_cb(param->report_param, frame->data, frame->length);
                    }
                    audio_encoder_frame_release(frame);
#ifdef ENABLE_COMPENSATE
                    if (compensate_wait_encorder == true)
                        compensate_wait_encorder = false;
#endif
                }
            }
            break;
        default:
            break;
    }
}

void audio_le_recorder_compensate(uint32_t num, uint8_t state)
{
    compensate_info_t *new_elem = NULL;

    for (uint8_t i = 0; i < num; i ++)
    {
        new_elem = (compensate_info_t *)pvPortMalloc(sizeof(compensate_info_t));

        if (new_elem)
        {
            new_elem->compensate_samples_num = 1;
            new_elem->compensate_state = state;

            if(xPortIsInsideInterrupt()) 
            {
                uint32_t old_basepri = taskENTER_CRITICAL_FROM_ISR();
                co_list_push_back(&compensate_list, &new_elem->hdr);
                taskEXIT_CRITICAL_FROM_ISR(old_basepri);
            }
            else
            {
                taskENTER_CRITICAL();
                co_list_push_back(&compensate_list, &new_elem->hdr);
                taskEXIT_CRITICAL();
            }
        }

        new_elem = NULL;
    }
}

__RAM_CODE void audio_le_recorder_start(void)
{
    __CODEC_ALL_MODULE_RESET();
    __SYSTEM_CODEC_CLK_ENABLE();
    NVIC_EnableIRQ(CODEC_IRQn);
}

audio_scene_operator_t audio_le_recoder_operator = {
    .allocate = allocate,
    .init = init,
    .destroy = destroy,
    .event_handler = event_handler,
    .decoder_started = NULL,
    .support_tone = false,
};
