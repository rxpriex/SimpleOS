// src\include\keyboard.h
// a definition file providing external function definitions, 
// structs and essential keymapping utilites for port 0x60 keycodes

#ifndef SCAN_CODES_H
#define SCAN_CODES_H

// Scan codes for special keys (PS/2 Scan Code Set 1, port 0x60)
// Values are in hexadecimal, representing the "make" code for key press

// Function Keys (F1 to F12)
#define SCANCODE_F1           0x3B
#define SCANCODE_F2           0x3C
#define SCANCODE_F3           0x3D
#define SCANCODE_F4           0x3E
#define SCANCODE_F5           0x3F
#define SCANCODE_F6           0x40
#define SCANCODE_F7           0x41
#define SCANCODE_F8           0x42
#define SCANCODE_F9           0x43
#define SCANCODE_F10          0x44
#define SCANCODE_F11          0x57 
#define SCANCODE_F12          0x58

// Navigation and Editing Keys
#define SCANCODE_TAB          0x0F
#define SCANCODE_ENTER        0x1C
#define SCANCODE_BACKSPACE    0x0E
#define SCANCODE_SPACE        0x39
#define SCANCODE_ESCAPE       0x01
#define SCANCODE_INSERT       0xE0  // Extended: 0xE0 0x52
#define SCANCODE_DELETE       0xE0  // Extended: 0xE0 0x53
#define SCANCODE_HOME         0xE0  // Extended: 0xE0 0x47
#define SCANCODE_END          0xE0  // Extended: 0xE0 0x4F
#define SCANCODE_PAGE_UP      0xE0  // Extended: 0xE0 0x49
#define SCANCODE_PAGE_DOWN    0xE0  // Extended: 0xE0 0x51
#define SCANCODE_LEFT_ARROW   0xE0  // Extended: 0xE0 0x4B
#define SCANCODE_RIGHT_ARROW  0xE0  // Extended: 0xE0 0x4D
#define SCANCODE_UP_ARROW     0xE0  // Extended: 0xE0 0x48
#define SCANCODE_DOWN_ARROW   0xE0  // Extended: 0xE0 0x50

// Control Keys
#define SCANCODE_LEFT_SHIFT   0x2A
#define SCANCODE_RIGHT_SHIFT  0x36
#define SCANCODE_LEFT_CTRL    0x1D
#define SCANCODE_RIGHT_CTRL   0xE0  // Extended: 0xE0 0x1D
#define SCANCODE_LEFT_ALT     0x38
#define SCANCODE_RIGHT_ALT    0xE0  // Extended: 0xE0 0x38
#define SCANCODE_CAPS_LOCK    0x3A
#define SCANCODE_NUM_LOCK     0x45
#define SCANCODE_SCROLL_LOCK  0x46

// Special Keys
#define SCANCODE_PRINT_SCREEN 0xE0  // Extended: 0xE0 0x37 (complex sequence)
#define SCANCODE_PAUSE        0xE1  // Extended: 0xE1 0x1D 0x45 (complex sequence)
#define SCANCODE_LEFT_WIN     0xE0  // Extended: 0xE0 0x5B
#define SCANCODE_RIGHT_WIN    0xE0  // Extended: 0xE0 0x5C
#define SCANCODE_MENU         0xE0  // Extended: 0xE0 0x5D

static const unsigned char KEYCODE_TO_ASCII[256][2] = {
    /* 0x00-0x0F */
    [0x01] = {0x00, 0x00},   // ESC
    [0x02] = {'1', '!'},     [0x03] = {'2', '@'},     [0x04] = {'3', '#'},
    [0x05] = {'4', '$'},     [0x06] = {'5', '%'},     [0x07] = {'6', '^'},
    [0x08] = {'7', '&'},     [0x09] = {'8', '*'},     [0x0A] = {'9', '('},
    [0x0B] = {'0', ')'},     [0x0C] = {'-', '_'},     [0x0D] = {'=', '+'},
    [0x0E] = {0x00, 0x00},   // Backspace
    [0x0F] = {0x00, 0x00},   // Tab

    /* 0x10-0x1F */
    [0x10] = {'q', 'Q'},     [0x11] = {'w', 'W'},     [0x12] = {'e', 'E'},
    [0x13] = {'r', 'R'},     [0x14] = {'t', 'T'},     [0x15] = {'y', 'Y'},
    [0x16] = {'u', 'U'},     [0x17] = {'i', 'I'},     [0x18] = {'o', 'O'},
    [0x19] = {'p', 'P'},     [0x1A] = {'[', '{'},     [0x1B] = {']', '}'},
    [0x1C] = {0x00, 0x00},   // Enter
    [0x1D] = {0x00, 0x00},   // L_Ctrl
    [0x1E] = {'a', 'A'},     [0x1F] = {'s', 'S'},

    /* 0x20-0x2F */
    [0x20] = {'d', 'D'},     [0x21] = {'f', 'F'},     [0x22] = {'g', 'G'},
    [0x23] = {'h', 'H'},     [0x24] = {'j', 'J'},     [0x25] = {'k', 'K'},
    [0x26] = {'l', 'L'},     [0x27] = {';', ':'},     [0x28] = {'\'','"'},
    [0x29] = {'`', '~'},     [0x2A] = {0x00, 0x00},   // L_Shift
    [0x2B] = {'\\','|'},     [0x2C] = {'z', 'Z'},     [0x2D] = {'x', 'X'},
    [0x2E] = {'c', 'C'},     [0x2F] = {'v', 'V'},

    /* 0x30-0x3F */
    [0x30] = {'b', 'B'},     [0x31] = {'n', 'N'},     [0x32] = {'m', 'M'},
    [0x33] = {',', '<'},     [0x34] = {'.', '>'},     [0x35] = {'/', '?'},
    [0x36] = {0x00, 0x00},   // R_Shift
    [0x37] = {'*', '*'},     // Keypad *
    [0x38] = {0x00, 0x00},   // L_Alt
    [0x39] = {' ', ' '},     // Space
    [0x3A] = {0x00, 0x00},   // CapsLock
    [0x3B] = {0x00, 0x00},   // F1
    [0x3C] = {0x00, 0x00},   // F2
    [0x3D] = {0x00, 0x00},   // F3
    [0x3E] = {0x00, 0x00},   // F4
    [0x3F] = {0x00, 0x00},   // F5

    /* 0x40-0x4F */
    [0x40] = {0x00, 0x00},   // F6
    [0x41] = {0x00, 0x00},   // F7
    [0x42] = {0x00, 0x00},   // F8
    [0x43] = {0x00, 0x00},   // F9
    [0x44] = {0x00, 0x00},   // F10
    [0x45] = {0x00, 0x00},   // NumLock
    [0x46] = {0x00, 0x00},   // ScrollLock
    [0x47] = {'7', 0x00},    // Keypad 7/Home
    [0x48] = {'8', 0x00},    // Keypad 8/Up
    [0x49] = {'9', 0x00},    // Keypad 9/PgUp
    [0x4A] = {'-', '-'},     // Keypad -
    [0x4B] = {'4', 0x00},    // Keypad 4/Left
    [0x4C] = {'5', 0x00},    // Keypad 5
    [0x4D] = {'6', 0x00},    // Keypad 6/Right
    [0x4E] = {'+', '+'},     // Keypad +
    [0x4F] = {'1', 0x00},    // Keypad 1/End

    /* 0x50-0x5F */
    [0x50] = {'2', 0x00},    // Keypad 2/Down
    [0x51] = {'3', 0x00},    // Keypad 3/PgDn
    [0x52] = {'0', 0x00},    // Keypad 0/Ins
    [0x53] = {'.', 0x00},    // Keypad ./Del
    [0x54] = {0x00, 0x00},   // Alt-SysRq (non-standard)
    [0x57] = {0x00, 0x00},   // F11
    [0x58] = {0x00, 0x00},   // F12
    [0x5B] = {0x00, 0x00},   // L_GUI (Windows key)
    [0x5C] = {0x00, 0x00},   // R_GUI
    [0x5D] = {0x00, 0x00}    // Menu
};

typedef struct KeyEvent{
    unsigned char code;
    unsigned char is_pressed;
}KeyEvent;

extern void read_key(KeyEvent*);

#endif // SCAN_CODES_H
