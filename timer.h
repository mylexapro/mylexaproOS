/* ------------------------------------------------------------
File: timer.h
Project: mylexaproOS (x86 kernel)
Author: Annabelle Webb
Created: 2026-06-03
Description:
    Timer driver header. Declares the IRQ0 handler and
    timer initialization function.
------------------------------------------------------------ */

#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

/* tick counter - incremented every timer interrupt (~18x per second) */
extern volatile uint32_t timer_ticks;

void timer_init(void);
void timer_handler(void);

#endif