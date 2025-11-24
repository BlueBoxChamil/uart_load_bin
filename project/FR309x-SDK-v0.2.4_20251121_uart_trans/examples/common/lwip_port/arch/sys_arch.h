#ifndef _SYS_ARCH_H
#define _SYS_ARCH_H

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

typedef QueueHandle_t       sys_mbox_t;
typedef SemaphoreHandle_t   sys_sem_t;
typedef SemaphoreHandle_t   sys_mutex_t;
typedef TaskHandle_t        sys_thread_t;

#define sys_msleep          sys_arch_msleep

#endif  // _SYS_ARCH_H
