/*
  ******************************************************************************
  * @file    driver_psram.h
  * @author  FreqChip Firmware Team
  * @version V1.1.0
  * @date    2024
  * @brief   Header file of PSRAM module.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 FreqChip.
  * All rights reserved.
  ******************************************************************************
*/
#ifndef __DRIVER_PSRAM_H__
#define __DRIVER_PSRAM_H__

#include "fr30xx.h"

/*
 * @brief   PSRAM Init structure definition
 */
typedef struct
{
    uint32_t Reserve;           /*!< reserve */
} PSRAM_InitTypeDef;

/* ################################ Initialization、Config Section END ################################## */
/**
  * @}
  */

enum psram_clk_sel_t {
    PSRAM_CLK_SEL_COREH,    // 24MHz
    PSRAM_CLK_SEL_SPLL,     // system PLL
    PSRAM_CLK_SEL_AUPLL,    // audio PLL
    PSRAM_CLK_SEL_MAX,
};

/* Exported functions --------------------------------------------------------*/

bool psram_init(enum psram_clk_sel_t clk_sel);
 
#endif  // __DRIVER_PSRAM_H__
