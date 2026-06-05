/* ------------------------------------------------------------
File: kmalloc.h
Project: mylexaproOS (x86 kernel)
Author: Annabelle Webb
Created: 2026-06-04
Description:
    Simple first-fit memory allocator for the kernel heap.
    The heap starts at 1MB (0x100000) and grows upward.
    Each allocation is preceded by a block header that tracks
    size, availability, and links to the next block.
------------------------------------------------------------ */

#ifndef KMALLOC_H
#define KMALLOC_H

#include <stdint.h>
#include <stddef.h>

/* block_header: sits directly before every allocated block in memory.
   kfree() finds it by stepping back from the returned pointer by sizeof(block_header) */
typedef struct block_header {
    uint32_t size;              /* size of data area in bytes */
    uint32_t free;              /* 1 = available, 0 - in use */
    struct block_header *next;  /* pointer to next block in list */
} block_header;

/* heap starts at 1MB - first useable RAM above legacy region */
#define HEAP_START 0x100000

/* allocate size bytes from the heap, returns pointer to data */
void *kmalloc(uint32_t size);

/* free a previosly allocated block */
void kfree(void *ptr);

#endif