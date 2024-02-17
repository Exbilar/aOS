//
// Created by huuxlion on 2024/1/14.
//

#ifndef AOS_IRQ_H
#define AOS_IRQ_H

#define PIC_M_CTRL 0x20
#define PIC_M_DATA 0x21
#define PIC_S_CTRL 0xa0
#define PIC_S_DATA 0xa1

extern void irq_0();
extern void irq_1();
extern void irq_2();
extern void irq_3();
extern void irq_4();
extern void irq_5();
extern void irq_6();
extern void irq_7();
extern void irq_8();
extern void irq_9();
extern void irq_10();
extern void irq_11();
extern void irq_12();
extern void irq_13();
extern void irq_14();
extern void irq_15();
extern void irq_16();
extern void irq_17();
extern void irq_18();
extern void irq_19();
extern void irq_20();
extern void irq_21();
extern void irq_22();
extern void irq_23();
extern void irq_24();
extern void irq_25();
extern void irq_26();
extern void irq_27();
extern void irq_28();
extern void irq_29();
extern void irq_30();
extern void irq_31();

extern void irq_32();
extern void irq_33();
extern void irq_34();
extern void irq_35();
extern void irq_36();
extern void irq_37();
extern void irq_38();
extern void irq_39();
extern void irq_40();
extern void irq_41();
extern void irq_42();
extern void irq_43();
extern void irq_44();
extern void irq_45();
extern void irq_46();
extern void irq_47();

extern int syscall_entry();

struct regs {
    unsigned int gs, fs, es, ds;
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
    unsigned int int_no, err_code;
    unsigned int eip, cs, eflags, useresp, ss;
};

#define EFLAGS_IF 0x00000200
#define GET_EFLAGS(EFLAG_VAR) asm volatile("pushfl; popl %0" : "=g" (EFLAG_VAR))

enum intr_status {
    INTR_OFF,
    INTR_ON
};

void irq_remap();
void init_intr();
void irq_uninstall(int irq);
void irq_install(int irq, void (*handler)(struct regs *r));
void irq_handler(struct regs *r);

enum intr_status intr_get_status();
enum intr_status intr_set_status(enum intr_status status);
enum intr_status push_off();
enum intr_status pop_off();

#endif //AOS_IRQ_H
