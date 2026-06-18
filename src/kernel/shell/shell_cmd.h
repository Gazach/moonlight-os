#ifndef SHELL_CMD_H
#define SHELL_CMD_H

#include "../terminal.h"
#include "../std/printf.h"
#include "../common/string.h"

// command handlers
void cmd_help(void);
void cmd_clear(void);
void cmd_echo(int argc, char* argv[]);
void cmd_about(void);

#endif // SHELL_CMD_H