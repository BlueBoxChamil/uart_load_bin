;************************* (C) COPYRIGHT 2023 FreqChip ***************************
;* File Name          : startup_fr30xx.s
;* Author             : FreqChip Firmware Team
;* Version            : V1.0.0
;* Date               : 2022
;* Description        : fr30xx Devices vector table for MDK-ARM toolchain. 
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

;//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------

;<h> Stack Configuration
;  <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
;</h>

Stack_Size      EQU      0x00001000

                AREA     STACK, NOINIT, READWRITE, ALIGN=3
__stack_limit
Stack_Mem       SPACE    Stack_Size
__initial_sp


;<h> Heap Configuration
;  <o> Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
;</h>

Heap_Size       EQU      0x00019000

                IF       Heap_Size != 0                      ; Heap is provided
                AREA     HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE    Heap_Size
__heap_limit
                ENDIF


                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset

                AREA     RESET, DATA, READONLY
                EXPORT   __Vectors
                EXPORT   __Vectors_End
                EXPORT   __Vectors_Size

__Vectors       DCD      __initial_sp                        ;     Top of Stack
                DCD      Reset_Handler                       ;     Reset Handler
                DCD      NMI_Handler                         ; -14 NMI Handler
                DCD      HardFault_Handler                   ; -13 Hard Fault Handler
                DCD      MemManage_Handler                   ; -12 MPU Fault Handler
                DCD      BusFault_Handler                    ; -11 Bus Fault Handler
                DCD      UsageFault_Handler                  ; -10 Usage Fault Handler
                DCD      SecureFault_Handler                 ;  -9 Secure Fault Handler
                DCD      0                                   ;     Reserved
                DCD      0                                   ;     Reserved
                DCD      0                                   ;     Reserved
                DCD      SVC_Handler                         ;  -5 SVCall Handler
                DCD      DebugMon_Handler                    ;  -4 Debug Monitor Handler
                DCD      0                                   ;     Reserved
                DCD      PendSV_Handler                      ;  -2 PendSV Handler
                DCD      SysTick_Handler                     ;  -1 SysTick Handler

                ; Interrupts
                DCD      timer0_irq                          ;   0 timer0
                DCD      timer1_irq                          ;   1 timer1
                DCD      timer2_irq                          ;   2 timer2
                DCD      timer3_irq                          ;   3 timer3
                DCD      dma0_irq                            ;   4 dma0
                DCD      dma1_irq                            ;   5 dma1
                DCD      sdioh0_irq                          ;   6 sdioh0
                DCD      sdioh1_irq                          ;   7 sdioh1
                DCD      ipc_mcu_irq                         ;   8 ipc mcu
                DCD      usbotg_irq                          ;   9 usbotg
                DCD      iir_irq                             ;   10 iir
                DCD      blend_irq                           ;   11 trigfunc
                DCD      fft_irq                             ;   12 fft
                DCD      sec_aes_irq                         ;   13 sec_aes
                DCD      Interrupt14_Handler                 ;   14 Interrupt rsv
                DCD      Interrupt15_Handler                 ;   15 Interrupt rsv
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
                DCD      Interrupt52_Handler                 ;   52 Interrupt rsv
                DCD      cali_irq                            ;   53 cali
                DCD      trng_irq                            ;   54 trng
                DCD      tick_irq                            ;   55 tick
                DCD      Interrupt56_Handler                 ;   56 Interrupt rsv
                DCD      Interrupt57_Handler                 ;   57 Interrupt rsv
                DCD      Interrupt58_Handler                 ;   58 Interrupt rsv
                DCD      Interrupt59_Handler                 ;   59 Interrupt rsv
                DCD      timer4_irq                          ;   60 timer4
                DCD      timer5_irq                          ;   61 timer5
                DCD      Interrupt62_Handler                 ;   62 Interrupt rsv
                DCD      ipc_dsp_irq                         ;   63 ipc
                DCD      yuv2rgb_irq                         ;   64 yuv2rgb
                DCD      pmu_irq                             ;   65 pmu
                ; OTA check flag
                DCD      0xAA55AA55                          ;   app check data
                DCD      0x00000001                          ;   app version
                DCD      0                                   ;   code length
                    
;                SPACE    (470 * 4)                           ;   Interrupts 10 .. 480 are left out
__Vectors_End
__Vectors_Size  EQU      __Vectors_End - __Vectors


                AREA     |.text|, CODE, READONLY

; Reset Handler

Reset_Handler   PROC
                EXPORT   Reset_Handler             [WEAK]
                IMPORT   SystemInit
                IMPORT   __main

                LDR      R0, =__stack_limit
                MSR      MSPLIM, R0                          ; Non-secure version of MSPLIM is RAZ/WI

                LDR      R0, =SystemInit
                BLX      R0
                LDR      R0, =__main
                BX       R0
                ENDP


; Macro to define default exception/interrupt handlers.
; Default handler are weak symbols with an endless loop.
; They can be overwritten by real handlers.
                MACRO
                Set_Default_Handler  $Handler_Name
$Handler_Name   PROC
                EXPORT   $Handler_Name             [WEAK]
                B        .
                ENDP
                MEND


; Default exception/interrupt handler

                Set_Default_Handler  NMI_Handler
                Set_Default_Handler  HardFault_Handler
                Set_Default_Handler  MemManage_Handler
                Set_Default_Handler  BusFault_Handler
                Set_Default_Handler  UsageFault_Handler
                Set_Default_Handler  SecureFault_Handler
                Set_Default_Handler  SVC_Handler
                Set_Default_Handler  DebugMon_Handler
                Set_Default_Handler  PendSV_Handler
                Set_Default_Handler  SysTick_Handler

                Set_Default_Handler  timer0_irq
                Set_Default_Handler  timer1_irq
                Set_Default_Handler  timer2_irq
                Set_Default_Handler  timer3_irq
                Set_Default_Handler  dma0_irq
                Set_Default_Handler  dma1_irq
                Set_Default_Handler  sdioh0_irq
                Set_Default_Handler  sdioh1_irq
                Set_Default_Handler  ipc_mcu_irq
                Set_Default_Handler  usbotg_irq
                Set_Default_Handler  iir_irq
                Set_Default_Handler  blend_irq
                Set_Default_Handler  fft_irq
                Set_Default_Handler  sec_aes_irq
                Set_Default_Handler  Interrupt14_Handler
                Set_Default_Handler  Interrupt15_Handler
                Set_Default_Handler  gpioa_irq
                Set_Default_Handler  gpiob_irq
                Set_Default_Handler  gpioc_irq
                Set_Default_Handler  gpiod_irq
                Set_Default_Handler  uart0_irq
                Set_Default_Handler  uart1_irq
                Set_Default_Handler  uart2_irq
                Set_Default_Handler  uart3_irq
                Set_Default_Handler  uart4_irq
                Set_Default_Handler  uart5_irq
                Set_Default_Handler  i2c0_irq     
                Set_Default_Handler  i2c1_irq     
                Set_Default_Handler  i2c2_irq     
                Set_Default_Handler  i2c3_irq     
                Set_Default_Handler  i2c4_irq     
                Set_Default_Handler  i2c5_irq     
                Set_Default_Handler  spim0_irq    
                Set_Default_Handler  spim1_irq    
                Set_Default_Handler  spim2_irq    
                Set_Default_Handler  spis0_irq    
                Set_Default_Handler  spis1_irq    
                Set_Default_Handler  spimx8_0_irq
                Set_Default_Handler  spimx8_1_irq 
                Set_Default_Handler  i2s0_irq     
                Set_Default_Handler  i2s1_irq     
                Set_Default_Handler  i2s2_irq     
                Set_Default_Handler  pdm0_irq
                Set_Default_Handler  pdm1_irq
                Set_Default_Handler  pdm2_irq
                Set_Default_Handler  adc_irq      
                Set_Default_Handler  codec_irq
                Set_Default_Handler  spdif_irq    
                Set_Default_Handler  sbc_dec_irq  
                Set_Default_Handler  sbc_enc_irq  
                Set_Default_Handler  mp3dec_irq   
                Set_Default_Handler  parallel0_irq
                Set_Default_Handler  Interrupt52_Handler      	
                Set_Default_Handler  cali_irq     
                Set_Default_Handler  trng_irq     	
                Set_Default_Handler  tick_irq	
                Set_Default_Handler  Interrupt56_Handler
                Set_Default_Handler  Interrupt57_Handler
                Set_Default_Handler  Interrupt58_Handler
                Set_Default_Handler  Interrupt59_Handler				
                Set_Default_Handler  timer4_irq         
                Set_Default_Handler  timer5_irq         
                Set_Default_Handler  Interrupt62_Handler
                Set_Default_Handler  ipc_dsp_irq	
                Set_Default_Handler  yuv2rgb_irq
                Set_Default_Handler  pmu_irq
                ALIGN


; User setup Stack & Heap

                IF      :DEF:__MICROLIB

                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit

                ELSE

                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap

__user_initial_stackheap PROC
                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR
                ENDP

                ALIGN

                ENDIF

                END
