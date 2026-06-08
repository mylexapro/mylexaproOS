
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
#include "timer.h"
#include "kmalloc.h"
#include "paging.h"
#include "process.h"
#include "scheduler.h"

/* test process - prints A in a loop */
void task_a(void) {
	while (1) {
		kprintf("A");
		for (volatile int i = 0; i < 1000000; i++);
	}
}

void kmain(void) {
	vga_clear();					// Clear screen
	vga_disable_cursor();			// Disable BIOS cursor
	idt_init();						// Set up interrupt descriptor table
	timer_init();					// Set up timer
	paging_init();					// Set up paging
	pic_remap();					// Remap PIC to avoid CPU exception overlap
	process_init();
	process_create(task_a);
	__asm__ __volatile__("sti");	// Enable interrupts

	kprintf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	kprintf("          mylexaproOS  v1.10.0          \n");
	kprintf("       programming is my lexapro        \n");
	kprintf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
	memory_print_map();

	/* paging test */
	kprintf("\nPaging ENABLED\n");

	/* keyboard input test */
	kprintf("\nPress any key...\n");

	for (;;) {
		__asm__ __volatile__("hlt");
	}
}
