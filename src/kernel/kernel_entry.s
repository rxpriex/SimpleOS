# src/kernel/kernel_entry.s
# 32-bit kernel entry point

.code32
.text
.globl _start

_start:
protected_mode:
    # Set up data segments
    mov $0x10, %ax              # Data segment selector
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss
    
    # Set up stack (place it somewhere safe, like 0x90000)
    mov $0x90000, %esp
    
    # Clear the screen
    call clear_screen
    
    # Call C kernel main function
    call kernel_main
    
    # If kernel_main returns, hang
hang:
    hlt
    jmp hang

# Clear VGA text mode screen (80x25, white on black)
clear_screen:
    push %eax
    push %ecx
    push %edi
    
    mov $0xB8000, %edi          # VGA text buffer
    mov $0x1720, %ax            # Space character (0x20) with white on black (0x07)
    mov $2000, %ecx             # 80*25 = 2000 characters
    rep stosw                   # Fill screen with spaces
    
    pop %edi
    pop %ecx
    pop %eax
    ret