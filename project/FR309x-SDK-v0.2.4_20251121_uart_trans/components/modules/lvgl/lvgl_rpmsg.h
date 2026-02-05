#ifndef _LVGL_RPMSG_H
#define _LVGL_RPMSG_H

#include "rpmsg.h"

#define RPMSG_SYNC_FUNC_LVGL_DRAW_SW_TRANSFORM      RPMSG_SYNC_FUNC_MSG(RPMSG_SYNC_FUNC_TYPE_LVGL, 0x0001)
#define RPMSG_SYNC_FUNC_LVGL_ARGB_AND_RGB_AA        RPMSG_SYNC_FUNC_MSG(RPMSG_SYNC_FUNC_TYPE_LVGL, 0x0002)
#define RPMSG_SYNC_FUNC_LVGL_DRAW_SW_IMG_DECODED    RPMSG_SYNC_FUNC_MSG(RPMSG_SYNC_FUNC_TYPE_LVGL, 0x0003)
#define RPMSG_SYNC_FUNC_LVGL_DRAW_SW_BLEND          RPMSG_SYNC_FUNC_MSG(RPMSG_SYNC_FUNC_TYPE_LVGL, 0x0004)

struct rpmsg_sync_msg_lvgl_draw_sw_transform_t {
    void * draw_ctx;
    const void * dest_area;
    const void * src_buf;
    int16_t src_w;
    int16_t src_h;
    int16_t src_stride;
    const void * draw_dsc;
    uint8_t cf;
    void * cbuf;
    void * abuf;
    uint32_t ck;
};

struct rpmsg_sync_msg_lvgl_argb_and_rgb_aa_t {
    const uint8_t * src;
    int32_t src_offset;
    int16_t src_w;
    int16_t src_h;
    int16_t src_stride;
    int32_t xs_ups;
    int32_t ys_ups;
    int32_t xs_step;
    int32_t ys_step;
    int32_t x_end;
    void * cbuf;
    uint8_t * abuf;
    uint8_t cf;
    uint32_t ck;
};

struct rpmsg_sync_msg_lvgl_draw_sw_img_decoded_t {
    void * draw_ctx;
    const void * draw_dsc;
    const void * coords;
    const uint8_t * src_buf;
    uint8_t cf;
    uint8_t mask_any;
    uint32_t max_buf_size;
    uint32_t ck;
};

struct rpmsg_sync_msg_lvgl_draw_sw_blend_t {
    void * draw_ctx;
    const void * dsc;
};

void lvgl_handler(struct rpmsg_lite_instance *rpmsg, struct rpmsg_msg_t *msg);

#endif  // _LVGL_RPMSG_H
