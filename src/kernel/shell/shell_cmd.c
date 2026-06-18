#include "shell_cmd.h"

void cmd_help(void) {
    printf("Commands:\n");
    printf("  help       show this message\n");
    printf("  clear      clear the screen\n");
    printf("  echo       print arguments\n");
    printf("  about      show OS info\n");
}

void cmd_clear(void) {
    terminal_initialize();
}

void cmd_echo(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        printf("%s", argv[i]);
        if (i < argc - 1) printf(" ");
    }
    printf("\n");
}

void cmd_about(void) {
    printf("Moonlight OS\n");
    printf("Arch: x86 (i686)\n");
    printf("Built with clang + nasm\n");
}