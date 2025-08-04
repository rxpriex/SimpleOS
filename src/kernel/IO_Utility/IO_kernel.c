//src/kernel/IO_Utility/IO_kernel.c
//a basic utility file for kernel IO operations

// VGA text mode buffer
#define VGA_BUFFER ((char*)0xB8000)
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

#define PS_2_DATA 0x60
#define PS_2_status 0x64

char kbd_US [128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',   
  '\t', /* <-- Tab */
  'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',     
    0, /* <-- control key */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',  0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',   0,
  '*',
    0,  /* Alt */
  ' ',  /* Space bar */
    0,  /* Caps lock */
    0,  /* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,  /* < ... F10 */
    0,  /* 69 - Num lock*/
    0,  /* Scroll Lock */
    0,  /* Home key */
    0,  /* Up Arrow */
    0,  /* Page Up */
  '-',
    0,  /* Left Arrow */
    0,
    0,  /* Right Arrow */
  '+',
    0,  /* 79 - End key*/
    0,  /* Down Arrow */
    0,  /* Page Down */
    0,  /* Insert Key */
    0,  /* Delete Key */
    0,   0,   0,
    0,  /* F11 Key */
    0,  /* F12 Key */
    0,  /* All other keys are undefined */
};

extern void clear_screen();

char get_ascii(int s_code){
    return kbd_US[s_code];
}

unsigned char inb(unsigned short port){
    unsigned char value;
    __asm__ volatile ("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

unsigned char read_key(){
    while(!(inb(0x64) & 0x01));

    unsigned char s_code = inb(PS_2_DATA);
    
    if(s_code & 0x80) return 0;

    return s_code;
}

// Make a VGA color byte
static inline unsigned char vga_color(unsigned char fg, unsigned char bg) {
    return fg | bg << 4;
}

// Make a VGA entry (character + color)
static inline unsigned short vga_entry(unsigned char c, unsigned char color) {
    return (unsigned short)c | (unsigned short)color << 8;
}

// Global variables for cursor position
static int cursor_x = 0;
static int cursor_y = 0;
static unsigned char current_color = 0;

// Initialize the kernel
void kernel_init() {
    cursor_x = 0;
    cursor_y = 0;
    current_color = vga_color(COLOR_LIGHT_GREY, COLOR_BLACK);
}

// Print a character at current cursor position
void putchar(char c) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else if (c == '\r') {
        cursor_x = 0;
    } else if (c == '\t') {
        cursor_x = (cursor_x + 8) & ~7;  // Round up to next tab stop
    } else {
        // Calculate position in VGA buffer
        int pos = cursor_y * VGA_WIDTH + cursor_x;
        if (pos < VGA_WIDTH * VGA_HEIGHT) {
            VGA_BUFFER[pos * 2] = c;
            VGA_BUFFER[pos * 2 + 1] = current_color;
        }
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
void print(const char* str) {
    while (*str) {
        putchar(*str);
        str++;
    }
}

// Print a string with newline
void println(const char* str) {
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
    
    char buffer[12];  // Enough for 32-bit int
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
    char buffer[9];  // 8 hex digits + null terminator
    int i;
    
    for (i = 7; i >= 0; i--) {
        buffer[i] = hex_chars[num & 0xF];
        num >>= 4;
    }
    buffer[8] = '\0';
    
    // Skip leading zeros
    i = 0;
    while (i < 7 && buffer[i] == '0') i++;
    
    print(&buffer[i]);
}

void print_float(float num){
    unsigned int inum = (int)num;
    print_int(inum);
    print(".");
    while(num > (int)num){
        num*=10;
    }
    print_int((int)num);
}

void print_kernel_startup_test(){
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
    println("System is ready. Kernel will now idle...");
}