#ifndef IDT_H
#define IDT_H
#include "types.h"

void idt_init(void);
void irq_ack(u8 irq);

#endif
