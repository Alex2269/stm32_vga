
TARGET = stm32h7_vga_dma

# debug build?
DEBUG = 1
# optimization
OPT = -O2
# debug output periph regs
OPT += -g3

ENABLE_SEMIHOSTING ?= 0

# Build paths
BUILD_DIR = build
BUILD_ASM_DIR = $(BUILD_DIR)/asm
BUILD_APP_DIR = $(BUILD_DIR)/app
BUILD_CC_DIR  = $(BUILD_DIR)/ccc
BUILD_CPP_DIR = $(BUILD_DIR)/cpp


# recursive find sources files
# sources
ROOT_DIR = .
C_SOURCES   += $(shell find ${ROOT_DIR} -name '*.c')
CPP_SOURCES += $(shell find ${ROOT_DIR} -name '*.cpp')
ASM_SOURCES += $(shell find ${ROOT_DIR} -name '*.s')

# binaries
PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
CXX = $(GCC_PATH)/$(PREFIX)g++
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
CXX = $(PREFIX)g++
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

#######################################
# CFLAGS
#######################################
CPU = -mcpu=cortex-m7
FPU = -mfpu=fpv5-sp-d16
FLOAT-ABI = -mfloat-abi=hard
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS =

# C defines
ifeq (1,$(ENABLE_SEMIHOSTING))
C_DEFS += -D SEMIHOSTING_ENABLE=1
endif

# C defines
C_DEFS +=
C_DEFS += '-D __align(value)=__attribute__((aligned(value)))'
C_DEFS += '-D __packed=__attribute__((__packed__))'
C_DEFS += '-D __weak=__attribute__((weak))'

C_DEFS += '-D  RAM_D1=__attribute__((section (".RAM_D1")))'
C_DEFS += '-D  RAM_D2=__attribute__((section (".RAM_D2")))'
C_DEFS += '-D  RAM_D3=__attribute__((section (".RAM_D3")))'

C_DEFS += '-D __irq=__attribute__((interrupt("IRQ")))'
#C_DEFS += '-D __irq=__attribute__((interrupt(irq)))'

# C_DEFS += -D USE_HAL_DRIVER
# C_DEFS += -D USE_FULL_LL_DRIVER
# C_DEFS += -D USE_STDPERIPH_DRIVER
# C_DEFS += -D CALL_ARM_SYSTEM_INIT
# C_DEFS += -D ARM_MATH_CM7
##
# C_DEFS += -D USE_HAL_DRIVER
C_DEFS += -D USE_FULL_LL_DRIVER
##
C_DEFS += -D STM32H750xx

# AS includes
AS_INCLUDES =

# includes
# Find header directories
INCDIR = .
C_INC    =$(shell find -L $(INCDIR) -name '*.h' -exec dirname {} \; | uniq)
CPP_INC +=$(shell find -L $(INCDIR) -name '*.hpp' -exec dirname {} \; | uniq)
C_INCLUDES  =$(C_INC:%=-I%)
CPP_INCLUDES=$(CPP_INC:%=-I%)

# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

GCCFLAGS += -Wall
GCCFLAGS += -fdata-sections
GCCFLAGS += -ffunction-sections
GCCFLAGS += -nostdlib
GCCFLAGS += -fno-threadsafe-statics
GCCFLAGS += --param max-inline-insns-single=500
GCCFLAGS += -fno-rtti
GCCFLAGS += -fno-exceptions
GCCFLAGS += -fno-use-cxa-atexit

CFLAGS_STD = -c -Os -w -std=gnu17 $(GCCFLAGS)
CXXFLAGS_STD = -c -Os -w -std=gnu++17 $(GCCFLAGS)

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) $(CFLAGS_STD)
CPPFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) $(CXXFLAGS_STD)

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif

# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

LDSCRIPT += $(shell find ${ROOT_DIR} -name '*_FLASH.ld')

FLASH_SIZE=131072
RAM_SIZE=1048576

# libraries
LIBDIR =
LIBS = -lc -lm

ifeq (1,$(ENABLE_SEMIHOSTING))
LDFLAGS += --specs=rdimon.specs
LIBS += -lrdimon
# LIBS += --specs=nano.specs -lrdimon_nano # no float printf
else
LDFLAGS += --specs=nano.specs
LIBS += -lnosys
endif

LDFLAGS += -Wl,--defsym=LD_FLASH_OFFSET=0
LDFLAGS += -Wl,--defsym=LD_MAX_SIZE=$(FLASH_SIZE)
LDFLAGS += -Wl,--defsym=LD_MAX_DATA_SIZE=$(RAM_SIZE)
LDFLAGS += $(MCU) -T$(LDSCRIPT)
LDFLAGS += -Wl,-gc-sections,--print-memory-usage,-Map=$(BUILD_CC_DIR)/$(TARGET).map
LDFLAGS += -Wl,--check-sections
LDFLAGS += -Wl,--gc-sections
LDFLAGS += -Wl,--entry=Reset_Handler
LDFLAGS += -Wl,--unresolved-symbols=report-all
LDFLAGS += -Wl,--warn-common
LDFLAGS += -Wl,--default-script=$(LDSCRIPT)
LDFLAGS += -Wl,--start-group
LDFLAGS +=  $(LIBDIR) $(LIBS)
LDFLAGS += -Wl,--end-group
LDFLAGS += -lgcc
LDFLAGS += -lstdc++

# default action: build all
all: $(BUILD_APP_DIR)/$(TARGET).elf $(BUILD_APP_DIR)/$(TARGET).hex $(BUILD_APP_DIR)/$(TARGET).bin

## shell color ##
green=\033[0;32m
YELLOW=\033[1;33m
NC=\033[0m
##-------------##

#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_CC_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

OBJECTS += $(addprefix $(BUILD_CPP_DIR)/,$(notdir $(CPP_SOURCES:.cpp=.o)))
vpath %.cpp $(sort $(dir $(CPP_SOURCES)))

# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_ASM_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_CC_DIR)/%.o: %.c Makefile | $(BUILD_CC_DIR)
	@echo "\n ${green} [compile:] ${YELLOW} $< ${NC}"
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_CC_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_CPP_DIR)/%.o: %.cpp Makefile | $(BUILD_CPP_DIR)
	@echo "\n ${green} [compile:] ${YELLOW} $< ${NC}"
	$(CXX) -c $(CPPFLAGS) -Wa,-a,-ad,-alms=$(BUILD_CPP_DIR)/$(notdir $(<:.cpp=.lst)) $< -o $@

$(BUILD_ASM_DIR)/%.o: %.s Makefile | $(BUILD_ASM_DIR)
	@echo "\n ${green} [compile:] ${YELLOW} $< ${NC}"
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_APP_DIR)/$(TARGET).elf: $(OBJECTS) Makefile | $(BUILD_APP_DIR)
	@echo "\n ${green} [linking:] ${YELLOW} $@ ${NC}"
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@ --format=Berkeley
#	$(SZ) $@ --format=SysV --radix=16

$(BUILD_APP_DIR)/%.hex: $(BUILD_APP_DIR)/%.elf | $(BUILD_APP_DIR)
	$(HEX) $< $@

$(BUILD_APP_DIR)/%.bin: $(BUILD_APP_DIR)/%.elf | $(BUILD_APP_DIR)
	$(BIN) $< $@

$(BUILD_CC_DIR):
	mkdir -p $@
$(BUILD_CPP_DIR):
	mkdir -p $@
$(BUILD_APP_DIR):
	mkdir -p $@
$(BUILD_ASM_DIR):
	mkdir -p $@


#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)

#######################################
# flash
#######################################
flash:
	st-flash --reset write $(BUILD_APP_DIR)/$(TARGET).bin 0x8000000

stm32flash:
	stm32flash -w $(BUILD_APP_DIR)/$(TARGET).bin -v -g 0x0 /dev/ttyUSB0

#######################################
# debug
#######################################
ifeq (1,$(ENABLE_SEMIHOSTING))
debug:
	killall -q openocd ; openocd \
	-f interface/stlink.cfg \
	-f target/stm32h7x.cfg \
	-c "init" \
	-c "arm semihosting enable" \
	sleep 1 &
	gdb-multiarch \
	--eval-command "tar ext :3333" \
	$(BUILD_APP_DIR)/$(TARGET).elf \
	--eval-command "load"
else
debug:
	arm-none-eabi-gdb \
	--eval-command "load" \
	-iex ' tar ext | openocd \
	-f interface/stlink.cfg \
	-f target/stm32h7x.cfg \
	-c "adapter_khz 1800" \
	-c "interface hla" \
	-c "gdb_port pipe" ' \
	-iex "monitor halt" \
	$(BUILD_APP_DIR)/$(TARGET).elf
endif

#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***
