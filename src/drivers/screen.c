#include <stdint.h>
#include "../libs/string.h"
#include "../libs/mem.h"
#include "screen.h"
#include "ports.h"

#define VIDEO_ADDRESS 0xb8000

#define MAX_ROWS 25
#define MAX_COLS 80 

#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5

int get_offset(int col, int row) { return 2 * (row * MAX_COLS + col); }
int get_offset_row(int offset) { return offset / (2 * MAX_COLS); }
int get_offset_col(int offset) { return (offset - (get_offset_row(offset) * 2 * MAX_COLS)) / 2; }

int get_cursor_offset();
void set_cursor_offset(int offset);
int print_char(char c, int col, int row, uint8_t attr);

void kprint_at(char* message, int col, int row, uint8_t attr) {
    int offset;

	if (!attr) {
		attr = WHITE_ON_BLACK;
	}

    if (col >= 0 && row >= 0) {
        offset = get_offset(col, row);
	} else {
        offset = get_cursor_offset();
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }

    int i = 0;

    while (message[i] != 0) {
        offset = print_char(message[i++], col, row, attr);
        col = get_offset_col(offset);
        row = get_offset_row(offset);
    }

    return;
}

void kprint(char* message) {
    kprint_at(message, -1, -1, WHITE_ON_BLACK);
    return;
}

void kprint_a(char* message, uint8_t attr) {
    kprint_at(message, -1, -1, attr);
	return;
}

int print_char(char c, int col, int row, uint8_t attr) {
    uint8_t* video_memory = (uint8_t*)VIDEO_ADDRESS;

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

    if (offset >= MAX_ROWS * MAX_COLS * 2) {
        int i;
        for (i = 1; i < MAX_ROWS; i++) {
            memory_copy((uint8_t*)(get_offset(0, i) + VIDEO_ADDRESS),(uint8_t*)(get_offset(0, i - 1) + VIDEO_ADDRESS), MAX_COLS * 2);
		}

        char* last_line = (char*)(get_offset(0, MAX_ROWS - 1) + VIDEO_ADDRESS);
        for (i = 0; i < MAX_COLS * 2; i++) {
			last_line[i] = 0;
		}

        offset -= 2 * MAX_COLS;
	}

    set_cursor_offset(offset);

    return offset;
}

void kprint_backspace() {
    int offset = get_cursor_offset() - 2;
    int row = get_offset_row(offset);
    int col = get_offset_col(offset);
    int offset_ = print_char(0x20, col, row, WHITE_ON_BLACK);
	set_cursor_offset(offset_ - 2);
	return;
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
    port_byte_out(REG_SCREEN_DATA, (uint8_t)(offset >> 8));
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (uint8_t)(offset & 0xff));
    return;
}

void clear_screen() {
    uint8_t* video_memory = (uint8_t*)VIDEO_ADDRESS;

    for (int i = 0; i < MAX_COLS * MAX_ROWS; i++) {
        video_memory[i * 2] = 0;
        video_memory[i * 2 + 1] = 0;
    }

    set_cursor_offset(get_offset(0, 0));

    return;
}
