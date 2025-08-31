# src\arch\i686\boot\boot.s
# 16-bit bootloader transitioning to 32-bit real mode and loading the kernel

.code16

.section .data
    .struct 0
boot_data_kernel_address:
    .long 0
boot_data_kernel_size:
    .long 0
boot_data_memory_size:
    .long 0
boot_data_size:

.equ BOOT_DATA_SIZE, boot_data_size

.section .text
.global _start

_start:
    cli
    xor %ax, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %ss
    mov $0x7C00, %sp
    sti

    mov %dl, drive_num
    
    # Reset disk system
    mov $0x00, %ah
    mov drive_num, %dl
    int $0x13
    jc disk_error

    mov $0x02, %ah          # Read sectors
    mov $0x40, %al          # Read 64 sectors (32KB) TODO: prevent kernel from not being read all the way
    mov $0x00, %ch          # Cylinder 0
    mov $0x02, %cl          # Sector 2
    mov $0x00, %dh          # Head 0
    mov drive_num, %dl
    mov $0x1000, %bx        # ES:BX = 0x1000:0x0000 (linear 0x10000)
    mov %bx, %es
    xor %bx, %bx
    int $0x13
    jc disk_error
    cli

    call populate_boot_data

    lgdtl (gdt_descriptor)

    in $0x92, %al
    or $2, %al
    out %al, $0x92

    mov %cr0, %eax
    or $1, %eax
    mov %eax, %cr0

    ljmp $0x08, $protected_mode

read_cmos_memory:
    # Read extended memory from CMOS 0x30-0x31
    movb    $0x30, %al          # CMOS register 0x30
    outb    %al, $0x70          # Select CMOS register
    inb     $0x71, %al          # Read low byte
    movb    %al, %bl
    
    movb    $0x31, %al          # CMOS register 0x31  
    outb    %al, $0x70          # Select CMOS register
    inb     $0x71, %al          # Read high byte
    movb    %al, %bh
    
    ret

populate_boot_data:
    movl $boot_data_struct, %eax
    movl kernel_load, %ebx
    movl %ebx, boot_data_kernel_address(%eax)
    movl $0x8000, %ebx  # 32KB (64 sectors)
    movl %ebx, boot_data_kernel_size(%eax)
    xor %ebx,%ebx
    xor %eax,%eax
    call read_cmos_memory
    movl $boot_data_struct, %eax
    movl %ebx, boot_data_memory_size(%eax)
    ret

.code32
protected_mode:
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss

    # Set up stack
    mov $0x90000, %esp

    pushl $boot_data_struct
    pushl boot_magic

    jmp *kernel_load

.code16
disk_error:
    jmp hang

hang:
    hlt
    jmp hang

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

drive_num:      .byte 0x00
boot_magic:     .long 0x07A37AAF
kernel_load:    .long 0x10000

boot_data_struct:
    .space BOOT_DATA_SIZE

# Pad to 510 bytes and add boot signature
.fill 510-(.-_start), 1, 0
.word 0xAA55