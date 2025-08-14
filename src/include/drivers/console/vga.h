// src\include\vga.h
// a definitions file providing some constants and extern 
// function definitions for basic screen operations

#ifndef VGA_CONSTANTS_H
#define VGA_CONSTANTS_H

#include <stdint.h>

// VGA text mode buffer
#define VGA_BUFFER ((char *)0xB8000)
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

// Colors for VGA text mode
#define COLOR_BLACK 0
#define COLOR_BLUE 1
#define COLOR_GREEN 2
#define COLOR_CYAN 3
#define COLOR_RED 4
#define COLOR_MAGENTA 5
#define COLOR_BROWN 6
#define COLOR_LIGHT_GREY 7
#define COLOR_DARK_GREY 8
#define COLOR_LIGHT_BLUE 9
#define COLOR_LIGHT_GREEN 10
#define COLOR_LIGHT_CYAN 11
#define COLOR_LIGHT_RED 12
#define COLOR_LIGHT_MAGENTA 13
#define COLOR_LIGHT_BROWN 14
#define COLOR_WHITE 15

extern void clear_screen();

// Global variables for cursor position
static int cursor_x = 0;
static int cursor_y = 0;
static unsigned char current_color = 0;

void init_vga();

void reset_vga();

void print(const char *);

void println(const char *);

void renderchar(char);

void print_int(int);

void print_bin(uint32_t);

void print_hex(unsigned int);

#endif 