#include "chsc6x_platform.h"
#include "fr30xx.h"

void touchpad_reset_set(void);
void touchpad_reset_clear(void);
void touchpad_delay_ms(uint32_t counter);

extern I2C_HandleTypeDef i2c_touchpad_handle;

/************************************************************************************
 * @fn      i2c_memory_read
 *
 * @brief   i2c memory read.
 */
bool i2c_memory_chsc6x_read_only(I2C_HandleTypeDef *hi2c, uint16_t fu16_DevAddress, uint8_t *fp_Data, uint32_t fu32_Size)
{
    uint32_t lu32_RxCount = fu32_Size;

    if (fu32_Size == 0) 
        goto _exit_i2c_read;

    __I2C_DISABLE(hi2c->I2Cx);

    hi2c->I2Cx->TAR.TAR = fu16_DevAddress >> 1;

    __I2C_ENABLE(hi2c->I2Cx);
    
    hi2c->I2Cx->DATA_CMD = (0 >> 8) & 0xFF;
    hi2c->I2Cx->DATA_CMD = 0 & 0xFF;

    while (!__I2C_IS_TxFIFO_EMPTY(hi2c->I2Cx));

    /* DevAddress NACK */
    if (i2c_get_int_status(hi2c, INT_TX_ABRT)) 
    {
        i2c_clear_int_status(hi2c, INT_TX_ABRT);

        __I2C_DISABLE(hi2c->I2Cx);

        goto _exit_i2c_read;
    }

    if (fu32_Size > 1) 
    {
        hi2c->I2Cx->DATA_CMD = CMD_RESTART | CMD_READ;

        while (fu32_Size - 2) 
        {
            if (!__I2C_IS_TxFIFO_FULL(hi2c->I2Cx))
            {
                hi2c->I2Cx->DATA_CMD = CMD_READ;

                fu32_Size--;
            }

            while (!__I2C_IS_RxFIFO_EMPTY(hi2c->I2Cx)) 
            {
                *fp_Data++ = hi2c->I2Cx->DATA_CMD & 0xFF;

                lu32_RxCount--;
            }

            if (i2c_get_int_status(hi2c, INT_TX_ABRT)) 
            {
                i2c_clear_int_status(hi2c, INT_TX_ABRT);

                __I2C_DISABLE(hi2c->I2Cx);

                goto _exit_i2c_read;
            }
        }

        /* Last byte with stop */
        while (__I2C_IS_TxFIFO_FULL(hi2c->I2Cx));
        hi2c->I2Cx->DATA_CMD = CMD_READ | CMD_STOP;
    }
    else 
    {
        hi2c->I2Cx->DATA_CMD = CMD_RESTART | CMD_READ | CMD_STOP;
    }
    
    uint8_t to_counter = 100;
    while (lu32_RxCount) 
    {
        if (!__I2C_IS_RxFIFO_EMPTY(hi2c->I2Cx)) 
        {
            *fp_Data++ = hi2c->I2Cx->DATA_CMD & 0xFF;

            lu32_RxCount--;
        }
        
        if (i2c_get_int_status(hi2c, INT_TX_ABRT)) 
        {
            i2c_clear_int_status(hi2c, INT_TX_ABRT);

            __I2C_DISABLE(hi2c->I2Cx);

            goto _exit_i2c_read;
        }
        
        system_delay_us(10);
        to_counter--;
        if (to_counter == 0) {
            break;
        }
    }

    while(__I2C_IS_BUSY(hi2c->I2Cx));
    
    if (lu32_RxCount == 0) {
        return true;
    }
    else {
        return false;
    }

_exit_i2c_read:

    __NOP();

    return false;
}

/************************************************************************************
 * @fn      i2c_memory_read
 *
 * @brief   i2c memory read.
 */
bool i2c_memory_chsc6x_read(I2C_HandleTypeDef *hi2c, uint16_t fu16_DevAddress, uint16_t fu16_MemAddress, uint8_t *fp_Data, uint32_t fu32_Size)
{
    uint32_t lu32_RxCount = fu32_Size;

    if (fu32_Size == 0) 
        goto _exit_i2c_read;

    __I2C_DISABLE(hi2c->I2Cx);

    hi2c->I2Cx->TAR.TAR = fu16_DevAddress >> 1;

    __I2C_ENABLE(hi2c->I2Cx);

    hi2c->I2Cx->DATA_CMD = (fu16_MemAddress >> 8) & 0xFF;
    hi2c->I2Cx->DATA_CMD = fu16_MemAddress & 0xFF;

    while (!__I2C_IS_TxFIFO_EMPTY(hi2c->I2Cx));

    /* DevAddress NACK */
    if (i2c_get_int_status(hi2c, INT_TX_ABRT)) 
    {
        i2c_clear_int_status(hi2c, INT_TX_ABRT);

        __I2C_DISABLE(hi2c->I2Cx);

        goto _exit_i2c_read;
    }

    if (fu32_Size > 1) 
    {
        hi2c->I2Cx->DATA_CMD = CMD_RESTART | CMD_READ;

        while (fu32_Size - 2) 
        {
            if (!__I2C_IS_TxFIFO_FULL(hi2c->I2Cx))
            {
                hi2c->I2Cx->DATA_CMD = CMD_READ;

                fu32_Size--;
            }

            while (!__I2C_IS_RxFIFO_EMPTY(hi2c->I2Cx)) 
            {
                *fp_Data++ = hi2c->I2Cx->DATA_CMD & 0xFF;

                lu32_RxCount--;
            }

            if (i2c_get_int_status(hi2c, INT_TX_ABRT)) 
            {
                i2c_clear_int_status(hi2c, INT_TX_ABRT);

                __I2C_DISABLE(hi2c->I2Cx);

                goto _exit_i2c_read;
            }
        }

        /* Last byte with stop */
        while (__I2C_IS_TxFIFO_FULL(hi2c->I2Cx));
        hi2c->I2Cx->DATA_CMD = CMD_READ | CMD_STOP;
    }
    else 
    {
        hi2c->I2Cx->DATA_CMD = CMD_RESTART | CMD_READ | CMD_STOP;
    }
    
    while (lu32_RxCount) 
    {
        if (!__I2C_IS_RxFIFO_EMPTY(hi2c->I2Cx)) 
        {
            *fp_Data++ = hi2c->I2Cx->DATA_CMD & 0xFF;

            lu32_RxCount--;
        }
    }

    while(__I2C_IS_BUSY(hi2c->I2Cx));
    
    return true;

_exit_i2c_read:

    __NOP();

    return false;
}

/************************************************************************************
 * @fn      i2c_memory_write
 *
 * @brief   i2c memory write.
 */
bool i2c_memory_chsc6x_write(I2C_HandleTypeDef *hi2c, uint16_t fu16_DevAddress, uint16_t fu16_MemAddress, uint8_t *fp_Data, uint32_t fu32_Size)
{
    __I2C_DISABLE(hi2c->I2Cx);

    hi2c->I2Cx->TAR.TAR = fu16_DevAddress >> 1;

    __I2C_ENABLE(hi2c->I2Cx);

    hi2c->I2Cx->DATA_CMD = (fu16_MemAddress >> 8) & 0xFF;
    hi2c->I2Cx->DATA_CMD = fu16_MemAddress & 0xFF;

    while (!__I2C_IS_TxFIFO_EMPTY(hi2c->I2Cx));

    /* DevAddress NACK */
    if (i2c_get_int_status(hi2c, INT_TX_ABRT)) 
    {
        i2c_clear_int_status(hi2c, INT_TX_ABRT);

        __I2C_DISABLE(hi2c->I2Cx);

        goto  _exit_i2c_write;
    }

    while (fu32_Size - 1 > 0) 
    {
        if (!__I2C_IS_TxFIFO_FULL(hi2c->I2Cx))
        {
            hi2c->I2Cx->DATA_CMD = *fp_Data++;
            
            fu32_Size--;
        }

        if (i2c_get_int_status(hi2c, INT_TX_ABRT)) 
        {
            i2c_clear_int_status(hi2c, INT_TX_ABRT);

            __I2C_DISABLE(hi2c->I2Cx);

            goto  _exit_i2c_write;
        }
    }

    /* Last byte with stop */
    while (__I2C_IS_TxFIFO_FULL(hi2c->I2Cx));
    hi2c->I2Cx->DATA_CMD = *fp_Data | CMD_STOP;

    while(__I2C_IS_BUSY(hi2c->I2Cx));

    //while(i2c_memory_is_busy(hi2c, fu16_DevAddress));

    return true;

_exit_i2c_write:

    __NOP();

    return false;
}

/* return: =read lenth succeed; <0 failed 
   read reg addr not need 
   just used for reading xy cord info*/
int chsc6x_i2c_read(unsigned char id, unsigned char *p_data, unsigned short lenth)
{    
    bool ret = i2c_memory_chsc6x_read_only(&i2c_touchpad_handle, id, p_data, lenth);
//    bool ret = i2c_memory_chsc6x_read(&i2c_touchpad_handle, id, 0, p_data, lenth);
    if (ret) {
        return lenth;
    }else{
        return -1;
    }
    
//    return i2cRead(id, lenth, p_data);
}

/* RETURN:0->pass else->fail */
int chsc6x_read_bytes_u16addr_sub(unsigned char id, unsigned short adr, unsigned char *rxbuf, unsigned short lenth)
{
    bool ret = i2c_memory_chsc6x_read(&i2c_touchpad_handle, id, adr, rxbuf, lenth);
    if (ret) {
        return 0;
    }else{
        return -1;
    }
}

/* RETURN:0->pass else->fail */
int chsc6x_write_bytes_u16addr_sub(unsigned char id, unsigned short adr, unsigned char *txbuf, unsigned short lenth)
{
    bool ret = i2c_memory_chsc6x_write(&i2c_touchpad_handle, id, adr, txbuf, lenth);
    if(ret) {
        return 0;
    }else{
        return -1;
    }
}

void chsc6x_msleep(int ms)
{
    touchpad_delay_ms(ms);
}

void chsc6x_tp_reset(void)
{
    touchpad_reset_clear();
    touchpad_delay_ms(30);//30ms
    touchpad_reset_set();
    touchpad_delay_ms(30);//30ms
}

void chsc6x_tp_reset_active(void)
{
    touchpad_reset_clear();
    touchpad_delay_ms(30);//30ms
    touchpad_reset_set();
    touchpad_delay_ms(30);//30ms
}

