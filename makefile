# definition of compiler and toolchain settings
CC := x86_64-elf-gcc
AS := x86_64-elf-as
LD := x86_64-elf-ld

CFLAGS := -m32 -ffreestanding -g -std=gnu99
BOOT_PATH := ./boot
KERNEL_PATH := ./kernel

# definition of output files
OBJ_FILES := boot.o \
			 intr.o \
			 kernel_main.o \
			 string.o \
			 vga.o \
			 assert.o \
			 stdio.o \
			 kalloc.o \
			 gdt.o \
			 idt.o \
			 irq.o \
			 timer.o \
			 vm.o \
			 thread.o \
			 list.o \
			 lock.o \
			 kbd.o \
			 uproc.o \
			 syscall.o \
			 ide.o \
			 sleeplock.o \
			 bitmap.o \
			 fs.o \

OUTPUT := aOS.bin

all: $(OUTPUT)
	- mkdir -p isodir/boot/grub
	cp aOS.bin isodir/boot/aOS.bin 
	cp grub.cfg isodir/boot/grub/grub.cfg 
	./grub/grub-mkrescue -o aOS.iso isodir

$(OUTPUT): $(OBJ_FILES)
	$(LD) -m elf_i386 -T linker.ld -o $@ $^

%.o: $(KERNEL_PATH)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: $(BOOT_PATH)/%.S
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean run img

clean:
	rm -f $(OBJ_FILES) $(OUTPUT)

run: all
	qemu-system-i386 -m 128 -cdrom aOS.iso -drive file=hdisk.img,format=raw -boot d

gdb: all
	qemu-system-i386 -m 128 -s -S -cdrom aOS.iso -drive file=hdisk.img,format=raw -boot d

img:
	qemu-img create hdisk.img 128M