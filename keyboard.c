/* ------------------------------------------------------------
 File: keyboard.c
 Project: mylexaproOS (x86 kernel)
 Author: Annabelle Webb
 Created: 2026-04-18
 Description:
   Basic PS/2 keyboard handler. Reads raw scancodes from
   port 0x60 and prints them using kprintf.
------------------------------------------------------------ */

#include "kprintf.h"
#include "vga.h"
#include <stdint.h>

/* US keyboard scancode map 
   Index = scancode, value = ASCII character
   0 means the key has no printable character (shift, ctrl, etc)
 */

static const char scancode_map[128] = {
	0,   0,  '1', '2', '3', '4', '5', '6', '7', '8',  /* 0x00 - 0x09 */
    '9', '0', '-', '=',  0,   0,  'q', 'w', 'e', 'r',  /* 0x0A - 0x13 */
    't', 'y', 'u', 'i', 'o', 'p', '[', ']',  0,   0,   /* 0x14 - 0x1D */
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',  /* 0x1E - 0x27 */
    '\'','`',  0, '\\','z', 'x', 'c', 'v', 'b', 'n',   /* 0x28 - 0x31 */
    'm', ',', '.', '/',  0,  '*',  0,  ' ',  0,   0,   /* 0x32 - 0x3B */
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    /* 0x3C - 0x45 */
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    /* 0x46 - 0x4F */
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    /* 0x50 - 0x59 */
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    /* 0x5A - 0x63 */
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    /* 0x64 - 0x6D */
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    /* 0x6E - 0x77 */
    0,   0,   0,   0,   0,   0,   0,   0               /* 0x78 - 0x7F */
};

/* Shifted US keyboard scancode map - same structure but returns shifted characters */
static const char shift_map[128] = {
   0,   0,  '!', '@', '#', '$', '%', '^', '&', '*',  /* 0x00 - 0x09 */
    '(', ')', '_', '+',  0,   0,  'Q', 'W', 'E', 'R',  /* 0x0A - 0x13 */
    'T', 'Y', 'U', 'I', 'O', 'P', '{', '}',  0,   0,   /* 0x14 - 0x1D */
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',  /* 0x1E - 0x27 */
    '"', '~',  0,  '|', 'Z', 'X', 'C', 'V', 'B', 'N',  /* 0x28 - 0x31 */
    'M', '<', '>', '?',  0,  '*',  0,  ' ',  0,   0,   /* 0x32 - 0x3B */
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    /* 0x3C - 0x45 */
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    /* 0x46 - 0x4F */
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    /* 0x50 - 0x59 */
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    /* 0x5A - 0x63 */
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    /* 0x64 - 0x6D */
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    /* 0x6E - 0x77 */
    0,   0,   0,   0,   0,   0,   0,   0               /* 0x78 - 0x7F */
};

/* Tracks whether shift is currently held down 1 = held, 0 = not held */
static int shift_held = 0;

/* Local inb helper: reads a byte from an I/O port */
static inline uint8_t inb(uint16_t port) {
	uint8_t ret;
	__asm__ __volatile__("inb %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
}

/* keyboard_handler: Called from ISR 33 (IRQ1)
   Reads scancode from port 0x60, looks up the character, 
   and prints it if it's a printable key*/
void keyboard_handler(void) {
	uint8_t scancode = inb(0x60);

   /* shift pressed */
   if (scancode == 0x2A || scancode == 0x36) {
      shift_held = 1;
      return;
   }

   /* shift released - break codes for left/right shift */
   if (scancode == 0xAA || scancode == 0xB6) {
      shift_held = 0;
      return;
   }

	/* ignore break codes (key release events) */
	if (scancode >= 128) return;

   /* enter key - move to next line */
   if (scancode == 0x1C) {
      kprintf("\n");
      return;
   }

   /* backspace - move cursor back and erase */
   if (scancode == 0x0E) {
      vga_backspace();
      return;
   }

	/* look up the character in map */
	char c = shift_held ? shift_map[scancode] : scancode_map[scancode];

	/* print if printable */
	if (c != 0) {
		kprintf("%c", c);
	}
}