// src\drivers\screen_text.c
// a simple utility file providing basic text functions(print)

#include <drivers/console/vga.h>
#include <kernel//info/System.h>

// Make a VGA color byte
unsigned char vga_color(unsigned char fg, unsigned char bg) {
  return fg | bg << 4;
}

void init_vga(){
  cursor_x = 0;
  cursor_y = 0;
  current_color = DEFAULT_VGA;
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
        current_color = vga_color(COLOR_BLACK, COLOR_WHITE);
        renderchar(' ');
        current_color = DEFAULT_VGA;
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
  
  if (cursor_x >= VGA_WIDTH) {
    cursor_x = 0;
    cursor_y++;
  }

  if (cursor_y >= VGA_HEIGHT) {
    cursor_y = 0;
  }
}

void print_c(const char *str,unsigned char vga){
  current_color = vga;
  print(str);
  current_color = DEFAULT_VGA;
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

void println_c(const char *str, unsigned char vga){
  current_color = vga;
  println(str);
  current_color = DEFAULT_VGA;
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

void set_color(unsigned char vga){
   current_color = vga;
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

void print_bin(uint32_t num){
  uint32_t i = 0x80000000;

  while(i > 0){
    if(num & i){
      putchar('1');
    }else{
      putchar('0');
    }
    i >>= 1;
  }
}

void reset_vga(){
  clear_screen();
  init_vga();
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

void print_system_info() {
  current_color = vga_color(COLOR_CYAN, DEFAULT_BACKGROUND);
  print("=== SOS Kernel v");
  print_int((int)&__KERNEL_VERSION_MAJOR);
  print(".");
  print_int((int)&__KERNEL_VERSION_MINOR);
  print(" ===");

  current_color = DEFAULT_VGA;
  println("");
  println("Kernel successfully loaded and running!");
  println("This is a basic 32-bit i686 kernel.");
  println("");

  // Show some system info
  print("Kernel loaded at: ");
  current_color = vga_color(COLOR_LIGHT_MAGENTA, DEFAULT_BACKGROUND);
  print_hex((int)&__KERNEL_LOADING_ADDRESS);
  current_color = DEFAULT_VGA;
  println("");
  print("VGA buffer at: ");
  current_color = vga_color(COLOR_LIGHT_MAGENTA, DEFAULT_BACKGROUND);
  print_hex((unsigned int)VGA_BUFFER);
  current_color = DEFAULT_VGA;
  println("");
  print("CPU: ");
  cpu_info_t cit;
  get_cpu_info(&cit);
  print_c(cit.vendor,vga_color(COLOR_LIGHT_MAGENTA,DEFAULT_BACKGROUND));
  println("");
}