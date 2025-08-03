cmake_minimum_required(VERSION 3.10)
project(SOS C ASM)

# === Cross-Compiler Toolchain ===
# You can also define this in a separate toolchain.cmake file for reusability.
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_C_COMPILER i686-elf-gcc)
set(CMAKE_ASM_COMPILER i686-elf-as)
set(CMAKE_LINKER i686-elf-ld)
set(CMAKE_OBJCOPY i686-elf-objcopy)

# Compilation flags with debug support
set(CMAKE_C_FLAGS "-g -O0 -ffreestanding -nostdlib -Wall -Wextra")
set(CMAKE_ASM_FLAGS "-g --32")

# === Project Structure ===
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(BOOTLOADER_DIR ${SRC_DIR}/bootloader)
set(KERNEL_DIR ${SRC_DIR}/kernel)

# === Output Directory Structure ===
set(BUILD_DIR ${CMAKE_BINARY_DIR})
set(OBJ_DIR ${BUILD_DIR}/obj)
set(BIN_DIR ${BUILD_DIR}/bin)
set(IMG_DIR ${BUILD_DIR}/images)

# Create output directories
file(MAKE_DIRECTORY ${OBJ_DIR})
file(MAKE_DIRECTORY ${OBJ_DIR}/bootloader)
file(MAKE_DIRECTORY ${OBJ_DIR}/kernel)
file(MAKE_DIRECTORY ${BIN_DIR})
file(MAKE_DIRECTORY ${IMG_DIR})

# === Object files ===
set(BOOT_OBJ ${OBJ_DIR}/bootloader/boot.o)
set(KERNEL_ENTRY_OBJ ${OBJ_DIR}/kernel/kernel_entry.o)
set(KERNEL_OBJ ${OBJ_DIR}/kernel/kernel.o)

# === Output binaries ===
set(BOOTLOADER_BIN ${BIN_DIR}/bootloader.bin)
set(KERNEL_ELF ${BIN_DIR}/kernel.elf)  # ELF with debug symbols
set(KERNEL_BIN ${BIN_DIR}/kernel.bin)  # Raw binary
set(BOOTABLE_IMG ${IMG_DIR}/os.img)

# === Assembling bootloader ===
add_custom_command(OUTPUT ${BOOT_OBJ}
    COMMAND ${CMAKE_ASM_COMPILER} -g --32 -o ${BOOT_OBJ} ${BOOTLOADER_DIR}/boot.s
    DEPENDS ${BOOTLOADER_DIR}/boot.s
    COMMENT "Assembling bootloader"
    VERBATIM
)

# === Assembling kernel entry ===
add_custom_command(OUTPUT ${KERNEL_ENTRY_OBJ}
    COMMAND ${CMAKE_ASM_COMPILER} -g --32 -o ${KERNEL_ENTRY_OBJ} ${KERNEL_DIR}/kernel_entry.s
    DEPENDS ${KERNEL_DIR}/kernel_entry.s
    COMMENT "Assembling kernel entry"
    VERBATIM
)

# === Compiling kernel.c ===
add_custom_command(OUTPUT ${KERNEL_OBJ}
    COMMAND ${CMAKE_C_COMPILER} -g -O0 -ffreestanding -nostdlib -Wall -Wextra -c -o ${KERNEL_OBJ} ${KERNEL_DIR}/kernel.c
    DEPENDS ${KERNEL_DIR}/kernel.c
    COMMENT "Compiling kernel C code"
    VERBATIM
)

# === Linking bootloader ===
add_custom_command(OUTPUT ${BOOTLOADER_BIN}
    COMMAND ${CMAKE_LINKER} -T ${BOOTLOADER_DIR}/linker.ld -o ${BOOTLOADER_BIN} ${BOOT_OBJ}
    DEPENDS ${BOOT_OBJ} ${BOOTLOADER_DIR}/linker.ld
    COMMENT "Linking bootloader with ${CMAKE_LINKER}"
    VERBATIM
)

# === Linking kernel (create ELF with debug symbols first) ===
add_custom_command(OUTPUT ${KERNEL_ELF}
    COMMAND ${CMAKE_LINKER} -T ${KERNEL_DIR}/linker.ld -o ${KERNEL_ELF} ${KERNEL_ENTRY_OBJ} ${KERNEL_OBJ}
    DEPENDS ${KERNEL_ENTRY_OBJ} ${KERNEL_OBJ} ${KERNEL_DIR}/linker.ld
    COMMENT "Linking kernel ELF with debug symbols"
    VERBATIM
)

# === Convert ELF to binary ===
add_custom_command(OUTPUT ${KERNEL_BIN}
    COMMAND ${CMAKE_OBJCOPY} -O binary ${KERNEL_ELF} ${KERNEL_BIN}
    DEPENDS ${KERNEL_ELF}
    COMMENT "Converting kernel ELF to binary"
    VERBATIM
)

# === Creating bootable image ===
add_custom_command(OUTPUT ${BOOTABLE_IMG}
    COMMAND dd if=/dev/zero of=${BOOTABLE_IMG} bs=512 count=2880
    COMMAND dd if=${BOOTLOADER_BIN} of=${BOOTABLE_IMG} bs=512 count=1 conv=notrunc
    COMMAND dd if=${KERNEL_BIN} of=${BOOTABLE_IMG} bs=512 seek=1 conv=notrunc
    DEPENDS ${BOOTLOADER_BIN} ${KERNEL_BIN}
    COMMENT "Creating bootable floppy image"
    VERBATIM
)

# === Add top-level build target ===
add_custom_target(build ALL
    DEPENDS ${BOOTABLE_IMG}
    COMMENT "Build complete: ${BOOTABLE_IMG}"
)

add_custom_target(run
    COMMAND qemu-system-i386 -drive format=raw,file=${BOOTABLE_IMG} -no-reboot -no-shutdown
    DEPENDS build
    COMMENT "Running in QEMU"
)

# === Debug targets ===
add_custom_target(debug
    COMMAND qemu-system-i386 -drive format=raw,file=${BOOTABLE_IMG} -s -S -no-reboot -no-shutdown
    DEPENDS build
    COMMENT "Starting QEMU with GDB server (waiting for connection on :1234)"
)

# === NEW: GDB debug session target ===
add_custom_target(gdb-debug
    COMMAND echo "Starting GDB debug session..."
    COMMAND echo "Run this in another terminal:"
    COMMAND echo "gdb ${KERNEL_ELF}"
    COMMAND echo "Then in GDB:"
    COMMAND echo "(gdb) target remote localhost:1234"
    COMMAND echo "(gdb) set architecture i386"
    COMMAND echo "(gdb) symbol-file ${KERNEL_ELF}"
    COMMAND echo "(gdb) add-symbol-file ${KERNEL_ELF} 0x1000"
    COMMAND echo "(gdb) break _start"
    COMMAND echo "(gdb) break protected_mode"
    COMMAND echo "(gdb) continue"
    COMMAND echo ""
    COMMAND echo "Starting QEMU with GDB server..."
    COMMAND qemu-system-i386 -drive format=raw,file=${BOOTABLE_IMG} -s -S -no-reboot -no-shutdown
    DEPENDS build
    COMMENT "Debug session with instructions"
)

# === NEW: Show debug info ===
add_custom_target(debug-info
    COMMAND echo "=== Kernel Symbol Table ==="
    COMMAND i686-elf-objdump -t ${KERNEL_ELF}
    COMMAND echo ""
    COMMAND echo "=== Kernel Disassembly ==="
    COMMAND i686-elf-objdump -d ${KERNEL_ELF}
    COMMAND echo ""
    COMMAND echo "=== Section Headers ==="
    COMMAND i686-elf-objdump -h ${KERNEL_ELF}
    DEPENDS ${KERNEL_ELF}
    COMMENT "Show kernel debug information"
)

# === NEW: Automated GDB script ===
add_custom_target(create-gdb-script
    COMMAND echo "# GDB script for kernel debugging" > ${BUILD_DIR}/debug.gdb
    COMMAND echo "target remote localhost:1234" >> ${BUILD_DIR}/debug.gdb
    COMMAND echo "set architecture i386" >> ${BUILD_DIR}/debug.gdb
    COMMAND echo "symbol-file ${KERNEL_ELF}" >> ${BUILD_DIR}/debug.gdb
    COMMAND echo "add-symbol-file ${KERNEL_ELF} 0x1000" >> ${BUILD_DIR}/debug.gdb
    COMMAND echo "break _start" >> ${BUILD_DIR}/debug.gdb
    COMMAND echo "break protected_mode" >> ${BUILD_DIR}/debug.gdb
    COMMAND echo "break kernel_main" >> ${BUILD_DIR}/debug.gdb
    COMMAND echo "continue" >> ${BUILD_DIR}/debug.gdb
    COMMAND echo "layout asm" >> ${BUILD_DIR}/debug.gdb
    COMMAND echo "GDB script created at ${BUILD_DIR}/debug.gdb"
    COMMAND echo "Usage: gdb -x ${BUILD_DIR}/debug.gdb"
    DEPENDS ${KERNEL_ELF}
    COMMENT "Create GDB debugging script"
)

add_custom_target(debug-monitor
    COMMAND qemu-system-i386 -drive format=raw,file=${BOOTABLE_IMG} -monitor stdio -no-reboot -no-shutdown -s -S
    DEPENDS build
    COMMENT "Running QEMU with monitor console"
)

add_custom_target(debug-verbose
    COMMAND qemu-system-i386 -drive format=raw,file=${BOOTABLE_IMG} -d int,cpu_reset,exec -D qemu.log -no-reboot -no-shutdown
    DEPENDS build
    COMMENT "Running QEMU with verbose logging to qemu.log"
)

# === Clean target for organized cleanup ===
add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${OBJ_DIR}
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${BIN_DIR}
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${IMG_DIR}
    COMMAND ${CMAKE_COMMAND} -E remove ${BUILD_DIR}/debug.gdb
    COMMAND ${CMAKE_COMMAND} -E remove ${BUILD_DIR}/qemu.log
    COMMAND ${CMAKE_COMMAND} -E make_directory ${OBJ_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${OBJ_DIR}/bootloader
    COMMAND ${CMAKE_COMMAND} -E make_directory ${OBJ_DIR}/kernel
    COMMAND ${CMAKE_COMMAND} -E make_directory ${BIN_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${IMG_DIR}
    COMMENT "Cleaning all build artifacts and recreating directories"
)