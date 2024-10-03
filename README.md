
# MLibs Project Overview

This project is a collection of modular C-based libraries for a variety of algorithms and data structures designed for embedded systems, as well as general-purpose usage. The libraries are tested and optimized to work efficiently on STM32F4 microcontrollers. They include components like priority queues, dynamic arrays, linked lists, open-addressed hash tables, spinlocks, and more.

## Setup and Build Instructions

1. **Extract the Required Libraries:**
   Extract `STLibs.tar` in place so that it becomes an immediate subdirectory of the `MLibs` directory.

2. **Configure the Environment:**
   The makefiles require a few environment variables to be set. Edit the `env.sh` script to point to your toolchain and the `MLibs` directory. You can also manually add these paths to your environment or set up a Linux distribution toolchain on your path.

3. **Build and Flash the Project:**
   Run `source ./env.sh` to set up the environment variables.
   - Navigate to the test directory you want to build, for example, `cd tests/asmtest`.
   - Use `make` to build the project manually, or use the provided Python build script (`bfr.py`).

4. **Using `bfr.py` for Automation:**
   The `bfr.py` script automates building, cleaning, flashing, and running the project.
   - Syntax: `./bfr <directory> <command>`, e.g., `./bfr tests build flashrun`.
   - Available commands: `clean`, `build`, `flashrun`.
   - The `flashrun` command will:
     - Erase the device.
     - Flash the `.bin` file using `st-flash`.
     - Start `openocd` and run the tests using `gdb` in an automated fashion.

5. **Toolchain and GDB Setup:**
   - The default build uses the `gcc-arm-none-eabi` toolchain (version 13.3 at this time).
   - `openocd` and `gdb-multiarch` are recommended for better results and integration.

6. **Adjusting for Your STM32 Board:**
   The suite currently works out of the box with STM32F401RE and STM32F411RE microcontrollers. For compatibility with other STM32F4 boards, update the `LINKER_SCRIPT` path in the `stm32.mak` file located at the root of the project to match your specific board's linker file.

   **Example:**
   ```makefile
   LINKER_SCRIPT := $(BSP_ROOT)/STM32F4xxxx/LinkerScripts/STM32F401RE_flash.lds
   ```
   Replace the above path with the correct linker script for your board.

7. **Using the `bfy.py` script for Advanced Operations:**
   - The `bfy.py` script can be used to automate various stages such as building, flashing, and running tests.
   - Use commands like `./bfy tests clean build flashrun` to perform all operations in sequence, or run each stage individually.

## Related Pages and Detailed Descriptions

Detailed descriptions of each library module, including usage and design considerations, are available under the "Related Pages" section in the generated Doxygen documentation. Each module is explained in depth with associated algorithms, data structures, and sample code.

- **Viewing the Documentation:**
   - Run `doxygen` in the root directory to generate the documentation.
   - Open the generated HTML files (usually in the `html` directory) and navigate to the "Related Pages" section to see detailed summaries of each module.

## Compatibility and Extensions

The project is designed for ARM Cortex-M microcontrollers and has been specifically tested on STM32F4xx series boards. However, the modular nature of the libraries allows for easy extension to other microcontrollers and platforms.

- **Supported Boards:**
  - STM32F401RE
  - STM32F411RE
  - Other STM32F4 boards with minor modifications to the linker script.

- **Libraries Overview:**
  - Priority Queues
  - Dynamic Arrays
  - Linked Lists
  - Open-Addressed Hash Tables
  - Spinlocks
  - Sorting and Searching Algorithms
  - Graph Algorithms
  - Bit Manipulation Utilities

## License

This project is licensed under the GPL version 2 (GPLv2).

