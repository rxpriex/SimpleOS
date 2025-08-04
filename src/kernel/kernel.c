// src/kernel/kernel.c
// Basic 32-bit kernel for i686

#include "../include/io.h"
#include "../include/System.h"

// Main kernel function - called from assembly
void kernel_main() {
  // Initialize kernel
  kernel_init();

  print_system_info();

  print_kernel_startup_test();  
  
  // Infinite loop - kernel stays running
  while (1) {
    char buffer[256];
    read_line(&buffer,sizeof(buffer));
    println("<");
    print(">");
  }
}
