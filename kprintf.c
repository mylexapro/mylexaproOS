/* ------------------------------------------------------------
 * File: kprintf.c
 * Project: mylexaproOS (x86 kernel)
 * Author: Annabelle Webb
 * Created: 2026-04-18
 * Description:
 *   Minimal printf-style formatting for VGA text mode output.
 *   Supports %d, %x, %s, and %% using vga_print() as the backend.
 * ------------------------------------------------------------ */

#include "vga.h"
#include <stdarg.h>

/* print_decimal: Converts a signed integer to decimal text and prints it. No dynamic allocation, no recursion — safe for kernel use */
static void print_decimal(int num) {
	if (num == 0) {
		vga_print("0", VGA_COLOR_PINK);
		return;
	}

	if (num < 0) {
		vga_print("-", VGA_COLOR_PINK);
		num = -num;
	}

	char buffer[12];
	int i = 0;

	while (num > 0) {
		buffer[i++] = '0' + (num % 10);
		num /= 10;
	}

	for (int j = i - 1; j >= 0; j--) {
		char c[2] = {buffer[j], '\0'};
		vga_print(c, VGA_COLOR_PINK);
	}
}

/* print_hex: Converts an unsigned integer to hexadecimal text. No "0x" prefix — raw hex output for debugging */
static void print_hex(unsigned int num) {
	char buffer[9];
	const char *digits = "0123456789ABCDEF";
	int i = 0;

	if (num == 0) {
		vga_print("0", VGA_COLOR_PINK);
		return;
	}

	while (num > 0) {
		buffer[i++] = digits[num % 16];
		num /= 16;
	}

	for (int j = i - 1; j >= 0; j--) {
		char c[2] = {buffer[j], '\0'};
		vga_print(c, VGA_COLOR_PINK);
	}
}

/*
 * kprintf:
 *   Lightweight kernel printf implementation.
 *   Supported:
 *     %d  - decimal integer
 *     %x  - hexadecimal integer
 *     %s  - C-string
 *     %%  - literal '%'
 *
 *   Uses vga_print() for all output.
 */
void kprintf(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);

	while (*fmt) {
		if (*fmt == '%') {
			fmt++;
			switch (*fmt) {
				case 'd':
					print_decimal(va_arg(args, int));
					break;
				case 'x':
					print_hex(va_arg(args, unsigned int));
					break;
				case 's':
					vga_print(va_arg(args, const char*), VGA_COLOR_PINK);
					break;
				case '%':
					vga_print("%", VGA_COLOR_PINK);
					break;
				case 'c': {
					char c = (char)va_arg(args, int);
					char str[2] = {c, '\0'};
					vga_print(str, VGA_COLOR_PINK);
					break;
				}
			}
		} else {
			char c[2] = {*fmt, '\0'};
			vga_print(c, VGA_COLOR_PINK);
		}
		fmt++;
	}

	va_end(args);
}
