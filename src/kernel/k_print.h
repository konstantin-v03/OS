#ifndef K_PRINT
#define K_PRINT

#define WHITE_ON_BLACK 0x0f

void k_clear_scr();

void k_print_str(char* str, int col, int row, unsigned char attr);

int k_print_char(char c, int col, int row, unsigned char attr);

#endif
