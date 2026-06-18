// Simple IO operations for x86 architecture

#ifndef IO_H
#define IO_H

#include <stdint.h>
// Write to I/O port
static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

// Read from I/O port
static inline uint8_t inb(uint16_t port) {
    uint8_t val;
    __asm__ volatile("inb %1, %0" : "=a"(val) : "Nd"(port));
    return val;
}

// Small delay for old hardware
static inline void io_wait(void) {
    outb(0x80, 0);
}


#endif