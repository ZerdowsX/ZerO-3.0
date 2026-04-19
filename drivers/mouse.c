#include "mouse.h"
#include "io.h"
#include "idt.h"
#include "gui.h"

static u8 packet[3];
static u8 packet_idx = 0;
static int g_mouse_ready = 0;

static int mouse_wait_write(void) {
    for (u32 i = 0; i < 1000000; ++i) {
        if (!(inb(0x64) & 0x02)) return 1;
    }
    return 0;
}

static int mouse_wait_read(void) {
    for (u32 i = 0; i < 1000000; ++i) {
        if (inb(0x64) & 0x01) return 1;
    }
    return 0;
}

static int mouse_write(u8 data) {
    if (!mouse_wait_write()) return 0;
    outb(0x64, 0xD4);
    if (!mouse_wait_write()) return 0;
    outb(0x60, data);
    return 1;
}

static u8 mouse_read(void) {
    if (!mouse_wait_read()) return 0xFF;
    return inb(0x60);
}

static int mouse_expect_ack(void) {
    for (int i = 0; i < 4; ++i) {
        u8 v = mouse_read();
        if (v == 0xFA) return 1;
        if (v == 0xFF) return 0;
    }
    return 0;
}

void mouse_init(void) {
    if (!mouse_wait_write()) return;
    outb(0x64, 0xA8);

    if (!mouse_wait_write()) return;
    outb(0x64, 0x20);
    u8 status = mouse_read() | 0x02;
    if (status == 0xFF) return;

    if (!mouse_wait_write()) return;
    outb(0x64, 0x60);
    if (!mouse_wait_write()) return;
    outb(0x60, status);

    if (!mouse_write(0xF6)) return;
    if (!mouse_expect_ack()) return;
    if (!mouse_write(0xF4)) return;
    if (!mouse_expect_ack()) return;

    g_mouse_ready = 1;
}

int mouse_is_ready(void) {
    return g_mouse_ready;
}

void irq12_handler(void) {
    if (!g_mouse_ready) {
        irq_ack(12);
        return;
    }

    u8 data = inb(0x60);

    if (packet_idx == 0 && !(data & 0x08)) {
        irq_ack(12);
        return;
    }

    packet[packet_idx++] = data;

    if (packet_idx == 3) {
        packet_idx = 0;
        int dx = (packet[0] & 0x10) ? (int)packet[1] - 256 : packet[1];
        int dy = (packet[0] & 0x20) ? (int)packet[2] - 256 : packet[2];
        gui_mouse_move(dx, -dy);
        gui_mouse_click(packet[0] & 0x1);
    }

    irq_ack(12);
}
