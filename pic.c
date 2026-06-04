/* ------------------------------------------------------------
File: pic.c
Project: mylexaproOS (x86 kernel)
Author: Annabelle Webb
Created: 2026-04-18
Description:
  Functions for remapping and controlling the 8259 PIC.
  Remaps IRQs 0-15 to interrupt vectors 32-47 to avoid
  conflicts with CPU exceptions (0-31).
------------------------------------------------------------ */

#include "pic.h"
#include "io.h"

void pic_remap(void) {

    /* Initialize both PICs — must be done in this exact order (ICW sequence) */
    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);

    /* Remap: master IRQ0 → int 32, slave IRQ8 → int 40 */
    outb(PIC1_DATA, 32);
    outb(PIC2_DATA, 40);

    /* Tell PICs how they connect to each other */
    outb(PIC1_DATA, 4);
    outb(PIC2_DATA, 2);

    /* Set 8086 mode */
    outb(PIC1_DATA, ICW4_8086);
    outb(PIC2_DATA, ICW4_8086);

    /* IRQ mask: 0xFC = 11111100 — allow IRQ0 (timer) and IRQ1 (keyboard) only
       In PIC masks: 0 = allowed, 1 = blocked */
    outb(PIC1_DATA, 0xFC);
    outb(PIC2_DATA, 0xFF);
}

void pic_send_eoi(uint8_t irq) {
    /* Slave IRQs (8-15) need EOI sent to slave first, then master
       Master IRQs (0-7) only need EOI sent to master */
    if (irq >= 8) {
        outb(PIC2_COMMAND, 0x20);
    }
    outb(PIC1_COMMAND, 0x20);
}