[bits 32]
section .text
global irq1_stub
global irq12_stub
global irq0_stub
global idt_load

extern irq0_handler
extern irq1_handler
extern irq12_handler

irq0_stub:
    pusha
    call irq0_handler
    popa
    iretd

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
