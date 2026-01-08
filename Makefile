# Toolchain
CC := gcc
LD := ld
AS := nasm
OBJCOPY := objcopy

# Flags
# -g adds debug symbols for GDB
CFLAGS := -m32 -ffreestanding -nostdlib -fno-pie -g -I include
ASFLAGS_KERNEL := -f elf32
ASFLAGS_BOOT := -f bin
# REMOVED --oformat binary from LDFLAGS to produce an ELF
LDFLAGS := -m elf_i386 -T linker.ld

# Build directories
BUILD_DIR := build
DIST_DIR := dist

# Source directories
BOOTLOADER_SRC_DIR := src/bootloader
KERNEL_SRC_DIR := src/kernel

# Find all source files
BOOTLOADER_SRCS := $(shell find $(BOOTLOADER_SRC_DIR) -name '*.asm')
KERNEL_C_SRCS := $(shell find $(KERNEL_SRC_DIR) -name '*.c')
KERNEL_ASM_SRCS := $(shell find $(KERNEL_SRC_DIR) -name '*.asm')

# Object outputs
BOOTLOADER_BIN := $(BUILD_DIR)/bootloader.bin
KERNEL_C_OBJS := $(patsubst $(KERNEL_SRC_DIR)/%.c,$(BUILD_DIR)/kernel/%.o,$(KERNEL_C_SRCS))
KERNEL_ASM_OBJS := $(patsubst $(KERNEL_SRC_DIR)/%.asm,$(BUILD_DIR)/kernel/%.o,$(KERNEL_ASM_SRCS))

ALL_OBJS := $(KERNEL_C_OBJS) $(KERNEL_ASM_OBJS)

# Final output files
KERNEL_ELF := $(BUILD_DIR)/kernel.elf
KERNEL_BIN := $(BUILD_DIR)/kernel.bin
OS_IMAGE := $(DIST_DIR)/os-image.bin

.PHONY: all run clean debug

all: $(OS_IMAGE)

# The image is the bootloader + the STRIPPED kernel binary
$(OS_IMAGE): $(BOOTLOADER_BIN) $(KERNEL_BIN) | $(DIST_DIR)
	cat $^ > $@

$(BOOTLOADER_BIN): $(BOOTLOADER_SRCS) | $(BUILD_DIR)
	@mkdir -p $(@D)
	$(AS) $(ASFLAGS_BOOT) $< -o $@
	truncate -s 512 $@

# 1. Link the ELF (This has symbols for debugging)
$(KERNEL_ELF): $(ALL_OBJS) linker.ld
	$(LD) $(LDFLAGS) -o $@ $(ALL_OBJS)

# 2. Extract the raw binary from the ELF for the actual bootloader to load
$(KERNEL_BIN): $(KERNEL_ELF)
	$(OBJCOPY) -O binary $< $@
	@echo "--- Kernel Build Summary ---"
	@BYTES=$$(stat -c %s $@); \
	 SECTORS=$$(( (BYTES + 511) / 512 )); \
	 echo "Physical Size: $$BYTES bytes"; \
	 echo "Disk Sectors:  $$SECTORS (Ceiling Division)"; \
	 echo "----------------------------"

# Compile C to ELF32 object
$(BUILD_DIR)/kernel/%.o: $(KERNEL_SRC_DIR)/%.c | $(BUILD_DIR)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile kernel ASM to ELF32 object
$(BUILD_DIR)/kernel/%.o: $(KERNEL_SRC_DIR)/%.asm | $(BUILD_DIR)
	@mkdir -p $(@D)
	$(AS) $(ASFLAGS_KERNEL) $< -o $@

# Make build and dist directories
$(BUILD_DIR) $(DIST_DIR):
	@mkdir -p $@

# Run with QEMU
run: $(OS_IMAGE)
	qemu-system-i386 -drive format=raw,file=$<

# NEW: Run with GDB support
# This starts QEMU and waits for a GDB connection
debug: $(OS_IMAGE) $(KERNEL_ELF)
	qemu-system-i386 -S -s -drive format=raw,file=$< &
	gdb -ex "target remote localhost:1234" -ex "symbol-file $(KERNEL_ELF)"

clean:
	rm -rf $(BUILD_DIR) $(DIST_DIR)