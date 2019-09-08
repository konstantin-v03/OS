#define VIDEO_MEMORY 0xb8000

void k_clear_scr() {
	char* p = VIDEO_MEMORY;	

	for (int i = 0; i < 80 * 25; i++, p++) {
		*p = 0;		
	}

	return;
}

void k_print_str(char* str, int row, int col) {
	char* addr = (char*)(VIDEO_MEMORY + 2 * (row * 80 + col)); 

	while (*str != '\n' && addr <= (VIDEO_MEMORY + 80 * 25)) {
		*addr = *str;
		*(addr + 1) = 0x0f;
	}

	return;
}

void k_print(char c, int row, int col) {
	char* addr = (char*)(VIDEO_MEMORY + 2 * (row * 80 + col)); 

	if (addr <= (VIDEO_MEMORY + 80 * 25)) {
		*addr = c;
		*(addr + 1) = 0x0f;
	}

	return;
}
