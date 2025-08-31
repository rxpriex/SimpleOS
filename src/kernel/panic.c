// src\kernel\panic\panic.c
// exit function for the kernel in 
// case of an uncaught error

#include <drivers/console/vga.h>
#include <drivers/keyboard/keyboard.h>
#include <drivers/port/port_io.h>

void kernel_panic(const char* c){
    asm volatile("cli");

    reset_vga();
    println_c("Kernel entered kernel_panic",vga_color(COLOR_RED,DEFAULT_BACKGROUND));
    print_c("Cause: ",vga_color(COLOR_RED,DEFAULT_BACKGROUND));
    println_c(c,vga_color(COLOR_LIGHT_CYAN,DEFAULT_BACKGROUND));
    println_c("Press r to reboot",vga_color(COLOR_RED,DEFAULT_BACKGROUND));
    wait_on_keypress('r');
    println("Rebooting...");

    uint8_t value = inb(0x92);
    outb(0x92, value | 0x01);

    print_c("FAILED",vga_color(COLOR_RED,DEFAULT_BACKGROUND));

    asm volatile("hlt");
}