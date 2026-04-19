[bits 32]
section .text
global irq1_stub
global irq12_stub
global idt_load

extern irq1_handler
extern irq12_handler

irq1_stub:
    pusha
    call irq1_handler
    popa
    iretd

irq12_stub:
    pusha
    call irq12_handler
    popa
    iretd

idt_load:
    mov eax, [esp + 4]
    lidt [eax]
    ret
