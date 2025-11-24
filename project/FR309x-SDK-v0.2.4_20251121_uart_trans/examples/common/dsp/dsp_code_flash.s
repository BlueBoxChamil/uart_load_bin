    AREA RO,DATA,READONLY 

; dsp_code_flash is compiled based on ROM code is stored in XIP flash
    EXPORT DSP_CODE_FLASH_BASE
    EXPORT DSP_CODE_FLASH_END
DSP_CODE_FLASH_BASE
        incbin dsp_code_flash
DSP_CODE_FLASH_END

    END
