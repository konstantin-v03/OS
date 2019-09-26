#include "../drivers/screen.h"
#include "../interrupts/isr.h"
#include "../libs/shell.h"
#include "../paging/paging.h"
#include "../libs/kalloc.h"

void krequest_manager(char* request);

void kernel_main(int32_t mem_size) {
    isr_install();
    asm volatile("sti");  
    initialise_paging(mem_size);
    run_shell(&krequest_manager);
	return;
}

void krequest_manager(char* request) {
    if (strcmp(request, "memmap\0") == 0) {
        kprint("0x0 -> 0x100000 (Kernel place)\n\0");
        kprint("0x100000 -> 0x600000 (Place for page tables)\n\0");
        kprint("0x600000 -> 0x1600000 (Place for heap(The heap max size is equal to 16.7Mb))\n\0");
        kprint("A kernel stack located at the end of memory and expand down (The kernel stack max size equal to 1Mb)\n\0");
    } else {
        kprint("Unknown command: \n\0");
    }
	return;
}

