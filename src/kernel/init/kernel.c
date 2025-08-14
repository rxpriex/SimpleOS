// src/kernel/kernel.c
// Basic 32-bit kernel for i686

#include <drivers/io.h>
#include <kernel/info/System.h>
#include <drivers/console/vga.h>
#include <cpu/idt/handler.h>
#include <idt/idt.h>

void kernel_init(){
  init_vga();

  println("--Initializing kernel...--");

  pic_remap(32, 40);
  
  init_idt();

  __asm__ ("sti");

  println("");
}

void kernel_main() {
  // Initialize kernel
  kernel_init();

  print_system_info();

  while (1) {
    char buffer[256];
    read_line(&buffer,sizeof(buffer));
    println("<");
    print(">");
  }
}