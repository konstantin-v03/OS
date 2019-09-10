#include "k_print.h"

void main() {
	char* str = "Hello, world!\n\0";
	k_clear_scr();
	k_print_str(str, -1, -1, WHITE_ON_BLACK);
	k_print_str(str, -1, -1, WHITE_ON_BLACK);
	k_print_str(str, -1, -1, WHITE_ON_BLACK);
}
