#include "../drivers/screen.h"
#include "../interrupts/isr.h"
#include "../libs/shell.h"
#include "../paging/paging.h"

void krequest_manager(char* request);

void kernel_main() {
    isr_install();
    asm volatile("sti");  
    initialise_paging(0x1000000);
    run_shell(&krequest_manager);

    //uint32_t *ptr = (uint32_t*)0x400000;
    //uint32_t do_page_fault = *ptr;
	
	return;
}

void krequest_manager(char* request) {
	kprint("YOUR COMMAND: \0");
	kprint(request);
	kprint("\n\0");
	return;
}

