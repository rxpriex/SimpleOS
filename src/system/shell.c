// src\kernel\System_Utility\Shell.c
// a file providing an initial command
// interpreter for basic system control

#include "../include/commands.h"

/* Command handler type */
typedef int (*shell_cmd_func)(int argc, char **argv);
 
typedef struct{
    const char* name;
    short parameter_count;
    shell_cmd_func cmd;
}cmd_struct;

static const cmd_struct command_map[] = {
    {"clear", 0, clear_screen_command}
};