# Toolchain
CC = gcc
LD = ld
AS = nasm

# Flags
CFLAGS = -m32 -ffreestanding -nostdlib -fno-pie
LDFLAGS = -m elf_i386 -T linker.ld --oformat binary

# Paths
BOOTLOADER_SRC = src/bootloader/boot.asm
KERNEL_SRC = src/kernel/kernel.c

BOOTLOADER_BIN = build/bootloader.bin
KERNEL_OBJ = build/kernel.o
KERNEL_BIN = build/kernel.bin
OS_IMAGE = build/os-image.bin

# Ensure build directory exists
BUILD_DIR := build

all: $(BUILD_DIR) $(OS_IMAGE)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BOOTLOADER_BIN): $(BOOTLOADER_SRC)
	$(AS) -f bin $< -o $@
	truncate -s 512 $@

$(KERNEL_OBJ): $(KERNEL_SRC)
	$(CC) $(CFLAGS) -c $< -o $@

$(KERNEL_BIN): $(KERNEL_OBJ) linker.ld
	$(LD) $(LDFLAGS) -o $@ $<

$(OS_IMAGE): $(BOOTLOADER_BIN) $(KERNEL_BIN)
	cat $(BOOTLOADER_BIN) $(KERNEL_BIN) > $@

run: $(OS_IMAGE)
	qemu-system-i386 -drive format=raw,file=$<

clean:
	rm -rf build

.PHONY: all run clean
