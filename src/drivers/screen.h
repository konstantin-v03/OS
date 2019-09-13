#ifndef SCREEN_H
#define SCREEN_H

#define WHITE_ON_BLACK 0x0f
#define RED_ON_WHITE 0xf4

void clear_screen();
void kprint_at(char* message, int col, int row);
void kprint(char* message);
void kprint_backspace();

#endif
