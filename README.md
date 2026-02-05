# 串口下载工具

## 功能
 将bin文件通过自定义串口协议下载到单片机的flash中

## 时间
20251124

## 开发板
单片机开发板均可

## 固件
    暂无，按需使用，主要移植串口部分

## 版本
V1：通过电脑将bin下载到单片机的flash中

## 工程

python:

```
20251124_uart_trans
```

单片机: 

```
FR309x-SDK-v0.2.4_20251121_uart_trans
```




## 使用方法

1. 将单片机上的**py_uart_trans.c**和**py_uart_trans.h**移植到需要下载的单片机上，使用的是任务来处理，可以按需修改串口引脚等

2. 将单片机的串口连上，记住不是打印信息的引脚，最好共地

3. 打开 [uart_trans.exe](tool\uart_trans.exe) ，它就是在电脑上下载bin文件的工具。它有两种使用方式，具体可以在cmd控制台中添加后缀 -h来查看

   ```
   D:\document\my_github\串口下载工具\tool>uart_trans.exe -h
   [INFO] config.json 文件不存在，正在创建默认配置文件...
   [INFO] 已创建config.json文件
   [ERROR] 请将需要下载的bin文件放入 D:\document\my_github\串口下载工具\tool\uart_trans_file 文件夹中，然后重新运行程序。
   
   uart_trans - 串口传输工具
   
   用法1:
       uart_trans -h        显示帮助信息
       uart_trans -p COMx   指定串口号
       uart_trans -b 115200 指定波特率
       uart_trans -t 5      指定超时时间
   
   用法2:
       直接配置config.json文件
   
   按任意键或回车退出…
   ```

4. 因此可以选择配置config文件然后直接双击exe使用，也可以在控制台来直接指定参数，例如

   ```
   D:\document\my_github\串口下载工具\tool>uart_trans.exe -p com5 -b 115200 -t 3
   ```

   在这里超时时间是指电脑超过N秒后未收到数据就会结束发送，具体会有说明手册 [串口下载工具说明手册.pdf](document\串口下载工具说明手册.pdf) 

## 更新详情

### V1
1. 做一个通用的串口协议，让单片机可以烧入文件较大的bin文件到flash中
1. 采用ACK的应答方式，防止数据传输过程中，数据处理快或者过慢而导致的数据丢失
1. 单片机端添加擦除响应
1. 每次运行exe都会生成一个新的load_bin.bin，因此下载的bin会跟随更新uart_trans_file更新

## 存在问题
  1. 暂无

## 测试命令行

通过ffmpeg将mp3音频转为sbc音频

> [!tip]
>
> 通过ffmpeg将mp3音频转为sbc音频。sbc音频换后缀就是bin文件
>
> ```
> ffmpeg -i 1.mp3 \
>   -ar 16000 \
>   -ac 1 \
>   -sample_fmt s16 \
>   -acodec sbc \
>   -b:a 48k \
>   11_low.sbc
> ```



> [!tip]
>
> 将sbc文件转为c文件数组
>
> ```
> xxd -i 11.sbc > sbc_data.c
> ```
