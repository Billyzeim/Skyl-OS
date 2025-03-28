CC = gcc
CFLAGS = -m32 -ffreestanding -nostdlib -Wall -fno-pic
ASM = nasm
LD = ld

SRC_DIR = src
BUILD_DIR = build

# Find all .c and .asm files dynamically
C_FILES = $(shell find $(SRC_DIR) -type f -name "*.c")
ASM_FILES = $(shell find $(SRC_DIR) -type f -name "*.asm")

# Generate object file paths for all source files
C_OBJ = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(C_FILES))
ASM_OBJ = $(patsubst $(SRC_DIR)/%.asm,$(BUILD_DIR)/%.o,$(ASM_FILES))

all: kernel

# Ensure the build directory exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compile assembly files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.asm | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(ASM) -f elf32 $< -o $@

# Compile C files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Link kernel
kernel: $(ASM_OBJ) $(C_OBJ)
	$(LD) -m elf_i386 -T $(SRC_DIR)/kernel/linker.ld -o kernel $^

# Cleanup
clean:
	rm -rf $(BUILD_DIR) kernel