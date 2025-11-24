
#ifndef __DRIVER_DISPLAY_H__
#define __DRIVER_DISPLAY_H__

#include <stdint.h>

#include "driver_spi.h"
#include "driver_dma.h"

#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
//
// Macro Variables definitions
//
//*****************************************************************************

#define __DISPLAY_CS_SET()              display_cs_set()
#define __DISPLAY_CS_CLEAR()            display_cs_clear()
#define __DISPLAY_RESET_SET()           display_reset_set()
#define __DISPLAY_RESET_CLEAR()         display_reset_clear()
#define __DISPLAY_VCI_SET()             display_vci_set()
#define __DISPLAY_VCI_CLEAR()           display_vci_clear()
#define __DISPLAY_DELAY_MS(counter)     display_delay_ms(counter)

//*****************************************************************************
//
// Global Variables definitions
//
//*****************************************************************************
extern SPI_HandleTypeDef spi_display_handle;
extern DMA_HandleTypeDef dma_display_handle;

//*****************************************************************************
//
// External function definitions
//
//*****************************************************************************

/************************************************************************************
 * @fn      display_cs_set
 *
 * @brief   Set display driver CS pin to HIGH, this function should be implemented by user when
 *          CS is controlled by software.
 */
void display_cs_set(void);

/************************************************************************************
 * @fn      display_cs_release
 *
 * @brief   Set display driver CS pin to LOW, this function should be implemented by user when
 *          CS is controlled by software.
 */
void display_cs_clear(void);

/************************************************************************************
 * @fn      display_reset_set
 *
 * @brief   Set display driver RESET pin to HIGH, this function should be implemented by user..
 */
void display_reset_set(void);

/************************************************************************************
 * @fn      display_reset_clear
 *
 * @brief   Set display driver RESET pin to LOW, this function should be implemented by user.
 */
void display_reset_clear(void);

/************************************************************************************
 * @fn      display_vci_set
 *
 * @brief   Set display driver VCI pin to HIGH, this function should be implemented by user..
 */
void display_vci_set(void);

/************************************************************************************
 * @fn      display_vci_clear
 *
 * @brief   Set display driver VCI pin to LOW, this function should be implemented by user.
 */
void display_vci_clear(void);

/************************************************************************************
 * @fn      display_delay_ms
 *
 * @brief   Used in display driver. co_delay_100us or vTaskDelay can be used for implementation
 *          by user.
 */
void display_delay_ms(uint32_t counter);

/************************************************************************************
 * @fn      display_init
 *
 * @brief   Initial display drivers.
 */
void display_init(void);

/************************************************************************************
 * @fn      display_set_window
 *
 * @brief   used to define area of frame memory where MCU can access. 
 *
 * @param   x_s: SC.
 *          x_e: EC.
 *          y_s: SP.
 *          y_e: EP.
 */
void display_set_window(uint16_t x_s, uint16_t x_e, uint16_t y_s, uint16_t y_e);

/************************************************************************************
 * @fn      display_update
 *
 * @brief   transfer data to framebuffer of display controller in block mode.
 *
 * @param   pixel_count: total pixels count to be sent.
 *          pixel_width: this parameter should be 16, 24.
 *          data: pointer to data buffer
 */
void display_update(uint32_t pixel_count, uint8_t pixel_width, void *data);

/************************************************************************************
 * @fn      display_update_dma
 *
 * @brief   transfer data to framebuffer of display controller in DMA mode.
 *
 * @param   pixel_count: total pixels count to be sent.
 *          pixel_width: this parameter should be 16, 24.
 *          data: pointer to data buffer
 */
void display_update_dma(uint32_t pixel_count, uint8_t pixel_width, void *data);

/************************************************************************************
 * @fn      display_update_dma_isr
 *
 * @brief   this function will be called in DMA isr handler when dma transfer is done.
 */
void display_update_dma_isr(void);

/************************************************************************************
 * @fn      display_power_off
 *
 * @brief   used to power off display to save power.
 */
void display_power_off(void);

/************************************************************************************
 * @fn      display_power_on
 *
 * @brief   turn on display.
 */
void display_power_on(void);


/************************************************************************************
 * @fn      display_brightness_adj
 *
 * @brief   
 */
void display_brightness_adj(void);

#ifdef __cplusplus
}
#endif

#endif	/* __DRIVER_DISPLAY_H__ */

