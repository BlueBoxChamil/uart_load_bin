/*
  ******************************************************************************
  * @file    driver_crc.h
  * @author  FreqChip Firmware Team
  * @version V1.0.0
  * @date    2024
  * @brief   Header file of CRC module.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 FreqChip.
  * All rights reserved.
  ******************************************************************************
*/
#ifndef __DRIVER_CRC_H__
#define __DRIVER_CRC_H__

#include "fr30xx.h"

/** @addtogroup CRC_Registers_Section
  * @{
  */
/* ################################ Register Section Start ################################ */

#define CRC_START (0x01)
#define CRC_CLEAR (0x08)

typedef struct 
{
    volatile   uint32_t           CRC_CTRL;           /* Offset 0x00 */
    volatile   uint32_t           CRC_STATUS;         /* Offset 0x04 */
    volatile   uint32_t           CRC_FIFO_DATA;      /* Offset 0x08*/
    volatile   uint32_t           CRC_RESULT;         /* Offset 0x0C */  
}struct_CRC_t;

#define CRC        ((struct_CRC_t *)CRC_BASE)

/* ################################ Register Section END ################################## */
/**
  * @}
  */

/** @addtogroup CRC_Initialization_Config_Section
  * @{
  */
/* ################################ Initialization_Config Section Start ################################ */

typedef enum
{
    CRC_INITVALUE_0          = 0x08u,
    CRC_INITVALUE_1          = 0x18u,

    CRC_MULTINOMIAL_16_1021  = 0x02u,
    CRC_MULTINOMIAL_16_8005  = 0x04u,
    CRC_MULTINOMIAL_32       = 0x06u,
}enum_CRC_Accumulate;

typedef enum
{                                                                       //  bit wide  |  polynomial  |  init value  |  Result XOR value  |  Input invert  |  Output invert
    CRC8                =  (CRC_INITVALUE_0),                           //     8      |     07       |     00       |        00          |     fasle      |      fasle
    CRC16_CCITT_FALSE   =  (CRC_INITVALUE_1 | CRC_MULTINOMIAL_16_1021), //     16     |     1021     |     FFFF     |        FFFF        |     fasle      |      fasle
    CRC16_XMODEM        =  (CRC_INITVALUE_0 | CRC_MULTINOMIAL_16_1021), //     16     |     1021     |     0000     |        0000        |     fasle      |      fasle
    CRC32_MPEG2         =  (CRC_INITVALUE_1 | CRC_MULTINOMIAL_32),      //     32     |     04C11DB7 |     FFFFFFFF |        00000000    |     fasle      |      fasle
}enum_CRC_MODE_SEL_t;

/* ################################ Initialization_Config Section END ################################## */
/**
  * @}
  */

/* Exported functions ---------------------------------------------------------*/

/* Initial crc with initial value and mode */
void crc_init(enum_CRC_MODE_SEL_t fe_crc_mode);

/* CRC Calculate */
uint32_t crc_Calculate(uint8_t *fp_Data, uint32_t fu32_size);

#endif
