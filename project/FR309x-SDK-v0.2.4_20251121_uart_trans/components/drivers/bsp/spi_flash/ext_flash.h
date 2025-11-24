#ifndef __EXT_FLASH__
#define __EXT_FLASH__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void     ext_flash_device_init(void);
uint32_t ext_flash_get_id(void);
void     spi_flash_cs_set(void);
void     spi_flash_cs_clear(void);

void    ext_flash_erase(uint32_t addr, uint32_t len);
void    ext_flash_chip_erase(void);
void    ext_flash_protect_enable(void);
void    ext_flash_protect_disable(void);
uint8_t ext_flash_read(uint32_t addr, int len,uint8_t* buffer);
uint8_t ext_flash_write(uint32_t addr, int len,uint8_t* buffer);


#ifdef __cplusplus
}
#endif

#endif /* __EXT_FLASH__ */
