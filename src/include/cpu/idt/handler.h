// src\include\cpu\idt\handler.h
// a header file defining the handler for isr and irq
// and macros for the interrupt types

#ifndef HANDLER_H
#define HANDLER_H

#include <stdint.h>
#include <cpu/interrupts/interrupts.h>

typedef struct {
    uint32_t ds;                                    // Data segment selector
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pushal
    uint32_t int_no, err_code;                      // Interrupt number and error code
    uint32_t eip, cs, eflags, useresp, ss;         // Pushed by processor automatically
}cpu_i;

void print_cpu_state(cpu_i*);

void isr_handler(cpu_i);
void irq_handler(cpu_i);

#endif