#ifndef SCREEN_H
#define SCREEN_H

#include <stdint.h>

#define WHITE_ON_BLACK 0x0f
#define RED_ON_WHITE 0xf4
#define YELLOW_ON_BLACK 0x0E

void clear_screen();
void kprint_at(char* message, int col, int row, uint8_t attr);
void kprint_a(char* message, uint8_t attr);
void kprint(char* message);
void kprint_backspace();

#endif
