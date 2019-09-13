#ifndef KEY_BOARD_H
#define KEY_BOARD_H

typedef void (*send_char)(uint8_t scan_code);

void init_keyboard(send_char cb);

#endif
