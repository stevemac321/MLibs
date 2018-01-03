BSP_ROOT ?= $(HOME)/STLibs/EmbeddedBSPs/arm-eabi
EFP_BASE ?= $(HOME)/STLibs/EmbeddedEFPs
TESTFW_BASE ?= $(HOME)/STLibs/TestFrameworks
TOOLCHAIN_ROOT ?= /home/comet/gcc-arm-none-eabi-5_4-2016q3
LIBSRC=$(HOME)/MLibs/lib
ASMSRC=$(HOME)/MLibs/lib/asm
LIBINC=$(HOME)/MLibs/cinc
#Embedded toolchain
CC := $(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc 
CXX := $(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc 
LD := $(CXX)
AR := $(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc 
OBJCOPY := $(TOOLCHAIN_ROOT)/bin/arm-none-eabi-objcopy

#Additional flags
PREPROCESSOR_MACROS += ARM_MATH_CM4 STM32F401RE stm32_flash_layout STM32F401xx
INCLUDE_DIRS += . $(BSP_ROOT)/STM32F4xxxx/STM32F4xx_StdPeriph_Driver/inc $(BSP_ROOT)/STM32F4xxxx/CMSIS_StdPeriph/Device/ST/STM32F4xx/Include $(BSP_ROOT)/STM32F4xxxx/CMSIS_StdPeriph/Include $(BSP_ROOT)/STM32F4xxxx/CMSIS_StdPeriph/RTOS/Template $(LIBINC)
LIBRARY_DIRS += 
LIBRARY_NAMES += 
ADDITIONAL_LINKER_INPUTS += 
MACOS_FRAMEWORKS += 
LINUX_PACKAGES += 

CFLAGS += -ggdb -DSTM32F401xx
CXXFLAGS += 
ASFLAGS += -mfpu=fpv4-sp-d16 -ggdb -DSTM32F401xx
LDFLAGS += -u _printf_float -specs=rdimon.specs 
COMMONFLAGS += -mcpu=cortex-m4 -mthumb -mfloat-abi=soft
LINKER_SCRIPT := $(BSP_ROOT)/STM32F4xxxx/LinkerScripts/STM32F401RE_flash.lds

