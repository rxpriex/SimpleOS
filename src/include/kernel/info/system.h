//src/include/system.h
//a file providing basic system functionalities

#ifndef SYSTEM_DEFINITIONS_H
#define SYSTEM_DEFINITIONS_H

#define KERNEL_VERSION 0.2
#define KERNEL_LOADING_ADDRESS 0x10000

typedef struct {
    char vendor[13];
    unsigned int model;
} cpu_info_t;

// Assembly function to get CPUID
extern void get_cpu_info(cpu_info_t *info);

#endif