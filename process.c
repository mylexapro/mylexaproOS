/* ------------------------------------------------------------
File: process.c
Project: mylexaproOS (x86 kernel)
Author: Annabelle Webb
Created: 2026-06-07
Description:
    Process management implementation. Maintains a table of
    PCBs, handles process creation, and performs round-robin
    scheduling with context switching.
------------------------------------------------------------ */

#include "process.h"
#include "kmalloc.h"

/* switch_context is implemented in switch.asm */
extern void switch_context(cpu_context *current, cpu_context *next);

/* maximum number of processes the scheduler can track */
#define MAX_PROCESSES 16

/* process table - one pcb per slot*/
pcb process_table[MAX_PROCESSES];

/* index of the currently running process */
uint32_t current_pid = 0;

/* total number of processes created so far */
static uint32_t process_count = 0;

/* process_init: zeros out the process table so all slots start empty and ready to be assigned */
void process_init(void) {
    for (uint32_t i = 0; i < MAX_PROCESSES; i++) {
        process_table[i].state = PROCESS_DEAD;
        process_table[i].pid = 0;
    }
}

/* process_create: finds a free slot in the process table, sets up the PCB, allocates a stack, and returns the PID */
uint32_t process_create(void (*entry)(void)) {
    /* find a free slot */
    for (uint32_t i = 0; i < MAX_PROCESSES; i++) {
        if (process_table[i].state == PROCESS_DEAD) {

            /* fill in PCB */
            process_table[i].pid    = ++process_count;
            process_table[i].state  = PROCESS_READY;

            /* allocate a stack for this process (4KB) */
            uint8_t *stack = kmalloc(4096);

            /* set up context so it starts at entry function */
            process_table[i].context.eip = (uint32_t)entry;
            process_table[i].context.esp = (uint32_t)(stack + 4096);
            process_table[i].context.eflags = 0x200;

            return process_table[i].pid;
        }
    }

    /* no free slot found */
    return 0;
}

/* process_switch: saves current process context and restores next. implemented in switch.asm */
void process_switch(pcb *current, pcb *next) {
    switch_context(&current->context, &next->context);
}