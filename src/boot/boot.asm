section .multiboot_header
align 8

header_start:
    dd 0xE85250D6
    dd 0
    dd header_end - header_start
    dd 0x100000000 - (0xE85250D6 + 0 + (header_end - header_start))
    dw 0
    dw 0
    dd 8
header_end:

section .bss
    align 16
stack_bottom:
    resb 16384
stack_top:

section .text
global _start
extern kernel_main

_start:
    cli
    mov esp, 0x90000
    call kernel_main

.hang:
    cli
    hlt
    jmp .hang