/*
  ******************************************************************************
  * @file    driver_wdt.h
  * @author  FreqChip Firmware Team
  * @version V1.0.0
  * @date    2022
  * @brief   Header file of WDT HAL module.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 FreqChip.
  * All rights reserved.
  ******************************************************************************
*/
#ifndef __DRIVER_WDT_H__
#define __DRIVER_WDT_H__

#include "fr30xx.h"

/** @addtogroup WDT_Registers_Section
  * @{
  */
/* ################################ Register Section Start ################################ */

/* WDT Control Register */
typedef struct
{
    uint32_t WDT_EN : 1;
    uint32_t RMOD   : 1;
    uint32_t RPL    : 3;
    uint32_t rsv_0  : 27;
}wdt_ctrl_t;

/* Timeout Range Register */
typedef struct
{
    uint32_t TOP      : 4;
    uint32_t TOP_INIT : 4;
    uint32_t rsv_0    : 24;
}wdt_timout_range_t;

typedef struct 
{
    volatile wdt_ctrl_t            wdt_CR;            /* Offset 0x00 */
    volatile wdt_timout_range_t    wdt_TORR;          /* Offset 0x04 */
    volatile uint32_t              wdt_CCVR;          /* Offset 0x08 */
    volatile uint32_t              wdt_CRR;           /* Offset 0x0C */
    volatile uint32_t              wdt_STAT;          /* Offset 0x10 */
    volatile uint32_t              wdt_EOI;           /* Offset 0x14 */
    volatile uint32_t              rsv_0;             /* Offset 0x18 */
    volatile uint32_t              rsv_1;             /* Offset 0x1C */
    volatile uint32_t              wdt_CNT;           /* Offset 0x20 */
}struct_WDT_t;

#define WDT    ((struct_WDT_t *)(WDT_BASE))

/* ################################ Register Section END ################################## */
/**
  * @}
  */


/** @addtogroup WDT_Initialization_Config_Section
  * @{
  */
/* ################################ Initialization¡¢Config Section Start ################################ */

typedef enum
{
    WDT_MODE_0,    /* Timeout reset system */
    WDT_MODE_1,    /* Timeout generate an interrupt, 
                      if the interrupt is not cleared by the time a second timeout occurs, then it generates a system reset */
}enum_WDTMode_t;

/* ################################ Initialization¡¢Config Section END ################################## */
/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/* wdt_init */
void wdt_init(enum_WDTMode_t fe_mode, uint32_t fu32_WDTCount);

/* wdt_start */
/* wdt_stop */
void wdt_start(void);
void wdt_stop(void);

/* wdt_feed */
void wdt_feed(void);

#endif

