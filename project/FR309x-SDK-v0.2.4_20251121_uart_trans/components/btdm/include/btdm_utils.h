#ifndef _BTDM_UTILS_H
#define _BTDM_UTILS_H

#include <stdint.h>

/* Little Endian to Host integer format conversion functions */
uint16_t LEtoHost16(const uint8_t *le_value);
uint32_t LEtoHost32(const uint8_t *le_value);

/* Big Endian to Host integer format conversion functions */
uint16_t BEtoHost16(const uint8_t* be_ptr);
uint32_t BEtoHost32(const uint8_t* be_ptr);

/* Store value into a buffer in Little Endian integer format */
void StoreLE16(uint8_t *buff, uint16_t le_value);
void StoreLE32(uint8_t *buff, uint32_t le_value);

/* Store value into a buffer in Big Endian integer format */
void StoreBE16(uint8_t *ptr, uint16_t be_value);
void StoreBE32(uint8_t *ptr, uint32_t be_value);

#endif	// _BTDM_UTILS_H
