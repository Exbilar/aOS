# aOS: A Simple i386 Kernel

### Environment
  * OS: macOS 14.2.1/(Linux?)
  * Compiler: x86_64-elf-gcc, x86_64-elf-ld, x86_64-elf-as
  * Emulator: qemu

### Getting Started

```bash
cd aOS
make all           #compile the project
make run           #compile and run
make clean         #clean all .o files
make gdb           #debug
```

### ToDo List:
  - [x] kernel thread support
  - [ ] user process support
  - [ ] syscall
  - [ ] file system
  - [ ] shell
