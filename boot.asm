; ------------------------------------------------------------
; File: boot.asm
; Project: mylexaproOS (x86 kernel)
; Author: Annabelle Webb
; Created: 2026-04-18
; Description:
;   BIOS boot sector (stage 1). Loads the next 10 sectors,
;   switches to 32-bit protected mode, clears VGA text memory,
;   and jumps to the 32-bit kernel entry at 0x8000.
; ------------------------------------------------------------

BITS 16
ORG 0x7C00

start:
	; Load 10 sectors starting at LBA 2 into ES:BX = 0x0800:0000
	mov ax, 0x0800
	mov es, ax
	mov bx, 0

	mov ah, 0x02				; BIOS disk read function
	mov al, 10					; number of sectors
	mov ch, 0					; cylinder
	mov cl, 2					; sector (starts at 1, so 2 = second sector)
	mov dh, 0					; head
	int 0x13					; BIOS disk interrupt

	; E820 memory map detection
	; Store count at 0x4FC, entries starting at 0x500
	mov di, 0x500				; ES is still 0x0800 from disk load - reset it to 0 first

	xor ax, ax
	mov es, ax					; ES = 0, so es:di = 0x0000:0x0500 = 0x500
	mov di, 0x500

	xor ebx, ebx				; start of list
	xor bp, bp 					; entry count

.e820_loop:
	mov eax, 0xE820
	mov ecx, 24
	mov edx, 0x534D4150			; "SMAP"
	int 0x15

	jc .e820_done				; carry flag set = error or e820_done
	cmp ebx, 0
	je .e820_done				; ebx = 0 means last entry

	inc bp						; count this entry
	add di, 24					; move to next slot
	jmp .e820_loop
	
.e820_done:
	mov [0x4FC], bp				; store entry count

	cli							; disable interrupts before mode switch
	lgdt [gdt_descriptor]

	; Enter protected mode 
	mov eax, cr0
	or eax, 1
	mov cr0, eax
	jmp 0x08:protected_mode		; far jump to 32-bit code segment

BITS 32
protected_mode:
	; Load data segments 
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov esp, 0x90000			; temporary stack 

	; Clear VGA text buffer (80x25 = 2000 chars)
	mov ecx, 2000
	mov edi, 0xB8000
.clear:
	mov byte [edi], ' '			; character 	
	mov byte [edi+1], 0x00		; attribute
	add edi, 2
	loop .clear

	; Jump to kernel entry at physical 0x8000
	mov eax, 0x8000
	call eax

.hang:
	cli
	hlt
	jmp .hang

; ---------------- GDT ----------------

gdt_start:
	dq 0						; null descriptor

gdt_code:
	dw 0xFFFF					; limit
	dw 0x0000					; base
	db 0x00
	db 10011010b				; code segment flags
	db 11001111b				; granularity
	db 0x00

gdt_data:
	dw 0xFFFF
	dw 0x0000
	db 0x00
	db 10010010b				; data segment flags
	db 11001111b
	db 0x00

gdt_end:

gdt_descriptor:
	dw gdt_end - gdt_start - 1
	dd gdt_start

; Boot signature 
times 510-($-$$) db 0
dw 0xAA55
