// src\drivers\keyboard.c
// a basic utility file for keyboard operations

#include <drivers/keyboard/keyboard.h>
#include <drivers/console/vga.h>

static KeyEvent event;

char get_ascii(int s_code) { return s_code < sizeof(KEYCODE_TO_ASCII) ? KEYCODE_TO_ASCII[s_code][0] : 0; }

void wait_on_keypress(char key){
wait_for_input:
    KeyEvent k;
    read_key(&k);
if(!k.is_pressed)
    goto wait_for_input;
if(!(get_ascii(k.code)==key) && (key != '\0'))
  goto wait_for_input;
  return;
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