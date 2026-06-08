/* ------------------------------------------------------------
File: timer.c
Project: mylexaproOS (x86 kernel)
Author: Annabelle Webb
Created: 2026-06-03
Description:
    PIT timer driver. Handles IRQ0 (~18 ticks per second).
    Increments a tick counter and displays uptime on screen.
------------------------------------------------------------ */

#include "timer.h"
#include "pic.h"
#include "vga.h"
#include "kprintf.h"
#include "scheduler.h"

/* tick counter - incremented every IRQ0 interrupt */
volatile uint32_t timer_ticks = 0;

/* timer_handler: called from isr32 on every timer tick
   increments tick counter and updates uptime display */
void timer_handler(void) {
    timer_ticks++;

    /* update uptime in top right corner of screen
       every 18 ticks is roughly one second */
    uint32_t seconds = timer_ticks / 18;

    /* save cursor position, print uptime, restore cursor */
    vga_print_at("Uptime:      ", 68, 0, VGA_COLOR_PINK);
    vga_print_number_at(seconds, 76, 0, VGA_COLOR_PINK);

    scheduler_tick();

    /* tell PIC we handled IRQ0 */
    pic_send_eoi(0);
}

/* timer_init: registers the timer handler in the IDT */
void timer_init(void) {
    /* IDT registration is handled in idt_init()
       this function is here for future PIT reprogramming */
}

