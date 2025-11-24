#ifndef _FREETYPE_RPMSG_H
#define _FREETYPE_RPMSG_H

#include "rpmsg.h"

#define RPMSG_SYNC_FUNC_FREETYPE_GRAY_RASTER_NEW            RPMSG_SYNC_FUNC_MSG(RPMSG_SYNC_FUNC_TYPE_FREETYPE, 0x0001)
#define RPMSG_SYNC_FUNC_FREETYPE_GRAY_RASTER_RESET          RPMSG_SYNC_FUNC_MSG(RPMSG_SYNC_FUNC_TYPE_FREETYPE, 0x0002)
#define RPMSG_SYNC_FUNC_FREETYPE_GRAY_RASTER_SET_MODE       RPMSG_SYNC_FUNC_MSG(RPMSG_SYNC_FUNC_TYPE_FREETYPE, 0x0003)
#define RPMSG_SYNC_FUNC_FREETYPE_GRAY_RASTER_RENDER         RPMSG_SYNC_FUNC_MSG(RPMSG_SYNC_FUNC_TYPE_FREETYPE, 0x0004)
#define RPMSG_SYNC_FUNC_FREETYPE_GRAY_RASTER_DONE           RPMSG_SYNC_FUNC_MSG(RPMSG_SYNC_FUNC_TYPE_FREETYPE, 0x0005)

struct rpmsg_sync_msg_freetype_gray_raster_new_t {
    void *memory;
    void *araster;
};

struct rpmsg_sync_msg_freetype_gray_raster_reset_t {
    void *raster;
    unsigned char *pool_base;
    unsigned long pool_size;
};

struct rpmsg_sync_msg_freetype_gray_raster_set_mode_t {
    void *raster;
    unsigned long mode;
    void *args;
};

struct rpmsg_sync_msg_freetype_gray_raster_render_t {
    void *raster;
    void *params;
};

struct rpmsg_sync_msg_freetype_gray_raster_done_t {
    void *raster;
};

void freetype_rpmsg_handler(struct rpmsg_lite_instance *rpmsg, struct rpmsg_msg_t *msg);

#endif  // _FREETYPE_RPMSG_H
