/*
 ******************************************************************************
 * @file    driver_aes.c
 * @author  FreqChip Firmware Team
 * @version V1.0.0
 * @date    2022
 * @brief   IIR module driver.
 *          This file provides firmware functions to manage the 
 *          SEC AES peripheral
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 FreqChip.
 * All rights reserved.
 ******************************************************************************
*/
#include "fr30xx.h"

/*********************************************************************
* @fn      aes_config
*
* @brief   config the aes mode and Endia.
*
* @param   fe_Mode : select AES mode.
* @param   fe_Endian: select AES Endian.
*/
void aes_config(enum_AES_MODE_t fe_Mode, enum_ENDIAN_t fe_Endian)
{
    /* AES Endian */
    SEC_AES->AES_CTRL.Bits.ENDIAN_SEL = fe_Endian;
    /* AES Mode */
    if (fe_Mode >= AES_CBC_128){
        SEC_AES->AES_CTRL.Bits.CBC = AES_CBC_MODE_ENABLE;
        SEC_AES->AES_CTRL.Bits.KEY_LEN = fe_Mode - AES_CBC_128;
    }
    else{
        SEC_AES->AES_CTRL.Bits.CBC = AES_CBC_MODE_DISABLE;
        SEC_AES->AES_CTRL.Bits.KEY_LEN = fe_Mode;
    }
}

/*********************************************************************
* @fn      aes_set_encrypt_key
*
* @brief   Set AES encryption key.
*
* @param   key : points to the AES key.
*/
void aes_set_encrypt_key(uint8_t *key)
{
    uint32_t *lp_Key = (uint32_t *)key;

    SEC_AES->KEY_0 = lp_Key[0];
    SEC_AES->KEY_1 = lp_Key[1];
    SEC_AES->KEY_2 = lp_Key[2];
    SEC_AES->KEY_3 = lp_Key[3];

    switch(SEC_AES->AES_CTRL.Bits.KEY_LEN)
    {
        case AES_ECB_192:{
            SEC_AES->KEY_4 = lp_Key[4];
            SEC_AES->KEY_5 = lp_Key[5];
        }break;

        case AES_ECB_256:{
            SEC_AES->KEY_4 = lp_Key[4];
            SEC_AES->KEY_5 = lp_Key[5];
            SEC_AES->KEY_6 = lp_Key[6];
            SEC_AES->KEY_7 = lp_Key[7];
        }break;

        default:break;
    }
}

/*********************************************************************
* @fn      aes_set_encrypt_iv
*
* @brief   Set IV for the encrypt/decrypt.
*
* @param   key : points to the AES key.
*/
void aes_set_encrypt_iv(uint8_t *iv)
{
    uint32_t *lp_IV = (uint32_t *)iv;

    SEC_AES->IV_0 = lp_IV[0];
    SEC_AES->IV_1 = lp_IV[1];
    SEC_AES->IV_2 = lp_IV[2];
    SEC_AES->IV_3 = lp_IV[3];
}

/*********************************************************************
* @fn      aes_encrypt
*
* @brief   aes encrypt. 
*
* @param   fp_Data_In:  the pointer of data which is to encrypt.
* @param   fu32_Size:   encrypt size.
* @param   fp_Data_Out: the pointer of data which saves the encryption data.
*/
void aes_encrypt(uint8_t *fp_Data_In, uint32_t fu32_Size, uint8_t *fp_Data_Out)
{
    uint32_t *DataIN  = (uint32_t *)fp_Data_In;
    uint32_t *DataOUT = (uint32_t *)fp_Data_Out;

    uint32_t InIndex = 0;
    uint32_t lu32_size;

    lu32_size = fu32_Size/16;
    if (fu32_Size % 16)
        lu32_size++;

    /* Block 16 byte */
    while(InIndex < lu32_size)
    {
        SEC_AES->DATAIN_0 = *DataIN++;
        SEC_AES->DATAIN_1 = *DataIN++;
        SEC_AES->DATAIN_2 = *DataIN++;
        SEC_AES->DATAIN_3 = *DataIN++;

        __AES_SET_OPCODE(AES_OPCODE_ENCRYPT);
        __AES_WORK_START();

        while(__AES_IS_BUSY());

        *DataOUT++ = SEC_AES->DATAOUT_0;
        *DataOUT++ = SEC_AES->DATAOUT_1;
        *DataOUT++ = SEC_AES->DATAOUT_2;
        *DataOUT++ = SEC_AES->DATAOUT_3;

        InIndex++;
    }
}

/*********************************************************************
* @fn      aes_decrypt
*
* @brief   aes decrypt
* 
* @param   fp_Data_In:  the pointer of data which is to decrypt.
* @param   fu32_Size:   decrypt size.
* @param   fp_Data_Out: the pointer of data which saves the decryption data.
*/
void aes_decrypt(uint8_t *fp_Data_In, uint32_t fu32_Size, uint8_t *fp_Data_Out)
{
    uint32_t *DataIN  = (uint32_t *)fp_Data_In;
    uint32_t *DataOUT = (uint32_t *)fp_Data_Out;

    uint32_t InIndex = 0;
    uint32_t lu32_size;

    lu32_size = fu32_Size/16;
    if (fu32_Size % 16)
        lu32_size++;

    while (InIndex < lu32_size)
    {
        SEC_AES->DATAIN_0 = *DataIN++;
        SEC_AES->DATAIN_1 = *DataIN++;
        SEC_AES->DATAIN_2 = *DataIN++;
        SEC_AES->DATAIN_3 = *DataIN++;

        __AES_SET_OPCODE(AES_OPCODE_KEY_EXPAND);
        __AES_WORK_START();
        while(__AES_IS_BUSY());

        __AES_SET_OPCODE(AES_OPCODE_DECRYPT);
        __AES_WORK_START();
        while(__AES_IS_BUSY());

        *DataOUT++ = SEC_AES->DATAOUT_0;
        *DataOUT++ = SEC_AES->DATAOUT_1;
        *DataOUT++ = SEC_AES->DATAOUT_2;
        *DataOUT++ = SEC_AES->DATAOUT_3;
        
        InIndex++;
    }
}
