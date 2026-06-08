/* ------------------------------------------------------------
File: process.h
Project: mylexaproOS (x86 kernel)
Author: Annabelle Webb
Created: 2026-06-07
Description:
    Process Control Block (PCB) definition and process
    management declarations. Defines process states, saved
    CPU context, and the PCB struct used by the scheduler.
------------------------------------------------------------ */

#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>

/* process states */
typedef enum {
    PROCESS_READY,      /* waiting to be scheduled */
    PROCESS_RUNNING,    /* currently executing on the CPU */
    PROCESS_BLOCKED,    /* waiting for something (I/O, sleep, etc) */
    PROCESS_DEAD       /* finished, waiting to be cleaned up */
} process_state;

/* cpu_context: stores the full register state of a paused process saved and restored during every context switch */
typedef struct {
    uint32_t eax, ebx, ecx, edx;    /* general purpose registers */
    uint32_t esi, edi;              /* index registers */
    uint32_t esp, ebp;              /* stack pointer and base pointer */
    uint32_t eip;                   /* instruction pointer - where to resume */
    uint32_t eflags;                /* CPU flags (zero, carry, interrupt, etc) */
} cpu_context;

/* pcb: process control block - represents a single process, contains everything the scheduler needs to manage it */
typedef struct {
    uint32_t        pid;        /* unique process ID */
    process_state   state;      /* current state of this process */
    cpu_context     context;    /* saved register state */
} pcb;

/* initialize the process table */
void process_init(void);

/* create a new process, return its pid */
uint32_t process_create(void (*entry)(void));

/* called by the scheduler to switch from one process to another */
void process_switch(pcb *current, pcb *next);

#endif