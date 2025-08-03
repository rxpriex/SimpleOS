# src/bootloader/boot.s
# 16-bit bootloader transitioning to 32-bit real mode and loading the kernel

.code16
.text
.globl _start

_start:
    # Set up segments and stack
    cli
    xor %ax, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %ss
    mov $0x7C00, %sp
    sti

    # Print startup message
    mov $msg_start, %si
    call print_string

    # Save drive number
    mov %dl, drive_num
    
    # Print drive number
    mov $msg_drive, %si
    call print_string
    mov drive_num, %al
    call print_hex_byte
    mov $msg_newline, %si
    call print_string
    
    # Reset disk system
    mov $0x00, %ah
    mov drive_num, %dl
    int $0x13
    jc disk_error
    
    # Print "Disk reset OK"
    mov $msg_reset_ok, %si
    call print_string

    # Load kernel from disk (to 0x10000 linear address)
    mov $0x02, %ah          # Read sectors
    mov $0x10, %al          # Read 16 sectors (8KB)
    mov $0x00, %ch          # Cylinder 0
    mov $0x02, %cl          # Sector 2
    mov $0x00, %dh          # Head 0
    mov drive_num, %dl
    mov $0x1000, %bx        # ES:BX = 0x1000:0x0000 (linear 0x10000)
    mov %bx, %es
    xor %bx, %bx
    int $0x13
    jc disk_error
    
    # Print "Kernel loaded OK"
    mov $msg_loaded_ok, %si
    call print_string

    # ===== BEGIN PROTECTED MODE SETUP =====
    cli

    # 1. Load GDT
    lgdtl (gdt_descriptor)

    # 2. Enable A20 line (fast method)
    in $0x92, %al
    or $2, %al
    out %al, $0x92

    # 3. Enable protected mode
    mov %cr0, %eax
    or $1, %eax
    mov %eax, %cr0

    # 4. Far jump to 32-bit code
    ljmp $0x08, $protected_mode

.code32
protected_mode:
    # Set up protected mode segments
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss

    # Set up stack (0x90000 is safe for early kernel)
    mov $0x90000, %esp

    # Jump to kernel (now at 0x10000 linear)
    jmp 0x10000

.code16
disk_error:
    mov $msg_disk_error, %si
    call print_string
    mov %ah, %al
    call print_hex_byte
    jmp hang

hang:
    mov $msg_hang, %si
    call print_string
    hlt
    jmp hang

# GDT for protected mode
.align 4
gdt_start:
    .quad 0x0000000000000000  # Null descriptor
gdt_code:
    .word 0xFFFF             # Limit (bits 0-15)
    .word 0x0000             # Base (bits 0-15)
    .byte 0x00               # Base (bits 16-23)
    .byte 0x9A               # Access byte (code, ring 0)
    .byte 0xCF               # Flags + Limit (bits 16-19)
    .byte 0x00               # Base (bits 24-31)
gdt_data:
    .word 0xFFFF             # Limit
    .word 0x0000             # Base
    .byte 0x00               # Base
    .byte 0x92               # Access byte (data, ring 0)
    .byte 0xCF               # Flags + Limit
    .byte 0x00               # Base
gdt_end:

gdt_descriptor:
    .word gdt_end - gdt_start - 1
    .long gdt_start

# Print string pointed to by SI
print_string:
    push %ax
    push %bx
    mov $0x0E, %ah          # Teletype output
    mov $0x07, %bl          # White on black
print_loop:
    lodsb                   # Load byte from SI into AL
    test %al, %al           # Check if null terminator
    jz print_done
    int $0x10               # BIOS video interrupt
    jmp print_loop
print_done:
    pop %bx
    pop %ax
    ret

# Print AL as 2-digit hex
print_hex_byte:
    push %ax
    push %cx
    mov %al, %cl            # Save original value
    
    # Print high nibble
    shr $4, %al
    add $0x30, %al          # Convert to ASCII
    cmp $0x39, %al
    jle print_high
    add $0x07, %al          # A-F
print_high:
    mov $0x0E, %ah
    int $0x10
    
    # Print low nibble
    mov %cl, %al
    and $0x0F, %al
    add $0x30, %al
    cmp $0x39, %al
    jle print_low
    add $0x07, %al          # A-F
print_low:
    mov $0x0E, %ah
    int $0x10
    
    pop %cx
    pop %ax
    ret

# Messages
msg_start:      .asciz "Bootloader starting...\r\n"
msg_drive:      .asciz "Boot drive: 0x"
msg_newline:    .asciz "\r\n"
msg_reset_ok:   .asciz "Disk reset OK\r\n"
msg_loading:    .asciz "Loading kernel...\r\n"
msg_loaded_ok:  .asciz "Kernel loaded OK\r\n"
msg_first_byte: .asciz "First byte: 0x"
msg_jumping:    .asciz "\r\nPerforming long jump to kernel[32]\r\n"
msg_disk_error: .asciz "Disk error! Code: 0x"
msg_hang:       .asciz "\r\nSystem halted.\r\n"

# Variables
drive_num:      .byte 0x00

# Pad to 510 bytes and add boot signature
.fill 510-(.-_start), 1, 0
.word 0xAA55