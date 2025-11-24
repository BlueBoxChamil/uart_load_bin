
#include "ext_flash.h"
#include "driver_gpio.h"
#include "driver_spi.h"
#include "driver_dma.h"
#include "IC_W25Qxx.h"
#include <stdint.h>


extern SPI_HandleTypeDef spi_flash_handle;
extern DMA_HandleTypeDef dma_flash_handle;


void ext_flash_gpio_init(void)
{
    /* ========================================================== */
    /* =========  External Flash interface configuration ======== */
    /* ========================================================== */
    GPIO_InitTypeDef gpio_config;

    /* config GPIO for external flash */
    gpio_config.Pin       = GPIO_PIN_8 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13;
    gpio_config.Mode      = GPIO_MODE_AF_PP;
    gpio_config.Pull      = GPIO_PULLUP;
    gpio_config.Alternate = GPIO_FUNCTION_7;
    gpio_init(GPIOC, &gpio_config);

    /* CS of external flash is controllerd by software */
    __SYSTEM_GPIOC_CLK_ENABLE();
    gpio_config.Pin       = GPIO_PIN_9;
    gpio_config.Mode      = GPIO_MODE_OUTPUT_PP;
    gpio_config.Pull      = GPIO_PULLUP;
    gpio_config.Alternate = GPIO_FUNCTION_0;
    gpio_init(GPIOC, &gpio_config);
}

void ext_flash_dma_init(void)
{
    /* config DMA0 for external flash */
    __SYSTEM_DMA0_CLK_ENABLE();
    dma_flash_handle.DMAx                       = DMA0;
    dma_flash_handle.Channel                    = DMA_Channel0;
    dma_flash_handle.Init.Data_Flow             = DMA_P2M_DMAC;
    dma_flash_handle.Init.Request_ID            = 2;
    system_dmac_request_id_config(SPIMX8_1_RX, DMA0_REQUEST_ID_2);
    dma_flash_handle.Init.Source_Master_Sel     = DMA_AHB_MASTER_1;
    dma_flash_handle.Init.Desination_Master_Sel = DMA_AHB_MASTER_4;
    dma_flash_handle.Init.Source_Inc            = DMA_ADDR_INC_NO_CHANGE;
    dma_flash_handle.Init.Desination_Inc        = DMA_ADDR_INC_INC;
    dma_flash_handle.Init.Source_Width          = DMA_TRANSFER_WIDTH_32;
    dma_flash_handle.Init.Desination_Width      = DMA_TRANSFER_WIDTH_32;
    dma_flash_handle.Init.Source_Burst_Len      = DMA_BURST_LEN_4;
    dma_flash_handle.Init.Desination_Burst_Len  = DMA_BURST_LEN_4;
    dma_init(&dma_flash_handle);
}

void ext_flash_controler_init(void)
{
    /* Initial SPIx8_1 for extern flash */
    __SYSTEM_SPI_MASTER1_X8_CLK_ENABLE();
    spi_flash_handle.SPIx                       = SPIMX8_1;
    spi_flash_handle.Init.Work_Mode             = SPI_WORK_MODE_3;
    spi_flash_handle.Init.Frame_Size            = SPI_FRAME_SIZE_8BIT;
    spi_flash_handle.Init.BaudRate_Prescaler    = 2;
    spi_flash_handle.Init.TxFIFOEmpty_Threshold = 20;
    spi_flash_handle.Init.RxFIFOFull_Threshold  = 0;
    spi_master_init(&spi_flash_handle);
//    __SPI_RX_SAMPLE_DLY(spi_flash_handle.SPIx, 2);
    spi_flash_cs_set();

    IC_W25Qxx_QuadConfig(true);

    NVIC_EnableIRQ(DMA0_IRQn);
    NVIC_EnableIRQ(SPIMX8_1_IRQn);
}

void ext_flash_device_init(void)
{
    ext_flash_gpio_init();
    ext_flash_dma_init();
    ext_flash_controler_init();
}

uint32_t ext_flash_get_id(void)
{
    ext_flash_device_init();
    return IC_W25Qxx_Read_ID();
}

// void spi_flash_cs_set(void)
// {
//     gpio_write_pin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
// }

// void spi_flash_cs_clear(void)
// {
//     gpio_write_pin(GPIOC, GPIO_PIN_9, GPIO_PIN_CLEAR);
// }

void ext_flash_erase(uint32_t addr, uint32_t len)
{
    for (int i = 0; i < len; i += 4096)
    {
        IC_W25Qxx_EraseSector(addr + i);
    }
}

void ext_flash_chip_erase(void)
{

}

void ext_flash_protect_enable(void)
{

}

void ext_flash_protect_disable(void)
{

}

uint8_t ext_flash_read(uint32_t addr, int len,uint8_t* buffer)
{
    IC_W25Qxx_Read_Data(buffer, addr, len);
    return 0;
}

uint8_t ext_flash_write(uint32_t addr, int len,uint8_t* buffer)
{
    IC_W25Qxx_PageProgram(buffer, addr, len);
    return 0;
}
