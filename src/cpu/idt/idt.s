# src\cpu\idt\idt.s
# asm util file for loading the idt from idt.c

.section .text
.global idt_load

idt_load:
    movl 4(%esp), %eax  
    lidt (%eax) 
    ret