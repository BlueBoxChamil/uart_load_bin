#ifndef _DRIVER_TOUCHPAD_H
#define _DRIVER_TOUCHPAD_H

#include <stdint.h>

#include "driver_i2c.h"

#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
//
// Macro Variables definitions
//
//*****************************************************************************

#define __TOUCHPAD_RESET_SET()           touchpad_reset_set()
#define __TOUCHPAD_RESET_CLEAR()         touchpad_reset_clear()
#define __TOUCHPAD_DELAY_MS(counter)     touchpad_delay_ms(counter)

//*****************************************************************************
//
// Global Variables definitions
//
//*****************************************************************************
extern I2C_HandleTypeDef i2c_touchpad_handle;

//*****************************************************************************
//
// External function definitions
//
//*****************************************************************************

/************************************************************************************
 * @fn      touchpad_reset_set
 *
 * @brief   Set touchpad driver RESET pin to HIGH, this function should be implemented by user..
 */
void touchpad_reset_set(void);

/************************************************************************************
 * @fn      touchpad_reset_clear
 *
 * @brief   Set touchpad driver RESET pin to LOW, this function should be implemented by user.
 */
void touchpad_reset_clear(void);

/************************************************************************************
 * @fn      touchpad_delay_ms
 *
 * @brief   Used in touchpad driver. co_delay_100us or vTaskDelay can be used for implementation
 *          by user.
 */
void touchpad_delay_ms(uint32_t counter);

/************************************************************************************
 * @fn      touchpad_read
 *
 * @brief   used to read data from touchpad, these data are update in interrupt
 *
 * @param   x: current x position.
 *          y: current y position.
 *
 * @return  true: pressed and x, y value is valid; false: released
 */
bool touchpad_read(int16_t *x, int16_t *y);

/************************************************************************************
 * @fn      touchpad_init
 *
 * @brief   used to init touchpad
 */
void touchpad_init(void);

/************************************************************************************
 * @fn      touchpad_sleep
 *
 * @brief   touchpad go to sleep
 */
void touchpad_sleep(void);

#ifdef __cplusplus
}
#endif

#endif  // _DRIVER_TOUCHPAD_H
