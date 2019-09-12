#include "../interrupts/isr.h"
#include "../drivers/keyboard.h"
#include "../drivers/ports.h"
#include "../libs/timer.h"

void main() {
	clear_screen();
    isr_install();
	asm volatile("sti");
    init_keyboard();
}
