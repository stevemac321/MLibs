BSP_ROOT ?= $(HOME)/MLibs/STLibs/EmbeddedBSPs/arm-eabi
EFP_BASE ?= $(HOME)/MLibs/STLibs/EmbeddedEFPs
TOOLCHAIN_ROOT ?= $(HOME)/MLibs/gcc-arm
LIBSRC=$(HOME)/MLibs/lib
TESTSRC=$(HOME)/MLibs/testsrc


#Embedded toolchain
CC := $(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc
CXX := $(TOOLCHAIN_ROOT)/bin/arm-none-eabi-g++
LD := $(CXX)
AR := $(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc 
OBJCOPY := $(TOOLCHAIN_ROOT)/bin/arm-none-eabi-objcopy

#Additional flags
PREPROCESSOR_MACROS += ARM_MATH_CM4 STM32F401RE stm32_flash_layout STM32F401xx
INCLUDE_DIRS += . $(BSP_ROOT)/STM32F4xxxx/STM32F4xx_StdPeriph_Driver/inc $(BSP_ROOT)/STM32F4xxxx/CMSIS_StdPeriph/Device/ST/STM32F4xx/Include $(BSP_ROOT)/STM32F4xxxx/CMSIS_StdPeriph/Include $(BSP_ROOT)/STM32F4xxxx/CMSIS_StdPeriph/RTOS/Template $(TOOLCHAIN_ROOT)/arm-none-eabi/include/c++/5.4.1 $(TOOLCHAIN_ROOT)/arm-none-eabi/include/c++/5.4.1/arm-none-eabi $(HOME)/MLibs/cppinc

LIBRARY_DIRS += 
LIBRARY_NAMES += 
ADDITIONAL_LINKER_INPUTS += 
MACOS_FRAMEWORKS += 
LINUX_PACKAGES += 

CFLAGS += -ggdb -DSTM32F401xx -std=c11 -DHEAP_SIZE_BYTES=8088 
CXXFLAGS += -ggdb -DSTM32F401xx -std=c++14 -DHEAP_SIZE_BYTES=8088 
ASFLAGS += -mfpu=fpv4-sp-d16
LDFLAGS += -u _printf_float -specs=rdimon.specs 
COMMONFLAGS += -mcpu=cortex-m4 -mthumb -mfloat-abi=soft
LINKER_SCRIPT := $(BSP_ROOT)/STM32F4xxxx/LinkerScripts/STM32F401RE_flash.lds

