//#define BTDM_MEM_USE_FREERTOS_HEAP

#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "fr30xx.h"
#include "heap.h"

#ifdef BTDM_MEM_USE_FREERTOS_HEAP
#include "FreeRTOS.h"
#endif

#define configTOTAL_BTDM_HEAP_SIZE  ( ( 30 * 1024 ) )

static bool is_mem_poll_inited = false;

__attribute__((section("dram_section"))) static uint32_t ucHeap[ (configTOTAL_BTDM_HEAP_SIZE >> 2) + 0x10];

void * btdm_malloc( size_t xWantedSize )
{
#ifdef BTDM_MEM_USE_FREERTOS_HEAP
    return pvPortMalloc(xWantedSize);
#else
    if (is_mem_poll_inited == false) {
        is_mem_poll_inited = true;
        heap_mem_init(HEAP_TYPE_BTDM_BLOCK, (void *)ucHeap, configTOTAL_BTDM_HEAP_SIZE);
    }
    
    return heap_mem_alloc(HEAP_TYPE_BTDM_BLOCK, xWantedSize);
#endif
}

void btdm_free( void * pv )
{
#ifdef BTDM_MEM_USE_FREERTOS_HEAP
    vPortFree(pv);
#else
    heap_mem_free(pv);
#endif
}

__RAM_CODE void *btdm_calloc(unsigned int count, unsigned int size)
{
    void *ptr;
    
    ptr = btdm_malloc(count * size);
    if (ptr) {
        memset(ptr, 0, count * size);
    }
    
    return ptr;
}
