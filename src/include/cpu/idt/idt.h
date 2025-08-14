// src\include\cpu\idt\idt.h
// definition file for the idt and related files

#ifndef IDT_DEF_H
#define IDT_DEF_H

#define IDT_TYPE_INTERRUPT_GATE  0x8E  // 32-bit interrupt gate
#define IDT_TYPE_TRAP_GATE       0x8F  // 32-bit trap gate

#include <stdint.h>

struct idt_entry{
    uint16_t handler_low;
    uint16_t selector;
    uint8_t zero;
    uint8_t type;
    uint16_t handler_high;
} __attribute__((packed));

struct idt_base{
    uint16_t limit;
    uint32_t base_addr;
} __attribute__((packed));

void init_idt(void);
void set_gate(uint8_t,uint32_t,uint16_t,uint8_t);
uint32_t read_handler(uint8_t);

// ISR handlers(isr.s)
extern void isr0(void);   // Divide by Zero
extern void isr1(void);   // Debug
extern void isr2(void);   // NMI
extern void isr3(void);   // Breakpoint
extern void isr4(void);   // Overflow
extern void isr5(void);   // Bound Range Exceeded
extern void isr6(void);   // Invalid Opcode
extern void isr7(void);   // Device Not Available
extern void isr8(void);   // Double Fault
extern void isr9(void);   // Coprocessor Segment Overrun
extern void isr10(void);  // Invalid TSS
extern void isr11(void);  // Segment Not Present
extern void isr12(void);  // Stack Segment Fault
extern void isr13(void);  // General Protection Fault
extern void isr14(void);  // Page Fault
extern void isr15(void);  // Reserved
extern void isr16(void);  // x87 Floating Point Exception
extern void isr17(void);  // Alignment Check
extern void isr18(void);  // Machine Check
extern void isr19(void);  // SIMD Floating Point Exception
extern void isr20(void);  // Virtualization Exception
extern void isr21(void);  // Control Protection Exception

// IRQ handlers(irq.s)
extern void irq0(void);   // Timer
extern void irq1(void);   // Keyboard
extern void irq2(void);   // Cascade
extern void irq3(void);   // COM2/4
extern void irq4(void);   // COM1/3
extern void irq5(void);   // LPT2/Sound
extern void irq6(void);   // Floppy
extern void irq7(void);   // LPT1
extern void irq8(void);   // RTC
extern void irq9(void);   // Free
extern void irq10(void);  // Free
extern void irq11(void);  // Free
extern void irq12(void);  // PS/2 Mouse
extern void irq13(void);  // Math Coprocessor
extern void irq14(void);  // Primary ATA
extern void irq15(void);  // Secondary ATA

#endif