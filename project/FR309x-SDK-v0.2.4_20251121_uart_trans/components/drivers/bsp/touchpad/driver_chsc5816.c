/*===============================================================================================*/
/**
 *   @file touch.c
 *
 *   @version v1.0
 */
/*=================================================================================================
 Rootlink Confidential Proprietary
 Advanced Technology and Software Operations

 (c) Copyright Rootlink 2015 - , All Rights Reserved

Revision History:

Modification Tracking
Author          Date            Number          Description of Changes
--------        --------        -------         ------------------------

Portability:
Indicate if this module is portable to other compilers or
platforms. If not, indicate specific reasons why is it not portable.

===================================================================================================
 INCLUDE FILES
=================================================================================================*/
#include <stdint.h>

#include "fr30xx.h"
#include "chsc5816_ctp.h"

#if 0
#define PRINT_DBG  printf
#else
#define PRINT_DBG(...)
#endif

#define RETRY                           1000 
#define _TOUCH_DOWN                     0
#define _TOUCH_UP                       1
#define _TOUCH_CONTACT                  2
#define  CHSC5816_ADDRESS               (0x2e<<1)
        
#define _XSIZE_PHYS                     368
#define _YSIZE_PHYS                     448

#define __CHSC5816_RESET_SET()          touchpad_reset_set()
#define __CHSC5816_RESET_CLEAR()        touchpad_reset_clear()
#define HAL_Delay(counter)              touchpad_delay_ms(counter)

extern I2C_HandleTypeDef i2c_touchpad_handle;

static struct sm_touch_dev st_dev;
static uint8_t counter;

void touchpad_reset_set(void);
void touchpad_reset_clear(void);
void touchpad_delay_ms(uint32_t counter);

/************************************************************************************
 * @fn      i2c_memory_read
 *
 * @brief   i2c memory read.
 */
bool i2c_memory_chsc5816_read(I2C_HandleTypeDef *hi2c, uint16_t fu16_DevAddress, uint32_t fu32_MemAddress, uint8_t *fp_Data, uint32_t fu32_Size)
{
    uint32_t lu32_RxCount = fu32_Size;

    counter = 0;

    if (fu32_Size == 0) 
        goto _exit_i2c_read;

    __I2C_DISABLE(hi2c->I2Cx);

    hi2c->I2Cx->TAR.TAR = fu16_DevAddress >> 1;

    __I2C_ENABLE(hi2c->I2Cx);

    hi2c->I2Cx->DATA_CMD = (fu32_MemAddress >> 24) & 0xFF;
    hi2c->I2Cx->DATA_CMD = (fu32_MemAddress >> 16) & 0xFF;

    while (!__I2C_IS_TxFIFO_EMPTY(hi2c->I2Cx));

    /* DevAddress NACK */
    if (i2c_get_int_status(hi2c, INT_TX_ABRT)) 
    {
        i2c_clear_int_status(hi2c, INT_TX_ABRT);

        __I2C_DISABLE(hi2c->I2Cx);

        counter = 1;

        goto _exit_i2c_read;
    }

    hi2c->I2Cx->DATA_CMD = (fu32_MemAddress >> 8) & 0xFF;
    hi2c->I2Cx->DATA_CMD = fu32_MemAddress & 0xFF;


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

                counter = 2;

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
    PRINT_DBG("I2c r:%d\n",counter);

    __NOP();

    return false;
}

/************************************************************************************
 * @fn      i2c_memory_write
 *
 * @brief   i2c memory write.
 */
bool i2c_memory_chsc5816_write(I2C_HandleTypeDef *hi2c, uint16_t fu16_DevAddress, uint8_t *fp_Data, uint32_t fu32_Size)
{
    counter = 0;

    if (fu32_Size < 2) 
        goto  _exit_i2c_write;
        
    __I2C_DISABLE(hi2c->I2Cx);

    hi2c->I2Cx->TAR.TAR = fu16_DevAddress >> 1;

    __I2C_ENABLE(hi2c->I2Cx);

    hi2c->I2Cx->DATA_CMD = *fp_Data++;
    hi2c->I2Cx->DATA_CMD = *fp_Data++;

    while (!__I2C_IS_TxFIFO_EMPTY(hi2c->I2Cx));

    /* DevAddress NACK */
    if (i2c_get_int_status(hi2c, INT_TX_ABRT)) 
    {
        i2c_clear_int_status(hi2c, INT_TX_ABRT);

        __I2C_DISABLE(hi2c->I2Cx);

        counter = 1;
        goto  _exit_i2c_write;
    }

    fu32_Size -= 2;

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

            counter = 2;
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
    PRINT_DBG("I2c w:%d\n",counter);

    __NOP();

    return false;
}


void semi_touch_power_int(void)
{
    return;   
}

static void semi_touch_msdelay(uint32_t millisecs)
{
    HAL_Delay(millisecs);
}

static void semi_touch_reset(void)
{
    __CHSC5816_RESET_SET();
    HAL_Delay(10);
    __CHSC5816_RESET_CLEAR();
    HAL_Delay(5);
    __CHSC5816_RESET_SET();
    HAL_Delay(200);
}

int32_t semi_touch_iic_write(uint32_t reg, uint8_t* pdata, uint16_t len)
{

    if(i2c_memory_chsc5816_write(&i2c_touchpad_handle, (uint16_t)CHSC5816_ADDRESS, pdata, len) == 0)
    {
        return -1;
    }

	return SEMI_DRV_ERR_OK;
}

/*
    reg : must 4B aligned
    len : must multiple of 4
*/
int32_t semi_i2c_read_bytes(uint32_t reg, uint8_t* pdata, uint16_t len)
{
    if(i2c_memory_chsc5816_read(&i2c_touchpad_handle, (uint16_t)CHSC5816_ADDRESS, reg,  pdata, len) == 0)
    {
        return -1;
    }

	return SEMI_DRV_ERR_OK;
}

/*
reg     - register address, must 4B aligned
buffer  - data buffer
len     - data length, must 4B aligned

return:
    0   - pass
 others - fail
*/
int32_t semi_touch_read_bytes(uint32_t reg, uint8_t* buffer, uint16_t len)
{
    int32_t ret = SEMI_DRV_ERR_OK;
    uint16_t once;
    uint32_t retry;

    while(len > 0){
        once = (len>MAX_IO_BUFFER_LEN)?MAX_IO_BUFFER_LEN:len;

        ret = -1;
        for(retry=0; retry<3; retry++){
            if(semi_i2c_read_bytes(reg, buffer,once) == SEMI_DRV_ERR_OK){
                ret = SEMI_DRV_ERR_OK;
                break;
            }
        }

        if(ret != SEMI_DRV_ERR_OK){
            break;
        }

        reg    += once;
        buffer += once;
        len    -= once;
    }

    return ret;
}

/*
reg      - register address, must 4B aligned
buffer   - data buffer
len      - data length, must 4B aligned

return:
    0    - pass
 others  - fail
*/
int32_t semi_touch_write_bytes(uint32_t reg, uint8_t* buffer, uint16_t len)
{
    int32_t ret = SEMI_DRV_ERR_OK;
    uint16_t once;
    uint32_t k, retry;
    uint8_t writeBuff[MAX_IO_BUFFER_LEN];

    while(len > 0){
        once = (len<(MAX_IO_BUFFER_LEN-4))?len:(MAX_IO_BUFFER_LEN-4);

        writeBuff[0] = (uint8_t)(reg>>24);
        writeBuff[1] = (uint8_t)(reg>>16);
        writeBuff[2] = (uint8_t)(reg>>8);
        writeBuff[3] = (uint8_t)(reg);
        for(k=0; k<once; k++){
            writeBuff[k+4] = buffer[k];
        }

        ret = -1;
        for(retry=0; retry<3; retry++){
            if(semi_touch_iic_write(reg, writeBuff, once+4) == SEMI_DRV_ERR_OK){
                ret = SEMI_DRV_ERR_OK;
                break;
            }
        }
        if(ret != SEMI_DRV_ERR_OK){
            break;
        }

        reg    += once;
        buffer += once;
        len    -= once;
    }

    return ret;
}

int32_t semi_touch_write_and_check(uint32_t addr, uint8_t* buffer, uint16_t len)
{
    int32_t ret = 0, once = 0, index = 0, retry = 0;
    uint8_t cmp_buffer[MAX_IO_BUFFER_LEN-4];

    while(len > 0){
        retry = 0;
        do{
            ret = SEMI_DRV_ERR_OK;
            once = (len<(MAX_IO_BUFFER_LEN-4))?len:(MAX_IO_BUFFER_LEN-4);
            ret = semi_touch_write_bytes(addr, buffer, once);
            ret = semi_touch_read_bytes(addr, cmp_buffer, once);
            for(index = 0; index < once; index++){
                if(cmp_buffer[index] != buffer[index]){
                    ret = -SEMI_DRV_ERR_CHECKSUM;
                    break;
                }
            }
            if(SEMI_DRV_ERR_OK == ret){
                break;
            }         
        }while(++retry < 3);

        if(SEMI_DRV_ERR_OK != ret){
            break;
        }   
        
        addr   += once;
        buffer += once;
        len    -= once;
    }

    return ret;
}

int32_t semi_touch_run_ram_code(const uint8_t* bin_code, uint16_t len)
{
    int32_t retry;
    int32_t ret = 0, reg_value = 0;

    for(retry = 0; retry < 5; retry++){
        //reset mcu
        semi_touch_reset();
        
        //hold mcu
        reg_value = 0x12044000;
        ret = semi_touch_write_bytes(0x40007000, (uint8_t*)&reg_value, 4);
        if(ret != 0){
            continue;
        }

        //open auto feed
        reg_value = 0x0000925a;
        ret = semi_touch_write_bytes(0x40007010, (uint8_t*)&reg_value, 4);
        if(ret != 0){
            continue;
        }

        //run ramcode
        ret = semi_touch_write_and_check(0x20000000, (uint8_t* )bin_code, len);
        if(ret != 0){
            continue;
        }

        break;
    }

    if(ret != 0){
        return -1;
    }

    //remap
    reg_value = 0x12044002;
    ret = semi_touch_write_bytes(0x40007000, (uint8_t*)&reg_value, 4);
    if(ret != 0){
        return -1;
    }

    //release mcu
    reg_value = 0x12044003;
    ret = semi_touch_write_bytes(0x40007000, (uint8_t*)&reg_value, 4);
    if(ret != 0){
        return -1;
    }

    semi_touch_msdelay(30);

    return 0;
}

static uint16_t caculate_checksum_u16(uint16_t *buf, uint16_t length)
{
    uint16_t sum, len, i;

    sum = 0;

    len = length >> 1;

    for (i = 0; i < len; i++) {
        sum += buf[i];
    }

    return sum;
}

static uint32_t caculate_checksum_ex(uint8_t * buf, uint16_t length)
{
    uint32_t combchk = 0;
    uint16_t k = 0, check = 0, checkex = 0;
    
    for (k = 0; k < length; k++) {
        check   += buf[k];
        checkex += (uint16_t)(k * buf[k]);
    }

    combchk = (checkex<<16) | check;

    return combchk;
}

static int32_t cmd_send_to_tp(struct m_ctp_cmd_std_t *ptr_cmd, struct m_ctp_rsp_std_t *ptr_rsp, 
                                    int32_t once_delay, int32_t poolgap)
{
    int32_t ret = -SEMI_DRV_ERR_HAL_IO;
    uint32_t retry = 0;
    uint32_t cmd_rsp_ok = 0;

    ptr_cmd->tag = 0xE9;
    ptr_cmd->chk = 1 + ~caculate_checksum_u16((uint16_t*)&ptr_cmd->d0, sizeof(struct m_ctp_cmd_std_t) - 2);
    ret = semi_touch_write_bytes(0x20000000, (uint8_t*)ptr_cmd, sizeof(struct m_ctp_cmd_std_t));
    if(ret != 0){   // TODO: need confirm!!!
        return -1;
    }

    semi_touch_msdelay(once_delay);
    while(retry++ < 20){
        semi_touch_msdelay(poolgap);
        ret = semi_touch_read_bytes(0x20000000, (uint8_t*)ptr_rsp, sizeof(struct m_ctp_rsp_std_t));
        if(ret != 0){   // TODO: need confirm!!!
            return -1;
        }

        if(ptr_cmd->id != ptr_rsp->id){
            continue;
        }

        if(!caculate_checksum_u16((uint16_t*)ptr_rsp, sizeof(struct m_ctp_rsp_std_t))){
            if(0 == ptr_rsp->cc){      //success
                cmd_rsp_ok = 1;
            }
            break;
        }
    }

    ret = -1;
    if(cmd_rsp_ok == 1){
        ret = SEMI_DRV_ERR_OK;
    }

    return ret;
}

/*
    return:
        0(SEMI_DRV_ERR_OK)   ->success
        others               ->fail
*/
static int32_t semi_touch_nvm_read(uint8_t *pdes, uint32_t adr, uint32_t len)
{
    int32_t ret = -1;
    uint32_t left = len;
    uint32_t local_check, retry;
    struct m_ctp_cmd_std_t cmd_send_tp;
    struct m_ctp_rsp_std_t ack_from_tp;

    cmd_send_tp.id = CMD_MEM_RD;

    while (left) {
        len = (left > 1024) ? 1024 : left;

        cmd_send_tp.d0 = adr & 0xffff;
        cmd_send_tp.d1 = len;
        cmd_send_tp.d2 = 0;
        cmd_send_tp.d3 = NVM_R;
        cmd_send_tp.d5 = (adr >> 16) & 0xffff;

        retry = 0;
        while (retry++ < 3) {
            ack_from_tp.id = CMD_NA;
            ret = cmd_send_to_tp(&cmd_send_tp, &ack_from_tp, 20, 10);
            if(SEMI_DRV_ERR_OK != ret){
                continue;
            }

            semi_touch_read_bytes(TP_RD_BUFF_ADDR, pdes, len);

            local_check = caculate_checksum_ex(pdes, len);
            if ((ack_from_tp.d0 != (uint16_t)local_check) || 
                (ack_from_tp.d1 != (uint16_t)(local_check >> 16))){
                ret = -SEMI_DRV_ERR_CHECKSUM;
                continue;
            }
            break;
        }

        adr  += len;
        left -= len;
        pdes += len;
        if(ret != SEMI_DRV_ERR_OK){
            break;
        }
    }

    return ret;
}

static int32_t semi_touch_nvm_write(uint8_t *psrc, uint32_t adr, uint32_t len)
{
    int32_t ret = -1;
    uint32_t left = len;
    uint32_t retry, combChk;
    struct m_ctp_cmd_std_t cmd_send_tp;
    struct m_ctp_rsp_std_t ack_from_tp;

    cmd_send_tp.id = CMD_MEM_WR;

    while (left) {
        len = (left > 1024) ? 1024 : left;
        combChk = caculate_checksum_ex(psrc, len);

        cmd_send_tp.d0 = adr & 0xffff;    /* addrss space[0,64K)  */    
        cmd_send_tp.d1 = len;
        cmd_send_tp.d3 = NVM_W;
        cmd_send_tp.d2 = (uint16_t) combChk;
        cmd_send_tp.d4 = (uint16_t) (combChk >> 16);
        cmd_send_tp.d5 = (adr >> 16) & 0xffff;

        retry = 0;
        while (++retry <= 3) {
            ret = semi_touch_write_bytes(TP_WR_BUFF_ADDR, psrc, len);
            if(SEMI_DRV_ERR_OK != ret) continue;

            ack_from_tp.id = CMD_NA;
            ret = cmd_send_to_tp(&cmd_send_tp, &ack_from_tp, 200, 20);
            if(SEMI_DRV_ERR_OK != ret) continue;

            break;
        }
        
        left -= len;
        adr  += len;
        psrc += len;
        if(ret != SEMI_DRV_ERR_OK){
            break;
        }
    }

    return ret;
}

static int32_t semi_touch_burn_erase(void)
{
    struct m_ctp_cmd_std_t cmd_send_tp;
    struct m_ctp_rsp_std_t ack_from_tp;

    cmd_send_tp.id = CMD_FLASH_ERASE;
    cmd_send_tp.d0 = 0x01;

    return cmd_send_to_tp(&cmd_send_tp, &ack_from_tp, 1000, 40);
}

/*
    This function push IC into NVM mode, call it carefully and must reset 
    IC to enter normal mode.
    return:
        0(SEMI_DRV_ERR_OK)   ->success
        others               ->fail
*/
static int32_t semi_touch_enter_burn_mode(void)
{
    struct m_ctp_cmd_std_t cmd_send_tp;
    struct m_ctp_rsp_std_t ack_from_tp;

    ack_from_tp.d0 = 0;
    cmd_send_tp.id = CMD_IDENTITY;
    cmd_send_to_tp(&cmd_send_tp, &ack_from_tp, 20, 5);
    if((ack_from_tp.d0 == 0xE9A2) && (ack_from_tp.d1 == 0x165d)){
        return SEMI_DRV_ERR_OK;
    }

    if(semi_touch_run_ram_code(fw_5816_burn, sizeof(fw_5816_burn)) != 0){
        return -1;
    }

    cmd_send_tp.id = CMD_IDENTITY;
    if(cmd_send_to_tp(&cmd_send_tp, &ack_from_tp, 20, 5) != 0){
        return -1;
    }
    if((ack_from_tp.d0 == 0xE9A2) && (ack_from_tp.d1 == 0x165d)){
        return SEMI_DRV_ERR_OK;
    }

    return -SEMI_DRV_ERR_HAL_IO;
}


static int32_t semi_get_backup_pid(uint32_t *id)
{
    st_dev.ctp_status = CTP_UPGRAD_RUNING;

    if(semi_touch_enter_burn_mode() != SEMI_DRV_ERR_OK){
        return -1;
    }

    return semi_touch_nvm_read((uint8_t *)id, VID_PID_BACKUP_ADDR, 4);
}

static int32_t semi_touch_update_check(void)
{
    uint32_t pvid;
    uint8_t * pfw;
    uint32_t * plist;
    int32_t k, idx_active;
    uint16_t upd_boot_ver = 0;
    struct chsc_updfile_header *upd_header;

    st_dev.needUpd = 0;
    st_dev.updPdata = 0;
    st_dev.newBootLen = 0;
    if(st_dev.setup_ok == 0){
        if(semi_get_backup_pid(&pvid) == 0){
            st_dev.vid_pid = pvid;
        }
    }

    if((uint32_t)chsc_upd_data & 3){
        PRINT_DBG("chsc::illegal memory buffer, must 4B aliged\n");
        return -SEMI_DRV_INVALID_PARAM;
    }

    upd_header = (struct chsc_updfile_header *)chsc_upd_data;

    if((upd_header->sig != 0x43534843) || (upd_header->n_match == 0)) {
        PRINT_DBG("chsc::illegal upd_header\n");
        return -SEMI_DRV_ERR_NOT_MATCH;
    }

    if((upd_header->len_boot <= 15*1024) || (upd_header->len_boot >= 40*1024)) {
        PRINT_DBG("chsc::illegal upd_header\n");
        return -SEMI_DRV_ERR_NOT_MATCH;
    }

    plist = (uint32_t *)((uint8_t *)chsc_upd_data + sizeof(struct chsc_updfile_header));
    pfw   = (uint8_t *)plist + (upd_header->n_match*4) + upd_header->len_cfg;

    if((pfw[0x30] != 0x16) || (pfw[0x31] != 0x58)){
        PRINT_DBG("chsc:no chsc5816 fw found\n");	
        return -SEMI_DRV_ERR_NOT_MATCH;
    }
    
    st_dev.updPdata = pfw;
    st_dev.newBootLen = upd_header->len_boot;

    idx_active = -1;
    upd_boot_ver = (pfw[0x3f] << 8) + pfw[0x3e];
    for (k=0; k<upd_header->n_match; k++) {
        pvid = plist[k];
        PRINT_DBG("chsc::pid_vid in list=0x%x\n", pvid);	
        if ((pvid & PID_VID_MASK) == (st_dev.vid_pid & PID_VID_MASK)) {
            PRINT_DBG("chsc::running_ver=%d,  upd_ver=%d\n", st_dev.fw_ver, upd_boot_ver);	
            if((st_dev.fw_ver < upd_boot_ver) || (st_dev.setup_ok == 0)){
                idx_active = k;
            }
            break;
        }
    }

    if((st_dev.setup_ok == 0) && (idx_active < 0)){
        idx_active = 0;
    }

    if(idx_active >= 0){
        st_dev.needUpd = 1;
    }

    return SEMI_DRV_ERR_OK;
}

/*
    return:
        0   ->success
    others  ->fail

*/
static int32_t semi_touch_update(uint8_t *pdata, uint32_t len)
{
    if((pdata == NULL) || (len<1024) || (len>0x9f00)){
        PRINT_DBG("chsc:semi_touch_update, not chsc5816 fw\n");	
        return -1;
    }

    if((pdata[0x30] != 0x16) || (pdata[0x31] != 0x58)){
        PRINT_DBG("chsc:semi_touch_update, not chsc5816 fw\n");	
        return -1;
    }

    if(semi_touch_enter_burn_mode() != 0){
        //kal_prompt_trace(MOD_WAP,"chsc::semi_touch_enter_burn_mode fail\n");	
        return -1;
    }

    if(semi_touch_burn_erase() != 0){
        PRINT_DBG("chsc::semi_touch_burn_erase fail\n");	
        return -1;
    }

    if(semi_touch_nvm_write(pdata, 0x00000000, len) != 0){
        PRINT_DBG("chsc::semi_touch_nvm_write fail\n");
        return -1;
    }

    return SEMI_DRV_ERR_OK;
}

static void semi_touch_setup_check(void)
{
    int32_t retry = 0;
    uint32_t naFlag = 0;
    img_header_t image_header;
    img_header_t image_confirm;

    //clean boot status
    semi_touch_write_bytes(0x20000018, (uint8_t*)&naFlag, 4);

    semi_touch_reset();

    st_dev.fw_ver    = 0;
    st_dev.vid_pid   = 0;
    st_dev.setup_ok  = 0;//default error

    image_header.sig = 0;
    for(retry=0; retry<10; retry++){
        semi_touch_msdelay(10);

        if(semi_touch_read_bytes(0x20000014, (uint8_t*)&image_header, sizeof(image_header)) != 0){
            continue;
        }

        if(semi_touch_read_bytes(0x20000014, (uint8_t*)&image_confirm, sizeof(image_confirm)) != 0){
            continue;
        }
        if((image_header.sig != image_confirm.sig) ||
            (image_header.vid_pid != image_confirm.vid_pid) ||
            (image_header.raw_offet != image_confirm.raw_offet) ||
            (image_header.dif_offet != image_confirm.dif_offet) ||
            (image_header.fw_ver != image_confirm.fw_ver)){
            //kal_prompt_trace(MOD_WAP,"chsc::double check, retry\n");
            continue;
        }

        if(image_header.sig == 0x43534843){ //"CHSC"
            st_dev.fw_ver   = image_header.fw_ver;
            st_dev.vid_pid  = image_header.vid_pid;
            st_dev.raw_adr  = image_confirm.raw_offet + 0x20000000;
            st_dev.setup_ok = 1;//pass
            st_dev.ctp_status = CTP_POINTING_WORK;
            break;
        }else if(image_header.sig == 0x4F525245){ //boot self check fail
            break;
        }
    }

    return;
}

int32_t semi_touch_dect(void)
{
    uint32_t u32Data, retry;

    for(retry=0; retry<3; retry++){
        semi_touch_reset();

        if(!semi_touch_read_bytes(0x20000000, (uint8_t *)&u32Data, 4)){  
            return 0;
        }
    }
    return -1;
}

void semi_touch_irq_handler_imp(void)
{
#define _UI_MAX_POINTS   1
#define GEST_CODE_ACT_LS	0x10 //left
#define GEST_CODE_ACT_RS	0x20 //right
#define GEST_CODE_ACT_US	0x30 //up
#define GEST_CODE_ACT_DS	0x40 //down

    int      pointed = 0;
    union rpt_point_t* ppt;
    unsigned char gestCode;
    unsigned char data[8];
    int       x, y;
    PRINT_DBG("semi_touch_irq_handler_imp\r\n");

    if(semi_touch_read_bytes(0x2000002c, data, 8)){
        PRINT_DBG("chsc:read pixel data fail\n" );
        return;
    }
		
    PRINT_DBG("imp = %x %x\r\n",data[0], data[1]);

    pointed = 0;
    ppt = (union rpt_point_t*)&data[2];
    if((data[0] == 0xff) && (data[1] <= 2)){
        if(data[1] > 0){
            pointed = 1;
            x = (unsigned int)(ppt->rp.x_h4 << 8) | ppt->rp.x_l8;
            y = (unsigned int)(ppt->rp.y_h4 << 8) | ppt->rp.y_l8;
        }
    }else{
        return;
    }
}

bool semi_touch_read_coordinate(int16_t *x, int16_t *y)
{
    int      pointed = 0;
    union rpt_point_t* ppt;
    unsigned char data[8];

    if(semi_touch_read_bytes(0x2000002c, data, 8)){
        PRINT_DBG("chsc:read pixel data fail\n" );
        return 0;
    }

    pointed = 0;
    ppt = (union rpt_point_t*)&data[2];
    if((data[0] == 0xff) && (data[1] <= 2)){
        if(data[1] > 0){
            pointed = 1;
            *x = (unsigned int)(ppt->rp.x_h4 << 8) | ppt->rp.x_l8;
            *y = (unsigned int)(ppt->rp.y_h4 << 8) | ppt->rp.y_l8;
            PRINT_DBG("x = %d y = %d \r\n",*x, *y);
        }
    }

    return pointed;
}

int semi_touch_init()
{
    semi_touch_power_int();

    if(semi_touch_dect() != SEMI_DRV_ERR_OK){
        PRINT_DBG("chsc:no chsc5816\r\n" );
        return -1;
    }

    semi_touch_setup_check();

    semi_touch_update_check();

    semi_touch_reset();
    st_dev.ctp_status = CTP_POINTING_WORK;

    PRINT_DBG("chsc init ok\r\n" );

    return 0;
}
