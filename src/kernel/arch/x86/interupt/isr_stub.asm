extern isr_handler
extern irq_handler

section .text

; existing exception stub
global isr_stub
isr_stub:
    pusha
    call isr_handler
    popa
    iret

; macro to create an IRQ stub
%macro IRQ 2
global irq_stub_%1
irq_stub_%1:
    push byte %2
    call irq_handler
    add esp, 4
    iret
%endmacro

; IRQ stubs 0-15
IRQ  0,  0
IRQ  1,  1
IRQ  2,  2
IRQ  3,  3
IRQ  4,  4
IRQ  5,  5
IRQ  6,  6
IRQ  7,  7
IRQ  8,  8
IRQ  9,  9
IRQ 10, 10
IRQ 11, 11
IRQ 12, 12
IRQ 13, 13
IRQ 14, 14
IRQ 15, 15