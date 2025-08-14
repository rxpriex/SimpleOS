// src\cpu\idt\isr.s
// file implementing the isr handlers from handler.h

.section .text

.macro ISR_NOERRCODE id
.global isr\id
isr\id:
    cli
    pushl $0
    pushl $\id
    jmp isr_stub
.endm

.macro ISR_ERRCODE id
.global isr\id
isr\id:
    cli
    pushl $\id
    jmp isr_stub
.endm

ISR_NOERRCODE 0     # Divide by Zero
ISR_NOERRCODE 1     # Debug
ISR_NOERRCODE 2     # NMI
ISR_NOERRCODE 3     # Breakpoint
ISR_NOERRCODE 4     # Overflow
ISR_NOERRCODE 5     # Bound Range Exceeded
ISR_NOERRCODE 6     # Invalid Opcode
ISR_NOERRCODE 7     # Device Not Available
ISR_ERRCODE   8     # Double Fault
ISR_NOERRCODE 9     # Coprocessor Segment Overrun
ISR_ERRCODE   10    # Invalid TSS
ISR_ERRCODE   11    # Segment Not Present
ISR_ERRCODE   12    # Stack Segment Fault
ISR_ERRCODE   13    # General Protection Fault
ISR_ERRCODE   14    # Page Fault
ISR_NOERRCODE 15    # Reserved
ISR_NOERRCODE 16    # x87 Floating Point Exception
ISR_ERRCODE   17    # Alignment Check
ISR_NOERRCODE 18    # Machine Check
ISR_NOERRCODE 19    # SIMD Floating Point Exception
ISR_NOERRCODE 20    # Virtualization Exception
ISR_ERRCODE   21    # Control Protection Exception

isr_stub:
    pushal

    movw %ds, %ax
    pushl %eax

    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs

    call isr_handler

    popl %eax
    movw %ax,%ds
    movw %ax,%es
    movw %ax,%fs
    movw %ax,%gs

    popal

    addl $8, %esp

    iret