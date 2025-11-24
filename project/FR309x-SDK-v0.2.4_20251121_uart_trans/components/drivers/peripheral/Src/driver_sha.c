/*
  ******************************************************************************
  * @file    driver_sha.c
  * @author  FreqChip Firmware Team
  * @version V1.0.0
  * @date    2022
  * @brief   IIR module driver.
  *          This file provides firmware functions to manage the 
  *          SEC SHA peripheral
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 FreqChip.
  * All rights reserved.
  ******************************************************************************
*/
#include "fr30xx.h"

static uint8_t  Context[128];    /* orignal data save buffer */
static uint16_t ContextIndex = 0;
static uint64_t TotalBits512[2];
static uint64_t TotalBits256;

/*********************************************************************
 * @fn      sha_init
 *
 * @brief   SHA256 initialization procedure
 *
 * @param   mode : SHA mode select
 */
void sha_init(enum_sha_mode_t fe_Mode)
{
    SEC_SHA->SHA_CTRL.Bits.MODE        = fe_Mode;
    SEC_SHA->SHA_CTRL.Bits.ENDIAN_MODE = SHA_BIG_ENDIAN;
    SEC_SHA->SHA_CTRL.Bits.ISR_EN      = 1;
    
    SEC_SHA->SHA_CTRL.Bits.INIT_EN     = 1;
    
    /*verity initialize*/
    TotalBits256    = 0;

    TotalBits512[0] = 0;
    TotalBits512[1] = 0;

    ContextIndex    = 0;
}

static void __sha_updata_256(uint8_t *fp8_Data, uint32_t fu32_Size);
static void __sha_updata_512(uint8_t *fp8_Data, uint32_t fu32_Size);

/*********************************************************************
 * @fn      sha_update
 *
 * @brief   SHA update procedure.
 *
 * @param   fp8_Data : message to hash.
 * @param   fu32_Size: length of message to hash.
 */
void sha_update(uint8_t *fp8_Data, uint32_t fu32_Size)
{
    if(SEC_SHA->SHA_CTRL.Bits.MODE >= SHA_512)
    {
        __sha_updata_512(fp8_Data, fu32_Size);
    }
    else
    {
        __sha_updata_256(fp8_Data, fu32_Size);
    }
}

/*********************************************************************
 * @fn      sha_final
 *
 * @brief   SHA Final calculation result
 *
 * @param   DataOut : SHA calculation result buffer.
 */
void sha_final(uint8_t *DataOut)
{
    uint32_t *DataIn = NULL;
    /*sha512 or sha384 or sha512/256 or sha512/224*/
    if(SEC_SHA->SHA_CTRL.Bits.MODE >= SHA_512)
    {
        if((TotalBits512[0] == 0) && (TotalBits512[1] == 0)) return;
        uint32_t LeftBits = (ContextIndex << 3);
        if(LeftBits == 0)
        {
            Context[0] = 0x80;
            memset(&Context[1], 0, (SHA_512_BLOCK_SIZE - 1 - 16));
            Context[SHA_512_BLOCK_SIZE - 1] = (uint8_t)  TotalBits512[0];
            Context[SHA_512_BLOCK_SIZE - 2] = (uint8_t) (TotalBits512[0] >> 8);
            Context[SHA_512_BLOCK_SIZE - 3] = (uint8_t) (TotalBits512[0] >> 16);
            Context[SHA_512_BLOCK_SIZE - 4] = (uint8_t) (TotalBits512[0] >> 24);
            Context[SHA_512_BLOCK_SIZE - 5] = (uint8_t) (TotalBits512[0] >> 32);
            Context[SHA_512_BLOCK_SIZE - 6] = (uint8_t) (TotalBits512[0] >> 40);
            Context[SHA_512_BLOCK_SIZE - 7] = (uint8_t) (TotalBits512[0] >> 48);
            Context[SHA_512_BLOCK_SIZE - 8] = (uint8_t) (TotalBits512[0] >> 56);
            
            Context[SHA_512_BLOCK_SIZE - 9] = (uint8_t)  TotalBits512[1];
            Context[SHA_512_BLOCK_SIZE - 10] = (uint8_t) (TotalBits512[1] >> 8);
            Context[SHA_512_BLOCK_SIZE - 11] = (uint8_t) (TotalBits512[1] >> 16);
            Context[SHA_512_BLOCK_SIZE - 12] = (uint8_t) (TotalBits512[1] >> 24);
            Context[SHA_512_BLOCK_SIZE - 13] = (uint8_t) (TotalBits512[1] >> 32);
            Context[SHA_512_BLOCK_SIZE - 14] = (uint8_t) (TotalBits512[1] >> 40);
            Context[SHA_512_BLOCK_SIZE - 15] = (uint8_t) (TotalBits512[1] >> 48);
            Context[SHA_512_BLOCK_SIZE - 16] = (uint8_t) (TotalBits512[1] >> 56); 
        }
        else if(LeftBits >= 896)
        {
            Context[ContextIndex++] = 0x80;
            memset(&Context[ContextIndex], 0, (SHA_512_BLOCK_SIZE - ContextIndex));
            
            DataIn = (uint32_t *)Context;
            for(uint32_t i=0; i<16;i++)
            {
                SEC_SHA->DATA_1[i] = DataIn[i];
                SEC_SHA->DATA_2[i] = DataIn[i + 16];
            }
            
            SEC_SHA->SHA_CTRL.Bits.CALCULATE = 1;
            while(SEC_SHA->SHA_INT_STATE.Bits.INT_DONE == 0);
            SEC_SHA->SHA_INT_STATE.Bits.INT_DONE = 0;
            
            /*one more need to process*/
            memset(&Context[0], 0, (SHA_512_BLOCK_SIZE - 16));
            Context[SHA_512_BLOCK_SIZE - 1] = (uint8_t)  TotalBits512[0];
            Context[SHA_512_BLOCK_SIZE - 2] = (uint8_t) (TotalBits512[0] >> 8);
            Context[SHA_512_BLOCK_SIZE - 3] = (uint8_t) (TotalBits512[0] >> 16);
            Context[SHA_512_BLOCK_SIZE - 4] = (uint8_t) (TotalBits512[0] >> 24);
            Context[SHA_512_BLOCK_SIZE - 5] = (uint8_t) (TotalBits512[0] >> 32);
            Context[SHA_512_BLOCK_SIZE - 6] = (uint8_t) (TotalBits512[0] >> 40);
            Context[SHA_512_BLOCK_SIZE - 7] = (uint8_t) (TotalBits512[0] >> 48);
            Context[SHA_512_BLOCK_SIZE - 8] = (uint8_t) (TotalBits512[0] >> 56);
            
            Context[SHA_512_BLOCK_SIZE - 9] = (uint8_t)  TotalBits512[1];
            Context[SHA_512_BLOCK_SIZE - 10] = (uint8_t) (TotalBits512[1] >> 8);
            Context[SHA_512_BLOCK_SIZE - 11] = (uint8_t) (TotalBits512[1] >> 16);
            Context[SHA_512_BLOCK_SIZE - 12] = (uint8_t) (TotalBits512[1] >> 24);
            Context[SHA_512_BLOCK_SIZE - 13] = (uint8_t) (TotalBits512[1] >> 32);
            Context[SHA_512_BLOCK_SIZE - 14] = (uint8_t) (TotalBits512[1] >> 40);
            Context[SHA_512_BLOCK_SIZE - 15] = (uint8_t) (TotalBits512[1] >> 48);
            Context[SHA_512_BLOCK_SIZE - 16] = (uint8_t) (TotalBits512[1] >> 56); 
        }
        else
        {
            Context[ContextIndex++] = 0x80;
            memset(&Context[ContextIndex], 0, (SHA_512_BLOCK_SIZE - ContextIndex - 16));
            Context[SHA_512_BLOCK_SIZE - 1] = (uint8_t)  TotalBits512[0];
            Context[SHA_512_BLOCK_SIZE - 2] = (uint8_t) (TotalBits512[0] >> 8);
            Context[SHA_512_BLOCK_SIZE - 3] = (uint8_t) (TotalBits512[0] >> 16);
            Context[SHA_512_BLOCK_SIZE - 4] = (uint8_t) (TotalBits512[0] >> 24);
            Context[SHA_512_BLOCK_SIZE - 5] = (uint8_t) (TotalBits512[0] >> 32);
            Context[SHA_512_BLOCK_SIZE - 6] = (uint8_t) (TotalBits512[0] >> 40);
            Context[SHA_512_BLOCK_SIZE - 7] = (uint8_t) (TotalBits512[0] >> 48);
            Context[SHA_512_BLOCK_SIZE - 8] = (uint8_t) (TotalBits512[0] >> 56);
            
            Context[SHA_512_BLOCK_SIZE - 9] = (uint8_t)  TotalBits512[1];
            Context[SHA_512_BLOCK_SIZE - 10] = (uint8_t) (TotalBits512[1] >> 8);
            Context[SHA_512_BLOCK_SIZE - 11] = (uint8_t) (TotalBits512[1] >> 16);
            Context[SHA_512_BLOCK_SIZE - 12] = (uint8_t) (TotalBits512[1] >> 24);
            Context[SHA_512_BLOCK_SIZE - 13] = (uint8_t) (TotalBits512[1] >> 32);
            Context[SHA_512_BLOCK_SIZE - 14] = (uint8_t) (TotalBits512[1] >> 40);
            Context[SHA_512_BLOCK_SIZE - 15] = (uint8_t) (TotalBits512[1] >> 48);
            Context[SHA_512_BLOCK_SIZE - 16] = (uint8_t) (TotalBits512[1] >> 56); 
        }
        
        DataIn = (uint32_t *)Context;
        for(uint32_t i=0; i<16;i++)
        {
            SEC_SHA->DATA_1[i] = DataIn[i];
            SEC_SHA->DATA_2[i] = DataIn[i + 16];
        }
        /*start to calculate the last package data*/
        SEC_SHA->SHA_CTRL.Bits.CALCULATE = 1;
        while(SEC_SHA->SHA_INT_STATE.Bits.INT_DONE == 0);
        SEC_SHA->SHA_INT_STATE.Bits.INT_DONE = 0;
    }
    else /*sha256 or sha1 or sha224*/
    {
        if(TotalBits256 == 0) return;
        uint32_t LeftBits = (ContextIndex << 3);
        if(LeftBits == 0)
        {
            Context[0] = 0x80;
            memset(&Context[1], 0, (SHA_256_BLOCK_SIZE - 1 - 8));
            Context[SHA_256_BLOCK_SIZE - 1] = (uint8_t)  TotalBits256;
            Context[SHA_256_BLOCK_SIZE - 2] = (uint8_t) (TotalBits256 >> 8);
            Context[SHA_256_BLOCK_SIZE - 3] = (uint8_t) (TotalBits256 >> 16);
            Context[SHA_256_BLOCK_SIZE - 4] = (uint8_t) (TotalBits256 >> 24);
            Context[SHA_256_BLOCK_SIZE - 5] = (uint8_t) (TotalBits256 >> 32);
            Context[SHA_256_BLOCK_SIZE - 6] = (uint8_t) (TotalBits256 >> 40);
            Context[SHA_256_BLOCK_SIZE - 7] = (uint8_t) (TotalBits256 >> 48);
            Context[SHA_256_BLOCK_SIZE - 8] = (uint8_t) (TotalBits256 >> 56);
        }
        else if(LeftBits >= 448)
        {
            Context[ContextIndex++] = 0x80;
            memset(&Context[ContextIndex], 0, (SHA_256_BLOCK_SIZE - ContextIndex));
            
            DataIn = (uint32_t *)Context;
            for(uint32_t i=0; i<16;i++)
            {
                SEC_SHA->DATA_1[i] = DataIn[i];
            }
            
            SEC_SHA->SHA_CTRL.Bits.CALCULATE = 1;
            while(SEC_SHA->SHA_INT_STATE.Bits.INT_DONE == 0);
            SEC_SHA->SHA_INT_STATE.Bits.INT_DONE = 0;
            
            /*one more need to process*/
            memset(&Context[0], 0, (SHA_256_BLOCK_SIZE - 8));
            Context[SHA_256_BLOCK_SIZE - 1] = (uint8_t)  TotalBits256;
            Context[SHA_256_BLOCK_SIZE - 2] = (uint8_t) (TotalBits256 >> 8);
            Context[SHA_256_BLOCK_SIZE - 3] = (uint8_t) (TotalBits256 >> 16);
            Context[SHA_256_BLOCK_SIZE - 4] = (uint8_t) (TotalBits256 >> 24);
            Context[SHA_256_BLOCK_SIZE - 5] = (uint8_t) (TotalBits256 >> 32);
            Context[SHA_256_BLOCK_SIZE - 6] = (uint8_t) (TotalBits256 >> 40);
            Context[SHA_256_BLOCK_SIZE - 7] = (uint8_t) (TotalBits256 >> 48);
            Context[SHA_256_BLOCK_SIZE - 8] = (uint8_t) (TotalBits256 >> 56);
        }
        else
        {
            Context[ContextIndex++] = 0x80;
            memset(&Context[ContextIndex], 0, (SHA_256_BLOCK_SIZE - ContextIndex - 8));
            Context[SHA_256_BLOCK_SIZE - 1] = (uint8_t)  TotalBits256;
            Context[SHA_256_BLOCK_SIZE - 2] = (uint8_t) (TotalBits256 >> 8);
            Context[SHA_256_BLOCK_SIZE - 3] = (uint8_t) (TotalBits256 >> 16);
            Context[SHA_256_BLOCK_SIZE - 4] = (uint8_t) (TotalBits256 >> 24);
            Context[SHA_256_BLOCK_SIZE - 5] = (uint8_t) (TotalBits256 >> 32);
            Context[SHA_256_BLOCK_SIZE - 6] = (uint8_t) (TotalBits256 >> 40);
            Context[SHA_256_BLOCK_SIZE - 7] = (uint8_t) (TotalBits256 >> 48);
            Context[SHA_256_BLOCK_SIZE - 8] = (uint8_t) (TotalBits256 >> 56);
        }
        
        DataIn = (uint32_t *)Context;
        for(uint32_t i=0; i<16;i++)
        {
            SEC_SHA->DATA_1[i] = DataIn[i];
        }
        
        /*start to calculate the last package data*/
        SEC_SHA->SHA_CTRL.Bits.CALCULATE = 1;
        while(SEC_SHA->SHA_INT_STATE.Bits.INT_DONE == 0);
        SEC_SHA->SHA_INT_STATE.Bits.INT_DONE = 0;
    }
    uint32_t LastDigest[16];
    memset(&LastDigest, 0, 64);
    /*data out*/
    if(SEC_SHA->SHA_CTRL.Bits.MODE >= SHA_512)
    {
        for(uint32_t i=0;i<8;i++)
        {
            LastDigest[2*i] = SEC_SHA->HASH_VAL_H[i];
            LastDigest[2*i + 1] = SEC_SHA->HASH_VAL_L[i];
            *(DataOut++) = (uint8_t)(LastDigest[2*i] >> 24);
            *(DataOut++) = (uint8_t)(LastDigest[2*i] >> 16);
            *(DataOut++) = (uint8_t)(LastDigest[2*i] >> 8);
            *(DataOut++) = (uint8_t)(LastDigest[2*i]);
            *(DataOut++) = (uint8_t)(LastDigest[2*i + 1] >> 24);
            *(DataOut++) = (uint8_t)(LastDigest[2*i + 1] >> 16);
            *(DataOut++) = (uint8_t)(LastDigest[2*i + 1] >> 8);
            *(DataOut++) = (uint8_t)(LastDigest[2*i + 1]);
        }
    }
    else
    {
        for(uint32_t i=0;i<8;i++)
        {
            LastDigest[i] = SEC_SHA->HASH_VAL_L[i];
            *(DataOut++) = (uint8_t)(LastDigest[i] >> 24);
            *(DataOut++) = (uint8_t)(LastDigest[i] >> 16);
            *(DataOut++) = (uint8_t)(LastDigest[i] >> 8);
            *(DataOut++) = (uint8_t)(LastDigest[i]);
        }
    }
}

static void __sha_updata_256(uint8_t *fp8_Data, uint32_t fu32_Size)
{
    /*total bits add*/
    TotalBits256 += (fu32_Size << 3);
    while(fu32_Size-- > 0)
    {
        Context[ContextIndex++] = *(fp8_Data++);
        if(ContextIndex >= SHA_256_BLOCK_SIZE)
        {
            ContextIndex = 0;
            uint32_t *DataIn = (uint32_t *)Context;
            for(uint32_t i=0; i<16;i++)
            {
                SEC_SHA->DATA_1[i] = DataIn[i];
            }
            SEC_SHA->SHA_CTRL.Bits.CALCULATE = 1;
            while(SEC_SHA->SHA_INT_STATE.Bits.INT_DONE == 0);
            SEC_SHA->SHA_INT_STATE.Bits.INT_DONE = 0;
        }
    }
}

static void __sha_updata_512(uint8_t *fp8_Data, uint32_t fu32_Size)
{
    TotalBits512[0] += (fu32_Size << 3);
    if(TotalBits512[0] < (uint64_t)(fu32_Size << 3) ) {
        TotalBits512[1]++;
    }
    while(fu32_Size-- > 0)
    {
        Context[ContextIndex++] = *(fp8_Data++);
        if(ContextIndex >= SHA_512_BLOCK_SIZE)
        {
            ContextIndex = 0;
            uint32_t *DataIn = (uint32_t *)Context;
            for(uint32_t i=0; i<16;i++)
            {
                SEC_SHA->DATA_1[i] = DataIn[i];
                SEC_SHA->DATA_2[i] = DataIn[i + 16];
            }
            SEC_SHA->SHA_CTRL.Bits.CALCULATE = 1;
            while(SEC_SHA->SHA_INT_STATE.Bits.INT_DONE == 0);
            SEC_SHA->SHA_INT_STATE.Bits.INT_DONE = 0;
        }
    }
}