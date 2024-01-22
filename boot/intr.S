.code32

.macro ZERO
    pushl $0
.endm

.macro ERR_CODE
    nop
.endm

.extern irq_handler

.section .text
.global gdt_flush

// gdt_flush sets the correct value of segment regs. ljmp sets the correct value of CS:IP
// 0x08 denotes the offset of kernel code segment in gdt, whereas 0x10 denotes the data segment
gdt_flush:
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss
    ljmp $0x08, $flush2
flush2:
    ret

.macro VECTOR no, err_code

.section .text

.global irq_\no
irq_\no:
    cli
    \err_code
    pushl $\no

    pusha
    push %ds
    push %es
    push %fs
    push %gs

    // set segment registers to kernel data segment
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    movl %esp, %eax
    pushl %eax
    movl $irq_handler, %eax
    call *%eax
    jmp intr_exit

.endm

.section .text
.global intr_exit

intr_exit:
    popl %eax
    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa
    add $8, %esp
    iretl

VECTOR 0, ZERO
VECTOR 1, ZERO
VECTOR 2, ZERO
VECTOR 3, ZERO
VECTOR 4, ZERO
VECTOR 5, ZERO
VECTOR 6, ZERO
VECTOR 7, ZERO
VECTOR 8, ERR_CODE
VECTOR 9, ZERO
VECTOR 10, ERR_CODE
VECTOR 11, ERR_CODE
VECTOR 12, ERR_CODE
VECTOR 13, ERR_CODE
VECTOR 14, ERR_CODE
VECTOR 15, ZERO
VECTOR 16, ZERO
VECTOR 17, ERR_CODE
VECTOR 18, ZERO
VECTOR 19, ZERO
VECTOR 20, ZERO
VECTOR 21, ZERO
VECTOR 22, ZERO
VECTOR 23, ZERO
VECTOR 24, ZERO
VECTOR 25, ZERO
VECTOR 26, ZERO
VECTOR 27, ZERO
VECTOR 28, ZERO
VECTOR 29, ZERO
VECTOR 30, ERR_CODE
VECTOR 31, ZERO
VECTOR 32, ZERO
VECTOR 33, ZERO
VECTOR 34, ZERO
VECTOR 35, ZERO
VECTOR 36, ZERO
VECTOR 37, ZERO
VECTOR 38, ZERO
VECTOR 39, ZERO
VECTOR 40, ZERO
VECTOR 41, ZERO
VECTOR 42, ZERO
VECTOR 43, ZERO
VECTOR 44, ZERO
VECTOR 45, ZERO
VECTOR 46, ZERO
VECTOR 47, ZERO

.section .text
.global switch_to

switch_to:
    pushl %esi
    pushl %edi
    pushl %ebx
    pushl %ebp
    /********** general protect for callee function *******/
    movl 20(%esp), %eax
    movl %esp, (%eax)                  // *(pthread->thread_stack) <- esp
    /******* protect thread stack environment *******/
    movl 24(%esp), %eax
    movl (%eax), %esp
    popl %ebp
    popl %ebx
    popl %edi
    popl %esi
    ret