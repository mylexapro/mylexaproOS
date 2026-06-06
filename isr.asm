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

; export symbols so the linker can find them from C/idt.c
global isr0
global isr1
global isr14
global isr32
global isr33

; CPU Exception: Divide-by-zero (#0)
; Fires when code tries to divide by zero
isr0:
	cli							; Disable interrupts while handling
	push byte 0					; Dummy error code (exception has none)
	push byte 0					; Interrupt number
	jmp isr_common				; Jump to generic exception handler

; CPU Exception: Debug (#1)
; Fires during single-step debugging
isr1:
	cli							; Diable interrupts while handling
	push byte 0					; Dummy error code
	push byte 1					; Interrupt number
	jmp isr_common				; Jump to generic exception handler

; CPU Exception: Page Fault (#14)
; CPU automatically pushes an error code
isr14:
	cli
	push byte 14
	jmp isr_common

; Hardware IRQ0: System Timer (mapped to interrupt 32)
; Fires ~18 times per second from the PIT chip
isr32:
	cli							; Disable interrupts while handling
	push byte 0					; Dummy error code (hardware IRQ has none)
	push byte 32				; Interrupt number
	jmp isr_common_timer		; Jump to timer-specific handler

; Timer Common Handler
; Saves registers, calls C handler, restores registers, returns
isr_common_timer:
	pusha						; Save all general purpose registers
	extern timer_handler		; timer_handler is defined in timer.c
	call timer_handler			; Calls the C timer handler
	popa						; Restore all general purpose registers
	add esp, 8					; Clean up 2 push bytes (2 x 4 = 8 bytes)
	iret						; Return from interrupt

; Hardware IRQ1: Keyboard (mapped to interrupt 33)
; Fires when key is pressed or released
isr33:
	cli							; Disable interrupts while handling
	push byte 0					; Dummy error code
	push byte 33				; Interrupt number
	jmp isr_common_kb			; Jump to keyboard-specific handler

; Keyboard Common Handler
; Saves registers, calls C handler, sends EOI, restores, returns
isr_common_kb:
	pusha						; Save all general purpose registers

	extern keyboard_handler		; Keyboard handler is defined in keyboard.c
	call keyboard_handler		; Call the C keyboard handler

	mov al, 0x20				; EOI (End of Interrupt) command
	out 0x20, al				; Send EOI to master PIC (port 0x20)

	popa						; Restore all gerneral purpose registers
	add esp, 8					; Clean up 2 push bytes (2 x 4 = 8 bytes)
	iret						; Return from interrupt

; Generic ISR handler for exceptions, saves segment registers, switches to kernel segments, 
; then restores everything and returns.
isr_common:
	pusha						; Save all general purpose registers
	push ds						; Save data segment
	push es						; Save extra segment
	push fs						; Save FS segment
	push gs						; Save GS segment

	mov ax, 0x10				; Kernel data segment selector
	mov ds, ax					; Reload all segment registers
	mov es, ax					; With kernel data segment
	mov fs, ax					; To ensure were operating in
	mov gs, ax					; Kernel memory space

	extern kpanic				; Calls kpanic with exception info
	push dword [esp + 40]		; Push error code
	push dword [esp + 44]		; Push exception number
	push exception_msg			; Push message string
	call kpanic

	add esp, 12					; Clean up 3 pushed args
	pop gs						; Restore GS (reverse order of push)
	pop fs						; Restore FS
	pop es						; Restore ES
	pop ds						; Restore DS
	popa						; Restore general purpose registers
	add esp, 8					; Clean up interrupt number + error code
	iret						; Return from interrupt

exception_msg db "CPU Exception", 0
