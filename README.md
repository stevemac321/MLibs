# MLibs Project Overview

This project is a collection of modular C-based libraries for a variety of algorithms and data structures designed for embedded systems, as well as general-purpose usage. The libraries are tested and optimized to work efficiently on STM32F4 microcontrollers. They include components like priority queues, dynamic arrays, linked lists, open-addressed hash tables, spinlocks, and more.

## Setup and Build Instructions

1. **Clone the Project:**
   ```bash
   git clone https://github.com/stevemac321/MLibs
   cd MLibs
   ```

2. **Install Required Tools and Libraries:**
   - To build, flash, and debug on Linux, you need to install the following tools:
     ```bash
     sudo apt-get update
     sudo apt-get install stlink-tools openocd gdb-multiarch libusb-1.0-0-dev
     ```
   - **Explanation of each package**:
     - `stlink-tools`: Provides the `st-flash` utility for flashing binaries to STM32 microcontrollers.
     - `openocd`: An open-source debugger for ARM Cortex microcontrollers.
     - `gdb-multiarch`: A multi-architecture GDB (GNU Debugger) that supports cross-debugging for ARM.
     - `libusb-1.0-0-dev`: Required for `stlink` to communicate with STM32 devices over USB.

3. **Configure the Environment:**
   The makefiles require a few environment variables to be set. Edit the `env.sh` script to point to your toolchain and the `MLibs` directory. You can also manually add these paths to your environment or set up a Linux distribution toolchain on your path. Check the `stm32.mak` file if you run into any problems.

  ### 🔐 USB Access for ST-Link/V2.1 (No `sudo` Required)

To use `st-flash` and `openocd` without `sudo`, create a udev rule that gives proper access to your ST-Link device:

```bash
sudo nano /etc/udev/rules.d/49-stlinkv2.rules
```

Paste this line for ST-Link/V2 or V2.1:

```
SUBSYSTEM=="usb", ATTR{idVendor}=="0483", ATTR{idProduct}=="3748", MODE="0666", GROUP="plugdev"
```

> This rule applies to ST-Link/V2 and ST-Link/V2.1 devices.  
> Do **not** use `374b`, which is for the Nucleo board’s ST-Link/V2-1.

Then reload udev and replug your device:

```bash
sudo udevadm control --reload
sudo udevadm trigger
```

Make sure your user is in the `plugdev` group:

```bash
sudo usermod -aG plugdev $USER
```

Log out and back in (or reboot) for the group change to take effect.

After this setup, you should be able to run `st-flash` and `bfr.py` without `sudo`. 

5. **Build and Flash the Project:**
   - Run `source ./env.sh` to set up the environment variables.
   - Navigate to the test directory you want to build, for example, `cd tests/asmtest`.
   - Use `make` to build the project manually, or use the provided Python build script (`bfr.py`).

6. **Using `bfr.py` for Automation:**
   The `bfr.py` script automates building, cleaning, flashing, and running the project.
   - Syntax: `./bfr <directory> <command>`, e.g., `./bfr tests build flashrun`.
   - Available commands: `clean`, `build`, `flashrun`.
   - The `flashrun` command will:
     - Erase the device.
     - Flash the `.bin` file using `st-flash`.
     - Start `openocd` and run the tests using `gdb` in an automated fashion.

7. **Toolchain and GDB Setup:**
   - The default build uses the `gcc-arm-none-eabi` toolchain (version 13.3 at this time).
   - `openocd` and `gdb-multiarch` are recommended for better results and integration.

8. **Adjusting for Your STM32 Board:**
   The suite currently works out of the box with STM32F401RE and STM32F411RE microcontrollers. For compatibility with other STM32F4 boards, update the `LINKER_SCRIPT` path in the `stm32.mak` file located at the root of the project to match your specific board's linker file.

   **Example:**
   ```makefile
   LINKER_SCRIPT := $(BSP_ROOT)/STM32F4xxxx/LinkerScripts/STM32F401RE_flash.lds
   ```
   Replace the above path with the correct linker script for your board.

9. **Using the `bfy.py` Script for Advanced Operations:**
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


