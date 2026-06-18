#include "shell.h"

static int shell_parse(char* cmd, char* argv[], int max_args) {
    int argc = 0;
    char* p = cmd;
    while (*p && argc < max_args) {
        while (*p == ' ') p++;
        if (!*p) break;
        argv[argc++] = p;
        while (*p && *p != ' ') p++;
        if (*p == ' ') *p++ = '\0';
    }
    return argc;
}

// PUBLIC API

void shell_run(void) {
    char buf[MAX_CMD_LEN];
    char* argv[MAX_ARGS];

    while (1) {
        printf("> ");
        keyboard_readline(buf, sizeof(buf));

        if (buf[0] == '\0') continue;

        int argc = shell_parse(buf, argv, MAX_ARGS);
        if (argc == 0) continue;
        // basic shell commands
        if      (strcmp(argv[0], "help")  == 0) cmd_help();
        else if (strcmp(argv[0], "clear") == 0) cmd_clear();
        else if (strcmp(argv[0], "echo")  == 0) cmd_echo(argc, argv);
        else if (strcmp(argv[0], "about") == 0) cmd_about();
        else if (strcmp(argv[0], "version") == 0) cmd_version();
        else if (strcmp(argv[0], "shell-version") == 0) cmd_shell_version();
        // Moonfetch command to display system information
        else if (strcmp(argv[0], "moonfetch") == 0) cmd_moonfetch();
        else {
            printf("%s: command not found\n", argv[0]);
        }
    }
}