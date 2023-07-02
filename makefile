# 定义编译器和工具链
CC := x86_64-elf-gcc
AS := x86_64-elf-as
LD := x86_64-elf-ld

# 定义编译选项和路径
CFLAGS := -m32 -ffreestanding -std=gnu99 -Wall
BOOT_PATH := ./boot
KERNEL_PATH := ./kernel

# 定义目标文件和输出文件
OBJ_FILES := boot.o \
			 kernel_main.o \
			 string.o \
			 vga.o

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

string.o: $(KERNEL_PATH)/string.c
	$(CC) $(CFLAGS) -c $(KERNEL_PATH)/string.c -o string.o

vga.o: $(KERNEL_PATH)/vga.c
	$(CC) $(CFLAGS) -c $(KERNEL_PATH)/vga.c -o vga.o

kernel_main.o: $(KERNEL_PATH)/kernel_main.c
	$(CC) $(CFLAGS) -c $(KERNEL_PATH)/kernel_main.c -o kernel_main.o

# 定义伪目标
.PHONY: clean run

clean:
	rm -f $(OBJ_FILES) $(OUTPUT)

run: all
	qemu-system-i386 -cdrom aOS.iso
