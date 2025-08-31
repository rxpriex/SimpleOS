// src\include\kernel\info\system.h
// a file providing basic system functionalities

#ifndef SYSTEM_DEFINITIONS_H
#define SYSTEM_DEFINITIONS_H

#include <stdint.h>

extern unsigned long __KERNEL_LOADING_ADDRESS;
extern const int __KERNEL_VERSION_MAJOR;
extern const int __KERNEL_VERSION_MINOR;
extern const uint32_t __USER_SPACE_START;
extern const uint32_t __USER_SPACE_END;
extern const uint32_t __KERNEL_SPACE_START;
extern const uint32_t __KERNEL_SPACE_END;

#define BOOT_MAGIC 0x07A37AAF
struct mBoot_info_t{
    uint32_t kernel_loading_address;
    uint32_t kernel_size;
    uint32_t memory_size;
}__attribute__((packed));

typedef struct {
    char vendor[13];
    unsigned int model;
} cpu_info_t;

extern void get_cpu_info(cpu_info_t *info);

const char* get_last_error(void);
void set_last_error(const char* error);

#endif