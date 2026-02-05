#include "freetype_rpmsg.h"
#include "FreeRTOS.h"

int gray_raster_dsp_render( void *raster,
                  const void*  params )
{
    struct rpmsg_sync_msg_freetype_gray_raster_render_t *sync_msg;
    int result;
    uint32_t ret;

    fputc('<', NULL);
    sync_msg = pvPortMalloc(sizeof(struct rpmsg_sync_msg_freetype_gray_raster_render_t));
    if (sync_msg == NULL) {
        return NULL;
    }
    sync_msg->raster = raster;
    sync_msg->params = (void *)params;

    ret = rpmsg_sync_invoke(rpmsg_get_remote_instance(), RPMSG_SYNC_FUNC_FREETYPE_GRAY_RASTER_RENDER, sync_msg, (uint32_t *)&result);

    vPortFree(sync_msg);

    fputc('>', NULL);
    return result;
}

int gray_raster_dsp_new( void *memory, void *araster )
{
    struct rpmsg_sync_msg_freetype_gray_raster_new_t *sync_msg;
    int result;
    uint32_t ret;

    sync_msg = pvPortMalloc(sizeof(struct rpmsg_sync_msg_freetype_gray_raster_new_t));
    if (sync_msg == NULL) {
        return NULL;
    }
    sync_msg->memory = memory;
    sync_msg->araster = araster;

    ret = rpmsg_sync_invoke(rpmsg_get_remote_instance(), RPMSG_SYNC_FUNC_FREETYPE_GRAY_RASTER_NEW, sync_msg, (uint32_t *)&result);

    vPortFree(sync_msg);

    return result;
}

void gray_raster_dsp_done( void *raster )
{
    struct rpmsg_sync_msg_freetype_gray_raster_done_t *sync_msg;
    int result;
    uint32_t ret;

    sync_msg = pvPortMalloc(sizeof(struct rpmsg_sync_msg_freetype_gray_raster_done_t));
    if (sync_msg == NULL) {
        return;
    }
    sync_msg->raster = raster;

    ret = rpmsg_sync_invoke(rpmsg_get_remote_instance(), RPMSG_SYNC_FUNC_FREETYPE_GRAY_RASTER_DONE, sync_msg, (uint32_t *)&result);

    vPortFree(sync_msg);
}

void gray_raster_dsp_reset( void *raster, unsigned char *pool_base, unsigned long pool_size )
{
    struct rpmsg_sync_msg_freetype_gray_raster_reset_t *sync_msg;
    int result;
    uint32_t ret;

    sync_msg = pvPortMalloc(sizeof(struct rpmsg_sync_msg_freetype_gray_raster_reset_t));
    if (sync_msg == NULL) {
        return;
    }
    sync_msg->raster = raster;
    sync_msg->pool_base = pool_base;
    sync_msg->pool_size = pool_size;

    ret = rpmsg_sync_invoke(rpmsg_get_remote_instance(), RPMSG_SYNC_FUNC_FREETYPE_GRAY_RASTER_RESET, sync_msg, (uint32_t *)&result);

    vPortFree(sync_msg);
}

int gray_raster_dsp_set_mode( void *raster, unsigned long  mode, void *args )
{
    struct rpmsg_sync_msg_freetype_gray_raster_set_mode_t *sync_msg;
    int result;
    uint32_t ret;

    sync_msg = pvPortMalloc(sizeof(struct rpmsg_sync_msg_freetype_gray_raster_set_mode_t));
    if (sync_msg == NULL) {
        return NULL;
    }
    sync_msg->raster = raster;
    sync_msg->mode = mode;
    sync_msg->args = args;

    ret = rpmsg_sync_invoke(rpmsg_get_remote_instance(), RPMSG_SYNC_FUNC_FREETYPE_GRAY_RASTER_SET_MODE, sync_msg, (uint32_t *)&result);

    vPortFree(sync_msg);

    return result;
}
