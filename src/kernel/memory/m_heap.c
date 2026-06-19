// Memory heap

#include "m_heap.h"
#include "pmm.h"
#include "../std/printf.h"

//define heap start and end addresses
#define HEAP_START 0x1000000 // 16MB
#define HEAP_END   0x2000000 // 32MB
#define HEAP_SIZE  (HEAP_END - HEAP_START)

#define HEAP_MAGIC 0xDEADBEEF // Magic number to identify valid heap blocks / sanity check

// block header structure for heap management
typedef struct block_header {
    uint32_t           magic;   // always HEAP_MAGIC
    size_t             size;    // size of data region (not including header)
    int                free;    // 1 = free, 0 = used
    struct block_header* next;  // next block in list
    struct block_header* prev;  // previous block in list
} block_header_t;

#define HEADER_SIZE sizeof(block_header_t)
#define MIN_SPLIT   (HEADER_SIZE + 16)  // minimum size worth splitting

static block_header_t* heap_start_block = 0;
static int heap_ready = 0;

// Helper function

// merge adjacent free blocks to prevent fragmentation
static void merge_free_blocks(void) {
    block_header_t* cur = heap_start_block;
    while (cur && cur->next) {
        if (cur->free && cur->next->free) {
            // absorb next block into current
            cur->size += HEADER_SIZE + cur->next->size;
            cur->next  = cur->next->next;
            if (cur->next)
                cur->next->prev = cur;
            // don't advance — check again in case next is also free
        } else {
            cur = cur->next;
        }
    }
}

// split a block if it's large enough to hold another allocation after ours
static void split_block(block_header_t* block, size_t size) {
    if (block->size < size + MIN_SPLIT) return;  // not worth splitting

    // carve out a new free block from the remainder
    block_header_t* new_block = (block_header_t*)((uint8_t*)block + HEADER_SIZE + size);
    new_block->magic = HEAP_MAGIC;
    new_block->size  = block->size - size - HEADER_SIZE;
    new_block->free  = 1;
    new_block->next  = block->next;
    new_block->prev  = block;

    if (block->next)
        block->next->prev = new_block;

    block->next = new_block;
    block->size = size;
}

// public API

// initialize the heap
void heap_init(void) {
    if (pmm_free_pages() < (HEAP_SIZE / PAGE_SIZE)) {
        printf("heap_init: not enough free pages!\n");
        return;
    }

    uint32_t addr = HEAP_START;
    while (addr < HEAP_END) {
        void* page = pmm_alloc();
        if (!page) {
            printf("heap_init: pmm_alloc failed at 0x%x\n", addr);
            return;
        }
        addr += PAGE_SIZE;
    }

    heap_start_block = (block_header_t*)HEAP_START;
    heap_start_block->magic = HEAP_MAGIC;
    heap_start_block->size  = HEAP_SIZE - HEADER_SIZE;
    heap_start_block->free  = 1;
    heap_start_block->next  = 0;
    heap_start_block->prev  = 0;

    heap_ready = 1;
    printf("Heap: initialized (%d MB at 0x%x)\n", HEAP_SIZE / 1024 / 1024, HEAP_START);
}

// allocate memory from the heap (kernel malloc)
void* kmalloc(size_t size) {
    if (!heap_ready || size == 0) return 0;

    // align size to 8 bytes
    size = (size + 7) & ~7;

    // first-fit search
    block_header_t* cur = heap_start_block;
    while (cur) {
        if (cur->magic != HEAP_MAGIC) {
            printf("kmalloc: heap corruption detected!\n");
            return 0;
        }
        if (cur->free && cur->size >= size) {
            split_block(cur, size);
            cur->free = 0;
            return (void*)((uint8_t*)cur + HEADER_SIZE);
        }
        cur = cur->next;
    }

    printf("kmalloc: out of heap memory!\n");
    return 0;
}

// free memory back to the heap (kernel free)
void kfree(void* ptr) {
    if (!ptr) return;

    block_header_t* block = (block_header_t*)((uint8_t*)ptr - HEADER_SIZE);

    if (block->magic != HEAP_MAGIC) {
        printf("kfree: invalid pointer or heap corruption!\n");
        return;
    }
    if (block->free) {
        printf("kfree: double free detected!\n");
        return;
    }

    block->free = 1;
    merge_free_blocks();
}

// reallocate memory (kernel realloc)
void* krealloc(void* ptr, size_t size) {
    if (!ptr)   return kmalloc(size);
    if (!size)  { kfree(ptr); return 0; }

    block_header_t* block = (block_header_t*)((uint8_t*)ptr - HEADER_SIZE);
    if (block->magic != HEAP_MAGIC) return 0;

    // already big enough
    if (block->size >= size) return ptr;

    // allocate new block and copy
    void* new_ptr = kmalloc(size);
    if (!new_ptr) return 0;

    uint8_t* src = (uint8_t*)ptr;
    uint8_t* dst = (uint8_t*)new_ptr;
    for (size_t i = 0; i < block->size; i++)
        dst[i] = src[i];

    kfree(ptr);
    return new_ptr;
}