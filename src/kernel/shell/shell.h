#ifndef SHELL_H
#define SHELL_H

#include "shell_cmd.h"
#include "../driver/keyboard.h"

#define MAX_CMD_LEN  256
#define MAX_ARGS     16

void shell_run(void);

#endif // SHELL_H