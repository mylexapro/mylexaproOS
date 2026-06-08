; ------------------------------------------------------------
; File: switch.asm
; Project: mylexaproOS (x86 kernel)
; Author: Annabelle Webb
; Created: 2026-06-07
; Description:
;   Low-level context switch implementation. Saves the current
;   process register state and restores the next process state.
;   Called from process_switch() in process.c
; ------------------------------------------------------------

BITS 32
global switch_context

switch_context:
    ; get pointer to current context into eax
    mov eax, [esp + 4]

    ; save all registers into current context
    mov [eax + 0],  eax
    mov [eax + 4],  ebx
    mov [eax + 8],  ecx
    mov [eax + 12], edx
    mov [eax + 16], esi
    mov [eax + 20], edi
    mov [eax + 24], esp
    mov [eax + 28], ebp

    ;save eip - we use the return address as eip
    mov ecx, [esp]
    mov [eax + 32], ecx

    ; save eflags
    pushfd
    pop ecx
    mov [eax + 36], ecx

    ; get pointer to next context into eax
    mov eax, [esp + 8]

    ; restore all registers from the next context
    mov ebx, [eax + 4]
    mov ecx, [eax + 8]
    mov edx, [eax + 12]
    mov esi, [eax + 16]
    mov edi, [eax + 20]
    mov esp, [eax + 24]
    mov ebp, [eax + 28]

    ; restore eflags
    mov ecx, [eax + 36]
    push ecx
    popfd

    ; jump to next process eip
    mov eax, [eax + 32]
    jmp eax