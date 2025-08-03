# Simple Operating System (SOS)

This repository contains a basic implementation of a Simple Operating System (SOS) with a 16-bit bootloader and a 32-bit kernel, built using a custom cross-compiler toolchain. The project is designed for educational purposes and can be run in a QEMU emulator.

## Overview
- **Bootloader**: A 16-bit bootloader written in assembly, responsible for initializing the system and loading the kernel.
- **Kernel**: A 32-bit kernel written in C and assembly, providing a minimal operating system environment.
- **Toolchain**: Uses a cross-compiler (`i686-elf-gcc`, `i686-elf-as`, `i686-elf-ld`, `i686-elf-objcopy`) for a freestanding environment.

## Prerequisites
- **Cross-Compiler**: Install the i686-elf toolchain.
- **QEMU**: Install QEMU for emulation
- **CMake**: Install CMake to manage the build process

## Project Structure
- `src/bootloader/`: Contains the 16-bit bootloader source (`boot.s`) and linker script (`linker.ld`).
- `src/kernel/`: Contains the 32-bit kernel source (`kernel_entry.s`, `kernel.c`) and linker script (`linker.ld`).

## Build Instructions
1. **Create Build Directory**:
   ```bash
   mkdir build && cd build
   ```
2. **Configure with CMake**:
   ```bash
   cmake -G "Unix Makefiles" ..
   ```
   - Note: Use `-G "Ninja"` if you prefer Ninja instead of Make.
3. **Build the Project**:
   ```bash
   make
   ```
   - This generates:
     - `bin/bootloader.bin`: 16-bit bootloader flat binary.
     - `bin/kernel.elf`: 32-bit kernel ELF with debug symbols.
     - `bin/kernel.bin`: Kernel flat binary.
     - `images/os.img`: Bootable floppy image combining bootloader and kernel.
4. **Run in QEMU**:
   ```bash
   make run
   ```

## Debugging
- **Start QEMU with GDB Server**:
  ```bash
  make debug
  ```
  - Connect with GDB in another terminal:
    ```bash
    i686-elf-gdb bin/kernel.elf
    (gdb) target remote localhost:1234
    (gdb) set architecture i386
    (gdb) symbol-file bin/kernel.elf
    (gdb) add-symbol-file bin/kernel.elf 0x10000
    (gdb) break _start
    (gdb) break protected_mode
    (gdb) continue
    ```

## Build Specifications
- **Toolchain**: Custom i686-elf cross-compiler.
- **C Flags**: `-g -O0 -ffreestanding -nostdlib -Wall -Wextra` (debug, no optimization, freestanding, no standard library, warnings).
- **Assembly Flags**: `-g --32` (debug, 32-bit mode).
- **Output**:
  - Bootloader: Linked at `0x7C00` with a 512-byte flat binary.
  - Kernel: Linked at `0x100000` as a 32-bit ELF, converted to a flat binary.
- **Image**: 2880-sector (1.44MB) floppy image (`os.img`) created with `dd`.
