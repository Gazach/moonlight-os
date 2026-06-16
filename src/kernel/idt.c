#include "IDT.h"
#include "isr.h"
#include <stdint.h>

static struct idt_entry idt[IDT_SIZE];
static struct idt_ptr idt_ptr;

extern void isr_stub(void); 

// helper function to set an entry in the IDT
static void set_idt_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].offset_low = base & 0xFFFF;
    idt[num].selector = sel;
    idt[num].zero = 0;
    idt[num].type_attr = flags;
    idt[num].offset_high = (base >> 16) & 0xFFFF;
}

// Initialize the IDT with a simple handler for all interrupts
void idt_init() {
    idt_ptr.limit = sizeof(struct idt_entry) * IDT_SIZE - 1;
    idt_ptr.base = (uint32_t)(uintptr_t)&idt;

    // Clear the IDT
    for (int i = 0; i < IDT_SIZE; i++) {
        set_idt_gate(i, (uint32_t)(uintptr_t)isr_stub, 0x10, 0x8E);
    }

    // Load the IDT
    __asm__ volatile("lidtl (%0)" : : "r"(&idt_ptr));
}