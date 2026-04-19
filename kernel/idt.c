#include "idt.h"
#include "io.h"

extern void irq1_stub(void);
extern void irq12_stub(void);
extern void idt_load(void* idtr);

typedef struct {
    u16 offset_low;
    u16 selector;
    u8 zero;
    u8 type_attr;
    u16 offset_high;
} __attribute__((packed)) idt_entry_t;

typedef struct {
    u16 limit;
    u32 base;
} __attribute__((packed)) idtr_t;

static idt_entry_t idt[256];
static idtr_t idtr;

static void idt_set_gate(int idx, u32 handler, u16 sel, u8 flags) {
    idt[idx].offset_low = handler & 0xFFFF;
    idt[idx].selector = sel;
    idt[idx].zero = 0;
    idt[idx].type_attr = flags;
    idt[idx].offset_high = (handler >> 16) & 0xFFFF;
}

static void pic_remap(void) {
    outb(0x20, 0x11); io_wait();
    outb(0xA0, 0x11); io_wait();
    outb(0x21, 0x20); io_wait();
    outb(0xA1, 0x28); io_wait();
    outb(0x21, 0x04); io_wait();
    outb(0xA1, 0x02); io_wait();
    outb(0x21, 0x01); io_wait();
    outb(0xA1, 0x01); io_wait();

    /*
     * Mask IRQ lines we do not handle yet.
     * Master PIC: enable IRQ1 (keyboard) and IRQ2 (cascade to slave).
     * Slave PIC:  enable IRQ12 (mouse).
     */
    outb(0x21, 0xF9);
    outb(0xA1, 0xEF);
}

void irq_ack(u8 irq) {
    if (irq >= 8) outb(0xA0, 0x20);
    outb(0x20, 0x20);
}

void idt_init(void) {
    for (int i = 0; i < 256; ++i) {
        idt_set_gate(i, 0, 0, 0);
    }

    pic_remap();
    idt_set_gate(0x21, (u32)irq1_stub, 0x08, 0x8E);
    idt_set_gate(0x2C, (u32)irq12_stub, 0x08, 0x8E);

    idtr.limit = sizeof(idt) - 1;
    idtr.base = (u32)&idt[0];
    idt_load(&idtr);

    __asm__ volatile ("sti");
}
