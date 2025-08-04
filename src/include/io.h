// src\include\io.h
// a io header file with a collection of function definitions
// for io related drivers

#ifndef IO_H
#define IO_H

#include "keyboard.h"
#include "vga.h"

void renderchar(char);

// Initialize the kernel
void kernel_init();

void print_system_info();

void print_kernel_startup_test();


void read_line(char*, int);

void get_ascii(int);


void print(const char *);

void println(const char *);

#endif