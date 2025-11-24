/*
  ******************************************************************************
  * @file    driver_efuse.c
  * @author  FreqChip Firmware Team
  * @version V1.0.0
  * @date    2021
  * @brief   eFuse module driver.
  *          This file provides firmware functions to manage the eFuse.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 FreqChip.
  * All rights reserved.
  ******************************************************************************
*/
#include "fr30xx.h"

/************************************************************************************
 * @fn      eFuse_siso_read
 *
 * @brief   read eFuse data.
 *
 * @param   fp_Data: read data buffer.
 */
void eFuse_siso_read(uint32_t *fp_Data)
{ 
    /* config read mode */
    EFUSE_SISO->eFuse_Ctrl = EFUSE_SISO_READ_MODE;
    /* wait config done */
    while(!(EFUSE_SISO->eFuse_Ctrl & EFUSE_SISO_CHECK_DONE));
    EFUSE_SISO->eFuse_Ctrl |= EFUSE_SISO_CHECK_DONE;

    /* read data */
    fp_Data[0] = EFUSE_SISO->eFuse_Data0;
    fp_Data[1] = EFUSE_SISO->eFuse_Data1;
    fp_Data[2] = EFUSE_SISO->eFuse_Data2;
}

/************************************************************************************
 * @fn      eFuse_siso_write
 *
 * @brief   write eFuse data.
 *
 * @param   fp_Data: write data buffer.
 */
void eFuse_siso_write(uint32_t *fp_Data)
{
    /* write data */
    EFUSE_SISO->eFuse_Data0 = fp_Data[0];
    EFUSE_SISO->eFuse_Data1 = fp_Data[1];
    EFUSE_SISO->eFuse_Data2 = fp_Data[2];
    
    /* config write mode */
    EFUSE_SISO->eFuse_Ctrl = EFUSE_SISO_WRITE_MODE;
    /* wait config done */
    while(!(EFUSE_SISO->eFuse_Ctrl & EFUSE_SISO_CHECK_DONE));
    EFUSE_SISO->eFuse_Ctrl |= EFUSE_SISO_CHECK_DONE;
}

/************************************************************************************
 * @fn      eFuse_pipo_read
 *
 * @brief   read eFuse data.
 *
 * @param   fu8_Addr: efuse address, unit byte, range 0x00 ~ 0xFF;
 * @param   fp_Data: read data buffer.
 */
void eFuse_pipo_read(uint8_t fu8_Addr, uint8_t *fp_Data)
{ 
    EFUSE_PIPO->eFuse_Addr = fu8_Addr;

    /* config read mode */
    EFUSE_PIPO->eFuse_Ctrl = EFUSE_PIPO_GO | EFUSE_PIPO_READ_MODE;
    /* wait config done */
    while(!(EFUSE_PIPO->eFuse_Ctrl & EFUSE_PIPO_CHECK_DONE));
    EFUSE_PIPO->eFuse_Ctrl |= EFUSE_PIPO_WRITE_MODE;
    EFUSE_PIPO->eFuse_Ctrl = 0x00;

    fp_Data[0] = EFUSE_PIPO->eFuse_RData;

    EFUSE_PIPO->eFuse_Ctrl &= ~EFUSE_PIPO_READ_MODE;
}

/************************************************************************************
 * @fn      eFuse_pipo_write
 *
 * @brief   write eFuse data.
 *
 * @param   fu8_Addr: efuse address, unit byte, range 0x00 ~ 0xFF;
 * @param   fp_Data: write data value.
 */
void eFuse_pipo_write(uint8_t fu8_Addr, uint8_t fu8_Data)
{ 
    EFUSE_PIPO->eFuse_Ctrl = EFUSE_PIPO_AVDDEN;

    EFUSE_PIPO->eFuse_Addr  = fu8_Addr;
    EFUSE_PIPO->eFuse_WData = fu8_Data;
    
    /* config write mode */
    EFUSE_PIPO->eFuse_Ctrl = EFUSE_PIPO_GO | EFUSE_PIPO_WRITE_MODE | EFUSE_PIPO_AVDDEN;
    /* wait config done */
    while(!(EFUSE_PIPO->eFuse_Ctrl & EFUSE_PIPO_CHECK_DONE));
    EFUSE_PIPO->eFuse_Ctrl |= EFUSE_PIPO_WRITE_MODE;
    EFUSE_PIPO->eFuse_Ctrl = 0x00;
}
