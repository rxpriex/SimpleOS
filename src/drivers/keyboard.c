// src\drivers\keyboard.c
// a basic utility file for keyboard operations

#include "../include/keyboard.h"

static KeyEvent event;

char get_ascii(int s_code) { return s_code < sizeof(KEYCODE_TO_ASCII) ? KEYCODE_TO_ASCII[s_code][0] : 0; }

unsigned char inb(unsigned short port) {
  unsigned char value;
  __asm__ volatile("inb %1, %0" : "=a"(value) : "Nd"(port));
  return value;
}

void read_line(char *buffer, int size) {
  //read_key();
  int x = 0;
  while(x < size - 1){
    read_key(&event);
    if(event.is_pressed){
      char c = get_ascii(event.code);
      if(!c){
        switch(event.code){
          case SCANCODE_ENTER:
            return;
          break;  
          case SCANCODE_BACKSPACE:
            takechar();
            x--;
            buffer[x] = '\0';
          break;
        }
      }else{
        buffer[x] = c;
        putchar(c);
        x++;
      }
    }
  }
  buffer[x+1]='\0';
}