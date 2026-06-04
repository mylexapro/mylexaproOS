/* ------------------------------------------------------------
File: memory.h
Project: mylexaproOS (x86 kernel)
Author: Annabelle Webb
Created: 2026-06-03
Description:
  E820 memory map reader. Provides structures and functions
  for reading the BIOS memory map stored at 0x500 during
  boot and displaying available RAM regions.
------------------------------------------------------------ */

#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

/* E820 memory region types */
#define MEMORY_USABLE   1
#define MEMORY_RESERVED 2

/* e820_entry: represents one region of physical 
  memory as reported by the BIOS int 0x15 E820 call.
  Each entry is exactly 24 bytes 
*/
struct e820_entry {
  uint64_t base;    /* physical start address of this region */
  uint64_t length;  /* size of this region in bytes */
  uint32_t type;    /* 1 = usable RAM, 2 = reserved */
  uint32_t unused;  /* padding - required to make struct 24 bytes */
};

/* Linker symbols pointing to E820 data placed by boot.asm */
extern uint16_t e820_count;
extern struct e820_entry e820_map[];

/* Reads and prints the E820 memory map */
void memory_print_map(void);

#endif