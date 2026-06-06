/* ------------------------------------------------------------
File: paging.h
Project: mylexaproOS (x86 kernel)
Author: Annabelle Webb
Created: 2026-06-05
Description:
    Paging setup for 32-bit protected mode. Defines the page
    directory and page table structures, maps the kernel using
    identity mapping, and enables the CR0 paging bit.
------------------------------------------------------------ */

#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>

/* flags for page directory and page table entries */
#define PAGE_PRESENT    0x1
#define PAGE_WRITABLE   0x2
#define PAGE_USER       0x4

/* page directory: 1024 entries, each pointing to a page table */
extern uint32_t page_directory[1024];

/* page table: 1024 entries, each pointing to a physical page */
extern uint32_t page_table[1024];

/* initializes paging and enabling it */
void paging_init(void);

#endif