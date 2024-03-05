# aOS: A Simple i386 Kernel

### Environment
  * OS: macOS 14.2.1/(Linux?)
  * Compiler: x86_64-elf-gcc, x86_64-elf-ld, x86_64-elf-as
  * Emulator: qemu

### Getting Started

```bash
cd aOS
make all           #compile the project
make img           #create virtual hard disk image hdisk.img
make run           #compile and run
make clean         #clean all .o files
make gdb           #debug
```

NOTICE: hdisk.img is not formatted, so partition_scan() and ide_init() (in kernel/ide.c) may occur error. 
To handle this problem, you can use fdisk to create a partition for hdisk.img

![qemu.gif](docs%2Fqemu.gif)

### ToDo List:
  - [x] kernel thread support
  - [x] user process support
  - [x] syscall
  - [x] disk driver
  - [ ] file system
  - [ ] shell

### Reference:
* [OSDev](https://wiki.osdev.org/)
* [xv6](https://github.com/mit-pdos/xv6-riscv)
* 《操作系统真相还原》(郑纲)
