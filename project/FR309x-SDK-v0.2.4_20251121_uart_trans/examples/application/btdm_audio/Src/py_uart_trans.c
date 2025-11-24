#include "py_uart_trans.h"

UART_HandleTypeDef Uart2_handle;
static uint8_t app_py_recv_char;
static uint16_t py_uart_recv_len = 0;
static py_uart_t py_uart;
static uint8_t py_uart_buf[MAX_PY_UART_BUF_SIZE];
const char file_name_str[10] = "name:";
const char file_size_str[10] = "size:";
static uint32_t py_now_write_offset = 0;
static TimerHandle_t py_timeout_timer;

static const crc32_table[] =
    {
        0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f,
        0xe963a535, 0x9e6495a3, 0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
        0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2,
        0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
        0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9,
        0xfa0f3d63, 0x8d080df5, 0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
        0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b, 0x35b5a8fa, 0x42b2986c,
        0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
        0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423,
        0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
        0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d, 0x76dc4190, 0x01db7106,
        0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
        0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d,
        0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
        0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
        0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
        0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7,
        0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
        0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa,
        0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
        0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
        0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
        0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84,
        0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
        0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb,
        0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
        0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e,
        0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
        0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55,
        0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
        0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28,
        0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
        0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f,
        0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
        0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
        0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
        0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69,
        0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
        0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc,
        0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
        0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693,
        0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
        0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d};

/**
 * @brief 下载串口初始化
 * 
 */
void py_uart_trans_init(void)
{

    GPIO_InitTypeDef gpio_config;
    /* configure PD0 and PD1 to UART4 function */
    __SYSTEM_GPIOB_CLK_ENABLE();
    gpio_config.Pin = GPIO_PIN_0 | GPIO_PIN_1;
    gpio_config.Mode = GPIO_MODE_AF_PP;
    gpio_config.Pull = GPIO_PULLUP;
    gpio_config.Alternate = GPIO_FUNCTION_1;
    gpio_init(GPIOB, &gpio_config);

    /* UART4: used for loading pydic */
    __SYSTEM_UART2_CLK_ENABLE();
    Uart2_handle.UARTx = UART2;
    Uart2_handle.Init.BaudRate = 115200;
    Uart2_handle.Init.DataLength = UART_DATA_LENGTH_8BIT;
    Uart2_handle.Init.StopBits = UART_STOPBITS_1;
    Uart2_handle.Init.Parity = UART_PARITY_NONE;
    Uart2_handle.Init.FIFO_Mode = UART_FIFO_ENABLE;
    Uart2_handle.TxCpltCallback = NULL;
    Uart2_handle.RxCpltCallback = app_py_rx_done;
    uart_init(&Uart2_handle);

    NVIC_EnableIRQ(UART2_IRQn);
    NVIC_SetPriority(UART2_IRQn, 4);

    printf("Initialize PD0 and PD1 as the UART downloading py dictionary\r\n");

    app_py_init(&Uart2_handle);
    py_timeout_timer = xTimerCreate("py_timeout_timer", pdMS_TO_TICKS(1000*3), pdFALSE, NULL, py_timeout_timer_func);

    py_uart.type = 0xA0;
    py_uart.pack_id = 0;
    py_uart.len = 0;
    py_uart.crc = 0;
    py_uart.data = py_uart_buf;

    py_uart_recv_len = 0;
    py_now_write_offset = 0;
}

/**
 * @brief 处理接收到的一帧串口数据
 * 
 * @param ptr 
 */
void py_uart_receive(py_uart_t *ptr)
{
    xTimerStart(py_timeout_timer, 0);
    ptr = &py_uart;
    // printf("ptr->type = %02x\r\n", ptr->type);
    // printf("ptr->pack_id = %d\r\n", ptr->pack_id);
    // printf("ptr->len = %d\r\n", ptr->len);
    // printf("ptr->crc = %08x\r\n", ptr->crc);
    // if (ptr->len > 0 && ptr->data != NULL)
    // {
    //     printf("ptr->data: ");
    //     for (uint16_t i = 0; i < ptr->len; i++)
    //     {
    //         printf("%02x ", ptr->data[i]);
    //     }
    //     printf("\r\n");
    // }

    uint32_t crc_res = py_uart_crc32(ptr);
    // printf("Calculated CRC32: %08x\r\n", crc_res);
    if (crc_res != ptr->crc)
    {
        printf("error crc check = %08x,correct = %08x\r\n", crc_res, ptr->crc);
        if (ptr->error_times < MAX_ERROR_TIMES)
        {
            ptr->error_times++;
            py_uart_send("try_again");
        }
        return;
    }
    else
    {
        ptr->error_times = 0;
    }

    // 处理数据
    switch (ptr->type)
    {
    // 获取文件名称和大小
    case 0xA0:
    {
        char *p1 = strstr((char *)ptr->data, file_name_str);
        char *p2 = strstr((char *)(ptr->data + strlen(p1) + 1), file_size_str);
        // printf("p1 = %s\r\n", p1);
        // printf("p2 = %s\r\n", p2);

        // 获取串口文件名称（字符串）
        uint8_t name_len = strlen(p1) - strlen(file_name_str);
        memcpy(ptr->file_name, p1 + strlen(file_name_str), name_len + 1);

        // 获取串口文件大小（数字）
        uint8_t size_len = strlen(p2) - strlen(file_size_str);
        uint8_t tem_str[16];
        memcpy(tem_str, p2 + strlen(file_size_str), size_len + 1);
        ptr->file_size = ascii_strn2val(tem_str, 10, size_len);

        printf("Received file name: %s\r\n", ptr->file_name);
        printf("Received file size: %d bytes\r\n", ptr->file_size);

        py_uart_send("ready");
    }
    break;

    // 处理数据
    case 0xA1:
    {
        uint8_t load_count = ptr->len / 256;
        uint8_t load_remain = ptr->len % 256;
        for (uint8_t i = 0; i < load_count; i++)
        {
            py_uart_flash_write(&(ptr->data[i * 256]), 256, py_now_write_offset);
            py_now_write_offset += 256;
        }

        if (load_remain > 0)
        {
            py_uart_flash_write(&(ptr->data[load_count * 256]), load_remain, py_now_write_offset);
            py_now_write_offset += load_remain;
        }
        printf("Current write offset: %d\r\n", py_now_write_offset);

        if (py_now_write_offset < ptr->file_size)
        {
            py_uart_send("continue");
        }
        else
        {
            py_uart_send("finish");
            py_now_write_offset = 0;
            xTimerStop(py_timeout_timer, 0);
        }
    }
    break;

    default:
        break;
    }
}

static void py_timeout_timer_func()
{
    printf("Timeout No data received, exit");
   
    py_uart_recv_len = 0;
    py_now_write_offset = 0;

    py_uart_send("time_out");
}


static void py_uart_flash_write(uint8_t *data, uint16_t len, uint32_t offset)
{
    // 写入flash
    system_delay_us(1000); // 模拟写入
}

static void py_uart_flash_read(uint8_t *data, uint16_t len, uint32_t offset)
{
    // 读取flash
    system_delay_us(1000); // 模拟读取
}

/**
 * @brief 向串口发送vt字典相关的信息
 *
 * @param str
 */
static void py_uart_send(char *str)
{
    uint8_t send_data[20];
    uint8_t str_len = strlen(str);
    memcpy(send_data, str, str_len);
    send_data[str_len] = '\n';
    uart_transmit(uart2Handler(), send_data, str_len + 1);
}

static uint32_t py_uart_crc32(py_uart_t *ptr)
{
    uint32_t crc = 0xFFFFFFFF; // 初始值为全 1

    crc = crc32_compute(ptr, 2, crc);

    uint8_t len_bytes[2];
    len_bytes[0] = (uint8_t)((ptr->len >> 8) & 0xFF);
    len_bytes[1] = (uint8_t)((ptr->len >> 0) & 0xFF);
    crc = crc32_compute(len_bytes, 2, crc);

    crc = crc32_compute(ptr->data, ptr->len, crc);
    return crc ^ 0xFFFFFFFF; // 最终结果取反
}

static uint32_t crc32_compute(const uint8_t *buf, size_t len, uint32_t crc)
{
    for (size_t i = 0; i < len; i++)
    {
        // printf("crc recive data %02x\r\n", buf[i]);
        uint8_t index = (uint8_t)((crc ^ buf[i]) & 0xFF);
        crc = (crc >> 8) ^ crc32_table[index];
    }
    return crc;
}

static void app_py_recv_c(uint8_t c)
{
    // printf("get uart4 = %x,LEN = %d\r\n", c, py_uart_recv_len);
    py_uart_recv_len++;

    // 获取帧头
    if (py_uart_recv_len == 1)
    {
        py_uart.type = c;
    }
    // 获取数据长度
    else if (py_uart_recv_len == 2)
    {
        py_uart.pack_id = c;
    }
    // 获取数据长度,2字节
    else if (py_uart_recv_len <= 4)
    {
        py_uart.len = ((py_uart.len << 8) | c) & 0xFFFF;
    }
    // 开始获取数据
    else if (py_uart_recv_len <= (py_uart.len + 4))
    {
        py_uart.data[py_uart_recv_len - 5] = c;
    }
    // 获取crc校验码，4字节
    else
    {
        py_uart.crc = ((py_uart.crc << 8) | c) & 0xFFFFFFFF;

        // 已经收到一帧数据，开始处理
        if (py_uart_recv_len == (py_uart.len + 8))
        {
            struct app_task_event *event;
            event = app_task_event_alloc(APP_TASK_EVENT_PY_TRANS, 1, false);
            if (event)
            {
                memcpy(event->param, &py_uart, 1);
                app_task_event_post(event, false);
            }
            py_uart_recv_len = 0;
        }
    }
}

static void app_py_rx_done(struct __UART_HandleTypeDef *handle)
{
    app_py_recv_c(app_py_recv_char);
    if (handle)
    {
        uart_receive_IT(handle, &app_py_recv_char, 1);
    }
}

static void app_py_init(struct __UART_HandleTypeDef *handle)
{
    uart_receive_IT(handle, &app_py_recv_char, 1);
}

UART_HandleTypeDef *uart2Handler()
{
    return &Uart2_handle;
}

void uart2_irq(void)
{
    uart_IRQHandler(&Uart2_handle);
}