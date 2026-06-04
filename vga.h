/* ------------------------------------------------------------
File: vga.h
Project: mylexaproOS (x86 kernel)
Author: Annabelle Webb
Created: 2026-04-18
Description:
  Constants and function declarations for the VGA text-mode
  driver. Provides screen dimensions, memory address, color
  codes, and output routines used throughout the kernel.
------------------------------------------------------------ */

#ifndef VGA_H
#define VGA_H

#include <stdint.h>

/* VGA text mode dimensions and memory location */
#define VGA_WIDTH	80
#define VGA_HEIGHT	25
#define VGA_MEMORY	0xB8000

/* VGA color attributes */
#define VGA_COLOR_BLACK	0
#define VGA_COLOR_PINK	0x0D

/* VGA driver functions */
void vga_clear(void);
void vga_putchar(char c, int x, int y, uint8_t color);
void vga_print(const char *str, uint8_t color);
void vga_backspace(void);
void vga_disable_cursor(void);
void vga_print_at(const char *str, int x, int y, uint8_t color);
void vga_print_number_at(uint32_t num, int x, int y, uint8_t color);

#endif

