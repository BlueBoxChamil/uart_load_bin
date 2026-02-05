# ---------------------------------- #
#    GCC Bulid command log config    #
# ---------------------------------- #
#CMD_LOG = 1

# echo suspend
ifdef CMD_LOG
  NO_ECHO = 
else
  NO_ECHO = @
endif

SYSTEM := $(filter Windows_NT, $(OS))

# ---------------------------------- #
# Project name                       #
# ---------------------------------- #
PROJECT_NAME := Project

# ---------------------------------- #
# Path config                        #
# ---------------------------------- #
TOP_DIR     := ../../../..
PROJECT_DIR := ../Src
OBJECT_DIR  := Objects
OUTPUT_DIR  := output

# ---------------------------------- #
# Toolchain config                   #
# ---------------------------------- #
TOOLCHAIN := arm-none-eabi

CC      := $(TOOLCHAIN)-gcc
AS      := $(TOOLCHAIN)-as
LD      := $(TOOLCHAIN)-ld
OBJDUMP := $(TOOLCHAIN)-objdump
OBJCOPY := $(TOOLCHAIN)-objcopy
SIZE    := $(TOOLCHAIN)-size

# ---------------------------------- #
# ld/lib config                      #
# ---------------------------------- #
LD   := $(TOP_DIR)/components/tools/gcc/ldscript_3092e.ld
LIB  := $(TOP_DIR)/components/btdm/libbtdm_host.a

# ---------------------------------- #
# Source files config                #
# ---------------------------------- #
SRC_FILES += $(TOP_DIR)/components/drivers/device/fr30xx/system_fr30xx.c 
SRC_FILES += $(TOP_DIR)/components/drivers/device/fr30xx/trim_fr30xx.c 
SRC_FILES += $(TOP_DIR)/components/drivers/device/fr30xx/gcc/syscalls.c 
SRC_FILES += $(TOP_DIR)/components/drivers/device/fr30xx/gcc/sysmem.c 
SRC_FILES += $(TOP_DIR)/components/drivers/device/fr30xx/gcc/startup_fr30xx.S 
SRC_FILES += $(TOP_DIR)/components/drivers/peripheral/Src/driver_cali.c 
SRC_FILES += $(TOP_DIR)/components/drivers/peripheral/Src/driver_codec.c 
SRC_FILES += $(TOP_DIR)/components/drivers/peripheral/Src/driver_efuse.c 
SRC_FILES += $(TOP_DIR)/components/drivers/peripheral/Src/driver_flash.c 
SRC_FILES += $(TOP_DIR)/components/drivers/peripheral/Src/driver_frspim.c 
SRC_FILES += $(TOP_DIR)/components/drivers/peripheral/Src/driver_gpio.c 
SRC_FILES += $(TOP_DIR)/components/drivers/peripheral/Src/driver_i2s.c 
SRC_FILES += $(TOP_DIR)/components/drivers/peripheral/Src/driver_ipc.c 
SRC_FILES += $(TOP_DIR)/components/drivers/peripheral/Src/driver_pdm.c 
SRC_FILES += $(TOP_DIR)/components/drivers/peripheral/Src/driver_pmu.c 
SRC_FILES += $(TOP_DIR)/components/drivers/peripheral/Src/driver_pmu_iwdt.c 
SRC_FILES += $(TOP_DIR)/components/drivers/peripheral/Src/driver_qspi.c 
SRC_FILES += $(TOP_DIR)/components/drivers/peripheral/Src/driver_sd.c 
SRC_FILES += $(TOP_DIR)/components/drivers/peripheral/Src/driver_sd_card.c
SRC_FILES += $(TOP_DIR)/components/drivers/peripheral/Src/driver_spdif.c  
SRC_FILES += $(TOP_DIR)/components/drivers/peripheral/Src/driver_uart.c 
SRC_FILES += $(TOP_DIR)/components/drivers/peripheral/Src/driver_trng.c 
SRC_FILES += $(TOP_DIR)/components/modules/audio/resample/resample.c 
SRC_FILES += $(TOP_DIR)/components/modules/audio/codec/codec.c 
SRC_FILES += $(TOP_DIR)/components/modules/audio/algorithm/algorithm.c 
SRC_FILES += $(TOP_DIR)/components/modules/audio/audio_decoder.c 
SRC_FILES += $(TOP_DIR)/components/modules/audio/audio_encoder.c 
SRC_FILES += $(TOP_DIR)/components/modules/audio/audio_hw.c 
SRC_FILES += $(TOP_DIR)/components/modules/audio/audio_rpmsg.c 
SRC_FILES += $(TOP_DIR)/components/modules/audio/audio_scene.c 
SRC_FILES += $(TOP_DIR)/components/modules/audio/audio_a2dp_sink.c 
SRC_FILES += $(TOP_DIR)/components/modules/audio/audio_a2dp_source.c 
SRC_FILES += $(TOP_DIR)/components/modules/audio/audio_sco.c 
SRC_FILES += $(TOP_DIR)/components/modules/audio/local_playback.c 
SRC_FILES += $(TOP_DIR)/components/modules/common/src/co_list.c 
SRC_FILES += $(TOP_DIR)/components/modules/common/src/co_log.c 
SRC_FILES += $(TOP_DIR)/components/modules/common/src/co_util.c 
SRC_FILES += $(TOP_DIR)/components/modules/crc/crc32.c 
SRC_FILES += $(TOP_DIR)/components/modules/ext_flash_program/ext_flash_program.c 
SRC_FILES += $(TOP_DIR)/components/modules/fatfs/source/ff.c 
SRC_FILES += $(TOP_DIR)/components/modules/fatfs/source/ffsystem.c 
SRC_FILES += $(TOP_DIR)/components/modules/fatfs/source/ffunicode.c 
SRC_FILES += $(TOP_DIR)/components/modules/FlashDB/flashdb/src/fdb.c 
SRC_FILES += $(TOP_DIR)/components/modules/FlashDB/flashdb/src/fdb_file.c 
SRC_FILES += $(TOP_DIR)/components/modules/FlashDB/flashdb/src/fdb_kvdb.c 
SRC_FILES += $(TOP_DIR)/components/modules/FlashDB/flashdb/src/fdb_tsdb.c 
SRC_FILES += $(TOP_DIR)/components/modules/FlashDB/flashdb/src/fdb_utils.c 
SRC_FILES += $(TOP_DIR)/components/modules/FlashDB/port/fal/src/fal.c 
SRC_FILES += $(TOP_DIR)/components/modules/FlashDB/port/fal/src/fal_flash.c 
SRC_FILES += $(TOP_DIR)/components/modules/FlashDB/port/fal/src/fal_partition.c 
SRC_FILES += $(TOP_DIR)/components/modules/FlashDB/port/fal/src/fal_rtt.c 
SRC_FILES += $(TOP_DIR)/components/modules/FreeRTOS/portable/ARMv8M/non_secure/portable/GCC/ARM_CM33_NTZ/portasm.c 
SRC_FILES += $(TOP_DIR)/components/modules/FreeRTOS/portable/ARMv8M/non_secure/port.c 
SRC_FILES += $(TOP_DIR)/components/modules/FreeRTOS/portable/MemMang/heap_6.c 
SRC_FILES += $(TOP_DIR)/components/modules/FreeRTOS/croutine.c 
SRC_FILES += $(TOP_DIR)/components/modules/FreeRTOS/event_groups.c 
SRC_FILES += $(TOP_DIR)/components/modules/FreeRTOS/freertos_sleep.c 
SRC_FILES += $(TOP_DIR)/components/modules/FreeRTOS/list.c 
SRC_FILES += $(TOP_DIR)/components/modules/FreeRTOS/queue.c 
SRC_FILES += $(TOP_DIR)/components/modules/FreeRTOS/stream_buffer.c 
SRC_FILES += $(TOP_DIR)/components/modules/FreeRTOS/tasks.c 
SRC_FILES += $(TOP_DIR)/components/modules/FreeRTOS/timers.c 
SRC_FILES += $(TOP_DIR)/components/modules/FreeRTOS/cpu_context_gcc.S 
SRC_FILES += $(TOP_DIR)/components/modules/heap/heap.c 
SRC_FILES += $(TOP_DIR)/components/modules/rpmsg-lite/lib/rpmsg_lite/rpmsg.c 
SRC_FILES += $(TOP_DIR)/components/modules/rpmsg-lite/lib/rpmsg_lite/rpmsg_lite.c 
SRC_FILES += $(TOP_DIR)/components/modules/rpmsg-lite/lib/rpmsg_lite/rpmsg_queue.c 
SRC_FILES += $(TOP_DIR)/components/modules/rpmsg-lite/lib/rpmsg_lite/porting/environment/rpmsg_env_freertos.c 
SRC_FILES += $(TOP_DIR)/components/modules/rpmsg-lite/lib/rpmsg_lite/porting/platform/fr30xx_cm33/rpmsg_platform.c 
SRC_FILES += $(TOP_DIR)/components/modules/rpmsg-lite/lib/common/llist.c 
SRC_FILES += $(TOP_DIR)/components/modules/rpmsg-lite/lib/virtio/virtqueue.c 
SRC_FILES += $(TOP_DIR)/components/modules/dsp/dsp.c 
SRC_FILES += $(TOP_DIR)/components/modules/dsp/dsp_mem.c 
SRC_FILES += $(TOP_DIR)/examples/common/btdm/SWD.c 
SRC_FILES += $(TOP_DIR)/examples/common/btdm/btdm_mem.c 
SRC_FILES += $(TOP_DIR)/examples/common/btdm/controller.c 
SRC_FILES += $(TOP_DIR)/examples/common/btdm/controller_code_revb.c 
SRC_FILES += $(TOP_DIR)/examples/common/btdm/controller_code_revc.c
SRC_FILES += $(TOP_DIR)/examples/common/btdm/host.c 
SRC_FILES += $(TOP_DIR)/examples/common/btdm/profile/ble_ANCS_AMCS/ANCS_AMS_client.c
SRC_FILES += $(TOP_DIR)/examples/common/btdm/profile/ble_simple_gatt/simple_gatt_service.c
SRC_FILES += $(TOP_DIR)/examples/common/btdm/profile/ble_hid/hid_service.c
SRC_FILES += $(TOP_DIR)/examples/common/btdm/profile/ble_ANCS_AMCS/AMS_client.c 
SRC_FILES += $(TOP_DIR)/examples/common/dsp/dsp_code_flash.c 
SRC_FILES += $(TOP_DIR)/examples/common/dsp/dsp_code_rom.c 
SRC_FILES += $(TOP_DIR)/examples/common/flashdb/fal_flash_port.c 
SRC_FILES += $(TOP_DIR)/examples/common/flashdb/fdb_app.c 
SRC_FILES += $(TOP_DIR)/examples/application/btdm_audio/Src/app_at.c 
SRC_FILES += $(TOP_DIR)/examples/application/btdm_audio/Src/app_ble.c 
SRC_FILES += $(TOP_DIR)/examples/application/btdm_audio/Src/app_bt.c 
SRC_FILES += $(TOP_DIR)/examples/application/btdm_audio/Src/app_btdm.c 
SRC_FILES += $(TOP_DIR)/examples/application/btdm_audio/Src/app_hid_sdp.c 
SRC_FILES += $(TOP_DIR)/examples/application/btdm_audio/Src/app_task.c 
SRC_FILES += $(TOP_DIR)/examples/application/btdm_audio/Src/app_audio.c 
SRC_FILES += $(TOP_DIR)/examples/application/btdm_audio/Src/app_rpmsg.c 
SRC_FILES += $(TOP_DIR)/examples/application/btdm_audio/Src/mp3_sample.c 
SRC_FILES += $(TOP_DIR)/examples/application/btdm_audio/Src/sbc_sample.c 
SRC_FILES += $(TOP_DIR)/examples/application/btdm_audio/Src/user_bt.c 
SRC_FILES += $(TOP_DIR)/examples/application/btdm_audio/Src/diskio.c 
SRC_FILES += $(TOP_DIR)/examples/application/btdm_audio/Src/main.c 

# ---------------------------------- #
# Include path config                #
# ---------------------------------- #
INC_PATH += -I"$(TOP_DIR)/components/btdm/include"
INC_PATH += -I"$(TOP_DIR)/components/drivers/cmsis"
INC_PATH += -I"$(TOP_DIR)/components/drivers/device"
INC_PATH += -I"$(TOP_DIR)/components/drivers/device/fr30xx"
INC_PATH += -I"$(TOP_DIR)/components/drivers/peripheral/Inc"
INC_PATH += -I"$(TOP_DIR)/components/modules/audio"
INC_PATH += -I"$(TOP_DIR)/components/modules/audio/algorithm"
INC_PATH += -I"$(TOP_DIR)/components/modules/audio/codec"
INC_PATH += -I"$(TOP_DIR)/components/modules/audio/resample"
INC_PATH += -I"$(TOP_DIR)/components/modules/common/include"
INC_PATH += -I"$(TOP_DIR)/components/modules/dsp"
INC_PATH += -I"$(TOP_DIR)/components/modules/fatfs/source"
INC_PATH += -I"$(TOP_DIR)/components/modules/heap"
INC_PATH += -I"$(TOP_DIR)/components/modules/rpmsg-lite/lib/include"
INC_PATH += -I"$(TOP_DIR)/components/modules/rpmsg-lite/lib/include/platform/fr30xx_m33"
INC_PATH += -I"$(TOP_DIR)/components/modules/crc"
INC_PATH += -I"$(TOP_DIR)/components/modules/ext_flash_program"
INC_PATH += -I"$(TOP_DIR)/components/modules/FlashDB/flashdb/inc"
INC_PATH += -I"$(TOP_DIR)/components/modules/FlashDB/port/fal/inc"
INC_PATH += -I"$(TOP_DIR)/components/modules/FreeRTOS/include"
INC_PATH += -I"$(TOP_DIR)/components/modules/FreeRTOS/portable/ARMv8M/non_secure"
INC_PATH += -I"$(TOP_DIR)/components/modules/FreeRTOS/portable/ARMv8M/non_secure/portable/GCC/ARM_CM33_NTZ"
INC_PATH += -I"$(TOP_DIR)/examples/common/btdm"
INC_PATH += -I"$(TOP_DIR)/examples/common/btdm/profile/ble_ANCS_AMCS"
INC_PATH += -I"$(TOP_DIR)/examples/common/btdm/profile/ble_hid"
INC_PATH += -I"$(TOP_DIR)/examples/common/btdm/profile/ble_simple_gatt"
INC_PATH += -I"$(TOP_DIR)/examples/common/flashdb"
INC_PATH += -I"$(TOP_DIR)/examples/application/btdm_audio/Inc"
INC_PATH += -I"$(TOP_DIR)/examples/application/btdm_audio/Src"

# ----------------------------------- #
# Objects files common to all targets #
# ----------------------------------- #
DIR_OBJS_c = $(SRC_FILES:%.c=%.o)
DIR_OBJS_s = $(DIR_OBJS_c:%.s=%.o)
DIR_OBJS_S = $(DIR_OBJS_s:%.S=%.o)
DIR_OBJS = $(DIR_OBJS_S)

BASE_SRC  = $(notdir $(DIR_OBJS)) 
BASE_OBJS = $(BASE_SRC:%.c=%.o)
OBJS      = $(BASE_OBJS:%.o=$(OBJECT_DIR)/%.o)

BASE_ElF  = $(OBJECT_DIR)/$(PROJECT_NAME).elf

# ---------------------------------- #
# Source files path                  #
# ---------------------------------- #
# VPATH = $(dir $(SRC_FILES)) 

# ---------------------------------- #
# C flags common to all targets      #
# ---------------------------------- #
CFLAGS += -mcpu=cortex-m33
CFLAGS += -mthumb -mfloat-abi=hard
CFLAGS += -Os
CFLAGS += -fmessage-length=0 -fsigned-char
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -g3
CFLAGS += -std=gnu11

# ---------------------------------- #
# LD flags common to all targets     #
# ---------------------------------- #
LDFLAGS += -mcpu=cortex-m33
LDFLAGS += -mthumb -mfloat-abi=hard
LDFLAGS += -O2
LDFLAGS += -fmessage-length=0 -fsigned-char
LDFLAGS += -ffunction-sections -fdata-sections
LDFLAGS += -g3
LDFLAGS += -Xlinker --gc-sections
LDFLAGS += --specs=nano.specs --specs=nosys.specs

# ---------------------------------- #
# all target list                    #
# ---------------------------------- #
all: Target_Path Target_OBJS Target_ELF Target_DONE

# ---------------------------------- #
# Create Path                        #
# ---------------------------------- #
Target_Path :
	$(shell if [ ! -d $(OBJECT_DIR) ];then mkdir $(OBJECT_DIR); fi)
    $(shell if [ ! -d $(OUTPUT_DIR) ];then mkdir $(OUTPUT_DIR); fi)

# ---------------------------------- #
# compile .c .s                      #
# ---------------------------------- #
Target_OBJS : $(DIR_OBJS)

%.o : %.c
	$(info Bulid: compiling $^)
	$(NO_ECHO)$(CC) $(CFLAGS) $(INC_PATH) -c -o  $(OBJECT_DIR)/$(notdir $@) $<

%.o : %.S
	$(info Bulid: compiling $^)
	$(NO_ECHO)$(CC) $(CFLAGS) $(INC_PATH) -c -o  $(OBJECT_DIR)/$(notdir $@) $<

%.o : %.s
	$(info Bulid: compiling $^)
	$(NO_ECHO)$(CC) $(CFLAGS) $(INC_PATH) -c -o  $(OBJECT_DIR)/$(notdir $@) $<

# ---------------------------------- #
# Create Image File                  #
# ---------------------------------- #
Target_ELF : $(BASE_ElF)

$(BASE_ElF) : $(OBJS)
	$(NO_ECHO)$(CC) $(LDFLAGS) -T $(LD) -Wl,-Map,"$(OUTPUT_DIR)/$(PROJECT_NAME).map" -o $(OBJECT_DIR)/$(PROJECT_NAME).elf $^ $(LIB) -lc -lm
	$(NO_ECHO)$(OBJCOPY) -O binary -S $(OBJECT_DIR)/$(PROJECT_NAME).elf $(OUTPUT_DIR)/$(PROJECT_NAME).bin
	$(NO_ECHO)$(OBJCOPY) -O ihex   -S $(OBJECT_DIR)/$(PROJECT_NAME).elf $(OUTPUT_DIR)/$(PROJECT_NAME).hex
	$(NO_ECHO)$(OBJDUMP) --source --all-headers --demangle --line-numbers --wide $(OBJECT_DIR)/$(PROJECT_NAME).elf > $(OUTPUT_DIR)/$(PROJECT_NAME).lst
	python.exe $(TOP_DIR)/components/tools/post_process.py $(PROJECT_NAME) $(OUTPUT_DIR)
	$(info Bulid: creating $(OBJECT_DIR)/$(PROJECT_NAME).elf)
	$(info Bulid: creating $(OUTPUT_DIR)/$(PROJECT_NAME).map)
	$(info Bulid: creating $(OUTPUT_DIR)/$(PROJECT_NAME).bin)
	$(info Bulid: creating $(OUTPUT_DIR)/$(PROJECT_NAME).hex)
	$(info Bulid: creating $(OUTPUT_DIR)/$(PROJECT_NAME).lst)

# ---------------------------------- #
# Output image file size             #
# ---------------------------------- #
.PHONY : Target_DONE
Target_DONE:
	@echo Bulid: Output image file info:
	$(NO_ECHO)$(SIZE) $(OBJECT_DIR)/$(PROJECT_NAME).elf
	@echo Bulid: $(PROJECT_NAME) make done.

# ---------------------------------------- #
# delete file(.o .elf .bin .dis .hex .map) #
# ---------------------------------------- #
.PHONY : clean

clean :
	$(NO_ECHO)rm -rf $(OBJECT_DIR) $(OUTPUT_DIR) *.elf *.bin *.dis *.hex *.map *.lst
	@echo delete *.o *.elf *.bin *.dis *.hex *.map *.lst
