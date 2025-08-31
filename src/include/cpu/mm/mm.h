// src\include\cpu\mm\mm.h
// memory management file providing
// definitions for the paging setup

/*
Bits:  31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
        [    Page Directory Index   ] [    Page Table Index      ] [          Page Offset            ]
        [        10 bits            ] [       10 bits            ] [          12 bits                ]
        [        bits 31-22         ] [       bits 21-12         ] [          bits 11-0              ]

Bit | PDE (Page Dir Entry)    | PTE (Page Table Entry)
----+------------------------+------------------------
 0  | Present                | Present
 1  | Read/Write             | Read/Write
 2  | User/Supervisor        | User/Supervisor
 3  | Write-Through          | Write-Through
 4  | Cache Disable          | Cache Disable
 5  | Accessed               | Accessed
 6  | Ignored                | Dirty
 7  | Page Size (0=4KB)      | PAT
 8  | Ignored                | Global
 9-11| Available              | Available
12-31| Page Table Address     | Physical Page Address
*/


#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H


#include <drivers/io.h>
#include <kernel/info/system.h>
#include <cpu/idt/idt.h>

#define PAGE_PRESENT    0x001   // Present in memory
#define PAGE_RW         0x002   // Read/Write
#define PAGE_USER       0x004   // User accessible
#define PAGE_PWT        0x008   // Write-through caching
#define PAGE_PCD        0x010   // Cache disable
#define PAGE_ACCESSED   0x020   // Accessed (set by CPU)
#define PAGE_DIRTY      0x040   // Dirty (set by CPU)
#define PAGE_PAT        0x080   // Page Attribute Table
#define PAGE_GLOBAL     0x100   // Global page

#define KERNEL_PHYSICAL_BASE    0x10000 // defined in system.h via linker script.
#define KERNEL_VIRTUAL_BASE     0xC0000000 // must  be calculated using  available ram(bootloader?)
#define KERNEL_HEAP_START       0xC0400000
#define KERNEL_HEAP_END         0xCFFFF000
#define KERNEL_STACK_TOP        0xC03FF000 // currently at 0x90000 (change in kernel_entry)
#define KERNEL_CODE_SIZE        0x00100000 // TODO: Make usable at compile time for accurate memory layout
#define USABLE_RAM_START        0x90000 // stack is placed at this address in the entry function
                                        // TODO: make compile time usable variables that define kernel
                                        // loading address and the stack pointer/kernel size for accurate memory layout

typedef struct {
    uint32_t start_addr;
    uint32_t end_addr;
    uint32_t next_free_page;
    uint32_t pages;
} physical_memory_manager_t;

typedef struct {
    uint32_t kernel_physical_base;
    uint32_t kernel_physical_limit;
    uint32_t kernel_virtual_base;
    uint32_t kernel_virtual_limit;
    uint32_t total_ram;
    uint32_t usable_ram;
}memory_layout_t;

// structure defined by hardware(naturally 32bit aligned so no packed attribute)
typedef union {
    struct {
        uint32_t present    : 1;
        uint32_t rw         : 1;
        uint32_t user       : 1;
        uint32_t pwt        : 1;
        uint32_t pcd        : 1;
        uint32_t accessed   : 1;
        uint32_t reserved   : 1;
        uint32_t size       : 1;
        uint32_t ignored    : 1;
        uint32_t avail      : 3;
        uint32_t frame      : 20;
    };
    uint32_t raw;
} page_directory_entry_t;

// structure defined by hardware(naturally 32bit aligned so no packed attribute)
typedef union {
    struct {
        uint32_t present    : 1;
        uint32_t rw         : 1;
        uint32_t user       : 1;
        uint32_t pwt        : 1;
        uint32_t pcd        : 1;
        uint32_t accessed   : 1;
        uint32_t dirty      : 1;
        uint32_t pat        : 1;
        uint32_t global     : 1;
        uint32_t avail      : 3;
        uint32_t frame      : 20;
    };
    uint32_t raw;
} page_table_entry_t;

/**
 * @brief Initializes the memory manager for paging.
 */
void mm_init();

/**
 * @brief Allocates a page table for a directory entry.
 * @return Returns the pointer to a initialized 4kb aligned page table.
 */
uint32_t* allocate_and_init_page_table();

#endif
