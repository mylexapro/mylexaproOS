/* ------------------------------------------------------------
 * File: vga.c
 * Project: mylexaproOS (x86 kernel)
 * Author: Annabelle Webb
 * Created: 2026-04-18
 * Description:
 *   Simple VGA text-mode driver. Provides screen clearing,
 *   character output, and string printing with a software
 *   cursor. Uses the VGA memory buffer at 0xB8000.
 * ------------------------------------------------------------ */

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
