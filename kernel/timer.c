#include "timer.h"
#include "idt.h"

static volatile u32 g_ticks = 0;
static volatile u32 g_seconds = 0;

void irq0_handler(void) {
    g_ticks++;
    if (g_ticks >= 18) {
        g_ticks = 0;
        g_seconds++;
    }
    irq_ack(0);
}

u32 timer_seconds(void) {
    return g_seconds;
}
