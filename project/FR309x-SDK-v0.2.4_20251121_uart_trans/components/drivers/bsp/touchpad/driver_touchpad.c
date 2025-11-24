#include <stdint.h>

#include "driver_touchpad.h"

//#define TOUCHPAD_SH8601A
//#define TOUCHPAD_ICNA3310
//#define TOUCHPAD_CHSC586
//#define TOUCHPAD_CHSC6x
#define TOUCHPAD_CTP820

#ifdef TOUCHPAD_SH8601A
#define TOUCHPAD_IIC_SLAVE_ADDRESS      (0x2C<<1)   // from sh8601a
#endif
#ifdef TOUCHPAD_ICNA3310
#define TOUCHPAD_IIC_SLAVE_ADDRESS (0x48 << 1)      // from icna3310
#endif
#ifdef TOUCHPAD_CTP820
#define TOUCHPAD_IIC_SLAVE_ADDRESS (0x15 << 1)      // from ctp820
#endif

#ifdef TOUCHPAD_CHSC586
extern bool semi_touch_read_coordinate(int16_t *x, int16_t *y);
extern int semi_touch_init();
#endif

#ifdef TOUCHPAD_CHSC6x
bool chsc6x_read_touch_info(int16_t *x, int16_t *y);
void chsc6x_init(void);
#endif

bool touchpad_read(int16_t *x, int16_t *y)
{
#ifdef TOUCHPAD_SH8601A
    uint8_t buffer[8];
    bool pressed = false;

    i2c_memory_read(&i2c_touchpad_handle, TOUCHPAD_IIC_SLAVE_ADDRESS, 0, buffer, 8);
    
    if (buffer[2] != 0) {
        *x = ((buffer[3]&0x0f)<<8) | buffer[4];
        *y = ((buffer[5]&0x0f)<<8) | buffer[6];
        pressed = true;
    }
    else {
        pressed = false;
    }
    
    return pressed;
#endif
#ifdef TOUCHPAD_ICNA3310
    uint8_t buffer[9];
    bool pressed = false;

    i2c_memory_read(&i2c_touchpad_handle, TOUCHPAD_IIC_SLAVE_ADDRESS, 0x1000, buffer, 9);
    
    if (buffer[1] == 1) {
        *x = ((buffer[4]&0x0f)<<8) | buffer[3];
        *y = ((buffer[6]&0x0f)<<8) | buffer[5];
        pressed = true;
    }
    else {
        pressed = false;
    }
    
    return pressed;
#endif

#ifdef TOUCHPAD_CHSC586
    return semi_touch_read_coordinate(x, y);
#endif

#ifdef TOUCHPAD_CHSC6x
    return chsc6x_read_touch_info(x, y);
#endif

#ifdef TOUCHPAD_CTP820
    uint8_t buffer[7];
    bool pressed = false;

    //i2c_memory_read(&i2c_touchpad_handle, TOUCHPAD_IIC_SLAVE_ADDRESS, 0x00, buffer, 7);
    i2c_sensor_read(&i2c_touchpad_handle, TOUCHPAD_IIC_SLAVE_ADDRESS, 0x00, 8, buffer, 7);
    
    if (buffer[2] == 1) {
        *x = ((buffer[3]&0x0f)<<8) | buffer[4];
        *y = ((buffer[5]&0x0f)<<8) | buffer[6];
        pressed = true;
    }
    else {
        pressed = false;
    }
    
    return pressed;
#endif

    return false;
}

void touchpad_init(void)
{
#ifdef TOUCHPAD_ICNA3310
    /* reset touch pad chip */
    __TOUCHPAD_RESET_SET();
    __TOUCHPAD_DELAY_MS(10);
    __TOUCHPAD_RESET_CLEAR();
    __TOUCHPAD_DELAY_MS(5);
    __TOUCHPAD_RESET_SET();
    __TOUCHPAD_DELAY_MS(200);
#endif

#ifdef TOUCHPAD_CHSC586
    semi_touch_init();
#endif
    
#ifdef TOUCHPAD_CHSC6x
    chsc6x_init();
#endif
    
#ifdef TOUCHPAD_CTP820
    /* reset touch pad chip */
    __TOUCHPAD_RESET_SET();
    __TOUCHPAD_DELAY_MS(10);
    __TOUCHPAD_RESET_CLEAR();
    __TOUCHPAD_DELAY_MS(10);
    __TOUCHPAD_RESET_SET();
    __TOUCHPAD_DELAY_MS(50);
#endif
}


void touchpad_sleep(void)
{

#ifdef TOUCHPAD_CTP820
    uint8_t value = 0x03;
    //i2c_memory_write(&i2c_touchpad_handle, TOUCHPAD_IIC_SLAVE_ADDRESS, 0xE5, &value, 1);
    i2c_sensor_write(&i2c_touchpad_handle, TOUCHPAD_IIC_SLAVE_ADDRESS, 0xE5, 8, &value, 1);

#endif

}
