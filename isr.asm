; ------------------------------------------------------------
; File: isr.asm
; Project: mylexaproOS (x86 kernel)
; Author: Annabelle Webb
; Created: 2026-04-18
; Description:
;   Interrupt Service Routine (ISR) stubs for CPU exceptions
;   and hardware IRQs. These run in 32-bit protected mode and
;   pass control to C-level handlers.
; ------------------------------------------------------------

BITS 32

global isr0
global isr1
global isr33

; -------------------------
; CPU Exception: Divide-by-zero (#0)
; -------------------------
isr0:
	cli
	push byte 0
	push byte 0
	jmp isr_common

; -------------------------
; CPU Exception: Debug (#1)
; -------------------------
isr1:
	cli
	push byte 0
	push byte 1
	jmp isr_common

; -------------------------
; Hardware IRQ1: Keyboard (mapped to interrupt 33)
; -------------------------
isr33:
	cli
	push byte 0
	push byte 33
	jmp isr_common_kb

; -------------------------
; Keyboard-specific ISR handler
; Calls keyboard_handler() in C
; -------------------------
isr_common_kb:
	pusha

	extern keyboard_handler
	call keyboard_handler

	mov al, 0x20
	out 0x20, al

	popa
	add esp, 8			; remove pushed interrupt number + error code
	iret

; -------------------------
; Generic ISR handler for exceptions
; Saves segment registers, switches to kernel segments,
; then restores everything and returns.
; -------------------------
isr_common:
	pusha
	push ds
	push es
	push fs
	push gs

	mov ax, 0x10		; kernel data segment
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	popa
	pop gs
	pop fs
	pop es
	pop ds
	add esp, 8			; remove interrupt number + error code
	iret
