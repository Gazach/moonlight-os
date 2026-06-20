#include "paging.h"
#include "pmm.h"
#include "../std/printf.h"

// page directory — 1024 entries, each 4 bytes = 4KB, must be page aligned
static uint32_t page_directory[1024] __attribute__((aligned(4096)));

// identity map a range of physical memory
static void identity_map_range(uint32_t start, uint32_t end, uint32_t flags) {
    start = start & ~0xFFF;  // align down to page boundary
    for (uint32_t addr = start; addr < end; addr += 0x1000)
        paging_map(addr, addr, flags);
}

void paging_map(uint32_t virt, uint32_t phys, uint32_t flags) {
    uint32_t pd_index = virt >> 22;          // top 10 bits
    uint32_t pt_index = (virt >> 12) & 0x3FF; // next 10 bits

    // get or create the page table
    uint32_t* page_table;

    if (page_directory[pd_index] & PAGE_PRESENT) {
        // page table already exists — get its address
        page_table = (uint32_t*)(uintptr_t)(page_directory[pd_index] & ~0xFFF);
    } else {
        // allocate a new page table
        page_table = (uint32_t*)pmm_alloc();
        if (!page_table) {
            printf("paging_map: out of memory\n");
            return;
        }
        // zero it out
        for (int i = 0; i < 1024; i++)
            page_table[i] = 0;

        page_directory[pd_index] = (uint32_t)(uintptr_t)page_table | PAGE_PRESENT | PAGE_WRITABLE;
    }

    // set the page table entry
    page_table[pt_index] = (phys & ~0xFFF) | flags | PAGE_PRESENT;
}

void paging_init(void) {
    // zero the page directory
    for (int i = 0; i < 1024; i++)
        page_directory[i] = 0;

    // identity map first 1MB (BIOS, VGA buffer etc)
    identity_map_range(0x0, 0x100000, PAGE_PRESENT | PAGE_WRITABLE);

    // identity map the kernel (1MB to 4MB)
    identity_map_range(0x100000, 0x400000, PAGE_PRESENT | PAGE_WRITABLE);

    // identity map the stack (0x80000 to 0x90000)
    identity_map_range(0x80000, 0x90000, PAGE_PRESENT | PAGE_WRITABLE);

    // identity map the heap (16MB to 32MB)
    identity_map_range(0x1000000, 0x2000000, PAGE_PRESENT | PAGE_WRITABLE);

    // load CR3 and enable paging
    __asm__ volatile("mov %0, %%cr3" : : "r"(page_directory));
    uint32_t cr0;
    __asm__ volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= (1 << 31);
    __asm__ volatile("mov %0, %%cr0" : : "r"(cr0));

}