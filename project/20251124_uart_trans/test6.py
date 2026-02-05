"""
Author: BlueboxChamil
Date: 2025-11-20 14:03:56
LastEditTime: 2025-11-20 14:04:24
FilePath: \test4.py
Description:
Copyright (c) 2025 by BlueboxChamil, All Rights Reserved.
"""

import json
import asyncio
import os
import struct
from dataclasses import field
import binascii
import sys
import serial


# 默认配置模板
default_config = {
    "serial": {
        "port": "COM15",  # 默认串口号
        "baudrate": 115200,  # 默认波特率
        "timeout": 3,  # 默认超时时间，单位：秒
    }
}


send_bin_file = None
bin_send_offset = 0
watchdog_event = None
error_max_count = 0
# 串口
ser = None


class DataPacket:
    header: int = 0xA1  # 固定帧头 uint8_t
    id: int = 0  # uint8_t
    data_len: int = 0  # uint8_t
    payload: bytes = field(default_factory=bytes)  # 字节数组
    checksum: int = 0  # uint8_t

    _next_id = 0  # 仅类内部使用，不暴露给外部

    MAX_PAYLOAD = 1024  # 最大负载长度

    # ------------------ 工具函数 ------------------
    @classmethod
    def _get_next_id(cls):
        cls._next_id = (cls._next_id + 1) & 0xFF  # 保持在0~255
        return cls._next_id

    @staticmethod
    def calculate_crc32(data):
        crc = binascii.crc32(data) & 0xFFFFFFFF
        return crc

    @classmethod
    def make(cls, header: int, payload: bytes):
        """
        创建一个完整数据包对象
        - 自动计算 data_len
        - 自动计算 CRC32
        - 返回 bytes 可以直接发送
        """
        if len(payload) > DataPacket.MAX_PAYLOAD:
            raise ValueError("payload 超过最大 1024 字节")

        id = cls._get_next_id()
        data_len = len(payload)
        header = header

        # "!B B H"是格式字符串，告诉python每个字段占多少字节，类型，顺序
        # !代表大端 B代表1字节，H代表两字节
        raw = struct.pack("!B B H", header, id & 0xFF, data_len) + payload

        crc = DataPacket.calculate_crc32(raw)
        # print("crc = ", hex(crc))

        packet = raw + struct.pack("!I", crc)
        return packet


"""
打印帮助相关参数
"""


def print_help():
    help_text = """
uart_trans - 串口传输工具

用法1:
    uart_trans -h        显示帮助信息
    uart_trans -p COMx   指定串口号
    uart_trans -b 115200 指定波特率
    uart_trans -t 5      指定超时时间

用法2:
    直接配置config.json文件
"""
    print(help_text)
    input("按任意键或回车退出…")  # 等待用户输入，然后程序结束


def wait_exit():
    global ser
    if ser and ser.is_open:
        ser.close()

    input("按任意键或回车退出…")
    sys.exit(0)


"""
加载config文件
"""


def load_configuration(config_file="config.json", config_dir="uart_trans_file"):
    ret1 = True
    current_dir = get_current_directory()

    # 判断config.json文件是否存在，不存在则创建，但不会退出
    config_path = os.path.join(current_dir, config_file)
    if not os.path.exists(config_path):
        # 创建并写入默认配置
        print("[INFO] config.json 文件不存在，正在创建默认配置文件...")

        with open(config_path, "w") as file:
            json.dump(default_config, file, indent=4)
            print("[INFO] 已创建config.json文件")

    # 判断下载bin文件的文件夹是否存在，不存在则创建，会强制退出
    load_bin_dir = os.path.join(current_dir, config_dir)
    if not os.path.exists(load_bin_dir):
        os.makedirs(load_bin_dir)
        print(
            f"[ERROR] 请将需要下载的bin文件放入 {load_bin_dir} 文件夹中，然后重新运行程序。"
        )
        ret1 = False

    if not ret1:
        return False
    else:
        return True


"""
获取当前运行的目录
"""


def get_current_directory():
    if getattr(sys, "frozen", False):  # 如果是打包的exe文件
        return os.path.dirname(sys.executable)  # 获取exe所在目录
    else:
        return os.path.dirname(os.path.abspath(__file__))  # 获取脚本所在目录


def merge_files():
    global send_bin_file
    current_dir = get_current_directory()
    load_bin_dir = os.path.join(current_dir, "uart_trans_file")

    # 如果之前合并的bin文件存在，先删除
    merged_bin_path = os.path.join(current_dir, "load_bin.bin")
    if os.path.exists(merged_bin_path):
        os.remove(merged_bin_path)

    # 获取uart_trans_file文件夹下所有bin文件
    bin_files = [f for f in os.listdir(load_bin_dir) if f.endswith(".bin")]

    if not bin_files:
        print(
            f"[ERROR] 在 {load_bin_dir} 文件夹中没有找到任何 .bin 文件。请将需要下载的bin文件放入该文件夹中，然后重新运行程序。"
        )
        wait_exit()

    offset = 0
    max_name_len = max(len(f) for f in bin_files)
    col_name = max_name_len + 4  # 加点缓冲
    col_index = 6
    col_size = 12
    col_offset = 12

    with open(merged_bin_path, "wb") as merged_bin:
        with open(
            os.path.join(load_bin_dir, "info.txt"), "w", encoding="utf-8"
        ) as info_file:
            # info_file.write("索引\t文件名\t大小\t偏移量\n")
            header = f"{'索引':<{col_index}}{'文件名':<{col_name}}{'大小':<{col_size}}{'偏移量':<{col_offset}}\n"
            info_file.write(header)
            for index, bin_file in enumerate(bin_files):
                bin_path = os.path.join(load_bin_dir, bin_file)
                size = os.path.getsize(bin_path)

                with open(bin_path, "rb") as bf:
                    merged_bin.write(bf.read())

                # info_file.write(f"{index + 1}\t{display_name}\t{size}\t{offset}\n")
                line = f"{index + 1:<{col_index}}{bin_file:<{col_name}}{size:<{col_size}}{offset:<{col_offset}}\n"
                info_file.write(line)
                offset += size

    print("[INFO] bin文件已成功合并为load_bin.bin，并生成info.txt")
    send_bin_file = open(merged_bin_path, "rb")


"""
配置串口参数
"""


def serial_init(port: str, baudrate: str):
    global ser  # 声明函数内使用的是全局变量

    try:
        ser = serial.Serial(port, baudrate, timeout=5)
        print("[INFO] 串口已打开")
    except serial.SerialException as e:
        print("[ERROR] 串口打开失败：", e)
        print("[ERROR] 请重新配置config.json文件或使用命令行参数指定正确的串口号")
        wait_exit()


async def main_while(time_out: int):
    # 启动异步读取串口数据的任务
    read_task = asyncio.create_task(read_serial_data(ser))

    # 启动定时器任务，周期2秒，可改
    asyncio.create_task(timer_task(time_out))

    try:
        await read_task
    except asyncio.CancelledError:
        print("[INFO] 主循环被取消，程序退出")


"""
定时器超时退出脚本
"""


async def timer_task(timeout):
    while True:
        try:
            # 等待事件被触发或超时
            await asyncio.wait_for(watchdog_event.wait(), timeout=timeout)
            # print("定时器重置")
            watchdog_event.clear()  # 清除事件，准备下一轮
        except asyncio.TimeoutError:
            print(f"[ERROR] {timeout}s内未收到数据，定时器超时")
            for task in asyncio.all_tasks():
                task.cancel()  # 取消所有 asyncio 任务
            input("按任意键或回车退出…")
            return
        except asyncio.CancelledError:
            # 协程被取消时静默退出，防止打印“Task exception was never retrieved”
            return


"""
异步读取串口数据
"""


async def read_serial_data(ser):
    while True:
        # 使用 asyncio.to_thread() 将串口读取操作放到独立线程执行
        data = await asyncio.to_thread(ser.readline)
        if data:
            decoded_data = data.decode("utf-8").strip()  # 处理接收到的数据
            if decoded_data:
                ret = process_serial_data(decoded_data)
                if not ret:
                    break


"""
处理串口读取数据，并配置发送文件
"""


def process_serial_data(uart_string: str):
    global error_max_count
    watchdog_event.set()

    if uart_string == "ready":
        error_max_count = 0
        send_bin_data()

    elif uart_string == "continue":
        error_max_count = 0
        send_bin_data()

    elif uart_string == "finish":
        print("[INFO] 单片机已接收完整个bin文件")
        for task in asyncio.all_tasks():
            task.cancel()  # 取消所有 asyncio 任务
        return False

    elif uart_string == "try_again":
        error_max_count += 1
        if error_max_count >= 3:
            print("[ERROR] 连续3次crc错误，退出程序。")
            return False  # 退出接收循环
        else:
            send_bin_data(is_next=False)

    elif uart_string == "time_out":
        print("[ERROR] 单片机超时未收到数据，已退出")
        return False

    return True


def send_bin_data(is_next: bool = True):
    global send_bin_file, bin_send_offset
    if not is_next:
        pos = send_bin_file.tell()
        target_pos = max(0, pos - DataPacket.MAX_PAYLOAD)
        send_bin_file.seek(target_pos)
        bin_send_offset = target_pos

    bin_data = send_bin_file.read(DataPacket.MAX_PAYLOAD)
    bin_send_offset += len(bin_data)
    print(f"[INFO] bin_send_offset = {bin_send_offset}")
    if len(bin_data) == 0:
        print("[INFO] bin 文件发送完成")
        return

    packet_bytes = DataPacket.make(header=0xA1, payload=bin_data)
    ser.write(packet_bytes)


def send_bin_info():
    print("[INFO] 发送文件信息")
    current_dir = get_current_directory()
    merged_bin_path = os.path.join(current_dir, "load_bin.bin")
    file_name = os.path.basename(merged_bin_path) + "\0"
    file_size = str(os.path.getsize(merged_bin_path)) + "\0"
    print(f"[INFO] file_name = {file_name}, file_size = {file_size}")
    file_content = f"name:{file_name}size:{file_size}"
    packet_bytes = DataPacket.make(header=0xA0, payload=file_content.encode("utf-8"))
    ser.write(packet_bytes)


"""
如果有输入参数，优先选用参数
如果没有输入参数，使用config.json中的配置
如果没有config.json文件，自动生成一个config.json文件，用户输入任意键退出
"""
if __name__ == "__main__":
    args = sys.argv[1:]  # 排除脚本名
    i = 0

    # 输入-h参数，显示帮助信息，并且不存在config.json文件也会自动生成，然后任意键退出
    if len(args) == 1 and args[0] == "-h":
        config_ret = load_configuration()
        print_help()
        sys.exit(0)

    # 判断加载配置文件和文件夹是否存在，不存在则创建，文件夹不存在会强制退出
    config_ret = load_configuration()
    if not config_ret:
        wait_exit()

    # 加载config.json文件
    print("[INFO] 检查 config.json 配置")
    current_dir = get_current_directory()
    config_path = os.path.join(current_dir, "config.json")

    if os.path.exists(config_path):
        try:
            with open(config_path, "r", encoding="utf-8") as f:
                user_config = json.load(f)
            # 自动补全 default_config
            serial_cfg = user_config.get("serial", {})
            default_config["serial"].update(
                {
                    "port": serial_cfg.get("port", default_config["serial"]["port"]),
                    "baudrate": serial_cfg.get(
                        "baudrate", default_config["serial"]["baudrate"]
                    ),
                    "timeout": serial_cfg.get(
                        "timeout", default_config["serial"]["timeout"]
                    ),
                }
            )
            print("[INFO] 已加载 config.json 配置")
        except Exception as e:
            print(f"[WARNING] 读取 config.json 出错，使用默认配置: {e}")
    else:
        print("[INFO] config.json 不存在，使用默认配置")

    # 解析命令行参数，覆盖配置文件中的设置
    while i < len(args):
        key = args[i]

        if i + 1 >= len(args):
            print(f"[ERROR] 错误，参数{key}缺少取值")
            sys.exit(1)
        value = args[i + 1]

        if key == "-p":
            default_config["serial"]["port"] = value
            print(f"[ERROR] 串口号: {value}")
        elif key == "-b":
            default_config["serial"]["baudrate"] = value
            print(f"[ERROR] 波特率: {value}")
        elif key == "-t":
            default_config["serial"]["timeout"] = value
            print(f"[ERROR] 超时时间: {value}")
        else:
            print(f"[ERROR] 未知参数: {key}")
            sys.exit(1)

        i = i + 2

    port = default_config["serial"]["port"]
    baudrate = int(default_config["serial"]["baudrate"])
    timeout = int(default_config["serial"]["timeout"])

    print(f"[INFO] 串口号：{port}, 波特率：{baudrate}, 超时时间：{timeout}秒")

    # 打开串口
    serial_init(port=port, baudrate=baudrate)

    # 打开定时器
    watchdog_event = asyncio.Event()  # 用于重置定时器

    # 加载文件，文件不存在返回错误，后续要做在文件夹里并且要生成合并的txt文件
    merge_files()

    # 发送文件信息
    send_bin_info()

    asyncio.run(main_while(time_out=int(timeout)))

    # 单片机定时器还没做
    # 运行之后应该先显示终端，显示信息来继续操作
    # 发送文件信息最好也分离出来，现在放在合并文件的函数中了 ✔
    # 还有打包exe，打包成一个exe是使用-F参数 ✔
    # 还有一个问题，不存在config文件和不存在uart_trans_file最好一次生成，目前的代码要两次才能生成  ✔
    # 需要换一个py文件，这个文件别动了  ✔
