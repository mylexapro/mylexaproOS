/* ------------------------------------------------------------
File: scheduler.h
Project: mylexaproOS (x86 kernel)
Author: Annabelle Webb
Created: 2026-06-07
Description:
    Round-robin process scheduler. Cycles through the process
    table on every timer tick and switches to the next ready
    process.
------------------------------------------------------------ */

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"

/* called from timer_handler on every tick to schedule next process */
void scheduler_tick(void);

#endif