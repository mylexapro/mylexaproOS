/* ------------------------------------------------------------
 * File: keyboard.h
 * Project: mylexaproOS (x86 kernel)
 * Author: Annabelle Webb
 * Created: 2026-04-18
 * Description:
 *   Declaration for the PS/2 keyboard interrupt handler.
 *   Implemented in keyboard.c and called from ISR 33.
 * ------------------------------------------------------------ */

#ifndef KEYBOARD_H
#define KEYBOARD_H

/* Keyboard IRQ1 handler */
void keyboard_handler(void);

#endif
