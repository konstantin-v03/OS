#include "../libs/mem.h"
#include "../libs/shell.h"
#include "../libs/kalloc.h"
#include "../libs/common.h"
#include "frame_alloc.h"
#include "kmalloc.h"
#include "paging.h"

#define PLACEMENT_ADDRESS 0x100000

#define KERNEL_STACK_SIZE 0x100000

#define PAGE_ALLOC_SIZE   0x500000 // 5Mb memory space for future pages allocation 

void page_fault(registers_t regs);

void nop() {
    return;
}

void initialise_paging(uint32_t mem_size) {
    set_placement_addr(PLACEMENT_ADDRESS);

    uint32_t nframes = (mem_size / 0x1000) + 1; // if mem_size / 0x1000 isn't integer will be problem with paging last few pages, so i added 1 to nframes!
    uint32_t* frames = (uint32_t*) kmalloc(INDEX_FROM_BIT(nframes) * 4);
    init_frames_bit_set(nframes, frames);
    memory_set((uint8_t*)frames, 0, INDEX_FROM_BIT(nframes) * 4);

    page_directory_t* kernel_dir = (page_directory_t*) kmalloc_a(sizeof(page_directory_t));
    memory_set((uint8_t*)kernel_dir, 0, sizeof(page_directory_t));


    // allocate a fixed size kernel stack which matches with memory
    for (uint32_t i = mem_size - KERNEL_STACK_SIZE; i < mem_size; i += 0x1000) {
        page_t* page = get_page(i, 1, kernel_dir);

        page->present = 1;
        page->rw = 0;
        page->user = 1;
        page->frame = i >> 12;

        set_frame(page->frame * 0x1000);
    }

    // allocate frames for kernel and heap(kmalloc.h, kmalloc.c)
    for (uint32_t i = 0; i < PLACEMENT_ADDRESS + PAGE_ALLOC_SIZE; i += 0x1000) {
        alloc_frame(get_page(i, 1, kernel_dir), 0, 0);
    }
    
    init_kheap(PLACEMENT_ADDRESS + PAGE_ALLOC_SIZE, kernel_dir);

    register_interrupt_handler(14, &page_fault);

    switch_page_directory(kernel_dir);

    return;
}

void switch_page_directory(page_directory_t* dir) {
    asm volatile("mov %0, %%cr3"::"r"( &dir->tablesPhysical));
    uint32_t cr0;
    asm volatile("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x80000000;
    asm volatile("mov %0, %%cr0"::"r"(cr0));
    return;
}

page_t* get_page(uint32_t address, int make, page_directory_t* dir) {
    address /= 0x1000;
    uint32_t table_idx = address / 1024;
    if (dir->tables[table_idx]) {
        return &dir->tables[table_idx]->pages[address % 1024];
    } else if (make) {
        uint32_t tmp;
        dir->tables[table_idx] = (page_table_t*) kmalloc_ap(sizeof(page_table_t), &tmp);
        memory_set((uint8_t*)dir->tables[table_idx], 0, 0x1000);
        dir->tablesPhysical[table_idx] = tmp | 0x7;
        return &dir->tables[table_idx]-> pages[address % 1024];
    } else {
        return 0;
    }
    return 0;
}

void page_fault(registers_t r) {
    UNUSED(r);
    shell_print_a("\nPAGE FAULT!\n\0", RED_ON_WHITE);
    asm volatile("cli");
    asm volatile("hlt");
    return;
}
