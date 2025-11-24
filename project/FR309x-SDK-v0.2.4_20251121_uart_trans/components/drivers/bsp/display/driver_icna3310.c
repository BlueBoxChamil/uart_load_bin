#include "driver_display.h"

static void reg_write(uint8_t addr, uint8_t *value, uint8_t length)
{
    uint8_t sdat[length + 4];
    
    sdat[0] = 0x02;
    sdat[1] = 0x00;
    sdat[2] = addr;
    sdat[3] = 0x00;
    memcpy(&sdat[4], value, length);
    __DISPLAY_CS_CLEAR();
    spi_master_transmit_X1(&spi_display_handle, sdat, sizeof(sdat));
    __DISPLAY_CS_SET();
}

static void reg_read(uint8_t addr, uint8_t *value, uint8_t length)
{
    uint8_t sdat[4];
    
    sdat[0] = 0x03;
    sdat[1] = 0x00;
    sdat[2] = addr;
    sdat[3] = 0x00;
    __DISPLAY_CS_CLEAR();
    spi_master_transmit_X1(&spi_display_handle, sdat, sizeof(sdat));
    spi_master_receive_X1(&spi_display_handle, value, length);
    __DISPLAY_CS_SET();
}

void icna3310_init(void)
{
    uint8_t buffer[4];
    
    __DISPLAY_VCI_CLEAR();
    __DISPLAY_RESET_CLEAR();
    __DISPLAY_DELAY_MS(200);
    __DISPLAY_VCI_SET();
    __DISPLAY_DELAY_MS(50);
    __DISPLAY_RESET_SET();
    __DISPLAY_DELAY_MS(5);
    __DISPLAY_RESET_CLEAR();
    __DISPLAY_DELAY_MS(5);
    __DISPLAY_RESET_SET();
    __DISPLAY_DELAY_MS(5);
    
//    __DISPLAY_VCI_CLEAR();
//    __DISPLAY_RESET_CLEAR();
//    __DISPLAY_DELAY_MS(20);
//    __DISPLAY_RESET_SET();
//    __DISPLAY_DELAY_MS(40);
//    __DISPLAY_VCI_SET();
//    __DISPLAY_DELAY_MS(80);

    buffer[0] = 0x20;
    reg_write(0xFE, &buffer[0], 1);
    buffer[0] = 0x5a;
    reg_write(0xF4, &buffer[0], 1);
    buffer[0] = 0x59;
    reg_write(0xF5, &buffer[0], 1);
    
    buffer[0] = 0x40;
    reg_write(0xFE, &buffer[0], 1);
    buffer[0] = 0x0a;
    reg_write(0x08, &buffer[0], 1);
    
    buffer[0] = 0x00;
    reg_write(0xFE, &buffer[0], 1);    
    buffer[0] = 0x80;
    reg_write(0xC4, &buffer[0], 1);//SPI sram write enable
    buffer[0] = 0x55;
    reg_write(0x3A, &buffer[0], 1);//55 RGB565, 77 RGB888      
    buffer[0] = 0x00;    
    reg_write(0x35, &buffer[0], 1);     
    buffer[0] = 0x20;    
    reg_write(0x53, &buffer[0], 1);
    buffer[0] = 0xFF;
    reg_write(0x51, &buffer[0], 1);
    buffer[0] = 0xFF;
    reg_write(0x63, &buffer[0], 1);
    buffer[0] = 0x00;
    buffer[1] = 0x06;
    buffer[2] = 0x01;
    buffer[3] = 0xD7;
    reg_write(0x2A, &buffer[0], 4);  // paritial update:466RGB    
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x01;
    buffer[3] = 0xD1;
    reg_write(0x2B, &buffer[0], 4);  // partial update:466line
//    buffer[0] = 0x00;
//    reg_write(0xFE, &buffer[0], 1);
    reg_write(0x11, NULL, 0);                                                                 
    __DISPLAY_DELAY_MS(120);
//    buffer[0] = 0x00;
//    reg_write(0xFE, &buffer[0], 1);
    reg_write(0x29, NULL, 0); 
    __DISPLAY_DELAY_MS(50);
}

void icna3310_set_window(uint16_t x_s, uint16_t x_e, uint16_t y_s, uint16_t y_e)
{
    uint8_t data[4];
    
    x_s += 6;
    x_e += 6;

    data[0] = x_s >> 8;
    data[1] = x_s & 0xff;
    data[2] = x_e >> 8;
    data[3] = x_e & 0xff;
    reg_write(0x2A, &data[0], 4);
 
    data[0] = y_s >> 8;
    data[1] = y_s & 0xff;
    data[2] = y_e >> 8;
    data[3] = y_e & 0xff;
    reg_write(0x2B, &data[0], 4);

//    reg_write(0x2C, &data[0], 4);
}

void icna3310_adjust_brightness(uint8_t value)  //Value 0x00 - 0xFF
{
    uint8_t buffer[1];

    buffer[0] = 0x00;
	reg_write(0xFE, &buffer[0], 1);
    buffer[0] = value;
	reg_write(0x51, &buffer[0], 1);
}

void icna3310_display(uint32_t pixel_count, uint8_t pixel_width, void *data)
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

void icna3310_display_dma(uint32_t pixel_count, uint8_t pixel_width, void *data)
{
    uint8_t spi_trans_width;
    uint32_t dma_sample_count;
    
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
    
    if (pixel_width != 32) {
        spi_display_handle.Init.Frame_Size          = spi_trans_width;
    }
    else {
        spi_display_handle.Init.Frame_Size          = SPI_FRAME_SIZE_24BIT;
    }
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
    if ((spi_trans_width == SPI_FRAME_SIZE_32BIT)
            && (pixel_width != 32)) {
        __SPI_TX_ENDIAN_SET(spi_display_handle.SPIx, TX_RX_Endian_2143);
    }
    else {
        __SPI_TX_ENDIAN_SET(spi_display_handle.SPIx, TX_RX_Endian_4321);
    }
    __SPI_ENABLE(spi_display_handle.SPIx);

    dma_start_IT(&dma_display_handle, (uint32_t)data, (uint32_t)&spi_display_handle.SPIx->DR, dma_sample_count);
}

void icna3310_display_dma_isr(void)
{
    while(__SPI_IS_BUSY(spi_display_handle.SPIx));

    // CS Release
    __DISPLAY_CS_SET();
	
    /* Clear Transfer complete status */
    dma_clear_tfr_Status(&dma_display_handle);
    /* channel Transfer complete interrupt disable */
    dma_tfr_interrupt_disable(&dma_display_handle);

    __SPI_DISABLE(spi_display_handle.SPIx);
    __SPI_DATA_FRAME_SIZE(spi_display_handle.SPIx, SPI_FRAME_SIZE_8BIT);
    __SPI_TX_ENDIAN_SET(spi_display_handle.SPIx, TX_RX_Endian_4321);
}
