#include "gdt.h"
#include "../../../std/printf.h"
#include <stdint.h>

// GDT entry structure
typedef struct {
    uint16_t limit_low;     // bytes 0-1
    uint16_t base_low;      // bytes 2-3
    uint8_t  base_mid;      // byte 4
    uint8_t  access;        // byte 5
    uint8_t  granularity;   // byte 6
    uint8_t  base_high;     // byte 7
} __attribute__((packed)) gdt_entry_t;

// GDT pointer structure
typedef struct {
    uint16_t limit;         // size of GDT - 1
    uint32_t base;          // address of GDT
} __attribute__((packed)) gdt_ptr_t;

// TSS structure — needed for ring 3 later
typedef struct {
    uint32_t prev_tss;
    uint32_t esp0;      // kernel stack pointer
    uint32_t ss0;       // kernel stack segment
    uint32_t esp1;
    uint32_t ss1;
    uint32_t esp2;
    uint32_t ss2;
    uint32_t cr3;
    uint32_t eip;
    uint32_t eflags;
    uint32_t eax, ecx, edx, ebx;
    uint32_t esp, ebp, esi, edi;
    uint32_t es, cs, ss, ds, fs, gs;
    uint32_t ldt;
    uint16_t trap;
    uint16_t iomap_base;
} __attribute__((packed)) tss_t;

#define GDT_ENTRIES 6

#define GDT_ADDRESS 0x00000800  // low memory, safe region

static gdt_entry_t gdt[GDT_ENTRIES] __attribute__((aligned(8)));
static gdt_ptr_t    gdt_ptr;
static tss_t       tss;

// kernel stack for TSS (used when returning from user mode)
static uint8_t kernel_stack[8192];

static void gdt_set_entry(int i, uint32_t base, uint32_t limit,
    uint8_t access, uint8_t granularity) {
    gdt[i].base_low   = base & 0xFFFF;
    gdt[i].base_mid   = (base >> 16) & 0xFF;
    gdt[i].base_high  = (base >> 24) & 0xFF;
    gdt[i].limit_low  = limit & 0xFFFF;
    gdt[i].granularity = ((limit >> 16) & 0x0F) | (granularity & 0xF0);
    gdt[i].access     = access;
}

static void tss_init(void) {
    uint32_t base  = (uint32_t)(uintptr_t)&tss;
    uint32_t limit = sizeof(tss_t) - 1;

    // TSS descriptor in GDT
    gdt_set_entry(5, base, limit, 0x89, 0x00);
    // 0x89 = present, ring 0, 32-bit TSS available

    // set up TSS
    tss.ss0 = GDT_KERNEL_DATA;  // 0x08
    tss.esp0 = (uint32_t)(uintptr_t)kernel_stack + sizeof(kernel_stack);
    tss.cs   = GDT_KERNEL_CODE | 3;  // ring 3 bits
    tss.ds = tss.es = tss.fs = tss.gs = tss.ss = GDT_USER_DATA | 3;
    tss.iomap_base = sizeof(tss_t);
}

// defined in gdt_flush.asm
extern void gdt_flush(uint32_t gdt_ptr);
extern void tss_flush(void);

void gdt_init(void) {
    gdt_ptr.limit = (sizeof(gdt_entry_t) * GDT_ENTRIES) - 1;
    gdt_ptr.base  = (uint32_t)(uintptr_t)gdt;  // no & needed for arrays

    //                base   limit       access  gran
    gdt_set_entry(0, 0, 0,          0x00, 0x00); // null     → 0x00
    gdt_set_entry(1, 0, 0xFFFFFFFF, 0x92, 0xCF); // k. data  → 0x08
    gdt_set_entry(2, 0, 0xFFFFFFFF, 0x9A, 0xCF); // k. code  → 0x10
    gdt_set_entry(3, 0, 0xFFFFFFFF, 0x92, 0xCF); // k. data2 → 0x18
    gdt_set_entry(4, 0, 0xFFFFFFFF, 0xFA, 0xCF); // u. code  → 0x20
    gdt_set_entry(5, 0, 0xFFFFFFFF, 0xF2, 0xCF); // u. data  → 0x28

    __asm__ volatile("lgdt %0" : : "m"(gdt_ptr));
}