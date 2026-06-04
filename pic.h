/* ------------------------------------------------------------
File: pic.h
Project: mylexaproOS (x86 kernel)
Author: Annabelle Webb
Created: 2026-04-18
Description:
  Definitions and constants for the 8259 Programmable
  Interrupt Controller (PIC). Provides port addresses,
  initialization command words, and function prototypes
  for remapping and sending EOIs.
------------------------------------------------------------ */

#ifndef PIC_H
#define PIC_H

#include <stdint.h>

/* PIC I/O port addresses */
#define PIC1_COMMAND	0x20
#define PIC1_DATA	0x21
#define PIC2_COMMAND	0xA0
#define PIC2_DATA	0xA1

/* Initialization Command Words (ICWs) */
#define ICW1_INIT	0x10    // Start initialization
#define ICW1_ICW4	0x01    // Expect ICW4
#define ICW4_8086	0x01    // 8086/88 mode

/* PIC control functions */
void pic_remap(void);
void pic_send_eoi(uint8_t irq);

#endif
