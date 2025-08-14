// src\include\cpu\interrupts\interrupts.h
// a file defining interrupt types and names

#ifndef INTERRUPT_DEF_H
#define INTERRUPT_DEF_H

#include <stdint.h>

static enum interrupt {
    // CPU Exceptions (0-21)
    INT_DIVIDE_BY_ZERO = 0,
    INT_DEBUG = 1,
    INT_NMI = 2,
    INT_BREAKPOINT = 3,
    INT_OVERFLOW = 4,
    INT_BOUND_RANGE_EXCEEDED = 5,
    INT_INVALID_OPCODE = 6,
    INT_DEVICE_NOT_AVAILABLE = 7,
    INT_DOUBLE_FAULT = 8,
    INT_COPROCESSOR_SEGMENT_OVERRUN = 9,
    INT_INVALID_TSS = 10,
    INT_SEGMENT_NOT_PRESENT = 11,
    INT_STACK_SEGMENT_FAULT = 12,
    INT_GENERAL_PROTECTION_FAULT = 13,
    INT_PAGE_FAULT = 14,
    INT_RESERVED_15 = 15,
    INT_X87_FLOATING_POINT_EXCEPTION = 16,
    INT_ALIGNMENT_CHECK = 17,
    INT_MACHINE_CHECK = 18,
    INT_SIMD_FLOATING_POINT_EXCEPTION = 19,
    INT_VIRTUALIZATION_EXCEPTION = 20,
    INT_CONTROL_PROTECTION_EXCEPTION = 21,
    
    // Reserved exceptions (22-31)
    INT_RESERVED_22 = 22,
    INT_RESERVED_23 = 23,
    INT_RESERVED_24 = 24,
    INT_RESERVED_25 = 25,
    INT_RESERVED_26 = 26,
    INT_RESERVED_27 = 27,
    INT_RESERVED_28 = 28,
    INT_RESERVED_29 = 29,
    INT_RESERVED_30 = 30,
    INT_RESERVED_31 = 31,
    
    // Hardware IRQs (32-47) - mapped from IRQ 0-15
    INT_IRQ0_TIMER = 32,
    INT_IRQ1_KEYBOARD = 33,
    INT_IRQ2_CASCADE = 34,
    INT_IRQ3_COM2_COM4 = 35,
    INT_IRQ4_COM1_COM3 = 36,
    INT_IRQ5_LPT2_SOUND = 37,
    INT_IRQ6_FLOPPY = 38,
    INT_IRQ7_LPT1 = 39,
    INT_IRQ8_RTC = 40,
    INT_IRQ9_FREE = 41,
    INT_IRQ10_FREE = 42,
    INT_IRQ11_FREE = 43,
    INT_IRQ12_PS2_MOUSE = 44,
    INT_IRQ13_MATH_COPROCESSOR = 45,
    INT_IRQ14_PRIMARY_ATA = 46,
    INT_IRQ15_SECONDARY_ATA = 47,
    
    INT_SYSCALL = 128,
    INT_USER_DEFINED = 255
};

// Array of interrupt names corresponding to the enum values
static const char* interrupt_names[] = {
    // CPU Exceptions (0-21)
    "Divide by Zero",                    // 0
    "Debug",                            // 1
    "Non-Maskable Interrupt",           // 2
    "Breakpoint",                       // 3
    "Overflow",                         // 4
    "Bound Range Exceeded",             // 5
    "Invalid Opcode",                   // 6
    "Device Not Available",             // 7
    "Double Fault",                     // 8
    "Coprocessor Segment Overrun",      // 9
    "Invalid TSS",                      // 10
    "Segment Not Present",              // 11
    "Stack Segment Fault",              // 12
    "General Protection Fault",         // 13
    "Page Fault",                       // 14
    "Reserved",                         // 15
    "x87 Floating Point Exception",     // 16
    "Alignment Check",                  // 17
    "Machine Check",                    // 18
    "SIMD Floating Point Exception",    // 19
    "Virtualization Exception",         // 20
    "Control Protection Exception",     // 21
    "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", // 22-26
    "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", // 27-31
    
    // Hardware IRQs (32-47)
    "Timer (IRQ0)",                     // 32
    "Keyboard (IRQ1)",                  // 33
    "Cascade (IRQ2)",                   // 34
    "COM2/4 (IRQ3)",                    // 35
    "COM1/3 (IRQ4)",                    // 36
    "LPT2/Sound (IRQ5)",                // 37
    "Floppy (IRQ6)",                    // 38
    "LPT1 (IRQ7)",                      // 39
    "Real Time Clock (IRQ8)",           // 40
    "Free for use (IRQ9)",              // 41
    "Free for use (IRQ10)",             // 42
    "Free for use (IRQ11)",             // 43
    "PS/2 Mouse (IRQ12)",               // 44
    "Math Coprocessor (IRQ13)",         // 45
    "Primary ATA (IRQ14)",              // 46
    "Secondary ATA (IRQ15)",            // 47
    
    "User Defined", "User Defined", "User Defined", "User Defined", "User Defined", // 48-52
    "User Defined", "User Defined", "User Defined", "User Defined", "User Defined", // 53-57
    "User Defined", "User Defined", "User Defined", "User Defined", "User Defined", // 58-62
    "User Defined", "User Defined", "User Defined", "User Defined", "User Defined", // 63-67
    "User Defined", "User Defined", "User Defined", "User Defined", "User Defined", // 68-72
    "User Defined", "User Defined", "User Defined", "User Defined", "User Defined", // 73-77
    "User Defined", "User Defined", "User Defined", "User Defined", "User Defined", // 78-82
    "User Defined", "User Defined", "User Defined", "User Defined", "User Defined", // 83-87
    "User Defined", "User Defined", "User Defined", "User Defined", "User Defined", // 88-92
    "User Defined", "User Defined", "User Defined", "User Defined", "User Defined", // 93-97
    "User Defined", "User Defined", "User Defined", "User Defined", "User Defined", // 98-102
    "User Defined", "User Defined", "User Defined", "User Defined", "User Defined", // 103-107
    "User Defined", "User Defined", "User Defined", "User Defined", "User Defined", // 108-112
    "User Defined", "User Defined", "User Defined", "User Defined", "User Defined", // 113-117
    "User Defined", "User Defined", "User Defined", "User Defined", "User Defined", // 118-122
    "User Defined", "User Defined", "User Defined", "User Defined", "User Defined", // 123-127
    
    "System Call",                      
    
    "User Defined", "User Defined", "User Defined", "User Defined", "User Defined", // 129-133
    "User Defined", "User Defined", "User Defined", "User Defined", "User Defined", // 134-138
    "User Defined", "User Defined", "User Defined", "User Defined", "User Defined", // 139-143
    "User Defined", "User Defined", "User Defined", "User Defined", "User Defined", // 144-148
    "User Defined", "User Defined", "User Defined", "User Defined", "User Defined", // 149-153
    "User Defined", "User Defined", "User Defined", "User Defined", "User Defined", // 154-158
    "User Defined", "User Defined", "User Defined", "User Defined", "User Defined", // 159-163
    "User Defined", "User Defined", "User Defined", "User Defined", "User Defined", // 164-168
    "User Defined", "User Defined", "User Defined", "User Defined", "User Defined", // 169-173
    "User Defined", "User Defined", "User Defined", "User Defined", "User Defined", // 174-178
    "User Defined", "User Defined", "User Defined", "User Defined", "User Defined", // 179-183
    "User Defined", "User Defined", "User Defined", "User Defined", "User Defined", // 184-188
    "User Defined", "User Defined", "User Defined", "User Defined", "User Defined", // 189-193
    "User Defined", "User Defined", "User Defined", "User Defined", "User Defined", // 194-198
    "User Defined", "User Defined", "User Defined", "User Defined", "User Defined", // 199-203
    "User Defined", "User Defined", "User Defined", "User Defined", "User Defined", // 204-208
    "User Defined", "User Defined", "User Defined", "User Defined", "User Defined", // 209-213
    "User Defined", "User Defined", "User Defined", "User Defined", "User Defined", // 214-218
    "User Defined", "User Defined", "User Defined", "User Defined", "User Defined", // 219-223
    "User Defined", "User Defined", "User Defined", "User Defined", "User Defined", // 224-228
    "User Defined", "User Defined", "User Defined", "User Defined", "User Defined", // 229-233
    "User Defined", "User Defined", "User Defined", "User Defined", "User Defined", // 234-238
    "User Defined", "User Defined", "User Defined", "User Defined", "User Defined", // 239-243
    "User Defined", "User Defined", "User Defined", "User Defined", "User Defined", // 244-248
    "User Defined", "User Defined", "User Defined", "User Defined", "User Defined", // 249-253
    "User Defined",                     // 254
    "User Defined"                      // 255
};

#endif