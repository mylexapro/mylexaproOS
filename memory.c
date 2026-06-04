/* ------------------------------------------------------------
File: memory.c
Project: mylexaproOS (x86 kernel)
Author: Annabelle Webb
Created: 2026-06-03
Description:
  Reads the E820 memory map stored by boot.asm at 0x500
  and prints each region's address, size, and type.
------------------------------------------------------------ */

#include "memory.h"
#include "kprintf.h"

/* memory_print_map:
  Reads the entry count from 0x4FC, then walks through each 24-byte E820 entry 
  starting at 0x500 and prints the base address, length, and type of each region.
*/
void memory_print_map(void) {
    /* read the count boot.asm stored as 0x4FC */
    uint16_t count = e820_count;

    /* point to the very first entry at 0x500 */
    struct e820_entry *entries = e820_map;

    kprintf("Memory map (%d entries):\n", count);
    kprintf("------------------------\n");

    /* loop through each entry and print it */
    for (uint16_t i = 0; i < count; i++) {
        /* get this entry */
        struct e820_entry *e = &entries[i];

        /* print base address and length in hex */
        kprintf("base: 0x%x len: 0x%x type: ",
            (uint32_t)e->base,
            (uint32_t)e->length
        );

        /* print human readable type */
        if (e->type == MEMORY_USABLE) {
            kprintf("usable\n");
        } else if (e->type == MEMORY_RESERVED) {
            kprintf("reserved\n");
        } else {
            kprintf("other\n");
        }
    }
}