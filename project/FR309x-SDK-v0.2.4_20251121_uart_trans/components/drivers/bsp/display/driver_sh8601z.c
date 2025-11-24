#include "driver_display.h"
#include "driver_sh8601z.h"

#define SH8601A_MAX_PARA_COUNT							(300)

#define SH8601A_QSPI_INST_CMD_WRITE						(0x02)
#define SH8601A_QSPI_INST_CMD_READ						(0x03)
#define SH8601A_QSPI_INST_1WIRE_PIXEL_WRITE				(0x02)
#define SH8601A_QSPI_INST_4WIRE_PIXEL_WRITE_TYPE1		(0x32)
#define SH8601A_QSPI_INST_4WIRE_PIXEL_WRITE_TYPE2		(0x12)
#define SH8601A_QSPI_SEQ_FINISH_CODE					(0x00)

typedef struct _SH8601A_CMD_DESC {
    uint8_t instruction;
    uint8_t index;
    uint16_t delay;
    uint16_t wordcount;
    uint8_t  payload[SH8601A_MAX_PARA_COUNT];
} SH8601A_CMD_DESC;

static const SH8601A_CMD_DESC SH8601A_PRE_OTP_POWERON_SEQ_CMD[] = {
    {SH8601A_QSPI_INST_CMD_WRITE, 0x11, 10, 0, {0}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0x2A, 1, 4, {0x00, 0x00, 0x01, 0x6F}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0x2B, 1, 4, {0x00, 0x00, 0x01, 0xBF}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0x44, 1, 2, {0x01, 0xBF}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0x3A, 1, 1, {0x05}},      // 16bits pixel
    {SH8601A_QSPI_INST_CMD_WRITE, 0x35, 1, 1, {0x00}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0x53, 25, 1, {0x20}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0x29, 1, 0, {0}},

//    {SH8601A_QSPI_INST_CMD_WRITE, 0x51, 1, 1, {0xFF}},

    {SH8601A_QSPI_SEQ_FINISH_CODE,	0,	0,	0,	{0}},
};

static const SH8601A_CMD_DESC SH8601A_POST_OTP_POWERON_SEQ_CMD[] = {
//    {SH8601A_QSPI_INST_CMD_WRITE, 0x11,	10,	0,	{0}},
//    {SH8601A_QSPI_INST_CMD_WRITE, 0x2A,	1,	4,	{0x00, 0x00, 0x01, 0xC5}},
//    {SH8601A_QSPI_INST_CMD_WRITE, 0x2B,	1,	4,	{0x00, 0x00, 0x01, 0xC5}},
//    {SH8601A_QSPI_INST_CMD_WRITE, 0x44,	1,	2,	{0x01, 0xC2}},
//    {SH8601A_QSPI_INST_CMD_WRITE, 0x35,	1,	1,	{0x00}},
//    {SH8601A_QSPI_INST_CMD_WRITE, 0x51,	1,	1,	{0xFF}},
//    {SH8601A_QSPI_INST_CMD_WRITE, 0x4A,	1,	1,	{0xFF}},
//    {SH8601A_QSPI_INST_CMD_WRITE, 0x63,	1,	1,	{0xFF}},
//    {SH8601A_QSPI_INST_CMD_WRITE, 0x53, 1, 1, {0x28}},
//	  {SH8601A_QSPI_INST_CMD_WRITE, 0xC4, 25, 1, {0x84}},
//    {SH8601A_QSPI_INST_CMD_WRITE, 0x29,	1,	0,	{0}},
//    {SH8601A_QSPI_SEQ_FINISH_CODE,	0,	0,	0,	{0}},

//    {SH8601A_QSPI_INST_CMD_WRITE, 0xC0, 1,  2,  {0x5A, 0x5A}},
//    {SH8601A_QSPI_INST_CMD_WRITE, 0xC1, 1,  2,  {0x5A, 0x5A}},
//    {SH8601A_QSPI_INST_CMD_WRITE, 0x11, 10, 0,  {0}},

//    {SH8601A_QSPI_INST_CMD_WRITE, 0xB1, 1, 51, {0xC0, 0xC6, 0x01, 0xC6, 0x01, 0x05, 0x00, 0x05, 0x00, 0x2B, 0x01, 0x2B, 0x01, 0x05, 0x00, 0x05, 0x00, 0x2B, 0x01, 0x2B, 0x01, 0x00, 0x52, 0x00, 0x64, 0x00, 0x8A, 0x00, 0xB0, 0x00, 0x52, 0x00, 0x64, 0x00, 0x8A, 0x00, 0xB0, 0x00, 0x00, 0x10, 0x00, 0x00, 0xDF, 0x01, 0x00, 0x00, 0xDF, 0x01, 0x00, 0x00, 0x00}},
//    {SH8601A_QSPI_INST_CMD_WRITE, 0xB4, 1, 65, {0x09, 0x02, 0x00, 0x00, 0x10, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x2C, 0x47, 0x47, 0x47, 0x47, 0x47, 0x47, 0x47, 0x47, 0x47, 0x47, 0x47, 0x47, 0x02, 0x46, 0x8A, 0x13, 0x57, 0x9B, 0x31, 0x75, 0xB9, 0x20, 0x64, 0xA8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x64, 0xA8, 0x31, 0x75, 0xB9, 0x13, 0x57, 0x9B, 0x02, 0x46, 0x8A, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00}},
//    {SH8601A_QSPI_INST_CMD_WRITE, 0xB5, 1, 58, {0x4C, 0x09, 0x09, 0x09, 0x49, 0x40, 0x00, 0x01, 0x2C, 0x00, 0x00, 0x00, 0x2C, 0x00, 0x1C, 0x04, 0x2C, 0x00, 0x1C, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0x00, 0x00, 0x03, 0x02, 0x6E, 0x00, 0x2C, 0x00, 0x2C, 0x00, 0x1C, 0x04, 0x00, 0x00, 0x00, 0x00, 0x2C, 0x00, 0x1C, 0x04, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x03, 0x00}},
//    {SH8601A_QSPI_INST_CMD_WRITE, 0xB6, 1, 26, {0x00, 0x10, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x2C, 0x47, 0x47, 0x47, 0x47, 0x47, 0x47, 0x47, 0x47, 0x47, 0x47, 0x47, 0x47}},
//    {SH8601A_QSPI_INST_CMD_WRITE, 0xB7, 1, 51, {0x0C, 0x00, 0x01, 0x2C, 0x00, 0x00, 0x00, 0x2C, 0x00, 0x1C, 0x04, 0x2C, 0x00, 0x1C, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0x00, 0x00, 0x03, 0x02, 0x6E, 0x00, 0x2C, 0x00, 0x2C, 0x00, 0x1C, 0x04, 0x00, 0x00, 0x00, 0x00, 0x2C, 0x00, 0x1C, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
//    {SH8601A_QSPI_INST_CMD_WRITE, 0xB8, 1, 95, {0x00, 0x67, 0x31, 0x22, 0x22, 0x22, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x22, 0x22, 0x22, 0x00, 0x00, 0x00, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x00, 0x00, 0x00, 0x22, 0x22, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x22, 0x00, 0x22, 0x00, 0x22, 0x22, 0x22, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x22, 0x22, 0x22, 0x00, 0x00, 0x00, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x00, 0x00, 0x00, 0x22, 0x22, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x22, 0x00, 0x22, 0x00, 0x00, 0x00}},

//    {SH8601A_QSPI_INST_CMD_WRITE, 0x2A, 1,  4,  {0x00, 0x00, 0x01, 0xC5}},
//    {SH8601A_QSPI_INST_CMD_WRITE, 0x2B, 1,  4,  {0x00, 0x00, 0x01, 0xC5}},
//    {SH8601A_QSPI_INST_CMD_WRITE, 0x44, 1,  2,  {0x01, 0xC2}},
//    {SH8601A_QSPI_INST_CMD_WRITE, 0x35, 1,  1,  {0x00}},
//    {SH8601A_QSPI_INST_CMD_WRITE, 0x51, 1,  1,  {0xFF}},
//    {SH8601A_QSPI_INST_CMD_WRITE, 0x4A, 1,  1,  {0xFF}},
//    {SH8601A_QSPI_INST_CMD_WRITE, 0x63, 1,  1,  {0xFF}},
//    {SH8601A_QSPI_INST_CMD_WRITE, 0x53, 1,  1,  {0x28}},
//    {SH8601A_QSPI_INST_CMD_WRITE, 0xC4, 25, 1,  {0x84}},
//    {SH8601A_QSPI_INST_CMD_WRITE, 0x29, 1,  0,  {0}},
//    {SH8601A_QSPI_INST_CMD_WRITE, 0xBA, 1,  1,  {0x80}},         // bist: 0x81,      exit bist: 0x80

//    {SH8601A_QSPI_INST_CMD_WRITE, 0xB1, 1,  1,  {0xC0}},			

//    {SH8601A_QSPI_INST_CMD_WRITE, 0xC1, 1,  2,  {0xA5, 0xA5}},
//    {SH8601A_QSPI_INST_CMD_WRITE, 0xC0, 1,  2,  {0xA5, 0xA5}},

//    {SH8601A_QSPI_SEQ_FINISH_CODE,   0, 0,  0,  {0}},	

    {SH8601A_QSPI_INST_CMD_WRITE, 0xC0, 1,  2,  {0x5A, 0x5A}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0xC1, 1,  2,  {0x5A, 0x5A}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0x11, 10, 0,  {0}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0x2A, 1,  4,  {0x00, 0x00, 0x01, 0xC5}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0x2B, 1,  4,  {0x00, 0x00, 0x01, 0xC5}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0x44, 1,  2,  {0x01, 0xC2}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0x35, 1,  1,  {0x00}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0xB0, 1,  1,  {0x16}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0xB1, 1,  9,  {0x01, 0x05, 0x00, 0xA2, 0x00, 0xA7, 0x00, 0xA7, 0x00}},   // 0x01=45Hz, 0x00=60Hz
    //{SH8601A_QSPI_INST_CMD_WRITE, 0x51, 1,  1,  {0xFF}},
    //{SH8601A_QSPI_INST_CMD_WRITE, 0x4A, 1,  1,  {0xFF}},
    //{SH8601A_QSPI_INST_CMD_WRITE, 0x63, 1,  1,  {0xFF}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0x53, 1,  1,  {0x28}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0xC4, 25, 1,  {0x84}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0x29, 1,  0,  {0}},
    //{SH8601A_QSPI_INST_CMD_WRITE, 0xBA, 1,  1,  {0x80}},         // bist: 0x81,      exit bist: 0x80
    {SH8601A_QSPI_INST_CMD_WRITE, 0xB1, 1,  1,  {0xC0}},

    {SH8601A_QSPI_INST_CMD_WRITE, 0xC0, 1,  2,  {0xA5, 0xA5}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0xC1, 1,  2,  {0xA5, 0xA5}},

    {SH8601A_QSPI_SEQ_FINISH_CODE,   0, 0,  0,  {0}},
};

static const SH8601A_CMD_DESC SH8601A_POWEROFF_SEQ_CMD[] = {
    {SH8601A_QSPI_INST_CMD_WRITE, 0x28,	15,	0,	{0}},
	{SH8601A_QSPI_INST_CMD_WRITE, 0x10,	0,	0,	{0}},
    {SH8601A_QSPI_SEQ_FINISH_CODE,	0,	0,	0,	{0}},
};

static const SH8601A_CMD_DESC SH8601A_OTP_WRITE[] = {
	{SH8601A_QSPI_INST_CMD_WRITE, 0xD0,	1000,	2,	{0x01}},
	{SH8601A_QSPI_INST_CMD_WRITE, 0xD0,	10,	2,	{0x00}},
    {SH8601A_QSPI_SEQ_FINISH_CODE,	0,	0,	0,	{0}},
};

#ifdef USE_DMA_LINK_MODE
static DMA_LLI_InitTypeDef *Link_Channel = (void *)0x1fffe000;
#endif

void sh8601z_brightness_adj(void);

static void write_cmd(uint8_t cmd)
{
    uint8_t spi_data[4];

    spi_data[0] = SH8601A_QSPI_INST_CMD_WRITE;
    spi_data[1] = 0x00;
    spi_data[2] = cmd;
    spi_data[3] = 0x00;

    __DISPLAY_CS_CLEAR();
    spi_master_transmit_X1(&spi_display_handle, (uint16_t *)spi_data, 4);
    __DISPLAY_CS_SET();
}

static void write_buff(uint8_t *buffer, uint8_t len)
{
    __DISPLAY_CS_CLEAR();
    spi_master_transmit_X1(&spi_display_handle, (uint16_t *)buffer, len);
    __DISPLAY_CS_SET();
}

static void read_reg(uint8_t reg, uint8_t *buffer, uint16_t len)
{
    uint8_t spi_data[4];

    spi_data[0] = SH8601A_QSPI_INST_CMD_READ;
    spi_data[1] = 0x00;
    spi_data[2] = reg;
    spi_data[3] = 0x00;

    __DISPLAY_CS_CLEAR();
    spi_master_transmit_X1(&spi_display_handle, (uint16_t *)spi_data, 4);
    spi_master_receive_X1(&spi_display_handle, buffer, len);
    __DISPLAY_CS_SET();
}

static void SH8601A_Reg_Write(const SH8601A_CMD_DESC* cmd)
{
    uint16_t idx = 0;	

    while (cmd[idx].instruction != SH8601A_QSPI_SEQ_FINISH_CODE) 
    {
        uint8_t sdat[cmd[idx].wordcount + 4];

        sdat[0] = cmd[idx].instruction;

        sdat[1] = 0;
        sdat[2] = cmd[idx].index;        // Set in the middle 8 bits ADDR[15:8] of the 24 bits ADDR[23:0]
        sdat[3] = 0;

        for(uint16_t i=0; i<cmd[idx].wordcount; i++)
        {
            sdat[i+4] = cmd[idx].payload[i];
        }

        __DISPLAY_CS_CLEAR();
        spi_master_transmit_X1(&spi_display_handle, sdat, sizeof(sdat));
        __DISPLAY_CS_SET();

        if (cmd[idx].delay != 0) 
        {
            __DISPLAY_DELAY_MS(cmd[idx].delay);
        }		

        idx++;
    }
}

static void SH8601A_Init_Pre_OTP(void)                                
{
	//pull low RESX
	//power on VBAT: VBAT = 3.7V
	//power on VDDI: VDDI = 1.8V
	//pull high VCI_EN: enable VCI = 3.3V
	//delay 10ms
	//pull high RESX: IC reset
	//delay 10ms
    SH8601A_Reg_Write(SH8601A_PRE_OTP_POWERON_SEQ_CMD);
}

static void SH8601A_Init_Post_OTP(void)                                
{
	//pull low RESX
	//power on VBAT: VBAT = 3.7V
	//power on VDDI: VDDI = 1.8V
	//pull high VCI_EN: enable VCI = 3.3V
	//delay 10ms
	//pull high RESX: IC reset
	//delay 10ms
    SH8601A_Reg_Write(SH8601A_POST_OTP_POWERON_SEQ_CMD);
}

static void SH8601A_Power_Off(void)                                
{
    SH8601A_Reg_Write(SH8601A_POWEROFF_SEQ_CMD);
	//delay 100ms
	//pull low RESX
	//delay 10ms
	//pull low VCI_EN: disable VCI
	//power off VDDI
	//power off VBAT
}

static void SH8601A_OTP_Write(void)                                
{
	/*********************************************
	* Register read: 
	* Index: 0xCF (OTP_STATUS)
	* Para: 1 Byte read
	* Check BANK_CHECK_MCS[1:0]:
	* - 00h: No writen, 3 times writable
	* - 01h: 1 time written, 2 times writable
	* - 02h: 2 times written, 1 time writable
	* - 03h: 3 time written, no longer be written
	**********************************************/
	
	//power on VOTP: VOTP = 6V external supply
	//delay 20ms	
    SH8601A_Reg_Write(SH8601A_OTP_WRITE);
	//power off VOTP
	//delay 20ms
	
	/*********************************************
	* OTP status verification: 
	* Index: 0xCF (OTP_STATUS)
	* Para: 3 Bytes read
	* Check PRG_ERR_1:
	* - 0: OK
	* - 1: FAIL
		   OTP rewrite(power off -> power on -> rewrite)
	* Check PRG_ERR_0:
	* - 0: OK
	* - 1: FAIL
		   Not rewrite OTP
	**********************************************/
	
	/* Go to power off sequence */
}

static void sh8601a_read_regs(void)
{
    uint8_t buffer[6];

    read_reg(0x90, buffer, 6);
    read_reg(0x2A, buffer, 4);
    read_reg(0x2B, buffer, 4);
}

void sh8601z_init(void)
{
    __DISPLAY_VCI_CLEAR();
    __DISPLAY_RESET_CLEAR();
    __DISPLAY_DELAY_MS(10);
    __DISPLAY_VCI_SET();
    __DISPLAY_DELAY_MS(10);
    __DISPLAY_RESET_SET();
    __DISPLAY_DELAY_MS(10);

    SH8601A_Init_Pre_OTP();
}

void sh8601z_set_window(uint16_t x_s, uint16_t x_e, uint16_t y_s, uint16_t y_e)
{
    uint8_t data[8];

    //BOARD_EVB_FR3092E_CM
    // x_s += 16;
    // x_e += 16;

    data[0] = 0x02;
    data[1] = 0x00;
    data[2] = 0x2A;
    data[3] = 0x00;
    data[4] = x_s >> 8;
    data[5] = x_s & 0xff;
    data[6] = x_e >> 8;
    data[7] = x_e & 0xff;
    write_buff(data, 8);

    data[0] = 0x02;
    data[1] = 0x00;
    data[2] = 0x2B;
    data[3] = 0x00;    
    data[4] = y_s >> 8;
    data[5] = y_s & 0xff;
    data[6] = y_e >> 8;
    data[7] = y_e & 0xff;
    write_buff(data, 8);

//    write_cmd(0x2c);
}

void sh8601z_display(uint32_t pixel_count, uint8_t pixel_width, void *data)
{
    uint8_t frame_size;

    if (pixel_width == 16) {
        frame_size = SPI_FRAME_SIZE_16BIT;
    }
    else if (pixel_width == 32) {
        frame_size = SPI_FRAME_SIZE_24BIT;
    }
    spi_display_handle.Init.Frame_Size              = frame_size;
    spi_display_handle.MultWireParam.Wire_X2X4X8    = Wire_X4;
    spi_display_handle.MultWireParam.InstructLength = INST_8BIT;
    spi_display_handle.MultWireParam.Instruct       = 0x32;
    spi_display_handle.MultWireParam.AddressLength  = ADDR_24BIT;
    spi_display_handle.MultWireParam.Address        = 0x002C00;

    __DISPLAY_CS_CLEAR();
    spi_master_transmit_X2X4X8(&spi_display_handle, data, pixel_count);
    __DISPLAY_CS_SET();

    __SPI_DISABLE(spi_display_handle.SPIx);
    __SPI_DATA_FRAME_SIZE(spi_display_handle.SPIx, SPI_FRAME_SIZE_8BIT);
}

void sh8601z_display_dma(uint32_t pixel_count, uint8_t pixel_width, void *data)
{
//#define USE_DMA_LINK_MODE
    uint8_t spi_trans_width;
    uint32_t dma_sample_count;
    
#ifdef USE_DMA_LINK_MODE
#define DMA_SINGLE_TRANSFER_SIZE       20000
    uint32_t link_count;
    uint32_t i;
    uint32_t link_trans_size;
    dma_LinkParameter_t LinkParameter;
    

    switch (dma_display_handle.Init.Source_Width) {
        case DMA_TRANSFER_WIDTH_32:
            dma_sample_count = pixel_count * pixel_width / 32;
            link_trans_size = 4 * DMA_SINGLE_TRANSFER_SIZE;
            break;
        case DMA_TRANSFER_WIDTH_16:
            dma_sample_count = pixel_count * pixel_width / 16;
            link_trans_size = 2 * DMA_SINGLE_TRANSFER_SIZE;
            break;
        case DMA_TRANSFER_WIDTH_8:
            dma_sample_count = pixel_count * pixel_width / 8;
            link_trans_size = DMA_SINGLE_TRANSFER_SIZE;
            break;
        default:
            return;
    }
    
    link_count = dma_sample_count / DMA_SINGLE_TRANSFER_SIZE;
    if(dma_sample_count % DMA_SINGLE_TRANSFER_SIZE)
    {
        link_count++;
    }
    
    for (i = 0; i < link_count; i++)
    {
        uint8_t all_set = (dma_sample_count <= DMA_SINGLE_TRANSFER_SIZE);
        
        LinkParameter.SrcAddr          = (uint32_t)data + i * link_trans_size;
        LinkParameter.DstAddr          = (uint32_t)&spi_display_handle.SPIx->DR;
        if(all_set)
        {
            LinkParameter.NextLink     = 0;
        }
        else
        {
            LinkParameter.NextLink     = (uint32_t)&Link_Channel[i + 1];
        }
        LinkParameter.Data_Flow             = dma_display_handle.Init.Data_Flow;
        LinkParameter.Request_ID            = dma_display_handle.Init.Request_ID;
        LinkParameter.Source_Master_Sel     = dma_display_handle.Init.Source_Master_Sel;
        LinkParameter.Desination_Master_Sel = dma_display_handle.Init.Desination_Master_Sel;
        LinkParameter.Source_Inc            = dma_display_handle.Init.Source_Inc;
        LinkParameter.Desination_Inc        = dma_display_handle.Init.Desination_Inc;
        LinkParameter.Source_Width          = dma_display_handle.Init.Source_Width;
        LinkParameter.Desination_Width      = dma_display_handle.Init.Desination_Width;
        LinkParameter.Source_Burst_Len      = dma_display_handle.Init.Source_Burst_Len;
        LinkParameter.Desination_Burst_Len  = dma_display_handle.Init.Desination_Burst_Len;
        LinkParameter.Size                  = all_set ? dma_sample_count : DMA_SINGLE_TRANSFER_SIZE;
        LinkParameter.gather_enable         = 0;
        LinkParameter.scatter_enable        = 0;
        dma_sample_count -= DMA_SINGLE_TRANSFER_SIZE;

        dma_linked_list_init(&Link_Channel[i], &LinkParameter);
    }
#else
    switch (dma_display_handle.Init.Source_Width) {
        case DMA_TRANSFER_WIDTH_32:
            dma_sample_count = pixel_count * pixel_width / 32;
            break;
        case DMA_TRANSFER_WIDTH_16:
            dma_sample_count = pixel_count * pixel_width / 16;
            break;
        case DMA_TRANSFER_WIDTH_8:
            dma_sample_count = pixel_count * pixel_width / 8;
            break;
        default:
            return;
    }
#endif

    switch (dma_display_handle.Init.Desination_Width) {
        case DMA_TRANSFER_WIDTH_32:
            spi_trans_width = SPI_FRAME_SIZE_32BIT;
            break;
        case DMA_TRANSFER_WIDTH_16:
            spi_trans_width = SPI_FRAME_SIZE_16BIT;
            break;
        case DMA_TRANSFER_WIDTH_8:
            spi_trans_width = SPI_FRAME_SIZE_8BIT;
            break;
        default:
            return;
    }
    
    spi_display_handle.Init.Frame_Size              = spi_trans_width;
    spi_display_handle.MultWireParam.Wire_X2X4X8    = Wire_X4;
    spi_display_handle.MultWireParam.InstructLength = INST_8BIT;
    spi_display_handle.MultWireParam.Instruct       = 0x32;
    spi_display_handle.MultWireParam.AddressLength  = ADDR_24BIT;
    spi_display_handle.MultWireParam.Address        = 0x002C00;

    __DISPLAY_CS_CLEAR();
    
    __SPI_DISABLE(spi_display_handle.SPIx);
    __SPI_TX_ENDIAN_SET(spi_display_handle.SPIx, TX_RX_Endian_4321);
    __SPI_ENABLE(spi_display_handle.SPIx);
    spi_master_transmit_X2X4X8_DMA(&spi_display_handle);
    
    __SPI_DISABLE(spi_display_handle.SPIx);
    if (spi_trans_width == SPI_FRAME_SIZE_32BIT) {
        __SPI_TX_ENDIAN_SET(spi_display_handle.SPIx, TX_RX_Endian_2143);
    }
    else {
        __SPI_TX_ENDIAN_SET(spi_display_handle.SPIx, TX_RX_Endian_4321);
    }
    __SPI_ENABLE(spi_display_handle.SPIx);
#ifndef USE_DMA_LINK_MODE
    dma_start_IT(&dma_display_handle, (uint32_t)data, (uint32_t)&spi_display_handle.SPIx->DR, dma_sample_count);
#else
    dma_linked_list_start_IT(Link_Channel, &LinkParameter, &dma_display_handle);
#endif
}

void sh8601z_power_off(void)
{
    SH8601A_Power_Off();

    __DISPLAY_DELAY_MS(100);
    __DISPLAY_RESET_CLEAR();
    __DISPLAY_DELAY_MS(10);
    __DISPLAY_VCI_CLEAR();
}

void sh8601z_power_on(void)
{
    __DISPLAY_VCI_CLEAR();
    __DISPLAY_RESET_CLEAR();
    __DISPLAY_DELAY_MS(10);
    __DISPLAY_VCI_SET();
    __DISPLAY_DELAY_MS(10);
    __DISPLAY_RESET_SET();
    __DISPLAY_DELAY_MS(10);

    SH8601A_Init_Pre_OTP();
    sh8601z_brightness_adj();
}

extern uint8_t fr_system_get_backlight(void);
void sh8601z_brightness_adj(void)	//Value 0x00 - 0xFF
{
    uint16_t Value = (fr_system_get_backlight() * 3 >> 1) + 112;
    if(Value > 255)
        Value = 255;
    printf("brightness %d \n",Value);

	//rtl_sh8601z_qspi_cmd_param(0x51, Value);
    //AOT mode
//	rtl_sh8601z_qspi_write_cmd(0x39);
//	rtl_sh8601z_qspi_cmd_param(0x4A, Value);		//brightness 0x00-0xFF

    SH8601A_CMD_DESC SH8601A_BRIGHTNESS_ADJ[] = {
        {SH8601A_QSPI_INST_CMD_WRITE, 0x51, 1,  1,  {0xFF}},
        {SH8601A_QSPI_SEQ_FINISH_CODE,	0,	0,	0,	{0}},
    };
    SH8601A_BRIGHTNESS_ADJ[0].payload[0] = Value;
    SH8601A_Reg_Write(SH8601A_BRIGHTNESS_ADJ);

}

void sh8601z_display_dma_isr(void)
{
    while(__SPI_IS_BUSY(spi_display_handle.SPIx));

    // CS Release
    __DISPLAY_CS_SET();
	
    /* Clear Transfer complete status */
    dma_clear_tfr_Status(&dma_display_handle);
    /* channel Transfer complete interrupt disable */
    dma_tfr_interrupt_disable(&dma_display_handle);

    __SPI_DISABLE(spi_display_handle.SPIx);
    __SPI_TX_ENDIAN_SET(spi_display_handle.SPIx, TX_RX_Endian_4321);
    __SPI_DATA_FRAME_SIZE(spi_display_handle.SPIx, SPI_FRAME_SIZE_8BIT);
}
