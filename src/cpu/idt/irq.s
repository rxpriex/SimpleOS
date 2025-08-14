// src\cpu\idt\irq.s
// file implementing the irq handlers from handler.h

.macro IRQ num, remap
.global irq\num
irq\num:
    cli 
    pushl $0 
    pushl $\remap
    jmp irq_stub
.endm

IRQ 0,  32    # Timer
IRQ 1,  33    # Keyboard
IRQ 2,  34    # Cascade
IRQ 3,  35    # COM2/4
IRQ 4,  36    # COM1/3
IRQ 5,  37    # LPT2/Sound
IRQ 6,  38    # Floppy
IRQ 7,  39    # LPT1
IRQ 8,  40    # RTC
IRQ 9,  41    # Free
IRQ 10, 42    # Free
IRQ 11, 43    # Free
IRQ 12, 44    # PS/2 Mouse
IRQ 13, 45    # Math Coprocessor
IRQ 14, 46    # Primary ATA
IRQ 15, 47    # Secondary ATA

irq_stub:
    pushal

    movw %ds, %ax
    pushl %eax

    movw $0x10,%ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs

    call irq_handler

    popl %eax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs

    popal

    addl $8, %esp

    iret
