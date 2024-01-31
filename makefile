# 定义编译器和工具链
CC := x86_64-elf-gcc
AS := x86_64-elf-as
LD := x86_64-elf-ld

# 定义编译选项和路径
CFLAGS := -m32 -ffreestanding -g -std=gnu99
BOOT_PATH := ./boot
KERNEL_PATH := ./kernel

# 定义目标文件和输出文件
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

OUTPUT := aOS.bin

# 定义目标和依赖关系

all: $(OUTPUT)
	- mkdir -p isodir/boot/grub
	cp aOS.bin isodir/boot/aOS.bin 
	cp grub.cfg isodir/boot/grub/grub.cfg 
	./grub/grub-mkrescue -o aOS.iso isodir

$(OUTPUT): $(OBJ_FILES)
	$(LD) -m elf_i386 -T linker.ld -o $(OUTPUT) $(OBJ_FILES)

boot.o: $(BOOT_PATH)/boot.S
	$(CC) $(CFLAGS) -c $(BOOT_PATH)/boot.S -o boot.o

intr.o: $(BOOT_PATH)/intr.S
	$(CC) $(CFLAGS) -c $(BOOT_PATH)/intr.S -o intr.o

string.o: $(KERNEL_PATH)/string.c
	$(CC) $(CFLAGS) -c $(KERNEL_PATH)/string.c -o string.o

vga.o: $(KERNEL_PATH)/vga.c
	$(CC) $(CFLAGS) -c $(KERNEL_PATH)/vga.c -o vga.o

assert.o: $(KERNEL_PATH)/assert.c
	$(CC) $(CFLAGS) -c $(KERNEL_PATH)/assert.c -o assert.o

kernel_main.o: $(KERNEL_PATH)/kernel_main.c
	$(CC) $(CFLAGS) -c $(KERNEL_PATH)/kernel_main.c -o kernel_main.o

stdio.o: $(KERNEL_PATH)/stdio.c
	$(CC) $(CFLAGS) -c $(KERNEL_PATH)/stdio.c -o stdio.o

kalloc.o: $(KERNEL_PATH)/kalloc.c
	$(CC) $(CFLAGS) -c $(KERNEL_PATH)/kalloc.c -o kalloc.o

gdt.o: $(KERNEL_PATH)/gdt.c
	$(CC) $(CFLAGS) -c $(KERNEL_PATH)/gdt.c -o gdt.o

idt.o: $(KERNEL_PATH)/idt.c
	$(CC) $(CFLAGS) -c $(KERNEL_PATH)/idt.c -o idt.o

irq.o: $(KERNEL_PATH)/irq.c
	$(CC) $(CFLAGS) -c $(KERNEL_PATH)/irq.c -o irq.o

timer.o: $(KERNEL_PATH)/timer.c
	$(CC) $(CFLAGS) -c $(KERNEL_PATH)/timer.c -o timer.o

vm.o: $(KERNEL_PATH)/vm.c
	$(CC) $(CFLAGS) -c $(KERNEL_PATH)/vm.c -o vm.o

thread.o: $(KERNEL_PATH)/thread.c
	$(CC) $(CFLAGS) -c $(KERNEL_PATH)/thread.c -o thread.o

list.o:  $(KERNEL_PATH)/list.c
	$(CC) $(CFLAGS) -c $(KERNEL_PATH)/list.c -o list.o

lock.o: $(KERNEL_PATH)/lock.c
	$(CC) $(CFLAGS) -c $(KERNEL_PATH)/lock.c -o lock.o

kbd.o: $(KERNEL_PATH)/kbd.c
	$(CC) $(CFLAGS) -c $(KERNEL_PATH)/kbd.c -o kbd.o

uproc.o:  $(KERNEL_PATH)/uproc.c
	$(CC) $(CFLAGS) -c $(KERNEL_PATH)/uproc.c -o uproc.o

syscall.o:  $(KERNEL_PATH)/syscall.c
	$(CC) $(CFLAGS) -c $(KERNEL_PATH)/syscall.c -o syscall.o

# 定义伪目标
.PHONY: clean run

clean:
	rm -f $(OBJ_FILES) $(OUTPUT)

run: all
	qemu-system-i386 -m 128 -cdrom aOS.iso

gdb: all
	qemu-system-i386 -m 128 -s -S -cdrom aOS.iso