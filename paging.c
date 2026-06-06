/* ------------------------------------------------------------
File: paging.c
Project: mylexaproOS (x86 kernel)
Author: Annabelle Webb
Created: 2026-06-06
Description:
    Sets up 32-bit x86 paging. Builds a page directory and
    page table, identity maps the kernel, loads CR3, and
    enables the PG bit in CR0 to activate virtual memory.
------------------------------------------------------------ */

#include "paging.h"

uint32_t page_directory[1024] __attribute__((aligned(4096)));
uint32_t page_table[1024]__attribute__((aligned(4096)));

/* paging_init: sets up the page directory and page table, identity maps the first 4MB of memory, loads CR3,
   and enabled paging in CR0 */
void paging_init(void) {
    for (int i = 0; i < 1024; i++) {
        page_directory[i] = 0;
    }

    for (int i = 0; i < 1024; i++) {
        page_table[i] = (i * 0x1000) | PAGE_PRESENT | PAGE_WRITABLE;
    }

    page_directory[0] = (uint32_t)page_table | PAGE_PRESENT | PAGE_WRITABLE;

    __asm__ __volatile__(
        "mov %0, %%cr3"
        :
        : "r"(page_directory)
    );

    uint32_t cr0;
    __asm__ __volatile__("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;
    __asm__ __volatile__("mov %0, %%cr0" : : "r"(cr0));
}