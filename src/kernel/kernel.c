#include "../drivers/screen.h"
#include "../interrupts/isr.h"
#include "../paging/paging.h"
#include "../libs/string.h"
#include "../libs/kalloc.h"
#include "../libs/shell.h"

void krequest_manager(char* request);

uint32_t mem_size;

void kernel_main(uint32_t mem_size_) {
    mem_size = mem_size_;
    isr_install();
    asm volatile("sti");  
    initialise_paging(mem_size);
    run_shell(&krequest_manager);
	return;
}

void krequest_manager(char* request) {
    kprint(request);    
    kprint("\n\0");    
	return;
}

