BINARYDIR := Debug

#Additional flags
PREPROCESSOR_MACROS := DEBUG=1
INCLUDE_DIRS := 
LIBRARY_DIRS := 
LIBRARY_NAMES := 
ADDITIONAL_LINKER_INPUTS := 
MACOS_FRAMEWORKS := 
LINUX_PACKAGES := 

CFLAGS := -ggdb -ffunction-sections -O0
CXXFLAGS := -ggdb -ffunction-sections -fno-exceptions -fno-rtti -O0
ASFLAGS := 
LDFLAGS := -Wl,-gc-sections
COMMONFLAGS := 
LINKER_SCRIPT := 

START_GROUP := -Wl,--start-group
END_GROUP := -Wl,--end-group

#Additional options detected from testing the toolchain
USE_DEL_TO_CLEAN := 1
CP_NOT_AVAILABLE := 1

#ADDITIONAL_MAKE_FILES := $(HOME)/MLibs/stm32.mak
#ADDITIONAL_MAKE_FILES := $(MLIBS_ROOT)/stm32.mak
GENERATE_BIN_FILE := 1
GENERATE_IHEX_FILE := 0
