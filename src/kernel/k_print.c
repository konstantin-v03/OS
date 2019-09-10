#include "../drivers/ports.h"
#include "k_print.h"

#define VIDEO_MEMORY 0xb8000

#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5

#define MAX_COLS 80
#define MAX_ROWS 25

#define get_offset(col, row) (2 * (row * MAX_COLS + col))

#define get_offset_row(offset) (offset / (2 * MAX_COLS))

#define get_offset_col(offset) ((offset - (get_offset_row(offset) * 2 * MAX_COLS)) / 2)

void set_cursor_offset(int offset);

int get_cursor_offset();

void k_clear_scr() {
	set_cursor_offset(get_offset(0, 0));

	char* p = VIDEO_MEMORY;	

	for (int i = 0; i < MAX_ROWS * MAX_COLS; i++, p++) {
		*p = 0;		
	}

	return;
}

void k_print_str(char* str, int col, int row, unsigned char attr) {
	int offset;	

	if (col >= 0 && row >= 0) {
		offset = get_offset(col, row);
	}
	else {
		offset = get_cursor_offset();
		row = get_offset_row(offset);
		col = get_offset_col(offset);
	}

	while (*str != 0) {
		offset = k_print_char(*str, col, row, attr);
		if (offset < 0) {
			return;
		}	
		row = get_offset_row(offset);
		col = get_offset_col(offset);
		str++;
	}

	return;
}

int k_print_char(char c, int col, int row, unsigned char attr) {
	unsigned char* video_memory = VIDEO_MEMORY;

	if (col >= MAX_COLS || row >= MAX_ROWS) {
		return -1;
	}

	if (!attr) {
		attr = WHITE_ON_BLACK;
	}	

	int offset;

	if (col >= 0 && row >= 0) {
		offset = get_offset(col, row);
	} else { 
		offset = get_cursor_offset();
	}

	if (c == '\n') {
		row = get_offset_row(offset);
		offset = get_offset(0, row + 1);
	} else {
		video_memory[offset] = c;
		video_memory[offset + 1] = attr;	
		offset += 2;
	}

	set_cursor_offset(offset);
	
	return offset;
}

int get_cursor_offset() {
    port_byte_out(REG_SCREEN_CTRL, 14);
    int offset = port_byte_in(REG_SCREEN_DATA) << 8;
    port_byte_out(REG_SCREEN_CTRL, 15);
    offset += port_byte_in(REG_SCREEN_DATA);
    return offset * 2;
}

void set_cursor_offset(int offset) {
    offset /= 2;
    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset & 0xff));
}
