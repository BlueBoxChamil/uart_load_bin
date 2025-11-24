/*
  ******************************************************************************
  * @file    fr30xx.h
  * @author  FreqChip Firmware Team
  * @brief   CMSIS fr30xx Device Peripheral Access Layer Header File.
  *
  *          This file contains:
  *           - Data structures and the address mapping for all peripherals
  *           - Configuration of the Processor and Core Peripherals
  *
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 FreqChip.
  * All rights reserved.
  ******************************************************************************
*/
#ifndef __FR30XX_H__
#define __FR30XX_H__

#ifdef __cplusplus
extern "C"
{
#endif

//#define CHIP_SEL_FR3066D
//#define CHIP_SEL_FR3068E
// #define CHIP_SEL_FR3068E_V2
//#define CHIP_SEL_FR3092E
#define CHIP_SEL_FR3092E_V4
//#define CHIP_SEL_FR5092E
//#define CHIP_SEL_FR5098E
//#define CHIP_SEL_FR5099E

#if ((defined(CHIP_SEL_FR3066D) \
        + defined(CHIP_SEL_FR3068E) \
        + defined(CHIP_SEL_FR3068E_V2) \
        + defined(CHIP_SEL_FR3092E) \
        + defined(CHIP_SEL_FR3092E_V4) \
        + defined(CHIP_SEL_FR5092E)) \
        + defined(CHIP_SEL_FR5098E) \
        + defined(CHIP_SEL_FR5099E) \
        != 1)
#error "only one chip should be selected!"
#endif

/** @group Peripheral_interrupt_number_definition
  * @{
  */
#if defined(__ARMCC_VERSION) || defined(__GNUC__) || defined(__ICCARM__)
typedef enum IRQn
{
    /******  Cortex-M33 Processor Exceptions Numbers ************************************************/
    NonMaskableInt_IRQn           = -14,    /*!< 2 Non Maskable Interrupt                           */
    HardFault_IRQn                = -13,    /*!< 3 Cortex-M33 Hard Fault Interrupt                  */
    SVCall_IRQn                   = -5,     /*!< 11 Cortex-M33 SV Call Interrupt                    */
    PendSV_IRQn                   = -2,     /*!< 14 Cortex-M33 Pend SV Interrupt                    */
    SysTick_IRQn                  = -1,     /*!< 15 Cortex-M33 System Tick Interrupt                */
	
    /******  CMSDK Specific Interrupt Numbers *******************************************************/
    TIMER0_IRQn                     = 0,     /*!<   */
    TIMER1_IRQn                     = 1,     /*!<   */
    TIMER2_IRQn                     = 2,     /*!<   */
    TIMER3_IRQn                     = 3,     /*!<   */
    DMA0_IRQn                       = 4,
    DMA1_IRQn                       = 5,     /*!<   */
    SDIOH0_IRQn                     = 6,     /*!<   */
    SDIOH1_IRQn                     = 7,     /*!<   */
    IPC_MCU_IRQn                    = 8,     /*!<   */
    USBOTG_IRQn                     = 9,     /*!<   */
    IIR_IRQn                        = 10,    /*!<   */	
    BLEND_IRQn                      = 11,    /*!<   */
    FFT_IRQn                        = 12,    /*!<   */
    SEC_AES_IRQn                    = 13,    /*!<   */
    GPIOA_IRQn                      = 16,    /*!<   */
    GPIOB_IRQn                      = 17,    /*!<   */
    GPIOC_IRQn                      = 18,    /*!<   */
    GPIOD_IRQn                      = 19,    /*!<   */
    UART0_IRQn                      = 20,    /*!<   */
    UART1_IRQn                      = 21,    /*!<   */
    UART2_IRQn                      = 22,    /*!<   */
    UART3_IRQn                      = 23,    /*!<   */
    UART4_IRQn                      = 24,
    UART5_IRQn                      = 25,
    I2C0_IRQn                       = 26,
    I2C1_IRQn                       = 27,
    I2C2_IRQn                       = 28,
    I2C3_IRQn                       = 29,
    I2C4_IRQn                       = 30,
    I2C5_IRQn                       = 31,
    SPIM0_IRQn                      = 32,
    SPIM1_IRQn                      = 33,
    SPIM2_IRQn                      = 34,
    SPIS0_IRQn                      = 35,
    SPIS1_IRQn                      = 36,
    SPIMX8_0_IRQn                   = 37,
    SPIMX8_1_IRQn                   = 38,
    I2S0_IRQn                       = 39,
    I2S1_IRQn                       = 40,
    I2S2_IRQn                       = 41,
    PDM0_IRQn                       = 42,
    PDM1_IRQn                       = 43,
    PDM2_IRQn                       = 44,
    ADC_IRQn                        = 45,
    CODEC_IRQn                      = 46,
    SPDIF_IRQn                      = 47,
    SBCDEC_IRQn                     = 48, 
    SBCENC_IRQn                     = 49,
    MP3DEC_IRQn                     = 50,
    PARALLEL_IRQn                   = 51,
    CALI_IRQn                       = 53,
    TRNG_IRQn                       = 54,
    TICK_IRQn                       = 55,
    TIMER4_IRQn                     = 60,
    TIMER5_IRQn                     = 61,
    IPC_DSP_IRQn                    = 63,
    YUV2RGB_IRQn                    = 64,
    PMU_IRQn                        = 65,
}IRQn_Type;
#endif  // defined(__ARMCC_VERSION) || defined(__GNUC__) || defined(__ICCARM__)

#ifdef __XTENSA__
typedef enum IRQn
{
    DSP_IPC_IRQn                    = 7,
}IRQn_Type;
#endif  // __XTENSA__
/**
  * @}
  */

#if defined(__ARMCC_VERSION) || defined(__GNUC__) || defined(__ICCARM__)
/**
  * @brief Configuration of the Processor and Core Peripherals
  */
#define __CM33_REV                0x0003U   /*!< Core revision r0p4                               */
#define __SAUREGION_PRESENT       0         /*!< SAU regions present                              */
#define __MPU_PRESENT             1         /*!< MPU present                                      */
#define __VTOR_PRESENT            1         /*!< VTOR present                                     */
#define __FPU_PRESENT             1         /*!< FPU present or not                               */
#define __DSP_PRESENT             1         /*!< DSP present or not                               */
#define __NVIC_PRIO_BITS          3         /*!< Number of Bits used for Priority Levels          */
#define __Vendor_SysTickConfig    0         /*!< Set to 1 if different SysTick Config is used     */



#include "core_cm33.h"
#if __SAUREGION_PRESENT == 1
#include "arm_cmse.h"
#endif
#endif  // defined(__ARMCC_VERSION) || defined(__GNUC__) || defined(__ICCARM__)

#ifdef __XTENSA__
#define __WEAK              __attribute__((weak))
#define __STATIC_INLINE     __attribute__((always_inline))
#endif  // __XTENSA__

/** @group Peripheral_memory_map
  * @{
  */
#define FLASH_DAC_BASE       (0x08000000)  

#define DMAC0_BASE           (0x10000000)
#define USB_OTG_BASE         (0x10010000)
#define APB_BASE             (0x10100000)
#define SBC_DEC_BASE         (0x10100000)
#define SBC_ENC_BASE         (0x10110000)
#define MP3_DEC_BASE         (0x10120000)
#define CRC_BASE             (0x10130000)
#define EFUSE_SISO_BASE      (0x10140000)
#define EFUSE_PIPO_BASE      (0x10150000)
#define SYSTEM_TIMER_BASE    (0x10160000)
#define FREE_COUNTER_BASE    (0x10170000)
#define CAN0_BASE            (0x10180000)
#define CAN1_BASE            (0x10190000)
#define SEC_BASE             (0x11000000)

#define DSP_FLASH_DAC_BASE   (0x28000000)

#define PSRAM_DAC_BASE       (0x38000000)
#define SDIOH0_BASE          (0x40000000)
#define SDIOH1_BASE          (0x40010000)
#define DMAC1_BASE           (0x40020000)
#define BLEND_AHB0_BASE      (0x40030000)
#define BLEND_AHB1_BASE      (0x40040000)

#define GPIOA_BASE           (0x50000000)
#define GPIOB_BASE           (0x50008000)
#define UART0_BASE           (0x50010000)
#define UART1_BASE           (0x50018000)
#define I2C0_BASE            (0x50020000)
#define I2C1_BASE            (0x50028000)
#define SPIM0_BASE           (0x50030000)
#define SPIS0_BASE           (0x50040000)
#define PWM0_BASE            (0x50050000)
#define I2S0_BASE            (0x50060000)
#define PDM0_BASE            (0x50070000)
#define IIR_BASE             (0x50080000)
#define TRI_FUNC_BASE        (0x50090000)
#define FFT_BASE             (0x500A0000)
#define AHBC_CACHE_BASE      (0x500B0000)
#define PSRAM_OSPI_BASE      (0x500B8000)
#define SPIMX8_0_BASE        (0x500C0000)
#define PARALLEL_BASE        (0x500D0000)

#define GPIOC_BASE           (0x50100000)
#define GPIOD_BASE           (0x50108000)
#define UART2_BASE           (0x50110000)
#define UART3_BASE           (0x50118000)
#define I2C2_BASE            (0x50120000)
#define I2C3_BASE            (0x50128000)
#define SPIM1_BASE           (0x50130000)
#define SPIS1_BASE           (0x50140000)
#define PWM1_BASE            (0x50150000)
#define I2S1_BASE            (0x50160000)
#define PDM1_BASE            (0x50170000)
#define SPDIF_BASE           (0x50180000)
#define CODEC_BASE           (0x50190000)
#define SPIMX8_1_BASE        (0x501C0000)

#define DSP_CTRL_BASE        (0x50200000)
#define UART4_BASE           (0x50210000)
#define UART5_BASE           (0x50218000)
#define I2C4_BASE            (0x50220000)
#define I2C5_BASE            (0x50228000)
#define SPIM2_BASE           (0x50230000)
#define DSP_TIM0_BASE        (0x50240000)
#define DSP_TIM1_BASE        (0x50240014)
#define DSP_WDT_BASE         (0x50250000)
#define I2S2_BASE            (0x50260000)
#define PDM2_BASE            (0x50270000)
#define DSP_IPC_BASE         (0x50280000)
#define DSP_QSPI_BASE        (0x50290000)
#define YUV2RGB_BASE         (0x502A0000)

#define SYSTEM_REG_BASE      (0xE0050000)
#define TIM0_BASE            (0xE0060000)
#define TIM1_BASE            (0xE0060014)
#define TIM2_BASE            (0xE0068000)
#define TIM3_BASE            (0xE0068014)
#define FRSPIM_BASE          (0xE0080000)
#define IPC_BASE             (0xE0090000)
#define CALIB_BASE           (0xE00A0000)
#define FLASH_CACHE_BASE     (0xE00B0000)
#define FLASH_QSPI_BASE      (0xE00C0000)
#define TRNG_BASE            (0xE00D0000)
#define ADC_BASE             (0xE00E0000)
#define SARADC_BASE          (0xE00F0000)


/**
  * @}
  */

/* ########################## Oscillator Values adaptation ####################*/
/**
  * @brief Adjust the value of External High Speed oscillator (HSE) used in your application.
  *        This value is used by the system clock calculation.
  */
#define HSE_VALUE    24000000U /*!< Value of the External oscillator in Hz */

/**
  * @brief Internal High Speed oscillator (HSI) value.
  *        This value is used by the system clock calculation.
  */
#define HSI_VALUE    24000000U /*!< Value of the Internal oscillator in Hz */

/* Peripheral drive */
#include "driver_common.h"
/* System driver    */
#include "system_fr30xx.h"
/* trim relative */
#include "trim_fr30xx.h"

#ifdef __cplusplus
}
#endif

#endif  // __FR30XX_H__
