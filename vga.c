/* ------------------------------------------------------------
File: vga.c
Project: mylexaproOS (x86 kernel)
Author: Annabelle Webb
Created: 2026-04-18
Description:
  Simple VGA text-mode driver. Provides screen clearing,
  character output, and string printing with a software
  cursor. Uses the VGA memory buffer at 0xB8000.
------------------------------------------------------------ */

#include "vga.h"

/* VGA text buffer starts at physical address 0xB8000 */
static uint16_t *vga_buffer = (uint16_t*)VGA_MEMORY;

/* Software cursor position */
static int cursor_x = 0;
static int cursor_y = 0;

/* vga_clear: Clears the entire 80x25 VGA text buffer by writing spaces with black background. Resets cursor to (0,0) */
void vga_clear(void) {
	for (int y = 0; y < VGA_HEIGHT; y++) {
		for (int x = 0; x < VGA_WIDTH; x++) {
			int index = y * VGA_WIDTH + x;
			vga_buffer[index] = ((uint16_t)VGA_COLOR_BLACK << 8) | ' ';
		}
	}
	cursor_x = 0;
	cursor_y = 0;
}

/* vga_putchar: Writes a single character at a specific (x,y) position with the given color attribute */
void vga_putchar(char c, int x, int y, uint8_t color) {
	int index = y * VGA_WIDTH + x;
	vga_buffer[index] = ((uint16_t)color << 8) | c;
}

void vga_print(const char *str, uint8_t color) {
	while (*str) {
		if (*str == '\n') {
			cursor_x = 0;
			cursor_y++;
		} else {
			vga_putchar(*str, cursor_x, cursor_y, color);
			cursor_x++;
			if (cursor_x >= VGA_WIDTH) {
				cursor_x = 0;
				cursor_y++;
			}
		}
		if (cursor_y >= VGA_HEIGHT) {
			cursor_y = 0;	// TODO: implement scrolling later
		}
		str++;
	}
}

/* vag_backspace: moves the cursor back one position and erases the character by writing a space
   Does nothing if already at the start of a line. */
void vga_backspace(void) {

	/* don't go past the beginning of the line */
	if (cursor_x > 0) {
		cursor_x--;
		vga_putchar(' ', cursor_x, cursor_y, VGA_COLOR_BLACK);
	}
}

/* vga_diable_cursor: Disables the hardware VGA blinking cursor by writing to the cursor start register via VGA control ports.
	The cursor is seperate from software cursor and needs to be explicity disabled through port I/O. */
void vga_disable_cursor(void) {
	__asm__ __volatile__(
		"outb %0, %1" : : "a"((uint8_t)0x0A), "Nd"((uint16_t)0x3D4)
	);
	__asm__ __volatile__(
		"outb %0, %1" : : "a"((uint8_t)0x20), "Nd"((uint16_t)0x3D5)
	);
}

/* vga_print_at: prints a string at a specific x,y position without moving the main software cursor */
void vga_print_at(const char *str, int x, int y, uint8_t color) {
	while (*str) {
		vga_putchar(*str, x, y, color);
		x++;
		str++;
	}
}

/* vga_print_number_at: prints a single digit number at x,y without moving the main software cursor */
void vga_print_number_at(uint32_t num, int x, int y, uint8_t color) {
	char buf[12];
	int i = 0;

	if (num == 0) {
		vga_putchar('0', x, y, color);
		return;
	}

	while (num > 0) {
		buf[i++] = '0' + (num % 10);
		num /= 10;
	}

	/* print digits in correct order */
	for (int j = i - 1; j >= 0; j--) {
		vga_putchar(buf[j], x, y, color);
		x++;
	}
}