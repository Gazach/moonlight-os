//kernel header
#include "terminal.h"
#include "arch/x86/io.h"
#include "arch/x86/interupt/idt.h"
#include "arch/x86/interupt/pic.h"
#include "arch/x86/interupt/irq.h"
#include "arch/x86/gdt/gdt.h"
#include "shell/shell.h"

//memory header
#include "memory/pmm.h"
#include "memory/m_heap.h"
#include "memory/paging.h"

//driver header
#include "driver/ps2.h"
#include "driver/keyboard.h"
#include "driver/timer.h"

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

    __asm__ volatile("cli");

    // initialize GDT, IDT, PIC, PS/2 controller
    pic_init();
    gdt_init();
    idt_init();
    PS2_init();

    // initialize IRQ handlers and enable interrupts
    keyboard_init();      // hardware setup
    timer_irq_init();     // register timer IRQ
    keyboard_irq_init();  // register keyboard IRQ
    irq_init();           // register all other IRQs
    __asm__ volatile("sti");

    // initialize memory management
    pmm_init(mb_info_addr);
    heap_init();
    paging_init();
    
    return 0;
}

// main kernel entry point
void kernel_main(uint32_t magic, uint32_t mb_info_addr) {
    if (magic != 0x36d76289) {  // multiboot2 bootloader magic
        // not loaded by a multiboot2 bootloader
        return;
    }
    if (kernel_init(mb_info_addr) != 0) {
        printf("Kernel initialization failed!\n");
        return;
    }
    shell_run();
}