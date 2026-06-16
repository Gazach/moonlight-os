# ====================================================================
# Moonlight Kernel
# ====================================================================

# Toolchain
ASM := nasm
CC  := clang
LD  := ld.lld

# Directories
SRCDIR   := src
BUILDDIR := build

# Output artifacts
ISO_DIR  := $(BUILDDIR)/iso
ISO_NAME := $(BUILDDIR)/moonlight.iso
KERNEL_ELF := $(BUILDDIR)/kernel.elf

# Compiler flags
CFLAGS := --target=i386-pc-none-elf \
          -ffreestanding \
          -nostdlib \
          -fno-builtin \
          -fno-stack-protector \
          -mno-sse \
          -mno-sse2 \
          -mno-avx \
          -mno-80387 \
          -mno-mmx \
          -march=i686 \
          -I$(SRCDIR)/kernel \
          -I$(SRCDIR)/kernel/common \
          -I$(SRCDIR)/kernel/std

ASMFLAGS := -f elf32
LDFLAGS  := -m elf_i386 -T $(SRCDIR)/linker.ld

# Sources
C_SRCS := $(SRCDIR)/kernel/kernel.c \
          $(SRCDIR)/kernel/vga.c \
          $(SRCDIR)/kernel/terminal.c \
          $(SRCDIR)/kernel/std/printf.c \
          $(SRCDIR)/kernel/common/string.c \
          $(SRCDIR)/kernel/common/div64.c

ASM_SRCS := $(SRCDIR)/boot/boot.asm

# Objects (mirror source tree under build/)
C_OBJS   := $(C_SRCS:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)
ASM_OBJS := $(ASM_SRCS:$(SRCDIR)/%.asm=$(BUILDDIR)/%.o)
OBJS     := $(ASM_OBJS) $(C_OBJS)

# ====================================================================
.PHONY: all run clean

all: $(ISO_NAME)

# Compile C sources
$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Assemble ASM sources
$(BUILDDIR)/%.o: $(SRCDIR)/%.asm
	@mkdir -p $(dir $@)
	$(ASM) $(ASMFLAGS) $< -o $@

# Link kernel
$(KERNEL_ELF): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $@

# Build ISO
$(ISO_NAME): $(KERNEL_ELF)
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(KERNEL_ELF) $(ISO_DIR)/boot/kernel.elf
	printf 'menuentry "Moonlight Kernel" {\n\tmultiboot2 /boot/kernel.elf\n\tboot\n}\n' > $(ISO_DIR)/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO_NAME) $(ISO_DIR)

run: $(ISO_NAME)
	qemu-system-i386 -cdrom $(ISO_NAME)

clean:
	rm -rf $(BUILDDIR)

.PHONY: all run clean