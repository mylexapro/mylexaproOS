/* ------------------------------------------------------------
File: panic.h
Project: mylexaproOS (x86 kernel)
Author: Annabelle Webb
Created: 2026-06-06
Description:
    Kernel panic handler. Displays a panic screen with the
    exception number and error code, then halts the CPU.
------------------------------------------------------------ */

#ifndef PANIC_H
#define PANIC_H

#include <stdint.h>

void kpanic(const char *msg, uint32_t exception, uint32_t error_code);

#endif