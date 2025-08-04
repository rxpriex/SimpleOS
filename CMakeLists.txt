cmake_minimum_required(VERSION 3.10)
project(SOS C ASM)

# === Cross-Compiler Toolchain ===
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_C_COMPILER i686-elf-gcc)
set(CMAKE_ASM_COMPILER i686-elf-as)
set(CMAKE_LINKER i686-elf-ld)
set(CMAKE_OBJCOPY i686-elf-objcopy)

# Compilation flags with debug support
set(CMAKE_C_FLAGS -g -O0 -ffreestanding -nostdlib -Wall -Wextra)
set(CMAKE_ASM_FLAGS -g --32)

# === Project Structure ===
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(BOOTLOADER_DIR ${SRC_DIR}/arch/i686/bootloader)
set(KERNEL_DIR ${SRC_DIR}/kernel)
set(DRIVERS_DIR ${SRC_DIR}/drivers)
set(SYSTEM_DIR ${SRC_DIR}/system)

# === Source File Lists ===
set(BOOTLOADER_ASM_FILES
    ${BOOTLOADER_DIR}/boot.s
)

set(KERNEL_ASM_FILES
    ${KERNEL_DIR}/kernel_entry.s
    ${DRIVERS_DIR}/keyboard_util.s
)

set(KERNEL_C_FILES
    ${KERNEL_DIR}/kernel.c
    ${DRIVERS_DIR}/keyboard.c
    ${DRIVERS_DIR}/screen_text.c
    ${SYSTEM_DIR}/shell.c
)

# === Output Directory Structure ===
set(BUILD_DIR ${CMAKE_BINARY_DIR})
set(OBJ_DIR ${BUILD_DIR}/obj)
set(BIN_DIR ${BUILD_DIR}/bin)
set(IMG_DIR ${BUILD_DIR}/images)

# Create output directories
file(MAKE_DIRECTORY ${OBJ_DIR}/bootloader)
file(MAKE_DIRECTORY ${OBJ_DIR}/kernel)
file(MAKE_DIRECTORY ${BIN_DIR})
file(MAKE_DIRECTORY ${IMG_DIR})

# === Generate Object Files ===
# Bootloader objects
set(BOOTLOADER_OBJS)
foreach(ASM_FILE ${BOOTLOADER_ASM_FILES})
    file(RELATIVE_PATH ASM_REL_PATH ${CMAKE_SOURCE_DIR} ${ASM_FILE})
    string(REGEX REPLACE "[^a-zA-Z0-9]" "_" ASM_NAME ${ASM_REL_PATH})
    set(OBJ_FILE ${OBJ_DIR}/bootloader/${ASM_NAME}.o)
    list(APPEND BOOTLOADER_OBJS ${OBJ_FILE})
    add_custom_command(
        OUTPUT ${OBJ_FILE}
        COMMAND ${CMAKE_ASM_COMPILER} ${CMAKE_ASM_FLAGS} -o ${OBJ_FILE} ${ASM_FILE}
        DEPENDS ${ASM_FILE}
        COMMENT "Assembling ${ASM_REL_PATH}"
        VERBATIM
    )
endforeach()

# Kernel assembly objects
set(KERNEL_ASM_OBJS)
foreach(ASM_FILE ${KERNEL_ASM_FILES})
    file(RELATIVE_PATH ASM_REL_PATH ${CMAKE_SOURCE_DIR} ${ASM_FILE})
    string(REGEX REPLACE "[^a-zA-Z0-9]" "_" ASM_NAME ${ASM_REL_PATH})
    set(OBJ_FILE ${OBJ_DIR}/kernel/${ASM_NAME}.o)
    list(APPEND KERNEL_ASM_OBJS ${OBJ_FILE})
    add_custom_command(
        OUTPUT ${OBJ_FILE}
        COMMAND ${CMAKE_ASM_COMPILER} ${CMAKE_ASM_FLAGS} -o ${OBJ_FILE} ${ASM_FILE}
        DEPENDS ${ASM_FILE}
        COMMENT "Assembling ${ASM_REL_PATH}"
        VERBATIM
    )
endforeach()

# Kernel C objects
set(KERNEL_C_OBJS)
foreach(C_FILE ${KERNEL_C_FILES})
    file(RELATIVE_PATH C_REL_PATH ${CMAKE_SOURCE_DIR} ${C_FILE})
    string(REGEX REPLACE "[^a-zA-Z0-9]" "_" C_NAME ${C_REL_PATH})
    set(OBJ_FILE ${OBJ_DIR}/kernel/${C_NAME}.o)
    list(APPEND KERNEL_C_OBJS ${OBJ_FILE})
    add_custom_command(
        OUTPUT ${OBJ_FILE}
        COMMAND ${CMAKE_C_COMPILER} ${CMAKE_C_FLAGS} -c -o ${OBJ_FILE} ${C_FILE}
        DEPENDS ${C_FILE}
        COMMENT "Compiling ${C_REL_PATH}"
        VERBATIM
    )
endforeach()

# Combine all kernel objects
set(KERNEL_OBJS ${KERNEL_ASM_OBJS} ${KERNEL_C_OBJS})

# === Output Binaries ===
set(BOOTLOADER_BIN ${BIN_DIR}/bootloader.bin)
set(KERNEL_ELF ${BIN_DIR}/kernel.elf)
set(KERNEL_BIN ${BIN_DIR}/kernel.bin)
set(BOOTABLE_IMG ${IMG_DIR}/os.img)

# === Linking Bootloader ===
add_custom_command(
    OUTPUT ${BOOTLOADER_BIN}
    COMMAND ${CMAKE_LINKER} -T ${BOOTLOADER_DIR}/linker.ld -o ${BOOTLOADER_BIN} ${BOOTLOADER_OBJS}
    DEPENDS ${BOOTLOADER_OBJS} ${BOOTLOADER_DIR}/linker.ld
    COMMENT "Linking bootloader"
    VERBATIM
)

# === Linking Kernel (ELF with debug symbols) ===
add_custom_command(
    OUTPUT ${KERNEL_ELF}
    COMMAND ${CMAKE_LINKER} -T ${KERNEL_DIR}/linker.ld -o ${KERNEL_ELF} ${KERNEL_OBJS}
    DEPENDS ${KERNEL_OBJS} ${KERNEL_DIR}/linker.ld
    COMMENT "Linking kernel ELF with debug symbols"
    VERBATIM
)

# === Convert ELF to Binary ===
add_custom_command(
    OUTPUT ${KERNEL_BIN}
    COMMAND ${CMAKE_OBJCOPY} -O binary ${KERNEL_ELF} ${KERNEL_BIN}
    DEPENDS ${KERNEL_ELF}
    COMMENT "Converting kernel ELF to binary"
    VERBATIM
)

# === Creating Bootable Image ===
add_custom_command(
    OUTPUT ${BOOTABLE_IMG}
    COMMAND dd if=/dev/zero of=${BOOTABLE_IMG} bs=512 count=2880
    COMMAND dd if=${BOOTLOADER_BIN} of=${BOOTABLE_IMG} bs=512 count=1 conv=notrunc
    COMMAND dd if=${KERNEL_BIN} of=${BOOTABLE_IMG} bs=512 seek=1 conv=notrunc
    DEPENDS ${BOOTLOADER_BIN} ${KERNEL_BIN}
    COMMENT "Creating bootable floppy image"
    VERBATIM
)

# === Top-Level Build Target ===
add_custom_target(build ALL
    DEPENDS ${BOOTABLE_IMG}
    COMMENT "Build complete: ${BOOTABLE_IMG}"
)

# === Run Target ===
add_custom_target(run
    COMMAND qemu-system-i386 -drive format=raw,file=${BOOTABLE_IMG} -no-reboot -no-shutdown
    DEPENDS build
    COMMENT "Running in QEMU"
)

# === Debug Targets ===
add_custom_target(debug
    COMMAND qemu-system-i386 -drive format=raw,file=${BOOTABLE_IMG} -s -S -no-reboot -no-shutdown
    DEPENDS build
    COMMENT "Starting QEMU with GDB server (waiting on :1234)"
)

add_custom_target(gdb-debug
    COMMAND echo "Starting GDB debug session..."
    COMMAND echo "Run: gdb ${KERNEL_ELF}"
    COMMAND echo "Then in GDB: target remote localhost:1234"
    COMMAND echo "set architecture i386"
    COMMAND echo "symbol-file ${KERNEL_ELF}"
    COMMAND echo "add-symbol-file ${KERNEL_ELF} 0x1000"
    COMMAND echo "break _start"
    COMMAND echo "break protected_mode"
    COMMAND echo "break kernel_main"
    COMMAND echo "continue"
    COMMAND qemu-system-i386 -drive format=raw,file=${BOOTABLE_IMG} -s -S -no-reboot -no-shutdown
    DEPENDS build
    COMMENT "Debug session with instructions"
)

add_custom_target(debug-info
    COMMAND echo "=== Kernel Symbol Table ==="
    COMMAND i686-elf-objdump -t ${KERNEL_ELF}
    COMMAND echo "=== Kernel Disassembly ==="
    COMMAND i686-elf-objdump -d ${KERNEL_ELF}
    COMMAND echo "=== Section Headers ==="
    COMMAND i686-elf-objdump -h ${KERNEL_ELF}
    DEPENDS ${KERNEL_ELF}
    COMMENT "Show kernel debug information"
)

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

# === Clean Target ===
add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${OBJ_DIR}
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${BIN_DIR}
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${IMG_DIR}
    COMMAND ${CMAKE_COMMAND} -E remove ${BUILD_DIR}/debug.gdb
    COMMAND ${CMAKE_COMMAND} -E remove ${BUILD_DIR}/qemu.log
    COMMAND ${CMAKE_COMMAND} -E make_directory ${OBJ_DIR}/bootloader
    COMMAND ${CMAKE_COMMAND} -E make_directory ${OBJ_DIR}/kernel
    COMMAND ${CMAKE_COMMAND} -E make_directory ${BIN_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${IMG_DIR}
    COMMENT "Cleaning all build artifacts and recreating directories"
)