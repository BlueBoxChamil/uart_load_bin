;************************* (C) COPYRIGHT 2023 FreqChip ***************************
;* File Name          : startup_fr30xx.s
;* Author             : FreqChip Firmware Team
;* Version            : V1.0.0
;* Date               : 2022
;* Description        : fr30xx Devices vector table for IAR toolchain. 
;*                      This module performs:
;*                      - Set the initial SP
;*                      - Set the initial PC == Reset_Handler
;*                      - Set the vector table entries with the exceptions ISR address
;*                      - Configure the clock system
;*                      - Branches to __main in the C library (which eventually
;*                        calls main()).
;*                      After Reset the Cortex-M33 processor is in Thread mode,
;*                      priority is Privileged, and the Stack is set to Main.
;*********************************************************************************
;* @attention
;*
;* Copyright (c) 2022 FreqChip.
;* All rights reserved.
;*******************************************************************************
;
; The modules in this file are included in the libraries, and may be replaced
; by any user-defined modules that define the PUBLIC symbol _program_start or
; a user defined start symbol.
; To override the cstartup defined in the library, simply add your modified
; version to the workbench project.
;
; The vector table is normally located at address 0.
; When debugging in RAM, it can be located in RAM, aligned to at least 2^6.
; The name "__vector_table" has special meaning for C-SPY:
; it is where the SP start value is found, and the NVIC vector
; table register (VTOR) is initialized to this address if != 0.
;
; Cortex-M version
;

        MODULE  ?cstartup

        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:NOROOT(2)

        EXTERN  __iar_program_start
        EXTERN  SystemInit
        PUBLIC  __vector_table
        PUBLIC  __Vectors
        PUBLIC  __Vectors_End
        PUBLIC  __Vectors_Size

        DATA
__vector_table
        DCD     sfe(CSTACK)
        DCD     Reset_Handler              ; Reset Handler

        DCD     NMI_Handler                ; NMI Handler
        DCD     HardFault_Handler          ; Hard Fault Handler
        DCD     MemManage_Handler          ; MPU Fault Handler
        DCD     BusFault_Handler           ; Bus Fault Handler
        DCD     UsageFault_Handler         ; Usage Fault Handler
        DCD     0                          ; Secure Fault Handler
        DCD     0                          ; Reserved
        DCD     0                          ; Reserved
        DCD     0                          ; Reserved
        DCD     SVC_Handler                ; SVCall Handler
        DCD     DebugMon_Handler           ; Debug Monitor Handler
        DCD     0                          ; Reserved
        DCD     PendSV_Handler             ; PendSV Handler
        DCD     SysTick_Handler            ; SysTick Handler

         ; External Interrupts
        DCD      timer0_irq                          ;   0 Interrupt 0
        DCD      timer1_irq                          ;   1 Interrupt 1
        DCD      timer2_irq                          ;   2 timer2
        DCD      timer3_irq                          ;   3 timer3
        DCD      dma0_irq                            ;   4 dma0
        DCD      dma1_irq                            ;   5 dma1
        DCD      sdioh0_irq                          ;   6 sdioh
        DCD      sdioh1_irq                          ;   7 sdiod
        DCD      ipc_mcu_irq                         ;   8 ipc mcu
        DCD      usbotg_irq                          ;   9 usbotg
        DCD      iir_irq                             ;   10 iir
        DCD      blend_irq                           ;   11 trigfunc
        DCD      fft_irq                             ;   12 fft
        DCD      sec_aes_irq                         ;   13 Interrupt 13
        DCD      Interrupt14_Handler                 ;   14 Interrupt 14
        DCD      Interrupt15_Handler                 ;   15 Interrupt 15
        DCD      gpioa_irq                           ;   16 GPIOA
        DCD      gpiob_irq                           ;   17 GPIOB
        DCD      gpioc_irq                           ;   18 GPIOC
        DCD      gpiod_irq                           ;   19 GPIOD
        DCD      uart0_irq                           ;   20 uart0
        DCD      uart1_irq                           ;   21 uart1
        DCD      uart2_irq                           ;   22 uart2
        DCD      uart3_irq                           ;   23 uart3
        DCD      uart4_irq                           ;   24 uart4
        DCD      uart5_irq                           ;   25 uart5
        DCD      i2c0_irq                            ;   26 i2c0
        DCD      i2c1_irq                            ;   27 i2c1
        DCD      i2c2_irq                            ;   28 i2c2
        DCD      i2c3_irq                            ;   29 i2c3
        DCD      i2c4_irq                            ;   30 i2c4
        DCD      i2c5_irq                            ;   31 i2c5
        DCD      spim0_irq                           ;   32 spim0
        DCD      spim1_irq                           ;   33 spim1
        DCD      spim2_irq                           ;   34 spim2
        DCD      spis0_irq                           ;   35 spis0
        DCD      spis1_irq                           ;   36 spis1
        DCD      spimx8_0_irq                        ;   37 spimx8_0
        DCD      spimx8_1_irq                        ;   38 spimx8_1
        DCD      i2s0_irq                            ;   39 i2s0
        DCD      i2s1_irq                            ;   40 i2s1
        DCD      i2s2_irq                            ;   41 i2s2
        DCD      pdm0_irq                            ;   42 pdm0
        DCD      pdm1_irq                            ;   43 pdm1
        DCD      pdm2_irq                            ;   44 pdm2
        DCD      adc_irq                             ;   45 adc
        DCD      codec_irq                           ;   46 codec
        DCD      spdif_irq                           ;   47 spdif
        DCD      sbc_dec_irq                         ;   48 sbc_dec
        DCD      sbc_enc_irq                         ;   49 sbc_enc
        DCD      mp3dec_irq                          ;   50 mp3dec
        DCD      parallel0_irq                       ;   51 parallel0
        DCD      Interrupt52_Handler                 ;   52 Interrupt 52
        DCD      cali_irq                            ;   53 cali
        DCD      trng_irq                            ;   54 trng
        DCD      tick_irq                            ;   55 Interrupt 55
        DCD      Interrupt56_Handler                 ;   56 Interrupt 56		
        DCD      Interrupt57_Handler                 ;   57 Interrupt 57	
        DCD      Interrupt58_Handler                 ;   58 Interrupt 58
        DCD      Interrupt59_Handler                 ;   59 Interrupt 59
        DCD      timer4_irq                          ;   60 timer4
        DCD      timer5_irq                          ;   61 timer5
        DCD      Interrupt62_Handler                 ;   62 Interrupt 62
        DCD      ipc_dsp_irq                         ;   63 Interrupt 63
        DCD      yuv2rgb_irq                         ;   64 yuv2rgb
        DCD      pmu_irq                             ;   65 pmu
        DCD      0xAA55AA55                          ;   app check data
        DCD      0x00000001                          ;   app version
        DCD      0                                   ;   code length
__Vectors_End

__Vectors       EQU   __vector_table
__Vectors_Size  EQU   __Vectors_End - __Vectors

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;
        THUMB
        PUBWEAK Reset_Handler
        SECTION .text:CODE:NOROOT:REORDER(2)
Reset_Handler
        LDR     R0, =SystemInit
        BLX     R0
        LDR     R0, =__iar_program_start
        BX      R0

        PUBWEAK NMI_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
NMI_Handler
        B NMI_Handler

        PUBWEAK HardFault_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
HardFault_Handler
        B HardFault_Handler

        PUBWEAK MemManage_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
MemManage_Handler
        B MemManage_Handler

        PUBWEAK BusFault_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
BusFault_Handler
        B BusFault_Handler

        PUBWEAK UsageFault_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
UsageFault_Handler
        B UsageFault_Handler

        PUBWEAK SVC_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
SVC_Handler
        B SVC_Handler

        PUBWEAK DebugMon_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
DebugMon_Handler
        B DebugMon_Handler

        PUBWEAK PendSV_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
PendSV_Handler
        B PendSV_Handler

        PUBWEAK SysTick_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
SysTick_Handler
        B SysTick_Handler

        PUBWEAK timer0_irq
        SECTION .text:CODE:NOROOT:REORDER(1)
timer0_irq
        B timer0_irq

        PUBWEAK timer1_irq
        SECTION .text:CODE:NOROOT:REORDER(1)
timer1_irq
        B timer1_irq

        PUBWEAK timer2_irq
        SECTION .text:CODE:NOROOT:REORDER(1)
timer2_irq
        B timer2_irq

        PUBWEAK timer3_irq
        SECTION .text:CODE:NOROOT:REORDER(1)
timer3_irq
        B timer3_irq

        PUBWEAK dma0_irq
        SECTION .text:CODE:NOROOT:REORDER(1)
dma0_irq
        B dma0_irq

        PUBWEAK dma1_irq
        SECTION .text:CODE:NOROOT:REORDER(1)
dma1_irq
        B dma1_irq

        PUBWEAK sdioh0_irq
        SECTION .text:CODE:NOROOT:REORDER(1)
sdioh0_irq
        B sdioh0_irq

        PUBWEAK sdioh1_irq
        SECTION .text:CODE:NOROOT:REORDER(1)
sdioh1_irq
        B sdioh1_irq

        PUBWEAK ipc_mcu_irq
        SECTION .text:CODE:NOROOT:REORDER(1)
ipc_mcu_irq
        B ipc_mcu_irq

        PUBWEAK usbotg_irq
        SECTION .text:CODE:NOROOT:REORDER(1)
usbotg_irq
        B usbotg_irq

        PUBWEAK iir_irq
        SECTION .text:CODE:NOROOT:REORDER(1)
iir_irq
        B iir_irq

        PUBWEAK blend_irq
        SECTION .text:CODE:NOROOT:REORDER(1)
blend_irq
        B blend_irq

        PUBWEAK fft_irq
        SECTION .text:CODE:NOROOT:REORDER(1)
fft_irq
        B fft_irq

        PUBWEAK sec_aes_irq
        SECTION .text:CODE:NOROOT:REORDER(1)
sec_aes_irq
        B sec_aes_irq

        PUBWEAK Interrupt14_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
Interrupt14_Handler
        B Interrupt14_Handler

        PUBWEAK Interrupt15_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
Interrupt15_Handler
        B Interrupt15_Handler

        PUBWEAK gpioa_irq
        SECTION .text:CODE:NOROOT:REORDER(1)
gpioa_irq
        B gpioa_irq

        PUBWEAK gpiob_irq
        SECTION .text:CODE:NOROOT:REORDER(1)
gpiob_irq
        B gpiob_irq

        PUBWEAK gpioc_irq
        SECTION .text:CODE:NOROOT:REORDER(1)
gpioc_irq
        B gpioc_irq

        PUBWEAK gpiod_irq
        SECTION .text:CODE:NOROOT:REORDER(1)
gpiod_irq
        B gpiod_irq

        PUBWEAK uart0_irq
        SECTION .text:CODE:NOROOT:REORDER(1)
uart0_irq
        B uart0_irq

        PUBWEAK uart1_irq
        SECTION .text:CODE:NOROOT:REORDER(1)
uart1_irq
        B uart1_irq

        PUBWEAK uart2_irq
        SECTION .text:CODE:NOROOT:REORDER(1)
uart2_irq
        B uart2_irq

        PUBWEAK uart3_irq
        SECTION .text:CODE:NOROOT:REORDER(1)
uart3_irq
        B uart3_irq

        PUBWEAK uart4_irq
        SECTION .text:CODE:NOROOT:REORDER(1)
uart4_irq
        B uart4_irq

        PUBWEAK uart5_irq
        SECTION .text:CODE:NOROOT:REORDER(1)
uart5_irq
        B uart5_irq

        PUBWEAK i2c0_irq
        SECTION .text:CODE:NOROOT:REORDER(1)
i2c0_irq
        B i2c0_irq

        PUBWEAK i2c1_irq
        SECTION .text:CODE:NOROOT:REORDER(1)
i2c1_irq
        B i2c1_irq

        PUBWEAK i2c2_irq
        SECTION .text:CODE:NOROOT:REORDER(1)
i2c2_irq
        B i2c2_irq

        PUBWEAK i2c3_irq
        SECTION .text:CODE:NOROOT:REORDER(1)
i2c3_irq
        B i2c3_irq

        PUBWEAK i2c4_irq
        SECTION .text:CODE:NOROOT:REORDER(1)
i2c4_irq
        B i2c4_irq

        PUBWEAK i2c5_irq
        SECTION .text:CODE:NOROOT:REORDER(1)
i2c5_irq
        B i2c5_irq

        PUBWEAK spim0_irq
        SECTION .text:CODE:NOROOT:REORDER(1)
spim0_irq
        B spim0_irq

        PUBWEAK spim1_irq
        SECTION .text:CODE:NOROOT:REORDER(1)
spim1_irq
        B spim1_irq

        PUBWEAK spim2_irq
        SECTION .text:CODE:NOROOT:REORDER(1)
spim2_irq
        B spim2_irq

        PUBWEAK spis0_irq
        SECTION .text:CODE:NOROOT:REORDER(1)
spis0_irq
        B spis0_irq

        PUBWEAK spis1_irq
        SECTION .text:CODE:NOROOT:REORDER(1)
spis1_irq
        B spis1_irq

        PUBWEAK spimx8_0_irq
        SECTION .text:CODE:NOROOT:REORDER(1)
spimx8_0_irq
        B spimx8_0_irq

        PUBWEAK spimx8_1_irq
        SECTION .text:CODE:NOROOT:REORDER(1)
spimx8_1_irq
        B spimx8_1_irq

        PUBWEAK i2s0_irq
        SECTION .text:CODE:NOROOT:REORDER(1)
i2s0_irq
        B i2s0_irq

        PUBWEAK i2s1_irq
        SECTION .text:CODE:NOROOT:REORDER(1)
i2s1_irq
        B i2s1_irq

        PUBWEAK i2s2_irq
        SECTION .text:CODE:NOROOT:REORDER(1)
i2s2_irq
        B i2s2_irq

        PUBWEAK pdm0_irq
        SECTION .text:CODE:NOROOT:REORDER(1)
pdm0_irq
        B pdm0_irq

        PUBWEAK pdm1_irq
        SECTION .text:CODE:NOROOT:REORDER(1)
pdm1_irq
        B pdm1_irq

        PUBWEAK pdm2_irq
        SECTION .text:CODE:NOROOT:REORDER(1)
pdm2_irq
        B pdm2_irq

        PUBWEAK adc_irq
        SECTION .text:CODE:NOROOT:REORDER(1)
adc_irq
        B adc_irq

        PUBWEAK codec_irq
        SECTION .text:CODE:NOROOT:REORDER(1)
codec_irq
        B codec_irq

        PUBWEAK spdif_irq
        SECTION .text:CODE:NOROOT:REORDER(1)
spdif_irq
        B spdif_irq

        PUBWEAK sbc_dec_irq
        SECTION .text:CODE:NOROOT:REORDER(1)
sbc_dec_irq
        B sbc_dec_irq

        PUBWEAK sbc_enc_irq
        SECTION .text:CODE:NOROOT:REORDER(1)
sbc_enc_irq
        B sbc_enc_irq

        PUBWEAK mp3dec_irq
        SECTION .text:CODE:NOROOT:REORDER(1)
mp3dec_irq
        B mp3dec_irq

        PUBWEAK parallel0_irq
        SECTION .text:CODE:NOROOT:REORDER(1)
parallel0_irq
        B parallel0_irq

        PUBWEAK Interrupt52_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
Interrupt52_Handler
        B Interrupt52_Handler

        PUBWEAK cali_irq
        SECTION .text:CODE:NOROOT:REORDER(1)
cali_irq
        B cali_irq

        PUBWEAK trng_irq
        SECTION .text:CODE:NOROOT:REORDER(1)
trng_irq
        B trng_irq

        PUBWEAK tick_irq
        SECTION .text:CODE:NOROOT:REORDER(1)
tick_irq
        B tick_irq

        PUBWEAK Interrupt56_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
Interrupt56_Handler
        B Interrupt56_Handler

        PUBWEAK Interrupt57_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
Interrupt57_Handler
        B Interrupt57_Handler

        PUBWEAK Interrupt58_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
Interrupt58_Handler
        B Interrupt58_Handler

        PUBWEAK Interrupt59_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
Interrupt59_Handler
        B Interrupt59_Handler

        PUBWEAK timer4_irq
        SECTION .text:CODE:NOROOT:REORDER(1)
timer4_irq
        B timer4_irq

        PUBWEAK timer5_irq
        SECTION .text:CODE:NOROOT:REORDER(1)
timer5_irq
        B timer5_irq

        PUBWEAK Interrupt62_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
Interrupt62_Handler
        B Interrupt62_Handler

        PUBWEAK ipc_dsp_irq
        SECTION .text:CODE:NOROOT:REORDER(1)
ipc_dsp_irq
        B ipc_dsp_irq

        PUBWEAK yuv2rgb_irq
        SECTION .text:CODE:NOROOT:REORDER(1)
yuv2rgb_irq
        B yuv2rgb_irq

        PUBWEAK pmu_irq
        SECTION .text:CODE:NOROOT:REORDER(1)
pmu_irq
        B pmu_irq

        END
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
