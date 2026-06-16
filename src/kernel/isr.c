#include "IDT.h"
#include "terminal.h"
void isr_handler() {
    terminal_writestring("!!! interrupt caught !!!\n");
    // For now, just print a message. In a real OS, you'd want to handle the interrupt properly.
    __asm__ volatile("cli; hlt"); // Disable interrupts to prevent re-entrancy
}