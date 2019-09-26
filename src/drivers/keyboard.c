#include "../interrupts/isr.h"
#include "../drivers/ports.h"
#include "../libs/string.h"
#include "../libs/common.h"
#include "keyboard.h"
#include "screen.h"

send_char call_back;

static void keyboard_callback(registers_t regs) {
    UNUSED(regs);
    uint8_t scancode = port_byte_in(0x60);
    
    if (scancode == 0) {
		kprint("Keyboard error.\n");
		return;
	} 

	call_back(scancode);

	return;
}

void init_keyboard(send_char cb) {
	call_back = cb;
	register_interrupt_handler(IRQ1, keyboard_callback); 
	return;	
}
