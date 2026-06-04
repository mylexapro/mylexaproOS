/* ------------------------------------------------------------
File: pic.c
Project: mylexaproOS (x86 kernel)
Author: Annabelle Webb
Created: 2026-04-18
Description:
  Functions for remapping and controlling the 8259 PIC.
  Remaps IRQs 0–15 to interrupt vectors 32–47 to avoid
  conflicts with CPU exceptions (0–31).
------------------------------------------------------------ */

#include "pic.h"
#include "io.h"

/* pic_remap:
Reprograms the master and slave PICs so that:
	IRQ0–7  → interrupt 32–39
	IRQ8–15 → interrupt 40–47
This avoids overlapping with CPU exceptions (0–31).
Saves and restores the PIC masks so existing IRQ masks are preserved.
*/
void pic_remap(void) {
	
	/* Start initialization sequence (cascade mode, expect ICW4) */
	outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);

	/* Set new vector offsets */
	outb(PIC1_DATA, 32);	// Master PIC → IRQ0 starts at 32
	outb(PIC2_DATA, 40);	// Slave PIC  → IRQ8 starts at 40

	/* Tell master PIC there is a slave at IRQ2, and slave its cascade ID */
	outb(PIC1_DATA, 4);
	outb(PIC2_DATA, 2);

	/* Set 8086/88 mode */
	outb(PIC1_DATA, ICW4_8086);
	outb(PIC2_DATA, ICW4_8086);

	/* Unmask IRQ1 (keyboard) only, mask everything else */
	outb(PIC1_DATA, 0xFD);
	outb(PIC2_DATA, 0xFF);
}

/* pic_send_eoi:
	Sends an End-of-Interrupt (EOI) command to the PIC.
	Required after handling any hardware IRQ.
	If the IRQ came from the slave PIC (IRQ ≥ 8),
	we must send EOI to both slave and master.
*/
void pic_send_eoi(uint8_t irq) {
	if (irq >= 8) {
		outb(PIC2_COMMAND, 0x20);
	}
	outb(PIC1_COMMAND, 0x20);
}
