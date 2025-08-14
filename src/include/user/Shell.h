// src\include\Shell.h
// a basic shell for low level system controll

#ifndef USER_SHELL_H
#define USER_SHELL_H

#include <user/commands.h>
#include <lib/string/string.h>

typedef void (*shell_cmd_func)(int argc, char **argv);
 
typedef struct{
    string name;
    short parameter_count;
    shell_cmd_func cmd;
}cmd_struct;

static const cmd_struct command_map[] = {
    {{"clear",5}, 0, clear_screen_command}
};

#endif