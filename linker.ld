/* ------------------------------------------------------------
File: linker.ld
Project: mylexaproOS (x86 kernel)
Author: Annabelle Webb
Created: 2026-04-18
Description:
  Linker script defining the physical layout of the boot
  sector and kernel. Places the boot code at 0x7C00 and
  the 32-bit kernel sections starting at 0x8000.
  Defines explicit program headers for proper segment
  permissions (no RWX segments).
------------------------------------------------------------ */

/* Fixed memory locations set by boot.asm for E820 data */
e820_count	= 0x4FC;
e820_map	= 0x500;

ENTRY(kmain)

PHDRS
{
	text	PT_LOAD FLAGS(5);	/* r-x - code, executable */
	rodata	PT_LOAD FLAGS(4);	/* r-- - read only data */
	data	PT_LOAD FLAGS(6);	/* rw- - read/write data */
}

SECTIONS
{
	/* BIOS loads the boot sector to 0x7C00 */
	. = 0x7C00;

	.boot :
	{
		*(.boot*)
	} :text

	/* Kernel is loaded by the bootloader to 0x8000 */
	. = 0x8000;

	/* Code section - read + execute only */
	.text	: { *(.text*) } :text

	/* Read-only data - read only */
	.rodata	: ALIGN(4) { *(.rodata*) } :rodata

	/* Initialized data - read + write */
	.data	: ALIGN(4) { *(.data*) } :data

	/* Uninitialized data (zeroed at runtime) - read + write */
	.bss	: ALIGN(4) { *(.bss*) *(COMMON) } :data
}
