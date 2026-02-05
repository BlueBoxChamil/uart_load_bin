/************************* (C) COPYRIGHT 2024 FreqChip ***************************
 * File Name          : startup_fr30xx.s
 * Author             : FreqChip Firmware Team
 * Version            : V1.0.0
 * Date               : 2024
 * Description        : fr30xx Devices vector table for MDK-ARM toolchain.
 *                      This module performs:
 *                      - Set the initial SP
 *                      - Set the initial PC == Reset_Handler
 *                      - Set the vector table entries with the exceptions ISR address
 *                      - Configure the clock system
 *                      - Branches to __main in the C library (which eventually
 *                        calls main()).
 *                      After Reset the Cortex-M33 processor is in Thread mode,
 *                      priority is Privileged, and the Stack is set to Main.
 *********************************************************************************
 * @attention
 *
 * Copyright (c) 2024 FreqChip.
 * All rights reserved.
 *********************************************************************************/

    .syntax unified
    .cpu cortex-m33
    .fpu softvfp
    .thumb

.global    	g_pfnVectors
.global    	Default_Handler
.global		_sheap
.global     _eheap

    .section .data.stack
    .align 4
_sstack:
    .space 0x00001000
_estack:

/*
	.section .data.heap
	.align 4
_sheap:
    .space 0x00010000
_eheap:
*/

/* start address for the initialization values of the .ram_code_front section.
defined in linker script */
.word    _siram_code_front
/* start address for the .ram_code_front section. defined in linker script */
.word    _sram_code_front
/* end address for the .ram_code_front section. defined in linker script */
.word    _eram_code_front
/* start address for the initialization values of the .ram_code section.
defined in linker script */
.word    _siram_code
/* start address for the .ram_code section. defined in linker script */
.word    _sram_code
/* end address for the .ram_code section. defined in linker script */
.word    _eram_code
/* start address for the initialization values of the .data section.
defined in linker script */
.word    _sidata
/* start address for the .data section. defined in linker script */
.word    _sdata
/* end address for the .data section. defined in linker script */
.word    _edata
/* start address for the .bss section. defined in linker script */
.word    _sbss
/* end address for the .bss section. defined in linker script */
.word    _ebss

/**
 * @brief  This is the code that gets called when the processor first
 *          starts execution following a reset event. Only the absolutely
 *          necessary set is performed, after which the application
 *          supplied main() routine is called.
 * @param  None
 * @retval : None
*/
    .section    .text.Reset_Handler
    .weak    Reset_Handler
    .type    Reset_Handler, %function
Reset_Handler:
    ldr   sp, =_estack    /* set stack pointer */

/* Call the clock system initialization function.*/
    bl  SystemInit

/* Copy the ram_code_front segment initializers from flash to IRAM */
    movs    r1, #0
    b    LoopCopyRamCodeFrontInit

CopyRamCodeFrontInit:
    ldr    r3, =_siram_code_front
    ldr    r3, [r3, r1]
    str    r3, [r0, r1]
    adds    r1, r1, #4

LoopCopyRamCodeFrontInit:
    ldr    r0, =_sram_code_front
    ldr    r3, =_eram_code_front
    adds    r2, r0, r1
    cmp    r2, r3
    bcc    CopyRamCodeFrontInit

/* Copy the ram_code segment initializers from flash to IRAM */
    movs    r1, #0
    b    LoopCopyRamCodeInit

CopyRamCodeInit:
    ldr    r3, =_siram_code
    ldr    r3, [r3, r1]
    str    r3, [r0, r1]
    adds    r1, r1, #4

LoopCopyRamCodeInit:
    ldr    r0, =_sram_code
    ldr    r3, =_eram_code
    adds    r2, r0, r1
    cmp    r2, r3
    bcc    CopyRamCodeInit

/* Copy the data segment initializers from flash to SRAM */
    movs    r1, #0
    b    LoopCopyDataInit

CopyDataInit:
    ldr    r3, =_sidata
    ldr    r3, [r3, r1]
    str    r3, [r0, r1]
    adds    r1, r1, #4

LoopCopyDataInit:
    ldr    r0, =_sdata
    ldr    r3, =_edata
    adds    r2, r0, r1
    cmp    r2, r3
    bcc    CopyDataInit

/* Zero fill the bss segment. */
    ldr    r2, =_sbss
    b    LoopFillZerobss

FillZerobss:
    movs    r3, #0
    str    r3, [r2], #4

LoopFillZerobss:
    ldr    r3, = _ebss
    cmp    r2, r3
    bcc    FillZerobss

/* Call static constructors */
    bl __libc_init_array
/* Call the application's entry point.*/
    bl    entry
    bl    main

LoopForever:
    b LoopForever

.size    Reset_Handler, .-Reset_Handler

/**
 * @brief  This is the code that gets called when the processor receives an
 *         unexpected interrupt.  This simply enters an infinite loop, preserving
 *         the system state for examination by a debugger.
 *
 * @param  None
 * @retval : None
*/
    .section    .text.Default_Handler,"ax",%progbits
Default_Handler:
Infinite_Loop:
    b    Infinite_Loop
    .size    Default_Handler, .-Default_Handler

/******************************************************************************
*
* The minimal vector table for a Cortex-M33.
*
******************************************************************************/
     .section    .isr_vector,"a",%progbits
    .type    g_pfnVectors, %object
    .size    g_pfnVectors, .-g_pfnVectors

g_pfnVectors:
    .word    _estack
    .word    Reset_Handler      
    .word    NMI_Handler        
    .word    HardFault_Handler  
    .word    MemManage_Handler  
    .word    BusFault_Handler   
    .word    UsageFault_Handler 
    .word    SecureFault_Handler
    .word    0                  
    .word    0                  
    .word    0                  
    .word    SVC_Handler        
    .word    DebugMon_Handler   
    .word    0                  
    .word    PendSV_Handler     
    .word    SysTick_Handler    

    .word    timer0_irq         
    .word    timer1_irq         
    .word    timer2_irq         
    .word    timer3_irq         
    .word    dma0_irq           
    .word    dma1_irq           
    .word    sdioh0_irq         
    .word    sdioh1_irq         
    .word    ipc_mcu_irq        
    .word    usbotg_irq         
    .word    iir_irq            
    .word    blend_irq          
    .word    fft_irq            
    .word    sec_aes_irq        
    .word    Interrupt14_Handler
    .word    Interrupt15_Handler
    .word    gpioa_irq          
    .word    gpiob_irq          
    .word    gpioc_irq          
    .word    gpiod_irq          
    .word    uart0_irq          
    .word    uart1_irq          
    .word    uart2_irq          
    .word    uart3_irq          
    .word    uart4_irq          
    .word    uart5_irq          
    .word    i2c0_irq           
    .word    i2c1_irq           
    .word    i2c2_irq           
    .word    i2c3_irq           
    .word    i2c4_irq           
    .word    i2c5_irq           
    .word    spim0_irq          
    .word    spim1_irq          
    .word    spim2_irq          
    .word    spis0_irq          
    .word    spis1_irq          
    .word    spimx8_0_irq       
    .word    spimx8_1_irq       
    .word    i2s0_irq           
    .word    i2s1_irq           
    .word    i2s2_irq           
    .word    pdm0_irq           
    .word    pdm1_irq           
    .word    pdm2_irq           
    .word    adc_irq            
    .word    codec_irq          
    .word    spdif_irq          
    .word    sbc_dec_irq        
    .word    sbc_enc_irq        
    .word    mp3dec_irq         
    .word    parallel0_irq      
    .word    Interrupt52_Handler
    .word    cali_irq           
    .word    trng_irq           
    .word    tick_irq           
    .word    Interrupt56_Handler
    .word    Interrupt57_Handler
    .word    Interrupt58_Handler
    .word    Interrupt59_Handler
    .word    timer4_irq         
    .word    timer5_irq         
    .word    Interrupt62_Handler
    .word    ipc_dsp_irq        
    .word    yuv2rgb_irq        
    .word    pmu_irq            
    .word    0xAA55AA55         
    .word    0x00000001         
    .word    0

/*******************************************************************************
*
* Provide weak aliases for each Exception handler to the Default_Handler.
* As they are weak aliases, any function with the same name will override
* this definition.
*
*******************************************************************************/

    .weak    NMI_Handler
    .thumb_set NMI_Handler,Default_Handler

    .weak    HardFault_Handler
    .thumb_set HardFault_Handler,Default_Handler

    .weak    MemManage_Handler
    .thumb_set MemManage_Handler,Default_Handler

    .weak    BusFault_Handler
    .thumb_set BusFault_Handler,Default_Handler

    .weak    UsageFault_Handler
    .thumb_set UsageFault_Handler,Default_Handler

    .weak    SecureFault_Handler
    .thumb_set SecureFault_Handler,Default_Handler

    .weak    SVC_Handler
    .thumb_set SVC_Handler,Default_Handler

    .weak    DebugMon_Handler
    .thumb_set DebugMon_Handler,Default_Handler

    .weak    PendSV_Handler
    .thumb_set PendSV_Handler,Default_Handler

    .weak    SysTick_Handler
    .thumb_set SysTick_Handler,Default_Handler

    .weak    timer0_irq
    .thumb_set timer0_irq,Default_Handler

    .weak    timer1_irq
    .thumb_set timer1_irq,Default_Handler

    .weak    timer2_irq
    .thumb_set timer2_irq,Default_Handler

    .weak    timer3_irq
    .thumb_set timer3_irq,Default_Handler

    .weak    dma0_irq
    .thumb_set dma0_irq,Default_Handler

    .weak    dma1_irq
    .thumb_set dma1_irq,Default_Handler

    .weak    sdioh0_irq
    .thumb_set sdioh0_irq,Default_Handler

    .weak    sdioh1_irq
    .thumb_set sdioh1_irq,Default_Handler

    .weak    ipc_mcu_irq
    .thumb_set ipc_mcu_irq,Default_Handler

    .weak    usbotg_irq
    .thumb_set usbotg_irq,Default_Handler

    .weak    iir_irq
    .thumb_set iir_irq,Default_Handler

    .weak    blend_irq
    .thumb_set blend_irq,Default_Handler

    .weak    fft_irq
    .thumb_set fft_irq,Default_Handler

    .weak    sec_aes_irq
    .thumb_set sec_aes_irq,Default_Handler

    .weak    Interrupt14_Handler
    .thumb_set Interrupt14_Handler,Default_Handler

    .weak    Interrupt15_Handler
    .thumb_set Interrupt15_Handler,Default_Handler

    .weak    gpioa_irq
    .thumb_set gpioa_irq,Default_Handler

    .weak    gpiob_irq
    .thumb_set gpiob_irq,Default_Handler

    .weak    gpioc_irq
    .thumb_set gpioc_irq,Default_Handler

    .weak    gpiod_irq
    .thumb_set gpiod_irq,Default_Handler

    .weak    uart0_irq
    .thumb_set uart0_irq,Default_Handler

    .weak    uart1_irq
    .thumb_set uart1_irq,Default_Handler

    .weak    uart2_irq
    .thumb_set uart2_irq,Default_Handler

    .weak    uart3_irq
    .thumb_set uart3_irq,Default_Handler

    .weak    uart4_irq
    .thumb_set uart4_irq,Default_Handler

    .weak    uart5_irq
    .thumb_set uart5_irq,Default_Handler

    .weak    i2c0_irq
    .thumb_set i2c0_irq,Default_Handler

    .weak    i2c1_irq
    .thumb_set i2c1_irq,Default_Handler

    .weak    i2c2_irq
    .thumb_set i2c2_irq,Default_Handler

    .weak    i2c3_irq
    .thumb_set i2c3_irq,Default_Handler

    .weak    i2c4_irq
    .thumb_set i2c4_irq,Default_Handler

    .weak    i2c5_irq
    .thumb_set i2c5_irq,Default_Handler

    .weak    spim0_irq
    .thumb_set spim0_irq,Default_Handler

    .weak    spim1_irq
    .thumb_set spim1_irq,Default_Handler

    .weak    spim2_irq
    .thumb_set spim2_irq,Default_Handler

    .weak    spis0_irq
    .thumb_set spis0_irq,Default_Handler

    .weak    spis1_irq
    .thumb_set spis1_irq,Default_Handler

    .weak    spimx8_0_irq
    .thumb_set spimx8_0_irq,Default_Handler

    .weak    spimx8_1_irq
    .thumb_set spimx8_1_irq,Default_Handler

    .weak    i2s0_irq
    .thumb_set i2s0_irq,Default_Handler

    .weak    i2s1_irq
    .thumb_set i2s1_irq,Default_Handler

    .weak    i2s2_irq
    .thumb_set i2s2_irq,Default_Handler

    .weak    pdm0_irq
    .thumb_set pdm0_irq,Default_Handler

    .weak    pdm1_irq
    .thumb_set pdm1_irq,Default_Handler

    .weak    pdm2_irq
    .thumb_set pdm2_irq,Default_Handler

    .weak    adc_irq
    .thumb_set adc_irq,Default_Handler

    .weak    codec_irq
    .thumb_set codec_irq,Default_Handler

    .weak    spdif_irq
    .thumb_set spdif_irq,Default_Handler

    .weak    sbc_dec_irq
    .thumb_set sbc_dec_irq,Default_Handler

    .weak    sbc_enc_irq
    .thumb_set sbc_enc_irq,Default_Handler

    .weak    mp3dec_irq
    .thumb_set mp3dec_irq,Default_Handler

    .weak    parallel0_irq
    .thumb_set parallel0_irq,Default_Handler

    .weak    Interrupt52_Handler
    .thumb_set Interrupt52_Handler,Default_Handler

    .weak    cali_irq
    .thumb_set cali_irq,Default_Handler

    .weak    trng_irq
    .thumb_set trng_irq,Default_Handler

    .weak    tick_irq
    .thumb_set tick_irq,Default_Handler

    .weak    Interrupt56_Handler
    .thumb_set Interrupt56_Handler,Default_Handler

    .weak    Interrupt57_Handler
    .thumb_set Interrupt57_Handler,Default_Handler

    .weak    Interrupt58_Handler
    .thumb_set Interrupt58_Handler,Default_Handler

    .weak    Interrupt59_Handler
    .thumb_set Interrupt59_Handler,Default_Handler

    .weak    timer4_irq
    .thumb_set timer4_irq,Default_Handler

    .weak    timer5_irq
    .thumb_set timer5_irq,Default_Handler

    .weak    Interrupt62_Handler
    .thumb_set Interrupt62_Handler,Default_Handler

    .weak    ipc_dsp_irq
    .thumb_set ipc_dsp_irq,Default_Handler

    .weak    yuv2rgb_irq
    .thumb_set yuv2rgb_irq,Default_Handler

    .weak    pmu_irq
    .thumb_set pmu_irq,Default_Handler

/************************ (C) COPYRIGHT FreqChip *****END OF FILE****/
