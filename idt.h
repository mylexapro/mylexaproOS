/* ------------------------------------------------------------
File: idt.h
Project: mylexaproOS (x86 kernel)
Author: Annabelle Webb
Created: 2026-04-18
Description:
   Definitions for the Interrupt Descriptor Table (IDT),
   including the IDT entry structure, IDT pointer structure,
   and initialization functions used by idt.c.
------------------------------------------------------------ */

#ifndef IDT_H
#define IDT_H

#include <stdint.h>

/* IDT entry structure (8bytes) Represents a single interrupt gate in the IDT */
struct idt_entry {
	uint16_t base_low;		// Lower 16 bits of ISR address
	uint16_t selector;		// Code segment selector (usually 0x08)
	uint8_t zero;			// Must be zero 
	uint8_t flags;			// Type and attributes (0x8E = interrupt gate)
	uint16_t base_high;		// Upper 16 bits of ISR address
} __attribute__((packed));

/* IDT pointer structure - passde to lidt to load the IDT */
struct idt_ptr {			// Side of IDT - 1
	uint16_t limit;			// Address of first IDT entry
	uint32_t base;
} __attribute__((packed));

/* IDT setup functions */
void idt_init(void);
void idt_set_gate(uint8_t num, uint32_t base, uint16_t selector, uint8_t flags);

#endif
