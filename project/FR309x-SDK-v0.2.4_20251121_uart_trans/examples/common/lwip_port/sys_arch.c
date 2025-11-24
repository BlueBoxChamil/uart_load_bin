#include <stdint.h>

#include "fr30xx.h"
#include "co_log.h"

#include "lwip/opt.h"
#include "lwip/sys.h"

//#ifdef LOG_LOCAL_LEVEL
//#undef LOG_LOCAL_LEVEL
//#endif
//#define LOG_LOCAL_LEVEL             LOG_LEVEL_INFO

#if NO_SYS
//static uint32_t protect_counter;
//static uint32_t prim;

//void sys_arch_protect(void)
//{
//    uint32_t prim_tmp;
//    
//    prim_tmp = __get_PRIMASK();
//    __disable_irq();
//    
//    if (protect_counter == 0) {
//        prim = prim_tmp;
//    }
//    
//    protect_counter++;
//}

//void sys_arch_unprotect(void)
//{
//    protect_counter--;
//    
//    if (protect_counter == 0) {
//        if (!prim) {
//            __enable_irq();
//        }
//    }
//}

uint32_t sys_now(void)
{
    static uint32_t ticks = 0;
    return ticks++;
}
#else
uint32_t errno;

err_t sys_mutex_new(sys_mutex_t *mutex)
{
    LOG_INFO(NULL, "sys_mutex_new: 0x%08x.\r\n", mutex);
    if (mutex == NULL) {
        return ERR_MEM;
    }
    *mutex = xSemaphoreCreateMutex();
    return ERR_OK;
}

void sys_mutex_lock(sys_mutex_t *mutex)
{
    LOG_INFO(NULL, "sys_mutex_lock: 0x%08x.\r\n", mutex);
    xSemaphoreTake(*mutex, 0);
}

void sys_mutex_unlock(sys_mutex_t *mutex)
{
    LOG_INFO(NULL, "sys_mutex_unlock: 0x%08x.\r\n", mutex);
    xSemaphoreGive(*mutex);
}

void sys_mutex_free(sys_mutex_t *mutex)
{
    LOG_INFO(NULL, "sys_mutex_free: 0x%08x.\r\n", mutex);
    vSemaphoreDelete(*mutex);
}

err_t sys_sem_new(sys_sem_t *sem, u8_t count)
{
    LOG_INFO(NULL, "sys_sem_new: 0x%08x, %d.\r\n", sem, count);
    *sem = xSemaphoreCreateCounting(0xff, count);
    return ERR_OK;
}

int sys_sem_valid(sys_sem_t *mbox)
{
	if(*mbox)
		return 1;
	else
		return 0;
}

void sys_sem_set_invalid(sys_sem_t *mbox)
{
}

void sys_sem_signal(sys_sem_t *sem)
{
    LOG_INFO(NULL, "sys_sem_signal: 0x%08x.\r\n", sem);
    xSemaphoreGive(*sem);
}

u32_t sys_arch_sem_wait(sys_sem_t *sem, u32_t timeout)
{
    portTickType StartTime, EndTime, Elapsed;
    
    LOG_INFO(NULL, "sys_arch_sem_wait: 0x%08x, %d.\r\n", sem, timeout);
 
    StartTime = xTaskGetTickCount();
    if (timeout != 0) {
        if (xSemaphoreTake(*sem, timeout / portTICK_RATE_MS) == pdTRUE) {
            EndTime = xTaskGetTickCount();
            Elapsed = (EndTime - StartTime) * portTICK_RATE_MS;
            
            return (Elapsed);
        }
        else {
            return SYS_ARCH_TIMEOUT;
        }
    }
    else {
        xSemaphoreTake( *sem, portMAX_DELAY );
		EndTime = xTaskGetTickCount();
		Elapsed = (EndTime - StartTime) * portTICK_RATE_MS;
 
		return ( Elapsed );
    }
}

void sys_sem_free(sys_sem_t *sem)
{
    LOG_INFO(NULL, "sys_sem_free: 0x%08x.\r\n", sem);
    vSemaphoreDelete(*sem);
}

err_t sys_mbox_new(sys_mbox_t *mbox, int size)
{
    LOG_INFO(NULL, "sys_mbox_new: 0x%08x, %d.\r\n", mbox, size);
    *mbox = xQueueCreate(size, sizeof(void *));
    
    return ERR_OK;
}

int sys_mbox_valid(sys_mbox_t *mbox)
{
	if(*mbox)
		return 1;
	else
		return 0;
}

void sys_mbox_set_invalid(sys_mbox_t *mbox)
{
}

void sys_mbox_post(sys_mbox_t *mbox, void *msg)
{
    LOG_INFO(NULL, "sys_mbox_post: 0x%08x.\r\n", mbox);
    xQueueSend(*mbox, &msg, portMAX_DELAY);
}

err_t sys_mbox_trypost(sys_mbox_t *mbox, void *msg)
{
    LOG_INFO(NULL, "sys_mbox_trypost: 0x%08x.\r\n", mbox);
    if (xQueueSend(*mbox, &msg, 0) == pdPASS) {
        return ERR_OK;
    }
    else {
        return ERR_MEM;
    }
}

err_t sys_mbox_trypost_fromisr(sys_mbox_t *mbox, void *msg)
{
    LOG_INFO(NULL, "sys_mbox_trypost_fromisr: 0x%08x.\r\n", mbox);
    if (xQueueSendFromISR(*mbox, &msg, 0) == pdPASS) {
        return ERR_OK;
    }
    else {
        return ERR_MEM;
    }
}

u32_t sys_arch_mbox_fetch(sys_mbox_t *mbox, void **msg, u32_t timeout)
{
    void *dummyptr;
    portTickType StartTime, EndTime, Elapsed;
    
    LOG_INFO(NULL, "sys_arch_mbox_fetch: 0x%08x, %d.\r\n", mbox, timeout);
    
    StartTime = xTaskGetTickCount();
    if ( timeout != 0 ) {
		if ( pdTRUE == xQueueReceive( *mbox, &(*msg), timeout / portTICK_RATE_MS ) ) {
			EndTime = xTaskGetTickCount();
			Elapsed = (EndTime - StartTime) * portTICK_RATE_MS;
            
            LOG_INFO(NULL, "sys_arch_mbox_fetch: SUCCESS.\r\n");
			
			return ( Elapsed );
		}
		else {
            LOG_INFO(NULL, "sys_arch_mbox_fetch: FAILED.\r\n");
			*msg = NULL;
			
			return SYS_ARCH_TIMEOUT;
		}
	}
	else {
		xQueueReceive( *mbox, &(*msg), portMAX_DELAY );
		EndTime = xTaskGetTickCount();
		Elapsed = (EndTime - StartTime) * portTICK_RATE_MS;
		
		return ( Elapsed );	
	}
}

u32_t sys_arch_mbox_tryfetch(sys_mbox_t *mbox, void **msg)
{
    void *dummyptr;
    
    LOG_INFO(NULL, "sys_arch_mbox_fetch: 0x%08x.\r\n", mbox);
 
	if ( msg == NULL ) {
		msg = &dummyptr;
	}
 
    if ( pdTRUE == xQueueReceive( *mbox, &(*msg), 0 ) ) {
        LOG_INFO(NULL, "sys_arch_mbox_tryfetch: SUCCESS.\r\n");
        return ERR_OK;
    }
    else {
        LOG_INFO(NULL, "sys_arch_mbox_tryfetch: FAILED.\r\n");
        return SYS_MBOX_EMPTY;
    }
}

void sys_mbox_free(sys_mbox_t *mbox)
{
    LOG_INFO(NULL, "sys_mbox_free: 0x%08x.\r\n", mbox);
    vQueueDelete( *mbox );
}

sys_thread_t sys_thread_new(const char *name, lwip_thread_fn thread, void *arg, int stacksize, int prio)
{
    xTaskHandle CreatedTask;
    int result;

    result = xTaskCreate( thread, ( portCHAR * ) name, stacksize, arg, prio, &CreatedTask );
    if(result == pdPASS) {
        return CreatedTask;
    }
    else {
        return NULL;
    }
}

void sys_init(void)
{
}

u32_t sys_now(void)
{
    return xTaskGetTickCount() * portTICK_RATE_MS;
}

void sys_arch_msleep(u32_t ms)
{
    vTaskDelay(pdMS_TO_TICKS(ms));
}
#endif

