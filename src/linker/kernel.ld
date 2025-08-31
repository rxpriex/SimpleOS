ENTRY(_start)

SECTIONS
{
    . = 0x10000;

    __KERNEL_VERSION_MAJOR = 0;
    __KERNEL_VERSION_MINOR = 2;
    __KERNEL_LOADING_ADDRESS = 0x10000;
    __USER_SPACE_START      = 0x00000000;
    __USER_SPACE_END        = 0xBFFFFFFF;
    __KERNEL_SPACE_START    = 0xC0000000;
    __KERNEL_SPACE_END      = 0xFFFFFFFF;
    
    .text : ALIGN(4)
    {
        *(.text)
    }
    
    .rodata : ALIGN(4)
    {
        *(.rodata)
    }
    
    .data : ALIGN(4)
    {
        *(.data)
    }
    
    .bss : ALIGN(4)
    {
        *(.bss)
        *(COMMON)
    }
    
    /DISCARD/ :
    {
        *(.comment)
        *(.eh_frame)
        *(.note.gnu.build-id)
    }
}