/* ------------------------------------------------------------
 * File: linker.ld
 * Project: mylexaproOS (x86 kernel)
 * Author: Annabelle Webb
 * Created: 2026-04-18
 * Description:
 *   Linker script defining the physical layout of the boot
 *   sector and kernel. Places the boot code at 0x7C00 and
 *   the 32-bit kernel sections starting at 0x8000.
 * ------------------------------------------------------------ */

ENTRY(start)

SECTIONS
{
	/* BIOS loads the boot sector to 0x7C00 */
	. = 0x7C00;

	.boot :
	{
		*(.boot*)
	}

	/* Kernel is loaded by the bootloader to 0x8000 */
	. = 0x8000;

	/* Code section */
	.text	: { *(.text*)	}

	/* Read-only data */
	.rodata	: { *(.rodata*)	}

	/* Initialized data */
	.data	: { *(.data*)	}

	/* Uninitialized data (zeroed at runtime) */
	.bss	: { *(.bss*) *(COMMON) }
}
