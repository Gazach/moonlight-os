extern isr_handler

section .text
global isr_stub

isr_stub:
    pusha
    call isr_handler
    popa
    iret