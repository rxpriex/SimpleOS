# src/kernel/kernel_entry.s
# 32-bit kernel entry point

.code32
.text
.globl _start
.global clear_screen
.global get_cpu_info

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
    
    mov $0xB8000, %edi
    mov $0x0720, %ax     
    mov $2000, %ecx  
    rep stosw                  
    
    pop %edi
    pop %ecx
    pop %eax
    ret

get_cpu_info:
    # Function prologue: Save base pointer and set up stack frame
    pushl %ebp
    movl %esp, %ebp

    # Parameter: pointer to cpu_info_t struct at 8(%ebp)
    movl 8(%ebp), %edi  # EDI = pointer to cpu_info_t

    # Call CPUID with EAX = 0 to get vendor string
    movl $0, %eax
    cpuid

    # CPUID returns vendor string in EBX, EDX, ECX
    # Store 12-byte vendor string (4 bytes per register)
    movl %ebx, (%edi)       # First 4 bytes
    movl %edx, 4(%edi)      # Next 4 bytes
    movl %ecx, 8(%edi)      # Last 4 bytes
    movb $0, 12(%edi)       # Null-terminate the string

    # Function epilogue: Restore stack and return
    movl %ebp, %esp
    popl %ebp
    ret