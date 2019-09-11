#include "k_print.h"
#include "../drivers/ports.h"

#define VIDEO_ADDRESS 0xb8000

#define MAX_ROWS 25
#define MAX_COLS 80 

#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5


int get_offset(int col, int row) { return 2 * (row * MAX_COLS + col); }
int get_offset_row(int offset) { return offset / (2 * MAX_COLS); }
int get_offset_col(int offset) { return (offset - (get_offset_row(offset)*2*MAX_COLS))/2; }

int get_cursor_offset();
void set_cursor_offset(int offset);
int print_char(char c, int col, int row, char attr);

void kprint_at(char *message, int col, int row) {
    int offset;

    if (col >= 0 && row >= 0) {
        offset = get_offset(col, row);
	} else {
        offset = get_cursor_offset();
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }

    int i = 0;

    while (message[i] != 0) {
        offset = print_char(message[i++], col, row, WHITE_ON_BLACK);
        col = get_offset_col(offset);
        row = get_offset_row(offset);
    }

    return;
}

void kprint(char *message) {
    kprint_at(message, -1, -1);
    return;
}

int print_char(char c, int col, int row, char attr) {
    unsigned char* video_memory = VIDEO_ADDRESS;

    if (!attr) {
        attr = WHITE_ON_BLACK;
    }

    if (col >= MAX_COLS || row >= MAX_ROWS) {
        video_memory[2 * (MAX_COLS) * (MAX_ROWS) - 2] = 'E';
        video_memory[2 * (MAX_COLS) * (MAX_ROWS) - 1] = RED_ON_WHITE;
        return get_offset(col, row);
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
    return;
}

void clear_screen() {
    unsigned char *video_memory = VIDEO_ADDRESS;

    for (int i = 0; i < MAX_COLS * MAX_ROWS; i++) {
        video_memory[i * 2] = 0;
        video_memory[i * 2 + 1] = 0;
    }

    set_cursor_offset(get_offset(0, 0));

    return;
}
