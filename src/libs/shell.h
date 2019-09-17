#ifndef SHELL_H
#define SHEEL_H

#include <stdint.h>
#include "../drivers/screen.h"

typedef void (*request_manager)(char* request);

void run_shell(request_manager rm);

void shell_print(char* message);

void shell_print_a(char* message, uint8_t attr);

#endif
