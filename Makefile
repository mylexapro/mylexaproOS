# ------------------------------------------------------------
# File: Makefile
# Project: mylexaproOS (x86 kernel)
# Author: Annabelle Webb
# Created: 2026-04-18
# Description:
#   Build system for assembling the boot sector, compiling the
#   32-bit kernel, linking it into an ELF, converting to a raw
#   binary, and producing a bootable disk image.
# ------------------------------------------------------------

AS = nasm
CC = gcc
LD = ld
OBJCOPY = objcopy

CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra

all: os-image.bin

# Build boot sector (16-bit real mode)
boot.bin: boot.asm
	$(AS) -f bin boot.asm -o boot.bin

# Compile kernel + drivers + ISRs
kernel.o: kernel.c vga.c vga.h kprintf.c kprintf.h
	$(AS) -f elf32 isr.asm -o isr.o
	$(CC) $(CFLAGS) -c kernel.c -o kernel.o
	$(CC) $(CFLAGS) -c vga.c -o vga.o
	$(CC) $(CFLAGS) -c kprintf.c -o kprintf.o
	$(CC) $(CFLAGS) -c idt.c -o idt.o
	$(CC) $(CFLAGS) -c pic.c -o pic.o
	$(CC) $(CFLAGS) -c keyboard.c -o keyboard.o
	$(CC) $(CFLAGS) -c memory.c -o memory.o
	$(CC) $(CFLAGS) -c timer.c -o timer.o

# Link kernel into a 32-bit ELF at 0x8000
kernel.elf: kernel.o
	$(LD) -m  elf_i386 -T linker.ld -o kernel.elf kernel.o vga.o kprintf.o idt.o isr.o pic.o keyboard.o memory.o timer.o

# Convert ELF to raw binary
kernel.bin: kernel.elf
	$(OBJCOPY) -O binary kernel.elf kernel.bin

# Combine boot sector + kernel into final image
os-image.bin: boot.bin kernel.bin
	cat boot.bin kernel.bin > os-image.bin

# Run in QEMU
run: os-image.bin
	qemu-system-i386 -drive format=raw,file=os-image.bin

# Clean build artifacts
clean:
	rm -f *.o *.elf *.bin
