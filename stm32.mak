#TOOLCHAIN_ROOT ?= /mnt/big/gcc-arm-none-eabi
MLIBS_ROOT ?=$(HOME)/MLibs
BSP_ROOT ?=$(MLIBS_ROOT)/STLibs/EmbeddedBSPs/arm-eabi
LIBINC ?=$(MLIBS_ROOT)/cinc
CPPINC ?=$(MLIBS_ROOT)/cppinc
LIBSRC ?=$(MLIBS_ROOT)/lib
ASMSRC ?=$(LIBSRC)/asm

#Embedded toolchain
CC := arm-none-eabi-gcc 
CXX := arm-none-eabi-gcc 
LD := $(CXX)
AR := arm-none-eabi-gcc 
OBJCOPY := arm-none-eabi-objcopy

#Additional flags
PREPROCESSOR_MACROS += ARM_MATH_CM4 STM32F401RE stm32_flash_layout STM32F401xx
INCLUDE_DIRS += $(BSP_ROOT)/STM32F4xxxx/STM32F4xx_StdPeriph_Driver/inc $(BSP_ROOT)/STM32F4xxxx/CMSIS_StdPeriph/Device/ST/STM32F4xx/Include $(BSP_ROOT)/STM32F4xxxx/CMSIS_StdPeriph/Include $(BSP_ROOT)/STM32F4xxxx/CMSIS_StdPeriph/RTOS/Template $(LIBINC) $(CPPINC)
LIBRARY_DIRS += 
LIBRARY_NAMES += 
ADDITIONAL_LINKER_INPUTS += 
MACOS_FRAMEWORKS += 
LINUX_PACKAGES += 

CFLAGS += -ggdb -DSTM32F401xx -std=c17 -DHEAP_SIZE_BYTES=8088
CXXFLAGS += -ggdb -DSTM32F401xx -std=c++20 
ASFLAGS += -mfpu=fpv4-sp-d16 -ggdb -DSTM32F401xx
LDFLAGS += -u _printf_float -specs=rdimon.specs 
COMMONFLAGS += -mcpu=cortex-m4 -mthumb -mfloat-abi=soft
LINKER_SCRIPT := $(BSP_ROOT)/STM32F4xxxx/LinkerScripts/STM32F401RE_flash.lds

