/* ------------------------------------------------------------
 * File: kprintf.h
 * Project: mylexaproOS (x86 kernel)
 * Author: Annabelle Webb
 * Created: 2026-04-18
 * Description:
 *   Header for the kernel's minimal printf implementation.
 *   Provides kprintf() for formatted VGA text output.
 * ------------------------------------------------------------ */

#ifndef KPRINTF_H
#define KPRINTF_H

/* Kernel printf (supports %d, %x, %s, and %%) */
void kprintf(const char *fmt, ...);

#endif
