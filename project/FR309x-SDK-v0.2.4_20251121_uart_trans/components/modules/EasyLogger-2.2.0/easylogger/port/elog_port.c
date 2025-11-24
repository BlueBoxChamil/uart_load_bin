/*
 * This file is part of the EasyLogger Library.
 *
 * Copyright (c) 2015, Armink, <armink.ztl@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * 'Software'), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Function: Portable interface for each platform.
 * Created on: 2015-04-28
 */
 
#include <elog.h>

#include "driver_common.h"



extern UART_HandleTypeDef Uart0_handle;

#ifdef ELOG_ASYNC_OUTPUT_ENABLE

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

StaticSemaphore_t xSemaphoreBuffer_lock;
StaticSemaphore_t xSemaphoreBuffer_notice;

SemaphoreHandle_t xSemaphore_lock;
SemaphoreHandle_t xSemaphore_notice;

extern TaskHandle_t Task_A_handle;
extern TaskHandle_t Task_B_handle;
extern TaskHandle_t Task_C_handle;
#endif

/**
 * EasyLogger port initialize
 *
 * @return result
 */
ElogErrCode elog_port_init(void) {
    ElogErrCode result = ELOG_NO_ERR;

    /* add your code here */
    #ifdef ELOG_ASYNC_OUTPUT_ENABLE
    
    xSemaphore_lock = xSemaphoreCreateCountingStatic(9999, 1, &xSemaphoreBuffer_lock);
    xSemaphore_notice = xSemaphoreCreateCountingStatic(9999, 0, &xSemaphoreBuffer_notice);   
    
    #endif
    
    return result;
}

/**
 * output log port interface
 *
 * @param log output of log
 * @param size log size
 */
void elog_port_output(const char *log, size_t size) {
    
    /* add your code here */
    uart_transmit(&Uart0_handle, (uint8_t *)log, size);
}

/**
 * output lock
 */
void elog_port_output_lock(void) {
    
    /* add your code here */
#ifdef ELOG_ASYNC_OUTPUT_ENABLE
    xSemaphoreTake(xSemaphore_lock, portMAX_DELAY);
#elif
    __disable_irq();
#endif    
}

/**
 * output unlock
 */
void elog_port_output_unlock(void) {
    
    /* add your code here */
#ifdef ELOG_ASYNC_OUTPUT_ENABLE
    xSemaphoreGive(xSemaphore_lock);
#elif
    __enable_irq();
#endif 
}

/**
 * get current time interface
 *
 * @return current time
 */
const char *elog_port_get_time(void) {
    
    /* add your code here */
    return "2023/2/2";
}

/**
 * get current process name interface
 *
 * @return current process name
 */
const char *elog_port_get_p_info(void) {

    /* add your code here */
    return "";
}

/**
 * get current thread name interface
 *
 * @return current thread name
 */
const char *elog_port_get_t_info(void) {
    
    /* add your code here */
#ifdef ELOG_ASYNC_OUTPUT_ENABLE
    if (eTaskGetState(Task_A_handle) == eRunning)
        return "Task_A";
    else if (eTaskGetState(Task_B_handle) == eRunning)
        return "Task_B";
    else if (eTaskGetState(Task_C_handle) == eRunning)
        return "Task_C";
    else
        return "";
#else
    return "";
#endif
}

#ifdef ELOG_ASYNC_OUTPUT_ENABLE

void elog_async_output_notice(void) 
{
    xSemaphoreGive(xSemaphore_notice);
}

void async_output_task(void *arg) 
{
    size_t get_log_size = 0;
    static char poll_get_buf[ELOG_LINE_BUF_SIZE - 4];

    while(true) 
    {
        /* waiting log */
        xSemaphoreTake(xSemaphore_notice, portMAX_DELAY);   
        
        /* polling gets and outputs the log */
        while(true) 
        {
#ifdef ELOG_ASYNC_LINE_OUTPUT
            get_log_size = elog_async_get_line_log(poll_get_buf, sizeof(poll_get_buf));
#else
            get_log_size = elog_async_get_log(poll_get_buf, sizeof(poll_get_buf));
#endif

            if (get_log_size) 
            {   
                elog_port_output_lock();
                elog_port_output(poll_get_buf, get_log_size);
                elog_port_output_unlock();
            } 
            else 
            {
                break;
            }
        }  
    }
}
#endif