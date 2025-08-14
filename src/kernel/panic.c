// src\kernel\panic\panic.c
// exit function for the kernel in 
// case of an uncaught error

void kernel_panic(){
    asm volatile("cli; hlt");
}