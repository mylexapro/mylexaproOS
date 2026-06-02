
/* ------------------------------------------------------------
 * File: kernel.c
 * Project: mylexaproOS (x86 kernel)
 * Author: Annabelle Webb
 * Created: 2026-04-18
 * Description:
 *   Kernel entry point. Initializes VGA output, IDT, and PIC,
 *   enables interrupts, and displays the boot banner.
 * ------------------------------------------------------------ */

#include "vga.h"
#include "kprintf.h"
#include "idt.h"
#include "pic.h"

void kmain(void) {
	vga_clear();					// Clear screen
	idt_init();						// Set up interrupt descriptor table
	pic_remap();					// Remap PIC to avoid CPU exception overlap
	__asm__ __volatile__("sti");	// Enable interrupts

	kprintf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	kprintf("        mylexaproOS  v1.3.0              \n");
	kprintf("      programming is my lexapro          \n");
	kprintf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");


	for (;;) {
		__asm__ __volatile__("hlt");
	}
}
