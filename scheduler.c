/* ------------------------------------------------------------
File: scheduler.c
Project: mylexaproOS (x86 kernel)
Author: Annabelle Webb
Created: 2026-06-07
Description:
    Round-robin scheduler implementation. On each timer tick,
    finds the next READY process and switches to it.
------------------------------------------------------------ */

#include "scheduler.h"
#include "process.h"

#define MAX_PROCESSES 16

extern pcb process_table[];
extern uint32_t current_pid;

void scheduler_tick(void) {
    /* find current process index */
    uint32_t current = current_pid % MAX_PROCESSES;
    uint32_t next = (current + 1) % MAX_PROCESSES;

    /* find next ready process */
    for (uint32_t i = 0; i < MAX_PROCESSES; i++) {
        if (process_table[next].state == PROCESS_READY) {
            process_table[current].state = PROCESS_READY;
            process_table[next].state = PROCESS_RUNNING;
            current_pid = next;
            process_switch(&process_table[current], &process_table[next]);
            return;
        }
        next = (next + 1) % MAX_PROCESSES;
    }
}