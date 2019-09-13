#include "../drivers/screen.h"
#include "../interrupts/isr.h"
#include "../libs/shell.h"

void krequest_manager(char* request);

void kernel_main() {
    isr_install();
	asm volatile("sti");
	run_shell(&krequest_manager);
	return;
}

void krequest_manager(char* request) {
	kprint("YOUR COMMAND: \0");
	kprint(request);
	kprint("\n\0");
	return;
}

