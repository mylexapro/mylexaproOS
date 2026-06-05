/* ------------------------------------------------------------
File: kmalloc.c
Project: mylexaproOS (x86 kernel)
Author: Annabelle Webb
Created: 2026-06-04
Description:
    First-fit kernel heap allocator. Manages a linked list
    of memory blocks starting at 0x100000 (1MB). Each block
    has a header tracking size, free status, and next pointer.
    kmalloc() finds the first free block big enough.
    kfree() marks a block free and merges adjacent free blocks.
------------------------------------------------------------ */

#include "kmalloc.h"

/* round size up to mearest multiple of 8 for alignment */
#define ALIGN8(size) (((size) + 7) & ~7)

/* size of the block header itself */
#define HEADER_SIZE (sizeof(block_header))

/* pointer to the first block in the heap */
static block_header *heap_start = NULL;

/* tracks where the heap ends so we can grow it */
static uint8_t *heap_end = (uint8_t*)HEAP_START;

/* new_block: carves a new block from the end of the heap when no existing free block is big enough */
static block_header *new_block(uint32_t size) {
    block_header *block = (block_header*)heap_end;
    block->size = size;
    block->free = 0;
    block->next = NULL;
    heap_end += HEADER_SIZE + size;
    return block;
}

/* kmalloc: allocates size bytes from the kernel heap, returns pointer to usable data area (after the header)
   returns NULL if size is 0 */
void *kmalloc(uint32_t size) {
    if (size == 0) return NULL;

    /* align size to 8 bytes */
    size = ALIGN8(size);

    /* first call - initialize the heap */
    if (heap_start == NULL) {
        heap_start = new_block(size);
        return (void*)(heap_start + 1);
    }

    /* walk the list looking for a free block big enough */
    block_header *current = heap_start;
    while (current != NULL) {
        if (current->free && current->size >= size) {
            /* found one - mark it used and return it */
            current->free = 0;
            return (void*)(current + 1);
        }
        current = current->next;
    }

    /* no free block found - grow the heap */
    block_header *block = new_block(size);

    /* attach to end of list */
    current = heap_start;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = block;

    return (void*)(block + 1);
}

/* kfree: marks a block as free merges with next block if it is also free */
void kfree(void *ptr) {
    if (ptr == NULL) return;

    /* step back from data pointer to find the header */
    block_header *block = (block_header*)ptr - 1;
    block->free = 1;

    /* coalesce: merge with next block if it is also free */
    if (block->next != NULL && block->next->free) {
        block->size += HEADER_SIZE + block->next->size;
        block->next = block->next->next;
    }
}