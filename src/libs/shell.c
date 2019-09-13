#include <stdint.h>
#include "../drivers/keyboard.h"
#include "../drivers/screen.h"
#include "../libs/string.h"
#include "shell.h"

request_manager rm;

const char sc_ascii[] = { '?', '?', '1', '2', '3', '4', '5', '6',     
    '7', '8', '9', '0', '-', '=', '?', '?', 'Q', 'W', 'E', 'R', 'T', 'Y', 
        'U', 'I', 'O', 'P', '[', ']', '?', '?', 'A', 'S', 'D', 'F', 'G', 
        'H', 'J', 'K', 'L', ';', '\'', '`', '?', '\\', 'Z', 'X', 'C', 'V', 
        'B', 'N', 'M', ',', '.', '/', '?', '?', '?', ' '};

#define BACKSPACE 0x0E
#define ENTER 0x1C
#define CAPS_LOCK 0x3A

#define SC_MAX 58

#define KEY_BUFFER_SIZE 128

char key_buffer[KEY_BUFFER_SIZE];

uint8_t caps = 0;

void shell_callback(uint8_t scancode) {
	if (scancode > SC_MAX) {
		return;
	}	

    if (scancode == BACKSPACE) {
        backspace(key_buffer);
        kprint_backspace();
    } else if (scancode == ENTER) {
        kprint("\n");
        rm(key_buffer);
        key_buffer[0] = '\0';	
		kprint_a(">", YELLOW_ON_BLACK);
    } else if (scancode == CAPS_LOCK) {
		if (caps == 0) {
			caps = 1;
		} else {
			caps = 0;
		}
	} else {
		if(strlen(key_buffer) > KEY_BUFFER_SIZE - 1) {
			return;
		}
        char letter = sc_ascii[(int)scancode];
		if (!caps && letter >= 0x41 && letter <= 0x5A) {
			letter += 0x20;
		}
        char str[2] = {letter, '\0'};
        append(key_buffer, letter);
        kprint(str);
    }

	return;
}

void shell_print(char* message) {
	kprint("\nShell: \0");
	kprint(message);
	kprint("\n\0");
	return;
}

void run_shell(request_manager rm_) {
	init_keyboard(&shell_callback);
	rm = rm_;
	clear_screen();	
	kprint_a("Shell is running!\n\0", YELLOW_ON_BLACK);
	kprint_a(">", YELLOW_ON_BLACK);
	return;
}
