#include "k_print.h"

void main() {
	clear_screen();
	char* str = "Hello, World!\n";
	for (int i = 0; i < 25; i++) {
		kprint(str);
	}

	return;	
}
