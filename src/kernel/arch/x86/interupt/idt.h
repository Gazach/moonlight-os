#ifndef IDT_H
#define IDT_H

#include <stdint.h>

struct idt_entry {
    uint16_t offset_low;   // lower 16 bits of handler function address
    uint16_t selector;     // code segment selector in GDT
    uint8_t  zero;         // always zero
    uint8_t  type_attr;    // type and attributes
    uint16_t offset_high;  // higher 16 bits of handler function address
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;        // size of the IDT - 1
    uint32_t base;         // base address of the IDT
} __attribute__((packed));

#define IDT_SIZE 256

void idt_init();
void set_idt_entry(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);
void isr_handler();

#endif /* IDT_H */
