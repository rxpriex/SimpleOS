// src\cpu\idt\idt.c
// implementation file for idt.h

#include <cpu/idt/idt.h>
#include <drivers/console/vga.h>

static struct idt_entry idt[256];
static struct idt_base idt_head;

extern void idt_load(uint32_t);

uint32_t read_handler(uint8_t code){
    uint32_t handler = 0x0;
    handler |= idt[code].handler_high;
    handler = (handler << 16)|idt[code].handler_low;
    return handler;
}

void set_gate(uint8_t num, uint32_t handler, uint16_t sel, uint8_t flags){
    idt[num].handler_low = handler & 0xFFFF;
    idt[num].selector = sel;
    idt[num].zero = 0;
    idt[num].type = flags;
    idt[num].handler_high = (handler >> 16) & 0xFFFF;
}

void init_idt(){
    idt_head.base_addr = (uint32_t)&idt;
    idt_head.limit = sizeof(idt) - 1;

    print("Idt base address: ");
    print_bin(idt_head.base_addr);
    println("");
    print("Idt limit: ");
    print_int(idt_head.limit);
    println("");

    print("Clearing ");print_int(idt_head.limit/sizeof(struct idt_entry));println(" idt entries...");
    for(int i = 0; i < idt_head.limit/sizeof(struct idt_entry);  i++){
        idt[i].handler_low = 0;
        idt[i].selector = 0;
        idt[i].zero = 0;
        idt[i].type = 0;
        idt[i].handler_high = 0;
    }

    print("Setting gates of type: ");print_hex(IDT_TYPE_INTERRUPT_GATE);println("...");
    set_gate(0, (uint32_t)isr0, 0x08, IDT_TYPE_INTERRUPT_GATE);
    set_gate(1, (uint32_t)isr1, 0x08, IDT_TYPE_INTERRUPT_GATE);
    set_gate(2, (uint32_t)isr2, 0x08, IDT_TYPE_INTERRUPT_GATE);
    set_gate(3, (uint32_t)isr3, 0x08, IDT_TYPE_INTERRUPT_GATE);
    set_gate(4, (uint32_t)isr4, 0x08, IDT_TYPE_INTERRUPT_GATE);
    set_gate(5, (uint32_t)isr5, 0x08, IDT_TYPE_INTERRUPT_GATE);
    set_gate(6, (uint32_t)isr6, 0x08, IDT_TYPE_INTERRUPT_GATE);
    set_gate(7, (uint32_t)isr7, 0x08, IDT_TYPE_INTERRUPT_GATE);
    set_gate(8, (uint32_t)isr8, 0x08, IDT_TYPE_INTERRUPT_GATE);
    set_gate(9, (uint32_t)isr9, 0x08, IDT_TYPE_INTERRUPT_GATE);
    set_gate(10, (uint32_t)isr10, 0x08, IDT_TYPE_INTERRUPT_GATE);
    set_gate(11, (uint32_t)isr11, 0x08, IDT_TYPE_INTERRUPT_GATE);
    set_gate(12, (uint32_t)isr12, 0x08, IDT_TYPE_INTERRUPT_GATE);
    set_gate(13, (uint32_t)isr13, 0x08, IDT_TYPE_INTERRUPT_GATE);
    set_gate(14, (uint32_t)isr14, 0x08, IDT_TYPE_INTERRUPT_GATE);
    set_gate(15, (uint32_t)isr15, 0x08, IDT_TYPE_INTERRUPT_GATE);
    set_gate(16, (uint32_t)isr16, 0x08, IDT_TYPE_INTERRUPT_GATE);
    set_gate(17, (uint32_t)isr17, 0x08, IDT_TYPE_INTERRUPT_GATE);
    set_gate(18, (uint32_t)isr18, 0x08, IDT_TYPE_INTERRUPT_GATE);
    set_gate(19, (uint32_t)isr19, 0x08, IDT_TYPE_INTERRUPT_GATE);
    set_gate(20, (uint32_t)isr20, 0x08, IDT_TYPE_INTERRUPT_GATE);
    set_gate(21, (uint32_t)isr21, 0x08, IDT_TYPE_INTERRUPT_GATE);
    
    set_gate(32, (uint32_t)irq0, 0x08, IDT_TYPE_INTERRUPT_GATE);
    set_gate(33, (uint32_t)irq1, 0x08, IDT_TYPE_INTERRUPT_GATE);
    set_gate(34, (uint32_t)irq2, 0x08, IDT_TYPE_INTERRUPT_GATE);
    set_gate(35, (uint32_t)irq3, 0x08, IDT_TYPE_INTERRUPT_GATE);
    set_gate(36, (uint32_t)irq4, 0x08, IDT_TYPE_INTERRUPT_GATE);
    set_gate(37, (uint32_t)irq5, 0x08, IDT_TYPE_INTERRUPT_GATE);
    set_gate(38, (uint32_t)irq6, 0x08, IDT_TYPE_INTERRUPT_GATE);
    set_gate(39, (uint32_t)irq7, 0x08, IDT_TYPE_INTERRUPT_GATE);
    set_gate(40, (uint32_t)irq8, 0x08, IDT_TYPE_INTERRUPT_GATE);
    set_gate(41, (uint32_t)irq9, 0x08, IDT_TYPE_INTERRUPT_GATE);
    set_gate(42, (uint32_t)irq10, 0x08, IDT_TYPE_INTERRUPT_GATE);
    set_gate(43, (uint32_t)irq11, 0x08, IDT_TYPE_INTERRUPT_GATE);
    set_gate(44, (uint32_t)irq12, 0x08, IDT_TYPE_INTERRUPT_GATE);
    set_gate(45, (uint32_t)irq13, 0x08, IDT_TYPE_INTERRUPT_GATE);
    set_gate(46, (uint32_t)irq14, 0x08, IDT_TYPE_INTERRUPT_GATE);
    set_gate(47, (uint32_t)irq15, 0x08, IDT_TYPE_INTERRUPT_GATE);

    println("Loading idt...");
    __asm__ volatile("lidt %0" : : "m" (idt_head));
}