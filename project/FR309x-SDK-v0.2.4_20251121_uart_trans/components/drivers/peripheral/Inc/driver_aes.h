/*
  ******************************************************************************
  * @file    driver_aes.h
  * @author  FreqChip Firmware Team
  * @version V1.0.0
  * @date    2022
  * @brief   Header file of aes module.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 FreqChip.
  * All rights reserved.
  ******************************************************************************
*/
#ifndef __DRIVER_AES_H__
#define __DRIVER_AES_H__

#include "fr30xx.h"

/** @addtogroup AES_Registers_Section
  * @{
  */
/* ################################ Register Section Start ################################ */

/*AES CTRL REG  0x40*/
typedef volatile union
{
    volatile uint32_t Word;
    struct
    {
        uint32_t START                : 1;//AES work enable,this bit only maintain active for one cycle and is cleared automatically
        uint32_t KEY_INT_EN           : 1;//whehter key expand finish interrupt enable
        uint32_t DATA_INT_EN          : 1;//whether encrypt or decrypt finish interrupt enable
        uint32_t CBC                  : 1;//CBC mode or ECB mode
        uint32_t KEY_LEN              : 2;//00->128bit 01->192bit 10->256bit 11->reserved
        uint32_t OPCODE               : 2;//00->encrypt 01->decrypt 10->keyexpand 11->reserved
        uint32_t ENDIAN_SEL           : 1;//0->small endian for data register 1->big endian for data register
        uint32_t RSV                  : 23;
    } Bits;
} REG_AES_CTRL_t;

/*AES STATE REG  0x44*/
typedef volatile union
{
    volatile uint32_t Word;
    struct
    {
        uint32_t BUSY                   : 1;//busy indication bit
        uint32_t KEY_INT_FLAG           : 1;//key expand finish interrupt flag
        uint32_t DATA_INT_FLAG          : 1;//data finish interrupt flag
        uint32_t RSV                    : 29;
    } Bits; 
} REG_AES_STATE_t;

/* ------------------------------------------------*/
/*                   AES Register                  */
/* ------------------------------------------------*/
typedef struct 
{
    volatile uint32_t           DATAIN_0;     /* Offset 0x00 */
    volatile uint32_t           DATAIN_1;     /* Offset 0x04 */
    volatile uint32_t           DATAIN_2;     /* Offset 0x08 */
    volatile uint32_t           DATAIN_3;     /* Offset 0x0C */
    volatile uint32_t           KEY_0;        /* Offset 0x10 */
    volatile uint32_t           KEY_1;        /* Offset 0x14 */
    volatile uint32_t           KEY_2;        /* Offser 0x18 */
    volatile uint32_t           KEY_3;        /* Offser 0x1c */
    volatile uint32_t           KEY_4;        /* Offser 0x20 */
    volatile uint32_t           KEY_5;        /* Offser 0x24 */
    volatile uint32_t           KEY_6;        /* Offser 0x28 */
    volatile uint32_t           KEY_7;        /* Offset 0x2C */
    volatile uint32_t           IV_0;         /* Offset 0x30 */
    volatile uint32_t           IV_1;         /* Offset 0x34 */
    volatile uint32_t           IV_2;         /* Offset 0x38 */
    volatile uint32_t           IV_3;         /* Offset 0x3C */
    volatile REG_AES_CTRL_t     AES_CTRL;     /* Offset 0x40 */
    volatile REG_AES_STATE_t    AES_STATE;    /* Offset 0x44 */
    volatile uint32_t           DATAOUT_0;    /* Offset 0x48 */
    volatile uint32_t           DATAOUT_1;    /* Offset 0x4C */
    volatile uint32_t           DATAOUT_2;    /* Offset 0x50 */
    volatile uint32_t           DATAOUT_3;    /* Offset 0x54 */
}struct_SEC_AES_t;

#define SEC_AES    ((struct_SEC_AES_t *)(SEC_BASE))
/* ################################ Register Section END ################################## */
/**
  * @}
  */


/** @addtogroup AES_Initialization_Config_Section
  * @{
  */
/* ################################ Initialization Config Section Start ################################ */

#define AES_OPCODE_ENCRYPT       (0)
#define AES_OPCODE_DECRYPT       (1)
#define AES_OPCODE_KEY_EXPAND    (2)

#define AES_CBC_MODE_DISABLE     (0)
#define AES_CBC_MODE_ENABLE      (1)

typedef enum
{
    AES_ECB_128,
    AES_ECB_192,
    AES_ECB_256,

    AES_CBC_128,
    AES_CBC_192,
    AES_CBC_256,
}enum_AES_MODE_t;

typedef enum
{
    AES_SMALL_ENDIAN,    /* small endian for data register */
    AES_BIG_ENDIAN,      /* big endian for data register */
}enum_ENDIAN_t;

/* ################################ Initialization Config Section END ################################## */
/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/

/* get aes status */
#define __AES_IS_BUSY()                 (SEC_AES->AES_STATE.Bits.BUSY)
/* set aes opcode */
#define __AES_SET_OPCODE(__OPCODE__)    (SEC_AES->AES_CTRL.Bits.OPCODE = __OPCODE__)
/* aes work start */
#define __AES_WORK_START()              (SEC_AES->AES_CTRL.Bits.START = 1)


/* Exported functions --------------------------------------------------------*/

/* AES config */
void aes_config(enum_AES_MODE_t fe_Mode, enum_ENDIAN_t fe_Endian);

/* AES set key/iv */
void aes_set_encrypt_key(uint8_t *key);
void aes_set_encrypt_iv(uint8_t *iv);

/* AES encrypt/decrypt */
void aes_encrypt(uint8_t *fp_Data_In, uint32_t fu32_Size, uint8_t *fp_Data_Out);
void aes_decrypt(uint8_t *fp_Data_In, uint32_t fu32_Size, uint8_t *fp_Data_Out);

#endif
