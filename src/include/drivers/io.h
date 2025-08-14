// src\include\io.h
// a io header file with a collection of function definitions
// for io related drivers

#ifndef IO_H
#define IO_H

#include <drivers/keyboard/keyboard.h>
#include <drivers/console/vga.h>

// Initialize the kernel
void kernel_init();

void print_system_info();

void print_kernel_startup_test();

void read_line(char*, int);

void get_ascii(int);

#endif