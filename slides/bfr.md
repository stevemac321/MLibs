/**
 * @page bfr Build-Flash-Run (BFR) Script
 * @brief Overview of the BFR Python script used for managing build, flash, and run operations.
 *
 * ## BFR Script Overview
 *
 * The `bfr` script (`bfr.py`) is a Python utility designed to automate the build, flash, and run process for embedded projects. It simplifies repetitive tasks and provides a convenient way to manage multiple projects using a single command.
 *
 * ### Key Features
 * - **Automated Build Process**: The script can traverse through specified directories and execute the build process (`make`) recursively.
 * - **Flashing to Embedded Devices**: Supports flashing binaries to embedded devices using `st-flash` and `openocd`.
 * - **Execution and Debugging**: Automatically starts debugging sessions using `gdb` with a pre-configured script.
 * - **Command-line Interface**: Accepts multiple commands (`clean`, `build`, `flashrun`, etc.) to control the flow of operations.
 *
 * ### Major Functions
 * - **`clean`**: Cleans up previous build artifacts in the specified directories.
 * - **`build`**: Compiles the source code and generates the binary files required for flashing.
 * - **`flashrun`**: Flashes the compiled binary to the microcontroller and runs the executable using `gdb`.
 * - **`run`**: Executes the compiled binary on the host (for x64 projects).
 *
 * ### Example Usage
 * 
 * ```bash
 * # Clean, build, and flash the project located in the `tests` directory
 * ./bfr tests clean build flashrun
 * 
 * # Build and run a specific project
 * ./bfr my_project build run
 * ```
 *
 * This script is particularly useful for automating the development workflow in embedded projects, especially those that involve ARM Cortex-M microcontrollers. See the detailed documentation for more information on each command.
 */


**Purpose**:  
The `build-flash-run` (`bfr`) harness is a command-line utility designed to automate common operations like cleaning, building, flashing, and running firmware projects in embedded development environments. It provides a unified interface to recursively perform these operations on multiple projects or target a specific project directory.

### Key Features

1. **Recursive Directory Traversal**:  
   The script is capable of traversing directories recursively, enabling the user to run commands like clean, build, and flash on multiple subdirectories at once. This feature is particularly useful for managing large projects with multiple modules or libraries.

2. **Command Set**:  
   The harness supports several common commands:
   - `clean`: Remove generated files and reset the build environment.
   - `build`: Compile the project using `make`.
   - `flasherase`: Erase the flash memory of the target device.
   - `flashrun`: Flash the binary onto the target device and run it.
   - `run`: Execute the project binary on the host machine.

3. **Integration with GDB and OpenOCD**:  
   The `flashrun` command flashes the binary onto the device and then runs `gdb` with a specified script to debug the application using OpenOCD. This enables a streamlined debugging workflow for embedded projects.

4. **Cross-Platform Compatibility**:  
   The harness is built using Python 3 and leverages standard tools like `make`, `st-flash`, and `gdb-multiarch`, making it compatible with Linux and other Unix-based systems where these tools are available.

### Command Descriptions

1. **`clean()`**:  
   Executes the `make clean` command to remove compiled objects and reset the build environment.

   ```python
   def clean():	
       bld = subprocess.Popen(['make', 'clean'])
       bld.wait()
   ```

2. **`build()`**:  
   Runs the `make` command to build the project.

   ```python
   def build():	
       bld = subprocess.Popen(['make'])
       bld.wait()
   ```

3. **`flasherase()`**:  
   Executes `st-flash erase` to erase the flash memory on the target microcontroller.

   ```python
   def flasherase():
       flash = subprocess.Popen(['st-flash', 'erase'])
       flash.wait()
   ```

4. **`flashrun()`**:  
   - Flashes the binary (`test.bin`) onto the target device using `st-flash`.
   - Resets the target device.
   - Starts OpenOCD to provide the GDB debug interface.
   - Executes `gdb-multiarch` with a specified script (`gdbscript`) to start debugging.

   ```python
   def flashrun():
       binfile = './test.bin'
       elffile = './test.elf'
       gdbscript = '/mnt/raiddrive/MLibs/gdbscript'  # Path to the gdbscript file

       # Flash the binary to the device
       flash1 = subprocess.Popen(['st-flash', 'write', binfile, '0x08000000'])
       flash1.wait()

       # Reset the device after flashing
       flash2 = subprocess.Popen(['st-flash', 'reset'])
       flash2.wait()

       # Start openocd for the debug interface
       stlink2 = subprocess.Popen(['openocd', '-f', 'interface/stlink.cfg', '-f', 'target/stm32f4x.cfg'])    

       # Check if the gdbscript file exists
       if not os.path.exists(gdbscript):
           print(f"Error: GDB script '{gdbscript}' not found.")
           return

       # Run GDB with the provided script and ELF file
       gdb2 = subprocess.Popen(['gdb-multiarch', '--batch', '--command=' + gdbscript, elffile])
       gdb2.wait()

       # Kill openocd once done
       stlink2.kill()
   ```

5. **`run()`**:  
   Executes the compiled binary (`./test`) on the host system.

   ```python
   def run():
       proc = subprocess.Popen('./test')
       proc.wait()
   ```

6. **Argument Parsing (`parseargs`)**:  
   The `parseargs()` function iterates over the command-line arguments and executes the corresponding commands in sequence. It switches the current directory based on the specified project directory and command.

   ```python
   def parseargs(root_dir, fullcur_dir, test_dir):
       idx = 2
       print(test_dir)
       while idx < argvlen:
           if sys.argv[idx] == 'clean':
               clean()
           elif sys.argv[idx] == 'build':
               build()
           elif sys.argv[idx] == 'flasherase':
               flasherase()
           elif sys.argv[idx] == 'flashrun':
               os.chdir(fullcur_dir + '/Debug')
               flashrun()
               os.chdir(fullcur_dir)
           elif sys.argv[idx] == 'run':
               run()
           else:
               print("bad arg: " + sys.argv[idx])
           idx += 1
   ```

### Usage Example

To use the script, navigate to the root directory of your project and execute the following command:

```bash
./bfr <folder> <commandlist...>
```

Where:
- `<folder>` is the relative path to the target directory (e.g., `MyProject`).
- `<commandlist...>` is one or more of the supported commands (`clean`, `build`, `flasherase`, `flashrun`, `run`).

**Example**:

```bash
./bfr MyProject clean build flashrun
```

This will:
1. Clean the project directory.
2. Build the project.
3. Flash the resulting binary onto the microcontroller and run it.

### Summary

The `build-flash-run` harness simplifies common tasks in embedded system development by providing a unified interface for building, flashing, and running projects. It supports recursive directory traversal, making it suitable for multi-module projects, and integrates seamlessly with tools like `st-flash`, `openocd`, and `gdb`. This script saves time by automating repetitive tasks and provides a straightforward mechanism for debugging and testing embedded systems.