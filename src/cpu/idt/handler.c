// src\cpu\idt\handler.c
// implementation file for handler.h

#include <idt/handler.h>
#include <console/vga.h>
#include <kernel/info/system.h>
#include <port/port_io.h>
#include "../kernel/panic.c"

void print_cpu_state(cpu_i *state)
{
    println("=== CPU STATE ===");

    // General purpose registers
    println("General Purpose Registers:");
    print("  EAX: ");
    print_hex(state->eax);
    println("");
    print("  EBX: ");
    print_hex(state->ebx);
    println("");
    print("  ECX: ");
    print_hex(state->ecx);
    println("");
    print("  EDX: ");
    print_hex(state->edx);
    println("");
    print("  ESI: ");
    print_hex(state->esi);
    println("");
    print("  EDI: ");
    print_hex(state->edi);
    println("");
    print("  EBP: ");
    print_hex(state->ebp);
    println("");
    print("  ESP: ");
    print_hex(state->esp);
    println("");

    // Segment registers
    println("Segment Registers:");
    print("  CS: ");
    print_hex(state->cs);
    println("");
    print("  DS: ");
    print_hex(state->ds);
    println("");
    print("  SS: ");
    print_hex(state->ss);
    println("");

    // Control registers and special
    println("Control & Special:");
    print("  EIP: ");
    print_hex(state->eip);
    println("");
    print("  EFLAGS: ");
    print_hex(state->eflags);
    println("");
    print("  User ESP: ");
    print_hex(state->useresp);
    println("");

    // Interrupt information
    println("Interrupt Information:");
    print("  Interrupt Number: ");
    print_int(state->int_no);
    println("");
    print("  Interrupt Type: ");
    print(interrupt_names[state->int_no]);
    println("");
    print("  Error Code: ");
    print_hex(state->err_code);
    println("");

    println("=================");
}

void err_enc(cpu_i reg) 
{
    reset_vga();
    set_color(vga_color(COLOR_RED,DEFAULT_BACKGROUND));
    println("Kernel crash");
    print_cpu_state(&reg);
    print("Press a key to enter kernel_panic");
    wait_on_keypress('\0');
    if(reg.int_no == 22 && get_last_error()){
        kernel_panic(get_last_error());
    }else{
        kernel_panic("Unexpected Cpu error");
    }
}

void isr_handler(cpu_i reg)
{
    err_enc(reg);
}

void irq_handler(cpu_i reg)
{
    switch (reg.int_no)
    {
    case INT_IRQ1_KEYBOARD:
        //err_enc(reg);
        break;
    default:
        // ignore
    }

    if (reg.int_no >= 40)
    {
        outb(PIC2_COMMAND, PIC_EOI);
    }

    outb(PIC1_COMMAND, PIC_EOI);
}