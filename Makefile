ARCH=i686-elf
CC=$(ARCH)-gcc
LD=$(ARCH)-ld
AS=nasm
OBJCOPY=$(ARCH)-objcopy

CFLAGS=-m32 -ffreestanding -fno-pic -fno-stack-protector -nostdlib -nostdinc -Wall -Wextra -Iinclude
LDFLAGS=-T linker.ld -nostdlib

BUILD=build

C_SOURCES=$(wildcard kernel/*.c drivers/*.c graphics/*.c gui/*.c)
C_OBJS=$(patsubst %.c,$(BUILD)/%.o,$(C_SOURCES))
ASM_KERNEL_OBJS=$(BUILD)/kernel/entry.o $(BUILD)/kernel/interrupts.o

all: zero3.iso

$(BUILD):
	mkdir -p $(BUILD)/kernel $(BUILD)/drivers $(BUILD)/graphics $(BUILD)/gui $(BUILD)/bootloader

$(BUILD)/bootloader/boot.bin: bootloader/boot.asm | $(BUILD)
	$(AS) -f bin $< -o $@

$(BUILD)/bootloader/stage2.bin: bootloader/stage2.asm | $(BUILD)
	$(AS) -f bin $< -o $@

$(BUILD)/kernel/entry.o: kernel/entry.asm | $(BUILD)
	$(AS) -f elf32 $< -o $@

$(BUILD)/kernel/interrupts.o: kernel/interrupts.asm | $(BUILD)
	$(AS) -f elf32 $< -o $@

$(BUILD)/%.o: %.c | $(BUILD)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/kernel/kernel.elf: $(ASM_KERNEL_OBJS) $(C_OBJS) linker.ld
	$(LD) -m elf_i386 $(LDFLAGS) -o $@ $(ASM_KERNEL_OBJS) $(C_OBJS)

$(BUILD)/kernel/kernel.bin: $(BUILD)/kernel/kernel.elf
	$(OBJCOPY) -O binary $< $@

os-image.bin: $(BUILD)/bootloader/boot.bin $(BUILD)/bootloader/stage2.bin $(BUILD)/kernel/kernel.bin
	cat $(BUILD)/bootloader/boot.bin $(BUILD)/bootloader/stage2.bin $(BUILD)/kernel/kernel.bin > $@

zero3.iso: os-image.bin
	mkdir -p iso/boot
	cp os-image.bin iso/boot/os-image.bin
	xorriso -as mkisofs -R -b boot/os-image.bin -no-emul-boot -boot-load-size 4 -o zero3.iso iso

iso: zero3.iso

run: os-image.bin
	qemu-system-i386 -drive format=raw,file=os-image.bin

run-iso: zero3.iso
	qemu-system-i386 -cdrom zero3.iso

clean:
	rm -rf $(BUILD) os-image.bin zero3.iso

.PHONY: all clean run iso run-iso
