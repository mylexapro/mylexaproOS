/* ------------------------------------------------------------
 * File: io.h
 * Project: mylexaproOS (x86 kernel)
 * Author: Annabelle Webb
 * Created: 2026-04-18
 * Description:
 *   Inline functions for low-level port I/O using the x86
 *   inb/outb instructions. Used for communicating with PIC,
 *   keyboard controller, and other hardware devices.
 * ------------------------------------------------------------ */

#ifndef IO_H
#define IO_H

#include <stdint.h>

/*
 * inb:
 *   Read a byte from an I/O port.
 *   port - I/O port number
 *   returns: 8-bit value read from the port
 */
static inline uint8_t inb(uint16_t port) {
	uint8_t result;
	__asm__ volatile ("inb %1, %0" : "=a"(result) : "Nd"(port));
	return result;
}

/*
 * outb:
 *   Write a byte to an I/O port.
 *   port - I/O port number
 *   data - 8-bit value to write
 */
static inline void outb(uint16_t port, uint8_t data) {
	__asm__ volatile ("outb %0, %1" : : "a"(data), "Nd"(port));
}

#endif
