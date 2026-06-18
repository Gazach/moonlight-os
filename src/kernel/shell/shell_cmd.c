#include "shell_cmd.h"

#include "../arch/x86/cpu.h"

// Basic shell commands

void cmd_help(void) {
    help_list_message();
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

void cmd_version(void) {
    printf("Moonlight OS version: %d.%d.%d\n", KERNEL_VERSION_MAJOR, KERNEL_VERSION_MINOR, KERNEL_VERSION_PATCH);
}

void cmd_shell_version(void) {
    printf("MoonShell version: %d.%d.%d\n", SHELL_VERSION_MAJOR, SHELL_VERSION_MINOR, SHELL_VERSION_PATCH);
}

// Moonfetch command to display system information

void cmd_moonfetch(void) {
    moonfetch_message();

}