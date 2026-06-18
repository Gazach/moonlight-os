#ifndef SHELL_CMD_H
#define SHELL_CMD_H

#include "../terminal.h"
#include "../std/printf.h"
#include "../common/string.h"
#include "../kernel_version.h"
#include "../common/message.h"

// command handlers
void cmd_help(void);
void cmd_clear(void);
void cmd_echo(int argc, char* argv[]);
void cmd_about(void);
void cmd_version(void);
void cmd_shell_version(void);
void cmd_moonfetch(void);

#endif // SHELL_CMD_H