# src/kernel/kernel_entry.s
# 32-bit kernel entry point

.code32
.section .text
.global _start
.global clear_screen
.global get_cpu_info

_start:
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
    pushl %ebp
    movl %esp, %ebp

    movl 8(%ebp), %edi  # EDI = pointer to cpu_info_t

    # Call CPUID with EAX = 0 to get vendor string
    movl $0, %eax
    cpuid

    movl %ebx, (%edi)     
    movl %edx, 4(%edi)    
    movl %ecx, 8(%edi)
    movb $0, 12(%edi)       # Null-terminate the string

    movl %ebp, %esp
    popl %ebp
    ret