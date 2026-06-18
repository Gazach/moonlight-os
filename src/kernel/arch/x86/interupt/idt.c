#include "IDT.h"
#include "isr.h"
#include <stdint.h>

static struct idt_entry idt[IDT_SIZE];
static struct idt_ptr idt_ptr;

extern void isr_stub(void); 

// IRQ stubs for hardware interrupts (IRQ0-IRQ15)
extern void irq_stub_0(void);
extern void irq_stub_1(void);
extern void irq_stub_2(void);
extern void irq_stub_3(void);
extern void irq_stub_4(void);
extern void irq_stub_5(void);
extern void irq_stub_6(void);
extern void irq_stub_7(void);
extern void irq_stub_8(void);
extern void irq_stub_9(void);
extern void irq_stub_10(void);
extern void irq_stub_11(void);
extern void irq_stub_12(void);
extern void irq_stub_13(void);
extern void irq_stub_14(void);
extern void irq_stub_15(void);

// helper function to set an entry in the IDT
void set_idt_entry(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
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
        set_idt_entry(i, (uint32_t)(uintptr_t)isr_stub, 0x10, 0x8E);
    }

    // register IRQ stubs at vectors 32-47
    set_idt_entry(32, (uint32_t)(uintptr_t)irq_stub_0,  0x10, 0x8E);
    set_idt_entry(33, (uint32_t)(uintptr_t)irq_stub_1,  0x10, 0x8E);
    set_idt_entry(34, (uint32_t)(uintptr_t)irq_stub_2,  0x10, 0x8E);
    set_idt_entry(35, (uint32_t)(uintptr_t)irq_stub_3,  0x10, 0x8E);
    set_idt_entry(36, (uint32_t)(uintptr_t)irq_stub_4,  0x10, 0x8E);
    set_idt_entry(37, (uint32_t)(uintptr_t)irq_stub_5,  0x10, 0x8E);
    set_idt_entry(38, (uint32_t)(uintptr_t)irq_stub_6,  0x10, 0x8E);
    set_idt_entry(39, (uint32_t)(uintptr_t)irq_stub_7,  0x10, 0x8E);
    set_idt_entry(40, (uint32_t)(uintptr_t)irq_stub_8,  0x10, 0x8E);
    set_idt_entry(41, (uint32_t)(uintptr_t)irq_stub_9,  0x10, 0x8E);
    set_idt_entry(42, (uint32_t)(uintptr_t)irq_stub_10, 0x10, 0x8E);
    set_idt_entry(43, (uint32_t)(uintptr_t)irq_stub_11, 0x10, 0x8E);
    set_idt_entry(44, (uint32_t)(uintptr_t)irq_stub_12, 0x10, 0x8E);
    set_idt_entry(45, (uint32_t)(uintptr_t)irq_stub_13, 0x10, 0x8E);
    set_idt_entry(46, (uint32_t)(uintptr_t)irq_stub_14, 0x10, 0x8E);
    set_idt_entry(47, (uint32_t)(uintptr_t)irq_stub_15, 0x10, 0x8E);

    __asm__ volatile("lidtl (%0)" : : "r"(&idt_ptr));
}