//kernel header
#include "terminal.h"
#include "arch/x86/interupt/idt.h"
#include "arch/x86/interupt/pic.h"
#include "arch/x86/interupt/irq.h"
#include "shell/shell.h"

//memory header
#include "memory/pmm.h"
#include "memory/m_heap.h"

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

/* check if its 32-bit ix86 cpu targets. */
#if !defined(__i386__)
#error "This thing needs to be compiled with a ix86-elf compiler"
#endif

// function to initialize the kernel
int kernel_init(uint32_t mb_info_addr) {

    terminal_initialize();
    idt_init();
    pic_init();
    irq_init();
    PS2_init();
    keyboard_init();
    // Enable interrupts
    __asm__ volatile("sti");

    // Initialize the Physical Memory Manager (PMM)
    pmm_init(mb_info_addr); // Assuming the multiboot info structure is

    // Initialize the heap
    heap_init();

    return 0;
}

// main kernel entry point
void kernel_main(uint32_t magic, uint32_t mb_info_addr) {
    // call the kernel initialization function
    if (kernel_init(mb_info_addr) != 0) { // if kernel initialization fails or returns non-zero, print an error message and halt the system
        printf("Kernel initialization failed!\n");
        return;
    }

    // run the shell
    shell_run();
}