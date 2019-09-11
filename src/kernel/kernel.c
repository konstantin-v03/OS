#include "../drivers/kprint.h"

void main() {
	clear_screen();
	char* str = "Kernel is running!\n";
	kprint(str);
	return;	
}
