// src\drivers\screen_text.c
// a simple utility file providing basic text functions(print)

#include "../include/vga.h"
#include "../include/system.h"

// Make a VGA color byte
static inline unsigned char vga_color(unsigned char fg, unsigned char bg) {
  return fg | bg << 4;
}

// Make a VGA entry (character + color)
static inline unsigned short vga_entry(unsigned char c, unsigned char color) {
  return (unsigned short)c | (unsigned short)color << 8;
}

void renderchar(char c){
    int pos = cursor_y * VGA_WIDTH + cursor_x;
    if (pos < VGA_WIDTH * VGA_HEIGHT) {
      VGA_BUFFER[pos * 2] = c;
      VGA_BUFFER[pos * 2 + 1] = current_color;
    }
}

void takechar(){
    if(cursor_x != 0){
        cursor_x--;
        current_color = vga_color(COLOR_LIGHT_GREY, COLOR_BLACK);
        renderchar(' ');
    }
}

// Print a character at current cursor position
void putchar(char c) {
  if (c == '\n') {
    cursor_x = 0;
    cursor_y++;
  } else if (c == '\r') {
    cursor_x = 0;
  } else if (c == '\t') {
    cursor_x = (cursor_x + 8) & ~7; // Round up to next tab stop
  } else {
    // Calculate position in VGA buffer
    renderchar(c);

    cursor_x++;
  }

  // Handle line wrapping
  if (cursor_x >= VGA_WIDTH) {
    cursor_x = 0;
    cursor_y++;
  }

  // Handle scrolling (simple version - just wrap to top)
  if (cursor_y >= VGA_HEIGHT) {
    cursor_y = 0;
  }
}

// Print a string
void print(const char *str) {
  while (*str) {
    putchar(*str);
    str++;
  }
}

// Print a string with newline
void println(const char *str) {
  print(str);
  putchar('\n');
}

// Convert integer to string (simple implementation)
void print_int(int num) {
  if (num == 0) {
    putchar('0');
    return;
  }

  if (num < 0) {
    putchar('-');
    num = -num;
  }

  char buffer[12]; // Enough for 32-bit int
  int i = 0;

  while (num > 0) {
    buffer[i++] = '0' + (num % 10);
    num /= 10;
  }

  // Print digits in reverse order
  while (i > 0) {
    putchar(buffer[--i]);
  }
}

// Print hex number
void print_hex(unsigned int num) {
  print("0x");
  char hex_chars[] = "0123456789ABCDEF";
  char buffer[9]; // 8 hex digits + null terminator
  int i;

  for (i = 7; i >= 0; i--) {
    buffer[i] = hex_chars[num & 0xF];
    num >>= 4;
  }
  buffer[8] = '\0';

  // Skip leading zeros
  i = 0;
  while (i < 7 && buffer[i] == '0')
    i++;

  print(&buffer[i]);
}

void print_float(float num) {
  unsigned int inum = (int)num;
  print_int(inum);
  print(".");
  while (num > (int)num) {
    num *= 10;
  }
  print_int((int)num);
}

void kernel_init(){
  cursor_x = 0;
  cursor_y = 0;
  current_color = vga_color(COLOR_LIGHT_GREY, COLOR_BLACK);
}

void print_kernel_startup_test() {
  println("");
  println("Testing basic functionality:");
  // Test integer printing
  print("- Integer test: ");
  print_int(42);
  print(", ");
  print_int(-123);
  println("");

  // Test hex printing
  print("- Hex test: ");
  print_hex(0xDEADBEEF);
  println("");

  // Color test
  println("- Color test:");
  current_color = vga_color(COLOR_LIGHT_RED, COLOR_BLACK);
  print("  RED ");
  current_color = vga_color(COLOR_LIGHT_GREEN, COLOR_BLACK);
  print("GREEN ");
  current_color = vga_color(COLOR_LIGHT_BLUE, COLOR_BLACK);
  print("BLUE ");
  current_color = vga_color(COLOR_LIGHT_GREY, COLOR_BLACK);
  println("NORMAL");

  println("");
  current_color = vga_color(COLOR_LIGHT_GREEN, COLOR_BLACK);
  println("Kernel initialization complete!");
  current_color = vga_color(COLOR_LIGHT_GREY, COLOR_BLACK);
  println("System is ready. Command shell is now available...");
  print(">");  
}

void print_system_info() {
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