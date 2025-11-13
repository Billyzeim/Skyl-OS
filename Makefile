# Toolchain
CC := gcc
LD := ld
AS := nasm

# Flags
CFLAGS := -m32 -ffreestanding -nostdlib -fno-pie -I include -fno-asynchronous-unwind-tables -fno-unwind-tables -fno-stack-protector
ASFLAGS_KERNEL := -f elf32
ASFLAGS_BOOT := -f bin
LDFLAGS := -m elf_i386 -T linker.ld --oformat binary

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

# ELF32 object outputs
BOOTLOADER_ELF_OBJS := $(patsubst $(BOOTLOADER_SRC_DIR)/%.asm,$(BUILD_DIR)/bootloader/%.o,$(BOOTLOADER_SRCS))
KERNEL_C_OBJS := $(patsubst $(KERNEL_SRC_DIR)/%.c,$(BUILD_DIR)/kernel/%.o,$(KERNEL_C_SRCS))
KERNEL_ASM_OBJS := $(patsubst $(KERNEL_SRC_DIR)/%.asm,$(BUILD_DIR)/kernel/%.o,$(KERNEL_ASM_SRCS))

ALL_OBJS := $(KERNEL_C_OBJS) $(KERNEL_ASM_OBJS)

# Final output files
BOOTLOADER_BIN := $(BUILD_DIR)/bootloader.bin
KERNEL_BIN := $(BUILD_DIR)/kernel.bin
OS_IMAGE := $(DIST_DIR)/os-image.bin

.PHONY: all run clean asm-elf docs

all: $(OS_IMAGE)

$(OS_IMAGE): $(BOOTLOADER_BIN) $(KERNEL_BIN) | $(DIST_DIR)
	cat $^ > $@

$(BOOTLOADER_BIN): $(BOOTLOADER_SRCS) | $(BUILD_DIR)
	@mkdir -p $(@D)
	$(AS) $(ASFLAGS_BOOT) $< -o $@
	truncate -s 512 $@

$(KERNEL_BIN): $(ALL_OBJS) linker.ld
	$(LD) $(LDFLAGS) -o $@ $(ALL_OBJS)

# Compile C to ELF32 object
$(BUILD_DIR)/kernel/%.o: $(KERNEL_SRC_DIR)/%.c | $(BUILD_DIR)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile kernel ASM to ELF32 object
$(BUILD_DIR)/kernel/%.o: $(KERNEL_SRC_DIR)/%.asm | $(BUILD_DIR)
	@mkdir -p $(@D)
	$(AS) $(ASFLAGS_KERNEL) $< -o $@

# Compile bootloader ASM to ELF32 object (optional, not used in binary image)
$(BUILD_DIR)/bootloader/%.o: $(BOOTLOADER_SRC_DIR)/%.asm | $(BUILD_DIR)
	@mkdir -p $(@D)
	$(AS) $(ASFLAGS_KERNEL) $< -o $@

# Make build and dist directories
$(BUILD_DIR) $(DIST_DIR):
	@mkdir -p $@

# Run with QEMU (x86_64)
run: $(OS_IMAGE)
	qemu-system-x86_64 -drive format=raw,file=$<

# Build all ELF32 objects from ASM sources
asm-elf: $(KERNEL_ASM_OBJS) $(BOOTLOADER_ELF_OBJS)

# Cleanup
clean:
	rm -rf $(BUILD_DIR) $(DIST_DIR)

# Docs placeholder
docs:
	@echo "Generating documentation..."
