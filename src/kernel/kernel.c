//kernel header
#include "terminal.h"
#include "arch/interupt/idt.h"
#include "arch/interupt/pic.h"
#include "arch/interupt/irq.h"
#include "shell/shell.h"

//driver header
#include "driver/ps2.h"
#include "driver/keyboard.h"

// libary headers
#include "std/printf.h"
#include "common/string.h"


/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

// function to initialize the kernel
int kernel_init() {
    
    terminal_initialize();
    idt_init();
    pic_init();
    irq_init();
    PS2_init();
    keyboard_init();
    __asm__ volatile("sti");

    return 0;
}

void kernel_main(void) {
    // call the kernel initialization function
    if (kernel_init() != 0) {
        printf("Kernel initialization failed!\n");
        return;
    }

    // run the shell
    shell_run();
}