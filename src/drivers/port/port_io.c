// src\drivers\ports\port_io.c
// implementation file for port_io.h

#include <drivers/port/port_io.h>

void outb(uint16_t port, uint8_t value){
   __asm__ volatile (
        "outb %0, %1"
        :
        : "a" (value), "d" (port)
    );
}

unsigned char inb(uint16_t port) {
  unsigned char value;
  __asm__ volatile("inb %1, %0" : "=a"(value) : "Nd"(port));
  return value;
}

void pic_remap(int offset1, int offset2) {
    uint8_t a1, a2;
    
    // Save masks
    a1 = inb(PIC1_DATA);
    a2 = inb(PIC2_DATA);
    
    // Start initialization sequence (in cascade mode)
    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    
    // ICW2: Master PIC vector offset
    outb(PIC1_DATA, offset1);
    // ICW2: Slave PIC vector offset  
    outb(PIC2_DATA, offset2);
    
    // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
    outb(PIC1_DATA, 4);
    // ICW3: tell Slave PIC its cascade identity (0000 0010)
    outb(PIC2_DATA, 2);
    
    // ICW4: have the PICs use 8086 mode (and not 8080 mode)
    outb(PIC1_DATA, ICW4_8086);
    outb(PIC2_DATA, ICW4_8086);
    
    // Restore saved masks
    outb(PIC1_DATA, a1);
    outb(PIC2_DATA, a2);
}