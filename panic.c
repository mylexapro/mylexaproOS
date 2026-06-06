/* ------------------------------------------------------------
File: panic.c
Project: mylexaproOS (x86 kernel)
Author: Annabelle Webb
Created: 2026-06-06
Description:
    Kernel panic handler. Clears the screen, prints a panic
    message with exception number and error code, and halts.
------------------------------------------------------------ */

#include "panic.h"
#include "vga.h"
#include "kprintf.h"

void kpanic(const char *msg, uint32_t exception, uint32_t error_code) {
    vga_clear();
    kprintf("========================================\n");
    kprintf("            OH SHIT, TWIN               \n");
    kprintf("            KERNEL PANIC!               \n");
    kprintf("========================================\n\n");
    kprintf("Exception : %d\n", exception);
    kprintf("Error code: %x\n", error_code);
    kprintf("Message   : %s\n", msg);
    kprintf("\n------------------------------------\n");
    kprintf("  You broke something, twin :(\n");
    kprintf("------------------------------------\n");

    __asm__ __volatile__("cli; hlt");
}