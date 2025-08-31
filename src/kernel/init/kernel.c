// src/kernel/kernel.c
// Basic 32-bit kernel for i686

#include <drivers/io.h>
#include <kernel/info/System.h>
#include <drivers/console/vga.h>
#include <cpu/idt/handler.h>
#include <idt/idt.h>
#include <mm/mm.h>

void kernel_init(){
  init_vga();

  println_c("--Initializing kernel...--",vga_color(COLOR_LIGHT_CYAN, DEFAULT_BACKGROUND));

  pic_remap(32, 40);
  
  init_idt();

  mm_init();

  __asm__ ("sti");

  println("");
}

void kernel_main(uint32_t magic, struct mBoot_info_t* bootinfo) {
  // Initialize kernel
  kernel_init();

  runtime_assert(magic == BOOT_MAGIC,"Invalid Boot Sequence");

  print_hex(magic);
  println("");

  print_hex(bootinfo->kernel_loading_address);
  println("");

  print_hex(bootinfo->kernel_size);
  println("");

  print_hex(bootinfo->memory_size);
  println("");

  //print_system_info();

  while (1) {
    char buffer[256];
    print(">");
    read_line(&buffer,sizeof(buffer));
    println("<");
    //((void(*)(void))read_handler(0))(); // causing divide by 0 int
    runtime_assert(0,"Pech gehabt");
  }
}