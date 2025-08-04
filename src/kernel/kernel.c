// src/kernel/kernel.c
// Basic 32-bit kernel for i686

#include "IO_Utility/IO_kernel.c"
#include "System_Utility/System_util.c"

void print_system_info(){
    current_color = vga_color(COLOR_LIGHT_CYAN, COLOR_BLACK);
    print("=== SOS Kernel v");
    print_float(KERNEL_VERSION);
    print(" ===");
    
    // Reset to normal color
    current_color = vga_color(COLOR_LIGHT_GREY, COLOR_BLACK);
    println("");
    println("Kernel successfully loaded and running!");
    println("This is a basic 32-bit i686 kernel.");
    println("");
    
    // Show some system info
    print("Kernel loaded at: ");
    current_color = vga_color(COLOR_LIGHT_GREEN, COLOR_BLACK);
    print_hex(KERNEL_LOADING_ADDRESS);
    current_color = vga_color(COLOR_LIGHT_GREY, COLOR_BLACK);
    println("");
    print("VGA buffer at: ");
    current_color = vga_color(COLOR_LIGHT_GREEN, COLOR_BLACK);
    print_hex((unsigned int)VGA_BUFFER);
    current_color = vga_color(COLOR_LIGHT_GREY, COLOR_BLACK);
    println("");
    print("CPU: ");
    cpu_info_t cit;
    get_cpu_info(&cit);
    current_color = vga_color(COLOR_LIGHT_GREEN, COLOR_BLACK);
    print(cit.vendor);
    current_color = vga_color(COLOR_LIGHT_GREY, COLOR_BLACK);
    print("");
}

// Main kernel function - called from assembly
void kernel_main() {
    // Initialize kernel
    kernel_init();
    
    print_system_info();

    print_kernel_startup_test();
    
    // Infinite loop - kernel stays running
    while (1) {
        unsigned char c = read_key();
        if(c != 0){
            print_hex((int)c);
            putchar(get_ascii(c));
            print("=");
        }
    }
}