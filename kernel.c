
/* ------------------------------------------------------------
File: kernel.c
Project: mylexaproOS (x86 kernel)
Author: Annabelle Webb
Created: 2026-04-18
Description:
	Kernel entry point. Initializes VGA output, IDT, and PIC,
	enables interrupts, and displays the boot banner.
------------------------------------------------------------ */

#include "vga.h"
#include "kprintf.h"
#include "idt.h"
#include "pic.h"
#include "memory.h"

void kmain(void) {
	vga_clear();					// Clear screen
	vga_disable_cursor();			// Disable BIOS cursor
	idt_init();						// Set up interrupt descriptor table
	pic_remap();					// Remap PIC to avoid CPU exception overlap
	__asm__ __volatile__("sti");	// Enable interrupts

	kprintf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	kprintf("          mylexaproOS  v1.5.0           \n");
	kprintf("       programming is my lexapro        \n");
	kprintf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
	memory_print_map();
	kprintf("\nPress any key...\n");

	for (;;) {
		__asm__ __volatile__("hlt");
	}
}
