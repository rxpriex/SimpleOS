cmake_minimum_required(VERSION 3.10)
project(SOS C ASM)

# Export compile commands for LSP support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# === Cross-Compiler Toolchain ===
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_C_COMPILER i686-elf-gcc)
set(CMAKE_ASM_COMPILER i686-elf-as)
set(CMAKE_LINKER i686-elf-ld)
set(CMAKE_OBJCOPY i686-elf-objcopy)

# === Project Structure ===
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(BOOTLOADER_DIR ${SRC_DIR}/arch/i686/boot)
set(KERNEL_DIR ${SRC_DIR}/kernel)
set(CPU_DIR ${SRC_DIR}/cpu)
set(DRIVERS_DIR ${SRC_DIR}/drivers)
set(USER_DIR ${SRC_DIR}/user)
set(LINKER_DIR ${SRC_DIR}/linker)
set(INCLUDE_DIR ${SRC_DIR}/include)

# === Include Directories ===
set(INCLUDE_PATHS
    ${INCLUDE_DIR}
    ${INCLUDE_DIR}/kernel
    ${INCLUDE_DIR}/drivers
    ${INCLUDE_DIR}/drivers/keyboard
    ${INCLUDE_DIR}/drivers/console
    ${INCLUDE_DIR}/user
    ${INCLUDE_DIR}/arch/i686
    ${INCLUDE_DIR}/lib
    ${INCLUDE_DIR}/kernel/memory
    ${INCLUDE_DIR}/cpu
)

# Build include flags as a list for C compiler
set(INCLUDE_FLAGS)
foreach(INCLUDE_PATH ${INCLUDE_PATHS})
    list(APPEND INCLUDE_FLAGS "-I${INCLUDE_PATH}")
endforeach()

# Compilation flags with debug support and include paths
set(CMAKE_C_FLAGS -g -O0 -ffreestanding -nostdlib -Wall -Wextra ${INCLUDE_FLAGS})
set(CMAKE_ASM_FLAGS -g --32)

# === Source File Lists ===
set(BOOTLOADER_ASM_FILES
    ${BOOTLOADER_DIR}/boot.s
)

set(KERNEL_ASM_FILES
    ${KERNEL_DIR}/init/kernel_entry.s
    ${KERNEL_DIR}/memory/memory_util.s
    ${DRIVERS_DIR}/keyboard/keyboard_util.s
    ${CPU_DIR}/idt/irq.s
    ${CPU_DIR}/idt/isr.s
    ${CPU_DIR}/idt/idt.s
)

set(KERNEL_C_FILES
    ${KERNEL_DIR}/init/kernel.c
    ${KERNEL_DIR}/memory/memory.c
    ${DRIVERS_DIR}/keyboard/keyboard.c
    ${DRIVERS_DIR}/console/vga_console.c
    ${DRIVERS_DIR}/port/port_io.c
    ${USER_DIR}/shell.c
    ${CPU_DIR}/idt/idt.c
    ${CPU_DIR}/idt/handler.c
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
    COMMAND ${CMAKE_LINKER} -T ${LINKER_DIR}/boot.ld -o ${BOOTLOADER_BIN} ${BOOTLOADER_OBJS}
    DEPENDS ${BOOTLOADER_OBJS} ${LINKER_DIR}/boot.ld
    COMMENT "Linking bootloader"
    VERBATIM
)

# === Linking Kernel (ELF with debug symbols) ===
add_custom_command(
    OUTPUT ${KERNEL_ELF}
    COMMAND ${CMAKE_LINKER} -T ${LINKER_DIR}/kernel.ld -o ${KERNEL_ELF} ${KERNEL_OBJS}
    DEPENDS ${KERNEL_OBJS} ${LINKER_DIR}/kernel.ld
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

# === Print Include Information ===
add_custom_target(show-includes
    COMMAND echo "=== Include Directories ==="
    COMMAND echo "${INCLUDE_PATHS}"
    COMMAND echo "=== C Compiler Flags ==="
    COMMAND echo "${CMAKE_C_FLAGS}"
    COMMENT "Show configured include paths"
)