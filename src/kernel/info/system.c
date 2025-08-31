// src\kernel\info\system.c
// implementation file for the system.h header

#include <kernel/info/system.h>

static const char* last_error = 0x0;  // Private to this file

const char* get_last_error(void) {
    return last_error;
}

void set_last_error(const char* error) {
    last_error = error;
}