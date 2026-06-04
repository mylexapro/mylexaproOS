/* ------------------------------------------------------------
File: idt.c
Project: mylexaproOS (x86 kernel)
Author: Annabelle Webb
Created: 2026-04-18
Description:
   IDT (Interrupt Descriptor Table) setup. Defines the IDT,
   installs interrupt gates, and loads the IDT register.
------------------------------------------------------------ */

#include "idt.h"

struct idt_entry idt[256];
struct idt_ptr idtp;

/* ISR handlers implemented in assembly */
extern void isr0();
extern void isr1();
extern void isr32();
extern void isr33();

/* idt_set_gate:
   Configures a single IDT entry.
   num      - interrupt number (0–255)
   base     - address of ISR handler
   selector - code segment selector (usually 0x08)
   flags    - type and privilege flags (0x8E = interrupt gate)
*/
void idt_set_gate(uint8_t num, uint32_t base, uint16_t selector, uint8_t flags) {
	idt[num].base_low = base & 0xFFFF;
	idt[num].base_high = (base >> 16) & 0xFFFF;
	idt[num].selector = selector;
	idt[num].zero = 0;
	idt[num].flags = flags;
}

/* idt_init:
Initializes the IDT by:
    - setting the IDT pointer
    - clearing all 256 entries
    - installing specific ISRs (0, 1, 33)
    - loading the IDT with lidt 
*/
void idt_init(void) {
	idtp.limit = (sizeof(struct idt_entry) * 256) - 1;
	idtp.base = (uint32_t)&idt;

	/* Clear all entries */
	for (int i = 0; i < 256; i++) {
		idt_set_gate(i, 0, 0, 0);
	}

	/* Install specific interrupt handlers */
	idt_set_gate(0, (uint32_t)isr0, 0x08, 0x8E);
	idt_set_gate(1, (uint32_t)isr1, 0x08, 0x8E);
	idt_set_gate(32, (uint32_t)isr32, 0x08, 0x8E);
	idt_set_gate(33, (uint32_t)isr33, 0x08, 0x8E);

	/* Load IDT */
	__asm__ __volatile__("lidt %0" : : "m"(idtp));
}
