// src\include\cpu\mm\mm.c
// memory management file providing
// implementations for the paging setup

#include <cpu/mm/mm.h>

page_directory_entry_t page_directory[1024] __attribute__((aligned(4096)));
physical_memory_manager_t phys_mm;

memory_layout_t m_layout;

uint32_t* allocate_and_init_page_table(uint32_t virtual_addr){
    uint32_t index_pd = (virtual_addr >> 22);
    uint32_t index_pt = (virtual_addr >> 12) & 0x000003FF;

    if(page_directory[index_pd].present) return 0;

    page_directory[index_pd].present = 1;
    page_directory[index_pd].rw = 1;
    page_directory[index_pd].user = 0;
    page_directory[index_pd].pwt = 0;
    page_directory[index_pd].pcd = 0;
    page_directory[index_pd].accessed = 0;
    page_directory[index_pd].reserved = 1;
    page_directory[index_pd].size = 0;
    page_directory[index_pd].ignored = 0;
    page_directory[index_pd].avail = 1;
    page_directory[index_pd].frame = &phys_mm.next_free_page;

    uint32_t* result = phys_mm.next_free_page;

    if(phys_mm.next_free_page < phys_mm.end_addr){
        phys_mm.next_free_page += 4096;
    }else{
        request_int(4);
    }

    return result;
}

void init_page_directory(){
    runtime_assert(sizeof(page_directory_entry_t) == 4,"Test"); // verifying size of page directory
    runtime_assert(sizeof(page_directory) == 4096,"Test"); 

    for(int i = 0; i < (sizeof(page_directory)/sizeof(page_directory_entry_t));i++){ 
        page_directory[i].raw=0; // iterating through entries and setting them 0(replace with memset later?)                           
    }
}

void mm_init(){


    println("Kernel memory layout:");
    print("User base: ");set_color(vga_color(COLOR_LIGHT_MAGENTA,DEFAULT_BACKGROUND));
    print_hex((int)&__USER_SPACE_START);set_color(DEFAULT_VGA);println("");
    print("User limit: ");set_color(vga_color(COLOR_LIGHT_MAGENTA,DEFAULT_BACKGROUND));
    print_hex((int)&__USER_SPACE_END);set_color(vga_color(COLOR_LIGHT_MAGENTA,DEFAULT_BACKGROUND));
    set_color(DEFAULT_VGA);set_color(DEFAULT_VGA);println("\n");
    print("Kernel base: ");set_color(vga_color(COLOR_LIGHT_MAGENTA,DEFAULT_BACKGROUND));
    print_hex((int)&__KERNEL_SPACE_START);set_color(DEFAULT_VGA);println("");
    print("Kernel limit: ");set_color(vga_color(COLOR_LIGHT_MAGENTA,DEFAULT_BACKGROUND));
    print_hex((int)&__KERNEL_SPACE_END);set_color(DEFAULT_VGA);println("");

    set_color(vga_color(COLOR_LIGHT_CYAN,DEFAULT_BACKGROUND));
    println("Initializing page directory...");
    set_color(DEFAULT_VGA);
    init_page_directory();

    phys_mm.start_addr = (USABLE_RAM_START + 4095) & ~4095;
    phys_mm.end_addr = (USABLE_RAM_START + 4095) & ~4095;
    phys_mm.next_free_page = phys_mm.start_addr;
    phys_mm.pages = 0;

    //allocate_and_init_page_table(0xb40500);

}