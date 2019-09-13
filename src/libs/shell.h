#ifndef SHELL_H
#define SHEEL_H

typedef void (*request_manager)(char* request);

void run_shell(request_manager rm);

#endif
